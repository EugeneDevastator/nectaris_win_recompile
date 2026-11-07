/*
 * winmm.dll proxy that virtualizes mouse/keyboard input for Nec.exe.
 * Injected by being placed in the game directory (WINMM is not a KnownDLL,
 * so the application directory copy is loaded for the game's static import).
 *
 * It forwards all real winmm exports to winmm_sys.dll (a copy of the system
 * DLL) and hijacks the game's input IAT entries.  A controller process writes
 * the desired input state into a shared memory section; the game sees it as if
 * the real mouse/keyboard had produced it, while the physical cursor never
 * moves.
 *
 * Build:
 *   python gen_def.py
 *   i686-w64-mingw32-gcc -shared -O2 -o winmm.dll proxy.c winmm.def \
 *       -Wl,--enable-stdcall-fixup -static-libgcc
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>

#define FAKE_MAGIC   0x4E454331u   /* 'NEC1' */
#define FAKE_MAPNAME "Local\\NecFakeInput"

typedef struct {
    volatile LONG magic;      /* 0 */
    volatile LONG enabled;    /* 4  1 = fake input active */
    volatile LONG seq;        /* 8  controller bump counter */
    volatile LONG screenX;    /* 12 screen coords returned by GetCursorPos */
    volatile LONG screenY;    /* 16 */
    volatile LONG clientX;    /* 20 informational */
    volatile LONG clientY;    /* 24 */
    volatile LONG buttons;    /* 28 bit0 left, bit1 right, bit2 middle */
    volatile LONG wheel;      /* 32 */
    volatile BYTE keys[256];  /* 36 */
} FakeInput;

static FakeInput *g_fi = NULL;
static HANDLE g_map = NULL;

typedef SHORT (WINAPI *PFN_GetKeyState)(int);
typedef BOOL  (WINAPI *PFN_GetCursorPos)(LPPOINT);
typedef BOOL  (WINAPI *PFN_SetCursorPos)(int, int);

static PFN_GetKeyState  real_GetKeyState  = NULL;
static PFN_GetCursorPos real_GetCursorPos = NULL;
static PFN_SetCursorPos real_SetCursorPos = NULL;

static int patched_key = 0, patched_get = 0, patched_set = 0;

/* diagnostics live at fixed offsets in the shared page */
#define DIAG_BASE   512
static volatile LONG *diag(int slot) { return (volatile LONG*)((BYTE*)g_fi + DIAG_BASE + slot*4); }
#define D_PATCHED 0
#define D_KEYCALL 1
#define D_GETCALL 2
#define D_SETCALL 3
#define D_LASTVK  4
#define D_ALIVE   5

static void ensure_map(void) {
    if (g_fi) return;
    g_map = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, FAKE_MAPNAME);
    if (g_map) {
        g_fi = (FakeInput*)MapViewOfFile(g_map, FILE_MAP_ALL_ACCESS, 0, 0, 4096);
        if (g_fi && g_fi->magic != (LONG)FAKE_MAGIC && g_fi->seq == 0) {
            /* leave it alone; controller initialises it */
        }
    }
}

static int fake_active(void) {
    return g_fi != NULL && g_fi->magic == (LONG)FAKE_MAGIC && g_fi->enabled == 1;
}

static SHORT WINAPI hook_GetKeyState(int vk) {
    ensure_map();
    if (g_fi) {
        InterlockedIncrement(diag(D_KEYCALL));
        *diag(D_LASTVK) = vk;
        if (vk >= 0 && vk < 256) ((volatile BYTE*)g_fi)[1024 + vk] = 1;
    }
    if (fake_active()) {
        int down = 0;
        if (vk == VK_LBUTTON)      down = (g_fi->buttons & 1) != 0;
        else if (vk == VK_RBUTTON) down = (g_fi->buttons & 2) != 0;
        else if (vk == VK_MBUTTON) down = (g_fi->buttons & 4) != 0;
        else if (vk >= 0 && vk < 256) down = g_fi->keys[vk] != 0;
        if (down) return (SHORT)0x8000;
        if (vk == VK_LBUTTON || vk == VK_RBUTTON || vk == VK_MBUTTON) return 0;
    }
    return real_GetKeyState ? real_GetKeyState(vk) : 0;
}

static BOOL WINAPI hook_GetCursorPos(LPPOINT pt) {
    ensure_map();
    if (g_fi) InterlockedIncrement(diag(D_GETCALL));
    if (pt && fake_active()) {
        pt->x = g_fi->screenX;
        pt->y = g_fi->screenY;
        return TRUE;
    }
    return real_GetCursorPos ? real_GetCursorPos(pt) : FALSE;
}

static BOOL WINAPI hook_SetCursorPos(int x, int y) {
    ensure_map();
    if (g_fi) InterlockedIncrement(diag(D_SETCALL));
    if (fake_active()) {
        g_fi->screenX = x;
        g_fi->screenY = y;
        return TRUE;            /* do not touch the real cursor */
    }
    return real_SetCursorPos ? real_SetCursorPos(x, y) : FALSE;
}

static int patch_iat(HMODULE mod, const char *dllname, const char *fname,
                     void *hook, void **realout) {
    BYTE *base = (BYTE*)mod;
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER*)base;
    if (!dos || dos->e_magic != IMAGE_DOS_SIGNATURE) return 0;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE) return 0;
    IMAGE_DATA_DIRECTORY *dir =
        &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (!dir->VirtualAddress) return 0;
    IMAGE_IMPORT_DESCRIPTOR *imp =
        (IMAGE_IMPORT_DESCRIPTOR*)(base + dir->VirtualAddress);

    for (; imp->Name; imp++) {
        const char *dll = (const char*)(base + imp->Name);
        if (_stricmp(dll, dllname) != 0) continue;
        IMAGE_THUNK_DATA *iat = (IMAGE_THUNK_DATA*)(base + imp->FirstThunk);
        IMAGE_THUNK_DATA *int_ = (IMAGE_THUNK_DATA*)(base +
            (imp->OriginalFirstThunk ? imp->OriginalFirstThunk : imp->FirstThunk));
        for (; int_->u1.AddressOfData; int_++, iat++) {
            if (int_->u1.Ordinal & IMAGE_ORDINAL_FLAG) continue;
            IMAGE_IMPORT_BY_NAME *ibn =
                (IMAGE_IMPORT_BY_NAME*)(base + int_->u1.AddressOfData);
            if (strcmp((const char*)ibn->Name, fname) != 0) continue;
            if (realout && !*realout) *realout = (void*)iat->u1.Function;
            DWORD old;
            if (VirtualProtect(&iat->u1.Function, sizeof(void*),
                               PAGE_READWRITE, &old)) {
                iat->u1.Function = (ULONG_PTR)hook;
                VirtualProtect(&iat->u1.Function, sizeof(void*), old, &old);
            }
            return 1;
        }
    }
    return 0;
}

static void install_hooks(void) {
    HMODULE main = GetModuleHandleW(NULL);
    HMODULE u32  = GetModuleHandleA("user32.dll");
    if (!main || !u32) return;

    if (!real_GetKeyState)
        real_GetKeyState = (PFN_GetKeyState)GetProcAddress(u32, "GetKeyState");
    if (!real_GetCursorPos)
        real_GetCursorPos = (PFN_GetCursorPos)GetProcAddress(u32, "GetCursorPos");
    if (!real_SetCursorPos)
        real_SetCursorPos = (PFN_SetCursorPos)GetProcAddress(u32, "SetCursorPos");

    if (!patched_key)
        patched_key = patch_iat(main, "USER32.dll", "GetKeyState",
                                (void*)hook_GetKeyState, (void**)&real_GetKeyState);
    if (!patched_get)
        patched_get = patch_iat(main, "USER32.dll", "GetCursorPos",
                                (void*)hook_GetCursorPos, (void**)&real_GetCursorPos);
    if (!patched_set)
        patched_set = patch_iat(main, "USER32.dll", "SetCursorPos",
                                (void*)hook_SetCursorPos, (void**)&real_SetCursorPos);

    if (g_fi)
        *diag(D_PATCHED) = (patched_key ? 1 : 0) | (patched_get ? 2 : 0) |
                           (patched_set ? 4 : 0);
}

static DWORD WINAPI retry_thread(LPVOID p) {
    (void)p;
    for (int i = 0; i < 40; i++) {
        install_hooks();
        if (patched_key && patched_get && patched_set) break;
        Sleep(250);
    }
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved) {
    (void)reserved;
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinst);
        ensure_map();
        if (g_fi) *diag(D_ALIVE) = 0x4E434431;   /* 'NCD1' */
        install_hooks();
        HANDLE t = CreateThread(NULL, 0, retry_thread, NULL, 0, NULL);
        if (t) CloseHandle(t);
    }
    return TRUE;
}
