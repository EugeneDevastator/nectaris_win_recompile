/* ================= FUN_00401030 @ 00401030 ================= */

void FUN_00401030(void)

{
  return;
}



/* ================= FUN_00401040 @ 00401040 ================= */

void FUN_00401040(void)

{
  return;
}



/* ================= FUN_00401050 @ 00401050 ================= */

void FUN_00401050(void)

{
  return;
}



/* ================= FUN_00401060 @ 00401060 ================= */

void FUN_00401060(void)

{
  return;
}



/* ================= FUN_00401070 @ 00401070 ================= */

void FUN_00401070(void)

{
  return;
}



/* ================= FUN_00401090 @ 00401090 ================= */

void FUN_00401090(void)

{
  return;
}



/* ================= FUN_004010e0 @ 004010e0 ================= */

void FUN_004010e0(void)

{
  int iVar1;
  undefined1 uVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  char cVar6;
  byte bVar7;
  
  DAT_00481fda = DAT_00483276;
  DAT_0048235a = DAT_00483276;
  DAT_00481fdb = DAT_00483277;
  DAT_00482358 = DAT_00483277;
  uVar4 = 0;
  do {
    cVar6 = (char)uVar4;
    if ((DAT_0048235a == DAT_0048327d) && (DAT_0048327e == DAT_00482358)) goto LAB_004012af;
    DAT_00482359 = 0xff;
    DAT_0048235b = (&DAT_00495c70)[(uint)DAT_0048235a + (uint)DAT_00482358 * 0x20] & 0xf;
    DAT_0048235e = (&DAT_00495c70)[(uint)DAT_0048235a + (uint)DAT_00482358 * 0x20] & 0xf0;
    DAT_0048235c = 0;
    do {
      DAT_00481fda = DAT_0048235a;
      DAT_00481fdb = DAT_00482358;
      FUN_004179b0(DAT_0048235c);
      if ((DAT_00481fda < DAT_00495c02) && (DAT_00481fdb < DAT_00495c03)) {
        if ((DAT_00481fda == DAT_0048327d) && (DAT_00481fdb == DAT_0048327e)) {
          DAT_00482359 = DAT_0048235c;
          break;
        }
        iVar1 = (uint)DAT_00481fdb * 0x20 + (uint)DAT_00481fda;
        bVar3 = (&DAT_00495c70)[iVar1];
        DAT_0048234c = (uint)bVar3;
        if (DAT_0048234c != 0) {
          if ((uint)DAT_0048235e == (DAT_0048234c & 0xf0)) {
            if (((uint)DAT_0048235b <= (DAT_0048234c & 0xf)) &&
               ((*(byte *)((int)&DAT_00481a80 + iVar1) & 0x80) == 0)) {
LAB_00401217:
              DAT_00482359 = DAT_0048235c;
              DAT_0048235b = bVar3 & 0xf;
              DAT_0048235e = bVar3 & 0xf0;
            }
          }
          else if ((uint)DAT_0048235e <= (DAT_0048234c & 0xf0)) goto LAB_00401217;
        }
      }
      DAT_0048235c = DAT_0048235c + 1;
    } while (DAT_0048235c != 6);
    DAT_00481fda = DAT_0048235a;
    DAT_00481fdb = DAT_00482358;
    FUN_004179b0(DAT_00482359);
    DAT_0048235a = DAT_00481fda;
    DAT_00482358 = DAT_00481fdb;
    (&DAT_00495f10)[uVar4] = (&DAT_0042a030)[DAT_00482359];
    uVar4 = (uint)(byte)(cVar6 + 1U);
  } while ((byte)(cVar6 + 1U) != 0x20);
  cVar6 = '\0';
LAB_004012af:
  DAT_00495f05 = cVar6;
  if (cVar6 != '\0') {
    bVar3 = 0;
    bVar7 = cVar6 - 1;
    if (bVar7 != 0) {
      do {
        uVar4 = (uint)bVar3;
        uVar5 = (uint)bVar7;
        bVar3 = bVar3 + 1;
        bVar7 = bVar7 - 1;
        uVar2 = (&DAT_00495f10)[uVar4];
        (&DAT_00495f10)[uVar4] = (&DAT_00495f10)[uVar5];
        (&DAT_00495f10)[uVar5] = uVar2;
      } while (bVar3 < bVar7);
    }
  }
  return;
}



/* ================= FUN_004012f0 @ 004012f0 ================= */

void FUN_004012f0(void)

{
  byte bVar1;
  
  DAT_00495f07 = 0;
  if (DAT_00495f05 != '\0') {
    bVar1 = FUN_004048e0(&LAB_004013f0,0x1e);
    if (bVar1 != 0xff) {
      DAT_00495f07 = 1;
      bVar1 = DAT_00495c63 >> 7;
      (&DAT_00482c69)[(uint)DAT_0048327b * 0x18] = 0;
      *(undefined *)(DAT_00482c20 + 0x14) = (&DAT_00495790)[bVar1];
      FUN_0040ccb0(DAT_00482c20,(DAT_0048327d - DAT_00495c05) * '\x02',
                   (DAT_0048327e - DAT_00495c06) * '\x02');
      *(undefined1 *)(DAT_00482c20 + 9) = 0;
      *(ushort *)(DAT_00482334 + 0x16) = (ushort)DAT_0048327d;
      *(ushort *)(DAT_00482334 + 0x18) = (ushort)DAT_0048327e;
      *(ushort *)(DAT_00482334 + 0x10) = (ushort)DAT_0048327b;
      *(ushort *)(DAT_00482334 + 0x12) = *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f;
      *(undefined2 *)(DAT_00482334 + 0x14) = 0;
    }
  }
  return;
}



/* ================= FUN_004015c0 @ 004015c0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_004015c0(void)

{
  _DAT_00495f00 = &DAT_00482050;
  FUN_004015f0();
  return;
}



/* ================= FUN_004015d0 @ 004015d0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_004015d0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  _DAT_00495f00 = &DAT_00495c70;
  puVar2 = (undefined4 *)&DAT_00495c70;
  for (iVar1 = 0xa0; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  FUN_004015f0();
  return;
}



/* ================= FUN_004015f0 @ 004015f0 ================= */

void FUN_004015f0(void)

{
  char cVar1;
  uint uVar2;
  
  DAT_00495efb = 0x80;
  DAT_00495efa = *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x80;
  if (DAT_00495efa != DAT_00495c63) {
    DAT_00495efb = 0x40;
  }
  uVar2 = *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f;
  DAT_00495ef0 = (&DAT_0042b230)[uVar2];
  cVar1 = DAT_00495ef8;
  if (DAT_00495ef8 == '\0') {
    cVar1 = (&DAT_0042b218)[uVar2];
  }
  DAT_00495f06 = cVar1 + 1;
  FUN_00401680(DAT_0048327d,DAT_0048327e,0x80,DAT_00495f30 << 4 | DAT_00495f06 & 0xf);
  return;
}



/* ================= FUN_00401680 @ 00401680 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00401680(byte param_1,byte param_2,byte param_3,byte param_4)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte local_8;
  
  while( true ) {
    bVar3 = param_4 & 0xf;
    local_8 = 0;
    DAT_00482360 = (uint)param_2 << 5 | (uint)param_1;
    DAT_00495ef4 = (byte *)(DAT_00482360 + _DAT_00495f00);
    DAT_00495ef9 = *DAT_00495ef4;
    bVar1 = *(byte *)((int)&DAT_00495820 + DAT_00482360);
    if ((((bVar1 != 0xff) && ((bVar1 & 0x80) != DAT_00495efa)) &&
        ((*(byte *)((int)&DAT_00495ab0 + (uint)(bVar1 & 0x3f)) & 0xe0) == 0)) &&
       (((&DAT_00495af0)[bVar1 & 0x3f] & 0x40) == 0)) {
      return;
    }
    DAT_00495efc = *(byte *)((int)&DAT_00481a80 + DAT_00482360);
    bVar1 = param_3 & 0x80;
    if (bVar1 == 0) {
      if (DAT_00495ef0 < 4) {
        bVar2 = (&DAT_0042b248)[(byte)((DAT_00495efc & 0xf) * '\x04' + DAT_00495ef0)];
        if ((bVar2 & 0x80) != 0) {
          return;
        }
        if (bVar2 == 0x7f) {
          bVar2 = DAT_00495f06 - 1;
        }
      }
      else {
        bVar2 = 1;
      }
      bVar3 = bVar3 - bVar2;
      if ((bVar3 - 1 & 0x80) != 0) {
        param_4 = (param_4 & 0xf0) - 0x10;
        if (param_4 < 0x10) {
          return;
        }
        bVar3 = DAT_00495f06 - bVar2;
        if ((bVar3 - 1 & 0x80) != 0) {
          return;
        }
      }
      local_8 = param_4 & 0xf0;
      param_4 = local_8 | bVar3;
    }
    bVar2 = *DAT_00495ef4 & 0xf0;
    if (local_8 <= bVar2) {
      if (local_8 != bVar2) {
        return;
      }
      if (bVar3 <= (*DAT_00495ef4 & 0xf)) {
        return;
      }
    }
    *DAT_00495ef4 = param_4;
    if ((DAT_00495ef9 == 0) && (DAT_00495c6e != '\0')) {
      DAT_00481e6c = param_1 - DAT_00495c05;
      DAT_00481e6d = param_2 - DAT_00495c06;
      FUN_0041a710();
    }
    if (((bVar1 == 0) && ((DAT_00495efc & DAT_00495efb) != 0)) || ((param_4 & 0xf) < 2)) {
      bVar3 = (param_4 & 0xf0) - 0x10;
      if (bVar3 == 0) {
        return;
      }
      param_4 = DAT_00495f06 | bVar3;
    }
    if (((bVar1 != 0) || ((&DAT_0042a1a8)[(byte)((param_1 & 1) << 3 | param_3)] != '\0')) &&
       ((byte)(param_1 - 1) < DAT_00495c02)) {
      FUN_00401680(param_1 - 1,param_2,5,param_4);
    }
    bVar3 = param_1 & 1;
    if (((((param_1 & 1) == 0) && ((bVar1 != 0 || ((&DAT_0042a1b8)[param_3] != '\0')))) &&
        ((byte)(param_2 - 1) < DAT_00495c03)) && ((byte)(param_1 - 1) < DAT_00495c02)) {
      FUN_00401680(param_1 - 1,param_2 - 1,6,param_4);
    }
    if (((bVar1 != 0) || ((&DAT_0042a1c0)[(byte)(bVar3 << 3 | param_3)] != '\0')) &&
       ((byte)(param_2 - 1) < DAT_00495c03)) {
      FUN_00401680(param_1,param_2 - 1,7,param_4);
    }
    if (((param_1 & 1) == 0) &&
       ((((bVar1 != 0 || ((&DAT_0042a1d0)[param_3] != '\0')) && ((byte)(param_2 - 1) < DAT_00495c03)
         ) && ((byte)(param_1 + 1) < DAT_00495c02)))) {
      FUN_00401680(param_1 + 1,param_2 - 1,0,param_4);
    }
    if (((bVar1 != 0) || ((&DAT_0042a1d8)[(byte)(bVar3 << 3 | param_3)] != '\0')) &&
       ((byte)(param_1 + 1) < DAT_00495c02)) {
      FUN_00401680(param_1 + 1,param_2,1,param_4);
    }
    if ((((param_1 & 1) != 0) && ((bVar1 != 0 || ((&DAT_0042a1e8)[param_3] != '\0')))) &&
       (((byte)(param_2 + 1) < DAT_00495c03 && ((byte)(param_1 + 1) < DAT_00495c02)))) {
      FUN_00401680(param_1 + 1,param_2 + 1,2,param_4);
    }
    if (((bVar1 != 0) || ((&DAT_0042a1f0)[(byte)(bVar3 << 3 | param_3)] != '\0')) &&
       ((byte)(param_2 + 1) < DAT_00495c03)) {
      FUN_00401680(param_1,param_2 + 1,3,param_4);
    }
    if ((param_1 & 1) == 0) break;
    if ((bVar1 == 0) && ((&DAT_0042a200)[param_3] == '\0')) {
      return;
    }
    param_2 = param_2 + 1;
    if (DAT_00495c03 <= param_2) {
      return;
    }
    param_1 = param_1 - 1;
    if (DAT_00495c02 <= param_1) {
      return;
    }
    param_3 = 4;
  }
  return;
}



/* ================= FUN_00401a40 @ 00401a40 ================= */

void __cdecl
FUN_00401a40(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6)

{
  undefined4 uVar1;
  undefined4 unaff_retaddr;
  
  uVar1 = FUN_00412890(1);
  FUN_00401ac0(CONCAT31((int3)((uint)uVar1 >> 8),(undefined1)param_1),unaff_retaddr,param_1,param_2,
               param_3,param_4,param_5,param_6);
  return;
}



/* ================= FUN_00401a60 @ 00401a60 ================= */

void __cdecl FUN_00401a60(undefined4 param_1,undefined4 param_2)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined4 unaff_ESI;
  undefined4 unaff_EDI;
  undefined4 unaff_retaddr;
  
  uVar2 = (undefined2)DAT_00480efc;
  uVar1 = (undefined2)DAT_00480ef8;
  FUN_00401aa0(param_1,unaff_EDI,unaff_ESI,unaff_retaddr,param_1,param_2);
  FUN_00412a80(CONCAT22(uVar1,uVar2));
  return;
}



/* ================= FUN_00401aa0 @ 00401aa0 ================= */

void __cdecl
FUN_00401aa0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6)

{
  undefined4 extraout_ECX;
  undefined4 unaff_retaddr;
  
  FUN_00412890(0xd);
  DAT_00495c6c = 1;
  FUN_00401ac0(CONCAT31((int3)((uint)extraout_ECX >> 8),(undefined1)param_1),unaff_retaddr,param_1,
               param_2,param_3,param_4,param_5,param_6);
  return;
}



/* ================= FUN_00401ac0 @ 00401ac0 ================= */

void __cdecl
FUN_00401ac0(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  char cVar1;
  
  FUN_0041d5e0();
  FUN_00412e90();
  thunk_FUN_00412060();
  DAT_00495b74 = 0x3800;
  FUN_00406830((&DAT_0042a208)[param_1 & 0xff],2,0x19);
  do {
    thunk_FUN_00412060();
  } while (DAT_004834ac != '\0');
  cVar1 = '-';
  do {
    thunk_FUN_00412060();
    if (DAT_004834ac != '\0') break;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_00406d70();
  FUN_0041d220(param_2,param_3,param_4,param_5,param_6,param_7,param_8);
  return;
}



/* ================= FUN_00401b30 @ 00401b30 ================= */

void FUN_00401b30(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  byte bVar4;
  undefined4 *puVar5;
  int iVar6;
  bool bVar7;
  uint local_8;
  uint local_4;
  
  DAT_00483274 = DAT_0048327b;
  puVar5 = (undefined4 *)&DAT_00495c70;
  for (iVar2 = 0xa0; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  bVar7 = DAT_00495c63 == '\0';
  iVar6 = 0;
  iVar2 = 0;
  do {
    local_8 = (uint)DAT_0048327d;
    local_4 = (uint)DAT_0048327e;
    FUN_00403490(&local_8,(int *)&local_4,iVar2);
    iVar3 = local_4 * 0x20 + local_8;
    bVar1 = *(byte *)((int)&DAT_00481a80 + iVar3) & 0xf;
    if ((((local_8 < DAT_00495c02) && (local_4 < DAT_00495c03)) &&
        ((bVar1 < 2 || ((bVar1 == 6 || ((byte)((-bVar7 & 0xfeU) + 0xd) == bVar1)))))) &&
       (*(char *)((int)&DAT_00495820 + iVar3) == -1)) {
      iVar6 = iVar6 + 1;
      (&DAT_00495c70)[iVar3] = (char)iVar2 + '\x01';
      DAT_00481e6c = (char)local_8 - DAT_00495c05;
      DAT_00481e6d = (char)local_4 - DAT_00495c06;
      FUN_0041a710();
    }
    bVar1 = DAT_0048327b;
    iVar2 = iVar2 + 1;
  } while (iVar2 < 6);
  if (iVar6 == 0) {
    FUN_00401ca0();
    return;
  }
  bVar4 = 0;
  do {
    if ((((&DAT_00495af0)[bVar4] & 0x40) != 0) && (((&DAT_00495af0)[bVar4] & 0x3f) == DAT_0048327b))
    break;
    bVar4 = bVar4 + 1;
  } while (bVar4 < 0x38);
  *(byte *)((int)&DAT_00495ab0 + (uint)bVar4) = DAT_0048327d;
  (&DAT_00495af0)[bVar4] = DAT_0048327e;
  (&DAT_00495af0)[bVar1] = (&DAT_00495af0)[bVar1] & 0x1f;
  FUN_0040a420();
  DAT_0048327b = bVar4;
  FUN_0040a400();
  FUN_0040a0b0();
  DAT_00481a68 = 1;
  DAT_00495c6d = 1;
  DAT_00483270 = FUN_00401cf0;
  return;
}



/* ================= FUN_00401ca0 @ 00401ca0 ================= */

void FUN_00401ca0(void)

{
  uint uVar1;
  
  FUN_0041df00();
  uVar1 = (uint)DAT_00483274;
  DAT_0048327b = DAT_00483274;
  DAT_0048327c = *(byte *)((int)&DAT_004957a0 + uVar1) & 0x1f;
  DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar1) & 0x1f;
  DAT_0048327e = (&DAT_00495af0)[uVar1] & 0x1f;
  FUN_0040a440();
  DAT_00483270 = &LAB_004165b0;
  return;
}



/* ================= FUN_00401cf0 @ 00401cf0 ================= */

void FUN_00401cf0(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined4 unaff_EBX;
  undefined1 local_8;
  
  bVar2 = DAT_00481a72;
  bVar3 = DAT_00481a71;
  if ((DAT_00481de9 & 0x20) == 0) {
    if ((DAT_00481de9 & 0x40) != 0) {
      FUN_0040a0d0();
      bVar3 = DAT_00483274 | 0x40;
      (&DAT_00495af0)[DAT_00483274] = (&DAT_00495af0)[DAT_00483274] | 0x20;
      (&DAT_00495af0)[DAT_0048327b] = bVar3;
      FUN_0040a0f0();
      FUN_0041ae00();
      DAT_00495c6d = 0;
      FUN_00401ca0();
    }
    return;
  }
  bVar1 = (&DAT_00495c70)[(uint)DAT_00481a72 + (uint)DAT_00481a71 * 0x20];
  if (bVar1 - 1 == -1) {
    FUN_00401a60(2,unaff_EBX);
    return;
  }
  FUN_0040a0d0();
  FUN_0040a420();
  DAT_00495f05 = 1;
  local_8 = (undefined1)(bVar1 - 1);
  DAT_00495f10 = local_8;
  FUN_004012f0();
  DAT_0048327d = bVar2;
  DAT_0048327e = bVar3;
  DAT_00483270 = &LAB_00401dc0;
  return;
}



/* ================= FUN_00401ea0 @ 00401ea0 ================= */

void FUN_00401ea0(void)

{
  undefined4 unaff_EBX;
  
  if (DAT_0048327b == -1) {
    FUN_00401a60(7,unaff_EBX);
    FUN_00401f70();
    return;
  }
  if ((DAT_00483278 & 0x40) != 0) {
    FUN_00401a60(0,unaff_EBX);
    FUN_00401f70();
    return;
  }
  if ((&DAT_0042b218)[DAT_0048327c] == '\0') {
    FUN_00401a60(0x10,unaff_EBX);
    FUN_00401f70();
    return;
  }
  DAT_00495c6b = DAT_0048327b;
  DAT_00480074 = DAT_0048327d;
  DAT_0048007c = DAT_0048327e;
  DAT_00480070 = DAT_00495c42;
  DAT_00480078 = DAT_00495f04;
  DAT_00495ef8 = DAT_00495f04;
  if (DAT_00495c42 != '\x01') {
    DAT_00495ef8 = 0;
  }
  DAT_00495c6e = 1;
  DAT_00495f30 = 1;
  FUN_004015d0();
  DAT_00495c6d = 1;
  DAT_00481a68 = 1;
  DAT_00483270 = FUN_00401f90;
  return;
}



/* ================= FUN_00401f70 @ 00401f70 ================= */

void FUN_00401f70(void)

{
  if (DAT_0048327b != -1) {
    FUN_0040a420();
  }
  FUN_0041df00();
  DAT_00483270 = &LAB_004165b0;
  return;
}



/* ================= FUN_00401f90 @ 00401f90 ================= */

void FUN_00401f90(void)

{
  int iVar1;
  undefined4 uVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  undefined4 unaff_ESI;
  uint local_8;
  uint local_4;
  
  if ((DAT_00481de9 & 0x20) == 0) {
    if ((DAT_00481de9 & 0x40) != 0) {
      FUN_0040cb10(0xc);
      FUN_0041df00();
      FUN_0041ae00();
      if (DAT_00495c42 == '\x01') {
        FUN_0040a440();
        DAT_00495bc1 = 1;
      }
      else {
        FUN_0040a420();
      }
      DAT_00483270 = &LAB_004165b0;
      DAT_00495c6d = 0;
    }
    return;
  }
  if ((DAT_00483278 & 0x80) != DAT_00495c63) {
    FUN_00401a60(1,unaff_ESI);
    return;
  }
  DAT_00483276 = DAT_00481a72;
  DAT_00483277 = DAT_00481a71;
  iVar1 = (uint)DAT_00481a71 * 0x20 + (uint)DAT_00481a72;
  if ((&DAT_00495c70)[iVar1] == '\0') {
    uVar2 = 2;
  }
  else {
    bVar5 = (&DAT_0042b288)[DAT_0048327c];
    if (((((bVar5 & 0x90) == 0x80) ||
         (bVar3 = *(byte *)((int)&DAT_00481a80 + iVar1) & 0xf, bVar3 < 0xb)) || (0xd < bVar3)) ||
       (DAT_00495c04 == bVar3)) {
      bVar3 = *(byte *)((int)&DAT_00495820 + iVar1);
      if ((bVar3 == 0xff) || (bVar4 = bVar3 & 0x3f, DAT_0048327b == bVar4)) goto LAB_004020d8;
      if ((bVar3 & 0x80) == DAT_00495c63) {
        if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)bVar4) & 0x1f] & 0x20) != 0) {
          if (((&DAT_00495af0)[DAT_0048327b] & 0x60) == 0x20) {
            uVar2 = 0xc;
          }
          else if (((&DAT_00495af0)[bVar4] & 0x60) == 0x20) {
            uVar2 = 0xb;
          }
          else if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)bVar4) & 0x1f] & 1) == 0) {
            if ((bVar5 & 0x81) == 0x81) goto LAB_004020d8;
            uVar2 = 10;
          }
          else {
            if ((bVar5 & 0x40) == 0) {
LAB_004020d8:
              FUN_0040cb10(0xb);
              FUN_004010e0();
              FUN_0041ae00();
              DAT_00481e6c = DAT_0048327d - DAT_00495c05;
              DAT_00481e6d = DAT_0048327e - DAT_00495c06;
              FUN_0041a710();
              local_8 = (uint)DAT_0048327d;
              local_4 = (uint)DAT_0048327e;
              bVar5 = 0;
              if (DAT_00495f05 != 0) {
                do {
                  bVar3 = bVar5 + 1;
                  FUN_00403490(&local_8,(int *)&local_4,(uint)(byte)(&DAT_00495f10)[bVar5]);
                  DAT_00481e6c = (char)local_8 - DAT_00495c05;
                  DAT_00481e6d = (char)local_4 - DAT_00495c06;
                  FUN_0041a710();
                  bVar5 = bVar3;
                } while (bVar3 < DAT_00495f05);
              }
              FUN_0040a420();
              DAT_00481a68 = 0;
              FUN_004012f0();
              DAT_00483270 = &LAB_00402210;
              return;
            }
            uVar2 = 10;
          }
          goto LAB_004021ab;
        }
      }
      uVar2 = 3;
    }
    else {
      uVar2 = 4;
    }
  }
LAB_004021ab:
  FUN_00401a60(uVar2,unaff_ESI);
  return;
}



/* ================= FUN_00402230 @ 00402230 ================= */

void __cdecl FUN_00402230(undefined4 param_1,undefined4 param_2)

{
  byte *pbVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  byte bVar5;
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined4 unaff_retaddr;
  undefined2 uVar6;
  undefined3 uVar7;
  undefined4 in_stack_fffffff8;
  byte bVar8;
  uint in_stack_fffffffc;
  
  if ((DAT_00481de9 & 0x20) == 0) {
    if ((DAT_00481de9 & 0x40) != 0) {
      FUN_0040a0d0();
      FUN_0040a400();
      FUN_0040a0f0();
      DAT_00483270 = FUN_00401ea0;
    }
    return;
  }
  FUN_0040a0d0();
  bVar2 = DAT_00483277;
  *(undefined1 *)((int)&DAT_00495820 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20) = 0xff;
  iVar3 = (uint)bVar2 * 0x20 + (uint)DAT_00483276;
  pbVar1 = (byte *)((int)&DAT_00495820 + iVar3);
  DAT_00495f04 = ((&DAT_00495c70)[iVar3] & 0xf) - 1;
  bVar5 = *pbVar1;
  if ((bVar5 != 0xff) && (bVar5 = bVar5 & 0x3f, DAT_0048327b != bVar5)) {
    (&DAT_00495af0)[DAT_0048327b] = bVar5 | 0x40;
    (&DAT_00495af0)[bVar5] = (&DAT_00495af0)[bVar5] & 0x1f | 0x20;
    FUN_0040a440();
    FUN_00423a80();
    FUN_0041ae00();
    FUN_00401a40(0xe,unaff_ESI,unaff_EBX,in_stack_fffffff8,in_stack_fffffffc,unaff_retaddr);
    FUN_0041df00();
    DAT_00495c6d = 0;
    DAT_00483270 = (code *)&LAB_004165b0;
    return;
  }
  DAT_0048327d = DAT_00483276;
  uVar4 = (uint)DAT_0048327b;
  *pbVar1 = DAT_0048327b | DAT_00495c63;
  DAT_0048327e = bVar2;
  *(byte *)((int)&DAT_00495ab0 + uVar4) =
       *(byte *)((int)&DAT_00495ab0 + uVar4) & 0xe0 | DAT_0048327d;
  (&DAT_00495af0)[uVar4] = (&DAT_00495af0)[uVar4] & 0xe0 | bVar2;
  FUN_00423a80();
  uVar6 = (undefined2)in_stack_fffffff8;
  FUN_0041ae00();
  FUN_0041df00();
  DAT_00495c6d = 0;
  bVar5 = *(byte *)((int)&DAT_00481a80 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20);
  uVar7 = CONCAT12(bVar5,uVar6);
  bVar5 = bVar5 & 0xf;
  if (DAT_00495c63 == 0) {
    uVar4 = CONCAT22((short)(CONCAT13(0xb,uVar7) >> 0x10),CONCAT11(0xd,(char)uVar6));
  }
  else {
    uVar4 = CONCAT22((short)(CONCAT13(0xd,uVar7) >> 0x10),CONCAT11(0xb,(char)uVar6));
  }
  bVar2 = (&DAT_0042b288)[DAT_0048327c] & 0x90;
  if ((bVar2 == 0x80) &&
     (in_stack_fffffffc = (uint)bVar5, (-(uint)(DAT_00495c63 == 0) & 2) + 8 == in_stack_fffffffc)) {
    DAT_00495c48 = '\x01' - (DAT_00495c63 == 0);
    FUN_00403c00(unaff_ESI,unaff_EBX);
    return;
  }
  bVar8 = (byte)(uVar4 >> 0x18);
  if (((bVar5 == 0xc) || (bVar5 == (byte)(uVar4 >> 8))) && (bVar2 == 0x80)) {
    *(byte *)((int)&DAT_00481a80 + (uint)DAT_00483276 + (uint)DAT_00483277 * 0x20) = bVar8;
    FUN_0040a570();
    DAT_00495c48 = '\x02';
    if (DAT_00495aa1 != '\0') {
      DAT_00495c48 = DAT_00495aa2 == '\0';
    }
    if (DAT_00495c48 != '\0') {
      DAT_00495c48 = DAT_00495c48 + -1;
      FUN_00403c00(unaff_ESI,unaff_EBX);
      return;
    }
    iVar3 = FUN_0040a610();
    if (iVar3 == 0) {
      FUN_00416860(unaff_ESI,unaff_EBX,uVar4,in_stack_fffffffc,unaff_retaddr,param_1,param_2);
      FUN_004025f0();
      FUN_0041d5e0();
      FUN_00403ab0();
      DAT_0048234c = 0x10;
      DAT_00482348 = (uint)DAT_0048327b;
      FUN_0040a460();
      DAT_00483270 = (code *)&LAB_004165b0;
      return;
    }
    FUN_00403c00(unaff_ESI,unaff_EBX);
    return;
  }
  if (((bVar5 != bVar8) && (((&DAT_0042b288)[DAT_0048327c] & 0x20) != 0)) &&
     (((&DAT_00495af0)[DAT_0048327b] & 0x60) == 0x20)) {
    bVar2 = 0;
    do {
      if ((((&DAT_00495af0)[bVar2] & 0x40) != 0) &&
         (((&DAT_00495af0)[bVar2] & 0x3f) == DAT_0048327b)) break;
      bVar2 = bVar2 + 1;
    } while (bVar2 < 0x38);
    if ((*(byte *)((int)&DAT_004957a0 + (uint)bVar2) & 0x40) == 0) {
      DAT_00483270 = FUN_00401b30;
      DAT_00495bc1 = 1;
      return;
    }
  }
  if (bVar5 == bVar8) {
    FUN_004025f0();
    FUN_00401a40(0xf,unaff_ESI,unaff_EBX,uVar4,in_stack_fffffffc,unaff_retaddr);
    DAT_00483270 = (code *)&LAB_004165b0;
    return;
  }
  if (((uVar4 & 0x800000) == 0) || (DAT_00495c42 == '\x01')) {
    if ((((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f] & 0x1c) == 0x1c
        ) && (DAT_00495c42 != '\x01')) {
      FUN_0041d220(unaff_ESI,unaff_EBX,uVar4,in_stack_fffffffc,unaff_retaddr,param_1,param_2);
      DAT_00483270 = FUN_004209e0;
      DAT_00495bc1 = 1;
      return;
    }
  }
  else if (((&DAT_00495af0)[DAT_0048327b] & 0x20) == 0) {
    FUN_0041d220(unaff_ESI,unaff_EBX,uVar4,in_stack_fffffffc,unaff_retaddr,param_1,param_2);
    DAT_00483270 = FUN_004209e0;
    DAT_00495bc1 = 1;
    return;
  }
  FUN_0041d220(unaff_ESI,unaff_EBX,uVar4,in_stack_fffffffc,unaff_retaddr,param_1,param_2);
  FUN_0040a440();
  DAT_00483270 = (code *)&LAB_004165b0;
  DAT_00495bc1 = 1;
  return;
}



/* ================= FUN_004025f0 @ 004025f0 ================= */

void FUN_004025f0(void)

{
  char cVar1;
  uint uVar2;
  byte bVar3;
  uint uVar4;
  
  *(undefined1 *)((int)&DAT_00495820 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20) = 0xff;
  cVar1 = FUN_00419e50();
  uVar2 = (uint)DAT_0048327b;
  if (((&DAT_00495af0)[uVar2] & 0x60) == 0x20) {
    uVar4 = 0;
    do {
      if ((((&DAT_00495af0)[uVar4] & 0x40) != 0) &&
         (((&DAT_00495af0)[uVar4] & 0x3f) == DAT_0048327b)) break;
      bVar3 = (char)uVar4 + 1;
      uVar4 = (uint)bVar3;
    } while (bVar3 < 0x38);
    (&DAT_00495af0)[uVar4] = 0;
    *(byte *)((int)&DAT_004957e0 + uVar4) = *(byte *)((int)&DAT_004957e0 + uVar4) | 7;
    *(byte *)((int)&DAT_004957a0 + uVar4) = *(byte *)((int)&DAT_004957a0 + uVar4) | 0x40;
    *(char *)((int)&DAT_00495ab0 + uVar4) = cVar1 << 5;
  }
  (&DAT_00495af0)[uVar2] = 0;
  *(byte *)((int)&DAT_004957e0 + uVar2) = *(byte *)((int)&DAT_004957e0 + uVar2) | 7;
  *(char *)((int)&DAT_00495ab0 + uVar2) = cVar1 << 5;
  FUN_0040a440();
  return;
}



/* ================= FUN_004029d0 @ 004029d0 ================= */

void FUN_004029d0(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = (uint)DAT_0048327c;
  if ((((&DAT_0042b288)[uVar2] & 0x10) == 0) || (((&DAT_0042b288)[uVar2] & 8) == 0)) {
    DAT_00482392 = '\x02';
    DAT_00482399 = 0;
    if ((&DAT_0042b1a0)[uVar2] == '\0') {
      if ((&DAT_0042b1b8)[uVar2] != '\0') {
        DAT_00482399 = 0x40;
      }
    }
    else if ((&DAT_0042b1b8)[uVar2] != '\0') {
      DAT_00482399 = 0xff;
    }
  }
  else {
    DAT_00482399 = 0;
    if (((&DAT_0042b1a0)[uVar2] == '\0') || (bVar1 = (&DAT_0042b1e8)[uVar2], bVar1 < 2)) {
      bVar1 = (&DAT_0042b200)[uVar2];
      DAT_00482399 = 0x40;
    }
    DAT_00482392 = bVar1 + 1;
  }
  FUN_00402a60();
  if (DAT_0048239e < 0x80) {
    return;
  }
  DAT_00482399 = 0xff;
  FUN_00402a60();
  return;
}



/* ================= FUN_00402a60 @ 00402a60 ================= */

void FUN_00402a60(void)

{
  byte bVar1;
  uint uVar2;
  byte local_5;
  uint local_4;
  
  DAT_0048239e = 0xff;
  DAT_00482398 = 0;
  local_5 = 0x37;
  do {
    uVar2 = (uint)local_5;
    if (((((&DAT_00495af0)[uVar2] & 0xc0) == 0) &&
        ((*(byte *)((int)&DAT_004957a0 + uVar2) & 0x80) != DAT_00495c63)) &&
       ((*(byte *)((int)&DAT_00495ab0 + uVar2) & 0xe0) == 0)) {
      if (DAT_00482399 < 0x80) {
        local_4 = (uint)(*(byte *)((int)&DAT_004957a0 + uVar2) & 0x1f);
        if (((&DAT_0042b288)[local_4] & 0x40) != DAT_00482399) goto LAB_00402c0a;
      }
      if (DAT_00482398 <= *(byte *)((int)&DAT_00481fa0 + uVar2)) {
        DAT_00481fda = *(byte *)((int)&DAT_00495ab0 + uVar2) & 0x1f;
        DAT_00481fdb = (&DAT_00495af0)[uVar2] & 0x1f;
        DAT_00483274 = local_5;
        DAT_00482396 = 0;
        DAT_00482397 = 0;
        DAT_00482393 = 1;
        DAT_00483276 = DAT_00481fda;
        DAT_00483277 = DAT_00481fdb;
        do {
          FUN_004179b0(4);
          DAT_00482395 = 0;
          do {
            DAT_00482394 = DAT_00482393;
            do {
              if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
                bVar1 = (&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20];
                if (bVar1 != 0) {
                  if (DAT_00482396 <= (bVar1 & 0xf0)) {
                    DAT_00482397 = bVar1 & 0xf;
                    DAT_00482396 = bVar1 & 0xf0;
                    DAT_0048239e = DAT_00481fda;
                    DAT_0048239f = DAT_00481fdb;
                  }
                }
              }
              FUN_004179b0(DAT_00482395);
              DAT_00482394 = DAT_00482394 - 1;
            } while (DAT_00482394 != 0);
            DAT_00482395 = DAT_00482395 + 1;
          } while (DAT_00482395 < 6);
          DAT_00482393 = DAT_00482393 + 1;
        } while (DAT_00482393 < DAT_00482392);
        if (DAT_00482396 != 0) {
          DAT_00482398 = *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00483274);
        }
      }
    }
LAB_00402c0a:
    local_5 = local_5 - 1;
    if (0x7f < local_5) {
      return;
    }
  } while( true );
}



/* ================= FUN_00402c20 @ 00402c20 ================= */

void FUN_00402c20(void)

{
  bool bVar1;
  undefined3 extraout_var;
  int iVar2;
  char cVar3;
  char local_1;
  
  DAT_00481fda = DAT_0048327d;
  DAT_00481fdb = DAT_0048327e;
  DAT_0048239e = 0xff;
  cVar3 = '\0';
  FUN_00419810();
  local_1 = '\x03';
  do {
    bVar1 = FUN_00419830();
    if ((CONCAT31(extraout_var,bVar1) != 1) &&
       ((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] != '\0')) {
      iVar2 = FUN_00403200();
      if ((iVar2 != 1) &&
         (*(char *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20) == -1)) {
        DAT_0048239e = DAT_00481fda;
        DAT_0048239f = DAT_00481fdb;
        return;
      }
    }
    cVar3 = cVar3 + -1;
    if (cVar3 == '\0') {
      local_1 = local_1 + -1;
      if (local_1 == '\0') {
        return;
      }
    }
  } while( true );
}



/* ================= FUN_00402cd0 @ 00402cd0 ================= */

undefined4 FUN_00402cd0(void)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = 0x37;
  DAT_00483276 = DAT_0048239e;
  DAT_0048235a = 0xff;
  DAT_0048327d = DAT_0048239e;
  DAT_00483277 = DAT_0048239f;
  DAT_0048327e = DAT_0048239f;
  do {
    if ((((((&DAT_00495af0)[uVar3] & 0xe0) == 0) &&
         (bVar1 = *(byte *)((int)&DAT_004957a0 + uVar3), (bVar1 & 0x40) == 0)) &&
        ((bVar1 & 0x80) == DAT_00495c63)) &&
       ((*(char *)((int)&DAT_00481fa0 + uVar3) == '\0' &&
        (((&DAT_0042b288)[bVar1 & 0x1f] & 0x20) != 0)))) {
      bVar1 = FUN_004179f0(*(byte *)((int)&DAT_00495ab0 + uVar3) & 0x1f,
                           (&DAT_00495af0)[uVar3] & 0x1f);
      if (bVar1 < DAT_0048235a) {
        DAT_00482358 = (char)uVar3;
        DAT_0048235a = bVar1;
      }
    }
    bVar1 = (char)uVar3 - 1;
    uVar3 = (uint)bVar1;
  } while (bVar1 < 0x80);
  if (DAT_0048235a < 0x80) {
    DAT_00482390 = DAT_00482358;
    DAT_0048327b = DAT_00482358;
    FUN_00417f20();
    if ((&DAT_00495c70)[(uint)DAT_0048239e + (uint)DAT_0048239f * 0x20] != '\0') {
      DAT_00482391 = 0xff;
      iVar2 = FUN_00402dd0();
      if (iVar2 == 0) {
        return 0;
      }
    }
  }
  DAT_00484710 = 0xff;
  return 1;
}



/* ================= FUN_00402dd0 @ 00402dd0 ================= */

undefined4 FUN_00402dd0(void)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  char cVar4;
  byte bVar5;
  byte local_1;
  
  DAT_0048327b = DAT_00482390;
  DAT_00483276 = DAT_0048239e;
  DAT_00483277 = DAT_0048239f;
  FUN_00419620();
  DAT_0048239e = DAT_00481fda;
  DAT_0048239f = DAT_00481fdb;
  if ((((DAT_00482391 < 0x80) && (DAT_00481fda == DAT_00483276)) && (DAT_00481fdb == DAT_00483277))
     && (DAT_00495f05 != 0)) {
    cVar4 = DAT_00495f05 - 1;
    if (cVar4 == '\0') {
      DAT_0048239e = DAT_0048327d;
      DAT_0048239f = DAT_0048327e;
    }
    else {
      DAT_00481fda = DAT_0048327d;
      local_1 = 0;
      DAT_00481fdb = DAT_0048327e;
      do {
        uVar3 = (uint)local_1;
        local_1 = local_1 + 1;
        FUN_004179b0((&DAT_00495f10)[uVar3]);
        cVar4 = cVar4 + -1;
      } while (cVar4 != '\0');
      DAT_0048239e = DAT_00481fda;
      DAT_0048239f = DAT_00481fdb;
    }
  }
  DAT_00481fdd = 0;
  bVar1 = FUN_0040cb50();
  DAT_00481f99 = 10;
  if ((bVar1 & 3) == 0) {
    DAT_00481f99 = 0;
  }
  DAT_00481dbc = 1;
  iVar2 = FUN_004198b0();
  if (iVar2 != 0) {
    DAT_00483276 = DAT_0048239e;
    bVar5 = 0;
    DAT_00483277 = DAT_0048239f;
    FUN_004010e0();
    DAT_00481fda = DAT_0048327d;
    DAT_00481fdb = DAT_0048327e;
    bVar1 = DAT_00495f05;
    if ((2 < DAT_00495f05) && (bVar1 = DAT_00495f05, DAT_00495f05 != 3)) {
      local_1 = DAT_00495f05 - 3;
      bVar1 = bVar5;
      do {
        bVar5 = bVar1 + 1;
        FUN_004179b0((&DAT_00495f10)[bVar1]);
        local_1 = local_1 + -1;
        bVar1 = bVar5;
      } while (local_1 != '\0');
      bVar1 = 3;
    }
    DAT_00482394 = bVar1;
    DAT_00482397 = 0;
    do {
      FUN_004179b0((&DAT_00495f10)[bVar5]);
      iVar2 = FUN_004198b0();
      if (iVar2 != 1) {
        DAT_0048239e = DAT_00481fda;
        DAT_0048239f = DAT_00481fdb;
        DAT_00482397 = DAT_00482397 + 1;
      }
      DAT_00482394 = DAT_00482394 - 1;
      bVar5 = bVar5 + 1;
    } while (DAT_00482394 != 0);
    if (DAT_00482397 == 0) {
      DAT_00482393 = 1;
      DAT_00482397 = 0;
      DAT_00481fda = DAT_0048239e;
      DAT_00481fdb = DAT_0048239f;
      do {
        FUN_004179b0(4);
        DAT_00482395 = 0;
        do {
          DAT_00482394 = DAT_00482393;
          do {
            if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
              iVar2 = FUN_004198b0();
              if (iVar2 != 1) {
                if (DAT_00482397 <=
                    ((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] & 0xf)) {
                  DAT_0048239e = DAT_00481fda;
                  DAT_0048239f = DAT_00481fdb;
                  DAT_00482397 = (&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] &
                                 0xf;
                }
              }
            }
            FUN_004179b0(DAT_00482395);
            DAT_00482394 = DAT_00482394 - 1;
          } while (DAT_00482394 != 0);
          DAT_00482395 = DAT_00482395 + 1;
        } while (DAT_00482395 < 6);
        if (DAT_00482397 != 0) goto LAB_004030f5;
        DAT_00482393 = DAT_00482393 + 1;
      } while (DAT_00482393 < 4);
      DAT_00483276 = DAT_0048239e;
      DAT_00483277 = DAT_0048239f;
      FUN_004010e0();
      uVar3 = 0;
      DAT_00481fda = DAT_0048327d;
      DAT_0048239e = 0xff;
      DAT_00481fdb = DAT_0048327e;
      do {
        FUN_004179b0((&DAT_00495f10)[uVar3]);
        iVar2 = FUN_004198b0();
        if (iVar2 != 1) {
          DAT_0048239e = DAT_00481fda;
          DAT_0048239f = DAT_00481fdb;
        }
        bVar1 = (char)uVar3 + 1;
        uVar3 = (uint)bVar1;
      } while (bVar1 < DAT_00495f05);
      if (0x7f < DAT_0048239e) {
        return 1;
      }
    }
  }
LAB_004030f5:
  if (DAT_00482391 < 0x80) {
    DAT_0048327b = DAT_00482390;
    DAT_00481fdd = 0;
    DAT_00481fda = DAT_0048239e;
    DAT_00481fdb = DAT_0048239f;
    iVar2 = FUN_00419ae0();
    if (iVar2 != 1) {
      DAT_00484710 = 0;
      DAT_00484711 = DAT_0048239e;
      DAT_00484712 = DAT_0048239f;
      DAT_00484714._0_1_ = DAT_0048235a;
      DAT_00484713 = 10;
      DAT_00484714._2_1_ = 0xff;
      DAT_00484714._1_1_ = DAT_00482358;
      uVar3 = (uint)DAT_00482390;
      *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482391) = *(byte *)((int)&DAT_00481fa0 + uVar3) | 8
      ;
      *(byte *)((int)&DAT_00481fa0 + uVar3) = 8;
      return 0;
    }
  }
  if ((DAT_0048327d == DAT_0048239e) && (DAT_0048327e == DAT_0048239f)) {
    return 1;
  }
  DAT_00484711 = DAT_0048239e;
  DAT_00484712 = DAT_0048239f;
  DAT_00484710 = 0;
  DAT_00484713 = 0xff;
  *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482390) =
       *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482390) | 8;
  return 0;
}



/* ================= FUN_00403200 @ 00403200 ================= */

undefined4 FUN_00403200(void)

{
  byte bVar1;
  byte bVar2;
  undefined1 uVar3;
  byte bVar4;
  uint uVar5;
  uint uVar6;
  byte local_1;
  
  bVar2 = DAT_00481fdb;
  bVar1 = DAT_00481fda;
  DAT_0048235a = DAT_00481fda;
  DAT_00482358 = DAT_00481fdb;
  DAT_00482348 = (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20;
  uVar3 = DAT_0048235e;
  if ((*(byte *)((int)&DAT_00495820 + DAT_00482348) == 0xff) ||
     ((&DAT_0042b218)
      [*(byte *)((int)&DAT_004957a0 + (uint)(*(byte *)((int)&DAT_00495820 + DAT_00482348) & 0x3f)) &
       0x1f] != '\0')) {
    bVar4 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf;
    if ((bVar4 < 2) || (bVar4 == 6)) {
      local_1 = 2;
      DAT_00482368 = '\0';
      DAT_0048235d = '\0';
      do {
        uVar6 = (uint)local_1;
        uVar5 = FUN_004033c0(*(byte *)(uVar6 + 0x42a21c));
        if ((byte)uVar5 == 0) {
          uVar5 = FUN_004033c0(*(byte *)(uVar6 + 0x42a220));
          if ((char)uVar5 == '\0') {
            DAT_0048235d = DAT_0048235d + '\x01';
          }
        }
        else if (0x7f < (byte)uVar5) {
          uVar5 = FUN_004033c0(*(byte *)(uVar6 + 0x42a220));
          if (((byte)uVar5 != 0) && (0x7f < (byte)uVar5)) {
            DAT_00482368 = DAT_00482368 + '\x01';
          }
        }
        local_1 = local_1 - 1;
      } while (local_1 < 0x80);
      uVar3 = DAT_0048235e;
      if ((DAT_00482368 != '\0') && (DAT_0048235d != '\0')) {
        DAT_00481fda = DAT_0048235a;
        DAT_00481fdb = DAT_00482358;
        FUN_00419810();
        DAT_0048235e = 'Z';
        DAT_0048235b = '\0';
        do {
          FUN_00419830();
          bVar4 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20);
          if ((bVar4 != 0xff) && ((bVar4 & 0x80) != DAT_00495c63)) {
            DAT_0048235b = DAT_0048235b + '\x01';
          }
          DAT_0048235e = DAT_0048235e + -1;
        } while (DAT_0048235e != '\0');
        uVar3 = 0;
        if (DAT_0048235b != '\0') {
          DAT_00481fdb = bVar2;
          DAT_00481fda = bVar1;
          return 0;
        }
      }
    }
  }
  DAT_0048235e = uVar3;
  DAT_00481fdb = bVar2;
  DAT_00481fda = bVar1;
  return 1;
}



/* ================= FUN_004033c0 @ 004033c0 ================= */

uint __cdecl FUN_004033c0(byte param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  DAT_00481fda = DAT_0048235a;
  DAT_00481fdb = DAT_00482358;
  uVar1 = FUN_00403420(param_1);
  if ((byte)uVar1 == 0) {
    return 0;
  }
  if ((byte)uVar1 < 0x80) {
    iVar2 = FUN_00403420(param_1);
    return (uint)(iVar2 != 0);
  }
  iVar2 = FUN_00403420(param_1);
  return (iVar2 == 0) - 1;
}



/* ================= FUN_00403420 @ 00403420 ================= */

undefined4 __cdecl FUN_00403420(byte param_1)

{
  byte bVar1;
  
  FUN_004179b0(param_1);
  if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
    bVar1 = *(byte *)((int)&DAT_00481a80 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20) & 0xf;
    if ((bVar1 < 2) || (bVar1 == 6)) {
      return 0xffffffff;
    }
    if ((bVar1 == 5) || (bVar1 == 4)) {
      return 0;
    }
  }
  return 1;
}



/* ================= FUN_00403490 @ 00403490 ================= */

void __cdecl FUN_00403490(uint *param_1,int *param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = *param_1;
  iVar2 = *param_2;
  *param_1 = (int)(char)(&DAT_0042a238)[param_3] + uVar1;
  *param_2 = (char)(&DAT_0042a228)[(uVar1 & 1) * 6 + param_3] + iVar2;
  return;
}



/* ================= FUN_00403870 @ 00403870 ================= */

void FUN_00403870(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  DAT_004834af = 0;
  FUN_00403bf0();
  FUN_00414ce0();
  FUN_004038b0(param_1,param_2,param_3,param_4);
  return;
}



/* ================= FUN_00403890 @ 00403890 ================= */

void FUN_00403890(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  DAT_004834af = 0;
  FUN_00403bd0();
  FUN_00414d00();
  FUN_004038b0(param_1,param_2,param_3,param_4);
  return;
}



/* ================= FUN_004038b0 @ 004038b0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl
FUN_004038b0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  DAT_00495c63 = 0;
  DAT_00495c68 = 1;
  _DAT_004834b0 = 0x4038d0;
  if (DAT_00495c53 == '\0') {
    return;
  }
  FUN_004038e0(param_1,param_2,param_3,param_4);
  return;
}



/* ================= FUN_004038e0 @ 004038e0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl
FUN_004038e0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 unaff_retaddr;
  
  FUN_00403bd0();
  DAT_00481a71 = 0;
  DAT_00481a72 = 0;
  DAT_00495c68 = 0;
  if (DAT_0042a4e0 == '\0') {
    DAT_00495c63 = 0;
    DAT_00495c51 = 0;
  }
  FUN_0040cbd0();
  FUN_00405ad0(unaff_retaddr,param_1,param_2,param_3,param_4);
  FUN_0041c2b0();
  _DAT_004834b0 = &LAB_00403930;
  return;
}



/* ================= FUN_00403a50 @ 00403a50 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00403a50(void)

{
  DAT_00495c56 = 0;
  DAT_004834af = 0;
  FUN_00421810();
  _DAT_004834b0 = &LAB_00403a70;
  return;
}



/* ================= FUN_00403a90 @ 00403a90 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00403a90(void)

{
  DAT_004834a1 = 0;
  FUN_00417220();
  _DAT_004834b0 = &LAB_00403ad0;
  return;
}



/* ================= FUN_00403ab0 @ 00403ab0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00403ab0(void)

{
  DAT_004834a1 = 0;
  FUN_0041a8f0();
  _DAT_004834b0 = &LAB_00403ad0;
  return;
}



/* ================= FUN_00403b00 @ 00403b00 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00403b00(void)

{
  DAT_004834a1 = 0;
  DAT_004834af = 0;
  FUN_00404880();
  FUN_00415550();
  _DAT_004834b0 = &LAB_00403b30;
  return;
}



/* ================= FUN_00403b60 @ 00403b60 ================= */

void FUN_00403b60(void)

{
  DAT_004834c4 = 0;
  DAT_00495c57 = 0;
  DAT_00495c56 = 0;
  DAT_00495c68 = 0;
  DAT_00495c50 = 0;
  DAT_004834af = 0;
  DAT_00483491 = 0;
  DAT_004834a1 = 0;
  DAT_0048349f = 0;
  DAT_00495c52 = 0;
  DAT_00481da4 = 0x70;
  DAT_00481da5 = 0;
  FUN_0041dee0();
  DAT_00481e32 = 0;
  FUN_0041be10();
  FUN_00403bd0();
  return;
}



/* ================= FUN_00403bd0 @ 00403bd0 ================= */

void FUN_00403bd0(void)

{
  FUN_00412a20();
  FUN_0040db70();
  FUN_00403be0();
  return;
}



/* ================= FUN_00403be0 @ 00403be0 ================= */

void FUN_00403be0(void)

{
  FUN_00412cf0();
  FUN_00403bf0();
  return;
}



/* ================= FUN_00403bf0 @ 00403bf0 ================= */

void FUN_00403bf0(void)

{
  FUN_00404880();
  FUN_0041bf20();
  return;
}



/* ================= FUN_00403c00 @ 00403c00 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00403c00(undefined4 param_1,undefined4 param_2)

{
  undefined4 unaff_EBX;
  undefined4 unaff_retaddr;
  
  DAT_00495c68 = 1;
  DAT_004834a1 = 0;
  DAT_00495c63 = 0;
  FUN_00403bf0();
  if ((((DAT_00495c69 == '\0') && (DAT_00495c48 < 0x80)) && (DAT_00495c51 < 0x32)) &&
     (((&DAT_004834a8)[DAT_00495c48] != '\0' && (DAT_004834a9 != '\0')))) {
    FUN_00403d70(unaff_EBX,unaff_retaddr,param_1,param_2);
    return;
  }
  FUN_0041b040();
  _DAT_004834b0 = FUN_00403c70;
  FUN_00403c70(unaff_EBX,unaff_retaddr,param_1,param_2);
  return;
}



/* ================= FUN_00403c70 @ 00403c70 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl
FUN_00403c70(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  if (DAT_00495c53 == '\0') {
    return;
  }
  if (DAT_00495c50 == '\0') {
    if (DAT_00495c69 != '\0') {
      thunk_FUN_00412a30();
      return;
    }
    if (0x7f < DAT_00495c48) {
      FUN_00403d70(param_1,param_2,param_3,param_4);
      return;
    }
    if (0x31 < DAT_00495c51) {
      FUN_00403d70(param_1,param_2,param_3,param_4);
      return;
    }
  }
  FUN_00403be0();
  if (DAT_004834a9 == '\0') {
    thunk_FUN_00412a30();
    return;
  }
  if (DAT_0042fb74 != '\0') {
    thunk_FUN_00412a30();
    return;
  }
  DAT_00495c52 = DAT_00495c52 + 1;
  if (DAT_00495c50 != '\0') {
    if (DAT_0048349f != '\0') {
      thunk_FUN_00412a30();
      return;
    }
    if (0xf < DAT_00495c52) {
      DAT_00495c52 = 0;
      DAT_00495c6a = DAT_00495c6a ^ 1;
    }
  }
  if (DAT_00495c52 < 0x10) {
    FUN_00403890(param_1,param_2,param_3,param_4);
    return;
  }
  _DAT_004834b0 = &LAB_00403d40;
  FUN_00413120();
  return;
}



/* ================= FUN_00403d70 @ 00403d70 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl
FUN_00403d70(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  _DAT_004834b0 = 0x403d90;
  FUN_00403bf0();
  FUN_0040b5b0();
  if (DAT_00495c53 == '\0') {
    return;
  }
  FUN_00403870(param_1,param_2,param_3,param_4);
  return;
}



/* ================= thunk_FUN_00412a30 @ 00403da0 ================= */

void thunk_FUN_00412a30(void)

{
  FUN_00412a30();
  return;
}



/* ================= FUN_00403db0 @ 00403db0 ================= */

undefined4 __cdecl FUN_00403db0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 unaff_retaddr;
  
  if ((DAT_004834a1 == '\0') || (DAT_00495c56 == '\0')) {
    return 0;
  }
  iVar1 = FUN_004108d0();
  if (iVar1 == 0x8003) {
    DAT_004834a1 = 0;
    if (DAT_004834a9 == '\0') {
      FUN_00403870(unaff_retaddr,param_1,param_2,param_3);
      return 1;
    }
    FUN_00403d70(unaff_retaddr,param_1,param_2,param_3);
    return 1;
  }
  if (iVar1 == 0x8004) {
    FUN_00404600();
    return 1;
  }
  return 1;
}



/* ================= FUN_00403e10 @ 00403e10 ================= */

void __cdecl FUN_00403e10(byte *param_1)

{
  uchar uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uchar *puVar5;
  uchar *puVar6;
  uchar *puVar7;
  uchar local_204 [260];
  uchar local_100 [256];
  
  FUN_004254b0(param_1,local_204,local_100,(uchar *)0x0,(uchar *)0x0);
  __strupr((char *)local_204);
  uVar2 = 0xffffffff;
  puVar5 = local_100;
  do {
    puVar7 = puVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    puVar7 = puVar5 + 1;
    uVar1 = *puVar5;
    puVar5 = puVar7;
  } while (uVar1 != '\0');
  uVar2 = ~uVar2;
  iVar3 = -1;
  puVar5 = local_204;
  do {
    puVar6 = puVar5;
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    puVar6 = puVar5 + 1;
    uVar1 = *puVar5;
    puVar5 = puVar6;
  } while (uVar1 != '\0');
  puVar5 = puVar7 + -uVar2;
  puVar7 = puVar6 + -1;
  for (uVar4 = uVar2 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)puVar7 = *(undefined4 *)puVar5;
    puVar5 = puVar5 + 4;
    puVar7 = puVar7 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *puVar7 = *puVar5;
    puVar5 = puVar5 + 1;
    puVar7 = puVar7 + 1;
  }
  iVar3 = lstrlenA((LPCSTR)local_204);
  if (3 < iVar3) {
    local_204[iVar3 + -1] = '\0';
  }
  FUN_0041c7a0(s_Settings_0042a730,s_LastDir_0042a73c,local_204);
  return;
}



/* ================= FUN_00403eb0 @ 00403eb0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00403eb0(void)

{
  byte bVar1;
  undefined1 uVar2;
  BOOL BVar3;
  HFILE hFile;
  uint uVar4;
  undefined1 uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  undefined4 unaff_EBX;
  undefined4 unaff_EBP;
  undefined4 unaff_ESI;
  char *pcVar9;
  undefined4 unaff_EDI;
  char *pcVar10;
  tagOFNA *ptVar11;
  byte *pbVar12;
  ushort *puVar13;
  undefined4 *puVar14;
  int iVar15;
  LPCSTR lpText;
  byte local_4f3;
  byte local_4f2;
  byte local_4f1;
  uint local_4f0;
  int local_4ec;
  int local_4e8;
  uint local_4e4;
  char local_4e0 [16];
  tagOFNA local_4d0 [5];
  char local_300 [65];
  undefined4 local_2bf;
  byte local_200 [256];
  CHAR local_100 [256];
  
  pcVar9 = (char *)&DAT_0042a7f8;
  pcVar10 = local_300;
  for (iVar6 = 0x10; iVar6 != 0; iVar6 = iVar6 + -1) {
    *(undefined4 *)pcVar10 = *(undefined4 *)pcVar9;
    pcVar9 = pcVar9 + 4;
    pcVar10 = pcVar10 + 4;
  }
  *pcVar10 = *pcVar9;
  puVar14 = &local_2bf;
  for (iVar6 = 0x2f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar14 = 0;
    puVar14 = puVar14 + 1;
  }
  *(undefined2 *)puVar14 = 0;
  *(undefined1 *)((int)puVar14 + 2) = 0;
  DAT_0042fb74 = '\0';
  DAT_0042a4e0 = '\0';
  local_4e8 = 0;
  local_4ec = 0;
  local_200[0] = 0;
  iVar6 = 0;
  while (local_300[0] != '\0') {
    if (local_300[iVar6] == '/') {
      local_300[iVar6] = '\0';
    }
    local_300[0] = local_300[iVar6 + 1];
    iVar6 = iVar6 + 1;
  }
  ptVar11 = local_4d0;
  for (iVar6 = 0x13; iVar6 != 0; iVar6 = iVar6 + -1) {
    ptVar11->lStructSize = 0;
    ptVar11 = (tagOFNA *)&ptVar11->hwndOwner;
  }
  local_4d0[0].lpstrFilter = local_300;
  local_4d0[0].lpstrFile = (LPSTR)local_200;
  local_4d0[0].hwndOwner = DAT_00480e28;
  local_4d0[0].lpstrFileTitle = local_100;
  local_4d0[0].nMaxFile = 0x100;
  local_4d0[0].nMaxFileTitle = 0x100;
  local_4d0[0].lStructSize = 0x4c;
  local_4d0[0].Flags = 0x1804;
  BVar3 = GetOpenFileNameA(local_4d0);
  if (BVar3 == 0) goto LAB_004045d2;
  FUN_00403e10((byte *)local_4d0[0].lpstrFile);
  hFile = _lopen(local_4d0[0].lpstrFile,0);
  if (hFile == -1) goto LAB_004045d2;
  _hread(hFile,local_4e0,0x10);
  iVar6 = _strncmp(local_4e0,s_NEC_MAP_V1_00_0042a4e8,0x10);
  if (iVar6 == 0) {
    lstrcpyA(&DAT_004814e0,s_USRMAP_0042a7f0);
    _hread(hFile,&local_4f1,1);
    _hread(hFile,&local_4f2,1);
    puVar14 = (undefined4 *)&DAT_004808a8;
    for (iVar6 = 0x160; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar14 = 0;
      puVar14 = puVar14 + 1;
    }
    local_4f0 = 0;
    if (local_4f2 != 0) {
      local_4d0[0].pvReserved = &DAT_004808a8;
      do {
        iVar6 = 0;
        if (local_4f1 != 0) {
          puVar13 = local_4d0[0].pvReserved;
          do {
            iVar6 = iVar6 + 1;
            _hread(hFile,puVar13,1);
            puVar13 = puVar13 + 1;
          } while (iVar6 < (int)(uint)local_4f1);
        }
        local_4f0 = local_4f0 + 1;
        local_4d0[0].pvReserved = (void *)((int)local_4d0[0].pvReserved + 0x40);
      } while ((int)local_4f0 < (int)(uint)local_4f2);
    }
    pbVar12 = &DAT_00481430;
    iVar6 = 0;
    do {
      _hread(hFile,pbVar12,1);
      if (*pbVar12 == 0xff) break;
      _hread(hFile,pbVar12 + 1,2);
      if ((*pbVar12 & 0x80) == 0) {
        local_4ec = local_4ec + 1;
      }
      else {
        local_4e8 = local_4e8 + 1;
      }
      iVar6 = iVar6 + 1;
      pbVar12 = pbVar12 + 3;
    } while (iVar6 < 0x38);
    _lclose(hFile);
    DAT_0042fb74 = '\x01';
LAB_004042ff:
    DAT_00495c02 = (-(local_4f1 == 0x10) & 0xf1U) + 0x1e;
    DAT_00495c03 = (-(local_4f2 == 0xb) & 0xf6U) + 0x14;
    if (DAT_0042fb74 == '\0') {
LAB_00404562:
      DAT_00495c69 = 0;
      DAT_00495c6c = 0;
      DAT_004834a1 = 0;
      DAT_00495c57 = 0;
      DAT_0048349f = 0;
      FUN_004038e0(unaff_EBP,unaff_EDI,unaff_ESI,unaff_EBX);
      FUN_00412a00();
      wsprintfA((LPSTR)&local_4d0[0].pvReserved,s_Nectaris____s_0042a744,local_4d0[0].lpstrFile);
      SetWindowTextA(DAT_00480e28,(LPCSTR)&local_4d0[0].pvReserved);
      return;
    }
    pcVar9 = &DAT_00481430;
    _DAT_0047f7d0 = 0xffffffff;
    DAT_0047f7d4 = 0xffffffff;
    DAT_0047f7d8 = 0xffffffff;
    DAT_0047f7dc = 0xffffffff;
    iVar6 = 0;
    do {
      if (*pcVar9 == -1) break;
      bVar1 = pcVar9[1];
      if ((bVar1 & 0xe0) != 0) {
        (&DAT_0047f7ce)[(uint)(bVar1 >> 5) * 2] = bVar1 & 0x1f;
        (&DAT_0047f7cf)[(uint)(bVar1 >> 5) * 2] = pcVar9[2] & 0x1f;
      }
      iVar6 = iVar6 + 1;
      pcVar9 = pcVar9 + 3;
    } while (iVar6 < 0x38);
    _DAT_0047f790 = 0xffffffff;
    if (DAT_0042a4e0 == '\0') {
      puVar14 = &DAT_00481a80;
      for (iVar6 = 0xa0; iVar6 != 0; iVar6 = iVar6 + -1) {
        *puVar14 = 0;
        puVar14 = puVar14 + 1;
      }
    }
    iVar6 = 0;
    if (local_4f2 != 0) {
      iVar15 = 0;
      local_4e4 = (uint)local_4f2;
      do {
        uVar4 = 0;
        if (local_4f1 != 0) {
          local_4d0[0].pvReserved = (void *)(uint)DAT_00495c02;
          local_4f0 = (uint)local_4f1;
          do {
            uVar7 = (((uVar4 & 1) == 0) - 1) + iVar6;
            if ((((int)uVar4 < (int)local_4d0[0].pvReserved) && (-1 < (int)uVar7)) &&
               ((int)uVar7 < (int)(uint)DAT_00495c03)) {
              if (DAT_0042a4e0 == '\0') {
                *(undefined *)((int)&DAT_00481a80 + uVar4 + uVar7 * 0x20) =
                     (&DAT_0042a530)[(ushort)(&DAT_004808a8)[iVar15 + uVar4]];
              }
              uVar2 = (undefined1)uVar4;
              uVar5 = (undefined1)uVar7;
              switch(*(byte *)((int)&DAT_00481a80 + uVar4 + uVar7 * 0x20) & 0xf) {
              case 8:
                _DAT_0047f790 = CONCAT11(uVar5,uVar2);
                break;
              case 10:
                _DAT_0047f790 = CONCAT13(uVar5,CONCAT12(uVar2,_DAT_0047f790));
                break;
              case 0xb:
              case 0xc:
              case 0xd:
                pbVar12 = &DAT_0047f7d0;
                do {
                  if ((*pbVar12 == uVar4) && (pbVar12[1] == uVar7)) goto switchD_00404463_caseD_9;
                  pbVar12 = pbVar12 + 2;
                } while (pbVar12 < (byte *)((int)&DAT_0047f7dc + 2));
                iVar8 = 0;
                pcVar9 = &DAT_0047f7d0;
                do {
                  if (*pcVar9 == -1) {
                    (&DAT_0047f7d0)[iVar8 * 2] = uVar2;
                    (&DAT_0047f7d1)[iVar8 * 2] = uVar5;
                    break;
                  }
                  pcVar9 = pcVar9 + 2;
                  iVar8 = iVar8 + 1;
                } while (pcVar9 < (char *)((int)&DAT_0047f7dc + 2));
              }
            }
switchD_00404463_caseD_9:
            uVar4 = uVar4 + 1;
          } while ((int)uVar4 < (int)local_4f0);
        }
        iVar15 = iVar15 + 0x20;
        iVar6 = iVar6 + 1;
      } while (iVar6 < (int)local_4e4);
    }
    if ((DAT_0047f790 == -1) && (DAT_0047f791 == -1)) {
      lpText = &DAT_0042a7bc;
    }
    else if (DAT_0047f792 == -1) {
      lpText = &DAT_0042a7a4;
    }
    else if (local_4ec == 0) {
      lpText = &DAT_0042a77c;
    }
    else {
      if (local_4e8 != 0) {
        DAT_004834a9 = 1;
        DAT_004834a8 = 0;
        DAT_00495c6a = 0;
        DAT_00495c52 = 0;
        goto LAB_00404562;
      }
      lpText = &DAT_0042a754;
    }
  }
  else {
    iVar6 = _strncmp(local_4e0,s_NEC_MAP_V1_01_0042a500,0x10);
    if (iVar6 == 0) {
      _hread(hFile,&DAT_004814e0,8);
      _hread(hFile,&local_4f1,1);
      _hread(hFile,&local_4f2,1);
      puVar14 = (undefined4 *)&DAT_004808a8;
      for (iVar6 = 0x160; iVar6 != 0; iVar6 = iVar6 + -1) {
        *puVar14 = 0;
        puVar14 = puVar14 + 1;
      }
      local_4f0 = 0;
      if (local_4f2 != 0) {
        local_4d0[0].pvReserved = &DAT_004808a8;
        do {
          iVar6 = 0;
          if (local_4f1 != 0) {
            puVar13 = local_4d0[0].pvReserved;
            do {
              iVar6 = iVar6 + 1;
              _hread(hFile,puVar13,2);
              *puVar13 = *puVar13 & 0x1ff;
              puVar13 = puVar13 + 1;
            } while (iVar6 < (int)(uint)local_4f1);
          }
          local_4f0 = local_4f0 + 1;
          local_4d0[0].pvReserved = (void *)((int)local_4d0[0].pvReserved + 0x40);
        } while ((int)local_4f0 < (int)(uint)local_4f2);
      }
      pbVar12 = &DAT_00481430;
      iVar6 = 0;
      do {
        _hread(hFile,pbVar12,1);
        if (*pbVar12 == 0xff) break;
        _hread(hFile,pbVar12 + 1,2);
        if ((*pbVar12 & 0x80) == 0) {
          local_4ec = local_4ec + 1;
        }
        else {
          local_4e8 = local_4e8 + 1;
        }
        iVar6 = iVar6 + 1;
        pbVar12 = pbVar12 + 3;
      } while (iVar6 < 0x38);
      _lclose(hFile);
      DAT_0042fb74 = '\x01';
      goto LAB_004042ff;
    }
    iVar6 = _strncmp(local_4e0,s_NEC_GAME_V1_00_0042a518,0x10);
    if (iVar6 == 0) {
      _hread(hFile,&DAT_0042fb74,1);
      _hread(hFile,&DAT_004814e0,8);
      _hread(hFile,&local_4f1,1);
      _hread(hFile,&local_4f2,1);
      local_4f0 = 0;
      if (local_4f2 != 0) {
        local_4d0[0].pvReserved = &DAT_004808a8;
        do {
          iVar6 = 0;
          if (local_4f1 != 0) {
            puVar13 = local_4d0[0].pvReserved;
            do {
              iVar6 = iVar6 + 1;
              _hread(hFile,puVar13,2);
              *puVar13 = *puVar13 & 0x1ff;
              puVar13 = puVar13 + 1;
            } while (iVar6 < (int)(uint)local_4f1);
          }
          local_4f0 = local_4f0 + 1;
          local_4d0[0].pvReserved = (void *)((int)local_4d0[0].pvReserved + 0x40);
        } while ((int)local_4f0 < (int)(uint)local_4f2);
      }
      _hread(hFile,&local_4f3,1);
      DAT_00495c52 = local_4f3 & 0xf;
      DAT_00495c6a = local_4f3 >> 4;
      _hread(hFile,&DAT_00495c51,1);
      _hread(hFile,&DAT_00495c63,1);
      _hread(hFile,&DAT_004834a8,2);
      _hread(hFile,&DAT_004957a0,0x38);
      _hread(hFile,&DAT_004957e0,0x38);
      _hread(hFile,&DAT_00495ab0,0x38);
      _hread(hFile,&DAT_00495af0,0x38);
      _hread(hFile,&DAT_00495c10,0x32);
      _hread(hFile,&DAT_00495bd0,0x32);
      _hread(hFile,&DAT_00481a80,0x280);
      _lclose(hFile);
      iVar6 = 0;
      do {
        if (((&DAT_00495af0)[iVar6] & 0xc0) == 0 &&
            (*(byte *)((int)&DAT_00495ab0 + iVar6) & 0xe0) == 0) {
          if ((*(byte *)((int)&DAT_004957a0 + iVar6) & 0x80) == 0) {
            local_4ec = local_4ec + 1;
          }
          else {
            local_4e8 = local_4e8 + 1;
          }
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 0x38);
      DAT_0042a4e0 = '\x01';
      goto LAB_004042ff;
    }
    lpText = &DAT_0042a7d4;
  }
  MessageBoxA((HWND)0x0,lpText,(LPCSTR)0x0,0);
LAB_004045d2:
  FUN_00412a30();
  return;
}



/* ================= FUN_00404600 @ 00404600 ================= */

void FUN_00404600(void)

{
  char cVar1;
  byte bVar2;
  BOOL BVar3;
  char *pcVar4;
  HFILE hFile;
  int iVar5;
  uint uVar6;
  uint uVar7;
  LPCCH lpBuffer;
  undefined4 *puVar8;
  byte *pbVar9;
  void **ppvVar10;
  tagOFNA *ptVar11;
  char *pcVar12;
  byte *pbVar13;
  char local_3df;
  byte local_3de;
  byte local_3dd;
  LPCCH local_3dc;
  int local_3d8;
  tagOFNA local_3d4;
  undefined4 local_35a [52];
  byte local_288 [256];
  _OFSTRUCT local_188;
  CHAR local_100 [256];
  
  puVar8 = &DAT_0042a844;
  ppvVar10 = &local_3d4.pvReserved;
  for (iVar5 = 0xb; iVar5 != 0; iVar5 = iVar5 + -1) {
    *ppvVar10 = (void *)*puVar8;
    puVar8 = puVar8 + 1;
    ppvVar10 = ppvVar10 + 1;
  }
  *(undefined2 *)ppvVar10 = *(undefined2 *)puVar8;
  puVar8 = local_35a;
  for (iVar5 = 0x34; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar8 = 0;
    puVar8 = puVar8 + 1;
  }
  *(undefined2 *)puVar8 = 0;
  local_288[0] = 0;
  iVar5 = 0;
  while ((char)local_3d4.pvReserved != '\0') {
    pcVar4 = (char *)((int)local_35a + iVar5 + -0x2e);
    if (*pcVar4 == '/') {
      *pcVar4 = '\0';
    }
    local_3d4.pvReserved._0_1_ = *(char *)((int)local_35a + iVar5 + -0x2d);
    iVar5 = iVar5 + 1;
  }
  ptVar11 = &local_3d4;
  for (iVar5 = 0x13; iVar5 != 0; iVar5 = iVar5 + -1) {
    ptVar11->lStructSize = 0;
    ptVar11 = (tagOFNA *)&ptVar11->hwndOwner;
  }
  local_3d4.lpstrFilter = (LPCSTR)&local_3d4.pvReserved;
  local_3d4.lpstrFile = (LPSTR)local_288;
  local_3d4.hwndOwner = DAT_00480e28;
  local_3d4.lpstrFileTitle = local_100;
  local_3d4.nMaxFile = 0x100;
  local_3d4.nMaxFileTitle = 0x100;
  local_3d4.lStructSize = 0x4c;
  local_3d4.Flags = 0x804;
  BVar3 = GetSaveFileNameA(&local_3d4);
  if (BVar3 != 0) {
    pcVar4 = _strchr(local_3d4.lpstrFile,0x2e);
    if (pcVar4 == (char *)0x0) {
      uVar6 = 0xffffffff;
      pcVar4 = (char *)&DAT_0042a83c;
      do {
        pcVar12 = pcVar4;
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        pcVar12 = pcVar4 + 1;
        cVar1 = *pcVar4;
        pcVar4 = pcVar12;
      } while (cVar1 != '\0');
      uVar6 = ~uVar6;
      iVar5 = -1;
      pbVar9 = (byte *)local_3d4.lpstrFile;
      do {
        pbVar13 = pbVar9;
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        pbVar13 = pbVar9 + 1;
        bVar2 = *pbVar9;
        pbVar9 = pbVar13;
      } while (bVar2 != 0);
      pbVar9 = (byte *)(pcVar12 + -uVar6);
      pbVar13 = pbVar13 + -1;
      for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pbVar13 = *(undefined4 *)pbVar9;
        pbVar9 = pbVar9 + 4;
        pbVar13 = pbVar13 + 4;
      }
      for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pbVar13 = *pbVar9;
        pbVar9 = pbVar9 + 1;
        pbVar13 = pbVar13 + 1;
      }
    }
    FUN_00403e10((byte *)local_3d4.lpstrFile);
    hFile = OpenFile(local_3d4.lpstrFile,&local_188,0x1000);
    if (hFile != -1) {
      _hwrite(hFile,s_NEC_GAME_V1_00_0042a518,0x10);
      _hwrite(hFile,&DAT_0042fb74,1);
      _hwrite(hFile,&DAT_004814e0,8);
      local_3de = (-(DAT_00495c02 == '\x0f') & 0xf0U) + 0x20;
      local_3dd = (-(DAT_00495c03 == '\n') & 0xf5U) + 0x16;
      _hwrite(hFile,(LPCCH)&local_3de,1);
      _hwrite(hFile,(LPCCH)&local_3dd,1);
      local_3d8 = 0;
      if (local_3dd != 0) {
        local_3dc = (LPCCH)&DAT_004808a8;
        do {
          iVar5 = 0;
          if (local_3de != 0) {
            lpBuffer = local_3dc;
            do {
              iVar5 = iVar5 + 1;
              _hwrite(hFile,lpBuffer,2);
              lpBuffer = lpBuffer + 2;
            } while (iVar5 < (int)(uint)local_3de);
          }
          local_3d8 = local_3d8 + 1;
          local_3dc = local_3dc + 0x40;
        } while (local_3d8 < (int)(uint)local_3dd);
      }
      local_3df = DAT_00495c6a * '\x10' + DAT_00495c52;
      _hwrite(hFile,&local_3df,1);
      _hwrite(hFile,&DAT_00495c51,1);
      _hwrite(hFile,&DAT_00495c63,1);
      _hwrite(hFile,&DAT_004834a8,2);
      _hwrite(hFile,(LPCCH)&DAT_004957a0,0x38);
      _hwrite(hFile,(LPCCH)&DAT_004957e0,0x38);
      _hwrite(hFile,(LPCCH)&DAT_00495ab0,0x38);
      _hwrite(hFile,&DAT_00495af0,0x38);
      _hwrite(hFile,&DAT_00495c10,0x32);
      _hwrite(hFile,&DAT_00495bd0,0x32);
      _hwrite(hFile,(LPCCH)&DAT_00481a80,0x280);
      _lclose(hFile);
    }
  }
  return;
}



/* ================= FUN_00404880 @ 00404880 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00404880(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_004823a0;
  for (iVar1 = 0x220; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  DAT_004823dc = 1;
  DAT_004823a0 = &DAT_004823e0;
  DAT_0048241c = 0xff;
  DAT_004823a8 = &DAT_00404870;
  _DAT_004823e8 = &DAT_00404870;
  DAT_004823e4 = &DAT_004823a0;
  DAT_004823e0 = 0xffffffff;
  DAT_00482310 = 2;
  return;
}



/* ================= FUN_004048e0 @ 004048e0 ================= */

byte __cdecl FUN_004048e0(undefined4 param_1,byte param_2)

{
  byte bVar1;
  byte bVar2;
  
  bVar1 = FUN_00404940(param_1,param_2);
  if (bVar1 == 0xff) {
    return 0xff;
  }
  bVar2 = FUN_00412dc0(param_2);
  if (bVar2 == 0xff) {
    FUN_00404a10(DAT_00482334);
    return 0xff;
  }
  DAT_00482334[3] = DAT_00482c20;
  return bVar1;
}



/* ================= FUN_00404940 @ 00404940 ================= */

byte __cdecl FUN_00404940(undefined4 param_1,byte param_2)

{
  undefined4 *puVar1;
  byte bVar2;
  undefined4 *puVar3;
  int *piVar4;
  
  if ('!' < DAT_00482310) {
    return 0xff;
  }
  puVar3 = &DAT_004823a0;
  bVar2 = 0;
  do {
    if (*(char *)(puVar3 + 0xf) == '\0') break;
    puVar3 = puVar3 + 0x10;
    bVar2 = bVar2 + 1;
  } while (bVar2 < 0x22);
  if (*(char *)(puVar3 + 0xf) != '\0') {
    return 0xff;
  }
  piVar4 = &DAT_004823a0;
  do {
    if ((param_2 & 0x7f) < (*(byte *)(piVar4 + 0xf) & 0x7f)) break;
    piVar4 = piVar4 + 0x10;
  } while (*piVar4 != -1);
  if ((param_2 & 0x7f) < (*(byte *)(piVar4 + 0xf) & 0x7f)) {
    puVar1 = (undefined4 *)piVar4[1];
    *puVar3 = piVar4;
    *puVar1 = puVar3;
    puVar3[1] = puVar1;
    piVar4[1] = (int)puVar3;
    puVar3[2] = param_1;
    *(byte *)(puVar3 + 0xf) = param_2;
    DAT_00482310 = DAT_00482310 + '\x01';
    DAT_00482334 = puVar3;
    return bVar2;
  }
  return 0xff;
}



/* ================= FUN_004049d0 @ 004049d0 ================= */

void FUN_004049d0(void)

{
  FUN_004049e0(DAT_00482330);
  return;
}



/* ================= FUN_004049e0 @ 004049e0 ================= */

void __cdecl FUN_004049e0(int *param_1)

{
  FUN_00404a10(param_1);
  FUN_00412e60((int *)param_1[3]);
  return;
}



/* ================= FUN_00404a00 @ 00404a00 ================= */

void FUN_00404a00(void)

{
  FUN_00404a10(DAT_00482330);
  return;
}



/* ================= FUN_00404a10 @ 00404a10 ================= */

void __cdecl FUN_00404a10(int *param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)param_1[1];
  iVar2 = *param_1;
  *piVar1 = iVar2;
  *(int **)(iVar2 + 4) = piVar1;
  if (DAT_00482330 == param_1) {
    DAT_00482330 = piVar1;
  }
  DAT_00482310 = DAT_00482310 + -1;
  *(undefined1 *)(param_1 + 0xf) = 0;
  param_1[2] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  return;
}



/* ================= FUN_00404a40 @ 00404a40 ================= */

void FUN_00404a40(void)

{
  undefined4 *puVar1;
  
  puVar1 = &DAT_004823a0;
  do {
    DAT_00482c20 = puVar1[3];
    DAT_00482330 = puVar1;
    (*(code *)puVar1[2])();
    puVar1 = (undefined4 *)*DAT_00482330;
  } while (puVar1 != (undefined4 *)0xffffffff);
  return;
}



/* ================= FUN_00404a70 @ 00404a70 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00404a70(void)

{
  if ((DAT_0048349f != '\0') && ((DAT_004834ac & 0x30) != 0)) {
    FUN_00412a30();
    return;
  }
  DAT_00481de6 = 1;
  if (((DAT_00495c57 == '\0') && (DAT_00495c68 == '\0')) &&
     ((&DAT_004834a8)[DAT_00495c63 >> 7] != '\0')) {
    DAT_00481de8 = 0;
    DAT_00481de9 = 0;
    DAT_00481dea = 0;
    DAT_00481de6 = 0;
    if (DAT_004834a1 != '\0') {
      if (DAT_00495c6c != '\0') {
        DAT_00495f34 = &LAB_0040fa10;
        DAT_00495c6c = '\0';
        DAT_00484720 = 0;
        DAT_00482030 = DAT_00482030 + '\x01';
        DAT_00484710 = 0xff;
      }
      FUN_0040cb30(DAT_00495f34);
    }
  }
                    /* WARNING: Could not recover jumptable at 0x00404b0f. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*_DAT_004834b0)();
  return;
}



/* ================= FUN_00404c70 @ 00404c70 ================= */

void FUN_00404c70(void)

{
  byte bVar1;
  
  bVar1 = FUN_004048e0(&LAB_00405320,0x53);
  if (bVar1 != 0xff) {
    *(undefined1 *)(DAT_00482c20 + 9) = 1;
    *(undefined2 *)(DAT_00482c20 + 0xc) = 0x10c;
    *(undefined2 *)(DAT_00482c20 + 0x10) = 0xf0;
    *(undefined2 *)(DAT_00482c20 + 0x12) = 0;
    *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
    FUN_0041c020(DAT_00482c20,0);
    *(undefined2 *)(DAT_00482334 + 0x18) = 0x180;
  }
  bVar1 = FUN_004048e0(&LAB_00405360,0x54);
  if (bVar1 != 0xff) {
    *(undefined1 *)(DAT_00482c20 + 9) = 2;
    *(undefined2 *)(DAT_00482c20 + 0xc) = 0x30;
    *(undefined2 *)(DAT_00482c20 + 0x10) = 0xb8;
    *(undefined2 *)(DAT_00482c20 + 0x12) = 0;
    FUN_0041c020(DAT_00482c20,1);
  }
  return;
}



/* ================= FUN_00404fa0 @ 00404fa0 ================= */

void FUN_00404fa0(void)

{
  undefined2 local_32;
  byte local_30 [4];
  undefined4 local_2c;
  undefined2 local_28;
  undefined1 local_26;
  char local_24 [4];
  char local_20 [4];
  char local_1c [4];
  char local_18 [4];
  char local_14;
  char local_10 [4];
  char local_c [4];
  char local_8 [4];
  char local_4 [4];
  
  local_24 = (char  [4])s_START_0042a998._0_4_;
  local_20 = (char  [4])s_START_0042a998._4_4_;
  local_1c = (char  [4])s_MANUAL_0042a98c._0_4_;
  local_18 = (char  [4])s_MANUAL_0042a98c._4_4_;
  local_14 = s_MANUAL_0042a98c[8];
  local_10 = (char  [4])s_1P_CONTINUE_0042a97c._0_4_;
  local_c = (char  [4])s_1P_CONTINUE_0042a97c._4_4_;
  local_8 = (char  [4])s_1P_CONTINUE_0042a97c._8_4_;
  local_4._0_2_ = s_1P_CONTINUE_0042a97c._12_2_;
  local_30[0] = 0;
  local_2c = DAT_0042a974;
  local_30[1] = 0xc0;
  local_30[2] = 10;
  local_30[3] = 0xca;
  local_28 = DAT_0042a978;
  local_32 = DAT_0042a970;
  local_26 = DAT_0042a97a;
  DAT_00495c4f = 0x70;
  DAT_00495c4e = 0x70;
  DAT_00495c4d = 0x70;
  DAT_00495c4c = 0x70;
  DAT_00481dac = 0xa4;
  (&DAT_00495c4c)[DAT_00495c73] = 0x60;
  DAT_00481da4 = DAT_00495c4c;
  DAT_00481da8 = local_24;
  FUN_00421620();
  DAT_00481dac = 0x164;
  DAT_00481da4 = DAT_00495c4d;
  DAT_00481da8 = local_1c;
  FUN_00421620();
  DAT_00481dac = 0xae;
  DAT_00481da4 = DAT_00495c4e;
  DAT_00481da8 = local_10;
  FUN_00421620();
  DAT_00481dac = 0x16e;
  DAT_00481da4 = DAT_00495c4f;
  DAT_00481da8 = (char *)&local_2c;
  FUN_00421620();
  DAT_00481da4 = 0x60;
  DAT_00481dac = local_30[DAT_00495c73] + 0xa4;
  DAT_00481da8 = (char *)&local_32;
  FUN_00421620();
  return;
}



/* ================= FUN_004052d0 @ 004052d0 ================= */

void FUN_004052d0(void)

{
  if ((DAT_004834bc & 7) == 0) {
    *(ushort *)(DAT_00482330 + 0x12) = *(short *)(DAT_00482330 + 0x12) + 1U & 0xf;
    FUN_0041be90(&DAT_0042a898 + (*(ushort *)(DAT_00482330 + 0x12) & 7) * 2,0x61,1);
  }
  return;
}



/* ================= FUN_00405860 @ 00405860 ================= */

void __cdecl FUN_00405860(undefined1 param_1)

{
  byte bVar1;
  uint uVar2;
  
  DAT_0048233c = 0x2000;
  uVar2 = 0;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_00419cd0(param_1,(&DAT_0042a8e8)[uVar2],(ushort)DAT_0048233c);
    DAT_0048233c = DAT_0048233c + 0x100;
    uVar2 = (uint)bVar1;
  } while (bVar1 < 0x10);
  return;
}



/* ================= FUN_004058d0 @ 004058d0 ================= */

void __cdecl FUN_004058d0(byte param_1,char param_2,short param_3)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  ushort uVar4;
  undefined3 extraout_var;
  
  do {
    FUN_0040da00(param_3);
    DAT_00482350 = 0;
    do {
      bVar1 = FUN_00418bf0();
      if (bVar1 == 0xfe) {
        bVar1 = FUN_00418bf0();
        DAT_00482350 = DAT_00482350 + bVar1;
        bVar2 = FUN_00418bf0();
        if (bVar2 == 0xff) {
          FUN_00418bf0();
          DAT_00482354 = -0x570;
        }
        else {
          cVar3 = FUN_00418bf0();
          DAT_00482354 = CONCAT31(extraout_var,cVar3) * 0x100 + (uint)bVar2;
        }
        do {
          FUN_0040daf0((short)DAT_00482354 + 0x600);
          bVar1 = bVar1 - 1;
        } while (bVar1 != 0);
      }
      else {
        if (bVar1 == 0xff) {
          FUN_00418bf0();
          uVar4 = 0x90;
        }
        else {
          bVar2 = FUN_00418bf0();
          uVar4 = (bVar2 + 6) * 0x100 + (ushort)bVar1;
        }
        FUN_0040daf0(uVar4);
        DAT_00482350 = DAT_00482350 + 1;
      }
    } while (DAT_00482350 != param_1);
    param_3 = param_3 + 0x40;
    param_2 = param_2 + -1;
  } while (param_2 != '\0');
  return;
}



/* ================= FUN_004059c0 @ 004059c0 ================= */

void FUN_004059c0(void)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  undefined2 local_10 [8];
  
  FUN_00412cd0();
  FUN_0041bf20();
  FUN_0041be90(&DAT_0042a8b8,0x21,1);
  DAT_004834c0 = 0;
  DAT_004834ba = 0xfd;
  FUN_004048e0(&LAB_004053c0,0x50);
  *(undefined1 *)(DAT_00482c20 + 9) = 2;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0x6d;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0xb8;
  FUN_0041c020(DAT_00482c20,1);
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    FUN_004048e0(&LAB_00405810,0x5a);
    uVar1 = local_10[iVar2];
    *(undefined1 *)(DAT_00482c20 + 9) = (&DAT_0042a8d0)[iVar2];
    *(undefined2 *)(DAT_00482c20 + 0xc) = uVar1;
    *(undefined2 *)(DAT_00482c20 + 0x10) = 0x60;
    FUN_0041c020(DAT_00482c20,2);
    iVar2 = iVar3;
  } while (iVar3 < 8);
  FUN_00404940(&LAB_00405220,10);
  return;
}



/* ================= FUN_00405ad0 @ 00405ad0 ================= */

void __cdecl
FUN_00405ad0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  undefined4 unaff_retaddr;
  
  DAT_004834ae = 1;
  DAT_00495c06 = 0;
  DAT_00495c05 = 0;
  DAT_00495c45 = 0;
  DAT_00495c44 = 0;
  DAT_00495c42 = 0;
  DAT_00495c46 = 0;
  DAT_00495bc0 = 0;
  DAT_00495c04 = 0;
  DAT_00495c47 = 0;
  DAT_00495c43 = 0;
  DAT_00495c48 = 0;
  DAT_00495bc2 = 0;
  DAT_00495bc1 = 0;
  if (DAT_0042fb74 == '\0') {
    DAT_00495c02 = (&DAT_0042a9a0)[DAT_00495c52];
    DAT_00495c03 = (&DAT_0042a9b0)[DAT_00495c52];
  }
  DAT_00495c6d = 0;
  if (DAT_0042fb74 == '\0') {
    FUN_00423c30();
  }
  FUN_00409f80();
  FUN_00410190();
  DAT_00481d80 = 0;
  FUN_00405b70(unaff_retaddr,param_1,param_2,param_3,param_4,param_5);
  DAT_0042a4e0 = 0;
  return;
}



/* ================= FUN_00405b70 @ 00405b70 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00405b70(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6)

{
  undefined4 unaff_retaddr;
  
  DAT_004834ae = 1;
  DAT_004834ba = 2;
  DAT_00495b74 = 0x3800;
  FUN_004068b0();
  FUN_0040cd80(0x11,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0x13,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0xd,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0xe,unaff_retaddr,param_1,param_2,param_3);
  FUN_0041e050();
  DAT_00481d80 = DAT_00481d80 & 1;
  FUN_0041bf50(4);
  _DAT_00481e32 = 0x42;
  FUN_0041be10();
  DAT_00481da4 = 0x70;
  FUN_0041dee0();
  FUN_004102b0();
  FUN_0040a050();
  FUN_0040a0f0();
  FUN_004246e0();
  FUN_0041d1c0();
  FUN_00416860(unaff_retaddr,param_1,param_2,param_3,param_4,param_5,param_6);
  FUN_0041d220(unaff_retaddr,param_1,param_2,param_3,param_4,param_5,param_6);
  DAT_00495f38 = 1;
  FUN_00416320();
  return;
}



/* ================= FUN_00405c20 @ 00405c20 ================= */

void FUN_00405c20(void)

{
  char cVar1;
  
  DAT_00495c43 = (-(DAT_00495c63 == '\0') & 2U) + 0xb;
  DAT_00495c47 = (-(DAT_00495c63 == '\0') & 2U) + 8;
  DAT_00495c04 = (-(DAT_00495c63 == '\0') & 0xfeU) + 0xd;
  DAT_00495bc2 = 0;
  DAT_00495bc1 = 0;
  DAT_00495bc0 = (-(DAT_00495c63 == '\0') & 0xfeU) + 10;
  FUN_00405ca0();
  FUN_00423a80();
  cVar1 = DAT_004834a8;
  if (DAT_00495c63 != '\0') {
    cVar1 = DAT_004834a9;
  }
  if (cVar1 != '\0') {
    FUN_00417900();
  }
  DAT_00495c6c = 0;
  return;
}



/* ================= FUN_00405ca0 @ 00405ca0 ================= */

void FUN_00405ca0(void)

{
  byte bVar1;
  
  if (DAT_00495c52 == '\x0f') {
    bVar1 = 6;
  }
  else if (DAT_00495aa1 >> 1 < DAT_00495aa2) {
    bVar1 = (-(DAT_00495aa2 >> 1 < DAT_00495aa1) & 2U) + 2;
  }
  else {
    bVar1 = 0;
  }
  if (DAT_00495c63 != '\0') {
    bVar1 = bVar1 + 1;
  }
  bVar1 = (&DAT_0042a9c0)[bVar1];
  if (DAT_00495bc2 != bVar1) {
    DAT_00495bc2 = bVar1;
    FUN_00412890(bVar1);
  }
  return;
}



/* ================= FUN_00405d00 @ 00405d00 ================= */

void FUN_00405d00(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_00405d40,10);
  return;
}



/* ================= FUN_00405d20 @ 00405d20 ================= */

void FUN_00405d20(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_00406700,10);
  return;
}



/* ================= FUN_00405e50 @ 00405e50 ================= */

void FUN_00405e50(void)

{
  FUN_00412b70(0x2c,0x60,0xdc,0xa8);
  return;
}



/* ================= FUN_00405ef0 @ 00405ef0 ================= */

void FUN_00405ef0(void)

{
  byte bVar1;
  
  bVar1 = FUN_004048e0(&LAB_00405f70,100);
  if (bVar1 != 0xff) {
    *(undefined1 *)(DAT_00482c20 + 9) = 0xa5;
    *(undefined2 *)(DAT_00482c20 + 0xc) = 0x48;
    *(undefined2 *)(DAT_00482c20 + 0x10) = 0xa8;
    *(undefined2 *)(DAT_00482c20 + 0x12) = 0;
    *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
    FUN_0041c020(DAT_00482c20,2);
    *(undefined2 *)(DAT_00482334 + 0x10) = 0;
    *(undefined2 *)(DAT_00482334 + 0x12) = 0;
    *(undefined2 *)(DAT_00482334 + 0x14) = 10;
  }
  return;
}



/* ================= FUN_004061a0 @ 004061a0 ================= */

void FUN_004061a0(void)

{
  return;
}



/* ================= FUN_004061b0 @ 004061b0 ================= */

undefined4 FUN_004061b0(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  if (DAT_004814fc == '\0') {
    return 0;
  }
  uVar1 = (int)(DAT_00480ef8 + -0x2c + (DAT_00480ef8 + -0x2c >> 0x1f & 0xfU)) >> 4;
  uVar3 = uVar1 & 0xff;
  uVar2 = (DAT_00480efc + -0x60) / 0x18 & 0xff;
  if ((int)*(short *)(DAT_00482330 + 0x12) != uVar2 ||
      (int)(short)*(ushort *)(DAT_00482330 + 0x10) != uVar3) {
    if (uVar2 * 0xb + uVar3 < 0x1a) {
      *(ushort *)(DAT_00482330 + 0x10) = (ushort)(byte)uVar1;
      *(short *)(DAT_00482330 + 0x12) = (short)uVar2;
      return 1;
    }
    FUN_004061a0();
  }
  return 0;
}



/* ================= FUN_00406250 @ 00406250 ================= */

void FUN_00406250(void)

{
  int iVar1;
  byte bVar2;
  
  DAT_0048234c = *(short *)(DAT_00482330 + 0x12) * 0xb + (int)*(short *)(DAT_00482330 + 0x10);
  if (DAT_00495b91 < 6) {
    *(char *)((int)&DAT_00495b88 + (uint)DAT_00495b91) = (char)DAT_0048234c;
    FUN_00412890(6);
    bVar2 = FUN_004048e0(&LAB_004064c0,0x50);
    if (bVar2 != 0xff) {
      *(char *)(DAT_00482c20 + 9) = (char)DAT_0048234c + -0x75;
      *(short *)(DAT_00482c20 + 0xc) = *(short *)(*(int *)(DAT_00482330 + 0xc) + 0xc) + 8;
      *(undefined2 *)(DAT_00482c20 + 0x10) = *(undefined2 *)(*(int *)(DAT_00482330 + 0xc) + 0x10);
      *(undefined2 *)(DAT_00482c20 + 0x12) = 0;
      *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
      FUN_0041c020(DAT_00482c20,0xe);
      iVar1 = DAT_00482334;
      (&DAT_00495ba0)[DAT_00495b91] = DAT_00482334;
      *(undefined2 *)(iVar1 + 0x10) = 9;
      *(short *)(DAT_00482334 + 0x12) = (0x110 - *(short *)(DAT_00482c20 + 0x10)) * 0x20;
      *(ushort *)(DAT_00482334 + 0x16) =
           (((ushort)DAT_00495b91 * 8 + 200) - *(short *)(DAT_00482c20 + 0xc)) * 0x20;
      DAT_00495b91 = DAT_00495b91 + 1;
    }
  }
  return;
}



/* ================= FUN_00406380 @ 00406380 ================= */

void FUN_00406380(void)

{
  uint uVar1;
  
  if (DAT_00495b91 == '\0') {
    FUN_00412a30();
    return;
  }
  DAT_00495b91 = DAT_00495b91 - 1;
  FUN_0041c050(*(int *)((&DAT_00495ba0)[DAT_00495b91] + 0xc));
  FUN_004049e0((int *)(&DAT_00495ba0)[DAT_00495b91]);
  uVar1 = (uint)DAT_00495b91;
  (&DAT_00495ba0)[uVar1] = 0;
  *(undefined1 *)((int)&DAT_00495b88 + uVar1) = 0;
  FUN_00412890(0xc);
  return;
}



/* ================= FUN_004063f0 @ 004063f0 ================= */

void FUN_004063f0(void)

{
  int iVar1;
  byte bVar2;
  
  if (5 < DAT_00495b91) {
    bVar2 = 5;
    do {
      if (*(short *)((&DAT_00495ba0)[bVar2] + 0x10) != 0) {
        return;
      }
      bVar2 = bVar2 - 1;
    } while ((bVar2 & 0x80) == 0);
    iVar1 = FUN_00406450();
    if (iVar1 == 0) {
      FUN_00406520();
      FUN_00412890(0xe);
      return;
    }
    FUN_00412890(0xff);
    FUN_00404a00();
    DAT_00495b84 = 1;
  }
  return;
}



/* ================= FUN_00406450 @ 00406450 ================= */

undefined4 FUN_00406450(void)

{
  byte bVar1;
  byte bVar2;
  
  bVar2 = 0;
  do {
    bVar1 = 0;
    while ((int)s_REVOLT_0042a9c8[(uint)bVar2 * 8 + (uint)bVar1] -
           (uint)*(byte *)((int)&DAT_00495b88 + (uint)bVar1) == 0x41) {
      bVar1 = bVar1 + 1;
      if (bVar1 == 6) {
        DAT_00495c6a = 0xf < bVar2;
        if ((bool)DAT_00495c6a) {
          bVar2 = bVar2 - 0x10;
        }
        DAT_00495c52 = bVar2;
        return 1;
      }
    }
    bVar2 = bVar2 + 1;
    if (bVar2 == 0x21) {
      return 0;
    }
  } while( true );
}



/* ================= FUN_00406520 @ 00406520 ================= */

void FUN_00406520(void)

{
  int *piVar1;
  uint uVar2;
  byte local_1;
  
  local_1 = 5;
  do {
    uVar2 = (uint)local_1;
    piVar1 = &DAT_00495ba0 + uVar2;
    FUN_0041c050(*(int *)(*piVar1 + 0xc));
    FUN_004049e0((int *)*piVar1);
    local_1 = local_1 - 1;
    *(undefined1 *)((int)&DAT_00495b88 + uVar2) = 0;
    *piVar1 = 0;
  } while ((local_1 & 0x80) == 0);
  DAT_00495b91 = 0;
  return;
}



/* ================= FUN_00406580 @ 00406580 ================= */

void __cdecl FUN_00406580(undefined2 param_1,ushort param_2,byte param_3)

{
  ushort uVar1;
  ushort uVar2;
  char cVar3;
  uint local_4;
  
  FUN_0040da10(param_1);
  FUN_0040da00(param_2);
  if (param_3 != 0) {
    local_4 = (uint)param_3;
    do {
      cVar3 = '\b';
      do {
        uVar1 = FUN_0040daa0();
        uVar2 = FUN_0040daa0();
        FUN_0040da50((uVar2 & 0xf0) >> 4 | (uVar2 & 0xf0ff) >> 8 | (uVar1 & 0xf0) << 4 |
                     uVar1 & 0xf000);
        FUN_0040da50(((uVar1 & 0xf) << 4 | uVar1 & 0xf00) << 4 | (uVar2 & 0xf00) >> 4 | uVar2 & 0xf)
        ;
        cVar3 = cVar3 + -1;
      } while (cVar3 != '\0');
      cVar3 = '\x18';
      do {
        FUN_0040da20(0);
        FUN_0040da20(0);
        cVar3 = cVar3 + -1;
      } while (cVar3 != '\0');
      local_4 = local_4 - 1;
    } while (local_4 != 0);
  }
  FUN_00412750((uint)param_2,(uint)param_3);
  return;
}



/* ================= FUN_004066a0 @ 004066a0 ================= */

void FUN_004066a0(void)

{
  char cVar1;
  byte bVar2;
  undefined3 extraout_var;
  short sVar3;
  short sVar4;
  short sVar5;
  
  DAT_00481f94 = &DAT_0042aad0;
  sVar4 = 0x1c0;
  sVar5 = 0x19;
  FUN_00418bb0();
  do {
    sVar3 = 0x20;
    FUN_0040da00(sVar4);
    do {
      cVar1 = FUN_00418bf0();
      bVar2 = FUN_00418bf0();
      FUN_0040daf0((short)CONCAT31(extraout_var,cVar1) + (bVar2 + 3) * 0x100);
      sVar3 = sVar3 + -1;
    } while (sVar3 != 0);
    sVar4 = sVar4 + 0x40;
    sVar5 = sVar5 + -1;
  } while (sVar5 != 0);
  return;
}



/* ================= FUN_004067e0 @ 004067e0 ================= */

void __cdecl
FUN_004067e0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 unaff_retaddr;
  
  FUN_0040c850(10,0xf,(byte *)s_NUMBER____2d_0042aec8,(uint)DAT_00495b90,unaff_retaddr,param_1,
               param_2,param_3,param_4,param_5,param_6,param_7,param_8);
  return;
}



/* ================= FUN_00406830 @ 00406830 ================= */

void FUN_00406830(byte param_1,undefined1 param_2,undefined1 param_3)

{
  FUN_00406900(param_1,param_2,param_3);
  FUN_00406a90();
  return;
}



/* ================= FUN_00406850 @ 00406850 ================= */

void __cdecl FUN_00406850(byte param_1,undefined1 param_2,undefined1 param_3)

{
  FUN_00406900(param_1,param_2,param_3);
  FUN_00406c30();
  FUN_00404940(&LAB_00406890,100);
  DAT_00495b34 = DAT_00482334;
  return;
}



/* ================= FUN_004068b0 @ 004068b0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_004068b0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  DAT_00495b82 = DAT_00495b74;
  DAT_00495b70 = 0;
  DAT_00495b72 = CONCAT11((byte)(DAT_00495b74 >> 0xc),(char)(DAT_00495b74 >> 4)) | 0x7000;
  DAT_00495b80 = 0;
  _DAT_00495b78 = 0;
  _DAT_00495b7c = 0;
  puVar2 = &DAT_00495b40;
  for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  return;
}



/* ================= FUN_00406900 @ 00406900 ================= */

void __cdecl FUN_00406900(byte param_1,undefined1 param_2,undefined1 param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  byte bVar3;
  int iVar4;
  uint uVar5;
  char cVar6;
  undefined2 *puVar7;
  byte local_3;
  undefined2 local_2;
  
  bVar3 = 0;
  DAT_00495b30 = 0;
  cVar6 = DAT_00495b78;
  while (cVar6 != '\0') {
    bVar3 = bVar3 + 1;
    cVar6 = (&DAT_00495b78)[bVar3];
  }
  DAT_00495b80 = bVar3;
  (&DAT_00495b68)[bVar3] = param_3;
  (&DAT_00495b78)[bVar3] = 1;
  (&DAT_00495b60)[bVar3] = param_2;
  cVar6 = '\0';
  FUN_0040da00(DAT_00495b82);
  DAT_00495b5a = 0;
  DAT_00495b59 = 0;
  DAT_00495b58 = 0;
  local_3 = 0;
  puVar7 = (undefined2 *)(&PTR_DAT_00435f28)[param_1];
  local_2 = *puVar7;
  iVar4 = _strncmp((char *)&local_2,&DAT_0042aedc,2);
  while (iVar4 != 0) {
    puVar7 = puVar7 + 1;
    iVar4 = _strncmp((char *)&local_2,&DAT_0042aed8,2);
    if (iVar4 == 0) {
      uVar5 = (uint)local_3;
      local_3 = local_3 + 1;
      (&DAT_00495b58)[uVar5] = cVar6;
      cVar6 = '\0';
    }
    else {
      cVar6 = cVar6 + '\x01';
      FUN_0040df50((char)local_2);
      FUN_00412680((uint)DAT_00495b30 * 0x40 + (uint)DAT_00495b82,4);
      DAT_00495b30 = DAT_00495b30 + 1;
    }
    local_2 = *puVar7;
    iVar4 = _strncmp((char *)&local_2,&DAT_0042aedc,2);
  }
  (&DAT_00495b58)[local_3] = cVar6;
  uVar2 = DAT_00495b5a;
  uVar1 = DAT_00495b59;
  uVar5 = (uint)DAT_00495b80;
  *(undefined1 *)((int)&DAT_00495b40 + uVar5 * 3) = DAT_00495b58;
  iVar4 = uVar5 * 3;
  *(undefined1 *)((int)&DAT_00495b40 + iVar4 + 1) = uVar1;
  *(undefined1 *)((int)&DAT_00495b40 + iVar4 + 2) = uVar2;
  DAT_00495b82 = DAT_00495b82 + (ushort)DAT_00495b30 * 0x40;
  return;
}



/* ================= FUN_00406a90 @ 00406a90 ================= */

void FUN_00406a90(void)

{
  byte bVar1;
  char cVar2;
  
  DAT_00495b38 = DAT_00495b72;
  bVar1 = 0;
  DAT_00495b3d = (&DAT_00495b68)[DAT_00495b80];
  DAT_00495b3c = (&DAT_00495b60)[DAT_00495b80];
LAB_00406abf:
  do {
    DAT_00495b70 = bVar1;
    if ((&DAT_00495b58)[bVar1] != '\0') {
      FUN_00406b60();
      bVar1 = DAT_00495b70;
      DAT_00495b3c = DAT_00495b3c + '\x02';
      cVar2 = (&DAT_00495b58)[DAT_00495b70] + -1;
      (&DAT_00495b58)[DAT_00495b70] = cVar2;
      if (cVar2 != '\0') goto LAB_00406abf;
    }
    FUN_00406b40();
    bVar1 = DAT_00495b70 + 1;
    if (bVar1 == 3) {
      DAT_00495b70 = bVar1;
      FUN_00406b20();
      return;
    }
  } while( true );
}



/* ================= FUN_00406b20 @ 00406b20 ================= */

void FUN_00406b20(void)

{
  DAT_00495b72 = DAT_00495b72 + (ushort)DAT_00495b30 * 4;
  return;
}



/* ================= FUN_00406b40 @ 00406b40 ================= */

void FUN_00406b40(void)

{
  DAT_00495b3d = DAT_00495b3d + '\x02';
  DAT_00495b3c = (&DAT_00495b60)[DAT_00495b80];
  return;
}



/* ================= FUN_00406b60 @ 00406b60 ================= */

void FUN_00406b60(void)

{
  ushort uVar1;
  
  FUN_0040da00((ushort)DAT_00495b3d * 0x40 + (ushort)DAT_00495b3c + DAT_004834c4);
  uVar1 = DAT_00495b38;
  DAT_00495b38 = DAT_00495b38 + 1;
  FUN_0040daf0(uVar1);
  uVar1 = DAT_00495b38;
  DAT_00495b38 = DAT_00495b38 + 1;
  FUN_0040daf0(uVar1);
  FUN_0040da00((DAT_00495b3d + 1) * 0x40 + (ushort)DAT_00495b3c + DAT_004834c4);
  uVar1 = DAT_00495b38;
  DAT_00495b38 = DAT_00495b38 + 1;
  FUN_0040daf0(uVar1);
  uVar1 = DAT_00495b38;
  DAT_00495b38 = DAT_00495b38 + 1;
  FUN_0040daf0(uVar1);
  return;
}



/* ================= FUN_00406c30 @ 00406c30 ================= */

void FUN_00406c30(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_00495b80;
  DAT_00495b70 = 0;
  DAT_00495b3e = 0;
  DAT_00495b3d = (&DAT_00495b68)[uVar1];
  DAT_00495b38 = DAT_00495b72;
  DAT_00495b3b = 1;
  DAT_00495b3c = (&DAT_00495b60)[uVar1];
  DAT_00495b3a = *(undefined1 *)((int)&DAT_00495b40 + uVar1 * 3);
  return;
}



/* ================= FUN_00406c80 @ 00406c80 ================= */

void FUN_00406c80(void)

{
  if (DAT_00495b3b == '\0') {
    return;
  }
  if (DAT_00495b3a != '\0') {
    if ((char)(DAT_00495b3e + '\x01') != '\x03') {
      DAT_00495b3e = DAT_00495b3e + '\x01';
      return;
    }
    DAT_00495b3e = '\0';
    FUN_00406b60();
    DAT_00495b3c = DAT_00495b3c + '\x02';
    DAT_00495b3a = DAT_00495b3a + -1;
    if (DAT_00495b3a != '\0') {
      return;
    }
    FUN_00406b40();
  }
  DAT_00495b70 = DAT_00495b70 + 1;
  DAT_00495b3a = *(undefined1 *)((int)&DAT_00495b40 + (uint)DAT_00495b70 + (uint)DAT_00495b80 * 3);
  if (DAT_00495b70 == 3) {
    DAT_00495b3b = 0;
    FUN_00406b20();
    return;
  }
  return;
}



/* ================= FUN_00406d00 @ 00406d00 ================= */

void FUN_00406d00(void)

{
  short sVar1;
  
  sVar1 = DAT_00495b82 - DAT_00495b74;
  FUN_0040da00(DAT_00495b74);
  do {
    FUN_0040da20(0);
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  do {
    FUN_00406e20((ushort)(byte)(&DAT_00495b68)[DAT_00495b80] << 6 |
                 (ushort)(byte)(&DAT_00495b60)[DAT_00495b80]);
    DAT_00495b80 = DAT_00495b80 - 1;
  } while (DAT_00495b80 != 0xff);
  FUN_004068b0();
  return;
}



/* ================= FUN_00406d70 @ 00406d70 ================= */

void FUN_00406d70(void)

{
  char cVar1;
  byte bVar2;
  ushort uVar3;
  short sVar4;
  
  uVar3 = (ushort)DAT_00495b30;
  sVar4 = DAT_00495b82 + uVar3 * -0x40;
  FUN_0040da00(sVar4);
  do {
    sVar4 = sVar4 + 1;
    FUN_0040da20(0);
  } while (DAT_00495b82 != sVar4);
  DAT_00495b82 = DAT_00495b82 + uVar3 * -0x40;
  bVar2 = 7;
  cVar1 = DAT_00495b7f;
  while (cVar1 == '\0') {
    bVar2 = bVar2 - 1;
    cVar1 = (&DAT_00495b78)[bVar2];
  }
  (&DAT_00495b78)[bVar2] = 0;
  FUN_00406e20((ushort)(byte)(&DAT_00495b68)[bVar2] * 0x40 + (ushort)(byte)(&DAT_00495b60)[bVar2] +
               DAT_004834c4);
  DAT_00495b80 = DAT_00495b80 + -1;
  DAT_00495b72 = DAT_00495b72 + (ushort)DAT_00495b30 * -4;
  return;
}



/* ================= FUN_00406e20 @ 00406e20 ================= */

void __cdecl FUN_00406e20(ushort param_1)

{
  byte bVar1;
  
  FUN_00406ea0(*(byte *)((int)&DAT_00495b40 + (uint)DAT_00495b80 * 3),param_1);
  bVar1 = *(byte *)((int)&DAT_00495b40 + (uint)DAT_00495b80 * 3 + 1);
  if (bVar1 != 0) {
    FUN_00406ea0(bVar1,param_1 + 0x80);
    bVar1 = *(byte *)((int)&DAT_00495b40 + (uint)DAT_00495b80 * 3 + 2);
    if (bVar1 != 0) {
      FUN_00406ea0(bVar1,param_1 + 0x100);
    }
  }
  return;
}



/* ================= FUN_00406ea0 @ 00406ea0 ================= */

void __cdecl FUN_00406ea0(byte param_1,ushort param_2)

{
  FUN_0040dba0(param_2 & 0x3f,(uint)(param_2 >> 6),(uint)param_1 * 2,2,0);
  return;
}



/* ================= FUN_00407470 @ 00407470 ================= */

char __cdecl FUN_00407470(byte param_1,byte param_2,byte param_3)

{
  byte bVar1;
  
  if (param_1 == 0) {
    return '%';
  }
  if (param_1 == 1) {
    if ((3 < param_2) && (param_2 != 0x16)) {
      return '\f';
    }
LAB_0040754e:
    bVar1 = FUN_0040cb50();
    return (bVar1 & 1) + 8;
  }
  if (param_1 < 4) goto LAB_0040754e;
  if (6 < param_1) {
    if (param_1 == 9) {
      if (param_3 < 6) {
        return '\x10';
      }
      return '\x11';
    }
    if (((param_1 < 9) || (param_1 == 0xd)) || (param_1 == 0xe)) {
LAB_004074ff:
      if (param_3 < 5) {
        bVar1 = FUN_0040cb50();
        return (bVar1 & 1) + 3;
      }
    }
    else {
      if (param_1 == 0xf) {
        if ((*(byte *)(DAT_00482330 + 0x2e) & 0x11) == 0) {
          if (param_3 < 5) {
            return '\x15';
          }
          return (-(param_3 < 7) & 0xfeU) + 0x16;
        }
        goto LAB_00407524;
      }
      if (param_1 == 0x11) {
        return '\x10';
      }
      if (0x11 < param_1) {
        if (param_1 == 0x14) goto LAB_004074ff;
        if (param_1 < 0x14) {
          bVar1 = FUN_0040cb50();
          return (bVar1 & 1) + 0xe;
        }
        if (param_1 == 0x16) {
          return '\f';
        }
      }
    }
  }
  if (DAT_00481dbc != '\0') {
    return DAT_00481dbc;
  }
LAB_00407524:
  if (param_3 < 5) {
    bVar1 = FUN_0040cb50();
    return (bVar1 & 1) + 5;
  }
  if (param_3 < 7) {
    bVar1 = FUN_0040cb50();
    return (bVar1 & 3) + 1;
  }
  return '\0';
}



/* ================= FUN_00407610 @ 00407610 ================= */

void FUN_00407610(void)

{
  byte bVar1;
  uint uVar2;
  byte local_1;
  
  DAT_00482391 = 0xff;
  local_1 = 0x37;
  do {
    uVar2 = (uint)local_1;
    if (((*(byte *)((int)&DAT_004957a0 + uVar2) & 0x80) == DAT_00495c63) &&
       ((*(byte *)((int)&DAT_00481fa0 + uVar2) & 7) == 1)) {
      DAT_0048327b = local_1;
      DAT_00481dbc = 0x80;
      DAT_00481fda = *(byte *)((int)&DAT_00495ab0 + uVar2) & 0x1f;
      DAT_00481fdb = (&DAT_00495af0)[uVar2] & 0x1f;
      FUN_0041da80();
      if (DAT_00481df5 < 0xb) {
        *(byte *)((int)&DAT_00481fa0 + uVar2) = *(byte *)((int)&DAT_00481fa0 + uVar2) | 0x80;
      }
      else {
        bVar1 = *(byte *)((int)&DAT_00481ff0 + uVar2) >> 4;
        if (bVar1 < DAT_00482391) {
          DAT_00482390 = local_1;
          DAT_00482391 = bVar1;
        }
      }
    }
    local_1 = local_1 - 1;
  } while (local_1 < 0x80);
  if (DAT_00482391 == 0xff) {
    FUN_00407a70();
    return;
  }
  FUN_004076d0();
  return;
}



/* ================= FUN_004076d0 @ 004076d0 ================= */

void FUN_004076d0(void)

{
  byte bVar1;
  uint uVar2;
  byte bVar3;
  int iVar4;
  bool bVar5;
  
  DAT_0048235a = 0xff;
  uVar2 = 0x37;
  DAT_00482391 = *(byte *)((int)&DAT_00495ab0 + (uint)DAT_00482390);
  DAT_00482392 = (&DAT_00495af0)[DAT_00482390];
  do {
    bVar3 = (byte)uVar2;
    if ((((((&DAT_00495af0)[uVar2] & 0xe0) == 0) &&
         ((*(byte *)((int)&DAT_004957a0 + uVar2) & 0x80) == DAT_00495c63)) &&
        (*(char *)((int)&DAT_00481fa0 + uVar2) == '\0')) && (DAT_00482390 != bVar3)) {
      DAT_0048327e = (&DAT_00495af0)[uVar2] & 0x1f;
      DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar2) & 0x1f;
      bVar1 = FUN_004179f0(DAT_00482391,DAT_00482392);
      if (bVar1 < DAT_0048235a) {
        DAT_00482358 = bVar3;
        DAT_0048235a = bVar1;
      }
    }
    uVar2 = (uint)(byte)(bVar3 - 1);
  } while ((byte)(bVar3 - 1) < 0x80);
  if (DAT_0048235a == 0xff) {
    FUN_00407a70();
    return;
  }
  DAT_0048327b = DAT_00482358;
  DAT_00481df5 = 0xff;
  DAT_00482393 = DAT_00482358;
  DAT_00482394 = *(byte *)((int)&DAT_00495ab0 + (uint)DAT_00482358) & 0x1f;
  DAT_00482395 = (&DAT_00495af0)[DAT_00482358] & 0x1f;
  DAT_00495f30 = 1;
  DAT_00495c6e = 0;
  DAT_00495ef8 = 0;
  DAT_0048327d = DAT_00482394;
  DAT_0048327e = DAT_00482395;
  FUN_004015d0();
  *(undefined1 *)((int)&DAT_00495820 + (uint)DAT_00482394 + (uint)DAT_00482395 * 0x20) = 0xff;
  DAT_0048239a = 0xff;
  DAT_00482397 = 1;
  DAT_00481fda = DAT_00482391;
  DAT_00481fdb = DAT_00482392;
  DAT_0048327b = DAT_00482390;
  do {
    FUN_004179b0(4);
    DAT_00482399 = 0;
    do {
      DAT_00482398 = DAT_00482397;
      do {
        bVar1 = DAT_00481fdb;
        bVar3 = DAT_00481fda;
        if (DAT_00481fdb < DAT_00495c03) {
          bVar5 = DAT_00481fda < DAT_00495c02;
          uVar2 = (uint)DAT_00482393;
          (&DAT_00495af0)[uVar2] = DAT_00481fdb;
          if (bVar5) {
            *(byte *)((int)&DAT_00495ab0 + uVar2) = bVar3;
            DAT_00482348 = (uint)bVar3 + (uint)bVar1 * 0x20;
            if ((((&DAT_00495c70)[DAT_00482348] != '\0') &&
                ((bVar3 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf, bVar3 < 0xb ||
                 (0xd < bVar3)))) && (*(char *)((int)&DAT_00495820 + DAT_00482348) == -1)) {
              *(byte *)((int)&DAT_00495820 + DAT_00482348) = DAT_00495c63 | DAT_00482393;
              FUN_00423a80();
              bVar1 = DAT_00481fdb;
              bVar3 = DAT_00481fda;
              DAT_00481dbc = 0x80;
              DAT_00481fda = DAT_00482391;
              DAT_00481fdb = DAT_00482392;
              FUN_0041da80();
              DAT_00481fda = bVar3;
              DAT_00481fdb = bVar1;
              *(undefined1 *)((int)&DAT_00495820 + (uint)bVar3 + (uint)bVar1 * 0x20) = 0xff;
              if (DAT_00481df5 < DAT_0048239a) {
                DAT_0048239a = DAT_00481df5;
                DAT_0048239b = bVar3;
                DAT_0048239c = bVar1;
              }
            }
          }
        }
        FUN_004179b0(DAT_00482399);
        bVar1 = DAT_00482395;
        bVar3 = DAT_00482393;
        DAT_00482398 = DAT_00482398 - 1;
      } while (DAT_00482398 != 0);
      DAT_00482399 = DAT_00482399 + 1;
    } while (DAT_00482399 < 6);
    DAT_00482397 = DAT_00482397 + 1;
  } while (DAT_00482397 < 4);
  uVar2 = (uint)DAT_00482393;
  *(byte *)((int)&DAT_00495ab0 + uVar2) = DAT_00482394;
  (&DAT_00495af0)[uVar2] = bVar1;
  *(byte *)((int)&DAT_00495820 + (uint)DAT_00482394 + (uint)bVar1 * 0x20) = DAT_00495c63 | bVar3;
  FUN_00423a80();
  if (DAT_0048239a == 0xff) {
    DAT_00484710 = 0xff;
    *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482393) =
         *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482393) | 0x80;
    return;
  }
  iVar4 = 0;
  DAT_0048327b = DAT_00482393;
  if ((DAT_0048239b != DAT_00482394) || (DAT_00482395 != DAT_0048239c)) {
    DAT_00484711 = DAT_0048239b;
    DAT_00484712 = DAT_0048239c;
    DAT_00484710 = 0;
    iVar4 = 3;
  }
  (&DAT_00484710)[iVar4] = 0xff;
  *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482393) =
       *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482393) | 8;
  return;
}



/* ================= FUN_00407a70 @ 00407a70 ================= */

void FUN_00407a70(void)

{
  thunk_FUN_00417af0();
  DAT_00484710 = 0xff;
  return;
}



/* ================= FUN_00407b80 @ 00407b80 ================= */

void FUN_00407b80(void)

{
  DAT_0048208d = 2;
  return;
}



/* ================= FUN_00407c40 @ 00407c40 ================= */

void FUN_00407c40(void)

{
  int iVar1;
  
  if (*(short *)(DAT_00482330 + 0x16) == 0) {
    if ((DAT_00481de8 & 0x40) != 0) {
      FUN_00407cd0();
      return;
    }
    if ((DAT_00481de8 & 0x20) == 0) {
      return;
    }
  }
  else {
    *(short *)(DAT_00482330 + 0x16) = 0;
  }
  iVar1 = DAT_00482330;
  DAT_00482330 = DAT_00495b34;
  FUN_00404a00();
  DAT_00482330 = iVar1;
  FUN_00406a90();
  DAT_00495b3b = 0;
  if (*(short *)(DAT_00482330 + 0x28) == 1) {
    *(undefined2 *)(DAT_00482330 + 0x16) = 1;
    DAT_0048208d = *(undefined1 *)(DAT_00482330 + 0x16);
    FUN_00407dc0();
    return;
  }
  FUN_00407d40();
  return;
}



/* ================= FUN_00407cd0 @ 00407cd0 ================= */

void FUN_00407cd0(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00482330;
  DAT_00482330 = DAT_00495b34;
  FUN_00404a00();
  DAT_00495b3b = 0;
  DAT_00482330 = uVar1;
  FUN_00407d00();
  return;
}



/* ================= FUN_00407d00 @ 00407d00 ================= */

void FUN_00407d00(void)

{
  FUN_00412890(0xc);
  FUN_00407f30();
  return;
}



/* ================= FUN_00407d40 @ 00407d40 ================= */

void FUN_00407d40(void)

{
  FUN_00407ec0();
  DAT_0048208d = 1;
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_00407d60;
  return;
}



/* ================= FUN_00407dc0 @ 00407dc0 ================= */

void FUN_00407dc0(void)

{
  FUN_00407dd0(1);
  return;
}



/* ================= FUN_00407dd0 @ 00407dd0 ================= */

void __cdecl FUN_00407dd0(byte param_1)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  
  *(ushort *)(DAT_00482330 + 0x26) = (ushort)param_1;
  iVar2 = *(int *)(DAT_00482330 + 0x30);
  pbVar3 = (byte *)(iVar2 + 2);
  *(byte **)(DAT_00482330 + 0x30) = pbVar3;
  bVar1 = *pbVar3;
  *(ushort *)(DAT_00482330 + 0x2a) = (ushort)bVar1;
  if (bVar1 == 0) {
    FUN_00407f30();
    return;
  }
  bVar1 = *(byte *)(iVar2 + 3);
  *(ushort *)(DAT_00482330 + 0x28) = (ushort)bVar1;
  if (((bVar1 != 2) && (bVar1 != 5)) && (DAT_004822d5 = *pbVar3, DAT_004822d5 == 0xe1)) {
    DAT_004822d5 = 0xb1;
  }
  FUN_00407b80();
  *(undefined **)(DAT_00482330 + 8) = &DAT_00407e50;
  return;
}



/* ================= FUN_00407ec0 @ 00407ec0 ================= */

void FUN_00407ec0(void)

{
  byte bVar1;
  ushort uVar2;
  int iVar3;
  byte bVar4;
  
  bVar4 = 0;
  bVar1 = DAT_0042af30;
  if (DAT_0042af30 != 0) {
    while (bVar1 != DAT_004822d5) {
      bVar4 = bVar4 + 2;
      bVar1 = (&DAT_0042af30)[bVar4];
      if (bVar1 == 0) {
        return;
      }
    }
    iVar3 = (uint)DAT_004822d4 * 0x40;
    uVar2 = (ushort)DAT_004822d5;
    (&DAT_004823cc)[(uint)DAT_004822d4 * 0x20] = uVar2;
    *(ushort *)(DAT_00482330 + 0x2a) = uVar2;
    *(byte **)(&DAT_004823d0 + iVar3) = &DAT_0042af30 + bVar4;
    *(ushort *)(&DAT_004823b2 + iVar3) = (ushort)(byte)(&DAT_0042af31)[bVar4];
  }
  return;
}



/* ================= FUN_00407f30 @ 00407f30 ================= */

void FUN_00407f30(void)

{
  byte bVar1;
  uint uVar2;
  
  DAT_0048208f = 0xff;
  uVar2 = 0;
  do {
    if ((&DAT_004820d0)[uVar2] != '\0') {
      (&DAT_004820d0)[uVar2] = 0;
      FUN_0041c050((int)(&DAT_00482c30 + uVar2 * 6));
      FUN_00412e60(&DAT_00482c30 + uVar2 * 6);
    }
    bVar1 = (char)uVar2 + 1;
    uVar2 = (uint)bVar1;
  } while (bVar1 != 0x40);
  *(code **)(DAT_00482330 + 8) = FUN_00407f90;
  DAT_004822d7 = 0xfd;
  FUN_00407f90();
  return;
}



/* ================= FUN_00407f90 @ 00407f90 ================= */

void FUN_00407f90(void)

{
  DAT_004822d7 = DAT_004822d7 + '\x01';
  if (DAT_004822d7 == '\0') {
    DAT_00495c71 = 0xfe;
    *(undefined1 **)(DAT_00482330 + 8) = &LAB_0041ef30;
  }
  return;
}



/* ================= thunk_FUN_00404a00 @ 00408110 ================= */

void thunk_FUN_00404a00(void)

{
  FUN_00404a10(DAT_00482330);
  return;
}



/* ================= FUN_004081f0 @ 004081f0 ================= */

void FUN_004081f0(void)

{
  byte bVar1;
  
  bVar1 = 0;
  do {
    if ((0x40 < (byte)(&DAT_004820d0)[bVar1]) && ((byte)(&DAT_004820d0)[bVar1] < 0x65)) {
      (&DAT_004821d0)[bVar1] = 0xff;
    }
    bVar1 = bVar1 + 1;
  } while (bVar1 != 0x41);
  return;
}



/* ================= FUN_00408220 @ 00408220 ================= */

void FUN_00408220(void)

{
  FUN_00404940(&LAB_00408130,0x32);
  *(undefined2 *)(DAT_00482334 + 0x12) = 0;
  *(short *)(DAT_00482334 + 0x2e) = (short)(DAT_00482330 + -0x4823a0 >> 6);
  return;
}



/* ================= FUN_00408260 @ 00408260 ================= */

void __cdecl FUN_00408260(byte param_1,byte param_2)

{
  FUN_00408280(0x53,param_1,param_2);
  return;
}



/* ================= FUN_00408280 @ 00408280 ================= */

void __cdecl FUN_00408280(byte param_1,byte param_2,byte param_3)

{
  byte bVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  uint uVar4;
  byte local_1;
  
  local_1 = 0;
  do {
    uVar4 = (uint)local_1;
    bVar1 = (&DAT_004820d0)[uVar4];
    if (0x3f < bVar1) {
      if (bVar1 < param_1) {
        uVar3 = FUN_00408370(bVar1,param_2,param_3,local_1);
        uVar2 = (undefined1)uVar3;
      }
      else {
        if (100 < bVar1) goto LAB_004082d1;
        uVar2 = 0;
        (&DAT_004821d0)[uVar4] = 0;
      }
      (&DAT_00482c39)[uVar4 * 0x18] = uVar2;
    }
LAB_004082d1:
    local_1 = local_1 + 1;
    if (local_1 == 0x41) {
      return;
    }
  } while( true );
}



/* ================= FUN_004082f0 @ 004082f0 ================= */

void __cdecl FUN_004082f0(byte *param_1,byte param_2,byte param_3)

{
  byte bVar1;
  undefined4 uVar2;
  byte bVar3;
  uint uVar4;
  byte *pbVar5;
  byte local_1;
  
  local_1 = 0;
  bVar3 = *param_1;
  pbVar5 = param_1 + 1;
  do {
    uVar4 = (uint)local_1;
    bVar1 = (&DAT_004820d0)[uVar4];
    if (0x3f < bVar1) {
      if (bVar1 == bVar3) {
        uVar2 = FUN_00408370(bVar1,param_2,param_3,local_1);
        (&DAT_00482c39)[uVar4 * 0x18] = (char)uVar2;
        bVar3 = *pbVar5;
        pbVar5 = pbVar5 + 1;
      }
      else if (bVar1 < 0x65) {
        (&DAT_004821d0)[uVar4] = 0;
        (&DAT_00482c39)[uVar4 * 0x18] = 0;
      }
    }
    local_1 = local_1 + 1;
  } while (local_1 != 0x41);
  return;
}



/* ================= FUN_00408370 @ 00408370 ================= */

undefined4 __cdecl FUN_00408370(char param_1,byte param_2,byte param_3,byte param_4)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  
  if ((DAT_00482088 == '\0') || ((DAT_004834bc & 0x1f) < 0x10)) {
    bVar2 = param_1 * '\x02' + 0x80;
    iVar1 = (uint)param_4 * 0x18;
    bVar3 = (&DAT_0042af90)[bVar2];
    (&DAT_004821d0)[param_4] = 0xff;
    *(ushort *)(&DAT_00482c3c + iVar1) = (ushort)bVar3 + (ushort)param_2 & 0xff;
    *(ushort *)(&DAT_00482c40 + iVar1) =
         (ushort)(byte)(&DAT_0042af91)[bVar2] + (ushort)param_3 & 0xff;
    return 0xb5;
  }
  (&DAT_004821d0)[param_4] = 0;
  return 0;
}



/* ================= caseD_0 @ 00408400 ================= */

void switchD_00408716::caseD_0(void)

{
  uint uVar1;
  byte bVar2;
  
  bVar2 = 0;
  do {
    uVar1 = (uint)bVar2;
    if ((&DAT_004821d0)[uVar1] == -1) {
      (&DAT_004821d0)[uVar1] = 0;
      (&DAT_00482c39)[uVar1 * 0x18] = 0;
    }
    bVar2 = bVar2 + 1;
  } while (bVar2 != 0x41);
  return;
}



/* ================= FUN_004086f0 @ 004086f0 ================= */

void __cdecl FUN_004086f0(byte param_1)

{
  byte bVar1;
  uint uVar2;
  
  DAT_00482088 = false;
  if (param_1 != 0) {
    DAT_00482088 = 0x7f < param_1;
    if ((bool)DAT_00482088) {
      param_1 = param_1 & 0x7f;
    }
    if (0x17 < param_1) {
      param_1 = 0;
    }
  }
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
  switch(param_1) {
  case 0:
    uVar2 = 0;
    break;
  case 1:
    FUN_00408260(0x70,0x80);
    return;
  case 2:
    FUN_004082f0(&DAT_0042afdc,0x70,0x80);
    return;
  case 3:
    FUN_00408260(0x6c,0x9a);
    return;
  case 4:
    FUN_004082f0(&DAT_0042afe0,0x6c,0x9a);
    return;
  case 5:
    FUN_004082f0((byte *)s__ABCDEFGHIJKLMNOSTUVWX_0042afe8,0x7a,0xaf);
    return;
  case 6:
    FUN_004082f0(&DAT_0042b000,0x8e,0xaf);
    return;
  case 7:
    FUN_004082f0(&DAT_0042b008,0x70,0x9a);
    return;
  case 8:
    FUN_00408280(0x47,0x70,0xa0);
    return;
  case 9:
    FUN_00408280(0x42,0x4f,0x9f);
    return;
  case 10:
    FUN_004082f0(&DAT_0042b010,0x6c,0x9a);
    return;
  case 0xb:
    FUN_004082f0(&DAT_0042b018,0x60,0x95);
    return;
  case 0xc:
    FUN_004082f0(&DAT_0042b020,0x60,0x95);
    return;
  case 0xd:
    FUN_00408280(0x41,0x60,0x9e);
    return;
  case 0xe:
    FUN_004082f0(&DAT_0042b024,0x62,0x9f);
    return;
  case 0xf:
    FUN_00408280(0x47,0x58,0xa4);
    return;
  case 0x10:
    FUN_00408260(0x86,0xb1);
    return;
  case 0x11:
    FUN_004082f0(&DAT_0042afe0,0x86,0xb1);
    return;
  case 0x12:
    FUN_00408280(0x41,0x58,0x90);
    return;
  case 0x13:
    FUN_00408280(0x41,0x76,0x8b);
    return;
  case 0x14:
    FUN_004082f0(&DAT_0042b028,0x4e,0x88);
    return;
  case 0x15:
    FUN_00408280(0x41,0x62,0xa6);
    return;
  case 0x16:
    FUN_00408280(0x47,0x60,0x80);
    return;
  case 0x17:
    FUN_00408280(0x47,0x62,0x95);
    return;
  }
  do {
    if ((&DAT_004821d0)[uVar2] == -1) {
      (&DAT_004821d0)[uVar2] = 0;
      (&DAT_00482c39)[uVar2 * 0x18] = 0;
    }
    bVar1 = (char)uVar2 + 1;
    uVar2 = (uint)bVar1;
  } while (bVar1 != 0x41);
  return;
}



/* ================= FUN_00408750 @ 00408750 ================= */

void __cdecl FUN_00408750(undefined1 *param_1)

{
  *(undefined1 **)(DAT_00482330 + 0x34) = param_1;
  *(undefined4 *)(DAT_00482330 + 0x38) = *(undefined4 *)(DAT_00482330 + 0x34);
  DAT_00482089 = *param_1;
  DAT_0048208a = param_1[1];
  *(undefined2 *)(DAT_00482330 + 0x1c) = 0;
  *(undefined2 *)(DAT_00482330 + 0x22) = 0x30;
  FUN_004094f0();
  *(undefined **)(DAT_00482330 + 8) = &DAT_00408b30;
  return;
}



/* ================= FUN_004087e0 @ 004087e0 ================= */

void __cdecl FUN_004087e0(undefined1 *param_1)

{
  int iVar1;
  
  *(undefined1 **)(DAT_00482330 + 0x38) = param_1;
  DAT_00482089 = *param_1;
  DAT_0048208a = param_1[1];
  *(undefined2 *)(DAT_00482330 + 0x1c) = 0;
  FUN_004094f0();
  DAT_0048208f = 0xfe;
  FUN_00409510();
  DAT_0048208f = -0xf;
  *(undefined4 *)(DAT_00482330 + 8) = 0x408840;
  if (DAT_0048208f == -1) {
    thunk_FUN_00404a00();
    return;
  }
  iVar1 = FUN_004094d0();
  if (iVar1 == 0) {
    FUN_00408c60();
    return;
  }
  return;
}



/* ================= FUN_00408950 @ 00408950 ================= */

void FUN_00408950(void)

{
  FUN_00408bf0();
  FUN_004094f0();
  FUN_00408b50();
  return;
}



/* ================= FUN_00408ad0 @ 00408ad0 ================= */

void FUN_00408ad0(void)

{
  FUN_00408ae0();
  FUN_00408c10();
  return;
}



/* ================= FUN_00408ae0 @ 00408ae0 ================= */

void FUN_00408ae0(void)

{
  short *psVar1;
  
  if (DAT_0048208f == -1) {
    thunk_FUN_00404a00();
    return;
  }
  DAT_0048208f = 0xfe;
  psVar1 = (short *)(DAT_00482330 + 0x1a);
  *(undefined2 *)(&DAT_00482c3c + *(short *)(DAT_00482330 + 0x1a) * 0x18) = 0x58;
  *(undefined2 *)(&DAT_00482c40 + *psVar1 * 0x18) = 0xab;
  FUN_00409510();
  return;
}



/* ================= FUN_00408b50 @ 00408b50 ================= */

void FUN_00408b50(void)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  
  if (DAT_0048208f != -2) {
    if (DAT_0048208f == -1) {
      thunk_FUN_00404a00();
      return;
    }
    iVar2 = FUN_004094d0();
    if (iVar2 != 0) {
      while( true ) {
        if (*(short *)(DAT_00482330 + 0x22) != 0) {
          *(short *)(DAT_00482330 + 0x22) = *(short *)(DAT_00482330 + 0x22) + -1;
          return;
        }
        if (DAT_0048208f == -1) break;
        piVar3 = (int *)(DAT_00482330 + 0x38);
        cVar1 = **(char **)(DAT_00482330 + 0x38);
        if (cVar1 != '\0') {
          DAT_0048208a = (*(char **)(DAT_00482330 + 0x38))[1];
          DAT_00482089 = cVar1;
          *piVar3 = *piVar3 + 2;
          *(short *)(DAT_00482330 + 0x1c) = *(short *)(DAT_00482330 + 0x1c) + 1;
          FUN_00408bf0();
          return;
        }
        *piVar3 = *(int *)(DAT_00482330 + 0x34);
      }
      thunk_FUN_00404a00();
      return;
    }
  }
  FUN_00408c10();
  return;
}



/* ================= FUN_00408bf0 @ 00408bf0 ================= */

void FUN_00408bf0(void)

{
  DAT_0048208f = 0xf1;
  *(undefined2 *)(DAT_00482330 + 0x22) = 0x30;
  return;
}



/* ================= FUN_00408c10 @ 00408c10 ================= */

void FUN_00408c10(void)

{
  if ((*(byte *)(DAT_00482330 + 0x1a) != 0) && (*(byte *)(DAT_00482330 + 0x1a) < 0xef)) {
    *(undefined **)(DAT_00482330 + 0x38) = &DAT_0042af68;
  }
  DAT_0048208f = 0xfe;
  FUN_00409510();
  *(undefined2 *)(DAT_00482330 + 0x22) = 0x30;
  DAT_00482089 = **(undefined1 **)(DAT_00482330 + 0x38);
  DAT_0048208a = (*(undefined1 **)(DAT_00482330 + 0x38))[1];
  FUN_00408c60();
  return;
}



/* ================= FUN_00408c60 @ 00408c60 ================= */

void FUN_00408c60(void)

{
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_00408050;
  FUN_00407ec0();
  return;
}



/* ================= FUN_00408f10 @ 00408f10 ================= */

void FUN_00408f10(void)

{
  FUN_00408f30();
  *(undefined2 *)(DAT_00482330 + 0x1a) = 0xf0;
  FUN_00408c10();
  return;
}



/* ================= FUN_00408f30 @ 00408f30 ================= */

void FUN_00408f30(void)

{
  char cVar1;
  uint uVar2;
  byte bVar3;
  
  bVar3 = 0;
  do {
    uVar2 = (uint)bVar3;
    cVar1 = (&DAT_004820d0)[uVar2];
    if (cVar1 == '\x10') {
      *(undefined2 *)(&DAT_00482c3c + uVar2 * 0x18) = 0x4e;
      *(undefined2 *)(&DAT_00482c40 + uVar2 * 0x18) = 0x87;
LAB_00408f70:
      (&DAT_004821d0)[uVar2] = 1;
    }
    else if ((cVar1 == ' ') || (cVar1 == '!')) goto LAB_00408f70;
    bVar3 = bVar3 + 1;
    if (bVar3 == 0x41) {
      return;
    }
  } while( true );
}



/* ================= FUN_00408fc0 @ 00408fc0 ================= */

void FUN_00408fc0(void)

{
  byte bVar1;
  undefined2 uVar2;
  uint uVar3;
  
  FUN_00409f00();
  if (DAT_0048208f == -1) {
    thunk_FUN_00404a00();
    return;
  }
  uVar3 = 0;
  DAT_0048208f = 0xf2;
  do {
    uVar2 = (undefined2)uVar3;
    if ((&DAT_004820d0)[uVar3] == '\x10') goto LAB_00408ff5;
    bVar1 = (char)uVar3 + 1;
    uVar3 = (uint)bVar1;
  } while (bVar1 != 0x41);
  FUN_00401090();
  uVar2 = 0x41;
LAB_00408ff5:
  *(undefined2 *)(DAT_00482330 + 0x22) = uVar2;
  *(undefined2 *)(DAT_00482330 + 0x20) = 0;
  *(undefined2 *)(DAT_00482330 + 0x14) = 0;
  FUN_00409030();
  return;
}



/* ================= FUN_00409030 @ 00409030 ================= */

void FUN_00409030(void)

{
  byte *pbVar1;
  
  DAT_00482086 = 1;
  DAT_0048208e = 0;
  *(undefined2 *)(DAT_00482330 + 0x16) = 0;
  pbVar1 = (&PTR_DAT_0042b0d0)[*(short *)(DAT_00482330 + 0x20) * 2];
  *(byte **)(DAT_00482330 + 0x34) = pbVar1;
  *(undefined **)(DAT_00482330 + 0x38) = (&PTR_DAT_0042b0d4)[*(short *)(DAT_00482330 + 0x20) * 2];
  *(ushort *)(DAT_00482330 + 0x10) = (ushort)*pbVar1;
  *(ushort *)(DAT_00482330 + 0x12) = (ushort)pbVar1[1];
  *(code **)(DAT_00482330 + 8) = FUN_004090b0;
  DAT_004822d6 = 1;
  FUN_004090b0();
  return;
}



/* ================= FUN_004090b0 @ 004090b0 ================= */

void FUN_004090b0(void)

{
  byte *pbVar1;
  byte bVar2;
  int iVar3;
  
  if (DAT_0048208f == 0xff) {
    thunk_FUN_00404a00();
    return;
  }
  iVar3 = FUN_004094d0();
  if (iVar3 == 0) {
    FUN_00409380();
    return;
  }
  if (*(short *)(DAT_00482330 + 0x16) == 0) {
    FUN_00409c50();
  }
  else {
    FUN_00409cb0();
  }
  if (DAT_0048208f == 10) {
    if (*(short *)(DAT_00482330 + 0x14) == 0) goto LAB_00409106;
LAB_00409132:
    if (0x7f < *(short *)(DAT_00482330 + 0x10)) {
      DAT_00482086 = 0;
      DAT_0048208e = 1;
    }
    FUN_00409a90();
    if (DAT_0048208f == 0) {
      FUN_00409250();
      return;
    }
    if (((byte)*(undefined2 *)(DAT_00482330 + 0x10) & 0xf) != 5) {
      return;
    }
  }
  else {
    if (DAT_0048208f != 0xc) {
      return;
    }
LAB_00409106:
    pbVar1 = (byte *)(DAT_00482330 + 0x10);
    DAT_0048208f = *pbVar1 & 0xf;
    if (DAT_0048208f != 5) {
      *(undefined2 *)(DAT_00482330 + 0x14) = 1;
      FUN_00409ed0(*pbVar1 & 0xf0);
      goto LAB_00409132;
    }
  }
  if (DAT_0048208f != 10) {
    return;
  }
  bVar2 = *(byte *)(DAT_00482330 + 0x12);
  if (bVar2 == 0) {
    FUN_00409250();
    return;
  }
  if (bVar2 == 2) {
    FUN_004081f0();
  }
  else {
    if (bVar2 < 2) {
      DAT_0048208e = 1;
      DAT_00482086 = 0;
      FUN_00409ed0((byte)*(undefined2 *)(DAT_00482330 + 0x10) & 0xf0);
      DAT_0048208f = 0;
      FUN_00409250();
      return;
    }
    if (bVar2 != 4) {
      if (bVar2 < 4) {
        FUN_004086f0(0x14);
        FUN_004093a0();
        return;
      }
      if ((bVar2 == 6) || (5 < bVar2)) {
        FUN_004086f0(0);
      }
      else {
        FUN_00409c10();
        *(short *)(DAT_00482330 + 0x16) = *(short *)(DAT_00482330 + 0x16) + 1;
        FUN_00409cb0();
      }
      DAT_0048208f = 0xb;
      goto LAB_0040922a;
    }
    FUN_004086f0(0x15);
  }
  if (DAT_0048208f == 0xff) {
    thunk_FUN_00404a00();
    return;
  }
  DAT_0048208f = 0;
LAB_0040922a:
  DAT_0048208e = 0;
  DAT_00482086 = 1;
  FUN_00409510();
  FUN_00409250();
  return;
}



/* ================= FUN_00409250 @ 00409250 ================= */

void FUN_00409250(void)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = *(int *)(DAT_00482330 + 0x34);
  *(int *)(DAT_00482330 + 0x34) = iVar2 + 2;
  bVar1 = *(byte *)(iVar2 + 2);
  *(ushort *)(DAT_00482330 + 0x10) = (ushort)bVar1;
  if (bVar1 == 0) {
    FUN_00409eb0();
    *(undefined2 *)(DAT_00482330 + 0x12) = 0x60;
    *(undefined1 **)(DAT_00482330 + 8) = &LAB_004092b0;
    return;
  }
  *(ushort *)(DAT_00482330 + 0x12) = (ushort)*(byte *)(iVar2 + 3);
  return;
}



/* ================= FUN_00409310 @ 00409310 ================= */

void FUN_00409310(void)

{
  int iVar1;
  short *psVar2;
  uint uVar3;
  
  psVar2 = (short *)(DAT_00482330 + 0x22);
  *(undefined2 *)(&DAT_00482c3c + *(short *)(DAT_00482330 + 0x22) * 0x18) = 0x4e;
  *(undefined2 *)(&DAT_00482c40 + *psVar2 * 0x18) = 0x87;
  uVar3 = (uint)DAT_00482087;
  (&DAT_00482150)[uVar3] = 0;
  iVar1 = uVar3 * 0x18;
  *(undefined2 *)(&DAT_00482c3c + iVar1) = 0x30;
  *(undefined2 *)(&DAT_00482c40 + iVar1) = 0x5b;
  return;
}



/* ================= FUN_00409370 @ 00409370 ================= */

void FUN_00409370(void)

{
  FUN_00409c70();
  FUN_00409030();
  return;
}



/* ================= FUN_00409380 @ 00409380 ================= */

void FUN_00409380(void)

{
  DAT_0048208f = 0xf2;
  FUN_004094f0();
  FUN_00409310();
  FUN_004086f0(0);
  FUN_00408c60();
  return;
}



/* ================= FUN_004093a0 @ 004093a0 ================= */

void FUN_004093a0(void)

{
  undefined4 *puVar1;
  byte bVar2;
  
  puVar1 = (undefined4 *)(DAT_00482330 + 0x38);
  bVar2 = *(byte *)*puVar1;
  *(ushort *)(DAT_00482330 + 0x10) = (ushort)bVar2;
  if (bVar2 == 0) {
    DAT_0048208e = 0;
    DAT_0048208f = 0;
    *(undefined2 *)(DAT_00482330 + 0x14) = 1;
    *(code **)(DAT_00482330 + 8) = FUN_004090b0;
    FUN_00409250();
    return;
  }
  *(ushort *)(DAT_00482330 + 0x12) = (ushort)((byte *)*puVar1)[1];
  *(int *)(DAT_00482330 + 0x38) = *(int *)(DAT_00482330 + 0x38) + 2;
  *(undefined2 *)(DAT_00482330 + 0x14) = 2;
  *(code **)(DAT_00482330 + 8) = FUN_00409430;
  FUN_00409430();
  return;
}



/* ================= FUN_00409430 @ 00409430 ================= */

void FUN_00409430(void)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  short sVar5;
  
  if (DAT_0048208f == -1) {
    thunk_FUN_00404a00();
    return;
  }
  iVar3 = FUN_004094d0();
  if (iVar3 == 0) {
    FUN_00409380();
    return;
  }
  bVar1 = *(byte *)(DAT_00482330 + 0x10);
  if (bVar1 != 0) {
    if (bVar1 == 2) {
      sVar2 = 0;
      sVar5 = 2;
    }
    else if (bVar1 < 2) {
      sVar2 = 2;
      sVar5 = 1;
    }
    else {
      sVar2 = -2;
      sVar5 = -1;
    }
    psVar4 = (short *)(DAT_00482330 + 0x22);
    *(short *)(&DAT_00482c3c + *(short *)(DAT_00482330 + 0x22) * 0x18) =
         *(short *)(&DAT_00482c3c + *(short *)(DAT_00482330 + 0x22) * 0x18) + sVar2;
    iVar3 = DAT_00482330;
    *(short *)(&DAT_00482c40 + *psVar4 * 0x18) = *(short *)(&DAT_00482c40 + *psVar4 * 0x18) + sVar5;
    *(short *)(iVar3 + 0x12) = *(short *)(iVar3 + 0x12) + -1;
    if (*(short *)(DAT_00482330 + 0x12) != 0) {
      return;
    }
  }
  FUN_004093a0();
  return;
}



/* ================= FUN_004094d0 @ 004094d0 ================= */

int FUN_004094d0(void)

{
  return (short)(&DAT_004823c6)[*(short *)(DAT_00482330 + 0x2e) * 0x20] + -3;
}



/* ================= FUN_004094f0 @ 004094f0 ================= */

void FUN_004094f0(void)

{
  (&DAT_004823c6)[*(short *)(DAT_00482330 + 0x2e) * 0x20] = 0;
  return;
}



/* ================= FUN_00409510 @ 00409510 ================= */

void FUN_00409510(void)

{
  byte bVar1;
  uint uVar2;
  byte bVar3;
  
  bVar3 = 0;
  do {
    uVar2 = (uint)bVar3;
    bVar1 = (&DAT_00482090)[uVar2];
    if (bVar1 != 0) {
      if (bVar1 == 0xb5) {
        FUN_00409620(bVar3);
      }
      else if (bVar1 == 0xb6) {
        FUN_00409590();
        bVar3 = DAT_00482087;
      }
      else if (bVar1 == 0xb9) {
        FUN_004095d0(bVar3);
      }
      else if ((((bVar1 == 0xce) || (bVar1 == 0xcf)) || (bVar1 == 0xd1)) || (bVar1 == 0xd2)) {
        (&DAT_00482c39)[uVar2 * 0x18] = ((&DAT_004821d0)[uVar2] == '\0') - 1U & bVar1;
      }
    }
    bVar3 = bVar3 + 1;
  } while (bVar3 != 0x41);
  return;
}



/* ================= FUN_00409590 @ 00409590 ================= */

void FUN_00409590(void)

{
  if ((DAT_0048208e == '\0') && ((DAT_004834bc & 0x1f) < 0x10)) {
    (&DAT_00482c39)[(uint)DAT_00482087 * 0x18] = (&DAT_00482090)[DAT_00482087];
    return;
  }
  (&DAT_00482c39)[(uint)DAT_00482087 * 0x18] = 0;
  return;
}



/* ================= FUN_004095d0 @ 004095d0 ================= */

void __cdecl FUN_004095d0(byte param_1)

{
  if (DAT_0048208d == '\x01') {
    if ((DAT_004834bc & 0x1f) < 0x10) {
      (&DAT_00482c39)[(uint)param_1 * 0x18] = (&DAT_00482090)[param_1];
      return;
    }
  }
  else {
    if (DAT_0048208d != '\x02') {
      return;
    }
    DAT_0048208d = '\0';
  }
  (&DAT_00482c39)[(uint)param_1 * 0x18] = 0;
  return;
}



/* ================= FUN_00409620 @ 00409620 ================= */

void __cdecl FUN_00409620(byte param_1)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  if ((&DAT_004821d0)[uVar1] == -1) {
    (&DAT_00482c39)[uVar1 * 0x18] = (&DAT_00482090)[uVar1];
    return;
  }
  (&DAT_00482c39)[uVar1 * 0x18] = 0;
  return;
}



/* ================= FUN_00409650 @ 00409650 ================= */

void __cdecl FUN_00409650(byte param_1,byte param_2)

{
  uint uVar1;
  
  uVar1 = DAT_00482330 + -0x4823a0 >> 6 & 0xff;
  (&DAT_00482250)[uVar1] = param_1;
  (&DAT_00482290)[uVar1] = param_2;
  FUN_004097a0(0x16,0xb5,(char)*(undefined2 *)(DAT_00482330 + 0x22),param_1,param_2);
  return;
}



/* ================= FUN_004096a0 @ 004096a0 ================= */

void FUN_004096a0(void)

{
  DAT_00482086 = 0;
  FUN_00409700(&DAT_0042b100,'\0');
  (&DAT_00482150)[DAT_00482087] = 0;
  return;
}



/* ================= FUN_004096d0 @ 004096d0 ================= */

void FUN_004096d0(void)

{
  FUN_00409700(&DAT_0042b0f8,'\0');
  return;
}



/* ================= FUN_004096e0 @ 004096e0 ================= */

void FUN_004096e0(void)

{
  FUN_004096d0();
  FUN_004096a0();
  FUN_00409700(&DAT_0042b0e0,'\x03');
  return;
}



/* ================= FUN_00409700 @ 00409700 ================= */

void __cdecl FUN_00409700(byte *param_1,char param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte bVar5;
  
  do {
    pbVar1 = param_1 + 4;
    pbVar2 = param_1 + 3;
    pbVar3 = param_1 + 2;
    pbVar4 = param_1 + 1;
    bVar5 = *param_1;
    param_1 = param_1 + 5;
    param_2 = param_2 + -1;
    FUN_004097a0(bVar5,*pbVar4,*pbVar3,*pbVar2,*pbVar1);
  } while (param_2 != -1);
  return;
}



/* ================= FUN_00409740 @ 00409740 ================= */

void FUN_00409740(void)

{
  FUN_004048e0(&LAB_00409860,10);
  FUN_0041c020(DAT_00482c20,0x1d);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0;
  *(undefined1 *)(DAT_00482c20 + 9) = 0;
  *(undefined1 *)(DAT_00482c20 + 10) = 0;
  DAT_0048208f = 0xfe;
  return;
}



/* ================= FUN_004097a0 @ 004097a0 ================= */

void __cdecl
FUN_004097a0(byte param_1,undefined1 param_2,undefined1 param_3,byte param_4,byte param_5)

{
  int iVar1;
  byte bVar2;
  
  bVar2 = FUN_00412dc0(0x32);
  if (bVar2 != 0xff) {
    FUN_0041c020(DAT_00482c20,param_1);
    iVar1 = DAT_00482c20;
    (&DAT_00482090)[bVar2] = param_2;
    *(undefined1 *)(iVar1 + 9) = 0;
    *(undefined1 *)(DAT_00482c20 + 10) = 0;
    iVar1 = DAT_00482c20;
    (&DAT_004820d0)[bVar2] = param_3;
    *(ushort *)(iVar1 + 0xc) = (ushort)param_4;
    *(ushort *)(DAT_00482c20 + 0x10) = (ushort)param_5;
    (&DAT_004821d0)[bVar2] = 0;
    if ((&DAT_00482090)[bVar2] != -0x4b) {
      (&DAT_004821d0)[bVar2] = 1;
    }
    if ((&DAT_00482090)[bVar2] == -0x4a) {
      DAT_00482087 = bVar2;
    }
    if ((&DAT_004820d0)[bVar2] == 'p') {
      *(short *)(DAT_00482c20 + 0xc) = *(short *)(DAT_00482c20 + 0xc) + 0x100;
      *(short *)(DAT_00482c20 + 0x10) = *(short *)(DAT_00482c20 + 0x10) + 0x100;
    }
  }
  return;
}



/* ================= FUN_00409a90 @ 00409a90 ================= */

void FUN_00409a90(void)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  
  bVar2 = DAT_00482087;
  if ((DAT_004834bc & 7) != 0) {
    return;
  }
  if (DAT_0048208f != '\n') {
    FUN_00409590();
    return;
  }
  if (DAT_00482086 == '\0') {
    DAT_0048208f = '\0';
    goto LAB_00409bad;
  }
  bVar4 = (byte)*(ushort *)(DAT_00482330 + 0x10);
  bVar3 = bVar4 & 0xf;
  if ((*(ushort *)(DAT_00482330 + 0x10) & 0xf) == 0) {
    return;
  }
  if (bVar3 == 2) {
    DAT_0048233c = 0;
    DAT_00482338 = 10;
  }
  else {
    if ((bVar4 & 0xf) < 2) {
      DAT_0048233c = 10;
    }
    else {
      if (bVar3 != 4) {
        if (3 < bVar3) {
          return;
        }
        DAT_0048233c = 0;
        DAT_00482338 = 0xfff6;
        goto LAB_00409b4d;
      }
      DAT_0048233c = 0xfff6;
    }
    bVar4 = (&DAT_00482150)[DAT_00482087] + 1;
    (&DAT_00482150)[DAT_00482087] = bVar4;
    if ((bVar4 & 1) == 0) {
      DAT_00482338 = 0xfffb;
    }
    else {
      DAT_00482338 = 5;
    }
  }
LAB_00409b4d:
  iVar1 = (uint)bVar2 * 0x18;
  *(short *)(&DAT_00482c3c + iVar1) = *(short *)(&DAT_00482c3c + iVar1) + (short)DAT_0048233c;
  *(short *)(&DAT_00482c40 + iVar1) = *(short *)(&DAT_00482c40 + iVar1) + (short)DAT_00482338;
  *(short *)(DAT_00482330 + 0x12) = *(short *)(DAT_00482330 + 0x12) + -1;
  if (*(short *)(DAT_00482330 + 0x12) == 0) {
    DAT_0048208f = '\0';
  }
  if (DAT_00482086 != '\0') {
    (&DAT_00482c39)[iVar1] = 0xb6;
    return;
  }
LAB_00409bad:
  (&DAT_00482c39)[(uint)bVar2 * 0x18] = 0;
  return;
}



/* ================= FUN_00409bd0 @ 00409bd0 ================= */

void __cdecl FUN_00409bd0(undefined2 param_1,byte *param_2)

{
  byte bVar1;
  char cVar2;
  
  cVar2 = '\x04';
  FUN_0040da00(param_1);
  do {
    bVar1 = *param_2;
    param_2 = param_2 + 1;
    FUN_0040daf0(bVar1 + 0x6600);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ================= FUN_00409c10 @ 00409c10 ================= */

void FUN_00409c10(void)

{
  FUN_00409bd0(0x353,&DAT_0042b10c);
  return;
}



/* ================= FUN_00409c30 @ 00409c30 ================= */

void FUN_00409c30(void)

{
  FUN_00409bd0(0x353,&DAT_0042b108);
  return;
}



/* ================= FUN_00409c50 @ 00409c50 ================= */

void FUN_00409c50(void)

{
  if ((DAT_004834bc & 0x1f) == 0) {
    FUN_00409c10();
    return;
  }
  if ((DAT_004834bc & 0x1f) == 0xf) {
    FUN_00409c30();
    return;
  }
  return;
}



/* ================= FUN_00409c70 @ 00409c70 ================= */

void FUN_00409c70(void)

{
  FUN_00409bd0(0x3d3,&DAT_0042b110);
  return;
}



/* ================= FUN_00409c90 @ 00409c90 ================= */

void FUN_00409c90(void)

{
  FUN_00409bd0(0x3d3,&DAT_0042b108);
  return;
}



/* ================= FUN_00409cb0 @ 00409cb0 ================= */

void FUN_00409cb0(void)

{
  if ((DAT_004834bc & 0x1f) == 0) {
    FUN_00409c70();
    return;
  }
  if ((DAT_004834bc & 0x1f) == 0xf) {
    FUN_00409c90();
    return;
  }
  return;
}



/* ================= FUN_00409cd0 @ 00409cd0 ================= */

void __cdecl FUN_00409cd0(byte param_1,undefined2 param_2,char param_3)

{
  char cVar1;
  ushort uVar2;
  
  if (param_3 != '\0') {
    if ((param_3 == '\x01') || ((DAT_004834bc & 0x1f) == 4)) {
      uVar2 = param_1 | 0x6600;
      goto LAB_00409cf8;
    }
    if ((DAT_004834bc & 0x1f) != 0x13) {
      return;
    }
  }
  uVar2 = param_1 | 0x5600;
LAB_00409cf8:
  cVar1 = '\x04';
  FUN_0040da00(param_2);
  do {
    FUN_0040daf0(uVar2);
    cVar1 = cVar1 + -1;
    uVar2 = uVar2 + 1;
  } while (cVar1 != '\0');
  return;
}



/* ================= FUN_00409d30 @ 00409d30 ================= */

void FUN_00409d30(void)

{
  FUN_00409cd0(0x58,0xd3,'\x01');
  return;
}



/* ================= FUN_00409d50 @ 00409d50 ================= */

void FUN_00409d50(void)

{
  FUN_00409cd0(0x58,0xd3,'\0');
  return;
}



/* ================= FUN_00409d90 @ 00409d90 ================= */

void FUN_00409d90(void)

{
  FUN_00409cd0(0x5c,0x113,'\x01');
  return;
}



/* ================= FUN_00409db0 @ 00409db0 ================= */

void FUN_00409db0(void)

{
  FUN_00409cd0(0x5c,0x113,'\0');
  return;
}



/* ================= FUN_00409df0 @ 00409df0 ================= */

void FUN_00409df0(void)

{
  FUN_00409cd0(0x60,0x153,'\x01');
  return;
}



/* ================= FUN_00409e10 @ 00409e10 ================= */

void FUN_00409e10(void)

{
  FUN_00409cd0(0x60,0x153,'\0');
  return;
}



/* ================= FUN_00409e50 @ 00409e50 ================= */

void FUN_00409e50(void)

{
  FUN_00409cd0(100,0x193,'\x01');
  return;
}



/* ================= FUN_00409e70 @ 00409e70 ================= */

void FUN_00409e70(void)

{
  FUN_00409cd0(100,0x193,'\0');
  return;
}



/* ================= FUN_00409eb0 @ 00409eb0 ================= */

void FUN_00409eb0(void)

{
  FUN_00409d50();
  FUN_00409db0();
  FUN_00409e10();
  FUN_00409e70();
  return;
}



/* ================= FUN_00409ed0 @ 00409ed0 ================= */

void __cdecl FUN_00409ed0(byte param_1)

{
  FUN_00409eb0();
  if (param_1 < 0xf) {
    return;
  }
  if (param_1 == 0x20) {
    FUN_00409d90();
    return;
  }
  if (param_1 < 0x20) {
    FUN_00409d30();
    return;
  }
  if (param_1 == 0x40) {
    FUN_00409df0();
    return;
  }
  FUN_00409e50();
  return;
}



/* ================= FUN_00409f00 @ 00409f00 ================= */

void FUN_00409f00(void)

{
  char cVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  char local_1;
  
  uVar3 = 0x46c5;
  sVar4 = 0x2d8;
  FUN_0040da00(0x2d8);
  local_1 = '\x04';
  while( true ) {
    cVar1 = '\a';
    uVar2 = uVar3;
    do {
      uVar3 = uVar2 + 1;
      FUN_0040daf0(uVar2);
      cVar1 = cVar1 + -1;
      uVar2 = uVar3;
    } while (cVar1 != '\0');
    local_1 = local_1 + -1;
    if (local_1 == '\0') break;
    sVar4 = sVar4 + 0x40;
    FUN_0040da00(sVar4);
  }
  cVar1 = '\x04';
  FUN_0040da00(0x29a);
  uVar3 = 0x46e1;
  do {
    FUN_0040daf0(uVar3);
    cVar1 = cVar1 + -1;
    uVar3 = uVar3 + 1;
  } while (cVar1 != '\0');
  return;
}



/* ================= FUN_00409f80 @ 00409f80 ================= */

void FUN_00409f80(void)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  byte local_1;
  
  puVar3 = &DAT_00495820;
  for (iVar1 = 0xa0; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = 0xffffffff;
    puVar3 = puVar3 + 1;
  }
  if (DAT_0042a4e0 == '\0') {
    puVar3 = &DAT_004957a0;
    for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 1;
    }
    puVar3 = &DAT_004957e0;
    for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 1;
    }
    puVar3 = &DAT_00495ab0;
    for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 1;
    }
    puVar3 = (undefined4 *)&DAT_00495af0;
    for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = 0xffffffff;
      puVar3 = puVar3 + 1;
    }
    FUN_0040a4c0();
    FUN_0040a570();
    return;
  }
  local_1 = 0;
  do {
    uVar2 = (uint)local_1;
    if (((&DAT_00495af0)[uVar2] & 0xc0) == 0 && (*(byte *)((int)&DAT_00495ab0 + uVar2) & 0xe0) == 0)
    {
      *(byte *)((int)&DAT_00495820 +
               ((uint)((&DAT_00495af0)[uVar2] & 0x1f) << 5 |
               (uint)(*(byte *)((int)&DAT_00495ab0 + uVar2) & 0x1f))) =
           *(byte *)((int)&DAT_004957a0 + uVar2) & 0x80 | local_1;
    }
    local_1 = local_1 + 1;
  } while (local_1 < 0x38);
  FUN_0040a570();
  return;
}



/* ================= FUN_0040a050 @ 0040a050 ================= */

void FUN_0040a050(void)

{
  uint uVar1;
  char cVar2;
  
  uVar1 = FUN_0041bfb0(3);
  DAT_00495790 = (undefined1)uVar1;
  uVar1 = FUN_0041bfb0(4);
  DAT_00495791 = (undefined1)uVar1;
  uVar1 = FUN_0041bfb0(6);
  DAT_00495aa0 = (undefined1)uVar1;
  cVar2 = '8';
  uVar1 = FUN_0041bfb0(7);
  DAT_00495aa3 = (undefined1)uVar1;
  FUN_00412cf0();
  do {
    FUN_00412dc0(0xb2);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ================= FUN_0040a0b0 @ 0040a0b0 ================= */

void FUN_0040a0b0(void)

{
  (&DAT_00482c68)[(uint)DAT_0048327b * 0x18] = (&DAT_00482c68)[(uint)DAT_0048327b * 0x18] & 0x7f;
  return;
}



/* ================= FUN_0040a0d0 @ 0040a0d0 ================= */

void FUN_0040a0d0(void)

{
  (&DAT_00482c68)[(uint)DAT_0048327b * 0x18] = (&DAT_00482c68)[(uint)DAT_0048327b * 0x18] | 0x80;
  return;
}



/* ================= FUN_0040a0f0 @ 0040a0f0 ================= */

void FUN_0040a0f0(void)

{
  byte bVar1;
  
  bVar1 = 0x37;
  do {
    FUN_0040a160(bVar1);
    bVar1 = bVar1 - 1;
  } while ((bVar1 & 0x80) == 0);
  return;
}



/* ================= FUN_0040a110 @ 0040a110 ================= */

void FUN_0040a110(void)

{
  byte bVar1;
  uint uVar2;
  
  bVar1 = 2;
  do {
    uVar2 = (uint)bVar1;
    bVar1 = bVar1 + 1;
    (&DAT_00482c39)[uVar2 * 0x18] = 0;
  } while (bVar1 < 0x3a);
  return;
}



/* ================= FUN_0040a130 @ 0040a130 ================= */

void FUN_0040a130(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 2;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_00412e60(&DAT_00482c30 + uVar2 * 6);
    uVar2 = (uint)bVar1;
  } while (bVar1 < 0x3a);
  return;
}



/* ================= FUN_0040a160 @ 0040a160 ================= */

void __cdecl FUN_0040a160(byte param_1)

{
  int iVar1;
  uint uVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  
  uVar7 = (uint)param_1;
  bVar3 = (&DAT_00495af0)[uVar7];
  iVar1 = uVar7 * 0x18;
  (&DAT_00482c69)[iVar1] = 0;
  if ((((bVar3 & 0x80) == 0) && ((*(byte *)((int)&DAT_00495ab0 + uVar7) & 0xe0) == 0)) &&
     ((bVar3 & 0x40) == 0)) {
    uVar2 = (*(byte *)((int)&DAT_00495ab0 + uVar7) & 0x1f) * 2;
    if ((DAT_00495c44 <= uVar2) && (iVar6 = uVar2 - DAT_00495c44, iVar6 < 0x1e)) {
      uVar2 = (uint)(bVar3 & 0x1f) * 2;
      if ((DAT_00495c45 <= uVar2) && (iVar5 = uVar2 - DAT_00495c45, iVar5 < 0x14)) {
        FUN_0040ccb0((int)(&DAT_00482c60 + uVar7 * 6),(char)iVar6,(char)iVar5);
        bVar3 = *(byte *)((int)&DAT_004957a0 + uVar7);
        bVar4 = (bVar3 & 0x1f) * '\x02';
        if ((bVar3 & 0x80) != 0) {
          bVar4 = bVar4 + 1;
        }
        (&DAT_00482c69)[iVar1] = (&DAT_0042b170)[bVar4];
        if ((bVar3 & 0x40) != 0) {
          (&DAT_00482c74)[iVar1] = DAT_00495aa3;
          return;
        }
        if ((bVar3 & 0x20) != 0) {
          (&DAT_00482c74)[iVar1] = DAT_00495aa0;
          return;
        }
        (&DAT_00482c74)[iVar1] = (&DAT_00495790)[bVar3 >> 7];
      }
    }
  }
  return;
}



/* ================= FUN_0040a260 @ 0040a260 ================= */

void FUN_0040a260(void)

{
  byte bVar1;
  
  bVar1 = FUN_004048e0(&LAB_0040a2e0,0x14);
  if (bVar1 != 0xff) {
    *(undefined2 *)(DAT_00482c20 + 0xc) = *(undefined2 *)(&DAT_00482c6c + (uint)DAT_0048235a * 0x18)
    ;
    *(undefined2 *)(DAT_00482c20 + 0x10) =
         *(undefined2 *)(&DAT_00482c70 + (uint)DAT_0048235a * 0x18);
    FUN_0041c020(DAT_00482c20,0xf);
    *(undefined2 *)(DAT_00482334 + 0x10) = 10;
    *(ushort *)(DAT_00482334 + 0x14) = (ushort)DAT_0048235a;
  }
  return;
}



/* ================= FUN_0040a400 @ 0040a400 ================= */

void FUN_0040a400(void)

{
  byte bVar1;
  
  bVar1 = DAT_0048327b;
  *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) | 0x20;
  FUN_0040a160(bVar1);
  return;
}



/* ================= FUN_0040a420 @ 0040a420 ================= */

void FUN_0040a420(void)

{
  byte bVar1;
  
  bVar1 = DAT_0048327b;
  *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0xdf;
  FUN_0040a160(bVar1);
  return;
}



/* ================= FUN_0040a440 @ 0040a440 ================= */

void FUN_0040a440(void)

{
  byte bVar1;
  
  bVar1 = DAT_0048327b;
  *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) | 0x40;
  FUN_0040a160(bVar1);
  return;
}



/* ================= FUN_0040a460 @ 0040a460 ================= */

void FUN_0040a460(void)

{
  byte bVar1;
  
  bVar1 = *(byte *)((int)&DAT_004957e0 + DAT_00482348) >> 3;
  if ((bVar1 != 0x1f) && (bVar1 = bVar1 + (char)DAT_0048234c, 0x1f < bVar1)) {
    bVar1 = 0x1f;
  }
  *(byte *)((int)&DAT_004957e0 + DAT_00482348) =
       bVar1 << 3 | *(byte *)((int)&DAT_004957e0 + DAT_00482348) & 7;
  return;
}



/* ================= FUN_0040a4a0 @ 0040a4a0 ================= */

void FUN_0040a4a0(void)

{
  byte bVar1;
  uint uVar2;
  
  bVar1 = 0x37;
  do {
    uVar2 = (uint)bVar1;
    bVar1 = bVar1 - 1;
    *(byte *)((int)&DAT_004957a0 + uVar2) = *(byte *)((int)&DAT_004957a0 + uVar2) & 0x9f;
  } while ((bVar1 & 0x80) == 0);
  return;
}



/* ================= FUN_0040a4c0 @ 0040a4c0 ================= */

void FUN_0040a4c0(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  byte *pbVar4;
  byte local_1;
  
  pbVar4 = (&PTR_DAT_0046b988)[(uint)DAT_00495c6a * 0x10 + (uint)DAT_00495c52];
  if (DAT_0042fb74 != '\0') {
    pbVar4 = &DAT_00481430;
  }
  local_1 = 0;
  do {
    bVar1 = *pbVar4;
    if (bVar1 == 0xff) {
      return;
    }
    uVar3 = (uint)local_1;
    *(byte *)((int)&DAT_004957a0 + uVar3) = bVar1;
    *(undefined1 *)((int)&DAT_004957e0 + uVar3) = 7;
    DAT_00482348 = bVar1 & 0x80;
    bVar1 = pbVar4[1];
    *(byte *)((int)&DAT_00495ab0 + uVar3) = bVar1;
    bVar2 = pbVar4[2];
    (&DAT_00495af0)[uVar3] = bVar2;
    if ((bVar1 & 0xe0) == 0) {
      *(byte *)((int)&DAT_00495820 + ((bVar2 & 0x1f) << 5 | (uint)(bVar1 & 0x1f))) =
           (byte)DAT_00482348 | local_1;
    }
    local_1 = local_1 + 1;
    pbVar4 = pbVar4 + 3;
  } while (local_1 < 0x38);
  return;
}



/* ================= FUN_0040a570 @ 0040a570 ================= */

void FUN_0040a570(void)

{
  uint uVar1;
  byte bVar2;
  byte local_1;
  
  DAT_00495aa2 = '\0';
  DAT_00495aa1 = '\0';
  local_1 = 0x37;
  do {
    uVar1 = (uint)local_1;
    if (((&DAT_00495af0)[uVar1] & 0x80) == 0) {
      if ((*(byte *)((int)&DAT_00495ab0 + uVar1) & 0xe0) == 0) {
        if ((*(byte *)((int)&DAT_004957a0 + uVar1) & 0x80) == 0) {
          DAT_00495aa1 = DAT_00495aa1 + '\x01';
        }
        else {
LAB_0040a5ef:
          DAT_00495aa2 = DAT_00495aa2 + '\x01';
        }
      }
      else {
        bVar2 = (((*(byte *)((int)&DAT_00495ab0 + uVar1) & 0xef) >> 4) + DAT_00495c52 * '\x10') - 2;
        bVar2 = *(byte *)((int)&DAT_00481a80 +
                         (uint)(byte)(&DAT_0047f7d0)[bVar2] +
                         (uint)(byte)(&DAT_0047f7d1)[bVar2] * 0x20) & 0xf;
        if (bVar2 == 0xb) {
          DAT_00495aa1 = DAT_00495aa1 + '\x01';
        }
        else if (bVar2 == 0xd) goto LAB_0040a5ef;
      }
    }
    local_1 = local_1 - 1;
    if ((local_1 & 0x80) != 0) {
      return;
    }
  } while( true );
}



/* ================= FUN_0040a610 @ 0040a610 ================= */

undefined4 FUN_0040a610(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  
  bVar2 = 0x37;
  DAT_00482348 = 0;
  DAT_0048234c = 0;
  do {
    uVar3 = (uint)bVar2;
    if ((byte)(&DAT_00495af0)[uVar3] < 0x80) {
      bVar1 = *(byte *)((int)&DAT_004957a0 + uVar3) & 0x1f;
      if ((bVar1 != 0) && ((bVar1 != 0x11 || ((*(byte *)((int)&DAT_00495ab0 + uVar3) & 0xe0) == 0)))
         ) {
        if ((*(byte *)((int)&DAT_00495ab0 + uVar3) & 0xe0) == 0) {
          if (*(byte *)((int)&DAT_004957a0 + uVar3) < 0x80) {
LAB_0040a64e:
            DAT_00482348 = DAT_00482348 + 1;
          }
          else {
LAB_0040a6b9:
            DAT_0048234c = DAT_0048234c + 1;
          }
        }
        else {
          bVar1 = (((*(byte *)((int)&DAT_00495ab0 + uVar3) & 0xef) >> 4) + DAT_00495c52 * '\x10') -
                  2;
          bVar1 = *(byte *)((int)&DAT_00481a80 +
                           (uint)(byte)(&DAT_0047f7d0)[bVar1] +
                           (uint)(byte)(&DAT_0047f7d1)[bVar1] * 0x20) & 0xf;
          if (bVar1 == 0xb) goto LAB_0040a64e;
          if (bVar1 == 0xd) goto LAB_0040a6b9;
        }
      }
    }
    bVar2 = bVar2 - 1;
    if (0x7f < bVar2) {
      if (DAT_00482348 == 0) {
        if (DAT_0048234c != 0) {
          DAT_00495aa1 = 0;
          DAT_00495c48 = 1;
          return 1;
        }
      }
      else if (DAT_0048234c == 0) {
        DAT_00495aa2 = 0;
        DAT_00495c48 = 0;
        return 1;
      }
      return 0;
    }
  } while( true );
}



/* ================= FUN_0040a6e0 @ 0040a6e0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int __cdecl FUN_0040a6e0(int param_1,int param_2)

{
  UINT uMode;
  int iVar1;
  UINT UVar2;
  undefined4 *puVar3;
  undefined1 auStack_16 [22];
  
  if (DAT_0042b410 != 0) {
    return DAT_0042b410;
  }
  if (DAT_0042b414 == 0) {
    DAT_0042b414 = param_1;
  }
  uMode = SetErrorMode(0x8000);
  DAT_00480088 = 0xffffffff;
  DAT_0042b410 = 1;
  if (param_2 != 0) {
    DAT_0042b418 = LoadLibraryA(s_DSOUND_DLL_0042b434);
    if (DAT_0042b418 != (HMODULE)0x0) {
      _DAT_00480084 = GetProcAddress(DAT_0042b418,s_DirectSoundCreate_0042b420);
      if (_DAT_00480084 != (FARPROC)0x0) {
        iVar1 = (*_DAT_00480084)(0,&DAT_00480080,0);
        if (iVar1 == 0) {
          DAT_0042b410 = 2;
          (**(code **)(*DAT_00480080 + 0x18))(DAT_00480080,param_2,1);
          (**(code **)(*DAT_00480080 + 0xc))(DAT_00480080,auStack_16 + -10,auStack_16 + -0xe,0);
          goto LAB_0040a7e9;
        }
      }
      FreeLibrary(DAT_0042b418);
      DAT_0042b418 = (HMODULE)0x0;
    }
  }
LAB_0040a7e9:
  if (DAT_0042b410 == 1) {
    UVar2 = waveOutGetNumDevs();
    if (UVar2 == 0) {
      DAT_0042b410 = -1;
    }
  }
  SetErrorMode(uMode);
  puVar3 = &DAT_00480090;
  for (iVar1 = 0x1b0; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  return DAT_0042b410;
}



/* ================= FUN_0040a890 @ 0040a890 ================= */

undefined4 __cdecl FUN_0040a890(LPCSTR param_1,int param_2,int param_3,int param_4)

{
  undefined4 *puVar1;
  int iVar2;
  HMODULE hModule;
  HRSRC hResInfo;
  HGLOBAL hResData;
  LPVOID pvVar3;
  int iVar4;
  int iVar5;
  undefined *puVar6;
  int *unaff_EBP;
  undefined4 *unaff_EDI;
  int local_1c;
  int local_18 [6];
  
  if ((-2 < DAT_0042b410) && (DAT_0042b410 < 1)) {
    return 0;
  }
  if (((param_3 < 0) || (0x23 < param_3)) ||
     (iVar2 = param_3 * 0x30, (&DAT_00480090)[param_3 * 0xc] != 0)) {
    return 0;
  }
  if (DAT_0042b414 == (HMODULE)0x0) {
    DAT_0042b414 = GetModuleHandleA((LPCSTR)0x0);
  }
  hModule = DAT_0042b414;
  *(undefined4 *)(&DAT_004800a0 + iVar2) = 0;
  *(undefined4 *)(&DAT_0048009c + iVar2) = 0;
  *(int *)(&DAT_004800a4 + iVar2) = param_2;
  (&DAT_00480094)[param_3 * 0xc] = 0;
  if (DAT_0042b410 == 1) {
    if (param_2 == 0) {
      hResInfo = FindResourceA(hModule,param_1,&DAT_0042b440);
      if (hResInfo == (HRSRC)0x0) {
        FUN_0040b550(-1,param_1);
        return 0;
      }
      hResData = LoadResource(hModule,hResInfo);
      (&DAT_00480094)[param_3 * 0xc] = hResData;
      pvVar3 = LockResource(hResData);
      *(LPVOID *)(&DAT_00480098 + iVar2) = pvVar3;
      if (pvVar3 != (LPVOID)0x0) {
        (&DAT_00480090)[param_3 * 0xc] = 1;
      }
    }
    else {
      (&DAT_00480094)[param_3 * 0xc] = 0;
      pvVar3 = FUN_0040b3d0(param_1,(SIZE_T *)0x0);
      *(LPVOID *)(&DAT_00480098 + iVar2) = pvVar3;
      if (pvVar3 != (LPVOID)0x0) {
        (&DAT_00480090)[param_3 * 0xc] = 1;
      }
    }
  }
  else if (DAT_0042b410 == 2) {
    puVar1 = (undefined4 *)(&DAT_00480098 + iVar2);
    local_1c = 0;
    if (param_2 == 0) {
      pvVar3 = FUN_0040b510(&DAT_0042b440,param_1);
      *puVar1 = pvVar3;
      if (pvVar3 == (LPVOID)0x0) {
        FUN_0040b550(-1,param_1);
        return 0;
      }
    }
    else {
      pvVar3 = FUN_0040b3d0(param_1,(SIZE_T *)0x0);
      *puVar1 = pvVar3;
      if (pvVar3 == (LPVOID)0x0) {
        return 0;
      }
    }
    if ((param_4 < 1) || (4 < param_4)) {
      param_4 = 4;
    }
    local_18[1] = 0;
    local_18[2] = 0;
    local_18[3] = 0;
    local_18[4] = 0;
    local_18[5] = 0;
    iVar4 = FUN_0040ab40((int *)*puVar1,local_18 + 5,local_18,(uint *)(local_18 + 3));
    if (iVar4 == 0) {
      return 0;
    }
    local_18[1] = 0x14;
    local_18[2] = 0xe2;
    iVar4 = (**(code **)(*DAT_00480080 + 0xc))(DAT_00480080,local_18 + 1,&local_1c,0);
    if (iVar4 == 0) {
      iVar4 = FUN_0040ac40(unaff_EBP,unaff_EDI,local_1c);
      if (iVar4 != 0) {
        *(int **)(&DAT_004800ac + iVar2) = unaff_EBP;
        (&DAT_00480090)[param_3 * 0xc] = (&DAT_00480090)[param_3 * 0xc] + 1;
        if (1 < param_4) {
          puVar6 = &DAT_004800b0 + iVar2;
          iVar4 = param_4 + -1;
          do {
            iVar5 = (**(code **)(*DAT_00480080 + 0x14))(DAT_00480080,unaff_EBP,puVar6);
            if (iVar5 == 0) {
              (&DAT_00480090)[param_3 * 0xc] = (&DAT_00480090)[param_3 * 0xc] + 1;
            }
            puVar6 = puVar6 + 4;
            iVar4 = iVar4 + -1;
          } while (iVar4 != 0);
        }
        iVar4 = (**(code **)(*unaff_EBP + 0x20))(unaff_EBP,&DAT_004800bc + iVar2);
        if (iVar4 != 0) {
          *(undefined4 *)(&DAT_004800bc + iVar2) = 0;
        }
      }
    }
  }
  DAT_0042b41c = 1;
  return 1;
}



/* ================= FUN_0040ab40 @ 0040ab40 ================= */

undefined4 __cdecl FUN_0040ab40(int *param_1,int *param_2,int *param_3,uint *param_4)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int *piVar4;
  
  if (param_2 != (int *)0x0) {
    *param_2 = 0;
  }
  if (param_3 != (int *)0x0) {
    *param_3 = 0;
  }
  if (param_4 != (uint *)0x0) {
    *param_4 = 0;
  }
  if (*param_1 != 0x46464952) {
    return 0;
  }
  iVar1 = param_1[1];
  piVar4 = param_1 + 3;
  if (param_1[2] != 0x45564157) {
    return 0;
  }
  do {
    if ((int *)((int)param_1 + iVar1 + 8) <= piVar4) {
      return 0;
    }
    uVar2 = piVar4[1];
    piVar3 = piVar4 + 2;
    if (*piVar4 == 0x20746d66) {
      if ((param_2 != (int *)0x0) && (*param_2 == 0)) {
        if (uVar2 < 0xe) {
          return 0;
        }
        *param_2 = (int)piVar3;
        if (((param_3 == (int *)0x0) || (*param_3 != 0)) &&
           ((param_4 == (uint *)0x0 || (*param_4 != 0)))) {
          return 1;
        }
      }
    }
    else if ((*piVar4 == 0x61746164) &&
            (((param_3 != (int *)0x0 && (*param_3 == 0)) ||
             ((param_4 != (uint *)0x0 && (*param_4 == 0)))))) {
      if (param_3 != (int *)0x0) {
        *param_3 = (int)piVar3;
      }
      if (param_4 != (uint *)0x0) {
        *param_4 = uVar2;
      }
      if ((param_2 == (int *)0x0) || (*param_2 != 0)) {
        return 1;
      }
    }
    piVar4 = (int *)((int)piVar3 + (uVar2 + 1 & 0xfffffffe));
  } while( true );
}



/* ================= FUN_0040ac40 @ 0040ac40 ================= */

undefined4 __cdecl FUN_0040ac40(int *param_1,undefined4 *param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puStack_24;
  undefined4 local_10;
  undefined1 local_c [4];
  undefined4 local_8;
  undefined1 local_4 [4];
  
  if (((param_1 != (int *)0x0) && (param_2 != (undefined4 *)0x0)) && (param_3 != 0)) {
    puStack_24 = (undefined4 *)0x0;
    iVar1 = (**(code **)(*param_1 + 0x2c))(param_1,0,param_3,local_4);
    if (iVar1 == 0) {
      puVar3 = param_2;
      for (uVar2 = (uint)&local_8 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puStack_24 = *puVar3;
        puVar3 = puVar3 + 1;
        puStack_24 = puStack_24 + 1;
      }
      for (iVar1 = 0; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined1 *)puStack_24 = *(undefined1 *)puVar3;
        puVar3 = (undefined4 *)((int)puVar3 + 1);
        puStack_24 = (undefined4 *)((int)puStack_24 + 1);
      }
      if (local_c != (undefined1 *)0x0) {
        puVar3 = (undefined4 *)((int)&local_8 + (int)param_2);
        puVar4 = &local_10;
        for (uVar2 = (uint)local_c >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
          *puVar4 = *puVar3;
          puVar3 = puVar3 + 1;
          puVar4 = puVar4 + 1;
        }
        for (iVar1 = 0; iVar1 != 0; iVar1 = iVar1 + -1) {
          *(undefined1 *)puVar4 = *(undefined1 *)puVar3;
          puVar3 = (undefined4 *)((int)puVar3 + 1);
          puVar4 = (undefined4 *)((int)puVar4 + 1);
        }
      }
      (**(code **)(*param_1 + 0x4c))(param_1,0,&local_8,&local_10,local_c);
      return 1;
    }
  }
  return 0;
}



/* ================= FUN_0040afd0 @ 0040afd0 ================= */

void FUN_0040afd0(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  if (DAT_0042b410 == 1) {
    sndPlaySoundA((LPCSTR)0x0,0);
    DAT_00480088 = 0xffffffff;
  }
  piVar3 = &DAT_00480090;
  do {
    if (*piVar3 != 0) {
      if (DAT_0042b410 == 1) {
        FreeResource((HGLOBAL)piVar3[1]);
        piVar3[1] = 0;
      }
      else if (DAT_0042b410 == 2) {
        iVar1 = 0;
        if (0 < *piVar3) {
          piVar2 = piVar3 + 7;
          do {
            iVar1 = iVar1 + 1;
            (**(code **)(*(int *)*piVar2 + 0x48))((int *)*piVar2);
            (**(code **)(*(int *)*piVar2 + 8))((int *)*piVar2);
            *piVar2 = 0;
            piVar2 = piVar2 + 1;
          } while (iVar1 < *piVar3);
        }
        *piVar3 = 0;
      }
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 0xc;
  } while (piVar3 < &DAT_00480750);
  DAT_0042b41c = 0;
  return;
}



/* ================= FUN_0040b080 @ 0040b080 ================= */

undefined4 __cdecl FUN_0040b080(int param_1,int param_2)

{
  BOOL BVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  byte local_4 [4];
  
  if (DAT_0042b410 != 1) {
    if (DAT_0042b410 == 2) {
      iVar4 = (&DAT_00480090)[param_1 * 0xc];
      if (iVar4 != 0) {
        if (param_2 < 0) {
          param_2 = *(int *)(&DAT_004800a8 + param_1 * 0x30);
        }
        else if (param_2 < iVar4) {
          iVar4 = 1;
        }
        else {
          iVar4 = 0;
          param_2 = 0;
        }
        iVar3 = 0;
        if (0 < iVar4) {
          while( true ) {
            param_2 = param_2 + -1;
            if (param_2 < 0) {
              param_2 = iVar4 + -1;
            }
            iVar2 = (**(code **)(**(int **)(&DAT_004800ac + (param_1 * 0xc + param_2) * 4) + 0x24))
                              (*(int **)(&DAT_004800ac + (param_1 * 0xc + param_2) * 4),local_4);
            if ((iVar2 == 0) && ((local_4[0] & 1) != 0)) break;
            iVar3 = iVar3 + 1;
            if (iVar4 <= iVar3) {
              return 0;
            }
          }
          return 1;
        }
      }
    }
    return 0;
  }
  if (DAT_00480088 != param_1) {
    return 0;
  }
  if ((DAT_0048008c == 0) && (BVar1 = sndPlaySoundA((LPCSTR)0x0,0x10), BVar1 != 0)) {
    return 0;
  }
  return 1;
}



/* ================= FUN_0040b170 @ 0040b170 ================= */

void __cdecl FUN_0040b170(int param_1,int param_2,int param_3)

{
  int iVar1;
  int local_4;
  
  if (DAT_0042b410 == 2) {
    iVar1 = (&DAT_00480090)[param_1 * 0xc];
    if ((iVar1 != 0) &&
       ((param_3 != 0 || (param_3 = *(int *)(&DAT_004800bc + param_1 * 0x30), param_3 != 0)))) {
      if (param_2 < 0) {
        param_2 = *(int *)(&DAT_004800a8 + param_1 * 0x30);
      }
      else if (param_2 < iVar1) {
        iVar1 = 1;
      }
      else {
        iVar1 = 0;
        param_2 = 0;
      }
      if (0 < iVar1) {
        local_4 = iVar1;
        do {
          param_2 = param_2 + -1;
          if (param_2 < 0) {
            param_2 = iVar1 + -1;
          }
          (**(code **)(**(int **)(&DAT_004800ac + (param_1 * 0xc + param_2) * 4) + 0x44))
                    (*(int **)(&DAT_004800ac + (param_1 * 0xc + param_2) * 4),param_3);
          local_4 = local_4 + -1;
        } while (local_4 != 0);
      }
    }
  }
  return;
}



/* ================= FUN_0040b260 @ 0040b260 ================= */

int __cdecl FUN_0040b260(int param_1,uint param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  uint unaff_ESI;
  uint unaff_EDI;
  undefined1 local_4 [4];
  
  iVar4 = param_1 * 0x30;
  if ((&DAT_00480090)[param_1 * 0xc] == 0) {
    return -1;
  }
  if (DAT_0042b410 == 1) {
    sndPlaySoundA(*(LPCSTR *)(&DAT_00480098 + iVar4),((param_2 & 1) == 0) - 1 & 8 | 5);
    DAT_00480088 = param_1;
    DAT_0048008c = (uint)((param_2 & 1) != 0);
    return 0;
  }
  if (DAT_0042b410 != 2) {
    return 0;
  }
  if ((int)(&DAT_00480090)[param_1 * 0xc] <= *(int *)(&DAT_004800a8 + iVar4)) {
    *(undefined4 *)(&DAT_004800a8 + iVar4) = 0;
  }
  iVar1 = *(int *)(&DAT_004800a8 + iVar4);
  *(int *)(&DAT_004800a8 + iVar4) = iVar1 + 1;
  piVar2 = *(int **)(&DAT_004800ac + (iVar1 + param_1 * 0xc) * 4);
  iVar5 = (**(code **)(*piVar2 + 0x24))(piVar2,local_4);
  if (iVar5 == 0) {
    if ((unaff_ESI & 1) != 0) {
      (**(code **)(*piVar2 + 0x48))(piVar2);
      (**(code **)(*piVar2 + 0x34))(piVar2,0);
    }
    (**(code **)(*piVar2 + 0x40))(piVar2,*(undefined4 *)(&DAT_0048009c + iVar4));
    piVar3 = *(int **)(&DAT_004800ac + (iVar1 + param_1 * 0xc) * 4);
    (**(code **)(*piVar3 + 0x3c))(piVar3,*(undefined4 *)(&DAT_004800a0 + iVar4));
    (**(code **)(*piVar2 + 0x30))(piVar2,0,0,unaff_EDI & 1);
  }
  return iVar1;
}



/* ================= FUN_0040b380 @ 0040b380 ================= */

void FUN_0040b380(void)

{
  if (DAT_0042b41c != 0) {
    FUN_0040afd0();
  }
  if (DAT_0042b410 == 2) {
    (**(code **)(*DAT_00480080 + 8))(DAT_00480080);
    DAT_00480080 = (int *)0x0;
    FreeLibrary(DAT_0042b418);
    DAT_0042b418 = (HMODULE)0x0;
  }
  DAT_0042b410 = 0;
  return;
}



/* ================= FUN_0040b3d0 @ 0040b3d0 ================= */

LPVOID __cdecl FUN_0040b3d0(LPCSTR param_1,SIZE_T *param_2)

{
  HFILE hFile;
  DWORD DVar1;
  SIZE_T dwBytes;
  HGLOBAL pvVar2;
  LPVOID lpBuffer;
  long lVar3;
  CHAR local_d8 [80];
  _OFSTRUCT local_88;
  
  hFile = OpenFile(param_1,&local_88,0x20);
  if (hFile == -1) {
    DVar1 = GetLastError();
    wsprintfA(local_d8,s__s__>__ld_0042b448,param_1,DVar1);
    FUN_0040b550(-4,local_d8);
    return (LPVOID)0x0;
  }
  dwBytes = _llseek(hFile,0,2);
  _llseek(hFile,0,0);
  pvVar2 = GlobalAlloc(0,dwBytes);
  lpBuffer = GlobalLock(pvVar2);
  if (lpBuffer == (LPVOID)0x0) {
    FUN_0040b550(-3,param_1);
    _lclose(hFile);
    return (LPVOID)0x0;
  }
  lVar3 = _hread(hFile,lpBuffer,dwBytes);
  if (lVar3 < (int)dwBytes) {
    FUN_0040b550(-4,param_1);
    pvVar2 = GlobalHandle(lpBuffer);
    GlobalUnlock(pvVar2);
    pvVar2 = GlobalHandle(lpBuffer);
    GlobalFree(pvVar2);
    _lclose(hFile);
    return (LPVOID)0x0;
  }
  _lclose(hFile);
  if (param_2 != (SIZE_T *)0x0) {
    *param_2 = dwBytes;
  }
  return lpBuffer;
}



/* ================= FUN_0040b510 @ 0040b510 ================= */

LPVOID __cdecl FUN_0040b510(LPCSTR param_1,LPCSTR param_2)

{
  HMODULE hModule;
  HRSRC hResInfo;
  HGLOBAL hResData;
  LPVOID pvVar1;
  
  hModule = GetModuleHandleA((LPCSTR)0x0);
  hResInfo = FindResourceA(hModule,param_2,param_1);
  if (hResInfo == (HRSRC)0x0) {
    return (LPVOID)0x0;
  }
  hResData = LoadResource(hModule,hResInfo);
  pvVar1 = LockResource(hResData);
  return pvVar1;
}



/* ================= FUN_0040b550 @ 0040b550 ================= */

void __cdecl FUN_0040b550(int param_1,undefined4 param_2)

{
  char *pcVar1;
  CHAR local_100 [256];
  
  if (param_1 < 0) {
    pcVar1 = s_error_0042b4a8;
    if (param_1 == -4) {
      pcVar1 = &DAT_0042b45c;
    }
    else if (param_1 == -3) {
      pcVar1 = &DAT_0042b480;
    }
    else if (param_1 == -1) {
      pcVar1 = &DAT_0042b490;
    }
    wsprintfA(local_100,s__s___s_0042b454,param_2,pcVar1);
  }
  return;
}



/* ================= FUN_0040b5b0 @ 0040b5b0 ================= */

void FUN_0040b5b0(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_0040b5e0,100);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0x3c;
  return;
}



/* ================= FUN_0040b810 @ 0040b810 ================= */

void __cdecl FUN_0040b810(uint param_1)

{
  char cVar1;
  
  DAT_00495c7e = DAT_00495c7e + '@';
  if (DAT_00495c7e == '\0') {
    DAT_004834c0 = DAT_004834c0 + 1;
  }
  cVar1 = (char)DAT_004834c0 - DAT_00495c7c;
  if ((cVar1 != '\0') && (DAT_00495c7b = cVar1 + DAT_00495c7b & 7, DAT_00495c7b == 0)) {
    DAT_0048233c = (param_1 & 0xffff) + (uint)DAT_00495c7d * 0x40;
    DAT_00482338 = (uint)DAT_00495c7d * 0x40 & 0x7c0;
    FUN_0040b9a0();
    DAT_00495c7d = DAT_00495c7d + 1;
  }
  DAT_00495c7c = (undefined1)DAT_004834c0;
  return;
}



/* ================= FUN_0040b8d0 @ 0040b8d0 ================= */

void FUN_0040b8d0(void)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  byte bVar4;
  char local_1;
  
  DAT_00481f94 = &DAT_0042b4b0;
  FUN_00418bb0();
  DAT_00482378 = 8000;
  DAT_0048235e = '\x14';
  do {
    DAT_0048235b = ' ';
    FUN_0040da00((short)DAT_00482378);
    do {
      cVar1 = FUN_00418bf0();
      cVar2 = FUN_00418bf0();
      FUN_0040daf0(CONCAT11(cVar2,cVar1) + 0x6a0);
      DAT_0048235b = DAT_0048235b + -1;
    } while (DAT_0048235b != '\0');
    DAT_00482378 = DAT_00482378 + 0x40;
    DAT_0048235e = DAT_0048235e + -1;
  } while (DAT_0048235e != '\0');
  DAT_00482378 = 0x1e88;
  local_1 = '\x02';
  bVar4 = 0;
  do {
    FUN_0040da00((short)DAT_00482378);
    do {
      uVar3 = (uint)bVar4;
      bVar4 = bVar4 + 1;
      FUN_0040daf0((byte)s____________________0_1_2_3_4_2_6_0042b7a0[uVar3] + 0x17a0);
    } while ((bVar4 & 0xf) != 0);
    DAT_00482378 = DAT_00482378 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_0040b9a0 @ 0040b9a0 ================= */

void FUN_0040b9a0(void)

{
  ushort uVar1;
  char cVar2;
  
  FUN_0040da00((short)DAT_00482338);
  cVar2 = ' ';
  FUN_0040da10((short)DAT_0048233c);
  do {
    uVar1 = FUN_0040da80();
    FUN_0040daf0(uVar1);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  return;
}



/* ================= FUN_0040b9e0 @ 0040b9e0 ================= */

void FUN_0040b9e0(void)

{
  char cVar1;
  char local_1;
  
  FUN_0040da00(0x1000);
  local_1 = ' ';
  do {
    cVar1 = '\0';
    do {
      FUN_0040daf0(0x90);
      cVar1 = cVar1 + -1;
    } while (cVar1 != '\0');
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_0040ba20 @ 0040ba20 ================= */

void FUN_0040ba20(void)

{
  byte bVar1;
  uint uVar2;
  
  DAT_004834c4 = 0x1000;
  uVar2 = 0;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_00406830((char)uVar2 - 0x2a,1,(&DAT_0042b7f8)[uVar2]);
    uVar2 = (uint)bVar1;
  } while (bVar1 != 5);
  DAT_004834c4 = 0;
  return;
}



/* ================= FUN_0040ba60 @ 0040ba60 ================= */

void FUN_0040ba60(void)

{
  byte bVar1;
  
  bVar1 = FUN_00404940(&LAB_0040ba90,0x32);
  if (bVar1 != 0xff) {
    DAT_00481a68 = 0xff;
    DAT_00495f3c = 1;
  }
  return;
}



/* ================= FUN_0040bbe0 @ 0040bbe0 ================= */

void FUN_0040bbe0(void)

{
  char cVar1;
  char local_1;
  
  if (*(short *)(DAT_00482330 + 0x10) == 0) {
    FUN_0040da00(0x1000);
    local_1 = '\r';
    do {
      cVar1 = '@';
      do {
        FUN_0040da20(0);
        cVar1 = cVar1 + -1;
      } while (cVar1 != '\0');
      local_1 = local_1 + -1;
    } while (local_1 != '\0');
    FUN_00412750(0x1000,0xd);
    FUN_00412cf0();
    FUN_00404a00();
    DAT_00495f3c = 0;
  }
  return;
}



/* ================= FUN_0040bf70 @ 0040bf70 ================= */

void __cdecl FUN_0040bf70(int param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  FUN_0040da10(((byte)(&DAT_0042b7ff)[*(short *)(DAT_00482330 + 0x12)] + 0x40) * 0x40);
  FUN_0040da00((*(short *)(DAT_00482330 + 0x24) + 0x4c) * 0x40);
  *(char *)(param_1 + 9) = (char)*(undefined2 *)(DAT_00482330 + 0x24) + -0x2d;
  uVar4 = (int)*(short *)(*(int *)(DAT_00482330 + 0x30) + 0xc) - (int)*(short *)(param_1 + 0xc);
  if (uVar4 == 0x10) {
    *(char *)(*(int *)(&DAT_0049574c +
                      ((int)*(short *)(DAT_00482330 + 0x1e) + (int)*(short *)(DAT_00482330 + 0x22))
                      * 4) + 9) = (&DAT_0042b7ff)[*(short *)(DAT_00482330 + 0x12)] + -0x39;
    return;
  }
  if (uVar4 == 0) {
    *(undefined1 *)(param_1 + 9) = 0;
    return;
  }
  if (uVar4 < 0x11) {
    iVar5 = 0x10;
    if ((int)uVar4 < 9) {
      do {
        uVar1 = FUN_0040daa0();
        uVar2 = FUN_0040daa0();
        uVar3 = CONCAT22(uVar1,uVar2) & *(uint *)(&DAT_0042b820 + uVar4 * 4);
        FUN_0040da50((short)(uVar3 >> 0x10));
        FUN_0040da50((short)uVar3);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      iVar5 = 0x10;
      do {
        FUN_0040da20(0);
        FUN_0040da20(0);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    else {
      do {
        uVar1 = FUN_0040da80();
        FUN_0040da20(uVar1);
        uVar1 = FUN_0040da80();
        FUN_0040da20(uVar1);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      iVar5 = 0x10;
      do {
        uVar1 = FUN_0040daa0();
        uVar2 = FUN_0040daa0();
        uVar3 = CONCAT22(uVar1,uVar2) & *(uint *)(&DAT_0042b800 + uVar4 * 4);
        FUN_0040da50((short)(uVar3 >> 0x10));
        FUN_0040da50((short)uVar3);
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    FUN_00412750((*(short *)(DAT_00482330 + 0x24) + 0x4c) * 0x40,1);
  }
  return;
}



/* ================= FUN_0040c120 @ 0040c120 ================= */

void FUN_0040c120(void)

{
  FUN_00404940(&LAB_0040c150,0x32);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  *(undefined2 *)(DAT_00482334 + 0x12) = 0xd2;
  return;
}



/* ================= FUN_0040c420 @ 0040c420 ================= */

void FUN_0040c420(void)

{
  int iVar1;
  byte bVar2;
  uint uVar3;
  byte bVar4;
  byte local_1;
  
  uVar3 = 0x37;
  while ((((0x7f < (byte)(&DAT_00495af0)[uVar3] ||
           ((*(byte *)((int)&DAT_004957a0 + uVar3) & 0x80) != DAT_00495c63)) ||
          (*(char *)((int)&DAT_00481fa0 + uVar3) != '\0')) ||
         (4 < (*(byte *)((int)&DAT_004957e0 + uVar3) & 7)))) {
    bVar2 = (byte)uVar3 - 1;
    uVar3 = (uint)bVar2;
    if (0x7f < bVar2) {
      thunk_FUN_00417af0();
      DAT_00484710 = 0xff;
      return;
    }
  }
  DAT_0048327c = *(byte *)((int)&DAT_004957a0 + uVar3) & 0x1f;
  DAT_0048235a = 0xff;
  local_1 = 0;
  DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar3) & 0x1f;
  DAT_0048327e = (&DAT_00495af0)[uVar3] & 0x1f;
  DAT_0048327b = (byte)uVar3;
  do {
    uVar3 = (uint)local_1;
    bVar2 = (&DAT_00482320)[uVar3];
    if (0x7f < bVar2) break;
    if (((*(byte *)((int)&DAT_00481a80 + (uint)bVar2 + (uint)(byte)(&DAT_00482328)[uVar3] * 0x20) &
         0xf) == DAT_00495c04) &&
       (bVar2 = FUN_004179f0(bVar2,(&DAT_00482328)[uVar3]), bVar2 < DAT_0048235a)) {
      DAT_00482390 = (&DAT_00482320)[uVar3];
      DAT_00482391 = (&DAT_00482328)[uVar3];
      DAT_00482358 = local_1;
      DAT_0048235a = bVar2;
    }
    local_1 = local_1 + 1;
  } while (local_1 != 8);
  if ((DAT_0048235a < 0x80) &&
     (((*(byte *)((int)&DAT_004957e0 + (uint)DAT_0048327b) & 7) < 3 ||
      (DAT_0048235a <= (byte)(&DAT_0042b218)[DAT_0048327c])))) {
    FUN_00417f20();
    if ((&DAT_00495c70)[(uint)DAT_00482390 + (uint)DAT_00482391 * 0x20] != 0) {
      DAT_00482348 = (uint)DAT_00495f30 -
                     (uint)((byte)(&DAT_00495c70)[(uint)DAT_00482390 + (uint)DAT_00482391 * 0x20] >>
                           4);
      if (DAT_00482348 == 0) {
LAB_0040c70f:
        DAT_00484711 = DAT_00482390;
        DAT_00484712 = DAT_00482391;
        DAT_00484710 = 0;
        DAT_00484713 = 0xff;
        *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) =
             *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) | 8;
        return;
      }
      if (DAT_00482348 <=
          (byte)(&DAT_0042b848)[*(byte *)((int)&DAT_004957e0 + (uint)DAT_0048327b) & 7]) {
        DAT_00483276 = DAT_00482390;
        DAT_00483277 = DAT_00482391;
        FUN_00419620();
        DAT_00482390 = DAT_00481fda;
        DAT_00482391 = DAT_00481fdb;
        DAT_00481f99 = 10;
        DAT_00481dbc = 0;
        DAT_00481fdd = 1;
        iVar1 = FUN_004198b0();
        if (iVar1 == 0) goto LAB_0040c70f;
        DAT_00482392 = 1;
        do {
          FUN_004179b0(4);
          DAT_00482394 = 0;
          DAT_00482395 = 0xff;
          DAT_00482396 = 0xf0;
          do {
            DAT_00482393 = DAT_00482392;
            do {
              if (((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) &&
                 (iVar1 = FUN_004198b0(), iVar1 != 1)) {
                bVar2 = (&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20];
                bVar4 = bVar2 & 0xf0;
                if (DAT_00482396 == bVar4) {
                  if ((bVar2 & 0xf) < DAT_00482395) {
LAB_0040c6b0:
                    DAT_00482395 = bVar2 & 0xf;
                    DAT_00482390 = DAT_00481fda;
                    DAT_00482391 = DAT_00481fdb;
                  }
                }
                else if (bVar4 < DAT_00482396) {
                  DAT_00482396 = bVar4;
                  goto LAB_0040c6b0;
                }
              }
              FUN_004179b0(DAT_00482394);
              DAT_00482393 = DAT_00482393 - 1;
            } while (DAT_00482393 != 0);
            DAT_00482394 = DAT_00482394 + 1;
          } while (DAT_00482394 < 6);
          if (DAT_00482395 < 0x80) goto LAB_0040c70f;
          DAT_00482392 = DAT_00482392 + 1;
        } while (DAT_00482392 < 9);
      }
    }
  }
  *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) | 0x80;
  DAT_00484710 = 0xff;
  return;
}



/* ================= FUN_0040c770 @ 0040c770 ================= */

int FUN_0040c770(void)

{
  byte bVar1;
  undefined3 extraout_var;
  
  bVar1 = FUN_0040cb50();
  return CONCAT31(extraout_var,bVar1) * 100 >> 8;
}



/* ================= FUN_0040c7e0 @ 0040c7e0 ================= */

void __cdecl FUN_0040c7e0(byte param_1,byte param_2,char param_3)

{
  DAT_00481dac = (ushort)param_2 << 6 | (ushort)param_1;
  for (; param_3 != '\0'; param_3 = param_3 + -1) {
    FUN_00421540(0x20);
  }
  return;
}



/* ================= FUN_0040c820 @ 0040c820 ================= */

void FUN_0040c820(byte param_1,byte param_2,undefined4 param_3)

{
  DAT_00481dac = (ushort)param_2 << 6 | (ushort)param_1;
  DAT_00481da8 = param_3;
  FUN_00421620();
  return;
}



/* ================= FUN_0040c850 @ 0040c850 ================= */

void __cdecl
FUN_0040c850(byte param_1,byte param_2,byte *param_3,undefined4 param_4,undefined4 param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
            undefined4 param_10,undefined4 param_11,undefined4 param_12,undefined4 param_13)

{
  char *pcVar1;
  byte bVar2;
  char cVar4;
  char cVar3;
  uint uVar5;
  byte bVar6;
  byte bVar7;
  short sVar8;
  short local_34 [6];
  undefined4 local_28 [4];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  local_34[0] = 1;
  local_34[1] = 10;
  local_28[0] = param_4;
  local_34[2] = 100;
  local_34[3] = 1000;
  local_34[4] = 10000;
  local_28[1] = param_5;
  local_28[2] = param_6;
  local_28[3] = param_7;
  local_18 = param_8;
  local_14 = param_9;
  local_10 = param_10;
  local_c = param_11;
  local_8 = param_12;
  local_4 = param_13;
  DAT_00495744 = 0;
  DAT_00481dac = (ushort)param_2 << 6 | (ushort)param_1;
  param_2 = 1;
  bVar7 = *param_3;
  bVar6 = param_2;
  do {
    if (bVar7 == 0) {
      return;
    }
    param_2 = bVar6;
    if (bVar7 == 0x25) {
      param_2 = bVar6 + 1;
      bVar7 = param_3[bVar6];
      if (bVar7 == 0x25) goto LAB_0040c98d;
      DAT_00495745 = 0x20;
      DAT_00495743 = 4;
      if (bVar7 == 0x30) {
        DAT_00495745 = 0x30;
        uVar5 = (uint)param_2;
        param_2 = bVar6 + 2;
        bVar7 = param_3[uVar5];
      }
      if ((0x30 < bVar7) && (bVar7 < 0x36)) {
        DAT_00495743 = bVar7 - 0x31;
        uVar5 = (uint)param_2;
        param_2 = param_2 + 1;
        bVar7 = param_3[uVar5];
      }
      bVar7 = bVar7 | 0x20;
      if (bVar7 == 100) {
        sVar8 = *(short *)(local_28 + DAT_00495744);
        DAT_00495744 = DAT_00495744 + 1;
        DAT_0048234c = 0;
        uVar5 = 4;
        do {
          cVar3 = -1;
          do {
            cVar4 = cVar3;
            cVar3 = cVar4 + '\x01';
            sVar8 = sVar8 - local_34[uVar5];
          } while (-1 < sVar8);
          sVar8 = sVar8 + local_34[uVar5];
          if (((cVar3 == '\0') && (DAT_0048234c == 0)) && ((char)uVar5 != '\0')) {
            bVar6 = DAT_00495745;
          }
          else {
            bVar6 = cVar4 + 0x31;
            DAT_0048234c = DAT_0048234c + 1;
          }
          bVar7 = (char)uVar5 - 1;
          *(byte *)(uVar5 + 0x495748) = bVar6;
          uVar5 = (uint)bVar7;
        } while ((bVar7 & 0x80) == 0);
        bVar6 = DAT_00495743;
        do {
          uVar5 = (uint)bVar6;
          bVar6 = bVar6 - 1;
          FUN_00421540(*(byte *)(uVar5 + 0x495748));
        } while ((bVar6 & 0x80) == 0);
      }
      else if (bVar7 == 0x78) {
        if (3 < DAT_00495743) {
          DAT_00495743 = 3;
        }
        bVar6 = 3;
        uVar5 = (uint)DAT_00495744;
        DAT_00495744 = DAT_00495744 + 1;
        DAT_0048234c = 0;
        sVar8 = *(short *)(local_28 + uVar5);
        do {
          bVar7 = (byte)((ushort)sVar8 >> 0xc);
          if ((bVar7 == 0) && (DAT_0048234c == 0)) {
            bVar2 = DAT_00495745;
          }
          else {
            bVar2 = bVar7 + 0x30;
            DAT_0048234c = DAT_0048234c + 1;
            if (0x39 < bVar2) {
              bVar2 = bVar7 + 0x37;
            }
          }
          uVar5 = (uint)bVar6;
          bVar6 = bVar6 - 1;
          *(byte *)(uVar5 + 0x495748) = bVar2;
          sVar8 = sVar8 << 4;
        } while ((bVar6 & 0x80) == 0);
        bVar6 = DAT_00495743;
        do {
          uVar5 = (uint)bVar6;
          bVar6 = bVar6 - 1;
          FUN_00421540(*(byte *)(uVar5 + 0x495748));
        } while ((bVar6 & 0x80) == 0);
      }
      else {
        if (bVar7 != 0x73) {
          if (bVar7 == 99) {
            uVar5 = (uint)DAT_00495744;
            DAT_00495744 = DAT_00495744 + 1;
            bVar7 = *(byte *)(local_28 + uVar5);
          }
          goto LAB_0040c98d;
        }
        uVar5 = (uint)DAT_00495744;
        DAT_00495744 = DAT_00495744 + 1;
        pcVar1 = (char *)local_28[uVar5];
        cVar3 = *pcVar1;
        bVar6 = 0;
        while (cVar3 != '\0') {
          FUN_00421540(pcVar1[bVar6]);
          cVar3 = pcVar1[(byte)(bVar6 + 1)];
          bVar6 = bVar6 + 1;
        }
      }
    }
    else {
LAB_0040c98d:
      FUN_00421540(bVar7);
    }
    bVar6 = param_2 + 1;
    bVar7 = param_3[param_2];
  } while( true );
}



/* ================= FUN_0040cb10 @ 0040cb10 ================= */

void __cdecl FUN_0040cb10(byte param_1)

{
  if (param_1 != 0xff) {
    FUN_00412890(param_1);
  }
  return;
}



/* ================= FUN_0040cb30 @ 0040cb30 ================= */

void FUN_0040cb30(undefined *UNRECOVERED_JUMPTABLE)

{
                    /* WARNING: Could not recover jumptable at 0x0040cb30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)UNRECOVERED_JUMPTABLE)();
  return;
}



/* ================= FUN_0040cb50 @ 0040cb50 ================= */

byte FUN_0040cb50(void)

{
  short sVar1;
  byte bVar2;
  
  bVar2 = DAT_0048349f;
  if (DAT_0048349f == 0) {
    sVar1 = CONCAT11(DAT_00495741 ^ DAT_00495740,DAT_00495740 ^ DAT_00495742) * 2;
    DAT_00495740 = (byte)sVar1 | (DAT_00495741 >> 6 ^ DAT_00495740) & 1;
    DAT_00495741 = (byte)((ushort)sVar1 >> 8);
    bVar2 = DAT_00495741 ^ DAT_00495740;
  }
  return bVar2;
}



/* ================= FUN_0040cbb0 @ 0040cbb0 ================= */

void FUN_0040cbb0(void)

{
  char cVar1;
  
  cVar1 = '\x18';
  DAT_00495741 = 0;
  DAT_00495740 = 0;
  DAT_00495742 = 0;
  do {
    FUN_0040cb50();
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}



/* ================= FUN_0040cbd0 @ 0040cbd0 ================= */

void FUN_0040cbd0(void)

{
  byte bVar1;
  
  bVar1 = DAT_004835d0 & 0xf;
  do {
    FUN_0040cb50();
    bVar1 = bVar1 - 1;
  } while (bVar1 != 0);
  return;
}



/* ================= FUN_0040cbf0 @ 0040cbf0 ================= */

void __cdecl FUN_0040cbf0(int param_1,char param_2)

{
  FUN_0040cc10(param_1,(short)param_2 << 4);
  return;
}



/* ================= FUN_0040cc10 @ 0040cc10 ================= */

void __cdecl FUN_0040cc10(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0xe) +
          (*(short *)(param_1 + 0xc) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0xe) = (short)iVar1;
  *(short *)(param_1 + 0xc) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_0040cc60 @ 0040cc60 ================= */

void __cdecl FUN_0040cc60(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0x12) +
          (*(short *)(param_1 + 0x10) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0x12) = (short)iVar1;
  *(short *)(param_1 + 0x10) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_0040ccb0 @ 0040ccb0 ================= */

void __cdecl FUN_0040ccb0(int param_1,char param_2,char param_3)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_00495c44;
  *(short *)(param_1 + 0xc) = param_2 * 8 + 0x32;
  *(short *)(param_1 + 0x10) = param_3 * 8 + 0x58;
  if (((int)(uVar1 + (int)(short)param_2) >> 1 & 1U) != 0) {
    *(short *)(param_1 + 0x10) = param_3 * 8 + 0x60;
  }
  return;
}



/* ================= FUN_0040cd00 @ 0040cd00 ================= */

void __cdecl FUN_0040cd00(byte param_1)

{
  *(ushort *)(DAT_00482330 + 0x10) = (ushort)param_1;
  *(undefined2 *)(DAT_00482330 + 0x12) = 4;
  return;
}



/* ================= FUN_0040cd20 @ 0040cd20 ================= */

void FUN_0040cd20(void)

{
  *(short *)(DAT_00482330 + 0x12) = *(short *)(DAT_00482330 + 0x12) + -1;
  if (*(short *)(DAT_00482330 + 0x12) == 0) {
    *(short *)(DAT_00482330 + 0x12) = 4;
    *(short *)(DAT_00482330 + 0x10) = *(short *)(DAT_00482330 + 0x10) + -1;
  }
  return;
}



/* ================= FUN_0040cd50 @ 0040cd50 ================= */

undefined * __cdecl FUN_0040cd50(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 4;
  return &DAT_00444348 +
         CONCAT21(CONCAT11((&DAT_004324d1)[iVar1],(&DAT_004324d2)[iVar1]),(&DAT_004324d3)[iVar1]);
}



/* ================= FUN_0040cd80 @ 0040cd80 ================= */

void __cdecl
FUN_0040cd80(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  byte bVar1;
  uint uVar2;
  char cVar3;
  undefined *puVar4;
  char *pcVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined *puVar9;
  undefined1 *puVar10;
  undefined4 *puVar11;
  char *pcVar12;
  undefined4 *puVar13;
  undefined1 unaff_retaddr;
  uint uVar14;
  undefined1 uStack00000088;
  undefined1 uStack0000008c;
  undefined1 uStack00000090;
  undefined1 uStack00000094;
  int in_stack_0000208c;
  
  FUN_00425720(unaff_retaddr);
  puVar11 = &param_2;
  for (iVar6 = 0x20; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar11 = 0;
    puVar11 = puVar11 + 1;
  }
  bVar1 = (&DAT_0042bb89)[in_stack_0000208c * 4];
  DAT_00481f94 = FUN_0040cd50((uint)bVar1);
  FUN_00418bb0();
  uVar7 = (uint)(byte)(&DAT_004324d0)[(uint)bVar1 * 4];
  if (uVar7 == 0) {
    uVar7 = 0x100;
  }
  uVar8 = uVar7 * 0x10;
  uVar14 = (uint)(byte)(&DAT_0042bb8a)[in_stack_0000208c * 4];
  if (uVar14 == 0) {
    uVar14 = 0x100;
  }
  uVar14 = uVar14 * 0x10 + uVar7 * -0x10;
  switch((&DAT_0042bb88)[in_stack_0000208c * 4]) {
  case 0:
    pcVar5 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 != 0) {
      pcVar12 = pcVar5;
      do {
        pcVar5 = pcVar12 + 1;
        cVar3 = FUN_00418bf0();
        *pcVar12 = cVar3;
        DAT_0048234c = DAT_0048234c + 1;
        pcVar12 = pcVar5;
      } while (DAT_0048234c < uVar7 * 0x20);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *pcVar5 = '\0';
        pcVar5 = pcVar5 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412680((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 4);
    return;
  case 1:
    puVar4 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 != 0) {
      do {
        cVar3 = FUN_00418bf0();
        uVar2 = DAT_0048234c;
        DAT_0048234c = DAT_0048234c + 1;
        (&uStack00000088)[uVar2] = cVar3;
      } while (DAT_0048234c < uVar8);
    }
    DAT_0048234c = 0;
    puVar10 = puVar4;
    if (uVar7 != 0) {
      do {
        iVar6 = DAT_0048234c * 0x10;
        param_3 = *(undefined4 *)(&uStack0000008c + iVar6);
        param_2 = *(undefined4 *)(&uStack00000088 + iVar6);
        puVar4 = puVar10 + 0x20;
        param_4 = *(undefined4 *)(&uStack00000090 + iVar6);
        param_5 = *(undefined4 *)(&uStack00000094 + iVar6);
        FUN_0040d770(puVar10,(byte *)&param_2);
        DAT_0048234c = DAT_0048234c + 1;
        puVar10 = puVar4;
      } while (DAT_0048234c < uVar7);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412680((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 4);
    return;
  case 2:
    puVar4 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 != 0) {
      do {
        cVar3 = FUN_00418bf0();
        uVar2 = DAT_0048234c;
        DAT_0048234c = DAT_0048234c + 1;
        (&uStack00000088)[uVar2] = cVar3;
      } while (DAT_0048234c < uVar7 << 3);
    }
    DAT_0048234c = 0;
    if ((uVar8 & 0xffffffc0) != 0) {
      puVar9 = puVar4;
      do {
        puVar11 = (undefined4 *)(&uStack00000088 + DAT_0048234c * 0x20);
        puVar13 = &param_2;
        for (iVar6 = 8; iVar6 != 0; iVar6 = iVar6 + -1) {
          *puVar13 = *puVar11;
          puVar11 = puVar11 + 1;
          puVar13 = puVar13 + 1;
        }
        puVar4 = puVar9 + 0x80;
        FUN_0040d3d0(puVar9,&param_2);
        DAT_0048234c = DAT_0048234c + 1;
        puVar9 = puVar4;
      } while (DAT_0048234c < uVar7 >> 2);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412750((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 6);
    return;
  case 3:
    puVar4 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 != 0) {
      do {
        cVar3 = FUN_00418bf0();
        uVar2 = DAT_0048234c;
        DAT_0048234c = DAT_0048234c + 1;
        (&uStack00000088)[uVar2] = cVar3;
      } while (DAT_0048234c < uVar8);
    }
    DAT_0048234c = 0;
    if ((uVar8 & 0xffffffc0) != 0) {
      puVar9 = puVar4;
      do {
        puVar11 = (undefined4 *)(&uStack00000088 + DAT_0048234c * 0x40);
        puVar13 = &param_2;
        for (iVar6 = 0x10; iVar6 != 0; iVar6 = iVar6 + -1) {
          *puVar13 = *puVar11;
          puVar11 = puVar11 + 1;
          puVar13 = puVar13 + 1;
        }
        puVar4 = puVar9 + 0x80;
        FUN_0040d3d0(puVar9,&param_2);
        DAT_0048234c = DAT_0048234c + 1;
        puVar9 = puVar4;
      } while (DAT_0048234c < uVar7 >> 2);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412750((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 6);
    return;
  case 4:
    puVar4 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 * 0x18 != 0) {
      do {
        cVar3 = FUN_00418bf0();
        uVar2 = DAT_0048234c;
        DAT_0048234c = DAT_0048234c + 1;
        (&uStack00000088)[uVar2] = cVar3;
      } while (DAT_0048234c < uVar7 * 0x18);
    }
    DAT_0048234c = 0;
    if ((uVar8 & 0xffffffc0) != 0) {
      puVar9 = puVar4;
      do {
        puVar4 = puVar9 + 0x80;
        puVar11 = (undefined4 *)(&uStack00000088 + DAT_0048234c * 0x60);
        puVar13 = &param_2;
        for (iVar6 = 0x18; iVar6 != 0; iVar6 = iVar6 + -1) {
          *puVar13 = *puVar11;
          puVar11 = puVar11 + 1;
          puVar13 = puVar13 + 1;
        }
        FUN_0040d3d0(puVar9,&param_2);
        DAT_0048234c = DAT_0048234c + 1;
        puVar9 = puVar4;
      } while (DAT_0048234c < uVar7 >> 2);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412750((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 6);
    return;
  case 5:
    pcVar5 = FUN_0040d3c0((&DAT_0042bb8b)[in_stack_0000208c * 4]);
    DAT_0048234c = 0;
    if (uVar7 != 0) {
      pcVar12 = pcVar5;
      do {
        pcVar5 = pcVar12 + 1;
        cVar3 = FUN_00418bf0();
        *pcVar12 = cVar3;
        DAT_0048234c = DAT_0048234c + 1;
        pcVar12 = pcVar5;
      } while (DAT_0048234c < uVar7 * 0x20);
    }
    if ((uVar14 != 0) && (DAT_0048234c = 0, (uVar14 & 0x7fffffff) != 0)) {
      do {
        *pcVar5 = '\0';
        pcVar5 = pcVar5 + 1;
        DAT_0048234c = DAT_0048234c + 1;
      } while (DAT_0048234c < uVar14 * 2);
    }
    FUN_00412750((uint)(byte)(&DAT_0042bb8b)[in_stack_0000208c * 4] << 8,uVar8 + uVar14 >> 6);
  }
  return;
}



/* ================= FUN_0040d3c0 @ 0040d3c0 ================= */

undefined * __cdecl FUN_0040d3c0(byte param_1)

{
  return &DAT_00484730 + (uint)param_1 * 0x200;
}



/* ================= FUN_0040d3d0 @ 0040d3d0 ================= */

void __cdecl FUN_0040d3d0(undefined1 *param_1,undefined4 *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  ushort uVar6;
  undefined1 *puVar7;
  undefined4 *puVar8;
  undefined1 local_84 [4];
  undefined4 local_80 [32];
  
  puVar8 = local_80;
  for (iVar5 = 0x20; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar8 = *param_2;
    param_2 = param_2 + 1;
    puVar8 = puVar8 + 1;
  }
  puVar8 = local_80;
  local_84 = (undefined1  [4])0x2;
  do {
    iVar5 = 0x10;
    puVar7 = param_1;
    do {
      bVar1 = *(byte *)((int)puVar8 + 1);
      bVar2 = *(byte *)((int)puVar8 + 0x21);
      bVar3 = *(byte *)((int)puVar8 + 0x41);
      bVar4 = *(byte *)((int)puVar8 + 0x61);
      uVar6 = 0;
      if ((bVar4 & 0x80) != 0) {
        uVar6 = 0x8000;
      }
      if ((bVar3 & 0x80) != 0) {
        uVar6 = uVar6 | 0x4000;
      }
      if ((bVar2 & 0x80) != 0) {
        uVar6 = uVar6 | 0x2000;
      }
      if ((bVar1 & 0x80) != 0) {
        uVar6 = uVar6 | 0x1000;
      }
      if ((bVar4 & 0x40) != 0) {
        uVar6 = uVar6 | 0x800;
      }
      if ((bVar3 & 0x40) != 0) {
        uVar6 = uVar6 | 0x400;
      }
      if ((bVar2 & 0x40) != 0) {
        uVar6 = uVar6 | 0x200;
      }
      if ((bVar1 & 0x40) != 0) {
        uVar6 = uVar6 | 0x100;
      }
      if ((bVar4 & 0x20) != 0) {
        uVar6 = uVar6 | 0x80;
      }
      if ((bVar3 & 0x20) != 0) {
        uVar6 = uVar6 | 0x40;
      }
      if ((bVar2 & 0x20) != 0) {
        uVar6 = uVar6 | 0x20;
      }
      if ((bVar1 & 0x20) != 0) {
        uVar6 = uVar6 | 0x10;
      }
      if ((bVar4 & 0x10) != 0) {
        uVar6 = uVar6 | 8;
      }
      if ((bVar3 & 0x10) != 0) {
        uVar6 = uVar6 | 4;
      }
      if ((bVar2 & 0x10) != 0) {
        uVar6 = uVar6 | 2;
      }
      if ((bVar1 & 0x10) != 0) {
        uVar6 = uVar6 | 1;
      }
      *puVar7 = (char)(uVar6 >> 8);
      puVar7[1] = (char)uVar6;
      uVar6 = 0;
      if ((bVar4 & 8) != 0) {
        uVar6 = 0x8000;
      }
      if ((bVar3 & 8) != 0) {
        uVar6 = uVar6 | 0x4000;
      }
      if ((bVar2 & 8) != 0) {
        uVar6 = uVar6 | 0x2000;
      }
      if ((bVar1 & 8) != 0) {
        uVar6 = uVar6 | 0x1000;
      }
      if ((bVar4 & 4) != 0) {
        uVar6 = uVar6 | 0x800;
      }
      if ((bVar3 & 4) != 0) {
        uVar6 = uVar6 | 0x400;
      }
      if ((bVar2 & 4) != 0) {
        uVar6 = uVar6 | 0x200;
      }
      if ((bVar1 & 4) != 0) {
        uVar6 = uVar6 | 0x100;
      }
      if ((bVar4 & 2) != 0) {
        uVar6 = uVar6 | 0x80;
      }
      if ((bVar3 & 2) != 0) {
        uVar6 = uVar6 | 0x40;
      }
      if ((bVar2 & 2) != 0) {
        uVar6 = uVar6 | 0x20;
      }
      if ((bVar1 & 2) != 0) {
        uVar6 = uVar6 | 0x10;
      }
      if ((bVar4 & 1) != 0) {
        uVar6 = uVar6 | 8;
      }
      if ((bVar3 & 1) != 0) {
        uVar6 = uVar6 | 4;
      }
      if ((bVar2 & 1) != 0) {
        uVar6 = uVar6 | 2;
      }
      if ((bVar1 & 1) != 0) {
        uVar6 = uVar6 | 1;
      }
      puVar7[2] = (char)(uVar6 >> 8);
      param_1 = puVar7 + 4;
      puVar8 = (undefined4 *)((int)puVar8 + 2);
      iVar5 = iVar5 + -1;
      puVar7[3] = (char)uVar6;
      puVar7 = param_1;
    } while (iVar5 != 0);
    puVar8 = (undefined4 *)(local_84 + 3);
    local_84 = (undefined1  [4])((int)local_84 + -1);
  } while (local_84 != (undefined1  [4])0x0);
  return;
}



/* ================= FUN_0040d590 @ 0040d590 ================= */

void __cdecl FUN_0040d590(int param_1,undefined4 *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  byte bVar5;
  byte *pbVar6;
  byte local_94;
  byte local_90;
  int local_88;
  int local_84;
  byte local_80 [128];
  
  pbVar6 = local_80;
  for (iVar4 = 0x20; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pbVar6 = *param_2;
    param_2 = param_2 + 1;
    pbVar6 = pbVar6 + 4;
  }
  pbVar6 = local_80;
  local_84 = 2;
  iVar4 = param_1;
  do {
    local_88 = 0x10;
    do {
      bVar1 = pbVar6[1];
      bVar5 = 0;
      bVar3 = 0;
      bVar2 = *pbVar6;
      local_90 = 0;
      local_94 = 0;
      if ((bVar2 & 0x80) != 0) {
        bVar5 = 0x80;
      }
      if ((bVar2 & 0x40) != 0) {
        local_90 = 0x80;
      }
      if ((bVar2 & 0x20) != 0) {
        local_94 = 0x80;
      }
      if ((bVar2 & 1) != 0) {
        bVar3 = 0x80;
      }
      if ((bVar2 & 8) != 0) {
        bVar5 = bVar5 | 0x40;
      }
      if ((bVar2 & 4) != 0) {
        local_90 = local_90 | 0x40;
      }
      if ((bVar2 & 2) != 0) {
        local_94 = local_94 | 0x40;
      }
      if ((bVar2 & 1) != 0) {
        bVar3 = bVar3 | 0x40;
      }
      if ((bVar1 & 0x80) != 0) {
        bVar5 = bVar5 | 0x20;
      }
      if ((bVar1 & 0x40) != 0) {
        local_90 = local_90 | 0x20;
      }
      if ((bVar1 & 0x20) != 0) {
        local_94 = local_94 | 0x20;
      }
      if ((bVar1 & 0x10) != 0) {
        bVar3 = bVar3 | 0x20;
      }
      if ((bVar1 & 8) != 0) {
        bVar5 = bVar5 | 0x10;
      }
      if ((bVar1 & 4) != 0) {
        local_90 = local_90 | 0x10;
      }
      if ((bVar1 & 2) != 0) {
        local_94 = local_94 | 0x10;
      }
      if ((bVar1 & 1) != 0) {
        bVar3 = bVar3 | 0x10;
      }
      bVar1 = pbVar6[3];
      bVar2 = pbVar6[2];
      pbVar6 = pbVar6 + 4;
      if ((bVar2 & 0x80) != 0) {
        bVar5 = bVar5 | 8;
      }
      if ((bVar2 & 0x40) != 0) {
        local_90 = local_90 | 8;
      }
      if ((bVar2 & 0x20) != 0) {
        local_94 = local_94 | 8;
      }
      if ((bVar2 & 1) != 0) {
        bVar3 = bVar3 | 8;
      }
      if ((bVar2 & 8) != 0) {
        bVar5 = bVar5 | 4;
      }
      if ((bVar2 & 4) != 0) {
        local_90 = local_90 | 4;
      }
      if ((bVar2 & 2) != 0) {
        local_94 = local_94 | 4;
      }
      if ((bVar2 & 1) != 0) {
        bVar3 = bVar3 | 4;
      }
      if ((bVar1 & 0x80) != 0) {
        bVar5 = bVar5 | 2;
      }
      if ((bVar1 & 0x40) != 0) {
        local_90 = local_90 | 2;
      }
      if ((bVar1 & 0x20) != 0) {
        local_94 = local_94 | 2;
      }
      if ((bVar1 & 0x10) != 0) {
        bVar3 = bVar3 | 2;
      }
      if ((bVar1 & 8) != 0) {
        bVar5 = bVar5 | 1;
      }
      if ((bVar1 & 4) != 0) {
        local_90 = local_90 | 1;
      }
      if ((bVar1 & 2) != 0) {
        local_94 = local_94 | 1;
      }
      if ((bVar1 & 1) != 0) {
        bVar3 = bVar3 | 1;
      }
      *(byte *)(iVar4 + 1) = bVar3;
      *(byte *)(iVar4 + 0x21) = local_94;
      *(byte *)(iVar4 + 0x41) = local_90;
      local_88 = local_88 + -1;
      *(byte *)(iVar4 + 0x61) = bVar5;
      iVar4 = iVar4 + 2;
    } while (local_88 != 0);
    iVar4 = param_1 + -1;
    local_84 = local_84 + -1;
  } while (local_84 != 0);
  return;
}



/* ================= FUN_0040d770 @ 0040d770 ================= */

void __cdecl FUN_0040d770(undefined1 *param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  
  iVar6 = 8;
  do {
    bVar1 = *param_2;
    bVar2 = param_2[1];
    bVar3 = param_2[0x10];
    bVar4 = param_2[0x11];
    param_2 = param_2 + 2;
    uVar5 = 0;
    if ((bVar4 & 0x80) != 0) {
      uVar5 = 0x80000000;
    }
    if ((bVar3 & 0x80) != 0) {
      uVar5 = uVar5 | 0x40000000;
    }
    if ((bVar2 & 0x80) != 0) {
      uVar5 = uVar5 | 0x20000000;
    }
    if ((bVar1 & 0x80) != 0) {
      uVar5 = uVar5 | 0x10000000;
    }
    if ((bVar4 & 0x40) != 0) {
      uVar5 = uVar5 | 0x800000;
    }
    if ((bVar3 & 0x40) != 0) {
      uVar5 = uVar5 | 0x400000;
    }
    if ((bVar2 & 0x40) != 0) {
      uVar5 = uVar5 | 0x200000;
    }
    if ((bVar1 & 0x40) != 0) {
      uVar5 = uVar5 | 0x100000;
    }
    if ((bVar4 & 0x20) != 0) {
      uVar5 = uVar5 | 0x8000;
    }
    if ((bVar3 & 0x20) != 0) {
      uVar5 = uVar5 | 0x4000;
    }
    if ((bVar2 & 0x20) != 0) {
      uVar5 = uVar5 | 0x2000;
    }
    if ((bVar1 & 0x20) != 0) {
      uVar5 = uVar5 | 0x1000;
    }
    if ((bVar4 & 0x10) != 0) {
      uVar5 = uVar5 | 0x80;
    }
    if ((bVar3 & 0x10) != 0) {
      uVar5 = uVar5 | 0x40;
    }
    if ((bVar2 & 0x10) != 0) {
      uVar5 = uVar5 | 0x20;
    }
    if ((bVar1 & 0x10) != 0) {
      uVar5 = uVar5 | 0x10;
    }
    if ((bVar4 & 8) != 0) {
      uVar5 = uVar5 | 0x8000000;
    }
    if ((bVar3 & 8) != 0) {
      uVar5 = uVar5 | 0x4000000;
    }
    if ((bVar2 & 8) != 0) {
      uVar5 = uVar5 | 0x2000000;
    }
    if ((bVar1 & 8) != 0) {
      uVar5 = uVar5 | 0x1000000;
    }
    if ((bVar4 & 4) != 0) {
      uVar5 = uVar5 | 0x80000;
    }
    if ((bVar3 & 4) != 0) {
      uVar5 = uVar5 | 0x40000;
    }
    if ((bVar2 & 4) != 0) {
      uVar5 = uVar5 | 0x20000;
    }
    if ((bVar1 & 4) != 0) {
      uVar5 = uVar5 | 0x10000;
    }
    if ((bVar4 & 2) != 0) {
      uVar5 = uVar5 | 0x800;
    }
    if ((bVar3 & 2) != 0) {
      uVar5 = uVar5 | 0x400;
    }
    if ((bVar2 & 2) != 0) {
      uVar5 = uVar5 | 0x200;
    }
    if ((bVar1 & 2) != 0) {
      uVar5 = uVar5 | 0x100;
    }
    if ((bVar4 & 1) != 0) {
      uVar5 = uVar5 | 8;
    }
    if ((bVar3 & 1) != 0) {
      uVar5 = uVar5 | 4;
    }
    if ((bVar2 & 1) != 0) {
      uVar5 = uVar5 | 2;
    }
    if ((bVar1 & 1) != 0) {
      uVar5 = uVar5 | 1;
    }
    *param_1 = (char)(uVar5 >> 0x18);
    iVar6 = iVar6 + -1;
    param_1[1] = (char)(uVar5 >> 0x10);
    param_1[2] = (char)(uVar5 >> 8);
    param_1[3] = (char)uVar5;
    param_1 = param_1 + 4;
  } while (iVar6 != 0);
  return;
}



/* ================= FUN_0040d920 @ 0040d920 ================= */

void __cdecl FUN_0040d920(int param_1)

{
  char cVar1;
  byte bVar2;
  undefined3 extraout_var;
  int iVar3;
  
  FUN_0040da00(0x6000);
  iVar3 = 0x200;
  DAT_00481f94 = FUN_0040cd50(param_1 + 0x1a);
  FUN_00418bb0();
  do {
    cVar1 = FUN_00418bf0();
    bVar2 = FUN_00418bf0();
    FUN_0040da20((ushort)CONCAT31(extraout_var,cVar1) | (ushort)bVar2 << 8);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  FUN_00412750(0x6000,8);
  return;
}



/* ================= FUN_0040d990 @ 0040d990 ================= */

void FUN_0040d990(void)

{
  char cVar1;
  byte bVar2;
  undefined3 extraout_var;
  int iVar3;
  int iVar4;
  
  iVar4 = 0;
  FUN_0040da00(0x6000);
  do {
    iVar3 = 0x100;
    DAT_00481f94 = FUN_0040cd50(iVar4 + 0x1a);
    FUN_00418bb0();
    do {
      cVar1 = FUN_00418bf0();
      bVar2 = FUN_00418bf0();
      FUN_0040da20((ushort)CONCAT31(extraout_var,cVar1) | (ushort)bVar2 << 8);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x17);
  FUN_00412750(0x6000,0x5c);
  return;
}



/* ================= FUN_0040da00 @ 0040da00 ================= */

void __cdecl FUN_0040da00(undefined2 param_1)

{
  DAT_00494730 = param_1;
  return;
}



/* ================= FUN_0040da10 @ 0040da10 ================= */

void __cdecl FUN_0040da10(undefined2 param_1)

{
  DAT_00494734 = param_1;
  return;
}



/* ================= FUN_0040da20 @ 0040da20 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0040da20(undefined2 param_1)

{
  *(undefined2 *)(&DAT_00484730 + (uint)DAT_00494730 * 2) = param_1;
  DAT_00494730 = DAT_00494730 + _DAT_00494732;
  return;
}



/* ================= FUN_0040da50 @ 0040da50 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0040da50(undefined2 param_1)

{
  *(ushort *)(&DAT_00484730 + (uint)DAT_00494730 * 2) =
       CONCAT11((char)param_1,(char)((ushort)param_1 >> 8));
  DAT_00494730 = DAT_00494730 + _DAT_00494732;
  return;
}



/* ================= FUN_0040da80 @ 0040da80 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_0040da80(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_00494734;
  DAT_00494734 = DAT_00494734 + _DAT_00494732;
  return *(undefined2 *)(&DAT_00484730 + uVar1 * 2);
}



/* ================= FUN_0040daa0 @ 0040daa0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 FUN_0040daa0(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_00494734;
  DAT_00494734 = DAT_00494734 + _DAT_00494732;
  return CONCAT11((char)*(undefined2 *)(&DAT_00484730 + uVar1 * 2),
                  (char)((ushort)*(undefined2 *)(&DAT_00484730 + uVar1 * 2) >> 8));
}



/* ================= FUN_0040dad0 @ 0040dad0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040dad0(void)

{
  _DAT_00494732 = 1;
  return;
}



/* ================= FUN_0040dae0 @ 0040dae0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040dae0(void)

{
  _DAT_00494732 = 0x40;
  return;
}



/* ================= FUN_0040daf0 @ 0040daf0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0040daf0(ushort param_1)

{
  if (DAT_00494730 < 0x800) {
    *(ushort *)(&DAT_00484730 + (uint)DAT_00494730 * 2) = param_1;
  }
  if (0xfff < DAT_00494730) {
    *(ushort *)(&DAT_00484730 + (uint)DAT_00494730 * 2) = param_1;
    DAT_00494730 = DAT_00494730 + _DAT_00494732;
    return;
  }
  if (DAT_00494730 < 0x800) {
    (&DAT_00494740)[DAT_00494730] = (param_1 & 0x7000) >> 1 | param_1 & 0x7ff;
  }
  DAT_00494730 = DAT_00494730 + _DAT_00494732;
  return;
}



/* ================= FUN_0040db70 @ 0040db70 ================= */

void FUN_0040db70(void)

{
  int iVar1;
  
  iVar1 = 0x800;
  FUN_0040da00(0);
  do {
    FUN_0040daf0(0x90);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



/* ================= FUN_0040dba0 @ 0040dba0 ================= */

void __cdecl FUN_0040dba0(int param_1,int param_2,int param_3,int param_4,byte param_5)

{
  int iVar1;
  int iVar2;
  
  if (param_2 < param_4 + param_2) {
    iVar2 = param_2;
    do {
      FUN_0040da00((short)iVar2 * 0x40 + (short)param_1);
      if (param_1 < param_3 + param_1) {
        iVar1 = param_3;
        do {
          FUN_0040daf0((ushort)CONCAT31((uint3)param_5,0x90));
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < param_2 + param_4);
  }
  return;
}



/* ================= FUN_0040dc10 @ 0040dc10 ================= */

int __cdecl FUN_0040dc10(ushort param_1)

{
  int iVar1;
  int iVar2;
  undefined4 local_4;
  
  local_4 = CONCAT22(local_4._2_2_,param_1);
  iVar1 = local_4;
  local_4._1_1_ = (byte)(param_1 >> 8);
  local_4._0_1_ = (byte)param_1;
  switch((uint)local_4._1_1_) {
  case 0x81:
    break;
  case 0x82:
    if (((byte)local_4 < 0x4f) || (0x58 < (byte)local_4)) {
      if (((byte)local_4 < 0x60) || (0x79 < (byte)local_4)) {
        if (((byte)local_4 < 0x81) || (0x9a < (byte)local_4)) {
          if ((0x9e < (byte)local_4) && ((byte)local_4 < 0xf2)) {
            iVar1 = 10;
          }
        }
        else {
          iVar1 = 9;
        }
      }
      else {
        iVar1 = 8;
      }
    }
    else {
      iVar1 = 7;
    }
    return (uint)param_1 +
           ((uint)(ushort)(&DAT_0042bc7a)[iVar1 * 2] - (uint)(ushort)(&DAT_0042bc78)[iVar1 * 2]);
  case 0x83:
    if (((byte)local_4 < 0x40) || (0x7e < (byte)local_4)) {
      if (((byte)local_4 < 0x80) || (0x96 < (byte)local_4)) {
        if (((byte)local_4 < 0x9f) || (0xb6 < (byte)local_4)) {
          if ((0xbe < (byte)local_4) && ((byte)local_4 < 0xd7)) {
            iVar1 = 0xe;
          }
        }
        else {
          iVar1 = 0xd;
        }
      }
      else {
        iVar1 = 0xc;
      }
    }
    else {
      iVar1 = 0xb;
    }
    return (uint)param_1 +
           ((uint)(ushort)(&DAT_0042bc7a)[iVar1 * 2] - (uint)(ushort)(&DAT_0042bc78)[iVar1 * 2]);
  case 0x84:
    if (((byte)local_4 < 0x40) || (0x60 < (byte)local_4)) {
      if (((byte)local_4 < 0x70) || (0x7e < (byte)local_4)) {
        if (((byte)local_4 < 0x80) || (0x91 < (byte)local_4)) {
          if ((0x9e < (byte)local_4) && ((byte)local_4 < 0xbf)) {
            iVar1 = 0x12;
          }
        }
        else {
          iVar1 = 0x11;
        }
      }
      else {
        iVar1 = 0x10;
      }
    }
    else {
      iVar1 = 0xf;
    }
    return (uint)param_1 +
           ((uint)(ushort)(&DAT_0042bc7a)[iVar1 * 2] - (uint)(ushort)(&DAT_0042bc78)[iVar1 * 2]);
  default:
    iVar1 = ((-0x110 - (uint)((byte)local_4 < 0x7f)) + (uint)local_4._1_1_ * 2) * 4;
    return (uint)param_1 +
           ((uint)*(ushort *)(&DAT_0042bcca + iVar1) - (uint)*(ushort *)(&DAT_0042bcc8 + iVar1));
  }
  if (((byte)local_4 < 0x40) || (0x7e < (byte)local_4)) {
    if (((byte)local_4 < 0x80) || (0xac < (byte)local_4)) {
      if (((byte)local_4 < 0xb8) || (0xbf < (byte)local_4)) {
        if (((byte)local_4 < 200) || (0xce < (byte)local_4)) {
          if (((byte)local_4 < 0xda) || (0xe8 < (byte)local_4)) {
            if (((byte)local_4 < 0xf0) || (0xf7 < (byte)local_4)) {
              iVar2 = 6;
              if ((byte)local_4 != 0xfc) {
                iVar2 = iVar1;
              }
            }
            else {
              iVar2 = 5;
            }
          }
          else {
            iVar2 = 4;
          }
        }
        else {
          iVar2 = 3;
        }
      }
      else {
        iVar2 = 2;
      }
    }
    else {
      iVar2 = 1;
    }
  }
  else {
    iVar2 = 0;
  }
  return (uint)param_1 +
         ((uint)(ushort)(&DAT_0042bc7a)[iVar2 * 2] - (uint)(ushort)(&DAT_0042bc78)[iVar2 * 2]);
}



/* ================= FUN_0040deb0 @ 0040deb0 ================= */

undefined1 * __cdecl FUN_0040deb0(undefined1 *param_1)

{
  byte *pbVar1;
  ushort uVar2;
  int iVar3;
  byte *pbVar4;
  byte *pbVar5;
  undefined *puVar6;
  undefined4 *puVar7;
  
  puVar7 = (undefined4 *)&DAT_00480750;
  for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  uVar2 = CONCAT11(*param_1,param_1[1]);
  if ((uVar2 < 0x8140) || (0x84be < uVar2)) {
    if (uVar2 < 0x889f) {
      return &DAT_00480750;
    }
    if (0x9872 < uVar2) {
      return &DAT_00480750;
    }
    puVar6 = &DAT_0046f3b1;
  }
  else {
    puVar6 = &DAT_0046d0e5;
  }
  iVar3 = FUN_0040dc10(uVar2);
  pbVar1 = puVar6 + iVar3 * 0x11 + 1;
  pbVar4 = &DAT_00480750;
  do {
    pbVar5 = pbVar4 + 4;
    *pbVar4 = pbVar1[-1];
    pbVar4[1] = *pbVar1 & 0xf0;
    pbVar4[2] = pbVar1[1];
    pbVar4[3] = *pbVar1 << 4;
    pbVar1 = pbVar1 + 3;
    pbVar4 = pbVar5;
  } while (pbVar5 < &DAT_00480768);
  return &DAT_00480750;
}



/* ================= FUN_0040df50 @ 0040df50 ================= */

void __cdecl FUN_0040df50(undefined1 param_1)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  byte *pbVar4;
  
  iVar3 = 8;
  pbVar2 = FUN_0040deb0(&param_1);
  pbVar4 = pbVar2;
  do {
    bVar1 = *pbVar4;
    DAT_00482348 = (uint)bVar1;
    DAT_0048234c = 0;
    if ((bVar1 & 0x80) != 0) {
      DAT_0048234c = 0x10000000;
    }
    if ((bVar1 & 0x40) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100000;
    }
    if ((bVar1 & 0x20) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000;
    }
    if ((bVar1 & 0x10) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10;
    }
    if ((bVar1 & 8) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000000;
    }
    if ((bVar1 & 4) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10000;
    }
    if ((bVar1 & 2) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100;
    }
    if ((bVar1 & 1) != 0) {
      DAT_0048234c = DAT_0048234c | 1;
    }
    pbVar4 = pbVar4 + 2;
    FUN_0040da50((short)(DAT_0048234c >> 0x10));
    FUN_0040da50((short)DAT_0048234c);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  pbVar4 = pbVar2 + 1;
  iVar3 = 8;
  do {
    bVar1 = *pbVar4;
    DAT_00482348 = (uint)bVar1;
    DAT_0048234c = 0;
    if ((bVar1 & 0x80) != 0) {
      DAT_0048234c = 0x10000000;
    }
    if ((bVar1 & 0x40) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100000;
    }
    if ((bVar1 & 0x20) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000;
    }
    if ((bVar1 & 0x10) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10;
    }
    if ((bVar1 & 8) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000000;
    }
    if ((bVar1 & 4) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10000;
    }
    if ((bVar1 & 2) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100;
    }
    if ((bVar1 & 1) != 0) {
      DAT_0048234c = DAT_0048234c | 1;
    }
    pbVar4 = pbVar4 + 2;
    FUN_0040da50((short)(DAT_0048234c >> 0x10));
    FUN_0040da50((short)DAT_0048234c);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  pbVar4 = pbVar2 + 0x10;
  iVar3 = 3;
  do {
    bVar1 = *pbVar4;
    DAT_00482348 = (uint)bVar1;
    DAT_0048234c = 0;
    if ((bVar1 & 0x80) != 0) {
      DAT_0048234c = 0x10000000;
    }
    if ((bVar1 & 0x40) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100000;
    }
    if ((bVar1 & 0x20) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000;
    }
    if ((bVar1 & 0x10) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10;
    }
    if ((bVar1 & 8) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000000;
    }
    if ((bVar1 & 4) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10000;
    }
    if ((bVar1 & 2) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100;
    }
    if ((bVar1 & 1) != 0) {
      DAT_0048234c = DAT_0048234c | 1;
    }
    pbVar4 = pbVar4 + 2;
    FUN_0040da50((short)(DAT_0048234c >> 0x10));
    FUN_0040da50((short)DAT_0048234c);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  iVar3 = 5;
  do {
    FUN_0040da20(0);
    FUN_0040da20(0);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  pbVar2 = pbVar2 + 0x11;
  iVar3 = 3;
  do {
    bVar1 = *pbVar2;
    DAT_00482348 = (uint)bVar1;
    DAT_0048234c = 0;
    if ((bVar1 & 0x80) != 0) {
      DAT_0048234c = 0x10000000;
    }
    if ((bVar1 & 0x40) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100000;
    }
    if ((bVar1 & 0x20) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000;
    }
    if ((bVar1 & 0x10) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10;
    }
    if ((bVar1 & 8) != 0) {
      DAT_0048234c = DAT_0048234c | 0x1000000;
    }
    if ((bVar1 & 4) != 0) {
      DAT_0048234c = DAT_0048234c | 0x10000;
    }
    if ((bVar1 & 2) != 0) {
      DAT_0048234c = DAT_0048234c | 0x100;
    }
    if ((bVar1 & 1) != 0) {
      DAT_0048234c = DAT_0048234c | 1;
    }
    pbVar2 = pbVar2 + 2;
    FUN_0040da50((short)(DAT_0048234c >> 0x10));
    FUN_0040da50((short)DAT_0048234c);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  iVar3 = 5;
  do {
    FUN_0040da20(0);
    FUN_0040da20(0);
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return;
}



/* ================= FUN_0040e300 @ 0040e300 ================= */

void __cdecl FUN_0040e300(byte param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  undefined1 uVar4;
  uint uVar5;
  int iVar6;
  char cVar7;
  uint uVar8;
  short sVar9;
  
  iVar3 = DAT_00482c20;
  cVar7 = '\x10';
  uVar8 = (uint)param_1;
  (&DAT_00482290)[uVar8] = 0;
  uVar5 = (uint)(byte)(&DAT_004820d0)[uVar8];
  iVar6 = uVar5 * 0x40;
  iVar2 = (&DAT_004823ac)[uVar5 * 0x10];
  *(undefined2 *)(iVar3 + 0x10) = *(undefined2 *)(iVar2 + 0x10);
  sVar9 = *(short *)(DAT_00482c20 + 0x10);
  if (sVar9 < 0xb0) {
    if (sVar9 < 0xa0) {
      sVar9 = sVar9 + -4;
      cVar7 = '\t';
    }
    else {
      sVar9 = sVar9 + -5;
    }
  }
  else {
    sVar9 = sVar9 + -8;
  }
  *(short *)(DAT_00482c20 + 0x10) = sVar9;
  if (DAT_00481dbc != '\0') {
    cVar7 = cVar7 + '\x06';
    *(short *)(DAT_00482c20 + 0x10) = *(short *)(DAT_00482c20 + 0x10) + -8;
  }
  uVar4 = 6;
  if (0x7e < (short)(&DAT_004823cc)[uVar5 * 0x20]) {
    cVar7 = -cVar7;
    uVar4 = 0x86;
  }
  (&DAT_00482090)[uVar8] = uVar4;
  *(short *)(DAT_00482c20 + 0xc) = *(short *)(iVar2 + 0xc) + (short)cVar7;
  *(undefined *)((int)&DAT_00482050 + uVar8) = (&DAT_004823c4)[iVar6];
  (&DAT_00482110)[uVar8] = (&DAT_004823ca)[iVar6];
  *(undefined2 *)(&DAT_00480768 + uVar8 * 2) = (&DAT_004823c6)[uVar5 * 0x20] << 7;
  *(short *)(&DAT_004807e8 + uVar8 * 2) = *(short *)(&DAT_004823c8 + iVar6) << 7;
  (&DAT_004823c6)[uVar5 * 0x20] = 0xff;
  FUN_0041c020(DAT_00482c20,0xc);
  *(undefined1 *)(DAT_00482c20 + 10) = 0x40;
  if (*(short *)(DAT_00482c20 + 0x10) < 0xb0) {
    (&DAT_00482290)[uVar8] = 0x80;
    bVar1 = *(byte *)((int)&DAT_00482050 + uVar8);
    if (bVar1 != 0x75) {
      if (bVar1 < 0x7a) {
        if (bVar1 < 0x73) {
          *(byte *)((int)&DAT_00482050 + uVar8) = bVar1 + 5;
          return;
        }
        *(byte *)((int)&DAT_00482050 + uVar8) = bVar1 + 3;
        return;
      }
      if (bVar1 < 0x8a) {
LAB_0040e4d3:
        *(byte *)((int)&DAT_00482050 + uVar8) = bVar1 + 8;
        return;
      }
      if (0x8b < bVar1) {
        if (bVar1 == 0x94) goto LAB_0040e4d3;
        if (bVar1 < 0x94) {
          *(byte *)((int)&DAT_00482050 + uVar8) = bVar1 + 9;
          return;
        }
        if (bVar1 == 0xa7) {
          FUN_0040e550(DAT_00482c20,-0x800);
          FUN_0040e580(DAT_00482c20,-0x600);
          *(undefined1 *)((int)&DAT_00482050 + uVar8) = 0x9b;
          return;
        }
        if (bVar1 == 0xa8) {
          FUN_0040e550(DAT_00482c20,0x800);
          FUN_0040e580(DAT_00482c20,-0x600);
          *(undefined1 *)((int)&DAT_00482050 + uVar8) = 0x96;
          return;
        }
      }
    }
  }
  else {
    if (*(char *)((int)&DAT_00482050 + uVar8) == -0x59) {
      FUN_0040e550(DAT_00482c20,-0x1200);
      FUN_0040e580(DAT_00482c20,-0x800);
      *(undefined1 *)((int)&DAT_00482050 + uVar8) = 0x92;
      return;
    }
    if (*(char *)((int)&DAT_00482050 + uVar8) == -0x58) {
      FUN_0040e550(DAT_00482c20,0x1800);
      FUN_0040e580(DAT_00482c20,-0x800);
      *(undefined1 *)((int)&DAT_00482050 + uVar8) = 0x8d;
    }
  }
  return;
}



/* ================= FUN_0040e550 @ 0040e550 ================= */

void __cdecl FUN_0040e550(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0xe) +
          (*(short *)(param_1 + 0xc) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0xe) = (short)iVar1;
  *(short *)(param_1 + 0xc) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_0040e580 @ 0040e580 ================= */

void __cdecl FUN_0040e580(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0x12) +
          (*(short *)(param_1 + 0x10) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0x12) = (short)iVar1;
  *(short *)(param_1 + 0x10) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_0040e5b0 @ 0040e5b0 ================= */

void __cdecl FUN_0040e5b0(byte param_1)

{
  char cVar1;
  undefined2 uVar2;
  byte bVar3;
  byte bVar4;
  short sVar5;
  byte *pbVar6;
  uint uVar7;
  int iVar8;
  undefined1 uVar9;
  ushort *puVar10;
  int *piVar11;
  ushort *puVar12;
  char local_b;
  byte local_a;
  byte local_9;
  undefined4 *local_8;
  byte *local_4;
  
  local_9 = 0;
  uVar7 = (uint)param_1;
  DAT_00482c20 = &DAT_00482c30 + uVar7 * 6;
  if ((&DAT_004820d0)[uVar7] == '\0') {
    return;
  }
  bVar4 = *(byte *)((int)&DAT_00482050 + uVar7);
  if (bVar4 == 0) goto LAB_0040f06d;
  if (bVar4 < 0x7a) {
LAB_0040e748:
    if (((&DAT_00482090)[uVar7] & 0xf) != 0) {
      if (((&DAT_00482090)[uVar7] & 0xf) == 5) {
        FUN_00412890(0x14);
      }
      bVar4 = (&DAT_00482090)[uVar7] - 1;
      (&DAT_00482090)[uVar7] = bVar4;
      bVar3 = bVar4 & 0xf;
      if (*(char *)((int)&DAT_00482050 + uVar7) != 'u') {
        if (bVar4 < 0x7f) {
          if ((byte)(&DAT_00482290)[uVar7] < 0x7f) {
            uVar9 = (&DAT_0042bd88)[bVar3];
          }
          else {
            uVar9 = (&DAT_0042bd98)[bVar3];
          }
        }
        else if ((byte)(&DAT_00482290)[uVar7] < 0x7f) {
          uVar9 = (&DAT_0042bd90)[bVar3];
        }
        else {
          uVar9 = (&DAT_0042bda0)[bVar3];
        }
        *(undefined1 *)((int)DAT_00482c20 + 9) = uVar9;
        if (bVar3 == 0) {
          (&DAT_00482090)[uVar7] = 0;
          return;
        }
        return;
      }
      if (bVar4 < 0x7f) {
        if ((byte)(&DAT_00482290)[uVar7] < 0x7f) {
          uVar9 = (&DAT_0042bd68)[bVar3];
        }
        else {
          uVar9 = (&DAT_0042bd78)[bVar3];
        }
      }
      else if ((byte)(&DAT_00482290)[uVar7] < 0x7f) {
        uVar9 = (&DAT_0042bd70)[bVar3];
      }
      else {
        uVar9 = (&DAT_0042bd80)[bVar3];
      }
      *(undefined1 *)((int)DAT_00482c20 + 9) = uVar9;
      if (bVar3 == 0) {
        (&DAT_00482090)[uVar7] = 0;
        return;
      }
      return;
    }
LAB_0040e846:
    local_a = (&DAT_00482150)[uVar7];
    local_9 = (&DAT_00482290)[uVar7] & 0x1f;
    if (local_9 != 0) goto LAB_0040f1dd;
  }
  else {
    if (bVar4 < 0x8a) {
      bVar4 = (&DAT_00482090)[uVar7];
      if ((bVar4 & 0xf) != 0) {
        if ((bVar4 & 0xf) == 5) {
          (&DAT_00482090)[uVar7] = bVar4 - 1;
          FUN_00412890(0x13);
        }
        else {
          if ((DAT_004834bc & 3) != 0) {
            return;
          }
          (&DAT_00482090)[uVar7] = bVar4 - 1;
          DAT_0048236c = (uint)(bVar4 - 1 & 0xf) << 8;
          if (*(ushort *)(&DAT_00480768 + uVar7 * 2) < 0x8000) {
            sVar5 = 0x600 - (short)DAT_0048236c;
          }
          else {
            sVar5 = (short)DAT_0048236c + -0x601;
          }
          FUN_0040e550((int)DAT_00482c20,sVar5);
          DAT_0048236c = ((byte)(&DAT_00482090)[uVar7] & 0xf) * 0x100;
          if ((char)(*(ushort *)(&DAT_004807e8 + uVar7 * 2) >> 8) != '\0') {
            if (*(ushort *)(&DAT_004807e8 + uVar7 * 2) < 0x8000) {
              sVar5 = 0x600 - (short)DAT_0048236c;
            }
            else {
              sVar5 = (short)DAT_0048236c + -0x601;
            }
            FUN_0040e580((int)DAT_00482c20,sVar5);
          }
        }
        if (0x3fff < *(ushort *)(&DAT_00480768 + uVar7 * 2)) {
          if (0xb7 < (short)DAT_00482c20[3]) goto LAB_0040f357;
          goto LAB_0040edef;
        }
        if (0x8b < (short)DAT_00482c20[3]) goto LAB_0040edef;
        goto LAB_0040f357;
      }
      goto LAB_0040e846;
    }
    if ((bVar4 < 0x8b) || (0x9c < bVar4)) goto LAB_0040e748;
    bVar3 = (&DAT_00482090)[uVar7] & 0xf;
    bVar4 = (&DAT_00482090)[uVar7] & 0xf;
    if (bVar3 != 0) {
      if (bVar3 == 5) {
        FUN_00412890(0x18);
        bVar4 = 0;
      }
      (&DAT_00482090)[uVar7] = (&DAT_00482090)[uVar7] + -1;
      if ((bVar4 & 1) == 0) {
        *(undefined1 *)((int)DAT_00482c20 + 9) = 0;
        return;
      }
      goto LAB_0040f357;
    }
    *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0xa7;
  }
  pbVar6 = &DAT_00482110 + uVar7;
  bVar4 = *pbVar6;
  if (*(ushort *)(&DAT_00480768 + uVar7 * 2) < 0x8000) {
    if (9 < (bVar4 & 0xf)) {
      if (0xe < bVar4) {
        if (bVar4 < 0xf0) {
LAB_0040ed95:
          if ((DAT_004834bc & 3) == 0) {
            *pbVar6 = *pbVar6 + 0x10;
            return;
          }
          return;
        }
        *pbVar6 = bVar4 & 0xf;
        if ((*(char *)((int)&DAT_00482050 + uVar7) == -0x7f) ||
           (*(char *)((int)&DAT_00482050 + uVar7) == -0x77)) {
          *(undefined2 *)(DAT_00482c20 + 3) = 0xb0;
          *(short *)(DAT_00482c20 + 4) = (short)DAT_00482c20[4] + 0x18;
          *(undefined2 *)(&DAT_004807e8 + uVar7 * 2) = 0xfe00;
          *(byte *)((int)&DAT_00482050 + uVar7) =
               (-(*(byte *)((int)&DAT_00482050 + uVar7) < 0x7f) & 8U) + 0x7a;
        }
        else {
          pbVar6 = (byte *)(DAT_00482c20 + 4);
          if (*pbVar6 < 0x80) {
            pbVar6[0] = 0x86;
            pbVar6[1] = 0;
            *(short *)(DAT_00482c20 + 3) = 0x150 - (short)DAT_00482c20[3];
          }
          else {
            *(undefined2 *)(DAT_00482c20 + 3) = 0xb0;
            *(short *)(DAT_00482c20 + 4) = (short)DAT_00482c20[4] + 0x10;
          }
        }
        cVar1 = *(char *)((int)&DAT_00482050 + uVar7);
        if (cVar1 == 's') {
          *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0x74;
        }
        else if (cVar1 == 'v') {
          *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0x77;
        }
        else if ((((cVar1 == 'z') || (cVar1 == -0x7f)) || (cVar1 == -0x7e)) || (cVar1 == -0x77))
        goto LAB_0040f357;
        *(short *)(&DAT_004807e8 + uVar7 * 2) = -*(short *)(&DAT_004807e8 + uVar7 * 2);
        goto LAB_0040f357;
      }
      if ((short)DAT_00482c20[4] < 0x80) {
        if ((*(char *)((int)&DAT_00482050 + uVar7) != 'z') &&
           (*(char *)((int)&DAT_00482050 + uVar7) != -0x7e)) goto LAB_0040edef;
        goto LAB_0040e9ba;
      }
      if ((short)DAT_00482c20[3] < 0xa8) {
        if (0x8b < (short)DAT_00482c20[3]) {
joined_r0x0040edd1:
          if (9 < (bVar4 & 0xf)) {
LAB_0040edef:
            piVar11 = DAT_00482c20;
            (&DAT_00482110)[uVar7] = (&DAT_00482110)[uVar7] + '\x10';
            *(undefined1 *)((int)piVar11 + 9) = 0;
            return;
          }
        }
        goto LAB_0040eeb4;
      }
    }
    puVar10 = (ushort *)(DAT_00482c20 + 3);
    if ((*puVar10 < 0x120) && (puVar12 = (ushort *)(DAT_00482c20 + 4), *puVar12 < 0xe2)) {
      local_8 = &DAT_004823a0;
      uVar2 = (&DAT_004823cc)[(uint)(byte)(&DAT_004820d0)[uVar7] * 0x20];
      local_9 = 0;
      (&DAT_00482150)[uVar7] = 0;
      local_b = '\0';
      do {
        if (((*(byte *)(local_8 + 0xb) & 0xc0) == 0xc0) &&
           ((*(byte *)(local_8 + 0xb) & 7) == ((byte)uVar2 & 7))) {
          iVar8 = local_8[3];
          if ((*(char *)((int)&DAT_00482050 + uVar7) == 'z') ||
             (*(char *)((int)&DAT_00482050 + uVar7) == -0x7e)) {
            DAT_00482348 = (int)*(short *)(iVar8 + 0x10) + 0x10;
            if ((uint)(int)(short)*puVar12 < DAT_00482348) {
              DAT_00482348 = (int)*(short *)(iVar8 + 0xc) - 10;
              if (DAT_00482348 < (uint)(int)(short)*puVar10) goto LAB_0040f119;
              FUN_0040e550((int)DAT_00482c20,0x800);
              *(byte *)((int)DAT_00482c20 + 9) =
                   (-((byte)(&DAT_00482290)[uVar7] < 0x7f) & 0xf8U) + 0x83;
              return;
            }
            FUN_0040e580((int)DAT_00482c20,-0x600);
            FUN_0040e550((int)DAT_00482c20,0x300);
            goto LAB_0040f357;
          }
          DAT_00482348 = ((int)*(short *)(iVar8 + 0x10) - (int)(short)*puVar12) + 8;
          if (DAT_00482348 < 0x14) {
            DAT_00482348 = ((int)(short)*puVar10 - (int)*(short *)(iVar8 + 0xc)) + 0x10;
            if ((*(short *)(iVar8 + 0xc) + -0x10 <= (int)(short)*puVar10) && (DAT_00482348 < 0x1e))
            goto LAB_0040f119;
            if (0xdd < (short)*puVar12) {
              *puVar12 = 0xda;
            }
          }
          else if (*(short *)(iVar8 + 0x10) < (short)*puVar12) {
            if (DAT_00481dbc == '\0') {
              FUN_0040e580((int)DAT_00482c20,-0x200);
            }
            else {
              FUN_0040e580((int)DAT_00482c20,-0x400);
            }
          }
          else if ((short)*puVar12 < 0xdb) {
            if (DAT_00481dbc == '\0') {
              FUN_0040e580((int)DAT_00482c20,0x100);
            }
            else {
              FUN_0040e580((int)DAT_00482c20,0x400);
            }
          }
          else {
            *puVar12 = 0xd8;
          }
          goto LAB_0040eeb4;
        }
        local_8 = local_8 + 0x10;
        local_b = local_b + '\x01';
      } while (local_b != '\"');
      if ((((0xb7 < (byte)*puVar12) || (*(char *)((int)&DAT_00482050 + uVar7) == -0x7f)) ||
          (*(char *)((int)&DAT_00482050 + uVar7) == -0x77)) ||
         ((bVar4 = FUN_0040cb50(), 3 < (bVar4 & 7) || ((short)DAT_00482c20[3] < 0x106)))) {
LAB_0040eeb4:
        FUN_0040e580((int)DAT_00482c20,*(short *)(&DAT_004807e8 + uVar7 * 2));
        FUN_0040e550((int)DAT_00482c20,*(ushort *)(&DAT_00480768 + uVar7 * 2));
        goto LAB_0040f357;
      }
    }
  }
  else {
    if (9 < (bVar4 & 0xf)) {
      if (0xe < bVar4) {
        if (bVar4 < 0xf0) goto LAB_0040ed95;
        *pbVar6 = bVar4 & 0xf;
        if ((*(char *)((int)&DAT_00482050 + uVar7) == -0x80) ||
           (*(char *)((int)&DAT_00482050 + uVar7) == -0x78)) {
          *(undefined2 *)(DAT_00482c20 + 3) = 0x92;
          *(short *)(DAT_00482c20 + 4) = (short)DAT_00482c20[4] + 0x18;
          *(undefined2 *)(&DAT_004807e8 + uVar7 * 2) = 0xfe00;
          *(byte *)((int)&DAT_00482050 + uVar7) =
               (-((byte)(&DAT_00482290)[uVar7] < 0x7f) & 0xf8U) + 0x85;
        }
        else {
          pbVar6 = (byte *)(DAT_00482c20 + 4);
          if (*pbVar6 < 0x80) {
            pbVar6[0] = 0x86;
            pbVar6[1] = 0;
            *(short *)(DAT_00482c20 + 3) = 0x130 - (short)DAT_00482c20[3];
          }
          else {
            *(undefined2 *)(DAT_00482c20 + 3) = 0x92;
            *(short *)(DAT_00482c20 + 4) = (short)DAT_00482c20[4] + 0x10;
          }
        }
        cVar1 = *(char *)((int)&DAT_00482050 + uVar7);
        if (cVar1 == 'r') {
          *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0x71;
        }
        else if (cVar1 == 'w') {
          *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0x76;
        }
        else if ((((cVar1 == '}') || (cVar1 == -0x80)) || (cVar1 == -0x7b)) || (cVar1 == -0x78))
        goto LAB_0040f357;
        *(short *)(&DAT_004807e8 + uVar7 * 2) = -*(short *)(&DAT_004807e8 + uVar7 * 2);
        goto LAB_0040f357;
      }
      if (*(byte *)(DAT_00482c20 + 4) < 0x80) {
        if ((*(char *)((int)&DAT_00482050 + uVar7) != '}') &&
           (*(char *)((int)&DAT_00482050 + uVar7) != -0x7b)) goto LAB_0040edef;
        goto LAB_0040e9ba;
      }
      if (0x97 < (short)DAT_00482c20[3]) {
        if ((short)DAT_00482c20[3] < 0xb8) goto joined_r0x0040edd1;
        goto LAB_0040eeb4;
      }
    }
    piVar11 = DAT_00482c20 + 3;
    if ((0x1f < (short)*piVar11) && (pbVar6 = (byte *)(DAT_00482c20 + 4), *(short *)pbVar6 < 0xe2))
    {
      local_8 = &DAT_004823a0;
      uVar2 = (&DAT_004823cc)[(uint)(byte)(&DAT_004820d0)[uVar7] * 0x20];
      local_9 = 0;
      (&DAT_00482150)[uVar7] = 0;
      local_b = '\0';
      do {
        if (((*(byte *)(local_8 + 0xb) & 0x30) == 0x30) &&
           ((*(byte *)(local_8 + 0xb) & 7) == ((byte)uVar2 & 7))) {
          iVar8 = local_8[3];
          if ((*(char *)((int)&DAT_00482050 + uVar7) == '}') ||
             (*(char *)((int)&DAT_00482050 + uVar7) == -0x7b)) {
            DAT_00482348 = (int)*(short *)(iVar8 + 0x10) + 0x10;
            if ((uint)(int)*(short *)pbVar6 < DAT_00482348) {
              DAT_00482348 = (int)*(short *)(iVar8 + 0xc) + 10;
              if ((uint)(int)(short)*piVar11 < DAT_00482348) goto LAB_0040f119;
              FUN_0040e550((int)DAT_00482c20,-0x800);
              *(byte *)((int)DAT_00482c20 + 9) =
                   (-((byte)(&DAT_00482290)[uVar7] < 0x7f) & 0xf8U) + 0x86;
              return;
            }
            FUN_0040e580((int)DAT_00482c20,-0x600);
            FUN_0040e550((int)DAT_00482c20,-0x300);
            goto LAB_0040f357;
          }
          DAT_00482348 = ((int)*(short *)(iVar8 + 0x10) - (int)*(short *)pbVar6) + 8;
          if (DAT_00482348 < 0x14) {
            DAT_0048236c = (int)*(short *)(iVar8 + 0xc) + 0x10;
            DAT_00482348 = DAT_0048236c - (int)(short)*piVar11;
            if (((uint)(int)(short)*piVar11 <= DAT_0048236c) && (DAT_00482348 < 0x1e))
            goto LAB_0040f119;
            if (0xdd < *(short *)pbVar6) {
              pbVar6[0] = 0xda;
              pbVar6[1] = 0;
            }
          }
          else if (*(short *)(iVar8 + 0x10) < *(short *)pbVar6) {
            if (DAT_00481dbc == '\0') {
              FUN_0040e580((int)DAT_00482c20,-0x200);
            }
            else {
              FUN_0040e580((int)DAT_00482c20,-0x400);
            }
          }
          else if (*(short *)pbVar6 < 0xe0) {
            if (DAT_00481dbc == '\0') {
              FUN_0040e580((int)DAT_00482c20,0x100);
            }
            else {
              FUN_0040e580((int)DAT_00482c20,0x400);
            }
          }
          else {
            pbVar6[0] = 0xd8;
            pbVar6[1] = 0;
          }
          goto LAB_0040eeb4;
        }
        local_8 = local_8 + 0x10;
        local_b = local_b + '\x01';
      } while (local_b != '\"');
      if ((((0xb7 < *pbVar6) || (*(char *)((int)&DAT_00482050 + uVar7) == -0x7f)) ||
          (*(char *)((int)&DAT_00482050 + uVar7) == -0x77)) ||
         ((bVar4 = FUN_0040cb50(), 3 < (bVar4 & 7) || (0x39 < (short)DAT_00482c20[3]))))
      goto LAB_0040eeb4;
    }
  }
LAB_0040e9ba:
  cVar1 = *(char *)((int)&DAT_00482050 + uVar7);
  if (cVar1 == 'z') goto LAB_0040f06d;
  if (cVar1 == '|') goto LAB_0040f06d;
  if (cVar1 == '}') goto LAB_0040f06d;
  if (cVar1 == '\x7f') goto LAB_0040f06d;
  if (cVar1 == -0x7e) goto LAB_0040f06d;
  if (cVar1 == -0x7c) goto LAB_0040f06d;
  if (cVar1 == -0x7b) goto LAB_0040f06d;
  if (cVar1 == -0x79) goto LAB_0040f06d;
  (&DAT_00482150)[uVar7] = 0;
  local_a = 0;
  if (cVar1 == -0x59) {
LAB_0040ea14:
    uVar7 = (uint)param_1;
    if ((&DAT_00482150)[uVar7] == '\0') {
      if ((&DAT_004807e9)[uVar7 * 2] == '\0') {
        if (((&DAT_00482290)[uVar7] & 0xf) == 1) {
          local_b = 0x9a;
        }
        else {
          if (((&DAT_00482290)[uVar7] & 0xf) == 2) goto LAB_0040f06d;
          local_b = 0x97;
        }
        *(char *)((int)DAT_00482c20 + 9) = local_b;
        (&DAT_00482290)[param_1] = (&DAT_00482290)[param_1] + '\x01';
        return;
      }
LAB_0040f06d:
      do {
        local_8 = &DAT_004823a0;
        local_b = '\0';
        while ((*(short *)((int)local_8 + 0x26) != 0xff ||
               (((*(ushort *)(local_8 + 0xb) & 0xf0) != 0x30 &&
                ((*(ushort *)(local_8 + 0xb) & 0xf0) != 0xc0))))) {
LAB_0040f15d:
          local_8 = local_8 + 0x10;
          local_b = local_b + '\x01';
          if (local_b == '\"') {
            uVar7 = (uint)param_1;
            bVar4 = (&DAT_004820d0)[uVar7];
            (&DAT_004820d0)[uVar7] = 0;
            *(undefined2 *)(&DAT_004823c4 + (uint)bVar4 * 0x40) = 0;
            *(undefined1 *)((int)&DAT_00482050 + uVar7) = 0;
            FUN_0041c050((int)DAT_00482c20);
            FUN_00412e60(DAT_00482c20);
            return;
          }
        }
        bVar4 = 0;
        do {
          if ((&DAT_00482150)[bVar4] == local_b) goto LAB_0040f15d;
          bVar4 = bVar4 + 1;
        } while (bVar4 != 0x42);
        *(undefined2 *)(DAT_00482c20 + 3) = *(undefined2 *)(local_8[3] + 0xc);
        *(undefined2 *)(DAT_00482c20 + 4) = *(undefined2 *)(local_8[3] + 0x10);
        (&DAT_00482290)[param_1] = 0;
        local_b = (char)((int)(local_8 + -0x1208e8) >> 6);
LAB_0040f119:
        local_9 = 0;
        uVar7 = (uint)param_1;
        (&DAT_00482150)[uVar7] = local_b;
        *(undefined2 *)((int)local_8 + 0x26) = 0;
        if ((*(short *)(local_8 + 4) < 0x80) && (0xbf < *(byte *)(DAT_00482c20 + 4))) {
          bVar4 = (&DAT_00482290)[uVar7] & 0xf;
        }
        else {
          bVar4 = (&DAT_00482290)[uVar7] & 0x8f;
        }
        local_4 = &DAT_00482290 + uVar7;
        local_a = 1;
        *local_4 = bVar4;
LAB_0040f1dd:
        if (*(char *)((int)&DAT_00482050 + (uint)param_1) == -0x59) goto LAB_0040ea14;
        if (local_9 != 0x14) break;
      } while( true );
    }
  }
  uVar7 = (uint)param_1;
  cVar1 = (&DAT_00482290)[uVar7];
  (&DAT_00482290)[uVar7] = cVar1 + 1U;
  if ((byte)(cVar1 + 1U) < 0x7f) {
    if (local_a == 0) {
      if (local_9 == 0) {
        FUN_00412890(0xf);
      }
      uVar9 = (&DAT_0042bd58)[local_9 >> 2];
    }
    else {
      if (local_9 == 0) {
        FUN_00412890(0xe);
      }
      uVar9 = (&DAT_0042bd48)[local_9 >> 2];
    }
  }
  else if (local_a == 0) {
    if (local_9 == 0) {
      FUN_00412890(0xf);
    }
    uVar9 = (&DAT_0042bd60)[local_9 >> 2];
  }
  else {
    if (local_9 == 0) {
      FUN_00412890(0x10);
    }
    uVar9 = (&DAT_0042bd50)[local_9 >> 2];
  }
  *(undefined1 *)((int)&DAT_00482050 + uVar7) = uVar9;
  if ((local_9 == 1) && (local_a != 0)) {
    uVar7 = (uint)local_a;
    iVar8 = uVar7 * 0x40;
    *(undefined2 *)(&DAT_004823ce + iVar8) = 0;
    bVar4 = (&DAT_004823b6)[iVar8];
    if ((bVar4 == 0xc0) || ((0xbf < bVar4 && (bVar4 < 0xef)))) {
      bVar4 = 0xd0;
    }
    else {
      (&DAT_004823b0)[uVar7 * 0x20] = 0;
      bVar4 = 0xb0;
    }
    *(ushort *)(&DAT_004823b6 + iVar8) = (ushort)bVar4;
    *(undefined2 *)(DAT_00482c20 + 3) = *(undefined2 *)((&DAT_004823ac)[uVar7 * 0x10] + 0xc);
    *(undefined2 *)(DAT_00482c20 + 4) = *(undefined2 *)((&DAT_004823ac)[uVar7 * 0x10] + 0x10);
    *(undefined1 *)((&DAT_004823ac)[uVar7 * 0x10] + 9) = 0;
  }
LAB_0040f357:
  *(undefined1 *)((int)DAT_00482c20 + 9) = *(undefined1 *)((int)&DAT_00482050 + (uint)param_1);
  return;
}



/* ================= FUN_0040f550 @ 0040f550 ================= */

void FUN_0040f550(void)

{
  FUN_0040f820();
  DAT_00484721 = DAT_00484721 + '\x01';
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_0040f580;
  FUN_0040cd00(0x51);
  return;
}



/* ================= FUN_0040f820 @ 0040f820 ================= */

void FUN_0040f820(void)

{
  FUN_00406850(DAT_00484721,5,0x12);
  return;
}



/* ================= FUN_0040f840 @ 0040f840 ================= */

void FUN_0040f840(void)

{
  int iVar1;
  int iVar2;
  
  DAT_00482338 = 199;
  iVar2 = 9;
  FUN_0040f900();
  FUN_0040f8b0(0,1,2);
  do {
    iVar1 = 0x18;
    FUN_0040f900();
    FUN_0040daf0(0x1204);
    do {
      FUN_0040da20(4);
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    FUN_0040daf0(0x1204);
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  FUN_0040f8b0(3,1,5);
  return;
}



/* ================= FUN_0040f8b0 @ 0040f8b0 ================= */

void __cdecl FUN_0040f8b0(byte param_1,byte param_2,byte param_3)

{
  int iVar1;
  
  FUN_0040f900();
  iVar1 = 0x18;
  FUN_0040daf0(param_1 + 0x1200);
  do {
    FUN_0040daf0(param_2 + 0x1200);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  FUN_0040daf0(param_3 + 0x1200);
  return;
}



/* ================= FUN_0040f900 @ 0040f900 ================= */

void FUN_0040f900(void)

{
  FUN_0040da00((short)DAT_00482338);
  DAT_00482338 = DAT_00482338 + 0x40;
  return;
}



/* ================= FUN_0040f920 @ 0040f920 ================= */

void FUN_0040f920(void)

{
  byte bVar1;
  ushort uVar2;
  char cVar3;
  char local_1;
  
  DAT_00481f94 = (&PTR_DAT_0042fa40)[DAT_00484721];
  FUN_00418bb0();
  DAT_00482370 = 0x148;
  local_1 = '\t';
  do {
    cVar3 = '\x18';
    FUN_0040da00((short)DAT_00482370);
    do {
      bVar1 = FUN_00418bf0();
      uVar2 = (ushort)bVar1;
      if ((char)bVar1 < -0x73) {
        uVar2 = CONCAT11(0x10,bVar1);
      }
      FUN_0040daf0(uVar2 + 0x600);
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_0040faa0 @ 0040faa0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040faa0(void)

{
  DAT_00484720 = 0;
  DAT_00495f34 = &LAB_0040fa10;
  _DAT_00484710 = 0xffffffff;
  DAT_00484714 = 0xffffffff;
  DAT_00484718 = 0xffffffff;
  DAT_0048471c = 0xffffffff;
  return;
}



/* ================= FUN_0040fad0 @ 0040fad0 ================= */

undefined1 FUN_0040fad0(void)

{
  byte bVar1;
  
  bVar1 = DAT_00484720;
  DAT_00484720 = DAT_00484720 + 1;
  return (&DAT_00484710)[bVar1];
}



/* ================= FUN_0040faf0 @ 0040faf0 ================= */

undefined1 FUN_0040faf0(void)

{
  return (&DAT_00484710)[DAT_00484720];
}



/* ================= FUN_0040fdf0 @ 0040fdf0 ================= */

void FUN_0040fdf0(void)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint local_c;
  uint local_8;
  uint local_4;
  
  local_4 = (uint)DAT_00481a71;
  uVar2 = (uint)DAT_00481a72;
  bVar1 = *(byte *)((int)&DAT_00481a80 + uVar2 + local_4 * 0x20) & 0xf;
  if ((bVar1 < 0xb) || (0xd < bVar1)) {
    DAT_00495f34 = &LAB_0040feb0;
    return;
  }
  iVar3 = 0;
  while (iVar3 < 6) {
    iVar4 = iVar3 + 1;
    local_c = local_4;
    local_8 = uVar2;
    FUN_00403490(&local_8,(int *)&local_c,iVar3);
    iVar3 = iVar4;
    if ((((int)local_8 < (int)(uint)DAT_00495c02) && ((int)local_c < (int)(uint)DAT_00495c03)) &&
       ((bVar1 = *(byte *)((int)&DAT_00481a80 + local_8 + local_c * 0x20) & 0xf, bVar1 < 0xb ||
        (0xd < bVar1)))) break;
  }
  FUN_0040ff30((byte)local_8,(byte)local_c,&LAB_0040feb0);
  return;
}



/* ================= FUN_0040ff30 @ 0040ff30 ================= */

void __cdecl FUN_0040ff30(byte param_1,byte param_2,undefined4 param_3)

{
  if ((param_1 < DAT_00495c02) && (param_2 < DAT_00495c03)) {
    DAT_00480870 = param_1;
    DAT_00480874 = param_2;
    DAT_00495f34 = &LAB_0040ff70;
    DAT_00480868 = param_3;
    return;
  }
  FUN_0040faa0();
  return;
}



/* ================= FUN_00410010 @ 00410010 ================= */

void __cdecl FUN_00410010(undefined4 param_1,undefined4 param_2)

{
  DAT_0048087c = param_1;
  DAT_00480878 = param_2;
  DAT_00495f34 = &LAB_00410030;
  return;
}



/* ================= FUN_00410190 @ 00410190 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00410190(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  byte *pbVar7;
  byte *pbVar6;
  ushort *puVar8;
  undefined4 *puVar9;
  ushort *puVar10;
  
  DAT_00481e6d = 0;
  DAT_00481e6c = 0;
  DAT_00481e72 = 0;
  DAT_00481e71 = 0;
  _DAT_004834a2 = 0;
  _DAT_004836e0 = 0;
  DAT_00481e69 = 0;
  DAT_00481e68 = 0;
  DAT_00495c06 = 0;
  DAT_00495c05 = 0;
  DAT_00495c45 = 0;
  DAT_00495c44 = 0;
  DAT_00481dbf = 0;
  DAT_00481dbe = 0;
  DAT_00481e62 = 0;
  DAT_00484701 = 0;
  DAT_00481e73 = 0;
  DAT_00484708 = 0;
  DAT_00484700 = 0x20;
  puVar8 = &DAT_004808a8;
  DAT_00484704 = &DAT_004808a8;
  if (DAT_0042fb74 == '\0') {
    iVar3 = (uint)DAT_00495c52 * 4;
    cVar1 = (&DAT_0043aaa8)[iVar3];
    cVar2 = (&DAT_0043aaa9)[iVar3];
    pbVar6 = &DAT_004372a8 + CONCAT11((&DAT_0043aaab)[iVar3],(&DAT_0043aaaa)[iVar3]);
    puVar9 = (undefined4 *)&DAT_004808a8;
    for (iVar3 = 0x160; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar9 = 0;
      puVar9 = puVar9 + 1;
    }
    if ((byte)(cVar2 * '\v') != 0) {
      uVar5 = (uint)(byte)(cVar2 * '\v');
      do {
        if ((byte)(cVar1 << 4) != 0) {
          uVar4 = (uint)(byte)(cVar1 << 4);
          pbVar7 = pbVar6;
          puVar10 = puVar8;
          do {
            pbVar6 = pbVar7 + 1;
            *puVar10 = (ushort)*pbVar7;
            uVar4 = uVar4 - 1;
            pbVar7 = pbVar6;
            puVar10 = puVar10 + 1;
          } while (uVar4 != 0);
        }
        puVar8 = puVar8 + 0x20;
        uVar5 = uVar5 - 1;
      } while (uVar5 != 0);
    }
  }
  return;
}



/* ================= FUN_004102b0 @ 004102b0 ================= */

void FUN_004102b0(void)

{
  FUN_004102e0();
  FUN_0041ae00();
  return;
}



/* ================= FUN_004102c0 @ 004102c0 ================= */

void FUN_004102c0(void)

{
  DAT_00481e62 = 1;
  FUN_0041ae00();
  DAT_00481e62 = 0;
  return;
}



/* ================= FUN_004102e0 @ 004102e0 ================= */

void FUN_004102e0(void)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int local_8;
  int local_4;
  
  sVar2 = 0x40;
  sVar1 = 0x81;
  local_4 = 2;
  do {
    local_8 = 0x15;
    uVar3 = (DAT_00495c45 + 0x28) * 0x20 + (uint)DAT_00495c44;
    do {
      FUN_0040da00(sVar1);
      iVar4 = 0x1f;
      uVar5 = uVar3;
      do {
        uVar6 = uVar5 + 1;
        FUN_0040da20((short)uVar5);
        if ((int)(uVar3 | 0x1f) < (int)uVar6) {
          uVar6 = uVar3 & 0xffe0;
        }
        iVar4 = iVar4 + -1;
        uVar5 = uVar6;
      } while (iVar4 != 0);
      uVar3 = uVar3 + 0x20;
      if (0x7bf < (uVar3 & 0xffe0)) {
        uVar3 = DAT_00495c44 + 0x500;
      }
      sVar1 = sVar1 + sVar2;
      local_8 = local_8 + -1;
    } while (local_8 != 0);
    sVar1 = 0x7c00;
    sVar2 = 0x1f;
    local_4 = local_4 + -1;
  } while (local_4 != 0);
  return;
}



/* ================= FUN_004103f0 @ 004103f0 ================= */

bool FUN_004103f0(void)

{
  bool bVar1;
  UINT uMode;
  FARPROC pFVar2;
  int iVar3;
  undefined4 uStack_9c;
  int *piStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  int *piStack_88;
  
  bVar1 = true;
  uMode = SetErrorMode(0x8000);
  DAT_00481938 = LoadLibraryA(s_DDRAW_DLL_0042fc3c);
  if (DAT_00481938 != (HMODULE)0x0) {
    piStack_88 = (int *)0x41042a;
    pFVar2 = GetProcAddress(DAT_00481938,s_DirectDrawCreate_0042fc28);
    if (pFVar2 != (FARPROC)0x0) {
      piStack_88 = (int *)0x0;
      uStack_8c = 0x410439;
      iVar3 = (*pFVar2)();
      if (iVar3 == 0) {
        bVar1 = false;
        goto LAB_00410457;
      }
    }
    FreeLibrary(DAT_00481938);
    DAT_00481938 = (HMODULE)0x0;
  }
LAB_00410457:
  SetErrorMode(uMode);
  if (bVar1) {
    return false;
  }
  piStack_88 = DAT_00481508;
  uStack_8c = 0x41047b;
  iVar3 = (**(code **)(*DAT_00481508 + 0x50))();
  if (iVar3 != 0) {
    return false;
  }
  uStack_8c = 8;
  uStack_90 = 0x1e0;
  uStack_94 = 0x280;
  piStack_98 = DAT_00481508;
  uStack_9c = 0x41049f;
  iVar3 = (**(code **)(*DAT_00481508 + 0x54))();
  if (iVar3 != 0) {
    return false;
  }
  piStack_88 = (int *)0x6c;
  uStack_9c = 0;
  iVar3 = (**(code **)(*DAT_00481508 + 0x18))(DAT_00481508,&piStack_88,&DAT_004814ec);
  if (iVar3 != 0) {
    return false;
  }
  uStack_9c = 4;
  iVar3 = (**(code **)(*DAT_004814ec + 0x30))(DAT_004814ec,&uStack_9c,&DAT_00480e70);
  if (iVar3 != 0) {
    return false;
  }
  DAT_00480f08 = &DAT_00480f20;
  iVar3 = (**(code **)(*DAT_00481508 + 0x14))(DAT_00481508,4,&DAT_00480f24,&DAT_00480e6c,0);
  if (iVar3 == 0) {
    iVar3 = (**(code **)(*DAT_004814ec + 0x7c))(DAT_004814ec,DAT_00480e6c);
    return iVar3 == 0;
  }
  return false;
}



/* ================= FUN_00410570 @ 00410570 ================= */

void FUN_00410570(void)

{
  if (DAT_00481508 != (int *)0x0) {
    if (DAT_00480e6c != (int *)0x0) {
      (**(code **)(*DAT_00480e6c + 8))(DAT_00480e6c);
      DAT_00480e6c = (int *)0x0;
    }
    if (DAT_00480e70 != (int *)0x0) {
      (**(code **)(*DAT_00480e70 + 8))(DAT_00480e70);
      DAT_00480e70 = (int *)0x0;
    }
    if (DAT_004814ec != (int *)0x0) {
      (**(code **)(*DAT_004814ec + 8))(DAT_004814ec);
      DAT_004814ec = (int *)0x0;
    }
    (**(code **)(*DAT_00481508 + 8))(DAT_00481508);
    DAT_00481508 = (int *)0x0;
    FreeLibrary(DAT_00481938);
    DAT_00481938 = (HMODULE)0x0;
  }
  return;
}



/* ================= FUN_00410610 @ 00410610 ================= */

void FUN_00410610(void)

{
  GetWindowLongA(DAT_00480e28,-0x10);
  return;
}



/* ================= FUN_00410620 @ 00410620 ================= */

void __cdecl FUN_00410620(LONG param_1)

{
  SetWindowLongA(DAT_00480e28,-0x10,param_1);
  SetWindowPos(DAT_00480e28,(HWND)0xfffffffe,0,0,0,0,0x123);
  return;
}



/* ================= FUN_00410660 @ 00410660 ================= */

void __cdecl FUN_00410660(int param_1)

{
  bool bVar1;
  uint uVar2;
  undefined3 extraout_var;
  
  if (DAT_0042fb68 != param_1) {
    DAT_0042fb68 = param_1;
    if (param_1 == 0) {
      FUN_00410570();
      uVar2 = FUN_00410610();
      FUN_00410620(uVar2 | 0xc00000);
      FUN_00411110(DAT_00480e28,0x280,0x1e0,1,1);
      return;
    }
    bVar1 = FUN_004103f0();
    if (CONCAT31(extraout_var,bVar1) == 0) {
      DAT_0042fb68 = 0;
      return;
    }
    uVar2 = FUN_00410610();
    FUN_00410620(uVar2 & 0xff3fffff);
  }
  return;
}



/* ================= FUN_004106e0 @ 004106e0 ================= */

void FUN_004106e0(void)

{
  HMENU pHVar1;
  int nPos;
  
  nPos = 1;
  pHVar1 = GetMenu(DAT_00480e28);
  pHVar1 = GetSubMenu(pHVar1,nPos);
  CheckMenuItem(pHVar1,0x70,(DAT_0048193c == 0) - 1 & 8);
  CheckMenuItem(pHVar1,0x71,-(uint)(DAT_0048193c == 0) & 8);
  CheckMenuItem(pHVar1,0x6c,(DAT_00480e2c == 0) - 1 & 8);
  CheckMenuItem(pHVar1,0x6d,-(uint)(DAT_00480e2c == 0) & 8);
  CheckMenuItem(pHVar1,0x6e,(DAT_00481504 == 0) - 1 & 8);
  CheckMenuItem(pHVar1,0x6f,-(uint)(DAT_00481504 == 0) & 8);
  return;
}



/* ================= FUN_00410780 @ 00410780 ================= */

void __cdecl FUN_00410780(HWND param_1)

{
  int X;
  int Y;
  tagRECT local_20;
  tagRECT local_10;
  
  GetWindowRect(param_1,&local_20);
  GetWindowRect(DAT_00480e28,&local_10);
  X = (local_10.right - local_10.left) / 2 + (local_20.left - local_20.right) / 2 + local_10.left;
  if (X < 0) {
    X = 0;
  }
  Y = (local_10.bottom - local_10.top) / 2 + (local_20.top - local_20.bottom) / 2 + local_10.top;
  if (Y < 0) {
    Y = 0;
  }
  MoveWindow(param_1,X,Y,local_20.right - local_20.left,local_20.bottom - local_20.top,1);
  return;
}



/* ================= FUN_004108d0 @ 004108d0 ================= */

void FUN_004108d0(void)

{
  if (DAT_0042fb68 != 0) {
    FUN_00410660((uint)(DAT_0042fb68 == 0));
  }
  DialogBoxParamA(DAT_004814dc,(LPCSTR)0x2,DAT_00480e28,(DLGPROC)&LAB_00410870,0);
  return;
}



/* ================= FUN_00410910 @ 00410910 ================= */

undefined4 __cdecl FUN_00410910(undefined4 param_1,undefined2 param_2)

{
  byte bVar1;
  
  bVar1 = (byte)DAT_0042fb8c;
  switch(param_2) {
  case 0x65:
    DialogBoxParamA(DAT_004814dc,(LPCSTR)0x1,DAT_00480e28,(DLGPROC)&LAB_00410820,0);
    return 0;
  case 0x66:
    PostMessageA(DAT_00480e28,0x10,0,0);
    return 0;
  case 0x6c:
    if (DAT_00480e2c == 0) {
      DAT_00480e2c = 1;
      FUN_00412890(bVar1);
      FUN_004106e0();
      return 0;
    }
    break;
  case 0x6d:
    if (DAT_00480e2c != 0) {
      FUN_00420870(&DAT_00480e78);
      DAT_00480e2c = 0;
      FUN_004106e0();
      return 0;
    }
    break;
  case 0x6e:
    if (DAT_00481504 == 0) {
      DAT_00481504 = 1;
      FUN_00412890(bVar1);
      FUN_004106e0();
      return 0;
    }
    break;
  case 0x6f:
    if (DAT_00481504 != 0) {
      DAT_00481504 = 0;
      FUN_00412890(bVar1);
      FUN_004106e0();
      return 0;
    }
    break;
  case 0x70:
    DAT_0048193c = 1;
    FUN_004106e0();
    return 0;
  case 0x71:
    DAT_0048193c = 0;
    FUN_004106e0();
    return 0;
  case 0x72:
    FUN_00410660((uint)(DAT_0042fb68 == 0));
    return 0;
  case 0x73:
    FUN_00403eb0();
    return 0;
  case 0x74:
    FUN_00404600();
  }
  return 0;
}



/* ================= FUN_00410aa0 @ 00410aa0 ================= */

undefined4 __cdecl FUN_00410aa0(int param_1)

{
  if (param_1 != 0) {
    if (DAT_004814ec != (int *)0x0) {
      (**(code **)(*DAT_004814ec + 0x6c))(DAT_004814ec);
    }
    if (DAT_00480e70 != (int *)0x0) {
      (**(code **)(*DAT_00480e70 + 0x6c))(DAT_00480e70);
    }
  }
  return 1;
}



/* ================= FUN_00410ae0 @ 00410ae0 ================= */

undefined4 FUN_00410ae0(void)

{
  return 0;
}



/* ================= FUN_00410af0 @ 00410af0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00410af0(uint param_1,undefined4 param_2)

{
  if ((param_1 & 0xffff) == DAT_00480e90) {
    switch(DAT_0042fb8c) {
    case 0x25:
    case 0x31:
    case 0x32:
      break;
    default:
      _DAT_00480e78 = param_2;
      _DAT_00480e7c = 0;
      _DAT_00480e8c = _DAT_00480e8c | 5;
      FUN_004206c0(&DAT_00480e78,0x806);
      return 0;
    case 0x29:
      FUN_00412890(0x2b);
      return 0;
    case 0x33:
      FUN_00412890(0x37);
      return 0;
    case 0x36:
      FUN_00412890(0x38);
    }
  }
  return 0;
}



/* ================= FUN_00410bb0 @ 00410bb0 ================= */

undefined4 FUN_00410bb0(void)

{
  return 0;
}



/* ================= FUN_00410bc0 @ 00410bc0 ================= */

undefined4 FUN_00410bc0(void)

{
  return 0;
}



/* ================= FUN_00410bd0 @ 00410bd0 ================= */

UINT FUN_00410bd0(HWND param_1,uint param_2,HWND param_3,uint param_4)

{
  UINT UVar1;
  HDC pHVar2;
  tagPAINTSTRUCT local_40;
  
  if (param_2 < 7) {
    if (param_2 == 6) {
      DAT_0042fb78 = 0;
    }
    else if (param_2 == 2) {
      DAT_0042fb78 = 0;
      FUN_004115c0();
      PostQuitMessage(0);
    }
  }
  else if (param_2 < 0x15) {
    if (param_2 == 0x14) {
      return 1;
    }
    if (param_2 == 0xf) {
      pHVar2 = BeginPaint(param_1,&local_40);
      if ((DAT_00480f10 != (HPALETTE)0x0) && (DAT_0042fb68 == 0)) {
        SelectPalette(pHVar2,DAT_00480f10,0);
      }
      RealizePalette(pHVar2);
      FUN_00411af0(pHVar2);
      EndPaint(param_1,&local_40);
      return 0;
    }
  }
  else if (param_2 < 0x101) {
    if (param_2 == 0x100) {
      switch(param_3) {
      case (HWND)0x10:
        DAT_0042fb78 = DAT_0042fb78 | 0x40;
        break;
      case (HWND)0x11:
      case (HWND)0x20:
        DAT_0042fb78 = DAT_0042fb78 | 0x20;
        break;
      case (HWND)0x25:
      case (HWND)0x64:
        DAT_0042fb78 = DAT_0042fb78 | 4;
        break;
      case (HWND)0x26:
      case (HWND)0x68:
        DAT_0042fb78 = DAT_0042fb78 | 1;
        break;
      case (HWND)0x27:
      case (HWND)0x66:
        DAT_0042fb78 = DAT_0042fb78 | 8;
        break;
      case (HWND)0x28:
      case (HWND)0x62:
        DAT_0042fb78 = DAT_0042fb78 | 2;
        break;
      case (HWND)0x70:
        DAT_0042fb78 = DAT_0042fb78 | 0x10;
        break;
      case (HWND)0x71:
        DAT_0042fb78 = DAT_0042fb78 | 0x80;
      }
    }
    else if (param_2 == 0x1c) {
      DAT_00480880 = param_3;
      FUN_00410aa0((int)param_3);
    }
  }
  else if (param_2 < 0x105) {
    if (param_2 == 0x104) {
      if ((DAT_0042fb68 != 0) && ((short)param_3 == 0x12)) {
        return 0;
      }
    }
    else if (param_2 == 0x101) {
      switch(param_3) {
      case (HWND)0x10:
        DAT_0042fb78 = DAT_0042fb78 & 0xffffffbf;
        break;
      case (HWND)0x11:
      case (HWND)0x20:
        DAT_0042fb78 = DAT_0042fb78 & 0xffffffdf;
        break;
      case (HWND)0x25:
      case (HWND)0x64:
        DAT_0042fb78 = DAT_0042fb78 & 0xfffffffb;
        break;
      case (HWND)0x26:
      case (HWND)0x68:
        DAT_0042fb78 = DAT_0042fb78 & 0xfffffffe;
        break;
      case (HWND)0x27:
      case (HWND)0x66:
        DAT_0042fb78 = DAT_0042fb78 & 0xfffffff7;
        break;
      case (HWND)0x28:
      case (HWND)0x62:
        DAT_0042fb78 = DAT_0042fb78 & 0xfffffffd;
        break;
      case (HWND)0x70:
        DAT_0042fb78 = DAT_0042fb78 & 0xffffffef;
        break;
      case (HWND)0x71:
        DAT_0042fb78 = DAT_0042fb78 & 0xffffff7f;
      }
    }
  }
  else if (param_2 < 0x310) {
    if (param_2 == 0x30f) {
LAB_00410ecc:
      pHVar2 = GetDC(param_1);
      if ((DAT_00480f10 != (HPALETTE)0x0) && (DAT_0042fb68 == 0)) {
        SelectPalette(pHVar2,DAT_00480f10,0);
      }
      UVar1 = RealizePalette(pHVar2);
      ReleaseDC(param_1,pHVar2);
      if (UVar1 != 0) {
        InvalidateRect(param_1,(RECT *)0x0,1);
      }
      return UVar1;
    }
    if (param_2 == 0x111) {
switchD_00410f39_caseD_3:
      UVar1 = FUN_00410910((uint)param_3 >> 0x10,(short)param_3);
      return UVar1;
    }
  }
  else if (param_2 == 0x311) {
    if (param_3 != param_1) goto LAB_00410ecc;
  }
  else if (param_2 == 0x3b9) {
    switch(param_3) {
    case (HWND)0x1:
      UVar1 = FUN_00410af0(param_4,param_1);
      return UVar1;
    case (HWND)0x2:
      UVar1 = FUN_00410bb0();
      return UVar1;
    case (HWND)0x4:
      UVar1 = FUN_00410ae0();
      return UVar1;
    case (HWND)0x8:
      UVar1 = FUN_00410bc0();
      return UVar1;
    }
    goto switchD_00410f39_caseD_3;
  }
  UVar1 = DefWindowProcA(param_1,param_2,(WPARAM)param_3,param_4);
  return UVar1;
}



/* ================= FUN_00411110 @ 00411110 ================= */

int __cdecl FUN_00411110(HWND param_1,int param_2,int param_3,BOOL param_4,int param_5)

{
  uint uVar1;
  uint uVar2;
  int nWidth;
  int nHeight;
  int iVar3;
  tagRECT local_20;
  tagRECT local_10;
  
  GetWindowRect(param_1,&local_20);
  GetClientRect(param_1,&local_10);
  if (param_2 == 0) {
    nWidth = local_20.right - local_20.left;
  }
  else {
    nWidth = (param_2 - local_20.left) + (local_20.right - local_10.right);
  }
  if (param_3 == 0) {
    nHeight = local_20.bottom - local_20.top;
  }
  else {
    nHeight = (param_3 - local_20.top) + (local_20.bottom - local_10.bottom);
  }
  uVar1 = GetSystemMetrics(0x10);
  uVar2 = GetSystemMetrics(0x11);
  if ((uVar1 < (uint)(local_20.left + nWidth)) &&
     (local_20.left = uVar1 - nWidth, local_20.left < 0)) {
    local_20.left = 0;
  }
  if ((uVar2 < (uint)(local_20.top + nHeight)) && (local_20.top = uVar2 - nHeight, local_20.top < 0)
     ) {
    local_20.top = 0;
  }
  if ((uVar1 < (uint)(local_20.left + nWidth)) ||
     (iVar3 = 1, uVar2 < (uint)(local_20.top + nHeight))) {
    iVar3 = 0;
  }
  if ((param_5 != 0) || (iVar3 != 0)) {
    MoveWindow(param_1,local_20.left,local_20.top,nWidth,nHeight,param_4);
  }
  return iVar3;
}



/* ================= FUN_00411210 @ 00411210 ================= */

undefined4 FUN_00411210(HINSTANCE param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  BOOL BVar2;
  tagMSG local_1c;
  
  DAT_004814dc = param_1;
  iVar1 = FUN_00411300(param_1,param_4);
  if (iVar1 == 0) {
    return 0;
  }
  iVar1 = FUN_004116e0();
  if (iVar1 == 0) {
    DestroyWindow(DAT_00480e28);
    return 0;
  }
  while( true ) {
    while (BVar2 = PeekMessageA(&local_1c,(HWND)0x0,0,0,0), BVar2 == 0) {
      if (DAT_00480880 == 0) {
        WaitMessage();
      }
      else {
        FUN_00411a70();
      }
    }
    BVar2 = GetMessageA(&local_1c,(HWND)0x0,0,0);
    if (BVar2 == 0) break;
    if ((DAT_0048088c == (HACCEL)0x0) ||
       (iVar1 = TranslateAcceleratorA(DAT_00480e28,DAT_0048088c,&local_1c), iVar1 == 0)) {
      TranslateMessage(&local_1c);
      DispatchMessageA(&local_1c);
    }
  }
  return local_1c.wParam;
}



/* ================= FUN_00411300 @ 00411300 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00411300(HINSTANCE param_1,int param_2)

{
  char cVar1;
  bool bVar2;
  ATOM AVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined3 extraout_var;
  char *pcVar7;
  HWND hWndParent;
  HMENU hMenu;
  HINSTANCE hInstance;
  LPVOID lpParam;
  WNDCLASSA local_32c;
  BYTE local_304 [2];
  char local_302 [258];
  CHAR local_200 [512];
  
  local_32c.style = 0x10008;
  local_32c.lpfnWndProc = FUN_00410bd0;
  local_32c.cbClsExtra = 0;
  local_32c.cbWndExtra = 0;
  local_32c.hInstance = param_1;
  local_32c.hIcon = LoadIconA(param_1,s_APPICON_0042fca0);
  local_32c.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_32c.hbrBackground = GetStockObject(4);
  local_32c.lpszMenuName = s_AppMenu_0042fc98;
  local_32c.lpszClassName = s_NectarisClass_0042fc88;
  AVar3 = RegisterClassA(&local_32c);
  if (AVar3 == 0) {
    return 0;
  }
  lpParam = (LPVOID)0x0;
  hMenu = (HMENU)0x0;
  hWndParent = (HWND)0x0;
  hInstance = param_1;
  iVar4 = GetSystemMetrics(1);
  iVar5 = GetSystemMetrics(0);
  DAT_00480e28 = CreateWindowExA(0x40000,s_NectarisClass_0042fc88,s_Nectaris_0042fc7c,0xca0000,0,0,
                                 iVar5,iVar4,hWndParent,hMenu,hInstance,lpParam);
  if (DAT_00480e28 == (HWND)0x0) {
    return 0;
  }
  DAT_0048088c = LoadAcceleratorsA(param_1,s_APPACCEL_0042fc70);
  if (DAT_0042fb68 == 0) {
    FUN_00411110(DAT_00480e28,0x280,0x1e0,1,1);
    FUN_00411110(DAT_00480e28,0x280,0x1e0,1,1);
  }
  ShowWindow(DAT_00480e28,param_2);
  UpdateWindow(DAT_00480e28);
  if (DAT_0042fb68 != 0) {
    uVar6 = FUN_00410610();
    FUN_00410620(uVar6 & 0xff3fffff);
    bVar2 = FUN_004103f0();
    if (CONCAT31(extraout_var,bVar2) == 0) {
      DestroyWindow(DAT_00480e28);
      return 0;
    }
  }
  FUN_0040a6e0(DAT_004814dc,(int)DAT_00480e28);
  iVar4 = 0;
  do {
    iVar5 = iVar4 + 1;
    wsprintfA(local_200,s_SE_02d_0042fc68,iVar5);
    FUN_0040a890(local_200,0,iVar4,1);
    iVar4 = iVar5;
  } while (iVar5 < 0x24);
  DAT_0042fb6c = FUN_0041d690();
  _DAT_0042fb70 = DAT_0042fb6c;
  FUN_00425990(&DAT_00481328,0x100);
  iVar4 = -1;
  pcVar7 = &DAT_00481328;
  do {
    if (iVar4 == 0) break;
    iVar4 = iVar4 + -1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
  } while (cVar1 != '\0');
  if (iVar4 == -5) {
    DAT_0048132a = 0;
  }
  DAT_0048193c = FUN_0041c5b0(s_Settings_0042a730,s_SoundMode_0042fc5c,1);
  DAT_00480e2c = FUN_0041c5b0(s_Settings_0042a730,s_BgmMode_0042fc54,1);
  DAT_00481504 = FUN_0041c5b0(s_Settings_0042a730,s_BgmType_0042fc4c,0);
  FUN_0041c650(s_Settings_0042a730,s_LastDir_0042a73c,&DAT_0042fc48,local_304,0x104);
  iVar4 = lstrlenA((LPCSTR)local_304);
  if (0 < iVar4) {
    __chdrive((char)local_304[0] + -0x40);
    if (2 < iVar4) {
      __chdir(local_302);
    }
  }
  return 1;
}



/* ================= FUN_004115c0 @ 004115c0 ================= */

void FUN_004115c0(void)

{
  HGLOBAL pvVar1;
  code *pcVar2;
  code *pcVar3;
  
  if (DAT_00480f18 == (LPCVOID)0x0) {
    pcVar2 = GlobalFree_exref;
    pcVar3 = GlobalUnlock_exref;
  }
  else {
    pvVar1 = GlobalHandle(DAT_00480f18);
    pcVar3 = GlobalUnlock_exref;
    GlobalUnlock(pvVar1);
    pvVar1 = GlobalHandle(DAT_00480f18);
    pcVar2 = GlobalFree_exref;
    GlobalFree(pvVar1);
  }
  if (DAT_00481500 != (LPCVOID)0x0) {
    pvVar1 = GlobalHandle(DAT_00481500);
    (*pcVar3)(pvVar1);
    pvVar1 = GlobalHandle(DAT_00481500);
    (*pcVar2)(pvVar1);
  }
  if (DAT_00480f14 != (LPCVOID)0x0) {
    pvVar1 = GlobalHandle(DAT_00480f14);
    (*pcVar3)(pvVar1);
    pvVar1 = GlobalHandle(DAT_00480f14);
    (*pcVar2)(pvVar1);
  }
  if (DAT_00480f10 != (HGDIOBJ)0x0) {
    DeleteObject(DAT_00480f10);
  }
  FUN_0040b380();
  FUN_0041c710(s_Settings_0042a730,s_SoundMode_0042fc5c,DAT_0048193c);
  FUN_0041c710(s_Settings_0042a730,s_BgmMode_0042fc54,DAT_00480e2c);
  FUN_0041c710(s_Settings_0042a730,s_BgmType_0042fc4c,DAT_00481504);
  return;
}



/* ================= FUN_004116a0 @ 004116a0 ================= */

undefined4 __cdecl FUN_004116a0(undefined4 param_1)

{
  CHAR local_100 [256];
  
  wsprintfA(local_100,s____CleanupAndExit_err____d_0042fcb0,param_1);
  MessageBoxA(DAT_00480e28,local_100,s_ERROR_0042fca8,0);
  return 0;
}



/* ================= FUN_004116e0 @ 004116e0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_004116e0(void)

{
  HGLOBAL pvVar1;
  undefined4 uVar2;
  HDC hdc;
  int iVar3;
  char cVar4;
  int iVar5;
  UINT UVar6;
  undefined4 *puVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 local_28 [3];
  undefined2 local_1c;
  undefined2 local_1a;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  local_1c = 1;
  local_28[0] = 0x28;
  local_28[1] = 0x2c0;
  local_28[2] = 0x220;
  local_1a = 8;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  local_c = 0;
  local_8 = 0;
  local_4 = 0;
  pvVar1 = GlobalAlloc(0,0x5d801);
  DAT_00480f18 = GlobalLock(pvVar1);
  if (DAT_00480f18 == (LPVOID)0x0) {
    uVar2 = FUN_004116a0(1);
    return uVar2;
  }
  pvVar1 = GlobalAlloc(0,0x80001);
  DAT_00481500 = GlobalLock(pvVar1);
  if (DAT_00481500 == (LPVOID)0x0) {
    uVar2 = FUN_004116a0(2);
    return uVar2;
  }
  pvVar1 = GlobalAlloc(0,0x100001);
  DAT_00480f14 = GlobalLock(pvVar1);
  if (DAT_00480f14 == (LPVOID)0x0) {
    uVar2 = FUN_004116a0(3);
    return uVar2;
  }
  DAT_00480890 = &DAT_00481510;
  puVar7 = local_28;
  puVar9 = &DAT_00481510;
  for (iVar5 = 10; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar9 = *puVar7;
    puVar7 = puVar7 + 1;
    puVar9 = puVar9 + 1;
  }
  cVar4 = '\0';
  iVar5 = 0;
  do {
    iVar8 = iVar5 + 4;
    *(char *)((int)DAT_00480890 + iVar5 + 0x2a) = cVar4;
    *(char *)((int)DAT_00480890 + iVar5 + 0x29) = cVar4;
    *(char *)((int)DAT_00480890 + iVar5 + 0x28) = cVar4;
    cVar4 = cVar4 + '\x01';
    *(undefined1 *)((int)DAT_00480890 + iVar5 + 0x2b) = 0;
    iVar5 = iVar8;
  } while (iVar8 < 0x400);
  DAT_00480f08 = &DAT_00480f20;
  _DAT_00480f20 = 0x300;
  DAT_00480f22 = 0x100;
  hdc = GetDC((HWND)0x0);
  DAT_00480ef4 = 10;
  iVar5 = GetDeviceCaps(hdc,0xc);
  if (8 < iVar5) {
    DAT_00480ef4 = 0;
  }
  DAT_00480f04 = 0x100 - DAT_00480ef4;
  if (0 < (int)DAT_00480ef4) {
    GetSystemPaletteEntries(hdc,0,DAT_00480ef4,(LPPALETTEENTRY)(DAT_00480f08 + 4));
    GetSystemPaletteEntries
              (hdc,DAT_00480f04,DAT_00480ef4,(LPPALETTEENTRY)(DAT_00480f08 + DAT_00480f04 * 4 + 4));
  }
  ReleaseDC((HWND)0x0,hdc);
  iVar8 = 0;
  iVar5 = 0;
  if (0 < (int)DAT_00480ef4) {
    do {
      *(undefined *)((int)DAT_00480890 + iVar5 + 0x2a) = DAT_00480f08[iVar5 + 4];
      *(undefined *)((int)DAT_00480890 + iVar5 + 0x29) = DAT_00480f08[iVar5 + 5];
      *(undefined *)((int)DAT_00480890 + iVar5 + 0x28) = DAT_00480f08[iVar5 + 6];
      *(undefined *)((int)DAT_00480890 + (DAT_00480f04 + iVar8) * 4 + 0x2a) =
           DAT_00480f08[(DAT_00480f04 + iVar8) * 4 + 4];
      *(undefined *)((int)DAT_00480890 + (DAT_00480f04 + iVar8) * 4 + 0x29) =
           DAT_00480f08[(DAT_00480f04 + iVar8) * 4 + 5];
      iVar3 = DAT_00480f04 + iVar8;
      iVar8 = iVar8 + 1;
      *(undefined *)(DAT_00480890 + iVar3 + 10) = DAT_00480f08[iVar3 * 4 + 6];
      iVar5 = iVar5 + 4;
    } while (iVar8 < (int)DAT_00480ef4);
  }
  iVar5 = 0;
  if (0 < (int)DAT_00480ef4) {
    do {
      DAT_00480f08[iVar5 * 4 + 7] = 0;
      iVar8 = DAT_00480f04 + iVar5;
      iVar5 = iVar5 + 1;
      DAT_00480f08[iVar8 * 4 + 7] = 0;
    } while (iVar5 < (int)DAT_00480ef4);
  }
  if ((int)DAT_00480ef4 < (int)DAT_00480f04) {
    iVar5 = DAT_00480ef4 * 4;
    UVar6 = DAT_00480ef4;
    do {
      UVar6 = UVar6 + 1;
      DAT_00480f08[iVar5 + 4] = *(undefined1 *)((int)DAT_00480890 + iVar5 + 0x2a);
      DAT_00480f08[iVar5 + 5] = *(undefined1 *)((int)DAT_00480890 + iVar5 + 0x29);
      DAT_00480f08[iVar5 + 6] = *(undefined1 *)((int)DAT_00480890 + iVar5 + 0x28);
      DAT_00480f08[iVar5 + 7] = 4;
      iVar5 = iVar5 + 4;
    } while ((int)UVar6 < (int)DAT_00480f04);
  }
  FUN_004106e0();
  DAT_00480898 = _DAT_0047f7d0;
  DAT_0048089c = DAT_0047f7d4;
  DAT_004808a0 = DAT_0047f7d8;
  DAT_004808a4 = DAT_0047f7dc;
  DAT_004814f4 = _DAT_0047f790;
  FUN_00413050();
  DAT_004814f0 = 0;
  _DAT_00480f00 = timeGetTime();
  DAT_00480ed0 = 0;
  DAT_00480888 = 0;
  return 1;
}



/* ================= FUN_00411a70 @ 00411a70 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00411a70(void)

{
  DWORD DVar1;
  
  DVar1 = timeGetTime();
  if ((DAT_00480888 == 0) && (DVar1 - _DAT_00480f00 < (uint)(DAT_004814f0 * 0x14))) {
    return;
  }
  FUN_00413100();
  FUN_00411ab0();
  FUN_00412060();
  return;
}



/* ================= FUN_00411ab0 @ 00411ab0 ================= */

void FUN_00411ab0(void)

{
  DAT_004834a4 = DAT_004834b6;
  DAT_004834b6 = (byte)DAT_0042fb78;
  FUN_00412bb0();
  DAT_004834ac = ~DAT_004834a4 & DAT_004834b6;
  if (((DAT_004834b6 & 0x10) == 0) || ((DAT_004834ac & 0x80) == 0)) {
    return;
  }
  FUN_00412a30();
  return;
}



/* ================= FUN_00411af0 @ 00411af0 ================= */

void __cdecl FUN_00411af0(HDC param_1)

{
  if (DAT_00480f18 != (void *)0x0) {
    StretchDIBits(param_1,0,0,0x280,0x1e0,0x20,0x20,0x280,0x1e0,DAT_00480f18,DAT_00480890,0,0xcc0020
                 );
  }
  return;
}



/* ================= FUN_00411b30 @ 00411b30 ================= */

void __cdecl FUN_00411b30(int param_1,int param_2,int param_3,int param_4)

{
  ulonglong uVar1;
  ulonglong uVar2;
  uint uVar3;
  ulonglong uVar4;
  int local_10;
  ulonglong *local_c;
  ulonglong *local_8;
  
  uVar3 = *(uint *)(&DAT_0042fb98 + param_4 * 4);
  local_c = (ulonglong *)(param_3 * 0x100 + DAT_00481500);
  local_8 = (ulonglong *)(param_2 * -0x2c0 + param_1 + DAT_00480f18 + 0x5ac00);
  if (DAT_0042fb6c == 0) {
    local_10 = 0x10;
    do {
      *(uint *)local_8 = (uint)*local_c | uVar3;
      *(uint *)((int)local_8 + 4) = *(uint *)((int)local_c + 4) | uVar3;
      *(uint *)(local_8 + 1) = (uint)local_c[1] | uVar3;
      *(uint *)((int)local_8 + 0xc) = *(uint *)((int)local_c + 0xc) | uVar3;
      local_8 = local_8 + 0x58;
      local_c = local_c + 2;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
    return;
  }
  local_10 = 4;
  uVar4 = CONCAT44(uVar3,uVar3);
  do {
    uVar1 = *local_c;
    *local_8 = uVar1 | uVar4;
    uVar2 = local_c[1];
    local_8[1] = uVar2 | uVar4;
    local_8[0x58] = uVar1 | uVar4;
    local_8[0x59] = uVar2 | uVar4;
    uVar1 = local_c[4];
    local_8[0xb0] = uVar1 | uVar4;
    uVar2 = local_c[5];
    local_8[0xb1] = uVar2 | uVar4;
    local_8[0x108] = uVar1 | uVar4;
    local_8[0x109] = uVar2 | uVar4;
    local_c = local_c + 8;
    local_8 = local_8 + 0x160;
    local_10 = local_10 + -1;
  } while (local_10 != 0);
  return;
}



/* ================= FUN_00411c50 @ 00411c50 ================= */

void __cdecl FUN_00411c50(int param_1,int param_2,int param_3,ushort param_4)

{
  ulonglong uVar1;
  ulonglong uVar2;
  ulonglong *puVar3;
  ushort uVar4;
  int iVar5;
  ulonglong uVar6;
  int local_10;
  ulonglong *local_c;
  ulonglong *local_8;
  
  uVar4 = param_4 << 8 | param_4;
  local_c = (ulonglong *)(param_3 * 0x400 + DAT_00480f14);
  local_8 = (ulonglong *)(param_2 * -0x2c0 + DAT_00480f18 + param_1 + 0x58000);
  local_10 = 0x20;
  if (DAT_0042fb6c != 0) {
    uVar2 = CONCAT44(CONCAT22(uVar4,uVar4),CONCAT22(uVar4,uVar4));
    do {
      uVar1 = *local_c;
      uVar6 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      *local_8 = uVar1 | *local_8 & uVar6 | ~uVar6 & uVar2;
      uVar1 = local_c[1];
      uVar6 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_8[1] = uVar1 | local_8[1] & uVar6 | ~uVar6 & uVar2;
      uVar1 = local_c[2];
      uVar6 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_8[2] = uVar1 | local_8[2] & uVar6 | ~uVar6 & uVar2;
      uVar1 = local_c[3];
      uVar6 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_8[3] = uVar1 | local_8[3] & uVar6 | ~uVar6 & uVar2;
      local_c = local_c + 4;
      local_8 = local_8 + 0x58;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
    return;
  }
  do {
    iVar5 = 4;
    puVar3 = local_8;
    do {
      local_8 = puVar3;
      if ((short)*local_c != 0) {
        *(ushort *)local_8 = (short)*local_c + uVar4;
      }
      if (*(short *)((int)local_c + 2) != 0) {
        *(ushort *)((int)local_8 + 2) = *(short *)((int)local_c + 2) + uVar4;
      }
      if (*(short *)((int)local_c + 4) != 0) {
        *(ushort *)((int)local_8 + 4) = *(short *)((int)local_c + 4) + uVar4;
      }
      if (*(short *)((int)local_c + 6) != 0) {
        *(ushort *)((int)local_8 + 6) = *(short *)((int)local_c + 6) + uVar4;
      }
      local_c = local_c + 1;
      iVar5 = iVar5 + -1;
      puVar3 = local_8 + 1;
    } while (iVar5 != 0);
    local_8 = local_8 + 0x55;
    local_10 = local_10 + -1;
  } while (local_10 != 0);
  return;
}



/* ================= FUN_00411df0 @ 00411df0 ================= */

void __cdecl FUN_00411df0(int param_1,int param_2,int param_3,ushort param_4)

{
  ulonglong uVar1;
  ulonglong uVar2;
  ulonglong *puVar3;
  ulonglong *puVar4;
  ushort uVar5;
  int iVar6;
  ulonglong uVar7;
  int local_10;
  ulonglong *local_c;
  ulonglong *local_8;
  
  uVar5 = param_4 << 8 | param_4;
  local_c = (ulonglong *)(param_2 * -0x2c0 + DAT_00480f18 + param_1 + 0x58000);
  local_8 = (ulonglong *)(param_3 * 0x400 + DAT_00480f14 + 0x3e0);
  local_10 = 0x20;
  if (DAT_0042fb6c != 0) {
    uVar2 = CONCAT44(CONCAT22(uVar5,uVar5),CONCAT22(uVar5,uVar5));
    do {
      uVar1 = *local_8;
      uVar7 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      *local_c = uVar1 | *local_c & uVar7 | ~uVar7 & uVar2;
      uVar1 = local_8[1];
      uVar7 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_c[1] = uVar1 | local_c[1] & uVar7 | ~uVar7 & uVar2;
      uVar1 = local_8[2];
      uVar7 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_c[2] = uVar1 | local_c[2] & uVar7 | ~uVar7 & uVar2;
      uVar1 = local_8[3];
      uVar7 = CONCAT26(-(ushort)((short)(uVar1 >> 0x30) == 0),
                       CONCAT24(-(ushort)((short)(uVar1 >> 0x20) == 0),
                                CONCAT22(-(ushort)((short)(uVar1 >> 0x10) == 0),
                                         -(ushort)((short)uVar1 == 0))));
      local_c[3] = uVar1 | local_c[3] & uVar7 | ~uVar7 & uVar2;
      local_8 = local_8 + -4;
      local_c = local_c + 0x58;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
    return;
  }
  do {
    iVar6 = 4;
    puVar3 = local_c;
    puVar4 = local_8;
    do {
      local_8 = puVar4;
      local_c = puVar3;
      if ((short)*local_8 != 0) {
        *(ushort *)local_c = (short)*local_8 + uVar5;
      }
      if (*(short *)((int)local_8 + 2) != 0) {
        *(ushort *)((int)local_c + 2) = *(short *)((int)local_8 + 2) + uVar5;
      }
      if (*(short *)((int)local_8 + 4) != 0) {
        *(ushort *)((int)local_c + 4) = *(short *)((int)local_8 + 4) + uVar5;
      }
      if (*(short *)((int)local_8 + 6) != 0) {
        *(ushort *)((int)local_c + 6) = *(short *)((int)local_8 + 6) + uVar5;
      }
      iVar6 = iVar6 + -1;
      puVar3 = local_c + 1;
      puVar4 = local_8 + 1;
    } while (iVar6 != 0);
    local_8 = local_8 + -7;
    local_c = local_c + 0x55;
    local_10 = local_10 + -1;
  } while (local_10 != 0);
  return;
}



/* ================= FUN_00411f90 @ 00411f90 ================= */

void __cdecl FUN_00411f90(int param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  puVar2 = (undefined4 *)((param_2 + param_4) * -0x2c0 + DAT_00480f18 + 0x5d800 + param_1);
  do {
    puVar3 = puVar2;
    for (uVar1 = param_3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar3 = 0;
      puVar3 = puVar3 + 1;
    }
    for (uVar1 = param_3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
      *(undefined1 *)puVar3 = 0;
      puVar3 = (undefined4 *)((int)puVar3 + 1);
    }
    param_4 = param_4 + -1;
    puVar2 = puVar2 + 0xb0;
  } while (param_4 != 0);
  return;
}



/* ================= FUN_00411fe0 @ 00411fe0 ================= */

void FUN_00411fe0(void)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 local_6c [4];
  int iStack_5c;
  undefined4 *puStack_48;
  
  puVar4 = (undefined4 *)(DAT_00480f18 + 0x57d60);
  puVar3 = local_6c;
  for (iVar1 = 0x1b; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  local_6c[0] = 0x6c;
  do {
    iVar1 = (**(code **)(*DAT_00480e70 + 100))(DAT_00480e70,0,local_6c,0,0);
  } while (iVar1 != 0);
  iVar1 = 0x1e0;
  puVar3 = puStack_48;
  do {
    puVar5 = puVar4;
    puVar6 = puVar3;
    for (iVar2 = 0xa0; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar6 = *puVar5;
      puVar5 = puVar5 + 1;
      puVar6 = puVar6 + 1;
    }
    puVar3 = (undefined4 *)((int)puVar3 + iStack_5c);
    iVar1 = iVar1 + -1;
    puVar4 = puVar4 + -0xb0;
  } while (iVar1 != 0);
  (**(code **)(*DAT_00480e70 + 0x80))(DAT_00480e70,puStack_48);
  return;
}



/* ================= FUN_00412060 @ 00412060 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00412060(void)

{
  byte *pbVar1;
  byte bVar2;
  ushort uVar3;
  DWORD DVar4;
  HDC hdc;
  ushort *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  char *pcVar11;
  uint uVar12;
  char *pcVar13;
  bool bVar14;
  int local_10;
  ushort *local_c;
  int local_8;
  uint local_4;
  
  if (-1 < DAT_0042fb90) {
    FUN_0040b260(DAT_0042fb90 + -1,0);
    DAT_0042fb90 = -1;
  }
  DAT_004814f0 = DAT_004814f0 + 1;
  DAT_00480888 = 1;
  DVar4 = timeGetTime();
  iVar8 = DAT_004814f0;
  uVar12 = DVar4 - _DAT_00480f00;
  DAT_00480f0c = (DAT_00480f0c + 1) % 0x3c;
  (&DAT_00480e30)[DAT_00480f0c] = 0;
  if (((uint)(iVar8 * 0x14) < uVar12) && (DAT_00480ed0 < 6)) {
    DAT_00480ed0 = DAT_00480ed0 + 1;
    return;
  }
  if (DAT_00480ed0 == 0) {
    DAT_00480888 = 0;
  }
  if (5 < DAT_00480ed0) {
    _DAT_00480f00 = timeGetTime();
    DAT_004814f0 = 0;
  }
  iVar8 = 0;
  bVar14 = DAT_004834aa == '\x01';
  (&DAT_00480e30)[DAT_00480f0c] = 1;
  DAT_00480ed0 = 0;
  if (bVar14) {
    puVar5 = &DAT_004834d0;
    do {
      *(char *)(DAT_00480890 + 0x6a + iVar8) = (char)*puVar5 << 3;
      iVar8 = iVar8 + 4;
      *(byte *)(DAT_00480890 + 0x65 + iVar8) = (byte)(*puVar5 >> 2) & 0xf8;
      *(byte *)(DAT_00480890 + 100 + iVar8) = (byte)(*puVar5 >> 7) & 0xf8;
      puVar5 = puVar5 + 1;
    } while (iVar8 < 0x200);
    iVar8 = 0;
    puVar5 = &DAT_004835e0;
    do {
      *(char *)(DAT_00480890 + 0x26a + iVar8) = (char)*puVar5 << 3;
      iVar8 = iVar8 + 4;
      *(byte *)(DAT_00480890 + 0x265 + iVar8) = (byte)(*puVar5 >> 2) & 0xf8;
      *(byte *)(DAT_00480890 + 0x264 + iVar8) = (byte)(*puVar5 >> 7) & 0xf8;
      puVar5 = puVar5 + 1;
    } while (iVar8 < 0x180);
    iVar8 = 0x40;
    do {
      iVar6 = iVar8 + 4;
      pbVar1 = (byte *)(DAT_00480890 + 0x26 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      pbVar1 = (byte *)(DAT_00480890 + 0x25 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      pbVar1 = (byte *)(DAT_00480890 + 0x24 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      (&DAT_00480f08->palPalEntry[0].peRed)[iVar8] = *(BYTE *)(DAT_00480890 + 0x26 + iVar6);
      (&DAT_00480f08->palPalEntry[0].peGreen)[iVar8] = *(BYTE *)(DAT_00480890 + 0x25 + iVar6);
      (&DAT_00480f08->palPalEntry[0].peBlue)[iVar8] = *(BYTE *)(DAT_00480890 + 0x24 + iVar6);
      iVar8 = iVar6;
    } while (iVar6 < 0x3c0);
    if (DAT_00480ef4 == 10) {
      iVar8 = 0;
      local_c = (ushort *)0x0;
      local_10 = 0;
      local_8 = 0;
      bVar14 = DAT_00480f04 == 10;
      if (10 < DAT_00480f04) {
        iVar6 = 0x28;
        iVar9 = 10;
        do {
          iVar7 = 4;
          pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
          pcVar13 = (char *)(DAT_00480890 + 0x48);
          do {
            if (iVar7 == 0) break;
            iVar7 = iVar7 + -1;
            bVar14 = *pcVar11 == *pcVar13;
            pcVar11 = pcVar11 + 1;
            pcVar13 = pcVar13 + 1;
          } while (bVar14);
          if (bVar14) {
            iVar7 = iVar8 + 1;
            iVar8 = iVar7;
LAB_0041230c:
            bVar2 = *(byte *)(DAT_00480890 + 0x28 + iVar6);
            local_4 = (uint)bVar2;
            if ((int)(local_4 - iVar7) < 0) {
              (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6] = (char)iVar7 + bVar2;
            }
            else {
              (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6] = bVar2 - (char)iVar7;
            }
            *(BYTE *)(DAT_00480890 + 0x28 + iVar6) = (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6];
          }
          else {
            iVar7 = 4;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x4c);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = (int)local_c + 1;
              local_c = (ushort *)iVar7;
              goto LAB_0041230c;
            }
            iVar7 = 4;
            bVar14 = false;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x400);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = local_10 + 1;
              local_10 = iVar7;
              goto LAB_0041230c;
            }
            iVar7 = 4;
            bVar14 = false;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x404);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = local_8 + 1;
              local_8 = iVar7;
              goto LAB_0041230c;
            }
          }
          iVar6 = iVar6 + 4;
          iVar9 = iVar9 + 1;
          bVar14 = iVar9 == DAT_00480f04;
        } while (iVar9 < DAT_00480f04);
      }
    }
    if (DAT_00480f10 != (HPALETTE)0x0) {
      DeleteObject(DAT_00480f10);
    }
    DAT_00480f10 = CreatePalette(DAT_00480f08);
    DAT_004834aa = '\0';
    if ((DAT_0042fb68 != 0) && (DAT_00480e6c != (int *)0x0)) {
      (**(code **)(*DAT_00480e6c + 0x18))(DAT_00480e6c,0,0,0x100,DAT_00480f08->palPalEntry);
    }
  }
  uVar12 = (DAT_004814f8 == 0) - 1 & 0x20;
  if (DAT_0042fb94 == 0) {
    uVar12 = 0x280;
    iVar8 = 0x20;
  }
  else {
    local_c = &DAT_00494740;
    local_10 = 0;
    do {
      uVar10 = local_10 - (uint)DAT_004834c0 & 0xff;
      if (0xf8 < uVar10) {
        uVar10 = uVar10 - 0x100;
      }
      if ((((int)uVar10 < 0xf1) && (iVar8 = uVar10 * 2, -0x11 < iVar8)) && (iVar8 < 0x1e1)) {
        iVar6 = 0;
        puVar5 = local_c;
        do {
          uVar10 = iVar6 - (uint)DAT_004834ba & 0x1ff;
          if (0x1f8 < uVar10) {
            uVar10 = uVar10 - 0x200;
          }
          if ((((int)uVar10 < 0x141) && (iVar9 = (uVar10 + uVar12) * 2, -0x11 < iVar9)) &&
             (iVar9 < 0x281)) {
            FUN_00411b30(iVar9 + 0x20,iVar8 + 0x20,*puVar5 & 0x7ff,(*puVar5 >> 0xb) + 1);
          }
          puVar5 = puVar5 + 1;
          iVar6 = iVar6 + 8;
        } while (iVar6 < 0x200);
      }
      local_c = local_c + 0x40;
      local_10 = local_10 + 8;
    } while (local_10 < 0x100);
    iVar8 = 0xff;
    do {
      iVar6 = ((uint)DAT_004836e8 * 0x100 + iVar8) * 8;
      iVar9 = (*(ushort *)(&DAT_004836f0 + iVar6) + uVar12) * 2;
      if ((-0x21 < iVar9 + -0x20) && (iVar9 + -0x20 < 0x281)) {
        iVar7 = (uint)*(ushort *)(&DAT_004836f2 + iVar6) * 2;
        if ((-0x21 < iVar7 + -0x20) && (iVar7 + -0x20 < 0x1e1)) {
          uVar3 = *(ushort *)(&DAT_004836f4 + iVar6);
          iVar6 = (uVar3 >> 8) + 0x40 + (*(ushort *)(&DAT_004836f6 + iVar6) & 0xff00);
          uVar10 = (uVar3 & 7) + 8;
          if (0xf < uVar10) {
            uVar10 = 0xf;
          }
          if ((uVar3 & 0x40) != 0) {
            iVar6 = iVar6 + 0x200;
          }
          if ((uVar3 & 0x80) == 0) {
            FUN_00411c50(iVar9,iVar7,iVar6,(ushort)(uVar10 << 4));
          }
          else {
            FUN_00411df0(iVar9,iVar7,iVar6,(ushort)(uVar10 << 4));
          }
        }
      }
      iVar8 = iVar8 + -1;
    } while (-1 < iVar8);
    if (DAT_004814f8 == 0) goto LAB_004125d7;
    FUN_00411f90(0x20,0x20,0x40,0x1e0);
    uVar12 = 0x40;
    iVar8 = 0x260;
  }
  FUN_00411f90(iVar8,0x20,uVar12,0x1e0);
LAB_004125d7:
  if (DAT_0042fb68 == 0) {
    hdc = GetDC(DAT_00480e28);
    if ((DAT_00480f10 != (HPALETTE)0x0) && (DAT_0042fb68 == 0)) {
      SelectPalette(hdc,DAT_00480f10,0);
    }
    RealizePalette(hdc);
    FUN_00411af0(hdc);
    ReleaseDC(DAT_00480e28,hdc);
    return;
  }
  FUN_00411fe0();
  do {
    iVar8 = (**(code **)(*DAT_004814ec + 0x2c))(DAT_004814ec,0,1);
    if (iVar8 == -0x7789fe3e) {
      (**(code **)(*DAT_004814ec + 0x6c))(DAT_004814ec);
      (**(code **)(*DAT_00480e70 + 0x6c))(DAT_00480e70);
      return;
    }
  } while (iVar8 != 0);
  return;
}



/* ================= FUN_00412680 @ 00412680 ================= */

void __cdecl FUN_00412680(int param_1,int param_2)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte local_10 [8];
  int local_8;
  int local_4;
  
  local_8 = (param_1 >> 4) * 0x100 + DAT_00481500;
  pbVar5 = &DAT_00484730 + (param_1 >> 4) * 0x20;
  if (0 < param_2) {
    local_4 = param_2;
    do {
      iVar4 = 8;
      pbVar6 = (byte *)(local_8 + 0xf1);
      do {
        local_10[0] = *pbVar5 >> 4;
        local_10[1] = pbVar5[1] >> 4;
        local_10[2] = pbVar5[2] >> 4;
        local_10[3] = pbVar5[3] >> 4;
        local_10[5] = pbVar5[1] & 0xf;
        iVar3 = 0;
        local_10[4] = *pbVar5 & 0xf;
        local_10[6] = pbVar5[2] & 0xf;
        local_10[7] = pbVar5[3] & 0xf;
        pbVar2 = pbVar6;
        do {
          bVar1 = local_10[iVar3];
          iVar3 = iVar3 + 1;
          *pbVar2 = bVar1;
          pbVar2[-1] = bVar1;
          pbVar2[-0x10] = bVar1;
          pbVar2[-0x11] = bVar1;
          pbVar2 = pbVar2 + 2;
        } while (iVar3 < 8);
        pbVar5 = pbVar5 + 4;
        pbVar6 = pbVar6 + -0x20;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      local_8 = local_8 + 0x100;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}



/* ================= FUN_00412750 @ 00412750 ================= */

void __cdecl FUN_00412750(int param_1,int param_2)

{
  byte bVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  int iVar7;
  byte local_1c [16];
  int local_c;
  int local_8;
  int local_4;
  
  local_c = (param_1 >> 6) * 0x400 + DAT_00480f14;
  pbVar3 = &DAT_00484730 + (param_1 >> 6) * 0x80;
  if (0 < param_2) {
    local_4 = param_2;
    do {
      pbVar4 = (byte *)(local_c + 0x803ff);
      pbVar6 = (byte *)(local_c + 0x3e1);
      local_8 = 0x10;
      do {
        pbVar2 = pbVar3;
        local_1c[1] = *pbVar2 & 0xf;
        local_1c[0] = *pbVar2 >> 4;
        local_1c[3] = pbVar2[1] & 0xf;
        local_1c[2] = pbVar2[1] >> 4;
        local_1c[5] = pbVar2[2] & 0xf;
        local_1c[4] = pbVar2[2] >> 4;
        local_1c[7] = pbVar2[3] & 0xf;
        local_1c[6] = pbVar2[3] >> 4;
        local_1c[9] = pbVar2[0x40] & 0xf;
        local_1c[8] = pbVar2[0x40] >> 4;
        local_1c[0xb] = pbVar2[0x41] & 0xf;
        local_1c[10] = pbVar2[0x41] >> 4;
        local_1c[0xd] = pbVar2[0x42] & 0xf;
        local_1c[0xc] = pbVar2[0x42] >> 4;
        iVar7 = 0;
        local_1c[0xe] = pbVar2[0x43] >> 4;
        local_1c[0xf] = pbVar2[0x43] & 0xf;
        pbVar3 = pbVar4;
        pbVar5 = pbVar6;
        do {
          bVar1 = local_1c[iVar7];
          *pbVar3 = bVar1;
          iVar7 = iVar7 + 1;
          pbVar3[-1] = bVar1;
          pbVar3[-0x20] = bVar1;
          pbVar3[-0x21] = bVar1;
          *pbVar5 = bVar1;
          pbVar5[-1] = bVar1;
          pbVar5[-0x20] = bVar1;
          pbVar5[-0x21] = bVar1;
          pbVar3 = pbVar3 + -2;
          pbVar5 = pbVar5 + 2;
        } while (iVar7 < 0x10);
        pbVar4 = pbVar4 + -0x40;
        pbVar6 = pbVar6 + -0x40;
        local_8 = local_8 + -1;
        pbVar3 = pbVar2 + 4;
      } while (local_8 != 0);
      local_c = local_c + 0x400;
      pbVar3 = pbVar2 + 0x44;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}



/* ================= FUN_00412890 @ 00412890 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00412890(byte param_1)

{
  char cVar1;
  DWORD DVar2;
  int iVar3;
  DWORD DVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  char local_180 [4];
  undefined1 local_17c;
  CHAR local_100 [256];
  
  if ((DAT_00480e2c != 0) && (((param_1 == 0x7f || (param_1 == 0x7d)) || (param_1 == 0x7c)))) {
    FUN_00420870(&DAT_00480e78);
  }
  if ((0x24 < param_1) && (param_1 < 0x39)) {
    uVar6 = (uint)param_1;
    DAT_0042fb8c = uVar6;
    if (DAT_00480e2c != 0) {
      FUN_00420870(&DAT_00480e78);
      uVar5 = 0xffffffff;
      pcVar7 = *(char **)(s_nec_m03_mid_0042fb44 + uVar6 * 4);
      do {
        pcVar8 = pcVar7;
        if (uVar5 == 0) break;
        uVar5 = uVar5 - 1;
        pcVar8 = pcVar7 + 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar8;
      } while (cVar1 != '\0');
      uVar5 = ~uVar5;
      pcVar7 = pcVar8 + -uVar5;
      pcVar8 = local_180;
      for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
        pcVar7 = pcVar7 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
        *pcVar8 = *pcVar7;
        pcVar7 = pcVar7 + 1;
        pcVar8 = pcVar8 + 1;
      }
      if (DAT_00481504 == 0) {
        local_17c = 0x42;
      }
      wsprintfA(local_100,s__s__s_s_0042fcd0,&DAT_00481328,s_SOUND__0042fb7c,local_180);
      DVar2 = timeGetTime();
      iVar3 = FUN_00420590(&DAT_00480e78,0x42fcd8,local_100);
      if (iVar3 == 0) {
        _DAT_00480e78 = DAT_00480e28;
        _DAT_00480e8c = _DAT_00480e8c | 1;
        FUN_004206c0(&DAT_00480e78,0x806);
      }
      DVar4 = timeGetTime();
      _DAT_00480f00 = _DAT_00480f00 + (DVar4 - DVar2);
      return;
    }
  }
  if (((DAT_0048193c != 0) && (param_1 != 0)) &&
     ((param_1 < 0x25 && (((int)DAT_0042fb90 < 0xe || (0x10 < (int)DAT_0042fb90)))))) {
    DAT_0042fb90 = (uint)param_1;
  }
  return;
}



/* ================= FUN_004129f0 @ 004129f0 ================= */

void FUN_004129f0(void)

{
  DAT_004814f8 = 1;
  return;
}



/* ================= FUN_00412a00 @ 00412a00 ================= */

void FUN_00412a00(void)

{
  DAT_004814f8 = 0;
  return;
}



/* ================= FUN_00412a10 @ 00412a10 ================= */

void FUN_00412a10(void)

{
  DAT_0042fb94 = 1;
  return;
}



/* ================= FUN_00412a20 @ 00412a20 ================= */

void FUN_00412a20(void)

{
  DAT_0042fb94 = 0;
  return;
}



/* ================= FUN_00412a30 @ 00412a30 ================= */

void FUN_00412a30(void)

{
  DAT_004834a4 = 0;
  DAT_004834b6 = 0;
  DAT_004834ac = 0;
  DAT_00495c63 = 0;
  DAT_004834a8 = 0;
  DAT_00495c6c = 0;
  DAT_004834a1 = 0;
  DAT_00495c68 = 0;
  DAT_00495c57 = 0;
  DAT_0048349f = 0;
  SetWindowTextA(DAT_00480e28,s_Nectaris_0042fc7c);
  FUN_00413050();
  return;
}



/* ================= FUN_00412a80 @ 00412a80 ================= */

void __cdecl FUN_00412a80(uint param_1)

{
  DAT_004814fc = 0;
  DAT_00480efc = param_1 & 0xffff;
  DAT_00480ef8 = param_1 >> 0x10;
  return;
}



/* ================= FUN_00412ab0 @ 00412ab0 ================= */

void __cdecl FUN_00412ab0(int param_1,int param_2)

{
  int iVar1;
  tagPOINT local_8;
  
  local_8.x = param_1 * 2;
  local_8.y = param_2 * 2;
  if (DAT_004814f8 != 0) {
    local_8.x = local_8.x + 0x40;
  }
  if (DAT_0042fb68 != 0) {
    iVar1 = GetSystemMetrics(0xf);
    local_8.y = local_8.y - iVar1;
  }
  ClientToScreen(DAT_00480e28,&local_8);
  SetCursorPos(local_8.x,local_8.y);
  DAT_00480ef8 = param_1;
  DAT_00480efc = param_2;
  DAT_004814fc = 0;
  return;
}



/* ================= FUN_00412b70 @ 00412b70 ================= */

void __cdecl
FUN_00412b70(undefined4 param_1,undefined4 param_2,undefined2 param_3,undefined2 param_4)

{
  DAT_00480884 = (short)param_1;
  DAT_00481428 = (short)param_2;
  DAT_00480ef8 = param_1;
  DAT_00480ef0 = param_3;
  DAT_00480efc = param_2;
  DAT_004814fc = 0;
  DAT_004814e8 = param_4;
  return;
}



/* ================= FUN_00412bb0 @ 00412bb0 ================= */

void FUN_00412bb0(void)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  tagPOINT local_8;
  
  GetCursorPos(&local_8);
  ScreenToClient(DAT_00480e28,&local_8);
  if (DAT_004814f8 != 0) {
    local_8.x = local_8.x + -0x40;
  }
  if (DAT_0042fb68 != 0) {
    iVar3 = GetSystemMetrics(0xf);
    local_8.y = local_8.y + iVar3;
  }
  uVar1 = GetKeyState(1);
  uVar2 = GetKeyState(2);
  DAT_004814fc = 0;
  local_8.x = local_8.x / 2;
  if ((((DAT_00480884 <= local_8.x) && (local_8.x < DAT_00480ef0)) &&
      (local_8.y = local_8.y / 2, DAT_00481428 <= local_8.y)) &&
     ((local_8.y < DAT_004814e8 && ((local_8.x != DAT_00480ef8 || (local_8.y != DAT_00480efc)))))) {
    DAT_004814fc = 0xff;
    DAT_00480ef8 = local_8.x;
    DAT_00480efc = local_8.y;
  }
  if ((uVar1 & 0x80) != 0) {
    DAT_004834b6 = DAT_004834b6 | 0x20;
  }
  if ((uVar2 & 0x80) != 0) {
    DAT_004834b6 = DAT_004834b6 | 0x40;
  }
  return;
}



/* ================= thunk_FUN_00412060 @ 00412ca0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void thunk_FUN_00412060(void)

{
  byte *pbVar1;
  byte bVar2;
  ushort uVar3;
  DWORD DVar4;
  HDC hdc;
  ushort *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  char *pcVar11;
  uint uVar12;
  char *pcVar13;
  bool bVar14;
  int iStack_10;
  ushort *puStack_c;
  int iStack_8;
  uint uStack_4;
  
  if (-1 < DAT_0042fb90) {
    FUN_0040b260(DAT_0042fb90 + -1,0);
    DAT_0042fb90 = -1;
  }
  DAT_004814f0 = DAT_004814f0 + 1;
  DAT_00480888 = 1;
  DVar4 = timeGetTime();
  iVar8 = DAT_004814f0;
  uVar12 = DVar4 - _DAT_00480f00;
  DAT_00480f0c = (DAT_00480f0c + 1) % 0x3c;
  (&DAT_00480e30)[DAT_00480f0c] = 0;
  if (((uint)(iVar8 * 0x14) < uVar12) && (DAT_00480ed0 < 6)) {
    DAT_00480ed0 = DAT_00480ed0 + 1;
    return;
  }
  if (DAT_00480ed0 == 0) {
    DAT_00480888 = 0;
  }
  if (5 < DAT_00480ed0) {
    _DAT_00480f00 = timeGetTime();
    DAT_004814f0 = 0;
  }
  iVar8 = 0;
  bVar14 = DAT_004834aa == '\x01';
  (&DAT_00480e30)[DAT_00480f0c] = 1;
  DAT_00480ed0 = 0;
  if (bVar14) {
    puVar5 = &DAT_004834d0;
    do {
      *(char *)(DAT_00480890 + 0x6a + iVar8) = (char)*puVar5 << 3;
      iVar8 = iVar8 + 4;
      *(byte *)(DAT_00480890 + 0x65 + iVar8) = (byte)(*puVar5 >> 2) & 0xf8;
      *(byte *)(DAT_00480890 + 100 + iVar8) = (byte)(*puVar5 >> 7) & 0xf8;
      puVar5 = puVar5 + 1;
    } while (iVar8 < 0x200);
    iVar8 = 0;
    puVar5 = &DAT_004835e0;
    do {
      *(char *)(DAT_00480890 + 0x26a + iVar8) = (char)*puVar5 << 3;
      iVar8 = iVar8 + 4;
      *(byte *)(DAT_00480890 + 0x265 + iVar8) = (byte)(*puVar5 >> 2) & 0xf8;
      *(byte *)(DAT_00480890 + 0x264 + iVar8) = (byte)(*puVar5 >> 7) & 0xf8;
      puVar5 = puVar5 + 1;
    } while (iVar8 < 0x180);
    iVar8 = 0x40;
    do {
      iVar6 = iVar8 + 4;
      pbVar1 = (byte *)(DAT_00480890 + 0x26 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      pbVar1 = (byte *)(DAT_00480890 + 0x25 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      pbVar1 = (byte *)(DAT_00480890 + 0x24 + iVar6);
      bVar2 = *pbVar1;
      *pbVar1 = bVar2 >> 5 | bVar2;
      (&DAT_00480f08->palPalEntry[0].peRed)[iVar8] = *(BYTE *)(DAT_00480890 + 0x26 + iVar6);
      (&DAT_00480f08->palPalEntry[0].peGreen)[iVar8] = *(BYTE *)(DAT_00480890 + 0x25 + iVar6);
      (&DAT_00480f08->palPalEntry[0].peBlue)[iVar8] = *(BYTE *)(DAT_00480890 + 0x24 + iVar6);
      iVar8 = iVar6;
    } while (iVar6 < 0x3c0);
    if (DAT_00480ef4 == 10) {
      iVar8 = 0;
      puStack_c = (ushort *)0x0;
      iStack_10 = 0;
      iStack_8 = 0;
      bVar14 = DAT_00480f04 == 10;
      if (10 < DAT_00480f04) {
        iVar6 = 0x28;
        iVar9 = 10;
        do {
          iVar7 = 4;
          pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
          pcVar13 = (char *)(DAT_00480890 + 0x48);
          do {
            if (iVar7 == 0) break;
            iVar7 = iVar7 + -1;
            bVar14 = *pcVar11 == *pcVar13;
            pcVar11 = pcVar11 + 1;
            pcVar13 = pcVar13 + 1;
          } while (bVar14);
          if (bVar14) {
            iVar7 = iVar8 + 1;
            iVar8 = iVar7;
LAB_0041230c:
            bVar2 = *(byte *)(DAT_00480890 + 0x28 + iVar6);
            uStack_4 = (uint)bVar2;
            if ((int)(uStack_4 - iVar7) < 0) {
              (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6] = (char)iVar7 + bVar2;
            }
            else {
              (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6] = bVar2 - (char)iVar7;
            }
            *(BYTE *)(DAT_00480890 + 0x28 + iVar6) = (&DAT_00480f08->palPalEntry[0].peBlue)[iVar6];
          }
          else {
            iVar7 = 4;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x4c);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = (int)puStack_c + 1;
              puStack_c = (ushort *)iVar7;
              goto LAB_0041230c;
            }
            iVar7 = 4;
            bVar14 = false;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x400);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = iStack_10 + 1;
              iStack_10 = iVar7;
              goto LAB_0041230c;
            }
            iVar7 = 4;
            bVar14 = false;
            pcVar11 = (char *)(DAT_00480890 + 0x28 + iVar6);
            pcVar13 = (char *)(DAT_00480890 + 0x404);
            do {
              if (iVar7 == 0) break;
              iVar7 = iVar7 + -1;
              bVar14 = *pcVar11 == *pcVar13;
              pcVar11 = pcVar11 + 1;
              pcVar13 = pcVar13 + 1;
            } while (bVar14);
            if (bVar14) {
              iVar7 = iStack_8 + 1;
              iStack_8 = iVar7;
              goto LAB_0041230c;
            }
          }
          iVar6 = iVar6 + 4;
          iVar9 = iVar9 + 1;
          bVar14 = iVar9 == DAT_00480f04;
        } while (iVar9 < DAT_00480f04);
      }
    }
    if (DAT_00480f10 != (HPALETTE)0x0) {
      DeleteObject(DAT_00480f10);
    }
    DAT_00480f10 = CreatePalette(DAT_00480f08);
    DAT_004834aa = '\0';
    if ((DAT_0042fb68 != 0) && (DAT_00480e6c != (int *)0x0)) {
      (**(code **)(*DAT_00480e6c + 0x18))(DAT_00480e6c,0,0,0x100,DAT_00480f08->palPalEntry);
    }
  }
  uVar12 = (DAT_004814f8 == 0) - 1 & 0x20;
  if (DAT_0042fb94 == 0) {
    uVar12 = 0x280;
    iVar8 = 0x20;
  }
  else {
    puStack_c = &DAT_00494740;
    iStack_10 = 0;
    do {
      uVar10 = iStack_10 - (uint)DAT_004834c0 & 0xff;
      if (0xf8 < uVar10) {
        uVar10 = uVar10 - 0x100;
      }
      if ((((int)uVar10 < 0xf1) && (iVar8 = uVar10 * 2, -0x11 < iVar8)) && (iVar8 < 0x1e1)) {
        iVar6 = 0;
        puVar5 = puStack_c;
        do {
          uVar10 = iVar6 - (uint)DAT_004834ba & 0x1ff;
          if (0x1f8 < uVar10) {
            uVar10 = uVar10 - 0x200;
          }
          if ((((int)uVar10 < 0x141) && (iVar9 = (uVar10 + uVar12) * 2, -0x11 < iVar9)) &&
             (iVar9 < 0x281)) {
            FUN_00411b30(iVar9 + 0x20,iVar8 + 0x20,*puVar5 & 0x7ff,(*puVar5 >> 0xb) + 1);
          }
          puVar5 = puVar5 + 1;
          iVar6 = iVar6 + 8;
        } while (iVar6 < 0x200);
      }
      puStack_c = puStack_c + 0x40;
      iStack_10 = iStack_10 + 8;
    } while (iStack_10 < 0x100);
    iVar8 = 0xff;
    do {
      iVar6 = ((uint)DAT_004836e8 * 0x100 + iVar8) * 8;
      iVar9 = (*(ushort *)(&DAT_004836f0 + iVar6) + uVar12) * 2;
      if ((-0x21 < iVar9 + -0x20) && (iVar9 + -0x20 < 0x281)) {
        iVar7 = (uint)*(ushort *)(&DAT_004836f2 + iVar6) * 2;
        if ((-0x21 < iVar7 + -0x20) && (iVar7 + -0x20 < 0x1e1)) {
          uVar3 = *(ushort *)(&DAT_004836f4 + iVar6);
          iVar6 = (uVar3 >> 8) + 0x40 + (*(ushort *)(&DAT_004836f6 + iVar6) & 0xff00);
          uVar10 = (uVar3 & 7) + 8;
          if (0xf < uVar10) {
            uVar10 = 0xf;
          }
          if ((uVar3 & 0x40) != 0) {
            iVar6 = iVar6 + 0x200;
          }
          if ((uVar3 & 0x80) == 0) {
            FUN_00411c50(iVar9,iVar7,iVar6,(ushort)(uVar10 << 4));
          }
          else {
            FUN_00411df0(iVar9,iVar7,iVar6,(ushort)(uVar10 << 4));
          }
        }
      }
      iVar8 = iVar8 + -1;
    } while (-1 < iVar8);
    if (DAT_004814f8 == 0) goto LAB_004125d7;
    FUN_00411f90(0x20,0x20,0x40,0x1e0);
    uVar12 = 0x40;
    iVar8 = 0x260;
  }
  FUN_00411f90(iVar8,0x20,uVar12,0x1e0);
LAB_004125d7:
  if (DAT_0042fb68 == 0) {
    hdc = GetDC(DAT_00480e28);
    if ((DAT_00480f10 != (HPALETTE)0x0) && (DAT_0042fb68 == 0)) {
      SelectPalette(hdc,DAT_00480f10,0);
    }
    RealizePalette(hdc);
    FUN_00411af0(hdc);
    ReleaseDC(DAT_00480e28,hdc);
    return;
  }
  FUN_00411fe0();
  do {
    iVar8 = (**(code **)(*DAT_004814ec + 0x2c))(DAT_004814ec,0,1);
    if (iVar8 == -0x7789fe3e) {
      (**(code **)(*DAT_004814ec + 0x6c))(DAT_004814ec);
      (**(code **)(*DAT_00480e70 + 0x6c))(DAT_00480e70);
      return;
    }
  } while (iVar8 != 0);
  return;
}



/* ================= FUN_00412cc0 @ 00412cc0 ================= */

void FUN_00412cc0(void)

{
  FUN_00412da0(0);
  return;
}



/* ================= FUN_00412cd0 @ 00412cd0 ================= */

void FUN_00412cd0(void)

{
  FUN_00412d20(0);
  return;
}



/* ================= FUN_00412ce0 @ 00412ce0 ================= */

void FUN_00412ce0(void)

{
  FUN_00412da0(1);
  return;
}



/* ================= FUN_00412cf0 @ 00412cf0 ================= */

void FUN_00412cf0(void)

{
  FUN_00412d20(1);
  return;
}



/* ================= FUN_00412d10 @ 00412d10 ================= */

void FUN_00412d10(void)

{
  FUN_00412d20(2);
  return;
}



/* ================= FUN_00412d20 @ 00412d20 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00412d20(int param_1)

{
  undefined1 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = &DAT_00482c30;
  for (iVar2 = 0x18c; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_0048203d = 3;
  puVar1 = &DAT_00482c3b;
  do {
    *puVar1 = 3;
    puVar1 = puVar1 + 0x18;
  } while (puVar1 < (undefined1 *)((int)&DAT_00483268 + 3));
  DAT_00482c38 = 1;
  DAT_00482c34 = &DAT_00482c48;
  DAT_00482c50 = 0xff;
  DAT_00482c30 = 0;
  DAT_004836e9 = 0;
  _DAT_00482c48 = &DAT_00482c30;
  _DAT_00482c4c = 0xffffffff;
  DAT_00482044 = 2;
  FUN_00412da0(param_1);
  return;
}



/* ================= FUN_00412da0 @ 00412da0 ================= */

void __cdecl FUN_00412da0(int param_1)

{
  DAT_004836e5 = 0;
  DAT_004846f4 = (&PTR_PTR_0047b898)[param_1];
  return;
}



/* ================= FUN_00412dc0 @ 00412dc0 ================= */

byte __cdecl FUN_00412dc0(byte param_1)

{
  undefined4 *puVar1;
  byte bVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  if (0x41 < DAT_00482044) {
    return 0xff;
  }
  puVar4 = &DAT_00482c30;
  bVar2 = 0;
  do {
    if (*(char *)(puVar4 + 2) == '\0') break;
    puVar4 = puVar4 + 6;
    bVar2 = bVar2 + 1;
  } while (bVar2 < 0x42);
  if (*(char *)(puVar4 + 2) != '\0') {
    return 0xff;
  }
  puVar3 = &DAT_00482c30;
  do {
    puVar1 = (undefined4 *)puVar3[1];
    if ((param_1 & 0x7f) < (*(byte *)(puVar1 + 2) & 0x7f)) break;
    puVar3 = puVar1;
  } while (puVar1[1] != -1);
  if (puVar3 == puVar1) {
    return 0xff;
  }
  puVar4[1] = puVar1;
  puVar3[1] = puVar4;
  *puVar4 = puVar3;
  *puVar1 = puVar4;
  *(byte *)(puVar4 + 2) = param_1;
  *(undefined1 *)((int)puVar4 + 0xb) = DAT_0048203d;
  *(undefined1 *)((int)puVar4 + 10) = 0;
  DAT_00482044 = DAT_00482044 + 1;
  DAT_00482c20 = puVar4;
  return bVar2;
}



/* ================= FUN_00412e60 @ 00412e60 ================= */

void __cdecl FUN_00412e60(int *param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = *param_1;
  piVar2 = (int *)param_1[1];
  *(undefined1 *)((int)param_1 + 9) = 0;
  *(undefined1 *)(param_1 + 2) = 0;
  *(int **)(iVar1 + 4) = piVar2;
  *piVar2 = iVar1;
  param_1[1] = 0;
  DAT_00482044 = DAT_00482044 + -1;
  return;
}



/* ================= FUN_00412e90 @ 00412e90 ================= */

void FUN_00412e90(void)

{
  bool bVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  
  DAT_004836e8 = DAT_004836e8 ^ 1;
  DAT_004836e6 = 0x100;
  puVar4 = &DAT_00482c30;
  DAT_004846f0 = (undefined4 *)(&DAT_004836f0 + (uint)DAT_004836e8 * 0x800);
  iVar3 = DAT_00482c34;
  while ((puVar2 = DAT_004846f0, iVar3 != -1 &&
         ((*(byte *)((int)puVar4 + 9) == 0 ||
          (iVar3 = FUN_00412f40((int)puVar4,*(byte *)((int)puVar4 + 9)), puVar2 = DAT_004846f0,
          iVar3 == 0))))) {
    puVar4 = (undefined4 *)puVar4[1];
    iVar3 = puVar4[1];
  }
  while (bVar1 = DAT_004836e6 != 0, DAT_004836e6 = DAT_004836e6 + -1, bVar1) {
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2 = puVar2 + 2;
  }
  DAT_004834a6 = 1;
  if (DAT_004836e5 != '\0') {
    FUN_00401040();
  }
  return;
}



/* ================= FUN_00412f40 @ 00412f40 ================= */

undefined4 __cdecl FUN_00412f40(int param_1,byte param_2)

{
  byte *pbVar1;
  byte bVar2;
  short sVar3;
  uint uVar4;
  byte bVar5;
  short sVar6;
  ushort uVar7;
  byte *pbVar8;
  short *psVar9;
  short *psVar10;
  
  pbVar1 = *(byte **)(DAT_004846f4 + -4 + (uint)param_2 * 4);
  pbVar8 = pbVar1 + 1;
  bVar2 = *pbVar1;
  uVar7 = (ushort)bVar2;
  if (DAT_004836e6 < bVar2) {
    return 0xffffffff;
  }
  DAT_004836e6 = DAT_004836e6 - bVar2;
  psVar9 = DAT_004846f0;
  do {
    sVar6 = (short)(char)*pbVar8 + *(short *)(param_1 + 0x10) + -0x30;
    psVar10 = psVar9;
    if ((sVar6 < 0x110) &&
       (sVar3 = (short)(char)pbVar8[1] + *(short *)(param_1 + 0xc) + -0x10, sVar3 < 0x150)) {
      *psVar9 = sVar3;
      psVar9[1] = sVar6;
      uVar4 = (uint)(ushort)pbVar8[2];
      if (DAT_004836e5 == '\0') {
        if (*(char *)(param_1 + 10) != '\0') {
          uVar4 = (uint)(ushort)(pbVar8[2] + 0x100);
        }
      }
      else {
        bVar2 = FUN_00401070();
        uVar4 = (uint)bVar2;
      }
      bVar2 = pbVar8[3];
      if ((bVar2 & 6) == 0) {
        bVar5 = *(byte *)(param_1 + 0x14) & 0xf;
      }
      else {
        bVar5 = (byte)((short)(ushort)*(byte *)(param_1 + 0x14) >> 4);
      }
      if ((bVar2 & 0x80) != 0) {
        bVar5 = bVar5 | 0x80;
      }
      if ((bVar2 & 8) != 0) {
        bVar5 = bVar5 | 0x40;
      }
      *(byte *)(psVar9 + 2) = bVar5 + 1;
      *(char *)((int)psVar9 + 5) = (char)uVar4;
      psVar10 = psVar9 + 4;
      psVar9[3] = (short)CONCAT31((int3)(uVar4 >> 8),1);
    }
    pbVar8 = pbVar8 + 4;
    uVar7 = uVar7 - 1;
    psVar9 = psVar10;
  } while (uVar7 != 0);
  DAT_004846f0 = psVar10;
  return 0;
}



/* ================= FUN_00413050 @ 00413050 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00413050(void)

{
  FUN_0040dad0();
  DAT_004834a6 = 0;
  DAT_004836e3 = 0;
  DAT_004835d0 = 0;
  DAT_004834bc = 0;
  DAT_0048349d = 0;
  DAT_0048349e = 0;
  DAT_004834aa = 0;
  DAT_004834ae = 0;
  DAT_004834af = 0;
  DAT_004834a0 = 0;
  DAT_004834b4 = 0;
  DAT_004834b8 = 0;
  DAT_004836e2 = 0;
  DAT_004836e4 = 0;
  DAT_00481a68 = 0;
  DAT_0042fb74 = 0;
  DAT_0042a4e0 = 0;
  _DAT_0047f7d0 = DAT_00480898;
  _DAT_004834b0 = &LAB_004034d0;
  DAT_0047f7d4 = DAT_0048089c;
  DAT_0047f7d8 = DAT_004808a0;
  DAT_0047f7dc = DAT_004808a4;
  _DAT_0047f790 = DAT_004814f4;
  return;
}



/* ================= FUN_00413100 @ 00413100 ================= */

void FUN_00413100(void)

{
  FUN_00404a70();
  FUN_0041ddd0();
  DAT_00495740 = DAT_00495740 + '\x01';
  DAT_004834bc = DAT_004834bc + '\x01';
  FUN_00404a40();
  FUN_00412e90();
  return;
}



/* ================= FUN_00413120 @ 00413120 ================= */

void FUN_00413120(void)

{
  DAT_00495c53 = 0;
  DAT_00495c80 = 0;
  FUN_00404940(&LAB_00413150,10);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0x3c;
  FUN_0040cbb0();
  return;
}



/* ================= FUN_00413330 @ 00413330 ================= */

void FUN_00413330(void)

{
  FUN_00404940(&LAB_00413d10,10);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0x3c;
  *(ushort *)(DAT_00482334 + 0x14) = (ushort)DAT_00495c75;
  DAT_00495c71 = 0x1e;
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_00413370;
  return;
}



/* ================= FUN_004137b0 @ 004137b0 ================= */

void FUN_004137b0(void)

{
  FUN_004048e0(&DAT_00413810,0x50);
  *(undefined1 *)(DAT_00482c20 + 9) = 0xc9;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x108;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0xa0;
  *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
  FUN_0041c020(DAT_00482c20,0x15);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  *(undefined2 *)(DAT_00482334 + 0x14) = 0;
  return;
}



/* ================= FUN_004138a0 @ 004138a0 ================= */

void FUN_004138a0(void)

{
  FUN_004048e0(&LAB_00413910,0x50);
  *(undefined1 *)(DAT_00482c20 + 9) = 0xc5;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x108;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0xa0;
  *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
  FUN_0041c020(DAT_00482c20,0x15);
  *(ushort *)(DAT_00482334 + 0x12) = (ushort)DAT_00431310;
  *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  *(undefined2 *)(DAT_00482334 + 0x14) = 0;
  return;
}



/* ================= FUN_004139b0 @ 004139b0 ================= */

void FUN_004139b0(void)

{
  byte bVar1;
  byte bVar2;
  
  bVar1 = FUN_0040cb50();
  bVar1 = (&DAT_00431318)[bVar1 & 7];
  bVar2 = FUN_004048e0(&LAB_00413aa0,100);
  if (bVar2 != 0xff) {
    *(undefined *)(DAT_00482c20 + 9) = (&DAT_00431320)[bVar1];
    bVar2 = FUN_0040cb50();
    *(ushort *)(DAT_00482c20 + 0xc) = (bVar2 & 0x7f) + 0x60;
    bVar2 = FUN_0040cb50();
    *(ushort *)(DAT_00482c20 + 0x10) = (bVar2 & 0x1f) + 0xe6;
    *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
    FUN_0041c020(DAT_00482c20,0x15);
    *(ushort *)(DAT_00482334 + 0x16) = (ushort)bVar1;
    *(ushort *)(DAT_00482334 + 0x12) = (ushort)(byte)(&DAT_00431324)[bVar1];
    *(undefined2 *)(DAT_00482334 + 0x10) = 0;
    *(undefined2 *)(DAT_00482334 + 0x14) = 0;
    bVar1 = FUN_0040cb50();
    if ((bVar1 & 1) == 0) {
      if ((bVar1 & 3) != 0) {
        FUN_00412890(0x20);
        return;
      }
      FUN_00412890(0x1f);
    }
  }
  return;
}



/* ================= FUN_00413b30 @ 00413b30 ================= */

void FUN_00413b30(void)

{
  if ((DAT_004834bc & 3) == 0) {
    DAT_00495c76 = DAT_00495c76 + 1;
    if (DAT_00495c76 != 9) {
      FUN_0041be90(&DAT_00431258 + (uint)DAT_00495c76 * 0x10,1,0xc);
      return;
    }
    DAT_00495c73 = 1;
    FUN_0041be90(&DAT_00431268,0,1);
  }
  return;
}



/* ================= FUN_00413b80 @ 00413b80 ================= */

void FUN_00413b80(void)

{
  FUN_0041be90(&DAT_00431250,0x10,0x10);
  return;
}



/* ================= FUN_00413ba0 @ 00413ba0 ================= */

void FUN_00413ba0(void)

{
  ushort uVar1;
  ushort uVar2;
  char cVar3;
  ushort uVar4;
  byte *pbVar5;
  byte *pbVar6;
  ushort uVar7;
  short sStack_2;
  
  sStack_2 = 0x500;
  DAT_0048235e = '\n';
  pbVar5 = &DAT_004312e8;
  uVar7 = 0x715;
  do {
    FUN_0040da00(sStack_2 + 0x800);
    FUN_0040da10(sStack_2);
    DAT_0048235b = '\x04';
    pbVar6 = pbVar5;
    do {
      pbVar5 = pbVar6 + 1;
      cVar3 = '\b';
      uVar4 = (ushort)*pbVar6;
      do {
        uVar4 = uVar4 * 2;
        if ((uVar4 & 0x100) == 0) {
          uVar2 = FUN_0040da80();
          uVar2 = uVar2 & 0xfff;
          uVar1 = uVar7;
        }
        else {
          FUN_0040da80();
          uVar1 = uVar7 + 1;
          uVar2 = uVar7;
        }
        uVar7 = uVar1;
        FUN_0040daf0(uVar2);
        cVar3 = cVar3 + -1;
      } while (cVar3 != '\0');
      DAT_0048235b = DAT_0048235b + -1;
      pbVar6 = pbVar5;
    } while (DAT_0048235b != '\0');
    sStack_2 = sStack_2 + 0x40;
    DAT_0048235e = DAT_0048235e + -1;
  } while (DAT_0048235e != '\0');
  return;
}



/* ================= FUN_00413c50 @ 00413c50 ================= */

void __cdecl FUN_00413c50(byte param_1)

{
  undefined2 uVar1;
  uint uVar2;
  byte bVar3;
  ushort uVar4;
  char local_6;
  byte local_5;
  
  uVar2 = (uint)param_1;
  DAT_00482338 = (uint)*(ushort *)(&DAT_00431348 + uVar2 * 2);
  local_5 = 0;
  DAT_0048233c = (byte)(&DAT_00431350)[uVar2] | 0x300;
  do {
    local_6 = '\b';
    bVar3 = (&DAT_00431358)[uVar2 * 8 + (uint)local_5];
    FUN_0040da10((short)DAT_00482338);
    FUN_0040da00((short)DAT_00482338);
    do {
      if (bVar3 < 0x80) {
        uVar1 = FUN_0040da80();
        FUN_0040da20(uVar1);
      }
      else {
        FUN_0040da80();
        uVar4 = (ushort)DAT_0048233c;
        DAT_0048233c = DAT_0048233c + 1;
        FUN_0040daf0(uVar4);
      }
      bVar3 = bVar3 * '\x02';
      local_6 = local_6 + -1;
    } while (local_6 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    local_5 = local_5 + 1;
  } while (local_5 != 8);
  return;
}



/* ================= FUN_00413f30 @ 00413f30 ================= */

void FUN_00413f30(void)

{
  byte bVar1;
  byte bVar2;
  
  bVar1 = FUN_004048e0(&LAB_004142e0,0x50);
  if (bVar1 != 0xff) {
    *(undefined1 *)(DAT_00482c20 + 9) = 0;
    *(undefined2 *)(DAT_00482c20 + 0xc) = 0xa8;
    *(undefined2 *)(DAT_00482c20 + 0x10) = 0xf0;
    FUN_0041c020(DAT_00482c20,0x1e);
    *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
    *(undefined2 *)(DAT_00482334 + 0x1a) = 0;
    *(undefined2 *)(DAT_00482334 + 0x10) = 10;
    *(undefined2 *)(DAT_00482334 + 0x12) = 0;
    bVar2 = FUN_004048e0(&LAB_00414260,0x51);
    if (bVar2 != 0xff) {
      *(undefined1 *)(DAT_00482c20 + 9) = 0xd7;
      *(undefined2 *)(DAT_00482c20 + 0xc) = 0xa8;
      *(undefined2 *)(DAT_00482c20 + 0x10) = 0xf0;
      FUN_0041c020(DAT_00482c20,0x1f);
      *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
      *(undefined2 *)(DAT_00482334 + 0x10) = 0;
      *(ushort *)(DAT_00482334 + 0x14) = (ushort)bVar1;
      bVar1 = FUN_004048e0(&LAB_004140f0,0x52);
      if (bVar1 != 0xff) {
        *(undefined1 *)(DAT_00482c20 + 9) = 0xda;
        *(undefined2 *)(DAT_00482c20 + 0xc) = 0x12e;
        *(undefined2 *)(DAT_00482c20 + 0x10) = 0x126;
        FUN_0041c020(DAT_00482c20,0x1f);
        *(undefined1 *)(DAT_00482c20 + 0xb) = 0x80;
        *(undefined2 *)(DAT_00482334 + 0x18) = 0;
        *(undefined2 *)(DAT_00482334 + 0x14) = 0;
        *(undefined2 *)(DAT_00482334 + 0x1a) = 1;
        *(undefined2 *)(DAT_00482334 + 0x10) = 6;
        *(ushort *)(DAT_00482334 + 0x12) = (ushort)DAT_004313a8;
        bVar2 = FUN_004048e0(&LAB_004141a0,0x50);
        if (bVar2 != 0xff) {
          FUN_0041c020(DAT_00482c20,0x1e);
          *(ushort *)(DAT_00482334 + 0x14) = (ushort)bVar1;
          *(undefined2 *)(DAT_00482334 + 0x18) = 0;
        }
      }
    }
  }
  return;
}



/* ================= FUN_004142c0 @ 004142c0 ================= */

void FUN_004142c0(void)

{
  FUN_0041c050(DAT_00482c20);
  FUN_004049d0();
  return;
}



/* ================= FUN_00414470 @ 00414470 ================= */

void FUN_00414470(void)

{
  char cVar1;
  byte bVar2;
  byte bVar3;
  char local_1;
  
  DAT_00495b74 = 0x4000;
  FUN_004068b0();
  FUN_0040da00(0x2000);
  local_1 = ' ';
  do {
    cVar1 = '\0';
    do {
      FUN_0040da20(0x90);
      cVar1 = cVar1 + -1;
    } while (cVar1 != '\0');
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  DAT_004834c4 = 0x2000;
  bVar2 = 0;
  do {
    bVar3 = bVar2 + 1;
    FUN_00406830(bVar2 - 0x25,1,(&DAT_004313b0)[bVar2]);
    bVar2 = bVar3;
  } while (bVar3 != 6);
  DAT_004834c4 = 0;
  return;
}



/* ================= FUN_004144f0 @ 004144f0 ================= */

void FUN_004144f0(void)

{
  int iVar1;
  char cVar2;
  char local_1;
  
  DAT_004834ba = 0;
  DAT_004834c0 = 0;
  FUN_0040da00(0x3000);
  local_1 = '@';
  do {
    cVar2 = '\0';
    do {
      FUN_0040da20(0);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_00412680(0x3000,0x400);
  local_1 = '\x1e';
  DAT_00482338 = 0;
  DAT_0048233c = 0x2300;
  do {
    cVar2 = ' ';
    FUN_0040da00((short)DAT_00482338);
    do {
      iVar1 = DAT_0048233c;
      DAT_0048233c = DAT_0048233c + 1;
      FUN_0040daf0((ushort)iVar1);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_0041be90(&DAT_004313b8,0x21,8);
  DAT_00495e65 = 0xff;
  DAT_00495e64 = 0xff;
  DAT_00482036 = 0x3000;
  return;
}



/* ================= FUN_004145c0 @ 004145c0 ================= */

void FUN_004145c0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00495820;
  for (iVar1 = 0x30; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = &DAT_00481a80;
  for (iVar1 = 0x30; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  FUN_00404940(&LAB_004145f0,100);
  return;
}



/* ================= FUN_00414740 @ 00414740 ================= */

uint FUN_00414740(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 0x3f;
  while( true ) {
    if ((*(char *)((int)&DAT_00495820 + uVar2) == '\0') && ((&DAT_00495860)[uVar2] == '\0')) break;
    bVar1 = (byte)uVar2 - 1;
    uVar2 = (uint)bVar1;
    if (0x7f < bVar1) {
      return 0xff;
    }
  }
  FUN_00414780((byte)uVar2);
  return uVar2;
}



/* ================= FUN_00414780 @ 00414780 ================= */

void __cdecl FUN_00414780(byte param_1)

{
  byte bVar1;
  uint uVar2;
  undefined1 local_1;
  
  FUN_0040cb50();
  uVar2 = (uint)param_1;
  bVar1 = FUN_0040cb50();
  local_1 = 0;
  (&DAT_004958e0)[uVar2] = bVar1;
  bVar1 = FUN_0040cb50();
  if (0x7f < bVar1) {
    local_1 = 0xff;
  }
  (&DAT_00481ac0)[uVar2] = local_1;
  FUN_0040cb50();
  bVar1 = FUN_0040cb50();
  local_1 = 0;
  *(byte *)((int)&DAT_00481a80 + uVar2) = bVar1;
  bVar1 = FUN_0040cb50();
  if (0x7f < bVar1) {
    local_1 = 0xff;
  }
  (&DAT_00495860)[uVar2] = 2;
  (&DAT_00481b00)[uVar2] = local_1;
  *(undefined1 *)((int)&DAT_00495820 + uVar2) = 0;
  (&DAT_004958a0)[uVar2] = 0;
  return;
}



/* ================= FUN_00414800 @ 00414800 ================= */

void FUN_00414800(void)

{
  undefined1 uVar1;
  int iVar2;
  short sVar3;
  uint uVar4;
  byte unaff_retaddr;
  byte bStack_1;
  
  bStack_1 = 0x3f;
  do {
    uVar4 = (uint)bStack_1;
    if ((*(char *)((int)&DAT_00495820 + uVar4) != '\0') || ((&DAT_00495860)[uVar4] != '\0')) {
      FUN_00414940(bStack_1);
      sVar3 = CONCAT11((&DAT_00481ac0)[uVar4],(&DAT_004958e0)[uVar4]) + 2;
      (&DAT_004958e0)[uVar4] = (char)sVar3;
      (&DAT_00481ac0)[uVar4] = (char)((ushort)sVar3 >> 8);
      iVar2 = FUN_00414960(unaff_retaddr);
      if (iVar2 != 0) {
        uVar1 = (&DAT_004958e0)[uVar4];
        (&DAT_004958e0)[uVar4] = (char)-CONCAT11((&DAT_00481ac0)[uVar4],uVar1);
        (&DAT_00481ac0)[uVar4] = (char)((ushort)-CONCAT11((&DAT_00481ac0)[uVar4],uVar1) >> 8);
      }
    }
    bStack_1 = bStack_1 - 1;
  } while (bStack_1 < 0x80);
  return;
}



/* ================= FUN_00414940 @ 00414940 ================= */

void FUN_00414940(byte param_1)

{
  DAT_0048235e = 0;
  FUN_00414a10(param_1);
  FUN_004149d0();
  return;
}



/* ================= FUN_00414960 @ 00414960 ================= */

undefined4 __cdecl FUN_00414960(byte param_1)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  DAT_0048235e = (byte)((int)((uint)(byte)(&DAT_00495860)[uVar1] << 8) >> 6) |
                 *(byte *)((int)&DAT_00495820 + uVar1) >> 6 | 0x20;
  FUN_00414a10(param_1);
  if ((((&DAT_00481ac0)[uVar1] ^ DAT_0048235a) < 0x80) ||
     (((&DAT_00481b00)[uVar1] ^ DAT_00482358) < 0x80)) {
    return 1;
  }
  FUN_004149d0();
  return 0;
}



/* ================= FUN_004149d0 @ 004149d0 ================= */

void FUN_004149d0(void)

{
  DAT_00495e64 = DAT_0048235a & 0xf8;
  DAT_00495e65 = DAT_00482358;
  FUN_00417170();
  FUN_004170d0();
  return;
}



/* ================= FUN_00414a10 @ 00414a10 ================= */

void __cdecl FUN_00414a10(byte param_1)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  DAT_00482348 = (uint)CONCAT11((&DAT_00481ac0)[uVar1],(&DAT_004958e0)[uVar1]) << 7;
  DAT_0048234c = (uint)CONCAT11((&DAT_00495860)[uVar1],*(undefined1 *)((int)&DAT_00495820 + uVar1));
  FUN_00414ab0();
  DAT_0048235a = (char)DAT_00482348 + -0x80;
  DAT_00482348 = (uint)CONCAT11((&DAT_00481b00)[uVar1],*(undefined1 *)((int)&DAT_00481a80 + uVar1))
                 << 7;
  DAT_0048234c = (uint)CONCAT11((&DAT_00495860)[uVar1],*(undefined1 *)((int)&DAT_00495820 + uVar1));
  FUN_00414ab0();
  DAT_00482358 = (char)DAT_00482348 + -0x80;
  return;
}



/* ================= FUN_00414ab0 @ 00414ab0 ================= */

void FUN_00414ab0(void)

{
  DAT_00482348 = (int)(short)DAT_00482348 / (int)(uint)(ushort)DAT_0048234c;
  return;
}



/* ================= FUN_00414ad0 @ 00414ad0 ================= */

void FUN_00414ad0(void)

{
  FUN_004048e0(&LAB_00414b30,100);
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0xa0;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0xc0;
  FUN_0041c020(DAT_00482c20,0x24);
  *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  *(ushort *)(DAT_00482334 + 0x12) = (ushort)DAT_004313c8;
  FUN_00412890(0x1a);
  return;
}



/* ================= FUN_00414bc0 @ 00414bc0 ================= */

void FUN_00414bc0(void)

{
  byte bVar1;
  byte bVar2;
  ushort uVar3;
  char cVar4;
  short sVar5;
  char local_1;
  
  DAT_00481f94 = &DAT_00431600;
  sVar5 = 0;
  FUN_00418bb0();
  local_1 = ' ';
  do {
    cVar4 = ' ';
    FUN_0040da00(sVar5);
    do {
      bVar1 = FUN_00418bf0();
      bVar2 = FUN_00418bf0();
      FUN_0040daf0((bVar2 + 4) * 0x100 + (ushort)bVar1);
      cVar4 = cVar4 + -1;
    } while (cVar4 != '\0');
    sVar5 = sVar5 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  local_1 = ' ';
  sVar5 = 0;
  do {
    cVar4 = ' ';
    FUN_0040da10(sVar5);
    FUN_0040da00(sVar5 + 0x800);
    do {
      uVar3 = FUN_0040da80();
      FUN_0040daf0(uVar3);
      cVar4 = cVar4 + -1;
    } while (cVar4 != '\0');
    sVar5 = sVar5 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_00413ba0();
  return;
}



/* ================= FUN_00414c80 @ 00414c80 ================= */

void FUN_00414c80(void)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  char local_1;
  
  DAT_00481f94 = &DAT_004313d0;
  sVar3 = 0xa0;
  FUN_00418bb0();
  local_1 = '\x1c';
  do {
    cVar2 = ' ';
    FUN_0040da00(sVar3);
    do {
      bVar1 = FUN_00418bf0();
      FUN_0040daf0(bVar1 + 0x300);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    sVar3 = sVar3 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_00414ce0 @ 00414ce0 ================= */

void FUN_00414ce0(void)

{
  FUN_00412890(0x7c);
  FUN_004163f0(2);
  FUN_00414d10();
  return;
}



/* ================= FUN_00414d00 @ 00414d00 ================= */

void FUN_00414d00(void)

{
  DAT_00483491 = 0;
  FUN_00414d10();
  return;
}



/* ================= FUN_00414d10 @ 00414d10 ================= */

void FUN_00414d10(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_00414d40,100);
  *(undefined2 *)(DAT_00482334 + 0x1e) = 0;
  return;
}



/* ================= FUN_00414f50 @ 00414f50 ================= */

void FUN_00414f50(void)

{
  int iVar1;
  
  if ((uint)DAT_00495c52 - (int)*(short *)(DAT_00482330 + 0x12) != -1) {
    iVar1 = (uint)(byte)*(short *)(DAT_00482330 + 0x12) * 4;
    *(ushort *)(DAT_00482330 + 0x14) = (ushort)(byte)(&DAT_00431d18)[iVar1];
    *(ushort *)(DAT_00482330 + 0x16) = (ushort)(byte)(&DAT_00431d19)[iVar1];
    *(ushort *)(DAT_00482330 + 0x18) = (ushort)(byte)(&DAT_00431d1a)[iVar1];
    *(undefined2 *)(DAT_00482330 + 0x1a) = 0;
    *(ushort *)(DAT_00482330 + 0x1c) = (ushort)(byte)(&DAT_00431d1b)[iVar1];
    *(short *)(DAT_00482330 + 0x12) = *(short *)(DAT_00482330 + 0x12) + 1;
    *(undefined1 **)(DAT_00482330 + 8) = &LAB_00415010;
    FUN_00415110();
    return;
  }
  DAT_00482334 = *(int *)(DAT_00482330 + 0x30);
  if (*(short *)(DAT_00482334 + 0x1e) != 2) {
    *(short *)(DAT_00482334 + 0x1e) = 1;
  }
  FUN_00404a00();
  return;
}



/* ================= FUN_00415110 @ 00415110 ================= */

void FUN_00415110(void)

{
  DAT_00482334 = *(int *)(DAT_00482330 + 0x30);
  if (*(short *)(DAT_00482334 + 0x1e) == 2) {
    FUN_00404a00();
    return;
  }
  return;
}



/* ================= FUN_00415260 @ 00415260 ================= */

void FUN_00415260(void)

{
  FUN_004048e0(&LAB_004152c0,0x32);
  *(undefined4 *)(DAT_00482334 + 0x30) = DAT_00482330;
  *(undefined2 *)(DAT_00482334 + 0x12) = 8;
  *(undefined2 *)(DAT_00482334 + 0x14) = 1;
  FUN_0041c020(DAT_00482c20,0x23);
  FUN_00415450(0);
  FUN_00415330();
  return;
}



/* ================= FUN_00415330 @ 00415330 ================= */

void FUN_00415330(void)

{
  undefined1 uVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int *piVar5;
  short sVar6;
  int iVar7;
  short sVar8;
  int local_1c;
  int local_14;
  int local_8;
  int local_4;
  
  iVar7 = 0;
  local_1c = 0;
  do {
    if (local_1c == 0) {
      uVar1 = 0xe1;
      sVar2 = 0x68;
      local_14 = 5;
      local_8 = 1;
      sVar3 = 0;
      sVar6 = 0x58;
    }
    else {
      uVar1 = 0xe2;
      sVar2 = 0x26;
      local_14 = 8;
      local_8 = 2;
      sVar3 = 0x40;
      sVar6 = 0x78;
    }
    do {
      local_4 = local_14;
      piVar5 = (int *)(&DAT_00495c70 + iVar7 * 4);
      sVar8 = sVar2;
      do {
        iVar7 = iVar7 + 1;
        FUN_00412dc0(10);
        iVar4 = DAT_00482c20;
        *piVar5 = DAT_00482c20;
        FUN_0041c020(iVar4,0x23);
        *(short *)(DAT_00482c20 + 0xc) = sVar8;
        *(short *)(DAT_00482c20 + 0x10) = sVar6;
        sVar8 = sVar8 + 0x20;
        local_4 = local_4 + -1;
        *(undefined1 *)(DAT_00482c20 + 9) = uVar1;
        piVar5 = piVar5 + 1;
      } while (local_4 != 0);
      sVar6 = sVar6 + sVar3;
      local_8 = local_8 + -1;
    } while (local_8 != 0);
    local_1c = local_1c + 1;
  } while (local_1c < 2);
  return;
}



/* ================= FUN_00415420 @ 00415420 ================= */

void FUN_00415420(void)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = (int *)&DAT_00495c70;
  do {
    piVar2 = piVar1 + 1;
    FUN_0041c050(*piVar1);
    FUN_00412e60((int *)*piVar1);
    piVar1 = piVar2;
  } while (piVar2 < &DAT_00495cc4);
  return;
}



/* ================= FUN_00415450 @ 00415450 ================= */

void __cdecl FUN_00415450(byte param_1)

{
  int iVar1;
  int iVar2;
  
  FUN_0040da00(0x1200);
  DAT_00482348 = 8;
  do {
    iVar2 = 8;
    do {
      iVar1 = 4;
      do {
        FUN_0040da20(*(undefined2 *)(&DAT_00431e00 + (uint)param_1 * 4));
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
      iVar1 = 4;
      do {
        FUN_0040da20(*(undefined2 *)(&DAT_00431e02 + (uint)param_1 * 4));
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    DAT_00482348 = DAT_00482348 + -1;
  } while (DAT_00482348 != 0);
  FUN_00412750(0x1200,8);
  return;
}



/* ================= FUN_004154d0 @ 004154d0 ================= */

void FUN_004154d0(void)

{
  char cVar1;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  
  DAT_00481f94 = &DAT_00431e20;
  FUN_00418bb0();
  DAT_00482378 = 0xc0;
  DAT_0048235e = '\x13';
  do {
    DAT_0048235b = ' ';
    FUN_0040da00((short)DAT_00482378);
    do {
      cVar1 = FUN_00418bf0();
      DAT_00482348 = CONCAT31(extraout_var,cVar1);
      cVar1 = FUN_00418bf0();
      DAT_00482348 = DAT_00482348 + (CONCAT31(extraout_var_00,cVar1) + 6) * 0x100;
      FUN_0040daf0((ushort)DAT_00482348);
      DAT_0048235b = DAT_0048235b + -1;
    } while (DAT_0048235b != '\0');
    DAT_00482378 = DAT_00482378 + 0x40;
    DAT_0048235e = DAT_0048235e + -1;
  } while (DAT_0048235e != '\0');
  return;
}



/* ================= FUN_00415550 @ 00415550 ================= */

void FUN_00415550(void)

{
  DAT_004834af = 0;
  DAT_00495c53 = 0;
  FUN_00401050();
  FUN_004163e0();
  FUN_00404940(&LAB_00415580,10);
  return;
}



/* ================= FUN_00415640 @ 00415640 ================= */

void __cdecl
FUN_00415640(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_0040cb30(DAT_00483498);
  if (DAT_00483493 == '\0') {
    return;
  }
  *(undefined4 *)(DAT_00482330 + 8) = 0x415670;
  if (DAT_00495f3c == '\0') {
    FUN_0040cd80(0x17,param_1,param_2,param_3,param_4);
    FUN_00401060();
    *(undefined1 **)(DAT_00482330 + 8) = &LAB_00406ed0;
  }
  return;
}



/* ================= FUN_004156f0 @ 004156f0 ================= */

void FUN_004156f0(void)

{
  char cVar1;
  char local_1;
  
  FUN_00415790(0x180,DAT_00481db3,DAT_00481db4,8);
  FUN_00415790(400,DAT_00481db4,DAT_00481db3,0);
  if (DAT_00481dbc != '\0') {
    FUN_0040dae0();
    DAT_00482338 = 399;
    local_1 = '\x02';
    do {
      cVar1 = '\x10';
      FUN_0040da00((short)DAT_00482338);
      do {
        FUN_0040daf0(0x3090);
        cVar1 = cVar1 + -1;
      } while (cVar1 != '\0');
      DAT_00482338 = DAT_00482338 + 1;
      local_1 = local_1 + -1;
    } while (local_1 != '\0');
    FUN_0040dad0();
  }
  return;
}



/* ================= FUN_00415790 @ 00415790 ================= */

void __cdecl FUN_00415790(short param_1,byte param_2,byte param_3,byte param_4)

{
  char cVar1;
  byte bVar2;
  undefined3 extraout_var;
  char cVar3;
  
  if (DAT_00481dbc == '\0') {
    bVar2 = (&DAT_004321a8)
            [(uint)param_4 +
             (uint)(byte)(&DAT_00432198)[param_2] * 0x10 + (uint)(byte)(&DAT_00432198)[param_3]];
  }
  else {
    bVar2 = (&DAT_00432228)[(uint)(byte)(&DAT_00432198)[param_2] * 2 + (uint)(param_4 >> 3)];
  }
  DAT_00481f94 = (&PTR_DAT_00431148)[bVar2];
  FUN_00418bb0();
  param_2 = 0x10;
  do {
    cVar3 = '\x10';
    FUN_0040da00(param_1);
    do {
      cVar1 = FUN_00418bf0();
      bVar2 = FUN_00418bf0();
      FUN_0040daf0((ushort)CONCAT31(extraout_var,cVar1) | (ushort)bVar2 << 8);
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    param_1 = param_1 + 0x40;
    param_2 = param_2 - 1;
  } while (param_2 != 0);
  return;
}



/* ================= FUN_00415850 @ 00415850 ================= */

void FUN_00415850(void)

{
  char cVar1;
  byte bVar2;
  
  if ((DAT_00481db6 != '\0') && (bVar2 = 4, DAT_00481db2 != DAT_00481e5e)) {
    if (DAT_00481db2 == '\0') {
      bVar2 = 8;
    }
    DAT_0048234c = (uint)bVar2;
    DAT_00482348 = (uint)DAT_0048327b;
    FUN_0040a460();
  }
  if (DAT_00481db2 != '\0') {
    bVar2 = 4;
    if (DAT_00481db2 == DAT_00481e5e) {
      bVar2 = 8;
    }
    DAT_0048234c = (uint)bVar2;
    DAT_00482348 = (uint)DAT_00483274;
    FUN_0040a460();
  }
  DAT_00481da2 = 0x480;
  DAT_00481da4 = 0x20;
  if (DAT_00495c63 == '\0') {
    DAT_0048235a = DAT_0048327b;
  }
  else {
    DAT_0048235a = DAT_00483274;
  }
  FUN_0041d4f0(0x87,'\0');
  if (DAT_00495c63 == '\0') {
    DAT_0048235a = DAT_00483274;
  }
  else {
    DAT_0048235a = DAT_0048327b;
  }
  FUN_0041d4f0(0x97,'\0');
  if (DAT_00495c63 == '\0') {
    cVar1 = DAT_00481db6;
  }
  else {
    cVar1 = DAT_00481db2;
  }
  FUN_004159c0(cVar1);
  if (DAT_00495c63 == '\0') {
    FUN_004159e0(DAT_00481db2);
    return;
  }
  FUN_004159e0(DAT_00481db6);
  return;
}



/* ================= FUN_00415970 @ 00415970 ================= */

void FUN_00415970(void)

{
  char cVar1;
  
  if (DAT_00495c63 == '\0') {
    cVar1 = DAT_00481e5f;
  }
  else {
    cVar1 = DAT_00481e5e;
  }
  FUN_004159c0(cVar1);
  if (DAT_00495c63 == '\0') {
    FUN_004159e0(DAT_00481e5e);
    return;
  }
  FUN_004159e0(DAT_00481e5f);
  return;
}



/* ================= FUN_004159c0 @ 004159c0 ================= */

void __cdecl FUN_004159c0(char param_1)

{
  FUN_00415a00(param_1,0x8b);
  return;
}



/* ================= FUN_004159e0 @ 004159e0 ================= */

void __cdecl FUN_004159e0(char param_1)

{
  FUN_00415a00(param_1,0x93);
  return;
}



/* ================= FUN_00415a00 @ 00415a00 ================= */

void __cdecl FUN_00415a00(char param_1,short param_2)

{
  byte bVar1;
  byte bVar2;
  char local_1;
  
  DAT_00482348 = 2;
  bVar1 = param_1 << 2;
  do {
    FUN_0040da00(param_2);
    local_1 = '\x02';
    bVar2 = bVar1;
    do {
      bVar1 = bVar2 + 1;
      FUN_0040daf0((byte)s_RSTUVWXYZ___Z____abcdefghijklmno_00432238[bVar2] + 0x3480);
      local_1 = local_1 + -1;
      bVar2 = bVar1;
    } while (local_1 != '\0');
    param_2 = param_2 + 0x40;
    DAT_00482348 = DAT_00482348 + -1;
  } while (DAT_00482348 != 0);
  return;
}



/* ================= FUN_00415a70 @ 00415a70 ================= */

void FUN_00415a70(void)

{
  ushort uVar1;
  short sVar2;
  
  sVar2 = 0x700;
  FUN_0040da10(0x900);
  FUN_0040da00(0x900);
  do {
    uVar1 = FUN_0040da80();
    FUN_0040da20(uVar1 | 0xcccc);
    sVar2 = sVar2 + -1;
  } while (sVar2 != 0);
  FUN_00412680(0x900,0x70);
  return;
}



/* ================= FUN_00415ac0 @ 00415ac0 ================= */

void FUN_00415ac0(void)

{
  ushort uVar1;
  short sVar2;
  
  sVar2 = 0x700;
  FUN_0040da10(0x900);
  FUN_0040da00(0x900);
  do {
    uVar1 = FUN_0040da80();
    FUN_0040da20(uVar1 & 0x3333);
    sVar2 = sVar2 + -1;
  } while (sVar2 != 0);
  FUN_00412680(0x900,0x70);
  return;
}



/* ================= FUN_00415b10 @ 00415b10 ================= */

void FUN_00415b10(void)

{
  int iVar1;
  
  iVar1 = 0x800;
  FUN_0040da00(0);
  do {
    FUN_0040daf0(0x3090);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



/* ================= FUN_00415b40 @ 00415b40 ================= */

void FUN_00415b40(void)

{
  FUN_00415b60(0x4a);
  FUN_00415b60(0x52);
  return;
}



/* ================= FUN_00415b60 @ 00415b60 ================= */

void __cdecl FUN_00415b60(short param_1)

{
  byte bVar1;
  byte bVar2;
  char local_1;
  
  bVar2 = 0;
  do {
    FUN_0040da00(param_1);
    local_1 = '\x04';
    bVar1 = bVar2;
    do {
      bVar2 = bVar1 + 1;
      FUN_0040daf0((byte)(&DAT_00432260)[bVar1] + 0x3480);
      local_1 = local_1 + -1;
      bVar1 = bVar2;
    } while (local_1 != '\0');
    param_1 = param_1 + 0x40;
  } while (bVar2 != 0x10);
  return;
}



/* ================= FUN_00415bb0 @ 00415bb0 ================= */

void __cdecl FUN_00415bb0(byte param_1,byte param_2,char param_3)

{
  int iVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  char local_2;
  char local_1;
  
  uVar4 = 0;
  local_1 = '\x03';
  DAT_00482338 = (uint)param_2 * 0x40 + (uint)param_1;
  do {
    FUN_0040da00((short)DAT_00482338);
    local_2 = '\x05';
    do {
      cVar3 = (char)uVar4;
      if (((&DAT_00432270)[uVar4] & 0x80) == 0) {
        DAT_00482348 = (uint)(char)(&DAT_00432272)[uVar4];
        if ((DAT_00483494 & (&DAT_00432270)[uVar4]) == 0) {
          if (((&DAT_00432271)[uVar4] & 0x80) == 0) {
            if ((DAT_00483494 & (&DAT_00432271)[uVar4]) != 0) {
              DAT_00482348 = (uint)(char)(&DAT_00432274)[uVar4];
            }
LAB_00415c54:
            cVar3 = cVar3 + '\x02';
          }
        }
        else {
          DAT_00482348 = (uint)(char)(&DAT_00432273)[uVar4];
          if (((&DAT_00432271)[uVar4] & 0x80) == 0) {
            if ((DAT_00483494 & (&DAT_00432271)[uVar4]) != 0) {
              DAT_00482348 = (uint)(char)(&DAT_00432275)[uVar4];
            }
            goto LAB_00415c54;
          }
        }
        cVar3 = cVar3 + '\x02';
      }
      else {
        DAT_00482348 = (uint)(char)(&DAT_00432271)[uVar4];
      }
      uVar4 = (uint)(byte)(cVar3 + 2);
      uVar2 = DAT_00482348;
      if ((0xf < (byte)DAT_00482348) && (param_3 != '\0')) {
        uVar2 = (uint)(byte)((byte)DAT_00482348 + 0xc);
      }
      FUN_0040daf0(((ushort)uVar2 & 0xff) + 0x2500);
      iVar1 = DAT_00482338;
      local_2 = local_2 + -1;
    } while (local_2 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    local_1 = local_1 + -1;
    if (local_1 == '\0') {
      FUN_0040da00((short)iVar1 + 0x42);
      FUN_0040daf0(0x250f);
      return;
    }
  } while( true );
}



/* ================= FUN_00415cd0 @ 00415cd0 ================= */

void __cdecl FUN_00415cd0(byte param_1,byte param_2,byte param_3)

{
  byte bVar1;
  byte *pbVar2;
  
  DAT_0048233c = (uint)param_2 << 6 | (uint)param_1;
  pbVar2 = (&PTR_s___________004322f8)[param_3];
  do {
    FUN_0040da00((short)DAT_0048233c);
    bVar1 = *pbVar2;
    while( true ) {
      if (bVar1 == 0) {
        return;
      }
      pbVar2 = pbVar2 + 1;
      if (bVar1 == 0xd) break;
      FUN_0040daf0(bVar1 + 0x2480);
      bVar1 = *pbVar2;
    }
    DAT_0048233c = DAT_0048233c + 0x40;
  } while( true );
}



/* ================= FUN_00415d40 @ 00415d40 ================= */

void __cdecl FUN_00415d40(byte param_1,byte param_2,byte param_3,char param_4)

{
  char cVar1;
  char cVar2;
  
  DAT_00482338 = (uint)param_2 * 0x40 + (uint)param_1;
  FUN_0040da00((short)DAT_00482338);
  FUN_0040daf0(0x34c9);
  cVar1 = param_3 - 2;
  cVar2 = cVar1;
  do {
    FUN_0040daf0(0x34ca);
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  FUN_0040daf0(0x34cb);
  param_2 = param_4 - 2;
  DAT_00482338 = DAT_00482338 + 0x40;
  do {
    FUN_0040da00((short)DAT_00482338);
    FUN_0040daf0(0x34cf);
    FUN_0040da00((short)DAT_00482338 + (ushort)param_3 + -1);
    FUN_0040daf0(0x34d0);
    DAT_00482338 = DAT_00482338 + 0x40;
    param_2 = param_2 - 1;
  } while (param_2 != 0);
  FUN_0040da00((short)DAT_00482338);
  FUN_0040daf0(0x34cc);
  do {
    FUN_0040daf0(0x34cd);
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  FUN_0040daf0(0x34ce);
  return;
}



/* ================= FUN_00416320 @ 00416320 ================= */

void FUN_00416320(void)

{
  FUN_00416330(0);
  return;
}



/* ================= FUN_00416330 @ 00416330 ================= */

void __cdecl FUN_00416330(undefined1 param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  DAT_0048327f = param_1;
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 2;
    *(undefined2 *)((int)&DAT_00483290 + iVar2) = *(undefined2 *)((int)&DAT_004834d0 + iVar2);
    *(undefined2 *)((int)&DAT_00483390 + iVar2) = *(undefined2 *)((int)&DAT_004835e0 + iVar2);
    iVar2 = iVar3;
  } while (iVar3 < 0x100);
  bVar1 = FUN_00404940(&LAB_00416390,0x14);
  if (bVar1 != 0xff) {
    DAT_00483491 = 1;
    DAT_00483490 = 0;
    DAT_00483492 = 0;
  }
  return;
}



/* ================= FUN_004163e0 @ 004163e0 ================= */

void FUN_004163e0(void)

{
  FUN_004163f0(0);
  return;
}



/* ================= FUN_004163f0 @ 004163f0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_004163f0(undefined1 param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  DAT_0048327f = param_1;
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 2;
    *(undefined2 *)((int)&DAT_00483290 + iVar2) = *(undefined2 *)((int)&DAT_004834d0 + iVar2);
    *(undefined2 *)((int)&DAT_00483390 + iVar2) = *(undefined2 *)((int)&DAT_004835e0 + iVar2);
    iVar2 = iVar3;
  } while (iVar3 < 0x100);
  bVar1 = FUN_00404940(&LAB_00416460,0x14);
  if (bVar1 != 0xff) {
    DAT_00483490 = 7;
    DAT_00483492 = 0;
    _DAT_00483280 = 6;
    DAT_004834b4 = 1;
    DAT_00483491 = 1;
  }
  return;
}



/* ================= FUN_004164b0 @ 004164b0 ================= */

void FUN_004164b0(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  ushort uVar4;
  uint uVar5;
  int iVar6;
  
  uVar5 = 0;
  if (DAT_00483490 == 7) {
    iVar6 = 0;
    do {
      iVar3 = iVar6 + 2;
      *(undefined2 *)((int)&DAT_004834d0 + iVar6) = *(undefined2 *)((int)&DAT_00483290 + iVar6);
      *(undefined2 *)((int)&DAT_004835e0 + iVar6) = *(undefined2 *)((int)&DAT_00483390 + iVar6);
      iVar6 = iVar3;
    } while (iVar3 != 0x100);
  }
  else {
    iVar6 = 0;
    uVar2 = (uint)DAT_00483490;
    uVar1 = uVar2 * 2;
    do {
      uVar4 = *(ushort *)((int)&DAT_00483290 + iVar6);
      uVar4 = (ushort)(*(ushort *)(&DAT_00432410 + ((uVar4 & 0x1c) * 2 | uVar2) * 2) |
                       *(ushort *)(&DAT_00432390 + ((uVar4 & 0x380) >> 3 | uVar1)) |
                       *(ushort *)(&DAT_00432310 + ((uVar4 & 0x7000) >> 8 | uVar1)) | 2) >> 1;
      if (iVar6 < 0x100) {
        *(ushort *)((int)&DAT_004834d0 + iVar6) = uVar4;
      }
      else {
        if ((uVar5 & 0xf) == 0) {
          uVar4 = 0;
        }
        *(ushort *)((int)&DAT_004834e0 + iVar6) = uVar4;
      }
      iVar6 = iVar6 + 2;
      uVar5 = uVar5 + 1;
    } while (iVar6 != 0x200);
  }
  DAT_004834aa = 1;
  DAT_004834ae = 0;
  return;
}



/* ================= FUN_00416860 @ 00416860 ================= */

void __cdecl
FUN_00416860(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  undefined4 *puVar1;
  undefined4 unaff_retaddr;
  char *pcVar2;
  undefined4 in_stack_fffffff4;
  undefined4 uVar3;
  undefined4 in_stack_fffffffc;
  undefined4 uVar4;
  
  uVar4 = CONCAT31((int3)((uint)in_stack_fffffffc >> 8),DAT_004324c8);
  uVar3 = CONCAT31((int3)((uint)in_stack_fffffff4 >> 8),DAT_004324c0);
  if (DAT_0042fb74 == '\0') {
    FUN_0040c850(0x25,0xe,&DAT_004324b8,(uint)DAT_00495c6a * 0x10 + (uint)DAT_00495c52 + 1,
                 DAT_004324bc,uVar3,DAT_004324c4,uVar4,unaff_retaddr,param_1,param_2,param_3,param_4
                );
    pcVar2 = s_REVOLT_0042a9c8 + ((uint)DAT_00495c6a * 0x10 + (uint)DAT_00495c52) * 8;
  }
  else {
    FUN_0040c850(0x25,0xe,&DAT_004324b4,DAT_004324bc,uVar3,DAT_004324c4,uVar4,unaff_retaddr,param_1,
                 param_2,param_3,param_4,param_5);
    pcVar2 = &DAT_004814e0;
  }
  FUN_0040c820(0x22,0x10,pcVar2);
  FUN_0040c850(0x26,0x13,&DAT_004324b0,DAT_00495c51 + 1,uVar4,unaff_retaddr,param_1,param_2,param_3,
               param_4,param_5,param_6,param_7);
  FUN_0040c850(0x25,0x17,&DAT_004324b8,(uint)DAT_00495aa1,uVar4,unaff_retaddr,param_1,param_2,
               param_3,param_4,param_5,param_6,param_7);
  FUN_0040c850(0x25,0x1a,&DAT_004324b8,(uint)DAT_00495aa2,uVar4,unaff_retaddr,param_1,param_2,
               param_3,param_4,param_5,param_6,param_7);
  puVar1 = (undefined4 *)((int)register0x00000010 + -4);
  if (DAT_00495c63 == '\0') {
    DAT_00481da4 = 0xf0;
  }
  if (DAT_004834a8 == '\0') {
    puVar1 = &param_1;
  }
  FUN_0040c850(0x22,0x16,&DAT_004324a8,puVar1,uVar4,unaff_retaddr,param_1,param_2,param_3,param_4,
               param_5,param_6,param_7);
  DAT_00481da4 = 0x70;
  puVar1 = (undefined4 *)((int)register0x00000010 + -4);
  if (DAT_00495c63 != '\0') {
    DAT_00481da4 = 0xf0;
  }
  if (DAT_004834a9 == '\0') {
    puVar1 = &param_1;
  }
  FUN_0040c850(0x22,0x19,&DAT_004324a0,puVar1,uVar4,unaff_retaddr,param_1,param_2,param_3,param_4,
               param_5,param_6,param_7);
  DAT_00481da4 = 0x70;
  return;
}



/* ================= FUN_004169d0 @ 004169d0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_004169d0(byte param_1,char param_2,undefined4 *param_3,ushort param_4)

{
  int iVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined4 *puVar4;
  byte bVar5;
  undefined1 local_200 [128];
  undefined1 local_180 [128];
  undefined1 local_100 [128];
  undefined1 local_80 [128];
  
  DAT_0048326c = param_1;
  DAT_00483264 = param_2;
  DAT_00483260 = param_2 << 5;
  _DAT_00483262 = param_4;
  DAT_00483268 = param_3;
  if (param_1 != 4) {
    DAT_00483268 = (undefined4 *)local_200;
    FUN_0040d590((int)local_200,param_3);
    FUN_0040d590((int)local_180,param_3 + 0x20);
    FUN_0040d590((int)local_100,param_3 + 0x40);
    FUN_0040d590((int)local_80,param_3 + 0x60);
    if (4 < DAT_0048326c) {
      DAT_0048326c = DAT_0048326c - 1;
    }
    FUN_0040da00(param_4);
    FUN_00416bf0((int)(&PTR_DAT_00432878)[(uint)DAT_0048326c * 4],
                 (int)(&PTR_DAT_0043287c)[(uint)DAT_0048326c * 4]);
    FUN_00416d20((int)(&PTR_DAT_00432880)[(uint)DAT_0048326c * 4],
                 (int)(&PTR_DAT_0043287c)[(uint)DAT_0048326c * 4]);
    FUN_00416bf0((int)(&PTR_DAT_00432878)[(uint)DAT_0048326c * 4],
                 (int)(&PTR_DAT_00432884)[(uint)DAT_0048326c * 4]);
    iVar1 = (uint)param_4 * 2;
    FUN_00416d20((int)(&PTR_DAT_00432880)[(uint)DAT_0048326c * 4],
                 (int)(&PTR_DAT_00432884)[(uint)DAT_0048326c * 4]);
    FUN_0040d3d0(&DAT_00484730 + iVar1,(undefined4 *)(&DAT_00484730 + iVar1));
    FUN_0040d3d0(&DAT_004847b0 + iVar1,(undefined4 *)(&DAT_004847b0 + iVar1));
    FUN_0040d3d0(&DAT_00484830 + iVar1,(undefined4 *)(&DAT_00484830 + iVar1));
    FUN_0040d3d0(&DAT_004848b0 + iVar1,(undefined4 *)(&DAT_004848b0 + iVar1));
    FUN_00412750((uint)param_4,4);
    return;
  }
  FUN_0040da00(param_4);
  DAT_00482348 = 4;
  do {
    bVar5 = DAT_00483260 >> 1;
    do {
      puVar4 = DAT_00483268;
      uVar2 = *(undefined1 *)DAT_00483268;
      DAT_00483268 = (undefined4 *)((int)DAT_00483268 + 1);
      uVar3 = *(undefined1 *)DAT_00483268;
      DAT_00483268 = (undefined4 *)((int)puVar4 + 2);
      FUN_0040da20(CONCAT11(uVar3,uVar2));
      bVar5 = bVar5 - 1;
    } while (bVar5 != 0);
    DAT_00482348 = DAT_00482348 + -1;
  } while (DAT_00482348 != 0);
  FUN_00412750((uint)param_4,4);
  return;
}



/* ================= FUN_00416bf0 @ 00416bf0 ================= */

void __cdecl FUN_00416bf0(int param_1,int param_2)

{
  byte bVar1;
  char cVar2;
  ushort uVar3;
  byte bVar4;
  ushort uVar5;
  ushort uVar6;
  byte local_4;
  byte local_3;
  byte local_2;
  char local_1;
  
  DAT_00483261 = DAT_00483264;
  local_4 = 0;
  do {
    local_1 = '\x10';
    local_2 = 0;
    local_3 = local_4;
    do {
      bVar4 = *(byte *)((uint)local_2 + param_2);
      cVar2 = local_1;
      if (bVar4 == 0) {
LAB_00416ce3:
        local_1 = cVar2;
        local_3 = local_3 + 2;
        local_2 = local_2 + 1;
      }
      else {
        if (bVar4 == 1) {
          local_2 = local_2 + 1;
        }
        else {
          if (bVar4 != 2) {
            uVar6 = 0;
            uVar3 = *(ushort *)((uint)local_3 + DAT_00483268);
            bVar4 = 0xff;
            while( true ) {
              do {
                bVar4 = bVar4 + 1;
                uVar5 = uVar3 << 0xf;
                uVar3 = uVar3 >> 1;
                bVar1 = *(byte *)((uint)bVar4 + param_1);
              } while (bVar1 == 0);
              if ((bVar1 & 0x80) != 0) break;
              uVar6 = uVar6 >> 1 | uVar5;
              if (bVar1 != 1) {
                uVar6 = uVar6 >> 1 | uVar5;
              }
            }
            for (bVar4 = *(byte *)((uint)bVar4 + param_1) & 0x7f; bVar4 != 0; bVar4 = bVar4 - 1) {
              uVar6 = uVar6 >> 1;
            }
            FUN_0040da20(uVar6);
            cVar2 = local_1 + -1;
            if ((*(byte *)((uint)local_2 + param_2) & 0x80) != 0) {
              FUN_0040da20(uVar6);
              cVar2 = local_1 + -2;
            }
            goto LAB_00416ce3;
          }
          local_3 = local_3 + 2;
        }
        FUN_0040da20(0);
        local_1 = local_1 + -1;
      }
    } while (local_1 != '\0');
    local_4 = local_4 + 0x20;
    DAT_00483261 = DAT_00483261 + -1;
    if (DAT_00483261 == '\0') {
      FUN_00416e50();
      return;
    }
  } while( true );
}



/* ================= FUN_00416d20 @ 00416d20 ================= */

void __cdecl FUN_00416d20(int param_1,int param_2)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  char cVar4;
  byte bVar5;
  ushort uVar6;
  ushort uVar7;
  byte local_4;
  byte local_3;
  byte local_2;
  char local_1;
  
  DAT_00483261 = DAT_00483264;
  local_4 = 0;
  do {
    local_1 = '\x10';
    local_2 = 0;
    local_3 = local_4;
    do {
      bVar5 = *(byte *)((uint)local_2 + param_2);
      cVar4 = local_1;
      if (bVar5 == 0) {
LAB_00416e14:
        local_1 = cVar4;
        local_3 = local_3 + 2;
        local_2 = local_2 + 1;
      }
      else {
        if (bVar5 == 1) {
          local_2 = local_2 + 1;
        }
        else {
          if (bVar5 != 2) {
            uVar6 = 0;
            bVar5 = 0xff;
            uVar7 = *(ushort *)((uint)local_3 + DAT_00483268);
            while( true ) {
              do {
                bVar5 = bVar5 + 1;
                uVar2 = uVar7 * 2;
                uVar3 = uVar7 >> 0xf;
                bVar1 = *(byte *)((uint)bVar5 + param_1);
                uVar7 = uVar2;
              } while (bVar1 == 0);
              if ((bVar1 & 0x80) != 0) break;
              uVar6 = uVar6 * 2 | uVar3;
              if (bVar1 != 1) {
                uVar6 = uVar6 * 2 | uVar3;
              }
            }
            for (bVar5 = *(byte *)((uint)bVar5 + param_1) & 0x7f; bVar5 != 0; bVar5 = bVar5 - 1) {
              uVar6 = uVar6 * 2;
            }
            FUN_0040da20(uVar6);
            cVar4 = local_1 + -1;
            if ((*(byte *)((uint)local_2 + param_2) & 0x80) != 0) {
              FUN_0040da20(uVar6);
              cVar4 = local_1 + -2;
            }
            goto LAB_00416e14;
          }
          local_3 = local_3 + 2;
        }
        FUN_0040da20(0);
        local_1 = local_1 + -1;
      }
    } while (local_1 != '\0');
    local_4 = local_4 + 0x20;
    DAT_00483261 = DAT_00483261 + -1;
    if (DAT_00483261 == '\0') {
      FUN_00416e50();
      return;
    }
  } while( true );
}



/* ================= FUN_00416e50 @ 00416e50 ================= */

void FUN_00416e50(void)

{
  char cVar1;
  
  for (cVar1 = ('\x04' - DAT_00483264) * '\x10'; cVar1 != '\0'; cVar1 = cVar1 + -1) {
    FUN_0040da20(0);
  }
  DAT_00483268 = DAT_00483268 + (uint)DAT_00483260;
  return;
}



/* ================= FUN_00416ed0 @ 00416ed0 ================= */

void FUN_00416ed0(void)

{
  byte bVar1;
  
  if (DAT_00482359 == DAT_00482358) {
    FUN_004171e0();
    DAT_0048203c = DAT_0048235a;
    DAT_00482032 = DAT_0048235c;
    do {
      FUN_004170c0();
      DAT_0048235a = DAT_0048235a + 1;
    } while (DAT_0048235a != DAT_0048235c);
  }
  else {
    if (DAT_0048235a != DAT_0048235c) {
      if (DAT_0048235c < DAT_0048235a) {
        bVar1 = DAT_0048235a - DAT_0048235c;
      }
      else {
        bVar1 = DAT_0048235c - DAT_0048235a;
      }
      if (DAT_00482359 < DAT_00482358) {
        DAT_00495e6f = DAT_00482358 - DAT_00482359;
      }
      else {
        DAT_00495e6f = DAT_00482359 - DAT_00482358;
      }
      DAT_00495e6c = '\0';
      if ((DAT_0048235a != DAT_0048235c) && (DAT_00495e6c = '\x01', DAT_0048235c < DAT_0048235a)) {
        DAT_00495e6c = -1;
      }
      DAT_00495e6d = '\0';
      if ((DAT_00482359 != DAT_00482358) && (DAT_00495e6d = '\x01', DAT_00482359 < DAT_00482358)) {
        DAT_00495e6d = -1;
      }
      DAT_00495e70 = bVar1 <= DAT_00495e6f;
      DAT_00495e6e = bVar1;
      if ((bool)DAT_00495e70) {
        DAT_00495e6e = DAT_00495e6f;
        DAT_00495e6f = bVar1;
      }
      DAT_00482034 = (ushort)DAT_00495e6f * 2 - (ushort)DAT_00495e6e;
      DAT_00495e71 = 0;
      if (DAT_00495e6e != 0) {
        do {
          DAT_00495e71 = DAT_00495e71 + 1;
          FUN_004170c0();
          for (; (DAT_00482034 != 0 && (DAT_00482034 < 0x8000));
              DAT_00482034 = DAT_00482034 + (ushort)DAT_00495e6e * -2) {
            if (DAT_00495e70 == '\0') {
              DAT_00482358 = DAT_00482358 + DAT_00495e6d;
            }
            else {
              DAT_0048235a = DAT_0048235a + DAT_00495e6c;
            }
          }
          if (DAT_00495e70 == '\0') {
            DAT_0048235a = DAT_0048235a + DAT_00495e6c;
          }
          else {
            DAT_00482358 = DAT_00482358 + DAT_00495e6d;
          }
          DAT_00482034 = DAT_00482034 + (ushort)DAT_00495e6f * 2;
        } while (DAT_00495e6e != DAT_00495e71);
      }
      FUN_004170c0();
      return;
    }
    FUN_00417200();
    do {
      FUN_004170c0();
      DAT_00482358 = DAT_00482358 + 1;
    } while (DAT_00482359 != DAT_00482358);
  }
  return;
}



/* ================= FUN_004170c0 @ 004170c0 ================= */

void FUN_004170c0(void)

{
  FUN_00417170();
  FUN_004170d0();
  return;
}



/* ================= FUN_004170d0 @ 004170d0 ================= */

void FUN_004170d0(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  uint uVar3;
  
  FUN_0040da10((short)DAT_00482378);
  uVar1 = FUN_0040daa0();
  uVar2 = FUN_0040daa0();
  uVar3 = CONCAT22(uVar1,uVar2) & *(uint *)(&DAT_00436340 + (uint)DAT_00495e69 * 4) |
          (uint)(DAT_0048235e & 0xf) << ((&DAT_00436360)[(uint)DAT_00495e69 * 4] & 0x1f);
  FUN_0040da00((short)DAT_00482378);
  FUN_0040da50((short)(uVar3 >> 0x10));
  FUN_0040da50((short)uVar3);
  FUN_00412680(DAT_00482378 & 0xfff0,1);
  return;
}



/* ================= FUN_00417170 @ 00417170 ================= */

void FUN_00417170(void)

{
  uint local_4;
  
  local_4 = (uint)(DAT_0048235a & 0xf8);
  DAT_00495e69 = DAT_0048235a & 7;
  DAT_00482378 = (uint)DAT_00482036 +
                 ((DAT_00482358 & 0xf8) * 0x20 + local_4 + (uint)(DAT_00482358 & 7)) * 2;
  return;
}



/* ================= FUN_004171d0 @ 004171d0 ================= */

void FUN_004171d0(void)

{
  DAT_00482036 = 0x2000;
  return;
}



/* ================= FUN_004171e0 @ 004171e0 ================= */

void FUN_004171e0(void)

{
  byte bVar1;
  
  bVar1 = DAT_0048235a;
  if (DAT_0048235c <= DAT_0048235a) {
    DAT_0048235a = DAT_0048235c;
    DAT_0048235c = bVar1;
  }
  return;
}



/* ================= FUN_00417200 @ 00417200 ================= */

void FUN_00417200(void)

{
  byte bVar1;
  
  bVar1 = DAT_00482358;
  if (DAT_00482359 <= DAT_00482358) {
    DAT_00482358 = DAT_00482359;
    DAT_00482359 = bVar1;
  }
  return;
}



/* ================= FUN_00417220 @ 00417220 ================= */

void FUN_00417220(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_00417240,0x32);
  return;
}



/* ================= FUN_00417420 @ 00417420 ================= */

void FUN_00417420(void)

{
  uint uVar1;
  
  uVar1 = FUN_0041bfb0((-((*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x80) == 0) & 0xfeU)
                       + 10);
  *(short *)(DAT_00482330 + 0x10) = (short)uVar1;
  FUN_00412dc0(100);
  *(int *)(DAT_00482330 + 0x30) = DAT_00482c20;
  *(undefined1 *)(DAT_00482c20 + 9) = 0xe9;
  *(undefined1 *)(DAT_00482c20 + 10) = 1;
  *(ushort *)(DAT_00482c20 + 0xc) = (-(ushort)(DAT_0048327c == '\x11') & 0xfffc) + 0x40;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x88;
  *(undefined1 *)(DAT_00482c20 + 0x14) = *(undefined1 *)(DAT_00482330 + 0x10);
  FUN_00412dc0(100);
  *(int *)(DAT_00482330 + 0x34) = DAT_00482c20;
  *(char *)(DAT_00482c20 + 9) =
       ((DAT_0048327c != '\x11' && DAT_0048327c != '\0') && DAT_0048327c != '\t') + -0x17;
  *(undefined1 *)(DAT_00482c20 + 10) = 1;
  *(ushort *)(DAT_00482c20 + 0xc) = (-(ushort)(DAT_0048327c == '\x11') & 0xfffc) + 0x68;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x88;
  *(undefined1 *)(DAT_00482c20 + 0x14) = *(undefined1 *)(DAT_00482330 + 0x10);
  return;
}



/* ================= FUN_00417550 @ 00417550 ================= */

void FUN_00417550(void)

{
  char cVar1;
  char local_1;
  
  DAT_00482370 = 0x7081;
  local_1 = '\x15';
  do {
    cVar1 = '\x1f';
    FUN_0040da00((short)DAT_00482370);
    do {
      FUN_0040daf0(0x90);
      cVar1 = cVar1 + -1;
    } while (cVar1 != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_004175a0 @ 004175a0 ================= */

void __cdecl
FUN_004175a0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  char cVar1;
  undefined3 extraout_var;
  char cVar2;
  byte bVar3;
  undefined4 unaff_EBX;
  undefined4 unaff_ESI;
  undefined4 unaff_retaddr;
  undefined4 in_stack_fffffffc;
  uint uVar4;
  uint uVar5;
  
  DAT_00481f94 = &DAT_004363c0;
  FUN_00418bb0();
  DAT_00482370 = 0x7081;
  uVar4 = CONCAT13(0x15,(int3)in_stack_fffffffc);
  do {
    cVar2 = '\x1f';
    FUN_0040da00((short)DAT_00482370);
    uVar5 = uVar4;
    do {
      cVar1 = FUN_00418bf0();
      FUN_0040daf0((short)CONCAT31(extraout_var,cVar1) + 0x500);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    cVar2 = (char)(uVar5 >> 0x18) + -1;
    uVar4 = CONCAT13(cVar2,(int3)uVar5);
  } while (cVar2 != '\0');
  uVar5 = uVar5 & 0xffffff;
  do {
    uVar4 = uVar5 >> 0x18;
    FUN_0040da00(*(undefined2 *)(&DAT_004363a8 + uVar4 * 2));
    FUN_0040daf0((byte)(&DAT_004363b8)[uVar4] + 0x6400);
    cVar2 = (char)(uVar5 >> 0x18) + '\x01';
    uVar5 = CONCAT13(cVar2,(int3)uVar5);
  } while (cVar2 != '\x06');
  DAT_00481dac = 0x142;
  DAT_00481da8 = s_M_77_0042b2a0 +
                 (uint)(*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f) * 8;
  FUN_00421620();
  bVar3 = 0;
  DAT_00481dac = 0x148;
  DAT_00481da8 = &DAT_0042b358 +
                 (uint)(*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f) * 8;
  FUN_00421620();
  do {
    if (((bVar3 != 4) || (PTR_DAT_00436398[DAT_0048327c] != '\0')) &&
       ((bVar3 != 5 || (PTR_DAT_0043639c[DAT_0048327c] != '\0')))) {
      FUN_0040c850((&DAT_00436380)[bVar3],(&DAT_00436388)[bVar3],&DAT_004324b0,
                   (uint)(byte)(&PTR_DAT_00436390)[bVar3][DAT_0048327c],unaff_ESI,unaff_EBX,uVar5,
                   unaff_retaddr,param_1,param_2,param_3,param_4,param_5);
    }
    bVar3 = bVar3 + 1;
  } while (bVar3 != 6);
  DAT_00495b74 = 0x3000;
  FUN_004068b0();
  FUN_00406830(DAT_0048327c * '\x02' + 0x20,3,0xe);
  if (DAT_0048327c != 0x15) {
    FUN_00406830(DAT_0048327c * '\x02' + 0x21,3,0x14);
  }
  return;
}



/* ================= FUN_00417760 @ 00417760 ================= */

void FUN_00417760(void)

{
  return;
}



/* ================= FUN_00417770 @ 00417770 ================= */

int __cdecl FUN_00417770(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  
  uVar2 = param_1 >> 1;
  iVar1 = 0;
  iVar5 = (int)((param_2 - (uVar2 & 1)) + 2) >> 1;
  iVar6 = iVar5 + -1;
  if ((-1 < iVar6) &&
     (bVar4 = *(byte *)((int)&DAT_00481a80 + uVar2 + iVar6 * 0x20) & 0xf, 10 < bVar4)) {
    iVar1 = bVar4 - 10;
  }
  uVar2 = uVar2 - 1;
  if (-1 < (int)uVar2) {
    iVar3 = iVar5 + -2;
    if ((uVar2 & 1) == 0) {
      iVar3 = iVar6;
    }
    if ((-1 < iVar3) && (bVar4 = *(byte *)(uVar2 + iVar3 * 0x20 + 0x481a80) & 0xf, 10 < bVar4)) {
      iVar1 = bVar4 - 10;
    }
    if ((uVar2 & 1) == 0) {
      iVar6 = iVar5;
    }
    if ((-1 < iVar6) && (bVar4 = *(byte *)(uVar2 + iVar6 * 0x20 + 0x481a80) & 0xf, 10 < bVar4)) {
      iVar1 = bVar4 - 10;
    }
  }
  return iVar1;
}



/* ================= FUN_00417800 @ 00417800 ================= */

void FUN_00417800(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    iVar1 = FUN_00417770((uint)DAT_00495c44 + iVar2,((uint)DAT_00495c45 - (uint)DAT_00481e63) + 0x15
                        );
    *(char *)((int)&DAT_00482050 + iVar2) = (char)iVar1;
    iVar2 = iVar3;
  } while (iVar3 < 0x1f);
  return;
}



/* ================= FUN_00417840 @ 00417840 ================= */

void FUN_00417840(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    iVar1 = FUN_00417770((uint)DAT_00495c44 + iVar2,(uint)DAT_00495c45);
    *(char *)((int)&DAT_00482050 + iVar2) = (char)iVar1;
    iVar2 = iVar3;
  } while (iVar3 < 0x1f);
  return;
}



/* ================= FUN_00417870 @ 00417870 ================= */

void FUN_00417870(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    iVar1 = FUN_00417770((uint)DAT_00495c44 + iVar2,DAT_00495c45 + 0x14);
    *(char *)((int)&DAT_00482050 + iVar2) = (char)iVar1;
    iVar2 = iVar3;
  } while (iVar3 < 0x1f);
  return;
}



/* ================= FUN_004178a0 @ 004178a0 ================= */

void FUN_004178a0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    iVar1 = FUN_00417770(DAT_00495c44 + 0x1e,(uint)DAT_00495c45 + iVar2);
    *(char *)((int)&DAT_00482050 + iVar2) = (char)iVar1;
    iVar2 = iVar3;
  } while (iVar3 < 0x15);
  return;
}



/* ================= FUN_004178d0 @ 004178d0 ================= */

void FUN_004178d0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    iVar1 = FUN_00417770((uint)DAT_00495c44,(uint)DAT_00495c45 + iVar2);
    *(char *)((int)&DAT_00482050 + iVar2) = (char)iVar1;
    iVar2 = iVar3;
  } while (iVar3 < 0x15);
  return;
}



/* ================= FUN_00417900 @ 00417900 ================= */

void FUN_00417900(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00481fa0;
  for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = &DAT_00481ff0;
  for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  DAT_00482028 = '\0';
  DAT_00481fde = 0;
  DAT_00482030 = 0;
  DAT_00481fd8 = 0;
  DAT_00481db5 = 0;
  FUN_00417b30();
  FUN_00417fc0();
  FUN_00418800();
  FUN_00417ea0();
  if (DAT_00482028 != '\0') {
    DAT_00481fde = 1;
  }
  return;
}



/* ================= FUN_00417970 @ 00417970 ================= */

void FUN_00417970(void)

{
  byte bVar1;
  
  if (DAT_00481db5 != '\0') {
    DAT_00481db5 = '\0';
    FUN_00417af0();
  }
  FUN_004247c0();
  DAT_00481a68 = 0xff;
  bVar1 = DAT_00482030;
  if (0x13 < DAT_00482030) {
    bVar1 = 0x14;
  }
  (*(code *)(&PTR_LAB_004364e8)[bVar1])();
  DAT_00481a68 = 1;
  return;
}



/* ================= FUN_004179b0 @ 004179b0 ================= */

void __cdecl FUN_004179b0(byte param_1)

{
  byte bVar1;
  
  bVar1 = param_1;
  if ((DAT_00481fda & 1) != 0) {
    bVar1 = param_1 + 6;
  }
  DAT_00481fda = DAT_00481fda + (&DAT_00436550)[param_1];
  DAT_00481fdb = DAT_00481fdb + (&DAT_00436540)[bVar1];
  return;
}



/* ================= FUN_004179f0 @ 004179f0 ================= */

void __cdecl FUN_004179f0(byte param_1,char param_2)

{
  byte bVar1;
  uint uVar2;
  byte bVar3;
  byte local_1;
  
  local_1 = 0;
  if (param_1 == DAT_0048327d) goto LAB_00417a97;
  if ((param_1 - DAT_0048327d & 0x80) == 0) {
    if ((DAT_0048327d & 1) != 0) goto joined_r0x00417a72;
    if ((param_2 - DAT_0048327e & 0x80U) == 0) goto LAB_00417a97;
  }
  else {
    if ((param_1 & 1) != 0) {
      if ((param_2 - DAT_0048327e & 0x80U) != 0) {
        local_1 = 1;
      }
      goto LAB_00417a97;
    }
joined_r0x00417a72:
    if ((param_2 - DAT_0048327e & 0x80U) != 0) goto LAB_00417a97;
  }
  local_1 = 1;
LAB_00417a97:
  bVar1 = param_1 - DAT_0048327d;
  if ((bVar1 & 0x80) != 0) {
    bVar1 = -bVar1;
  }
  bVar3 = param_2 - DAT_0048327e;
  DAT_00482348 = (uint)bVar1;
  uVar2 = (int)(local_1 + DAT_00482348) >> 1;
  if ((bVar3 & 0x80) != 0) {
    bVar3 = -bVar3;
  }
  if ((uVar2 & 0x80) == 0) {
    DAT_00482348 = DAT_00482348 + (byte)(bVar3 - (char)uVar2);
  }
  return;
}



/* ================= thunk_FUN_00417af0 @ 00417ae0 ================= */

void thunk_FUN_00417af0(void)

{
  DAT_00482030 = DAT_00482030 + '\x01';
  FUN_00417af0();
  return;
}



/* ================= FUN_00417af0 @ 00417af0 ================= */

void FUN_00417af0(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  
  bVar1 = DAT_00495c63;
  bVar2 = 0x37;
  do {
    uVar3 = (uint)bVar2;
    if (((&DAT_00495af0)[uVar3] & 0x80) == 0) {
      if ((*(byte *)((int)&DAT_004957a0 + uVar3) & 0x80) == bVar1) {
        *(byte *)((int)&DAT_00481fa0 + uVar3) = *(byte *)((int)&DAT_00481fa0 + uVar3) & 0x7f;
      }
      else {
        *(byte *)((int)&DAT_00481ff0 + uVar3) = *(byte *)((int)&DAT_00481ff0 + uVar3) & 0x7f;
      }
    }
    bVar2 = bVar2 - 1;
  } while ((bVar2 & 0x80) == 0);
  return;
}



/* ================= FUN_00417b30 @ 00417b30 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00417b30(void)

{
  undefined1 uVar1;
  byte bVar2;
  byte bVar3;
  char cVar4;
  byte bVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  byte bVar9;
  undefined4 *puVar10;
  byte local_3;
  byte local_2;
  byte local_1;
  
  puVar10 = &DAT_00482050;
  for (iVar6 = 0xe; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar10 = 0;
    puVar10 = puVar10 + 1;
  }
  local_1 = 0;
  local_2 = DAT_00495c52 << 4;
  DAT_0048235a = 0;
  do {
    uVar8 = (uint)local_2;
    bVar5 = (&DAT_0047f7d0)[uVar8];
    if (0x7f < bVar5) break;
    bVar9 = *(byte *)((int)&DAT_00481a80 + (uint)bVar5 + (uint)(byte)(&DAT_0047f7d1)[uVar8] * 0x20)
            & 0xf;
    if ((bVar9 == DAT_00495c43) || (bVar9 == 0xc)) {
      uVar7 = (uint)local_1;
      local_1 = local_1 + 1;
      (&DAT_00482060)[uVar7] = bVar9;
      *(byte *)((int)&DAT_00482050 + uVar7) = bVar5;
      bVar5 = DAT_0048235a;
      (&DAT_00482058)[uVar7] = (&DAT_0047f7d1)[uVar8];
      (&DAT_00482068)[uVar7] = bVar5;
    }
    local_2 = local_2 + 2;
    DAT_0048235a = DAT_0048235a + 1;
  } while (DAT_0048235a < 8);
  bVar9 = DAT_00495c47;
  bVar5 = DAT_00495c52 * '\x02';
  if (DAT_00495c63 == '\0') {
    bVar5 = bVar5 + 1;
  }
  uVar7 = (uint)bVar5 * 2 & 0xff;
  uVar8 = (uint)local_1;
  uVar1 = (&DAT_0047f791)[uVar7];
  *(undefined1 *)((int)&DAT_00482050 + uVar8) = (&DAT_0047f790)[uVar7];
  (&DAT_00482058)[uVar8] = uVar1;
  (&DAT_00482060)[uVar8] = bVar9;
  (&DAT_00482068)[uVar8] = 7;
  do {
    _DAT_00482070 = 0xffffffff;
    local_1 = 0;
    DAT_00482074 = 0xffffffff;
    do {
      iVar6 = FUN_00417ee0(local_1);
      if (((iVar6 == 0) && (uVar8 = (uint)local_1, *(char *)((int)&DAT_00481fa0 + uVar8) == '\0'))
         && (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + uVar8) & 0x1f] & 0x90) == 0x80)) {
        DAT_00483274 = local_1;
        DAT_0048327b = local_1;
        if (((&DAT_00495af0)[uVar8] & 0x40) != 0) {
          DAT_0048327b = (&DAT_00495af0)[uVar8] & 0x3f;
        }
        uVar8 = (uint)DAT_0048327b;
        DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar8) & 0x1f;
        DAT_0048327e = (&DAT_00495af0)[uVar8] & 0x1f;
        uVar7 = 0;
        DAT_00482088 = '\0';
        DAT_0048327c = *(byte *)((int)&DAT_004957a0 + uVar8) & 0x1f;
        do {
          if (((&DAT_00482060)[uVar7] != '\0') &&
             (bVar5 = FUN_004179f0(*(byte *)((int)&DAT_00482050 + uVar7),(&DAT_00482058)[uVar7]),
             bVar5 < 0x18)) {
            DAT_00482088 = DAT_00482088 + '\x01';
            break;
          }
          bVar5 = (char)uVar7 + 1;
          uVar7 = (uint)bVar5;
        } while (bVar5 < 8);
        if (DAT_00482088 != '\0') {
          local_3 = 0;
          DAT_00495c6e = 0;
          DAT_00495ef8 = 0;
          DAT_00495f30 = (&DAT_00436558)[DAT_0048327c];
          FUN_004015d0();
        }
        local_2 = 0;
        do {
          uVar8 = (uint)local_2;
          if ((&DAT_00482060)[uVar8] != '\0') {
            if ((DAT_00482088 == '\0') ||
               ((&DAT_00495c70)
                [(uint)*(byte *)((int)&DAT_00482050 + uVar8) +
                 (uint)(byte)(&DAT_00482058)[uVar8] * 0x20] == 0)) {
              cVar4 = FUN_004179f0(*(byte *)((int)&DAT_00482050 + uVar8),(&DAT_00482058)[uVar8]);
              bVar5 = cVar4 + 0x40;
            }
            else {
              bVar5 = (byte)(&DAT_00495c70)
                            [(uint)*(byte *)((int)&DAT_00482050 + uVar8) +
                             (uint)(byte)(&DAT_00482058)[uVar8] * 0x20] >> 4;
              DAT_00482348 = (uint)bVar5;
              cVar4 = DAT_00495f30;
              if (((&DAT_00495af0)[DAT_0048327b] & 0x20) != 0) {
                cVar4 = DAT_00495f30 + '\x01';
              }
              bVar5 = cVar4 - bVar5;
            }
            bVar9 = DAT_00483274;
            if ((bVar5 == (&DAT_00482070)[uVar8]) || (bVar5 < (byte)(&DAT_00482070)[uVar8])) {
              (&DAT_00482070)[uVar8] = bVar5;
              (&DAT_00482078)[uVar8] = DAT_0048327b;
              (&DAT_00482080)[uVar8] = bVar9;
            }
          }
          local_2 = local_2 + 1;
        } while (local_2 < 8);
      }
      local_1 = local_1 + 1;
    } while (local_1 < 0x38);
    bVar9 = 0;
    bVar5 = 0xff;
    do {
      bVar2 = (&DAT_00482060)[bVar9];
      if (((bVar2 != 0) && (bVar3 = (&DAT_00482070)[bVar9], bVar3 < 0x80)) &&
         ((bVar3 < bVar5 || ((bVar3 == bVar5 && (bVar3 = bVar2, bVar2 == DAT_00495c47)))))) {
        bVar5 = bVar3;
        local_3 = bVar9;
      }
      bVar9 = bVar9 + 1;
    } while (bVar9 != 8);
    if (bVar5 == 0xff) {
      return;
    }
    uVar8 = (uint)local_3;
    (&DAT_00482060)[uVar8] = 0;
    bVar9 = (&DAT_00482078)[uVar8];
    if (0xf < bVar5) {
      bVar5 = 0xf;
    }
    bVar2 = (&DAT_00482068)[uVar8];
    *(undefined1 *)((int)&DAT_00481fa0 + (uint)bVar9) = 1;
    *(byte *)((int)&DAT_00481ff0 + (uint)bVar9) = bVar2 | bVar5 << 4;
    if ((&DAT_00482080)[uVar8] != bVar9) {
      *(undefined1 *)((int)&DAT_00481fa0 + (uint)(byte)(&DAT_00482080)[uVar8]) = 8;
    }
  } while( true );
}



/* ================= FUN_00417ea0 @ 00417ea0 ================= */

void FUN_00417ea0(void)

{
  byte bVar1;
  uint uVar2;
  byte bVar3;
  
  bVar1 = DAT_00495c63;
  bVar3 = 0x37;
  do {
    uVar2 = (uint)bVar3;
    if (((*(byte *)((int)&DAT_004957a0 + uVar2) & 0x80) == bVar1) &&
       ((((&DAT_00495af0)[uVar2] & 0xc0) != 0 ||
        ((*(byte *)((int)&DAT_00495ab0 + uVar2) & 0xe0) != 0)))) {
      *(undefined1 *)((int)&DAT_00481fa0 + uVar2) = 8;
    }
    bVar3 = bVar3 - 1;
  } while (bVar3 < 0x80);
  return;
}



/* ================= FUN_00417ee0 @ 00417ee0 ================= */

undefined4 __cdecl FUN_00417ee0(byte param_1)

{
  uint uVar1;
  
  uVar1 = (uint)param_1;
  if (((((&DAT_00495af0)[uVar1] & 0x80) == 0) &&
      ((*(byte *)((int)&DAT_004957a0 + uVar1) & 0x80) == DAT_00495c63)) &&
     ((*(byte *)((int)&DAT_00495ab0 + uVar1) & 0xe0) == 0)) {
    return 0;
  }
  return 1;
}



/* ================= FUN_00417f20 @ 00417f20 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00417f20(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_0048327b;
  DAT_00483278 = *(byte *)((int)&DAT_004957a0 + uVar1);
  DAT_0048327c = DAT_00483278 & 0x1f;
  DAT_00483279 = *(byte *)((int)&DAT_00495ab0 + uVar1);
  DAT_00481fda = DAT_00483279 & 0x1f;
  DAT_0048327a = (&DAT_00495af0)[uVar1];
  DAT_00481fdb = DAT_0048327a & 0x1f;
  _DAT_0048202c = (int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20;
  DAT_00495f30 = (&DAT_00436558)[DAT_0048327c];
  DAT_00495ef8 = 0;
  DAT_00495c6e = 0;
  DAT_0048327d = DAT_00481fda;
  DAT_0048327e = DAT_00481fdb;
  FUN_004015d0();
  return;
}



/* ================= FUN_00417fb0 @ 00417fb0 ================= */

void FUN_00417fb0(void)

{
  DAT_00484710 = 3;
  return;
}



/* ================= FUN_00417fc0 @ 00417fc0 ================= */

void FUN_00417fc0(void)

{
  byte *pbVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  uint uVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  undefined1 *puVar9;
  byte local_c;
  
  DAT_0048235a = '\0';
  bVar8 = 0x37;
  do {
    iVar4 = FUN_00417ee0(bVar8);
    if ((iVar4 == 0) &&
       (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)bVar8) & 0x1f] & 0x40) != 0)) {
      DAT_0048235a = DAT_0048235a + '\x01';
    }
    bVar8 = bVar8 - 1;
  } while (0x7f < bVar8);
  bVar8 = 0x37;
LAB_00418005:
  DAT_00482390 = bVar8;
  uVar5 = (uint)DAT_00482390;
  bVar8 = (&DAT_00495af0)[uVar5];
  DAT_00482391 = DAT_00482390;
  if ((bVar8 < 0x80) &&
     (pbVar1 = (byte *)((int)&DAT_004957a0 + uVar5), (*pbVar1 & 0x80) != DAT_00495c63)) {
    bVar7 = *(byte *)((int)&DAT_00495ab0 + uVar5);
    if ((bVar7 & 0xe0) != 0) goto LAB_00418671;
    DAT_0048327d = bVar7 & 0x1f;
    DAT_0048327c = *pbVar1 & 0x1f;
    DAT_0048327e = bVar8 & 0x1f;
    DAT_00481f9a = 0;
    bVar7 = (&DAT_0042b288)[DAT_0048327c];
    if ((bVar7 & 0x90) != 0x80) {
      DAT_0048327b = DAT_00482390;
      if ((bVar8 & 0x40) != 0) goto LAB_00418659;
      if ((bVar7 & 0x20) == 0) {
        if ((bVar7 & 8) == 0) {
          if ((((bVar7 & 3) != 2) || (DAT_0048235a != '\0')) && (DAT_0048327c != 0)) {
            DAT_00495c6e = 0;
            DAT_00495ef8 = 0;
            DAT_00495f30 = '\x01';
            puVar9 = &DAT_00495c70;
            FUN_004015d0();
            FUN_00418740(DAT_0048327b);
            DAT_0048235c = 0;
            local_c = DAT_00495c03;
            do {
              DAT_00482358 = DAT_00495c02 - 1;
              do {
                if (puVar9[DAT_00482358] != '\0') {
                  uVar5 = 5;
                  DAT_00481fda = DAT_00482358;
                  DAT_00481fdb = DAT_0048235c;
                  do {
                    FUN_004179b0(*(byte *)(uVar5 + 0x436570));
                    if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
                      FUN_00418780(DAT_00481fda,DAT_00481fdb);
                    }
                    bVar8 = (char)uVar5 - 1;
                    uVar5 = (uint)bVar8;
                  } while (bVar8 < 0x80);
                }
                DAT_00482358 = DAT_00482358 - 1;
              } while (DAT_00482358 < 0x80);
              puVar9 = puVar9 + 0x20;
              DAT_0048235c = DAT_0048235c + 1;
              local_c = local_c - 1;
            } while (local_c != 0);
          }
        }
        else if (((bVar7 & 3) != 2) || (DAT_0048235a != '\0')) {
          FUN_00418740(DAT_00482390);
          bVar8 = (&DAT_0042b1e8)[*pbVar1 & 0x1f];
          if (bVar8 < 2) {
            bVar8 = (&DAT_0042b200)[*pbVar1 & 0x1f];
          }
          DAT_0048235e = bVar8 + 1;
          DAT_00481fda = *(byte *)((int)&DAT_00495ab0 + uVar5) & 0x1f;
          DAT_00481fdb = (&DAT_00495af0)[uVar5] & 0x1f;
          FUN_004179b0(4);
          DAT_0048235b = 2;
          do {
            FUN_004179b0(4);
            DAT_0048235d = 0;
            do {
              DAT_00482368 = DAT_0048235b;
              do {
                if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
                  FUN_00418780(DAT_00481fda,DAT_00481fdb);
                }
                FUN_004179b0(DAT_0048235d);
                DAT_00482368 = DAT_00482368 - 1;
              } while (DAT_00482368 != 0);
              DAT_0048235d = DAT_0048235d + 1;
            } while (DAT_0048235d < 6);
            DAT_0048235b = DAT_0048235b + 1;
          } while (DAT_0048235b < DAT_0048235e);
        }
        goto LAB_00418659;
      }
      cVar3 = DAT_00495c52 * '\x02';
      if (DAT_00495c63 != 0) {
        cVar3 = cVar3 + '\x01';
      }
      DAT_00481fda = (&DAT_0047f790)[(byte)(cVar3 * '\x02')];
      DAT_00481fdb = (&DAT_0047f791)[(byte)(cVar3 * '\x02')];
      FUN_004179f0(DAT_00481fda,DAT_00481fdb);
      if (DAT_00482348 <= (byte)((&DAT_0042b218)[DAT_0048327c] + 4)) {
        DAT_00495ef8 = 0;
        DAT_00495c6e = 0;
        DAT_00495f30 = '\x01';
        cVar3 = '<';
        FUN_004015d0();
        FUN_00419810();
        do {
          FUN_00419830();
          if ((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] != '\0') {
            cVar3 = '\x06';
            DAT_00481fda = DAT_0048327d;
            DAT_00481fdb = DAT_0048327e;
            FUN_00419810();
            goto LAB_004182c6;
          }
          cVar3 = cVar3 + -1;
        } while (cVar3 != '\0');
      }
      goto LAB_004183e3;
    }
    DAT_00495f30 = '\x03';
    if ((bVar8 & 0x40) != 0) {
      DAT_00482391 = bVar8 & 0x3f;
      DAT_00495f30 = '\x02';
    }
    DAT_00495c6e = 0;
    DAT_00495ef8 = 0;
    DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + (uint)DAT_00482391) & 0x1f;
    DAT_0048327e = (&DAT_00495af0)[DAT_00482391] & 0x1f;
    DAT_0048327b = DAT_00482391;
    FUN_004015d0();
    cVar3 = DAT_00495c52 * '\x02';
    if (DAT_00495c63 != 0) {
      cVar3 = cVar3 + '\x01';
    }
    bVar7 = cVar3 * '\x02';
    bVar8 = (&DAT_00495c70)
            [(uint)(byte)(&DAT_0047f790)[bVar7] + (uint)(byte)(&DAT_0047f791)[bVar7] * 0x20];
    if (bVar8 == 0) {
      bVar8 = FUN_004179f0((&DAT_0047f790)[bVar7],(&DAT_0047f791)[bVar7]);
      if (bVar8 >> 2 < 2) {
        DAT_00482028 = (bVar8 >> 2) + 1;
        FUN_00418710(0xff);
      }
      else {
        DAT_00482374 = 8;
        bVar8 = DAT_00495c52 << 4;
        do {
          if (0x7f < (byte)(&DAT_0047f7d0)[bVar8]) break;
          DAT_00482348 = (uint)(byte)(&DAT_0047f7d1)[bVar8] * 0x20 +
                         (uint)(byte)(&DAT_0047f7d0)[bVar8];
          if (((&DAT_00495c70)[DAT_00482348] != '\0') &&
             ((bVar7 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf, bVar7 == 0xc ||
              (DAT_00495c04 == bVar7)))) {
            FUN_00418710(100);
          }
          bVar8 = bVar8 + 2;
          DAT_00482374 = DAT_00482374 + -1;
        } while (DAT_00482374 != 0);
      }
    }
    else {
      FUN_00418710(0xff);
      DAT_00482028 = (DAT_00495f30 - (bVar8 >> 4)) + '\x01';
    }
    goto LAB_00418659;
  }
  goto LAB_00418671;
  while (cVar3 = cVar3 + -1, cVar3 != '\0') {
LAB_004182c6:
    FUN_00419830();
    bVar7 = DAT_0048327e;
    bVar8 = DAT_0048327d;
    if ((*(byte *)((int)&DAT_00481a80 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20) & 0xf) ==
        DAT_00495c43) {
      DAT_0048327d = DAT_00481fda;
      DAT_0048327e = DAT_00481fdb;
      FUN_00419de0();
      bVar6 = 0;
      bVar2 = DAT_00495c70;
      if (DAT_00495c70 < 0x80) goto LAB_0041833b;
      break;
    }
  }
  goto LAB_0041836b;
  while( true ) {
    bVar6 = bVar6 + 1;
    bVar2 = (&DAT_00495c70)[bVar6];
    if (0x7f < bVar2) break;
LAB_0041833b:
    DAT_0048327d = bVar8;
    DAT_0048327e = bVar7;
    if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)bVar2) & 0x1f] & 0x90) == 0x80)
    goto LAB_004183dc;
  }
LAB_0041836b:
  cVar3 = '$';
  DAT_00481fda = bVar8;
  DAT_00481fdb = bVar7;
  DAT_0048327d = bVar8;
  DAT_0048327e = bVar7;
  FUN_00419810();
  do {
    FUN_00419830();
    bVar8 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20);
    if (((bVar8 != 0xff) && ((bVar8 & 0x80) != DAT_00495c63)) &&
       (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)(bVar8 & 0x3f)) & 0x1f] & 0x90) ==
        0x80)) goto LAB_004183dc;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
LAB_004183e3:
  if (((&DAT_00495af0)[DAT_0048327b] & 0x20) != 0) {
    uVar5 = 0x37;
    do {
      bVar8 = (&DAT_00495af0)[uVar5];
      if (((bVar8 & 0x40) != 0) && ((bVar8 & 0x3f) == DAT_0048327b)) break;
      bVar7 = (char)uVar5 - 1;
      uVar5 = (uint)bVar7;
    } while (bVar7 < 0x80);
    if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (bVar8 & 0x3f))] & 0x90) == 0x80) {
      FUN_00418710(0x30);
    }
    else {
      FUN_00418710(0x20);
    }
  }
LAB_00418659:
  if (*(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482391) <= DAT_00481f9a) {
    *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00482391) = DAT_00481f9a;
  }
LAB_00418671:
  bVar8 = DAT_00482390 - 1;
  if (0x7f < (byte)(DAT_00482390 - 1)) {
    if (((DAT_00495c6a == '\0') && (DAT_00495c52 < 3)) && (DAT_0042fb74 == '\0')) {
      DAT_00482028 = '\0';
    }
    if (DAT_00482028 != '\0') {
      cVar3 = DAT_00495c52 * '\x02';
      if (DAT_00495c63 != 0) {
        cVar3 = cVar3 + '\x01';
      }
      bVar8 = *(byte *)((int)&DAT_00495820 +
                       (uint)(byte)(&DAT_0047f790)[(byte)(cVar3 * '\x02')] +
                       (uint)(byte)(&DAT_0047f791)[(byte)(cVar3 * '\x02')] * 0x20);
      if ((bVar8 != 0xff) && ((bVar8 & 0x80) != DAT_00495c63)) {
        *(undefined1 *)((int)&DAT_00481fa0 + (uint)(bVar8 & 0x3f)) = 0xff;
      }
    }
    return;
  }
  goto LAB_00418005;
LAB_004183dc:
  DAT_00482028 = '\x03';
  goto LAB_004183e3;
}



/* ================= FUN_00418710 @ 00418710 ================= */

void __cdecl FUN_00418710(byte param_1)

{
  if ((uint)param_1 + (uint)DAT_00481f9a < 0x100) {
    DAT_00481f9a = DAT_00481f9a + param_1;
    return;
  }
  DAT_00481f9a = 0xff;
  return;
}



/* ================= FUN_00418740 @ 00418740 ================= */

void __cdecl FUN_00418740(byte param_1)

{
  if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)param_1) & 0x1f] & 2) == 0) {
    DAT_00482359 = 0;
    return;
  }
  DAT_00482359 = (-(((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)param_1) & 0x1f] & 1) == 0
                   ) & 0x41U) - 1;
  return;
}



/* ================= FUN_00418780 @ 00418780 ================= */

void __cdecl FUN_00418780(byte param_1,byte param_2)

{
  byte bVar1;
  
  bVar1 = *(byte *)((int)&DAT_00495820 + (uint)param_1 + (uint)param_2 * 0x20);
  if ((bVar1 != 0xff) && ((bVar1 & 0x80) == DAT_00495c63)) {
    if ((DAT_00482359 == 0xff) ||
       (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)(bVar1 & 0x3f)) & 0x1f] & 0x40) ==
        DAT_00482359)) {
      FUN_00418710(1);
      if (*(char *)((int)&DAT_00481fa0 + (uint)(bVar1 & 0x3f)) == '\x01') {
        FUN_00418710(0x20);
      }
    }
  }
  return;
}



/* ================= FUN_00418800 @ 00418800 ================= */

void FUN_00418800(void)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  byte bStack_1;
  
  if (DAT_00482028 != '\0') {
    cVar1 = DAT_00495c52 * '\x02';
    if (DAT_00495c63 != '\0') {
      cVar1 = cVar1 + '\x01';
    }
    DAT_00482394 = 0xff;
    DAT_00495c6e = 0;
    DAT_00495ef8 = 0;
    DAT_00495f30 = 1;
    DAT_00482393 = 0xff;
    bStack_1 = 0x37;
    DAT_00482390 = (&DAT_0047f790)[(byte)(cVar1 * '\x02')];
    DAT_00482391 = (&DAT_0047f791)[(byte)(cVar1 * '\x02')];
    do {
      DAT_0048327b = bStack_1;
      iVar2 = FUN_00417ee0(bStack_1);
      if (iVar2 == 0) {
        uVar3 = (uint)bStack_1;
        if (((&DAT_00495af0)[uVar3] & 0x40) == 0) {
          DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar3) & 0x1f;
          DAT_0048327e = (&DAT_00495af0)[uVar3];
          FUN_004179f0(DAT_00482390,DAT_00482391);
          if ((DAT_00482348 ==
               (byte)(&DAT_0042b218)[*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f]) ||
             (DAT_00482348 <
              (byte)(&DAT_0042b218)[*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f])) {
            DAT_00482392 = (byte)((int)((uint)(byte)(&DAT_0042b1d0)
                                                    [*(byte *)((int)&DAT_004957a0 + uVar3) & 0x1f] *
                                       (uint)(*(byte *)((int)&DAT_004957e0 + uVar3) & 7)) >> 5);
            if ((DAT_00482393 == 0xff) || (DAT_00482393 < DAT_00482392)) {
              FUN_004015d0();
              if ((&DAT_00495c70)[(uint)DAT_00482390 + (uint)DAT_00482391 * 0x20] != '\0') {
                DAT_00482394 = DAT_0048327b;
                DAT_00482393 = DAT_00482392;
              }
            }
          }
        }
      }
      bStack_1 = DAT_0048327b - 1;
    } while (bStack_1 < 0x80);
    if (DAT_00482394 < 0x80) {
      DAT_00481fe0 = 1;
      *(undefined1 *)((int)&DAT_00481fa0 + (uint)DAT_00482394) = 2;
    }
  }
  return;
}



/* ================= FUN_004189a0 @ 004189a0 ================= */

void FUN_004189a0(void)

{
  undefined1 *puVar1;
  byte bVar2;
  bool bVar3;
  
  (&DAT_00495af0)[DAT_0048327b] = (&DAT_00495af0)[DAT_0048327b] | 0x80;
  FUN_00423a80();
  bVar2 = DAT_0048327d;
  puVar1 = (undefined1 *)((int)&DAT_00495820 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20);
  bVar3 = DAT_00495f04 != 0;
  DAT_00482396 = *puVar1;
  *puVar1 = 0xff;
  if (bVar3) {
    DAT_00481dbc = 0;
    DAT_00481fda = bVar2;
    DAT_00481fdb = DAT_0048327e;
    FUN_0041da80();
    if (9 < DAT_00481df5) {
      DAT_00482390 = 1;
      do {
        FUN_004179b0(4);
        DAT_00482393 = 0xb;
        DAT_00482392 = 0;
        do {
          DAT_00482391 = DAT_00482390;
          do {
            if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
              DAT_00482348 = (uint)DAT_00481fdb * 0x20 + (uint)DAT_00481fda;
              if (((&DAT_00495c70)[DAT_00482348] != '\0') &&
                 (*(char *)((int)&DAT_00495820 + DAT_00482348) == -1)) {
                bVar2 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf;
                if ((bVar2 < 0xb) || (0xd < bVar2)) {
                  FUN_0041da80();
                  if (DAT_00481df5 < DAT_00482393) {
                    DAT_00482393 = DAT_00481df5;
                    DAT_00482394 = DAT_00481fda;
                    DAT_00482395 = DAT_00481fdb;
                  }
                }
                else if ((DAT_00495c04 == bVar2) &&
                        ((*(byte *)((int)&DAT_004957e0 + (uint)DAT_0048327b) & 7) < 4)) {
                  DAT_00482394 = DAT_00481fda;
                  DAT_00482395 = DAT_00481fdb;
                  goto LAB_00418ac0;
                }
              }
            }
            FUN_004179b0(DAT_00482392);
            DAT_00482391 = DAT_00482391 - 1;
          } while (DAT_00482391 != 0);
          DAT_00482392 = DAT_00482392 + 1;
        } while (DAT_00482392 < 6);
        if (DAT_00482393 < 0xb) {
LAB_00418ac0:
          DAT_00484710 = DAT_00482394;
          DAT_00484711 = DAT_00482395;
          goto LAB_00418b74;
        }
        DAT_00482390 = DAT_00482390 + 1;
      } while ((DAT_00482390 < DAT_00495f04) || (DAT_00482390 == DAT_00495f04));
    }
  }
  DAT_00484710 = 0xff;
LAB_00418b74:
  *(undefined1 *)((int)&DAT_00495820 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20) =
       DAT_00482396;
  (&DAT_00495af0)[DAT_0048327b] = (&DAT_00495af0)[DAT_0048327b] & 0x7f;
  FUN_00423a80();
  return;
}



/* ================= FUN_00418bb0 @ 00418bb0 ================= */

void FUN_00418bb0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_00481e90;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0x20202020;
    puVar2 = puVar2 + 1;
  }
  DAT_00481f92 = 0;
  DAT_00481f91 = 0xef;
  DAT_00481e86 = *DAT_00481f94;
  DAT_00481f94 = DAT_00481f94 + 1;
  DAT_00481e85 = 8;
  return;
}



/* ================= FUN_00418bf0 @ 00418bf0 ================= */

char FUN_00418bf0(void)

{
  char cVar1;
  uint uVar2;
  
  if (DAT_00481f92 == '\0') {
    cVar1 = FUN_00418c80(1);
    if (cVar1 != '\0') {
      cVar1 = FUN_00418c80(8);
      goto LAB_00418c44;
    }
    DAT_00481f90 = FUN_00418c80(8);
    cVar1 = FUN_00418c80(4);
    DAT_00481f92 = cVar1 + '\x02';
  }
  uVar2 = (uint)DAT_00481f90;
  DAT_00481f90 = DAT_00481f90 + 1;
  DAT_00481f92 = DAT_00481f92 + -1;
  cVar1 = *(char *)((int)&DAT_00481e90 + uVar2);
LAB_00418c44:
  *(char *)((int)&DAT_00481e90 + (uint)DAT_00481f91) = cVar1;
  DAT_00481f91 = DAT_00481f91 + 1;
  return cVar1;
}



/* ================= FUN_00418c80 @ 00418c80 ================= */

char __cdecl FUN_00418c80(int param_1)

{
  char cVar1;
  
  cVar1 = '\0';
  do {
    cVar1 = cVar1 * '\x02';
    if ((DAT_00481e86 & 0x80) != 0) {
      cVar1 = cVar1 + '\x01';
    }
    DAT_00481e86 = DAT_00481e86 << 1;
    DAT_00481e85 = DAT_00481e85 + -1;
    if (DAT_00481e85 == '\0') {
      DAT_00481e86 = *DAT_00481f94;
      DAT_00481f94 = DAT_00481f94 + 1;
      DAT_00481e85 = '\b';
    }
    param_1 = param_1 + -1;
  } while (param_1 != 0);
  return cVar1;
}



/* ================= FUN_004194f0 @ 004194f0 ================= */

void FUN_004194f0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0x17;
  do {
    iVar2 = iVar1 + 1;
    FUN_0040c7e0(10,(byte)iVar1,'\f');
    iVar1 = iVar2;
  } while (iVar2 < 0x1d);
  return;
}



/* ================= FUN_00419510 @ 00419510 ================= */

void __cdecl FUN_00419510(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 unaff_retaddr;
  
  FUN_00419590(6,0x18,(int)DAT_00481e82,(uint)DAT_00481e84,unaff_retaddr,param_1,param_2,param_3);
  return;
}



/* ================= FUN_00419530 @ 00419530 ================= */

void __cdecl FUN_00419530(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 unaff_retaddr;
  
  FUN_00419590(6,0x1a,(int)DAT_00481e80,(uint)DAT_00481e84,unaff_retaddr,param_1,param_2,param_3);
  return;
}



/* ================= FUN_00419550 @ 00419550 ================= */

void __cdecl FUN_00419550(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 unaff_retaddr;
  
  FUN_00419590(0x17,0x18,(int)DAT_00481e7c,(uint)DAT_00481e79,unaff_retaddr,param_1,param_2,param_3)
  ;
  return;
}



/* ================= FUN_00419570 @ 00419570 ================= */

void __cdecl FUN_00419570(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 unaff_retaddr;
  
  FUN_00419590(0x17,0x1a,(int)DAT_00481e7a,(uint)DAT_00481e79,unaff_retaddr,param_1,param_2,param_3)
  ;
  return;
}



/* ================= FUN_00419590 @ 00419590 ================= */

void __cdecl
FUN_00419590(undefined4 param_1,undefined4 param_2,int param_3,int param_4,undefined4 param_5,
            undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  int iVar1;
  undefined4 unaff_retaddr;
  
  iVar1 = param_4 * param_3;
  if (999 < iVar1) {
    iVar1 = 999;
  }
  FUN_0040c850((byte)param_1,(byte)param_2,&DAT_004324b8,iVar1,unaff_retaddr,param_1,param_2,param_3
               ,param_4,param_5,param_6,param_7,param_8);
  return;
}



/* ================= FUN_00419620 @ 00419620 ================= */

undefined4 FUN_00419620(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  
  FUN_004010e0();
  if (DAT_00495f05 != 0) {
    DAT_00481fdb = DAT_0048327e;
    DAT_00481fda = DAT_0048327d;
    uVar3 = 0;
    DAT_0048234c = (uint)((&DAT_00495c70)[(uint)DAT_0048327d + (uint)DAT_0048327e * 0x20] & 0xf0);
    do {
      bVar1 = DAT_00481fdb;
      bVar2 = DAT_00481fda;
      FUN_004179b0((&DAT_00495f10)[uVar3]);
      if (((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] & 0xf0) != DAT_0048234c)
      {
        DAT_00481fda = bVar2;
        DAT_00481fdb = bVar1;
        return 1;
      }
      bVar2 = (char)uVar3 + 1;
      uVar3 = (uint)bVar2;
    } while (DAT_00495f05 != bVar2);
  }
  return 0;
}



/* ================= FUN_004196f0 @ 004196f0 ================= */

undefined4 FUN_004196f0(void)

{
  uint uVar1;
  byte local_2;
  byte local_1;
  
  if (((byte)(&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] >> 4 != DAT_00495f30)
     && ((byte)(((byte)(&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] >> 4) -
               DAT_00495f30) != -1)) {
    DAT_0048235a = DAT_00495f30 << 4;
    local_2 = 0x20;
    if ((byte)(&DAT_0042b288)[DAT_0048327c] < 0x80) {
      if (((&DAT_0042b288)[DAT_0048327c] & 0x40) != 0) {
        return 1;
      }
      local_2 = 0x21;
    }
    local_1 = 0x37;
    DAT_00482358 = local_2;
    do {
      uVar1 = (uint)local_1;
      if ((((((&DAT_00495af0)[uVar1] & 0xe0) == 0) &&
           ((*(byte *)((int)&DAT_004957a0 + uVar1) & 0x80) == DAT_00495c63)) &&
          (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + uVar1) & 0x1f] & 0x21) == local_2)) &&
         (((*(byte *)((int)&DAT_00495ab0 + uVar1) & 0xe0) == 0 &&
          (((&DAT_00495c70)
            [(*(byte *)((int)&DAT_00495ab0 + uVar1) & 0x1f) +
             ((byte)(&DAT_00495af0)[uVar1] & 0x1f) * 0x20] & 0xf0) == DAT_0048235a)))) {
        DAT_00481fda = *(byte *)((int)&DAT_00495ab0 + (uint)local_1) & 0x1f;
        DAT_00481fdb = (&DAT_00495af0)[local_1] & 0x1f;
        return 0;
      }
      local_1 = local_1 - 1;
    } while (local_1 < 0x80);
  }
  return 1;
}



/* ================= FUN_00419810 @ 00419810 ================= */

void FUN_00419810(void)

{
  DAT_00481e74 = 5;
  DAT_00481e75 = 1;
  DAT_00481e76 = 0;
  return;
}



/* ================= FUN_00419830 @ 00419830 ================= */

bool FUN_00419830(void)

{
  DAT_00481e75 = DAT_00481e75 + -1;
  if (DAT_00481e75 == '\0') {
    DAT_00481e74 = DAT_00481e74 + 1;
    DAT_00481e75 = DAT_00481e76;
    if (5 < DAT_00481e74) {
      DAT_00481e75 = DAT_00481e76 + '\x01';
      DAT_00481e74 = 0;
      DAT_00481e76 = DAT_00481e75;
      FUN_004179b0(4);
    }
  }
  FUN_004179b0(DAT_00481e74);
  if (DAT_00481fdb < DAT_00495c03) {
    return (bool)('\x01' - (DAT_00481fda < DAT_00495c02));
  }
  return true;
}



/* ================= FUN_004198b0 @ 004198b0 ================= */

undefined4 FUN_004198b0(void)

{
  byte bVar1;
  
  DAT_00481df5 = 0;
  if ((DAT_00481fdb < DAT_00495c03) && (DAT_00481fda < DAT_00495c02)) {
    DAT_00482348 = (uint)DAT_00481fdb * 0x20 + (uint)DAT_00481fda;
    bVar1 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20);
    if (((((bVar1 == 0xff) || ((bVar1 & 0x3f) == DAT_0048327b)) &&
         ((bVar1 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf, bVar1 != DAT_00495c04 ||
          ((DAT_00481fdd != 0 && (DAT_00481fdd < 0x80)))))) &&
        (((bVar1 != 0xc && (bVar1 != DAT_00495c43)) ||
         ((DAT_00481fdd != 0 && (0x7f < DAT_00481fdd)))))) &&
       ((((DAT_00481fde & 0x80) != 0 || (bVar1 != DAT_00495c47)) &&
        (((((DAT_00481fde & 1) == 0 || (bVar1 != DAT_00495bc0)) &&
          ((byte)(&DAT_00495c70)[DAT_00482348] >> 4 == DAT_00495f30)) &&
         ((DAT_00481f99 == 0 || (FUN_0041da80(), DAT_00481df5 < DAT_00481f99)))))))) {
      return 0;
    }
  }
  return 1;
}



/* ================= FUN_004199c0 @ 004199c0 ================= */

undefined4 FUN_004199c0(void)

{
  byte bVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  uint uVar4;
  
  uVar3 = DAT_00481fdb;
  uVar2 = DAT_00481fda;
  DAT_00481fda = DAT_0048327d;
  DAT_00481fdb = DAT_0048327e;
  if (7 < (byte)((&DAT_0042b218)[DAT_0048327c] - 1)) {
    DAT_00481fda = uVar2;
    DAT_00481fdb = uVar3;
    return 1;
  }
  DAT_00482374 = (uint)(byte)(&DAT_00436590)[(byte)((&DAT_0042b218)[DAT_0048327c] - 1)];
  FUN_00419810();
  while( true ) {
    bVar1 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20);
    if (bVar1 != 0xff) {
      uVar4 = bVar1 & 0x3f;
      bVar1 = *(byte *)((int)&DAT_004957a0 + uVar4);
      if ((((bVar1 & 0x80) == DAT_00495c63) && (((&DAT_0042b288)[bVar1 & 0x1f] & 0x20) != 0)) &&
         (((&DAT_00495af0)[uVar4] & 0x20) == 0)) {
        if (((&DAT_0042b288)[bVar1 & 0x1f] & 1) == 0) {
          if (((&DAT_0042b288)[DAT_0048327c] & 0x80) != 0) {
            DAT_00482358 = DAT_00481fdb;
            DAT_0048235a = DAT_00481fda;
            return 0;
          }
        }
        else if (((&DAT_0042b288)[DAT_0048327c] & 0x40) == 0) {
          DAT_0048235a = DAT_00481fda;
          DAT_00482358 = DAT_00481fdb;
          return 0;
        }
      }
    }
    DAT_00482374 = DAT_00482374 - 1;
    if (DAT_00482374 == 0) break;
    FUN_00419830();
  }
  DAT_00481fda = uVar2;
  DAT_00481fdb = uVar3;
  return 1;
}



/* ================= FUN_00419ae0 @ 00419ae0 ================= */

undefined4 FUN_00419ae0(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  bool bVar6;
  byte bVar7;
  undefined3 extraout_var;
  int iVar8;
  uint uVar9;
  char cVar10;
  undefined4 *puVar11;
  
  uVar5 = DAT_0048327e;
  uVar4 = DAT_0048327d;
  bVar1 = DAT_0048327b;
  bVar3 = DAT_00481fdb;
  bVar2 = DAT_00481fda;
  if (((&DAT_00495af0)[DAT_0048327b] & 0x60) == 0x20) {
    uVar9 = 0x37;
    do {
      cVar10 = (char)uVar9;
      bVar7 = (&DAT_00495af0)[uVar9];
      if (((bVar7 < 0x80) && ((bVar7 & 0x40) != 0)) && ((bVar7 & 0x3f) == DAT_0048327b)) {
        if ((*(byte *)((int)&DAT_004957a0 + uVar9) & 0x40) != 0) {
          return 1;
        }
        DAT_00483274 = cVar10;
        if ((&DAT_0042b218)[*(byte *)((int)&DAT_004957a0 + uVar9) & 0x1f] == '\0') {
          puVar11 = (undefined4 *)&DAT_00495c70;
          for (iVar8 = 0xa0; iVar8 != 0; iVar8 = iVar8 + -1) {
            *puVar11 = 0;
            puVar11 = puVar11 + 1;
          }
          (&DAT_00495c70)[(uint)DAT_00483276 + (uint)DAT_00483277 * 0x20] = 1;
        }
        else {
          DAT_0048327d = DAT_00483276;
          DAT_00495c6e = 0;
          DAT_00495ef8 = 0;
          DAT_00495f30 = 1;
          DAT_0048327e = DAT_00483277;
          DAT_0048327b = cVar10;
          FUN_004015d0();
          DAT_0048327b = bVar1;
          DAT_0048327d = uVar4;
          DAT_0048327e = uVar5;
        }
        cVar10 = '\x06';
        FUN_00419810();
        DAT_0048235c = 0;
        do {
          bVar6 = FUN_00419830();
          if (CONCAT31(extraout_var,bVar6) != 1) {
            DAT_00482348 = (uint)DAT_00481fdb * 0x20 + (uint)DAT_00481fda;
            bVar1 = (&DAT_00495c70)[DAT_00482348];
            if ((((bVar1 != 0) &&
                 (DAT_0048234c = (uint)bVar1, *(char *)((int)&DAT_00495820 + DAT_00482348) == -1))
                && (((bVar7 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf, bVar7 == 6 ||
                     (bVar7 < 2)) || ((DAT_00481fdd == '\x01' && (bVar7 == DAT_00495c04)))))) &&
               (DAT_0048235c <= (bVar1 & 0xf))) {
              DAT_0048235a = DAT_00481fda;
              DAT_00482358 = DAT_00481fdb;
              DAT_0048235c = bVar1 & 0xf;
            }
          }
          cVar10 = cVar10 + -1;
        } while (cVar10 != '\0');
        if (DAT_0048235c == 0) {
          DAT_00481fda = bVar2;
          DAT_00481fdb = bVar3;
          return 1;
        }
        DAT_00481fda = bVar2;
        DAT_00481fdb = bVar3;
        return 0;
      }
      uVar9 = (uint)(byte)(cVar10 - 1U);
    } while ((byte)(cVar10 - 1U) < 0x80);
  }
  return 1;
}



/* ================= FUN_00419cd0 @ 00419cd0 ================= */

void __cdecl FUN_00419cd0(undefined1 param_1,undefined1 param_2,ushort param_3)

{
  uint uVar1;
  ushort uVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  
  iVar4 = 0;
  psVar3 = &DAT_00436598;
  do {
    if (*psVar3 == CONCAT11(param_1,param_2)) break;
    psVar3 = psVar3 + 1;
    iVar4 = iVar4 + 1;
  } while (psVar3 != (short *)&DAT_004366a8);
  if (iVar4 != 0x88) {
    iVar5 = 0x20;
    puVar6 = (uint *)(&DAT_004366a8 + iVar4 * 0x80);
    FUN_0040da00(param_3);
    do {
      uVar1 = *puVar6;
      puVar6 = puVar6 + 1;
      uVar2 = (ushort)(uVar1 >> 0x10);
      FUN_0040da50(uVar2 & 0x1111);
      FUN_0040da50((ushort)uVar1 & 0x1111);
      FUN_0040da50(uVar2 >> 1 & 0x1111);
      FUN_0040da50((ushort)(uVar1 >> 1) & 0x1111);
      FUN_0040da50(uVar2 >> 2 & 0x1111);
      FUN_0040da50((ushort)(uVar1 >> 2) & 0x1111);
      FUN_0040da50(uVar2 >> 3 & 0x1111);
      FUN_0040da50((ushort)(uVar1 >> 3) & 0x1111);
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    FUN_00412750((uint)param_3,4);
  }
  return;
}



/* ================= FUN_00419de0 @ 00419de0 ================= */

void FUN_00419de0(void)

{
  DAT_00495c70 = 0xff;
  DAT_0048235a = FUN_00419e50();
  if (DAT_0048235a == 0xffffffff) {
    return;
  }
  FUN_00419e00();
  return;
}



/* ================= FUN_00419e00 @ 00419e00 ================= */

void FUN_00419e00(void)

{
  byte bVar1;
  uint uVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  
  bVar1 = DAT_0048235a;
  bVar3 = 0;
  uVar5 = 0;
  do {
    if (((((&DAT_00495af0)[uVar5] & 0x80) == 0) &&
        ((*(byte *)((int)&DAT_00495ab0 + uVar5) & 0xe0) != 0)) &&
       (*(byte *)((int)&DAT_00495ab0 + uVar5) >> 5 == bVar1)) {
      uVar2 = (uint)bVar3;
      bVar3 = bVar3 + 1;
      (&DAT_00495c70)[uVar2] = (char)uVar5;
    }
    bVar4 = (char)uVar5 + 1;
    uVar5 = (uint)bVar4;
  } while (bVar4 < 0x38);
  (&DAT_00495c70)[bVar3] = 0xff;
  return;
}



/* ================= FUN_00419e50 @ 00419e50 ================= */

char FUN_00419e50(void)

{
  char cVar1;
  byte bVar2;
  
  cVar1 = '\0';
  bVar2 = DAT_00495c52 << 4;
  while (((&DAT_0047f7d0)[bVar2] != DAT_0048327d || ((&DAT_0047f7d1)[bVar2] != DAT_0048327e))) {
    bVar2 = bVar2 + 2;
    cVar1 = cVar1 + '\x01';
    if (cVar1 == '\a') {
      return -1;
    }
  }
  return cVar1 + '\x01';
}



/* ================= FUN_00419e90 @ 00419e90 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00419e90(void)

{
  undefined2 uVar1;
  int iVar2;
  char cVar3;
  undefined4 *puVar4;
  char local_1;
  
  puVar4 = (undefined4 *)&DAT_00481e10;
  for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  *(undefined2 *)puVar4 = 0;
  puVar4 = &DAT_00482050;
  for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
  }
  *(undefined2 *)puVar4 = 0;
  if (DAT_00481e73 != '\0') {
    if (DAT_00481e73 == '\x01') {
      DAT_00481e72 = DAT_00481e72 - 1;
      if (DAT_00481e72 == 0xff) {
        DAT_00481e72 = 0x15;
      }
      DAT_00482338 = 0x81;
      _DAT_004834a2 = (ushort)DAT_00481e72 << 3;
      FUN_0040da00(0x7c1f);
      local_1 = '\x14';
      do {
        cVar3 = '\x1f';
        FUN_0040da10((short)DAT_00482338);
        do {
          uVar1 = FUN_0040da80();
          FUN_0040da20(uVar1);
          cVar3 = cVar3 + -1;
        } while (cVar3 != '\0');
        DAT_00482338 = DAT_00482338 + 0x40;
        local_1 = local_1 + -1;
      } while (local_1 != '\0');
      FUN_0041a100();
      FUN_0041a6c0();
      return;
    }
    if (DAT_00481e73 == '\x02') {
      DAT_00482338 = 0x81;
      DAT_0048233c = 0x7c01;
      local_1 = '\x15';
      DAT_00481e71 = DAT_00481e71 - 1 & 0x1f;
      _DAT_004836e0 = (ushort)DAT_00481e71 << 3;
      do {
        FUN_0040da00((short)DAT_0048233c);
        cVar3 = '\x1e';
        FUN_0040da10((short)DAT_00482338);
        do {
          uVar1 = FUN_0040da80();
          FUN_0040da20(uVar1);
          cVar3 = cVar3 + -1;
        } while (cVar3 != '\0');
        DAT_00482338 = DAT_00482338 + 0x40;
        DAT_0048233c = DAT_0048233c + 0x1f;
        local_1 = local_1 + -1;
      } while (local_1 != '\0');
      FUN_0041a270();
      FUN_0041a6c0();
      return;
    }
    if (DAT_00481e73 == '\x03') {
      DAT_00482338 = 0x82;
      DAT_0048233c = 0x7c00;
      local_1 = '\x15';
      DAT_00481e71 = DAT_00481e71 + 1 & 0x1f;
      _DAT_004836e0 = (ushort)DAT_00481e71 << 3;
      do {
        FUN_0040da00((short)DAT_0048233c);
        cVar3 = '\x1e';
        FUN_0040da10((short)DAT_00482338);
        do {
          uVar1 = FUN_0040da80();
          FUN_0040da20(uVar1);
          cVar3 = cVar3 + -1;
        } while (cVar3 != '\0');
        DAT_00482338 = DAT_00482338 + 0x40;
        DAT_0048233c = DAT_0048233c + 0x1f;
        local_1 = local_1 + -1;
      } while (local_1 != '\0');
      FUN_0041a2c0();
      FUN_0041a6c0();
      return;
    }
  }
  DAT_00481e72 = DAT_00481e72 + 1;
  if (DAT_00481e72 == 0x16) {
    DAT_00481e72 = 0;
  }
  DAT_00482338 = 0xc1;
  _DAT_004834a2 = (ushort)DAT_00481e72 << 3;
  FUN_0040da00(0x7c00);
  local_1 = '\x14';
  do {
    cVar3 = '\x1f';
    FUN_0040da10((short)DAT_00482338);
    do {
      uVar1 = FUN_0040da80();
      FUN_0040da20(uVar1);
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_0041a150();
  FUN_0041a6c0();
  return;
}



/* ================= FUN_0041a100 @ 0041a100 ================= */

void FUN_0041a100(void)

{
  if (DAT_00495c6d != '\0') {
    FUN_0041c870();
  }
  FUN_00417840();
  DAT_00481e6f = DAT_00481e72;
  DAT_00481e6e = DAT_00481e71;
  DAT_00481e6a = 0x7c00;
  FUN_0041af20();
  FUN_0041a1c0(DAT_004846fc);
  return;
}



/* ================= FUN_0041a150 @ 0041a150 ================= */

void FUN_0041a150(void)

{
  if (DAT_00495c6d != '\0') {
    FUN_0041cb00();
  }
  FUN_00417870();
  if (DAT_00481e72 + 0x14 < 0x16) {
    DAT_00481e6f = DAT_00481e72 + 0x14;
  }
  else {
    DAT_00481e6f = DAT_00481e72 - 2;
  }
  DAT_00481e6a = 0x7e6c;
  DAT_00481e6e = DAT_00481e71;
  FUN_0041af20();
  FUN_0041a1c0((ushort *)((uint)DAT_00484700 * 0x14 + DAT_004846fc));
  return;
}



/* ================= FUN_0041a1c0 @ 0041a1c0 ================= */

void __cdecl FUN_0041a1c0(ushort *param_1)

{
  ushort uVar1;
  undefined *puVar2;
  char cVar3;
  ushort *puVar4;
  
  DAT_00481e70 = 0;
  do {
    if ((((DAT_00481e71 ^ DAT_00481e70) & 2) == 0) || ((DAT_00481e72 & 1) == 0)) {
      puVar4 = param_1;
    }
    else {
      puVar4 = param_1 + DAT_00484700;
    }
    puVar2 = &DAT_0043aae8;
    if (((DAT_00481e72 * '\x02' ^ DAT_00481e71 ^ DAT_00481e70) & 2) != 0) {
      puVar2 = &DAT_0043aaec;
    }
    puVar4 = (ushort *)(puVar2 + (uint)*puVar4 * 8);
    cVar3 = '\x02';
    do {
      uVar1 = *puVar4;
      puVar4 = puVar4 + 1;
      FUN_0041a3f0(uVar1);
      DAT_00481e6a = DAT_00481e6a + 1;
      DAT_00481e6e = DAT_00481e6e + '\x01';
      DAT_00481e70 = DAT_00481e70 + 1;
      if (DAT_00481e70 == 0x1f) {
        return;
      }
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    param_1 = param_1 + 1;
  } while( true );
}



/* ================= FUN_0041a270 @ 0041a270 ================= */

void FUN_0041a270(void)

{
  if (DAT_00495c6d != '\0') {
    FUN_0041cfa0();
  }
  FUN_004178d0();
  DAT_00481e6f = DAT_00481e72;
  DAT_00481e6e = DAT_00481e71;
  DAT_00481e6a = 0x7c00;
  FUN_0041af20();
  FUN_0041a310(DAT_004846fc);
  return;
}



/* ================= FUN_0041a2c0 @ 0041a2c0 ================= */

void FUN_0041a2c0(void)

{
  if (DAT_00495c6d != '\0') {
    FUN_0041cdd0();
  }
  FUN_004178a0();
  DAT_00481e6a = 0x7c1e;
  DAT_00481e6f = DAT_00481e72;
  DAT_00481e6e = DAT_00481e71 + '\x1e';
  FUN_0041af20();
  FUN_0041a310((ushort *)(DAT_004846fc + 0x1e));
  return;
}



/* ================= FUN_0041a310 @ 0041a310 ================= */

void __cdecl FUN_0041a310(ushort *param_1)

{
  char cVar1;
  ushort *puVar2;
  ushort *puVar3;
  
  DAT_00481e70 = 0;
LAB_0041a31b:
  puVar3 = param_1;
  do {
    puVar2 = (ushort *)(&DAT_0043aae8 + (uint)*puVar3 * 8);
    if ((((DAT_00481e73 * '\x02' ^ DAT_00481e71) & 2) != 0) && ((DAT_00481e70 & 1) == 0)) {
      puVar2 = (ushort *)(&DAT_0043aaec + (uint)*puVar3 * 8);
    }
    if ((DAT_00481e71 & 1) != 0) {
      puVar2 = puVar2 + 1;
    }
    cVar1 = '\x02';
    while( true ) {
      FUN_0041a3f0(*puVar2);
      DAT_00481e6a = DAT_00481e6a + 0x1f;
      DAT_00481e6f = DAT_00481e6f + '\x01';
      DAT_00481e70 = DAT_00481e70 + 1;
      if (DAT_00481e70 == 0x15) {
        return;
      }
      puVar2 = puVar2 + 2;
      if (((DAT_00481e73 * '\x02' ^ DAT_00481e71) & 2) != 0) break;
      cVar1 = cVar1 + -1;
      if (cVar1 == '\0') {
        param_1 = param_1 + DAT_00484700;
        goto LAB_0041a31b;
      }
    }
    if ((DAT_00481e70 & 1) != 0) {
      puVar3 = puVar3 + DAT_00484700;
    }
  } while( true );
}



/* ================= FUN_0041a3f0 @ 0041a3f0 ================= */

void __cdecl FUN_0041a3f0(ushort param_1)

{
  DAT_00481e6e = DAT_00481e6e & 0x1f;
  if (0x15 < DAT_00481e6f) {
    DAT_00481e6f = DAT_00481e6f - 0x16;
  }
  DAT_00482378 = (uint)DAT_00481e6e + (DAT_00481e6f + 0x28) * 0x20;
  DAT_004846f8 = (short)DAT_00482378 << 4;
  FUN_0041a490(param_1);
  FUN_0040da00(DAT_00481e6a);
  if (*(byte *)((int)&DAT_00482050 + (uint)DAT_00481e70) != 0) {
    param_1 = (ushort)(byte)(&DAT_004364db)[*(byte *)((int)&DAT_00482050 + (uint)DAT_00481e70)] << 8
    ;
  }
  FUN_0040da20(param_1 & 0xf000 | (ushort)DAT_00482378);
  return;
}



/* ================= FUN_0041a490 @ 0041a490 ================= */

void __cdecl FUN_0041a490(ushort param_1)

{
  undefined *puVar1;
  undefined1 uVar2;
  int iVar3;
  uint uVar4;
  byte bVar5;
  char cVar6;
  uint uVar7;
  undefined *puVar8;
  byte local_1;
  
  iVar3 = (param_1 & 0xfff) * 0x20;
  puVar8 = &DAT_0043f2a8 + iVar3;
  FUN_0040da00(DAT_004846f8);
  if ((DAT_00495c6d == '\0') || (cVar6 = (&DAT_00481e10)[DAT_00481e70], cVar6 == '\0')) {
    cVar6 = '\x10';
    do {
      uVar2 = *puVar8;
      puVar1 = puVar8 + 1;
      puVar8 = puVar8 + 2;
      FUN_0040da50(CONCAT11(uVar2,*puVar1));
      cVar6 = cVar6 + -1;
    } while (cVar6 != '\0');
    FUN_00412680((uint)DAT_004846f8,1);
    return;
  }
  uVar7 = 0;
  local_1 = cVar6 - 1;
  if (local_1 == 0) {
    do {
      cVar6 = (char)uVar7;
      puVar1 = puVar8 + uVar7;
      bVar5 = cVar6 + 2;
      uVar7 = (uint)bVar5;
      uVar4 = (uint)local_1;
      local_1 = local_1 + 1;
      FUN_0040da50(*(ushort *)(&DAT_00443fe8 + uVar4 * 2) |
                   CONCAT11(*puVar1,puVar8[(byte)(cVar6 + 1)]));
    } while (bVar5 != 0x20);
    FUN_00412680((uint)DAT_004846f8,1);
    return;
  }
  local_1 = cVar6 - 2;
  if (local_1 == 0) {
    do {
      cVar6 = (char)uVar7;
      puVar1 = puVar8 + uVar7;
      bVar5 = cVar6 + 2;
      uVar7 = (uint)bVar5;
      uVar4 = (uint)local_1;
      local_1 = local_1 + 1;
      FUN_0040da50(*(ushort *)(&DAT_00443fc8 + uVar4 * 2) |
                   CONCAT11(*puVar1,puVar8[(byte)(cVar6 + 1)]));
    } while (bVar5 != 0x20);
    FUN_00412680((uint)DAT_004846f8,1);
    return;
  }
  if (cVar6 == '\x03') {
    uVar7 = 2;
    FUN_0040da50(CONCAT11(*puVar8,(&DAT_0043f2a9)[iVar3]) | 0xf000);
    do {
      cVar6 = (char)uVar7;
      puVar1 = puVar8 + uVar7;
      bVar5 = cVar6 + 2;
      uVar7 = (uint)bVar5;
      FUN_0040da50(CONCAT11(*puVar1,puVar8[(byte)(cVar6 + 1)]));
    } while (bVar5 != 0x20);
    FUN_00412680((uint)DAT_004846f8,1);
    return;
  }
  uVar7 = 4;
  FUN_0040da20(0xffff);
  FUN_0040da20(0xffff);
  do {
    cVar6 = (char)uVar7;
    puVar1 = puVar8 + uVar7;
    bVar5 = cVar6 + 2;
    uVar7 = (uint)bVar5;
    FUN_0040da50(CONCAT11(*puVar1,puVar8[(byte)(cVar6 + 1)]));
  } while (bVar5 != 0x20);
  FUN_00412680((uint)DAT_004846f8,1);
  return;
}



/* ================= FUN_0041a6c0 @ 0041a6c0 ================= */

void FUN_0041a6c0(void)

{
  ushort uVar1;
  char cVar2;
  short sVar3;
  char local_1;
  
  local_1 = '\x15';
  sVar3 = 0x81;
  FUN_0040da10(0x7c00);
  do {
    cVar2 = '\x1f';
    FUN_0040da00(sVar3);
    do {
      uVar1 = FUN_0040da80();
      FUN_0040daf0(uVar1);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    sVar3 = sVar3 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_0041a710 @ 0041a710 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0041a710(void)

{
  ushort uVar1;
  ushort uVar2;
  uint uVar3;
  byte bVar4;
  undefined2 uVar5;
  char local_2;
  byte local_1;
  
  bVar4 = DAT_00481e6c == -1;
  if (0xf < (byte)(DAT_00481e6c + 1U)) {
    if ((byte)(DAT_00481e6c + 1U) != 0x10) {
      return;
    }
    bVar4 = bVar4 + 1;
  }
  if (DAT_00481e6d == -1) {
    bVar4 = bVar4 + 1;
  }
  if (10 < (byte)(DAT_00481e6d + 1U)) {
    if ((byte)(DAT_00481e6d + 1U) != 0xb) {
      return;
    }
    bVar4 = bVar4 + 1;
  }
  DAT_0048234c = (uint)bVar4;
  local_1 = 7;
  DAT_00482338 = (((int)(char)(DAT_00495c05 + DAT_00481e6c & 1) + DAT_00481e6d * 2) * 0x20 +
                 (int)DAT_00481e6c) * 2 + 0x81;
  do {
    uVar3 = (uint)local_1;
    _DAT_00482340 = DAT_00482338 + (uint)*(byte *)(uVar3 + 0x444008);
    if ((DAT_0048234c == 0) ||
       (((((_DAT_00482340 & 0x3f) != 0 && (((byte)_DAT_00482340 & 0x3f) < 0x20)) &&
         (0x7f < _DAT_00482340)) && (_DAT_00482340 < 0x5c0)))) {
      FUN_0040da10((short)_DAT_00482340);
      uVar1 = FUN_0040da80();
      DAT_0048233c = (uVar1 & 0xfff) << 4;
      uVar5 = (undefined2)DAT_0048233c;
      if (*(char *)(uVar3 + 0x444010) < '\0') {
        if (*(char *)(uVar3 + 0x444010) == -1) {
          FUN_0040da10(uVar5);
          FUN_0040da00((short)DAT_0048233c);
          uVar1 = FUN_0040daa0();
          FUN_0040da50(uVar1 | 0xf000);
        }
        else {
          FUN_0040da00(uVar5);
          FUN_0040da20(0xffff);
          FUN_0040da20(0xffff);
        }
      }
      else {
        FUN_0040da10(uVar5);
        FUN_0040da00((short)DAT_0048233c);
        local_2 = '\x10';
        bVar4 = *(char *)(uVar3 + 0x444010) * '\x02';
        do {
          uVar1 = *(ushort *)(&DAT_00443fc8 + (uint)bVar4 * 2);
          uVar2 = FUN_0040daa0();
          FUN_0040da50(uVar1 | uVar2);
          local_2 = local_2 + -1;
          bVar4 = bVar4 + 1;
        } while (local_2 != '\0');
      }
      FUN_00412680(DAT_0048233c,1);
    }
    local_1 = local_1 - 1;
  } while ((local_1 & 0x80) == 0);
  DAT_00484708 = 2;
  return;
}



/* ================= FUN_0041a8f0 @ 0041a8f0 ================= */

void FUN_0041a8f0(void)

{
  DAT_00495c53 = 0;
  FUN_00404940(&LAB_0041a910,10);
  return;
}



/* ================= FUN_0041ad60 @ 0041ad60 ================= */

void FUN_0041ad60(void)

{
  byte bVar1;
  
  DAT_00481f94 = &DAT_00444038;
  FUN_00418bb0();
  DAT_00482370 = 0x7081;
  DAT_0048235e = '\x15';
  do {
    FUN_0040da00((short)DAT_00482370);
    DAT_0048235b = '\x1f';
    do {
      bVar1 = FUN_00418bf0();
      DAT_00482348 = (uint)bVar1;
      bVar1 = FUN_00418bf0();
      DAT_00482348 = DAT_00482348 + (bVar1 + 5) * 0x100;
      FUN_0040daf0((ushort)DAT_00482348);
      DAT_0048235b = DAT_0048235b + -1;
    } while (DAT_0048235b != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    DAT_0048235e = DAT_0048235e + -1;
  } while (DAT_0048235e != '\0');
  if (DAT_00495c63 != '\0') {
    FUN_0041be90(&DAT_00444340,0x11,5);
  }
  return;
}



/* ================= FUN_0041ae00 @ 0041ae00 ================= */

void FUN_0041ae00(void)

{
  ushort uVar1;
  undefined *puVar2;
  char cVar3;
  ushort *puVar4;
  ushort *puVar5;
  ushort local_4 [2];
  
  local_4[0] = 0x81;
  local_4[1] = 0x7c00;
  DAT_00481e6a = local_4[DAT_00481e62];
  FUN_0041af20();
  DAT_00481e63 = 0x15;
LAB_0041ae32:
  FUN_00417800();
  DAT_00481e64 = 0x1f;
  puVar4 = DAT_004846fc;
  do {
    if ((((DAT_00481e64 ^ DAT_00481e71) & 2) == 0) && ((DAT_00481e63 & 1) == 0)) {
      puVar5 = puVar4 + DAT_00484700;
    }
    else {
      puVar5 = puVar4;
    }
    puVar2 = &DAT_0043aae8;
    if (((DAT_00481e63 * '\x02' ^ DAT_00481e64 ^ DAT_00481e71) & 2) != 0) {
      puVar2 = &DAT_0043aaec;
    }
    puVar5 = (ushort *)(puVar2 + (uint)*puVar5 * 8);
    cVar3 = '\x02';
    do {
      uVar1 = *puVar5;
      puVar5 = puVar5 + 1;
      FUN_0041af50(uVar1);
      DAT_00481e6a = DAT_00481e6a + 1;
      DAT_00481e64 = DAT_00481e64 - 1;
      if (DAT_00481e64 == 0) {
        if (DAT_00481e62 == 0) {
          DAT_00481e6a = (DAT_00481e6a & 0xffc0) + 0x41;
        }
        DAT_00481e63 = DAT_00481e63 - 1;
        if (DAT_00481e63 == 0) {
          return;
        }
        if ((DAT_00481e63 & 1) != 0) {
          DAT_004846fc = DAT_004846fc + DAT_00484700;
        }
        goto LAB_0041ae32;
      }
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    puVar4 = puVar4 + 1;
  } while( true );
}



/* ================= FUN_0041af20 @ 0041af20 ================= */

void FUN_0041af20(void)

{
  DAT_004846fc = DAT_00484704 + ((uint)DAT_00481e69 * (uint)DAT_00484700 + (uint)DAT_00481e68) * 2;
  return;
}



/* ================= FUN_0041af50 @ 0041af50 ================= */

void __cdecl FUN_0041af50(ushort param_1)

{
  int iVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  
  uVar3 = param_1 & 0xf000;
  FUN_0040da10(DAT_00481e6a);
  FUN_0040da00(DAT_00481e6a);
  uVar2 = FUN_0040da80();
  DAT_004846f8 = uVar2 & 0xfff;
  if ((&DAT_0048206f)[-(uint)DAT_00481e64] != 0) {
    uVar3 = (ushort)(byte)(&DAT_004364db)[(byte)(&DAT_0048206f)[-(uint)DAT_00481e64]] << 8;
  }
  FUN_0040daf0(DAT_004846f8 | uVar3);
  DAT_004846f8 = DAT_004846f8 << 4;
  iVar4 = 0;
  FUN_0040da00(DAT_004846f8);
  do {
    iVar1 = (uint)(ushort)((param_1 & 0x3ff) << 5) + iVar4;
    iVar4 = iVar4 + 2;
    FUN_0040da20(*(undefined2 *)(&DAT_0043f2a8 + iVar1));
  } while (iVar4 < 0x20);
  FUN_00412680((uint)DAT_004846f8,1);
  return;
}



/* ================= FUN_0041b040 @ 0041b040 ================= */

void FUN_0041b040(void)

{
  int iVar1;
  byte bVar2;
  
  DAT_00495c53 = 0;
  if ((DAT_00495c51 < 0x32) && (DAT_00495c48 < 0x80)) {
    FUN_00412890(0x33);
    if (0xe < DAT_00495c52) {
      FUN_00404940(&LAB_0041b0c0,10);
      *(undefined2 *)(DAT_00482334 + 0x10) = 0xf0;
      return;
    }
  }
  else {
    FUN_00412890(0x7c);
  }
  FUN_00404940(&LAB_0041b0e0,10);
  iVar1 = DAT_00482334;
  bVar2 = FUN_0040c120();
  *(ushort *)(iVar1 + 0x10) = (ushort)bVar2;
  return;
}



/* ================= FUN_0041b330 @ 0041b330 ================= */

void FUN_0041b330(void)

{
  int iVar1;
  char cVar2;
  char local_1;
  
  FUN_0040da00(0x1000);
  local_1 = '@';
  do {
    cVar2 = '\0';
    do {
      FUN_0040da20(0);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_00412680(0x1000,0x400);
  local_1 = '\r';
  DAT_00482338 = 0xc0;
  DAT_0048233c = 0x2200;
  do {
    cVar2 = ' ';
    FUN_0040da00((short)DAT_00482338);
    do {
      iVar1 = DAT_0048233c;
      DAT_0048233c = DAT_0048233c + 1;
      FUN_0040daf0((ushort)iVar1);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_0041be90(&DAT_0046a8b4,0x21,3);
  FUN_0041be90(&DAT_0046a8b8,0x61,2);
  FUN_0041b3f0();
  return;
}



/* ================= FUN_0041b3f0 @ 0041b3f0 ================= */

void FUN_0041b3f0(void)

{
  byte bVar1;
  byte bVar2;
  char local_4;
  
  DAT_0048235e = 0x23;
  local_4 = '\x02';
  bVar2 = 0;
  do {
    DAT_0048235a = (&DAT_0046a8c0)[bVar2];
    DAT_00482358 = (&DAT_0046a8c1)[bVar2];
    DAT_0048235c = (&DAT_0046a8c2)[bVar2];
    DAT_00482359 = (&DAT_0046a8c3)[bVar2];
    FUN_00416ed0();
    local_4 = local_4 + -1;
    bVar2 = bVar2 + 4;
  } while (local_4 != '\0');
  bVar2 = 0;
  do {
    DAT_0048235a = 0x44;
    DAT_0048235c = 0x4c;
    bVar1 = bVar2 + 1;
    DAT_0048235e = bVar1 | 0x20;
    DAT_00482358 = bVar2 * '\b' + '\f';
    DAT_00482359 = DAT_00482358;
    FUN_00416ed0();
    bVar2 = bVar1;
  } while (bVar1 != 2);
  return;
}



/* ================= FUN_0041b480 @ 0041b480 ================= */

void FUN_0041b480(void)

{
  byte bVar1;
  char cVar2;
  byte local_3;
  byte local_1;
  
  bVar1 = DAT_00495c51 + 1;
  local_1 = 0;
  if (0x32 < bVar1) {
    bVar1 = 0x32;
  }
  if ((bVar1 < 0x1b) && (local_1 = 1, bVar1 < 0xe)) {
    local_1 = 2;
  }
  DAT_0048235e = 0x23;
  DAT_00482350 = (uint)(byte)(&DAT_0046a8d8)[local_1];
  cVar2 = (&DAT_0046a8d8)[local_1] + 0x1d;
  local_3 = bVar1;
  do {
    DAT_00482358 = 100;
    DAT_00482359 = 0x66;
    DAT_0048235a = cVar2;
    DAT_0048235c = cVar2;
    FUN_00416ed0();
    cVar2 = cVar2 + (char)DAT_00482350;
    local_3 = local_3 - 1;
  } while (local_3 != 0);
  FUN_0041b530(bVar1,local_1,0);
  FUN_0041b530(bVar1,local_1,1);
  return;
}



/* ================= FUN_0041b530 @ 0041b530 ================= */

void __cdecl FUN_0041b530(byte param_1,byte param_2,byte param_3)

{
  byte bVar1;
  undefined3 extraout_var;
  uint uVar2;
  
  bVar1 = FUN_00404940(&LAB_0041b5e0,0x14);
  if (CONCAT31(extraout_var,bVar1) != 0xff) {
    *(ushort *)(DAT_00482334 + 0x10) = (ushort)param_1;
    *(ushort *)(DAT_00482334 + 0x12) = (ushort)param_2;
    uVar2 = (uint)param_3;
    *(ushort *)(DAT_00482334 + 0x14) = (ushort)(byte)(&DAT_0046a8c8)[uVar2];
    *(ushort *)(DAT_00482334 + 0x14) = (ushort)(byte)(&DAT_0046a8c8)[uVar2];
    *(undefined **)(DAT_00482334 + 0x30) = (&PTR_DAT_0046a8d0)[uVar2];
    *(undefined2 *)(DAT_00482334 + 0x1a) = 0;
    *(undefined2 *)(DAT_00482334 + 0x1e) = 99;
    *(undefined2 *)(DAT_00482334 + 0x1c) = 0x1d;
    *(undefined2 *)(DAT_00482334 + 0x20) = 0x1e;
  }
  return;
}



/* ================= FUN_0041b710 @ 0041b710 ================= */

void FUN_0041b710(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 2;
  do {
    FUN_00412dc0(100);
    bVar1 = (char)uVar2 - 1;
    *(undefined1 *)(DAT_00482c20 + 9) = *(undefined1 *)(uVar2 + 0x46a8f0);
    *(undefined2 *)(DAT_00482c20 + 0xc) = *(undefined2 *)(uVar2 * 2 + 0x46a8f8);
    *(ushort *)(DAT_00482c20 + 0x10) = (ushort)*(byte *)(uVar2 + 0x46a900);
    FUN_0041c020(DAT_00482c20,*(byte *)(uVar2 + 0x46a8f4));
    uVar2 = (uint)bVar1;
  } while (bVar1 < 0x80);
  return;
}



/* ================= FUN_0041b780 @ 0041b780 ================= */

void FUN_0041b780(void)

{
  byte bVar1;
  ushort uVar2;
  
  DAT_00481f94 = &DAT_0046a908;
  FUN_00418bb0();
  DAT_00482378 = 0x480;
  DAT_0048235e = '\x0e';
  do {
    DAT_0048235b = ' ';
    FUN_0040da00((short)DAT_00482378);
    do {
      bVar1 = FUN_00418bf0();
      if (bVar1 == 0xff) {
        uVar2 = 0x90;
      }
      else if (bVar1 < 0x69) {
        uVar2 = bVar1 + 0x700;
      }
      else {
        uVar2 = bVar1 + 0x1700;
      }
      FUN_0040daf0(uVar2);
      DAT_0048235b = DAT_0048235b + -1;
    } while (DAT_0048235b != '\0');
    DAT_00482378 = DAT_00482378 + 0x40;
    DAT_0048235e = DAT_0048235e + -1;
  } while (DAT_0048235e != '\0');
  return;
}



/* ================= FUN_0041b810 @ 0041b810 ================= */

void __cdecl FUN_0041b810(uint param_1,uint param_2,int param_3)

{
  char cVar1;
  undefined1 uVar2;
  byte bVar3;
  short sVar4;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  undefined3 extraout_var_02;
  undefined3 extraout_var_03;
  undefined3 extraout_var_04;
  undefined3 extraout_var_05;
  undefined3 extraout_var_06;
  undefined3 extraout_var_07;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint local_4;
  
  uVar7 = *(byte *)((int)&DAT_004957a0 + param_1) & 0x1f;
  uVar9 = *(byte *)((int)&DAT_004957a0 + param_2) & 0x1f;
  DAT_00481e5f = (*(byte *)((int)&DAT_004957e0 + param_1) & 7) + 1;
  DAT_00481e5e = (*(byte *)((int)&DAT_004957e0 + param_2) & 7) + 1;
  if ((&DAT_0042b230)[uVar9] == '\x04') {
    bVar3 = (&DAT_0042b200)[uVar7];
  }
  else {
    bVar3 = (&DAT_0042b1e8)[uVar7];
  }
  if ((&DAT_0042b230)[uVar7] == '\x04') {
    cVar1 = (&DAT_0042b200)[uVar9];
  }
  else {
    cVar1 = (&DAT_0042b1e8)[uVar9];
  }
  if (param_3 == 0) {
    if (bVar3 < 2) {
      uVar2 = FUN_0041bc10(param_1,param_2);
      DAT_00481e4c = (short)CONCAT31(extraout_var_00,uVar2);
    }
    else {
      DAT_00481e4c = 0;
    }
    if (cVar1 == '\x01') {
      uVar2 = FUN_0041bc10(param_2,param_1);
      DAT_00481e50 = (short)CONCAT31(extraout_var_01,uVar2);
      goto LAB_0041b8f9;
    }
  }
  else if (bVar3 < 2) {
    DAT_00481e4c = 0;
  }
  else {
    uVar2 = FUN_0041bc10(param_1,param_2);
    DAT_00481e4c = (short)CONCAT31(extraout_var,uVar2);
  }
  DAT_00481e50 = 0;
LAB_0041b8f9:
  uVar2 = FUN_0041bc50(param_1);
  DAT_00481e48 = (short)CONCAT31(extraout_var_02,uVar2);
  uVar2 = FUN_0041bc50(param_2);
  DAT_00481e4e = (short)CONCAT31(extraout_var_03,uVar2);
  DAT_00481e55 = FUN_0041bc70(param_1);
  iVar8 = 0;
  DAT_00481e5a = FUN_0041bc70(param_2);
  DAT_00481e58 = 0;
  DAT_00481e54 = 0;
  if (param_3 == 0) {
    do {
      bVar3 = FUN_0041bcc0(param_2,iVar8);
      if ((((CONCAT31(extraout_var_04,bVar3) != 0xff) && (DAT_00495c63 == (bVar3 & 0x80))) &&
          (uVar9 = bVar3 & 0x3f, param_1 != uVar9)) &&
         (((*(byte *)((int)&DAT_00495ab0 + uVar9) & 0xe0) == 0 &&
          (((&DAT_00495af0)[uVar9] & 0x40) == 0)))) {
        uVar2 = FUN_0041bc10(uVar9,param_2);
        DAT_00481e58 = DAT_00481e58 +
                       (short)CONCAT31(extraout_var_05,uVar2) *
                       ((*(byte *)((int)&DAT_004957e0 + uVar9) & 7) + 1);
        DAT_00481e54 = DAT_00481e54 | '\x01' << ((byte)iVar8 & 0x1f);
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < 6);
    DAT_00481e58 = (short)((int)DAT_00481e58 / (DAT_00481e5f * 2));
  }
  iVar8 = 0;
  DAT_00481e5c = 0;
  DAT_00481e56 = 0;
  if (param_3 == 0) {
    do {
      bVar3 = FUN_0041bcc0(param_1,iVar8);
      if (((CONCAT31(extraout_var_06,bVar3) != 0xff) && (DAT_00495c63 != (bVar3 & 0x80))) &&
         ((uVar9 = bVar3 & 0x3f, param_2 != uVar9 &&
          (((*(byte *)((int)&DAT_00495ab0 + uVar9) & 0xe0) == 0 &&
           (((&DAT_00495af0)[uVar9] & 0x40) == 0)))))) {
        uVar2 = FUN_0041bc50(uVar9);
        DAT_00481e5c = DAT_00481e5c +
                       (short)CONCAT31(extraout_var_07,uVar2) *
                       ((*(byte *)((int)&DAT_004957e0 + uVar9) & 7) + 1);
        DAT_00481e56 = DAT_00481e56 | '\x01' << ((byte)iVar8 & 0x1f);
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < 6);
    DAT_00481e5c = (short)((int)DAT_00481e5c / (DAT_00481e5f * 2));
  }
  iVar8 = 0;
  uVar9 = 0;
  DAT_00481e4a = '\0';
  if (param_3 == 0) {
    do {
      DAT_00481e4a = (byte)(uVar9 >> 1);
      bVar3 = FUN_0041bd40(param_2,iVar8);
      uVar9 = (uint)DAT_00481e4a;
      if ((bVar3 & 0x40) != 0) {
        uVar9 = DAT_00481e4a | 0x20;
      }
      DAT_00481e4a = (byte)uVar9;
      iVar8 = iVar8 + 1;
    } while (iVar8 < 6);
  }
  iVar8 = (int)DAT_00481e58 + (int)DAT_00481e4c;
  iVar5 = (int)DAT_00481e4e + (int)DAT_00481e5a + (int)DAT_00481e5c;
  if (DAT_00481e4a == '?') {
    iVar5 = iVar5 / 2;
  }
  if (100 < iVar8) {
    iVar8 = 100;
  }
  if (100 < iVar5) {
    iVar5 = 100;
  }
  local_4 = (uint)(*(byte *)((int)&DAT_004957e0 + param_1) >> 5);
  if (param_3 == 0) {
    iVar6 = (int)DAT_00481e50;
    iVar10 = (int)DAT_00481e48 + (int)DAT_00481e55;
    if (DAT_00481e4a == '?') {
      iVar6 = iVar6 / 2;
    }
    if (100 < iVar6) {
      iVar6 = 100;
    }
    if (100 < iVar10) {
      iVar10 = 100;
    }
    sVar4 = (short)((int)((uint)(byte)(&DAT_0042b158)[*(byte *)((int)&DAT_004957e0 + param_2) >> 5]
                         * (int)(short)(((100 - iVar10) * iVar6) / 100)) / 100);
  }
  else {
    sVar4 = 0;
  }
  DAT_00481e52 = (short)DAT_00481e5f *
                 (short)((int)((uint)(byte)(&DAT_0042b158)[local_4] *
                              (int)(short)(((100 - iVar5) * iVar8) / 100)) / 100);
  DAT_00481e60 = DAT_00481e5e * sVar4;
  return;
}



/* ================= FUN_0041bc10 @ 0041bc10 ================= */

undefined1 __cdecl FUN_0041bc10(int param_1,int param_2)

{
  undefined *puVar1;
  
  puVar1 = &DAT_0042b1a0;
  if ((&DAT_0042b230)[*(byte *)((int)&DAT_004957a0 + param_2) & 0x1f] == '\x04') {
    puVar1 = &DAT_0042b1b8;
  }
  return puVar1[*(byte *)((int)&DAT_004957a0 + param_1) & 0x1f];
}



/* ================= FUN_0041bc50 @ 0041bc50 ================= */

undefined1 __cdecl FUN_0041bc50(int param_1)

{
  return (&DAT_0042b1d0)[*(byte *)((int)&DAT_004957a0 + param_1) & 0x1f];
}



/* ================= FUN_0041bc70 @ 0041bc70 ================= */

undefined1 __cdecl FUN_0041bc70(int param_1)

{
  if (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + param_1) & 0x1f] & 0x40) == 0) {
    return (&DAT_0042b160)
           [*(byte *)((int)&DAT_00481a80 +
                     (uint)(*(byte *)((int)&DAT_00495ab0 + param_1) & 0x1f) +
                     ((byte)(&DAT_00495af0)[param_1] & 0x1f) * 0x20) & 0xf];
  }
  return 0;
}



/* ================= FUN_0041bcc0 @ 0041bcc0 ================= */

undefined1 __cdecl FUN_0041bcc0(int param_1,int param_2)

{
  uint local_8;
  uint local_4;
  
  local_4 = *(byte *)((int)&DAT_00495ab0 + param_1) & 0x1f;
  local_8 = (uint)((&DAT_00495af0)[param_1] & 0x1f);
  FUN_00403490(&local_4,(int *)&local_8,param_2);
  if ((local_4 < DAT_00495c02) && (local_8 < DAT_00495c03)) {
    return *(undefined1 *)((int)&DAT_00495820 + local_4 + local_8 * 0x20);
  }
  return 0xff;
}



/* ================= FUN_0041bd40 @ 0041bd40 ================= */

undefined1 __cdecl FUN_0041bd40(int param_1,int param_2)

{
  uint local_8;
  uint local_4;
  
  local_4 = *(byte *)((int)&DAT_00495ab0 + param_1) & 0x1f;
  local_8 = (uint)((&DAT_00495af0)[param_1] & 0x1f);
  FUN_00403490(&local_4,(int *)&local_8,param_2);
  if ((local_4 < DAT_00495c02) && (local_8 < DAT_00495c03)) {
    return *(undefined1 *)((int)&DAT_00481a80 + local_4 + local_8 * 0x20);
  }
  return 0;
}



/* ================= FUN_0041bdc0 @ 0041bdc0 ================= */

ushort __cdecl FUN_0041bdc0(ushort param_1)

{
  return *(short *)(&DAT_0046ba08 + (param_1 >> 5 & 0xfffffffe)) << 5 |
         *(short *)(&DAT_0046ba08 + (param_1 & 7) * 2) << 10 |
         *(ushort *)(&DAT_0046ba08 + ((param_1 & 0x38) >> 2)) | 1;
}



/* ================= FUN_0041be10 @ 0041be10 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0041be10(void)

{
  if (DAT_004834d0 == 1) {
    DAT_004834d0 = 0;
  }
  _DAT_00483550 = DAT_004834d0;
  _DAT_00483530 = DAT_004834d0;
  _DAT_00483510 = DAT_004834d0;
  _DAT_004834f0 = DAT_004834d0;
  _DAT_004835b0 = DAT_004834d0;
  _DAT_00483590 = DAT_004834d0;
  _DAT_00483570 = DAT_004834d0;
  _DAT_004836c0 = 0;
  _DAT_004836a0 = 0;
  _DAT_00483680 = 0;
  _DAT_00483660 = 0;
  DAT_004834aa = 1;
  _DAT_00483640 = 0;
  _DAT_00483620 = 0;
  _DAT_00483600 = 0;
  DAT_004835e0 = 0;
  return;
}



/* ================= FUN_0041be90 @ 0041be90 ================= */

void __cdecl FUN_0041be90(byte *param_1,ushort param_2,byte param_3)

{
  ushort uVar1;
  uint uVar2;
  ushort uVar3;
  undefined2 *puVar4;
  ushort *puVar5;
  ushort *puVar6;
  byte *pbVar7;
  
  puVar4 = &DAT_004835e0;
  if (param_2 < 0x100) {
    puVar4 = &DAT_004834d0;
  }
  uVar2 = (uint)param_3;
  puVar5 = puVar4 + (param_2 & 0xff);
  if (uVar2 != 0) {
    do {
      if (8 < uVar2) {
        uVar2 = 8;
      }
      uVar3 = (ushort)*param_1;
      param_3 = param_3 - (char)uVar2;
      puVar6 = puVar5;
      pbVar7 = param_1 + 1;
      do {
        uVar3 = uVar3 * 2;
        param_1 = pbVar7 + 1;
        puVar5 = puVar6 + 1;
        uVar1 = FUN_0041bdc0((ushort)*pbVar7 | uVar3 & 0x100);
        *puVar6 = uVar1;
        uVar2 = uVar2 - 1;
        puVar6 = puVar5;
        pbVar7 = param_1;
      } while (uVar2 != 0);
      uVar2 = (uint)param_3;
    } while (uVar2 != 0);
  }
  FUN_0041be10();
  return;
}



/* ================= FUN_0041bf20 @ 0041bf20 ================= */

void FUN_0041bf20(void)

{
  DAT_00481e38 = 0xffffffff;
  DAT_00481e3c = 0xffffffff;
  DAT_00481e40 = 0;
  DAT_00481e44 = 0;
  return;
}



/* ================= FUN_0041bf50 @ 0041bf50 ================= */

void __cdecl FUN_0041bf50(byte param_1)

{
  FUN_0041be90((&PTR_DAT_0046c8d0)[(uint)param_1 * 2],0,
               (byte)*(undefined4 *)(&DAT_0046c8d4 + (uint)param_1 * 8));
  return;
}



/* ================= FUN_0041bf80 @ 0041bf80 ================= */

void FUN_0041bf80(void)

{
  byte local_4 [4];
  
  local_4[0] = 0x80;
  local_4[1] = 0xb6;
  local_4[2] = 0x92;
  FUN_0041be90(local_4,0x71,2);
  return;
}



/* ================= FUN_0041bfb0 @ 0041bfb0 ================= */

uint __cdecl FUN_0041bfb0(byte param_1)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    if (*(byte *)((int)&DAT_00481e38 + uVar2) == param_1) goto LAB_0041c014;
    bVar1 = (char)uVar2 + 1;
    uVar2 = (uint)bVar1;
  } while (bVar1 < 8);
  uVar2 = 0;
  while (*(char *)((int)&DAT_00481e40 + uVar2) != '\0') {
    bVar1 = (char)uVar2 + 1;
    uVar2 = (uint)bVar1;
    if (7 < bVar1) {
      return 0xff;
    }
  }
  *(byte *)((int)&DAT_00481e38 + uVar2) = param_1;
  FUN_0041be90(&DAT_0046c958 + (uint)param_1 * 0x12,((short)uVar2 + 0x10) * 0x10,0x10);
LAB_0041c014:
  *(char *)((int)&DAT_00481e40 + uVar2) = *(char *)((int)&DAT_00481e40 + uVar2) + '\x01';
  return uVar2;
}



/* ================= FUN_0041c020 @ 0041c020 ================= */

void __cdecl FUN_0041c020(int param_1,byte param_2)

{
  uint uVar1;
  
  uVar1 = FUN_0041bfb0(param_2);
  *(char *)(param_1 + 0x14) = (char)uVar1;
  return;
}



/* ================= FUN_0041c040 @ 0041c040 ================= */

void __cdecl FUN_0041c040(byte param_1)

{
  *(char *)((int)&DAT_00481e40 + (uint)param_1) = *(char *)((int)&DAT_00481e40 + (uint)param_1) + -1
  ;
  return;
}



/* ================= FUN_0041c050 @ 0041c050 ================= */

void __cdecl FUN_0041c050(int param_1)

{
  FUN_0041c040(*(byte *)(param_1 + 0x14));
  return;
}



/* ================= FUN_0041c070 @ 0041c070 ================= */

void FUN_0041c070(void)

{
  byte bVar1;
  
  bVar1 = FUN_00404940(&LAB_0041c090,0x78);
  if (bVar1 != 0xff) {
    DAT_00495f3c = 1;
  }
  return;
}



/* ================= FUN_0041c200 @ 0041c200 ================= */

void __cdecl FUN_0041c200(byte param_1)

{
  byte *pbVar1;
  uint uVar2;
  
  uVar2 = (uint)(*(byte *)((int)&DAT_004957a0 + (uint)param_1) & 0x1f);
  DAT_00481f94 = FUN_0040cd50(uVar2 + 0x1a);
  FUN_00418bb0();
  pbVar1 = (&PTR_DAT_0042fee8)[uVar2];
  *(ushort *)(DAT_00482330 + 0x18) = (ushort)*pbVar1;
  *(byte **)(DAT_00482330 + 0x30) = pbVar1 + 1;
  *(undefined2 *)(DAT_00482330 + 0x1a) = 0;
  return;
}



/* ================= FUN_0041c260 @ 0041c260 ================= */

void FUN_0041c260(void)

{
  undefined2 uVar1;
  int iVar2;
  
  iVar2 = 0x5c0;
  FUN_0040da10(0x1000);
  FUN_0040da00(0x2800);
  do {
    uVar1 = FUN_0040da80();
    FUN_0040da20(uVar1);
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  FUN_00412750(0x2800,0x17);
  return;
}



/* ================= FUN_0041c2b0 @ 0041c2b0 ================= */

void FUN_0041c2b0(void)

{
  int iVar1;
  
  FUN_0041c260();
  iVar1 = 0x5c0;
  FUN_0040da00(0x1000);
  do {
    FUN_0040da20(0);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  FUN_00412750(0x1000,0x17);
  return;
}



/* ================= FUN_0041c2f0 @ 0041c2f0 ================= */

void FUN_0041c2f0(void)

{
  FUN_0041c310(4);
  return;
}



/* ================= FUN_0041c300 @ 0041c300 ================= */

void FUN_0041c300(void)

{
  FUN_0041c310(0);
  return;
}



/* ================= FUN_0041c310 @ 0041c310 ================= */

void __cdecl FUN_0041c310(byte param_1)

{
  byte bVar1;
  
  bVar1 = FUN_00404940(FUN_0041c360,5);
  if (bVar1 != 0xff) {
    *(ushort *)(DAT_00482334 + 0x10) = (ushort)param_1;
    *(undefined2 *)(DAT_00482334 + 0x12) = 0;
    *(undefined2 *)(DAT_00482334 + 0x14) = 0;
    *(undefined2 *)(DAT_00482334 + 0x16) = 4;
    DAT_00495f3c = 1;
  }
  return;
}



/* ================= FUN_0041c360 @ 0041c360 ================= */

void FUN_0041c360(void)

{
  int iVar1;
  ushort uVar2;
  char cVar3;
  char local_1;
  
  *(short *)((int)DAT_00482330 + 0x12) = *(short *)((int)DAT_00482330 + 0x12) + -1;
  if ((*(byte *)((int)DAT_00482330 + 0x12) & 0x80) != 0) {
    iVar1 = (short)DAT_00482330[4] * 4;
    DAT_00482370 = (uint)*(ushort *)(&DAT_0046ba18 + (uint)*(byte *)(DAT_00482330 + 5) * 2);
    local_1 = (&DAT_0046ba20)[*(byte *)(DAT_00482330 + 5)];
    FUN_0040da10(*(ushort *)(&DAT_0046ba18 + (uint)*(byte *)(DAT_00482330 + 5) * 2) + 0x1800);
    FUN_0040da00((short)DAT_00482370);
    do {
      cVar3 = '\x10';
      do {
        uVar2 = FUN_0040da80();
        FUN_0040da20(uVar2 & *(ushort *)(&DAT_0046ba28 + iVar1));
        uVar2 = FUN_0040da80();
        FUN_0040da20(uVar2 & *(ushort *)(&DAT_0046ba28 + iVar1));
        uVar2 = FUN_0040da80();
        FUN_0040da20(uVar2 & *(ushort *)(&DAT_0046ba2a + iVar1));
        uVar2 = FUN_0040da80();
        FUN_0040da20(uVar2 & *(ushort *)(&DAT_0046ba2a + iVar1));
        cVar3 = cVar3 + -1;
      } while (cVar3 != '\0');
      local_1 = local_1 + -1;
    } while (local_1 != '\0');
    FUN_00412750((uint)*(ushort *)(&DAT_0046ba18 + (uint)*(byte *)(DAT_00482330 + 5) * 2),
                 (uint)(byte)(&DAT_0046ba20)[*(byte *)(DAT_00482330 + 5)]);
    *(undefined2 *)((int)DAT_00482330 + 0x12) = 0;
    *(short *)(DAT_00482330 + 5) = (short)DAT_00482330[5] + 1;
    if ((short)DAT_00482330[5] == 4) {
      *(short *)(DAT_00482330 + 5) = 0;
      *(short *)(DAT_00482330 + 4) = (short)DAT_00482330[4] + 1;
      *(short *)((int)DAT_00482330 + 0x16) = *(short *)((int)DAT_00482330 + 0x16) + -1;
      if (*(short *)((int)DAT_00482330 + 0x16) == 0) {
        DAT_00495f3c = 0;
        FUN_00404a10(DAT_00482330);
      }
    }
  }
  return;
}



/* ================= FUN_0041c4a0 @ 0041c4a0 ================= */

HKEY FUN_0041c4a0(void)

{
  LSTATUS LVar1;
  HKEY local_10;
  HKEY local_c;
  HKEY local_8;
  DWORD local_4;
  
  local_8 = (HKEY)0x0;
  local_10 = (HKEY)0x0;
  local_c = (HKEY)0x0;
  LVar1 = RegOpenKeyExA((HKEY)0x80000001,"Software",0,0x2001f,&local_10);
  if ((LVar1 == 0) &&
     (LVar1 = RegCreateKeyExA(local_10,"HUDSON",0,(LPSTR)0x0,0,0x2001f,(LPSECURITY_ATTRIBUTES)0x0,
                              &local_c,&local_4), LVar1 == 0)) {
    RegCreateKeyExA(local_c,"NECTARIS",0,(LPSTR)0x0,0,0x2001f,(LPSECURITY_ATTRIBUTES)0x0,&local_8,
                    &local_4);
  }
  if (local_10 != (HKEY)0x0) {
    RegCloseKey(local_10);
  }
  if (local_c != (HKEY)0x0) {
    RegCloseKey(local_c);
  }
  return local_8;
}



/* ================= FUN_0041c550 @ 0041c550 ================= */

undefined4 __cdecl FUN_0041c550(LPCSTR param_1)

{
  HKEY hKey;
  HKEY local_8;
  DWORD local_4;
  
  local_8 = (HKEY)0x0;
  hKey = FUN_0041c4a0();
  if (hKey == (HKEY)0x0) {
    return 0;
  }
  RegCreateKeyExA(hKey,param_1,0,(LPSTR)0x0,0,0x2001f,(LPSECURITY_ATTRIBUTES)0x0,&local_8,&local_4);
  RegCloseKey(hKey);
  return local_8;
}



/* ================= FUN_0041c5b0 @ 0041c5b0 ================= */

UINT __cdecl FUN_0041c5b0(LPCSTR param_1,LPCSTR param_2,UINT param_3)

{
  HKEY hKey;
  LSTATUS LVar1;
  UINT UVar2;
  DWORD local_c;
  UINT local_8;
  DWORD local_4;
  
  if (PTR_s_HUDSON_0046ba48 == (undefined *)0x0) {
    UVar2 = GetPrivateProfileIntA(param_1,param_2,param_3,"NECTARIS");
    return UVar2;
  }
  hKey = (HKEY)FUN_0041c550(param_1);
  if (hKey == (HKEY)0x0) {
    return param_3;
  }
  local_c = 4;
  LVar1 = RegQueryValueExA(hKey,param_2,(LPDWORD)0x0,&local_4,(LPBYTE)&local_8,&local_c);
  RegCloseKey(hKey);
  if (LVar1 == 0) {
    return local_8;
  }
  return param_3;
}



/* ================= FUN_0041c650 @ 0041c650 ================= */

uint __cdecl
FUN_0041c650(LPCSTR param_1,LPCSTR param_2,undefined4 param_3,LPBYTE param_4,uint param_5)

{
  HKEY hKey;
  LSTATUS LVar1;
  DWORD DVar2;
  DWORD local_8;
  DWORD local_4;
  
  if (PTR_s_HUDSON_0046ba48 == (undefined *)0x0) {
    DVar2 = GetPrivateProfileStringA
                      (param_1,param_2,&DAT_0042fc48,(LPSTR)param_4,param_5,"NECTARIS");
    return DVar2;
  }
  hKey = (HKEY)FUN_0041c550(param_1);
  if (hKey == (HKEY)0x0) {
    return 0;
  }
  LVar1 = RegQueryValueExA(hKey,param_2,(LPDWORD)0x0,&local_4,(LPBYTE)0x0,&local_8);
  if ((LVar1 == 0) && (local_8 <= param_5)) {
    RegQueryValueExA(hKey,param_2,(LPDWORD)0x0,&local_4,param_4,&local_8);
  }
  else {
    local_8 = 0;
  }
  RegCloseKey(hKey);
  return local_8;
}



/* ================= FUN_0041c710 @ 0041c710 ================= */

uint __cdecl FUN_0041c710(LPCSTR param_1,LPCSTR param_2,undefined4 param_3)

{
  HKEY hKey;
  LSTATUS LVar1;
  uint uVar2;
  CHAR local_10 [16];
  
  if (PTR_s_HUDSON_0046ba48 == (undefined *)0x0) {
    wsprintfA(local_10,&DAT_0046ba4c,param_3);
    uVar2 = WritePrivateProfileStringA(param_1,param_2,local_10,"NECTARIS");
    return uVar2;
  }
  hKey = (HKEY)FUN_0041c550(param_1);
  if (hKey == (HKEY)0x0) {
    return 0;
  }
  LVar1 = RegSetValueExA(hKey,param_2,0,4,(BYTE *)&param_3,4);
  RegCloseKey(hKey);
  return (uint)(LVar1 == 0);
}



/* ================= FUN_0041c7a0 @ 0041c7a0 ================= */

uint __cdecl FUN_0041c7a0(LPCSTR param_1,LPCSTR param_2,BYTE *param_3)

{
  HKEY hKey;
  LSTATUS LVar1;
  int iVar2;
  uint uVar3;
  
  if (PTR_s_HUDSON_0046ba48 == (undefined *)0x0) {
    uVar3 = WritePrivateProfileStringA(param_1,param_2,(LPCSTR)param_3,"NECTARIS");
    return uVar3;
  }
  if (param_2 == (LPCSTR)0x0) {
    hKey = FUN_0041c4a0();
    if (hKey == (HKEY)0x0) {
      return 0;
    }
    LVar1 = RegDeleteKeyA(hKey,param_1);
  }
  else if (param_3 == (BYTE *)0x0) {
    hKey = (HKEY)FUN_0041c550(param_1);
    if (hKey == (HKEY)0x0) {
      return 0;
    }
    LVar1 = RegDeleteValueA(hKey,param_2);
  }
  else {
    hKey = (HKEY)FUN_0041c550(param_1);
    if (hKey == (HKEY)0x0) {
      return 0;
    }
    iVar2 = lstrlenA((LPCSTR)param_3);
    LVar1 = RegSetValueExA(hKey,param_2,0,1,param_3,iVar2 + 1);
  }
  RegCloseKey(hKey);
  return (uint)(LVar1 == 0);
}



/* ================= FUN_0041c870 @ 0041c870 ================= */

void FUN_0041c870(void)

{
  byte bVar1;
  char *pcVar2;
  uint uVar3;
  byte bVar4;
  uint uVar5;
  byte bVar6;
  
  pcVar2 = &DAT_00495c50 + (int)(char)(DAT_00481dbe + -1) + (uint)DAT_00481dbf * 0x20;
  if ((DAT_00495c45 & 1) == 0) {
    if (DAT_00481dbf != 0) {
      if ((DAT_00481dbe != '\0') && (*pcVar2 != '\0')) {
        if ((DAT_00495c44 & 2) == 0) {
          DAT_00481e10 = '\x02';
          DAT_00481e11 = 3;
        }
        else {
          DAT_00481e10 = '\x03';
        }
      }
      uVar5 = 0;
      do {
        if (pcVar2[uVar5 + 1] != '\0') {
          uVar3 = uVar5 * 2 & 0xff;
          if ((DAT_00495c44 + uVar3 & 2) == 0) {
            (&DAT_00481e11)[uVar3] = 4;
            if ((&DAT_00481e12)[uVar3] == '\0') {
              (&DAT_00481e12)[uVar3] = 3;
            }
          }
          else {
            (&DAT_00481e10)[uVar3] = 1;
            (&DAT_00481e12)[uVar3] = 2;
            (&DAT_00481e13)[uVar3] = 3;
          }
        }
        bVar4 = (char)uVar5 + 1;
        uVar5 = (uint)bVar4;
      } while (bVar4 != 0xf);
      if (((DAT_00495c44 & 2) == 0) && (pcVar2[0x10] != '\0')) {
        DAT_00481e2e = 1;
      }
    }
    if (((DAT_00481dbe != '\0') && ((DAT_00495c44 & 2) != 0)) && (pcVar2[0x20] != '\0')) {
      DAT_00481e10 = '\x01';
    }
    bVar4 = DAT_00495c44 & 2;
    bVar6 = bVar4 >> 1;
    do {
      if (pcVar2[bVar6 + 0x21] != '\0') {
        bVar1 = bVar6 * '\x02';
        (&DAT_00481e10)[bVar1] = 2;
        (&DAT_00481e11)[bVar1] = 4;
        (&DAT_00481e12)[bVar1] = 1;
      }
      bVar6 = bVar6 + 2;
    } while (bVar6 < 0xf);
    if ((bVar4 != 0) && (pcVar2[0x30] != '\0')) {
      DAT_00481e2e = 2;
      return;
    }
  }
  else {
    if ((DAT_00481dbe != '\0') && (*pcVar2 != '\0')) {
      if ((DAT_00495c44 & 2) == 0) {
        DAT_00481e10 = '\x01';
      }
      else {
        DAT_00481e10 = '\x02';
        DAT_00481e11 = 3;
      }
    }
    bVar4 = 0;
    do {
      if (pcVar2[bVar4 + 1] != '\0') {
        uVar5 = (uint)(byte)(bVar4 * '\x02');
        if ((DAT_00495c44 + uVar5 & 2) == 0) {
          (&DAT_00481e10)[uVar5] = 1;
          (&DAT_00481e12)[uVar5] = 2;
          (&DAT_00481e13)[uVar5] = 3;
        }
        else {
          (&DAT_00481e10)[uVar5] = 2;
          (&DAT_00481e11)[uVar5] = 4;
          (&DAT_00481e12)[uVar5] = 1;
        }
      }
      bVar4 = bVar4 + 1;
    } while (bVar4 != 0xf);
    if ((pcVar2[0x10] != '\0') && (DAT_00481e2e = 2, (DAT_00495c44 & 2) != 0)) {
      DAT_00481e2e = 1;
    }
    if (1 < DAT_00481dbf) {
      if ((((DAT_00481dbe != '\0') && ((DAT_00495c44 & 2) == 0)) && (pcVar2[-0x20] != '\0')) &&
         (DAT_00481e10 == '\0')) {
        DAT_00481e10 = '\x03';
      }
      bVar4 = ~(DAT_00495c44 >> 1) & 1;
      do {
        if (pcVar2[bVar4 - 0x1f] != '\0') {
          bVar6 = bVar4 * '\x02';
          (&DAT_00481e11)[bVar6] = 4;
          if ((&DAT_00481e12)[bVar6] == '\0') {
            (&DAT_00481e12)[bVar6] = 3;
          }
        }
        bVar4 = bVar4 + 2;
      } while (bVar4 < 0xf);
    }
  }
  return;
}



/* ================= FUN_0041cb00 @ 0041cb00 ================= */

void FUN_0041cb00(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  byte bVar4;
  uint uVar5;
  byte bVar6;
  char *pcVar7;
  
  cVar2 = DAT_00481dbe + -1;
  if (cVar2 < '\0') {
    cVar2 = '\0';
  }
  pcVar7 = &DAT_00495db0 + (int)cVar2 + (uint)DAT_00481dbf * 0x20;
  if ((DAT_00495c45 & 1) != 0) {
    if (DAT_00481dbf < 10) {
      if (DAT_00481dbe != '\0') {
        if (*pcVar7 != '\0') {
          if ((DAT_00495c44 & 2) == 0) {
            DAT_00481e10 = '\x01';
          }
          else {
            DAT_00481e10 = '\x02';
            DAT_00481e11 = '\x03';
          }
        }
        pcVar7 = pcVar7 + 1;
      }
      bVar4 = 0;
      do {
        if (pcVar7[bVar4] != '\0') {
          uVar5 = (uint)(byte)(bVar4 * '\x02');
          if ((DAT_00495c44 + uVar5 & 2) == 0) {
            (&DAT_00481e10)[uVar5] = 1;
            (&DAT_00481e12)[uVar5] = 2;
            if ((&DAT_00481e13)[uVar5] == '\0') {
              (&DAT_00481e13)[uVar5] = 3;
            }
          }
          else {
            (&DAT_00481e10)[uVar5] = 2;
            (&DAT_00481e11)[uVar5] = 4;
            (&DAT_00481e12)[uVar5] = 1;
          }
        }
        bVar4 = bVar4 + 1;
      } while (bVar4 != 0xf);
      if ((DAT_00495c44 < 0x1e) && (pcVar7[0xf] != '\0')) {
        if ((DAT_00495c44 & 2) == 0) {
          DAT_00481e2e = 2;
          pcVar7 = pcVar7 + -0x20;
          goto LAB_0041cd83;
        }
        DAT_00481e2e = 1;
      }
      pcVar7 = pcVar7 + -0x20;
    }
    else if (DAT_00481dbe == '\0') {
      pcVar7 = pcVar7 + -0x20;
    }
    else {
      pcVar7 = pcVar7 + -0x1f;
    }
LAB_0041cd83:
    bVar4 = ~(DAT_00495c44 >> 1) & 1;
    do {
      if (pcVar7[bVar4] != '\0') {
        bVar6 = bVar4 * '\x02';
        (&DAT_00481e11)[bVar6] = 4;
        if ((&DAT_00481e12)[bVar6] == '\0') {
          (&DAT_00481e12)[bVar6] = 3;
        }
      }
      bVar4 = bVar4 + 2;
    } while (bVar4 < 0xf);
    return;
  }
  if (DAT_00481dbf < 10) {
    if (DAT_00481dbe != '\0') {
      if (((DAT_00495c44 & 2) != 0) && (*pcVar7 != '\0')) {
        DAT_00481e10 = '\x01';
      }
      pcVar7 = pcVar7 + 1;
    }
    bVar4 = DAT_00495c44 & 2;
    bVar6 = bVar4 >> 1;
    do {
      if (pcVar7[bVar6] != '\0') {
        bVar1 = bVar6 * '\x02';
        (&DAT_00481e10)[bVar1] = 2;
        (&DAT_00481e11)[bVar1] = 4;
        (&DAT_00481e12)[bVar1] = 1;
      }
      bVar6 = bVar6 + 2;
    } while (bVar6 < 0xf);
    if (((bVar4 != 0) && (DAT_00495c44 < 0x1e)) && (pcVar7[0xf] != '\0')) {
      DAT_00481e2e = 2;
    }
    pcVar7 = pcVar7 + -0x21;
    if (DAT_00481dbe != '\0') goto LAB_0041cbd4;
  }
  else {
    if (DAT_00481dbe == '\0') {
      pcVar7 = pcVar7 + -0x20;
      goto LAB_0041cc0c;
    }
    pcVar7 = pcVar7 + -0x20;
LAB_0041cbd4:
    if (*pcVar7 != '\0') {
      if ((DAT_00495c44 & 2) == 0) {
        DAT_00481e10 = '\x02';
        if (DAT_00481e11 == '\0') {
          DAT_00481e11 = '\x03';
        }
      }
      else if (DAT_00481e10 == '\0') {
        DAT_00481e10 = '\x03';
      }
    }
  }
  pcVar7 = pcVar7 + 1;
LAB_0041cc0c:
  uVar5 = 0;
  do {
    if (pcVar7[uVar5] != '\0') {
      uVar3 = uVar5 * 2 & 0xff;
      if ((DAT_00495c44 + uVar3 & 2) == 0) {
        (&DAT_00481e11)[uVar3] = 4;
        if ((&DAT_00481e12)[uVar3] == '\0') {
          (&DAT_00481e12)[uVar3] = 3;
        }
      }
      else {
        (&DAT_00481e10)[uVar3] = 1;
        (&DAT_00481e12)[uVar3] = 2;
        if ((&DAT_00481e13)[uVar3] == '\0') {
          (&DAT_00481e13)[uVar3] = 3;
        }
      }
    }
    bVar4 = (char)uVar5 + 1;
    uVar5 = (uint)bVar4;
  } while (bVar4 != 0xf);
  if ((((DAT_00495c44 & 2) == 0) && (DAT_00495c44 < 0x1e)) && (pcVar7[0xf] != '\0')) {
    DAT_00481e2e = 1;
    return;
  }
  return;
}



/* ================= FUN_0041cdd0 @ 0041cdd0 ================= */

void FUN_0041cdd0(void)

{
  char *pcVar1;
  char *pcVar2;
  byte bVar3;
  byte bVar4;
  
  pcVar1 = &DAT_00495c5e + (uint)DAT_00481dbe + (uint)DAT_00481dbf * 0x20;
  if ((DAT_00495c44 & 1) == 0) {
    bVar4 = (DAT_00495c44 & 2) >> 1;
    bVar3 = bVar4 ^ 1;
    pcVar2 = pcVar1;
    if (((DAT_00481dbf != 0) && (*pcVar1 != '\0')) && (DAT_00481e10 = 2, bVar3 != 0)) {
      DAT_00481e10 = 3;
    }
    do {
      pcVar2 = pcVar2 + 0x20;
      if (*pcVar2 != '\0') {
        (&DAT_00481e10)[bVar4] = 1;
        (&DAT_00481e11)[bVar4] = 2;
        (&DAT_00481e12)[bVar4] = 3;
      }
      bVar4 = bVar4 + 2;
    } while (bVar4 < 0x14);
    bVar4 = DAT_00481dbe & 1;
    if (((bVar4 == 0) && (DAT_00481dbf < 10)) && (pcVar1[0x160] != '\0')) {
      DAT_00481e24 = 1;
    }
    if (DAT_00481dbe < 0x1d) {
      pcVar2 = pcVar1 + 1;
      if (((DAT_00481dbf != 0) && (bVar3 != 0)) && (*pcVar2 != '\0')) {
        DAT_00481e10 = 1;
      }
      do {
        pcVar2 = pcVar2 + 0x20;
        if (*pcVar2 != '\0') {
          (&DAT_00481e10)[bVar3] = 2;
          (&DAT_00481e11)[bVar3] = 1;
        }
        bVar3 = bVar3 + 2;
      } while (bVar3 < 0x14);
    }
    if (((bVar4 != 0) && (DAT_00481dbf < 10)) && (pcVar1[0x161] != '\0')) {
      DAT_00481e24 = 2;
      return;
    }
  }
  else {
    bVar4 = (DAT_00495c44 & 2) >> 1;
    bVar3 = bVar4 ^ 1;
    pcVar2 = pcVar1;
    if ((DAT_00481dbf != 0) && (*pcVar1 != '\0')) {
      (&DAT_00481e10)[bVar3] = 3;
    }
    do {
      pcVar2 = pcVar2 + 0x20;
      if (*pcVar2 != '\0') {
        (&DAT_00481e11)[bVar4] = 3;
      }
      bVar4 = bVar4 + 2;
    } while (bVar4 < 0x14);
    pcVar2 = pcVar1 + 1;
    if (DAT_00481dbe < 0x1d) {
      if ((DAT_00481dbf != 0) && (*pcVar2 != '\0')) {
        (&DAT_00481e10)[bVar3] = 4;
      }
      do {
        pcVar2 = pcVar2 + 0x20;
        if (*pcVar2 != '\0') {
          (&DAT_00481e10)[bVar3] = 4;
          (&DAT_00481e12)[bVar3] = 4;
        }
        bVar3 = bVar3 + 2;
      } while (bVar3 < 0x14);
    }
    if ((((DAT_00481dbe & 1) != 0) && (DAT_00481dbf < 10)) && (pcVar1[0x161] != '\0')) {
      DAT_00481e24 = 4;
    }
  }
  return;
}



/* ================= FUN_0041cfa0 @ 0041cfa0 ================= */

void FUN_0041cfa0(void)

{
  char *pcVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  char *pcVar6;
  char *pcVar5;
  
  pcVar1 = &DAT_00495c50 + (int)(char)(DAT_00481dbe - 1) + (uint)DAT_00481dbf * 0x20;
  if ((DAT_00495c44 & 1) == 0) {
    bVar3 = (DAT_00495c44 & 2) >> 1;
    bVar2 = bVar3 ^ 1;
    if (DAT_00481dbe != 0) {
      pcVar5 = pcVar1;
      if (((DAT_00481dbf != 0) && (*pcVar1 != '\0')) && (DAT_00481e10 = 2, (DAT_00495c44 & 2) != 0))
      {
        DAT_00481e10 = 3;
      }
      do {
        pcVar6 = pcVar5;
        pcVar5 = pcVar6 + 0x20;
        if (*pcVar5 != '\0') {
          (&DAT_00481e10)[bVar2] = 1;
          (&DAT_00481e11)[bVar2] = 2;
          (&DAT_00481e12)[bVar2] = 3;
        }
        bVar2 = bVar2 + 2;
      } while (bVar2 < 0x14);
      if ((((DAT_00481dbe & 1) != 0) && (DAT_00481dbf < 10)) && (pcVar6[0x40] != '\0')) {
        DAT_00481e24 = '\x01';
      }
    }
    pcVar5 = pcVar1 + 1;
    if (((DAT_00481dbf != 0) && (bVar3 != 0)) && (*pcVar5 != '\0')) {
      DAT_00481e10 = 1;
    }
    do {
      pcVar5 = pcVar5 + 0x20;
      if (*pcVar5 != '\0') {
        (&DAT_00481e10)[bVar3] = 2;
        (&DAT_00481e11)[bVar3] = 1;
      }
      bVar3 = bVar3 + 2;
    } while (bVar3 < 0x14);
    if ((((DAT_00481dbe & 1) == 0) && (DAT_00481dbf < 10)) && (pcVar1[0x161] != '\0')) {
      DAT_00481e24 = 2;
      return;
    }
  }
  else {
    bVar4 = (DAT_00495c44 & 2) >> 1;
    bVar3 = (bVar4 ^ 1) + 1;
    pcVar5 = pcVar1;
    bVar2 = bVar4;
    if (((DAT_00481dbe != 0) && (DAT_00481dbf != 0)) && (*pcVar1 != '\0')) {
      (&DAT_00481e10)[bVar4] = 4;
    }
    do {
      pcVar6 = pcVar5;
      pcVar5 = pcVar6 + 0x20;
      if (*pcVar5 != '\0') {
        (&DAT_00481e10)[bVar2] = 4;
        (&DAT_00481e12)[bVar2] = 4;
      }
      bVar2 = bVar2 + 2;
    } while (bVar2 < 0x14);
    bVar2 = DAT_00481dbe & 1;
    if (((bVar2 != 0) && (DAT_00481dbf < 10)) && (pcVar5 = pcVar6 + 0x40, *pcVar5 != '\0')) {
      DAT_00481e24 = '\x04';
    }
    if (1 < DAT_00481dbe) {
      pcVar5 = pcVar1 + -1;
      if (((DAT_00481dbf != 0) && (bVar4 != 1)) &&
         ((*pcVar5 != '\0' && ((&DAT_00481e10)[bVar4] == '\0')))) {
        (&DAT_00481e10)[bVar4] = '\x03';
      }
      do {
        pcVar5 = pcVar5 + 0x20;
        if ((*pcVar5 != '\0') && ((&DAT_00481e10)[bVar3] == '\0')) {
          (&DAT_00481e10)[bVar3] = '\x03';
        }
        bVar3 = bVar3 + 2;
      } while (bVar3 < 0x14);
    }
    if ((((bVar2 != 0) && (DAT_00481dbf < 10)) && (pcVar5[0x20] != '\0')) && (DAT_00481e24 == '\0'))
    {
      DAT_00481e24 = '\x03';
    }
  }
  return;
}



/* ================= FUN_0041d1c0 @ 0041d1c0 ================= */

void FUN_0041d1c0(void)

{
  FUN_00412dc0(100);
  DAT_00481dfc = DAT_00482c20;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0x35;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x114;
  FUN_00412dc0(100);
  DAT_00481df8 = DAT_00482c20;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0xb5;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x114;
  DAT_00481df6 = 0;
  return;
}



/* ================= FUN_0041d220 @ 0041d220 ================= */

void __cdecl
FUN_0041d220(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  byte bVar1;
  uint uVar2;
  undefined4 unaff_EBX;
  undefined4 unaff_retaddr;
  
  DAT_0048235a = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481a72 + (uint)DAT_00481a71 * 0x20);
  if (DAT_0048235a != 0xff) {
    DAT_0048235a = DAT_0048235a & 0x3f;
    DAT_00482358 = *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048235a);
  }
  DAT_00481da2 = 0x400;
  FUN_0041d5e0();
  DAT_00481da4 = 0x60;
  DAT_00481dac = 0x61b;
  FUN_0041d610(0xe4);
  uVar2 = *(byte *)((int)&DAT_00481a80 + (uint)DAT_00481a72 + (uint)DAT_00481a71 * 0x20) & 0xf;
  FUN_0040c850(0x1b,0x1a,(byte *)s__2d___0046ba60,(uint)(byte)(&DAT_0042b160)[uVar2],unaff_EBX,
               unaff_retaddr,param_1,param_2,param_3,param_4,param_5,param_6,param_7);
  DAT_00481da4 = 0x50;
  DAT_00481dac = 0x658;
  FUN_00421650((&DAT_0046ba90)[uVar2]);
  FUN_00421650((&DAT_0046ba90)[uVar2] + 1);
  DAT_00481dac = DAT_00481dac + 0x3e;
  FUN_00421650((&DAT_0046ba90)[uVar2] + 2);
  FUN_00421650((&DAT_0046ba90)[uVar2] + 3);
  DAT_00481da4 = 0x70;
  if ((DAT_0048235a & 0x80) == 0) {
    DAT_00481df6 = 1;
    *(undefined *)(DAT_00481dfc + 9) = (&DAT_0042b170)[(uint)(DAT_00482358 & 0x1f) * 2];
    *(undefined *)(DAT_00481dfc + 0x14) = (&DAT_00495790)[DAT_00482358 >> 7];
    if (((&DAT_00495af0)[DAT_0048235a] & 0x60) == 0x20) {
      uVar2 = 0x37;
      do {
        if ((((&DAT_00495af0)[uVar2] & 0x40) != 0) &&
           (((&DAT_00495af0)[uVar2] & 0x3f) == DAT_0048235a)) break;
        bVar1 = (char)uVar2 - 1;
        uVar2 = (uint)bVar1;
      } while ((bVar1 & 0x80) == 0);
      DAT_00482368 = (byte)uVar2;
      *(undefined *)(DAT_00481df8 + 9) =
           (&DAT_0042b170)[(uint)(*(byte *)((int)&DAT_004957a0 + uVar2) & 0x1f) * 2];
      *(undefined *)(DAT_00481df8 + 0x14) =
           (&DAT_00495790)[*(byte *)((int)&DAT_004957a0 + uVar2) >> 7];
      FUN_0040c850(0x14,0x1a,&DAT_0046ba58,
                   (*(byte *)((int)&DAT_004957e0 + (uint)DAT_00482368) & 7) + 1,unaff_EBX,
                   unaff_retaddr,param_1,param_2,param_3,param_4,param_5,param_6,param_7);
    }
    else {
      *(undefined1 *)(DAT_00481df8 + 9) = 0;
    }
    if (((&DAT_0042b288)[DAT_00482358 & 0x1f] & 0x20) != 0) {
      DAT_00481da4 = 0x60;
      DAT_00481dac = 0x612;
      FUN_0041d610(0xa5);
    }
    DAT_00481da4 = 0x60;
    FUN_0041d4f0(0x60e,-0x53);
    DAT_00481da4 = 0x70;
    FUN_0040c850(4,0x1a,&DAT_0046ba50,(*(byte *)((int)&DAT_004957e0 + (uint)DAT_0048235a) & 7) + 1,
                 unaff_EBX,unaff_retaddr,param_1,param_2,param_3,param_4,param_5,param_6,param_7);
    DAT_00481da5 = 1;
    DAT_00481dac = 0x688;
    DAT_00481da8 = &DAT_0042b358 + (uint)(DAT_00482358 & 0x1f) * 8;
    FUN_00421620();
    DAT_00481dac = 0x608;
    DAT_00481da8 = s_M_77_0042b2a0 + (uint)(DAT_00482358 & 0x1f) * 8;
    FUN_00421620();
    DAT_00481da5 = 0;
  }
  return;
}



/* ================= FUN_0041d4f0 @ 0041d4f0 ================= */

void __cdecl FUN_0041d4f0(short param_1,char param_2)

{
  byte bVar1;
  
  DAT_00481dac = param_1;
  FUN_00421650(param_2 + 0xf);
  FUN_00421650(param_2 + 0x10);
  DAT_00481dac = param_1 + 0x40;
  bVar1 = (*(byte *)((int)&DAT_004957e0 + (uint)DAT_0048235a) >> 3) + 1 & 0xfc;
  FUN_0041d590(bVar1,param_2);
  FUN_0041d590(bVar1 + 1,param_2);
  DAT_00481dac = param_1 + 0x80;
  FUN_0041d590(bVar1 + 2,param_2);
  FUN_0041d590(bVar1 + 3,param_2);
  return;
}



/* ================= FUN_0041d590 @ 0041d590 ================= */

void __cdecl FUN_0041d590(byte param_1,char param_2)

{
  if ((&DAT_0046ba68)[param_1] == -1) {
    FUN_00421650(0xff);
    return;
  }
  FUN_00421650((&DAT_0046ba68)[param_1] + param_2);
  return;
}



/* ================= FUN_0041d5d0 @ 0041d5d0 ================= */

void FUN_0041d5d0(void)

{
  if (DAT_00481df6 == '\0') {
    return;
  }
  FUN_0041d5e0();
  return;
}



/* ================= FUN_0041d5e0 @ 0041d5e0 ================= */

void FUN_0041d5e0(void)

{
  DAT_00481df6 = 0;
  *(undefined1 *)(DAT_00481dfc + 9) = 0;
  *(undefined1 *)(DAT_00481df8 + 9) = 0;
  FUN_0040dba0(1,0x18,0x1f,4,0x70);
  return;
}



/* ================= FUN_0041d610 @ 0041d610 ================= */

void __cdecl FUN_0041d610(byte param_1)

{
  byte bVar1;
  char local_2;
  char local_1;
  
  local_1 = '\x02';
  do {
    local_2 = '\x03';
    bVar1 = param_1;
    do {
      param_1 = bVar1 + 1;
      FUN_00421650(bVar1);
      local_2 = local_2 + -1;
      bVar1 = param_1;
    } while (local_2 != '\0');
    DAT_00481dac = DAT_00481dac + 0x3d;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_0041d650 @ 0041d650 ================= */

bool FUN_0041d650(void)

{
  uint uVar1;
  byte in_AF;
  byte in_TF;
  byte in_IF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  uint uVar2;
  undefined1 uStack00000000;
  undefined1 local_8 [4];
  
  uVar2 = (uint)(in_NT & 1) * 0x4000 | (uint)SBORROW4((int)(local_8 + 4),4) * 0x800 |
          (uint)(in_IF & 1) * 0x200 | (uint)(in_TF & 1) * 0x100 | (uint)((int)local_8 < 0) * 0x80 |
          (uint)(&uStack00000000 == (undefined1 *)0x8) * 0x40 | (uint)(in_AF & 1) * 0x10 |
          (uint)((POPCOUNT((uint)local_8 & 0xff) & 1U) == 0) * 4 |
          (uint)(local_8 + 4 < (undefined1 *)0x4) | (uint)(in_ID & 1) * 0x200000 |
          (uint)(in_VIP & 1) * 0x100000 | (uint)(in_VIF & 1) * 0x80000 | (uint)(in_AC & 1) * 0x40000
  ;
  uVar1 = uVar2 ^ 0x200000;
  return ((uint)((uVar1 & 0x4000) != 0) * 0x4000 | (uint)((uVar1 & 0x800) != 0) * 0x800 |
          (uint)((uVar1 & 0x200) != 0) * 0x200 | (uint)((uVar1 & 0x100) != 0) * 0x100 |
          (uint)((uVar1 & 0x80) != 0) * 0x80 | (uint)((uVar1 & 0x40) != 0) * 0x40 |
          (uint)((uVar1 & 0x10) != 0) * 0x10 | (uint)((uVar1 & 4) != 0) * 4 |
          (uint)((uVar1 & 1) != 0) | (uint)((uVar1 & 0x200000) != 0) * 0x200000 |
         (uint)((uVar1 & 0x40000) != 0) * 0x40000) != uVar2;
}



/* ================= FUN_0041d690 @ 0041d690 ================= */

/* WARNING: Removing unreachable block (ram,0x0041d6b0) */

uint FUN_0041d690(void)

{
  int iVar1;
  bool bVar2;
  undefined3 extraout_var;
  
  bVar2 = FUN_0041d650();
  if (CONCAT31(extraout_var,bVar2) == 0) {
    return 0;
  }
  iVar1 = cpuid_Version_info(1);
  return *(uint *)(iVar1 + 8) & 0x800000;
}



/* ================= FUN_0041d810 @ 0041d810 ================= */

void FUN_0041d810(void)

{
  byte bVar1;
  byte bVar2;
  ushort uVar3;
  
  FUN_0041b810((uint)DAT_0048327b,(uint)(DAT_00483274 & 0x3f),(uint)DAT_00481dbc);
  bVar1 = 0;
  bVar2 = (byte)(DAT_00481e60 >> 6);
  bVar2 = ((byte)(DAT_00481e52 >> 6) ^ bVar2) & 3 ^ bVar2;
  if (bVar2 != 0) {
    do {
      bVar2 = bVar2 >> 1;
      bVar1 = bVar1 + 1;
    } while (bVar2 != 0);
    if (4 < bVar1) {
      bVar1 = 4;
    }
  }
  DAT_00482350 = (uint)bVar1;
  uVar3 = (*(short *)(&DAT_0046bab0 + (uint)bVar1 * 2) - DAT_00481e60) + DAT_00481e52;
  if (0x7fff < uVar3) {
    uVar3 = 0;
  }
  uVar3 = uVar3 >> (bVar1 + 1 & 0x1f);
  if (uVar3 < 0x40) {
    DAT_00481dee = (char)uVar3;
    return;
  }
  DAT_00481dee = 0x3f;
  return;
}



/* ================= FUN_0041d910 @ 0041d910 ================= */

void FUN_0041d910(void)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  char cVar5;
  uint uVar6;
  int iVar7;
  byte bVar8;
  byte bVar9;
  undefined4 *puVar10;
  
  uVar4 = DAT_00495f30;
  uVar3 = DAT_0048327e;
  uVar2 = DAT_0048327d;
  uVar1 = DAT_0048327b;
  uVar6 = 0x37;
  puVar10 = &DAT_00482050;
  for (iVar7 = 0xa0; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar10 = 0;
    puVar10 = puVar10 + 1;
  }
  DAT_00495c6e = 0;
  DAT_00495ef8 = 0;
  DAT_00495f30 = 1;
  do {
    bVar9 = (&DAT_00495af0)[uVar6];
    if ((((bVar9 < 0x80) && ((bVar9 & 0x40) == 0)) &&
        ((*(byte *)((int)&DAT_004957a0 + uVar6) & 0x80) != DAT_00495c63)) &&
       ((*(byte *)((int)&DAT_00495ab0 + uVar6) & 0xe0) == 0)) {
      DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar6) & 0x1f;
      DAT_0048327e = bVar9 & 0x1f;
      DAT_0048327b = (char)uVar6;
      FUN_004015c0();
    }
    bVar9 = (char)uVar6 - 1;
    uVar6 = (uint)bVar9;
  } while (bVar9 < 0x80);
  puVar10 = &DAT_00482050;
  bVar9 = 0;
  DAT_0048234c = DAT_0048234c & 0xff;
  cVar5 = DAT_00495c03;
  do {
    bVar8 = DAT_00495c02 - 1;
    do {
      uVar6 = DAT_0048234c;
      if (*(char *)((uint)bVar8 + (int)puVar10) != '\0') {
        bVar9 = bVar9 + 1;
        if ((bVar9 == 0) && (DAT_0048234c = DAT_0048234c + 1, (char)DAT_0048234c == '\0')) {
          DAT_0048234c = uVar6 - 0xff;
        }
      }
      bVar8 = bVar8 - 1;
    } while (bVar8 < 0x80);
    puVar10 = puVar10 + 8;
    cVar5 = cVar5 + -1;
  } while (cVar5 != '\0');
  DAT_0048234c = (uint)bVar9 + (DAT_0048234c & 0xff00);
  uVar6 = DAT_00481df2 - DAT_0048234c;
  DAT_00482348 = uVar6;
  if (uVar6 < 0x8000) {
    if (0x3f < uVar6) {
      uVar6 = 0x40;
    }
  }
  else {
    uVar6 = 0;
  }
  DAT_00481def = (char)uVar6;
  DAT_0048327d = uVar2;
  DAT_0048327e = uVar3;
  DAT_0048327b = uVar1;
  DAT_00495f30 = uVar4;
  return;
}



/* ================= FUN_0041da80 @ 0041da80 ================= */

void FUN_0041da80(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined1 uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  undefined1 uVar7;
  undefined1 uVar8;
  byte bVar9;
  undefined1 uVar10;
  undefined1 uVar11;
  undefined1 uVar12;
  char cVar13;
  bool bVar14;
  undefined3 extraout_var;
  byte bVar15;
  byte bVar16;
  sbyte sVar17;
  int iVar18;
  uint uVar19;
  undefined4 *puVar20;
  byte local_1;
  
  uVar12 = DAT_00495f30;
  uVar11 = DAT_0048327e;
  uVar10 = DAT_0048327d;
  bVar9 = DAT_0048327c;
  uVar8 = DAT_0048327b;
  uVar7 = DAT_00483277;
  uVar6 = DAT_00483276;
  uVar5 = DAT_00483275;
  uVar4 = DAT_00483274;
  bVar3 = DAT_00481fdb;
  bVar2 = DAT_00481fda;
  DAT_00483274 = DAT_0048327b;
  DAT_00481df5 = 0;
  DAT_00483275 = DAT_0048327c;
  DAT_00483276 = DAT_00481fda;
  DAT_00483277 = DAT_00481fdb;
  DAT_00495c6e = 0;
  DAT_00495ef8 = 0;
  DAT_00495f30 = 1;
  local_1 = 0x37;
  do {
    uVar19 = (uint)local_1;
    if (((((&DAT_00495af0)[uVar19] & 0xc0) == 0) &&
        ((*(byte *)((int)&DAT_004957a0 + uVar19) & 0x80) != DAT_00495c63)) &&
       ((*(byte *)((int)&DAT_00495ab0 + uVar19) & 0xe0) == 0)) {
      bVar15 = *(byte *)((int)&DAT_004957a0 + uVar19) & 0x1f;
      DAT_0048327b = local_1;
      bVar16 = (&DAT_0042b288)[bVar15];
      if ((bVar16 & 0x10) != 0) {
        if (((&DAT_0042b288)[DAT_00483275] & 0x40) == 0) {
          bVar1 = bVar16 & 1;
        }
        else {
          bVar1 = bVar16 & 2;
        }
        if (bVar1 != 0) {
          DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar19) & 0x1f;
          DAT_0048327e = (&DAT_00495af0)[uVar19] & 0x1f;
          DAT_0048327c = bVar15;
          if ((bVar16 & 8) == 0) {
            if (DAT_00481dbc != 1) {
              cVar13 = FUN_004179f0(DAT_00483276,DAT_00483277);
              if (((byte)(cVar13 - 1U) == (&DAT_0042b218)[bVar15]) ||
                 ((byte)(cVar13 - 1U) < (byte)(&DAT_0042b218)[bVar15])) {
                puVar20 = &DAT_00482050;
                for (iVar18 = 0xa0; iVar18 != 0; iVar18 = iVar18 + -1) {
                  *puVar20 = 0;
                  puVar20 = puVar20 + 1;
                }
                cVar13 = '\x06';
                FUN_004015c0();
                DAT_00481fda = DAT_00483276;
                DAT_00481fdb = DAT_00483277;
                FUN_00419810();
                do {
                  bVar14 = FUN_00419830();
                  if ((CONCAT31(extraout_var,bVar14) != 1) &&
                     (*(char *)((int)&DAT_00482050 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20)
                      != '\0')) goto LAB_0041dc2f;
                  cVar13 = cVar13 + -1;
                } while (cVar13 != '\0');
              }
            }
          }
          else if ((DAT_00481dbc < 0x80) &&
                  (FUN_004179f0(DAT_00483276,DAT_00483277), DAT_00482348 != 1)) {
            if (((&DAT_0042b288)[DAT_00483275] & 0x40) == 0) {
              bVar16 = (&DAT_0042b1e8)[DAT_0048327c];
            }
            else {
              bVar16 = (&DAT_0042b200)[DAT_0048327c];
            }
            if (DAT_00482348 <= bVar16) {
LAB_0041dc2f:
              if (((&DAT_0042b288)[DAT_00483275] & 0x40) == 0) {
                bVar16 = (&DAT_0042b1a0)[DAT_0048327c];
              }
              else {
                bVar16 = (&DAT_0042b1b8)[DAT_0048327c];
              }
              sVar17 = 5;
              if (((&DAT_0042b288)[DAT_0048327c] & 8) != 0) {
                sVar17 = 4;
              }
              DAT_0048234c = ((*(byte *)((int)&DAT_004957e0 + (uint)DAT_00483275) & 7) + 1) *
                             (uint)bVar16 >> sVar17;
              if (DAT_00481df5 + DAT_0048234c < 0x100) {
                DAT_00481df5 = DAT_00481df5 + (char)DAT_0048234c;
              }
              else {
                DAT_00481df5 = 0xff;
              }
            }
          }
        }
      }
    }
    local_1 = local_1 - 1;
    if (0x7f < local_1) {
      DAT_00495f30 = uVar12;
      DAT_00481fdb = bVar3;
      DAT_00481fda = bVar2;
      DAT_0048327c = bVar9;
      DAT_00483275 = uVar5;
      DAT_0048327d = uVar10;
      DAT_0048327e = uVar11;
      DAT_0048327b = uVar8;
      DAT_00483276 = uVar6;
      DAT_00483277 = uVar7;
      DAT_00483274 = uVar4;
      return;
    }
  } while( true );
}



/* ================= FUN_0041ddd0 @ 0041ddd0 ================= */

void FUN_0041ddd0(void)

{
  byte bVar1;
  byte local_2 [2];
  
  if (DAT_00481de6 == '\0') {
    return;
  }
  local_2[0] = DAT_004834b6;
  if (DAT_0046bac8 == '\0') {
    local_2[1] = DAT_004834b6;
  }
  else {
    local_2[1] = DAT_004834b7;
  }
  bVar1 = local_2[DAT_00495c63 >> 7];
  if (bVar1 == 0) {
    DAT_00481ded = '\0';
    DAT_00481de5 = 0;
    DAT_00481de4 = '\0';
  }
  else if (bVar1 == DAT_00481dea) {
    DAT_00481de5 = DAT_00481de5 - 1;
    if ((DAT_00481de5 & 0x80) != 0) {
      if (DAT_00481ded == '\0') goto LAB_0041de7f;
      DAT_00481de5 = DAT_00481deb;
      goto LAB_0041dea4;
    }
  }
  else {
LAB_0041de7f:
    if (DAT_00481de4 == '\0') {
      DAT_00481de4 = '\x0f';
      DAT_00481ded = '\x01';
      DAT_00481de5 = DAT_00481dec;
LAB_0041dea4:
      DAT_00481de8 = bVar1;
      goto LAB_0041debf;
    }
  }
  DAT_00481de8 = 0;
LAB_0041debf:
  DAT_00481de7 = DAT_00481dea;
  if (DAT_00481de4 != '\0') {
    DAT_00481de4 = DAT_00481de4 + -1;
  }
  DAT_00481de9 = ~DAT_00481dea & bVar1;
  DAT_00481dea = bVar1;
  return;
}



/* ================= FUN_0041dee0 @ 0041dee0 ================= */

void FUN_0041dee0(void)

{
  DAT_00481dec = 0x17;
  DAT_00481deb = 2;
  return;
}



/* ================= FUN_0041def0 @ 0041def0 ================= */

void FUN_0041def0(void)

{
  DAT_00481dec = 0x17;
  DAT_00481deb = 7;
  return;
}



/* ================= FUN_0041df00 @ 0041df00 ================= */

void FUN_0041df00(void)

{
  FUN_0041df20(0x6000);
  return;
}



/* ================= FUN_0041df10 @ 0041df10 ================= */

void FUN_0041df10(void)

{
  FUN_0041df20(0x7000);
  return;
}



/* ================= FUN_0041df20 @ 0041df20 ================= */

void __cdecl FUN_0041df20(short param_1)

{
  byte bVar1;
  byte bVar2;
  char local_1;
  
  DAT_00482338 = (uint)*(ushort *)(&DAT_0046bad0 + (uint)DAT_00495c46 * 2);
  bVar2 = (&DAT_0046bad8)[DAT_00495c46];
  DAT_0048234c = 2;
  do {
    FUN_0040da00((short)DAT_00482338);
    local_1 = '\a';
    do {
      bVar1 = bVar2;
      FUN_0040daf0((ushort)(byte)(&DAT_0046bb60)[bVar1] + param_1 + 0x400);
      local_1 = local_1 + -1;
      bVar2 = bVar1 + 1;
    } while (local_1 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    bVar2 = bVar1 + 2;
    DAT_0048234c = DAT_0048234c + -1;
  } while (DAT_0048234c != 0);
  return;
}



/* ================= FUN_0041dfb0 @ 0041dfb0 ================= */

byte __cdecl FUN_0041dfb0(byte param_1,undefined2 param_2)

{
  byte bVar1;
  char local_1;
  
  FUN_0040da00(param_2);
  local_1 = '!';
  do {
    bVar1 = param_1 + 1;
    FUN_0040daf0((byte)(&DAT_0046bae0)[param_1] + 0x5400);
    local_1 = local_1 + -1;
    param_1 = bVar1;
  } while (local_1 != '\0');
  return bVar1;
}



/* ================= FUN_0041e000 @ 0041e000 ================= */

byte __cdecl FUN_0041e000(byte param_1,undefined2 param_2)

{
  byte bVar1;
  char local_1;
  
  FUN_0040da00(param_2);
  local_1 = '\x1a';
  do {
    bVar1 = param_1 + 1;
    FUN_0040daf0((byte)(&DAT_0046bb28)[param_1] + 0x5400);
    local_1 = local_1 + -1;
    param_1 = bVar1;
  } while (local_1 != '\0');
  return bVar1;
}



/* ================= FUN_0041e050 @ 0041e050 ================= */

void FUN_0041e050(void)

{
  byte bVar1;
  ushort uVar2;
  char cVar3;
  byte bVar4;
  char local_1;
  
  bVar1 = FUN_0041dfb0(0,0x40);
  cVar3 = '\x1f';
  FUN_0041dfb0(bVar1,0x700);
  FUN_0040da00(0x5c1);
  do {
    FUN_0040daf0(0x5409);
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  FUN_0040dae0();
  bVar1 = FUN_0041e000(0,0x80);
  bVar4 = 0;
  FUN_0041e000(bVar1,0xa0);
  FUN_0040dad0();
  DAT_00482338 = 0x61;
  DAT_00482348 = 0x6400;
  do {
    FUN_0040da00((short)DAT_00482338);
    local_1 = '\b';
    do {
      if ((&DAT_0046bb60)[bVar4] == 0xff) {
        uVar2 = 0x90;
      }
      else {
        uVar2 = (ushort)(byte)(&DAT_0046bb60)[bVar4] + (short)DAT_00482348;
      }
      bVar4 = bVar4 + 1;
      FUN_0040daf0(uVar2);
      local_1 = local_1 + -1;
    } while (local_1 != '\0');
    DAT_00482338 = DAT_00482338 + 0x40;
    if (bVar4 == 0x60) {
      DAT_00482348 = 0x7400;
    }
  } while (bVar4 != 0xe0);
  return;
}



/* ================= FUN_0041e150 @ 0041e150 ================= */

void FUN_0041e150(void)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  undefined4 unaff_EDI;
  undefined4 *puVar7;
  int iVar8;
  uint local_c;
  uint local_8;
  uint local_4;
  
  if (DAT_00481da0 != 0xff) {
    DAT_0048327b = (&DAT_00495c70)[DAT_00481da0];
    bVar1 = (&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x1f];
    puVar7 = (undefined4 *)&DAT_00495c70;
    for (iVar4 = 0xa0; iVar4 != 0; iVar4 = iVar4 + -1) {
      *puVar7 = 0;
      puVar7 = puVar7 + 1;
    }
    iVar8 = 0;
    iVar4 = 0;
    do {
      local_c = (uint)DAT_0048327d;
      local_8 = (uint)DAT_0048327e;
      FUN_00403490(&local_c,(int *)&local_8,iVar4);
      if (((local_c < DAT_00495c02) && (local_8 < DAT_00495c03)) &&
         (iVar5 = local_8 * 0x20 + local_c, (*(byte *)((int)&DAT_00481a80 + iVar5) & 0xf) < 2)) {
        bVar2 = *(byte *)((int)&DAT_00495820 + iVar5);
        bVar3 = false;
        if (bVar2 == 0xff) goto LAB_0041e272;
        if ((bVar2 & 0x80) == DAT_00495c63) {
          local_4 = (uint)(*(byte *)((int)&DAT_004957a0 + (uint)(bVar2 & 0x3f)) & 0x1f);
          if ((((&DAT_0042b288)[local_4] & 0x20) != 0) &&
             (((&DAT_00495af0)[bVar2 & 0x3f] & 0x60) != 0x20)) {
            if (((&DAT_0042b288)[local_4] & 1) == 0) {
              if ((bVar1 & 0x80) != 0) goto LAB_0041e272;
            }
            else if ((bVar1 & 0x40) == 0) {
LAB_0041e272:
              bVar3 = true;
            }
          }
        }
        if (bVar3) {
          iVar8 = iVar8 + 1;
          (&DAT_00495c70)[iVar5] = (char)iVar4 + '\x01';
          DAT_00481e6c = (char)local_c - DAT_00495c05;
          DAT_00481e6d = (char)local_8 - DAT_00495c06;
          FUN_0041a710();
        }
      }
      bVar2 = DAT_0048327e;
      iVar4 = iVar4 + 1;
    } while (iVar4 < 6);
    if (iVar8 != 0) {
      uVar6 = (uint)DAT_0048327b;
      *(byte *)((int)&DAT_00495ab0 + uVar6) = DAT_0048327d;
      (&DAT_00495af0)[uVar6] = bVar2;
      *(byte *)((int)&DAT_004957a0 + uVar6) =
           *(byte *)((int)&DAT_004957a0 + uVar6) & 0x7f | DAT_00495c63;
      FUN_0040a400();
      DAT_00481a68 = 1;
      DAT_00495c6d = 1;
      DAT_00483270 = FUN_0041e330;
      return;
    }
    FUN_00401a60(9,unaff_EDI);
  }
  DAT_00483270 = (code *)&LAB_004165b0;
  return;
}



/* ================= FUN_0041e330 @ 0041e330 ================= */

void FUN_0041e330(void)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  undefined4 unaff_EBX;
  undefined1 local_8;
  
  bVar3 = DAT_00481a72;
  bVar2 = DAT_00481a71;
  if ((DAT_00481de9 & 0x20) == 0) {
    if ((DAT_00481de9 & 0x40) != 0) {
      DAT_0048235a = FUN_00419e50();
      *(byte *)((int)&DAT_00495ab0 + (uint)DAT_0048327b) =
           *(byte *)((int)&DAT_00495ab0 + (uint)DAT_0048327b) | DAT_0048235a << 5;
      FUN_0041ae00();
      FUN_0040a0f0();
      DAT_00495c6d = 0;
      DAT_00483270 = &LAB_004165b0;
    }
    return;
  }
  bVar1 = (&DAT_00495c70)[(uint)DAT_00481a72 + (uint)DAT_00481a71 * 0x20];
  if (bVar1 - 1 == -1) {
    FUN_00401a60(2,unaff_EBX);
    return;
  }
  FUN_0040a420();
  DAT_00495f05 = 1;
  *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_004957a0 + (uint)DAT_0048327b) & 0x7f | DAT_00495c63;
  local_8 = (undefined1)(bVar1 - 1);
  DAT_00495f10 = local_8;
  FUN_004012f0();
  DAT_0048327d = bVar3;
  DAT_0048327e = bVar2;
  DAT_00483270 = &LAB_0041e410;
  return;
}



/* ================= FUN_0041e4e0 @ 0041e4e0 ================= */

void FUN_0041e4e0(void)

{
  FUN_00404940(&LAB_0041e4f0,100);
  return;
}



/* ================= FUN_0041e5d0 @ 0041e5d0 ================= */

void FUN_0041e5d0(void)

{
  DAT_00495c8c = 1;
  DAT_00495c78 = 1;
  DAT_00495c76 = 0;
  FUN_0041e610();
  DAT_00495c77 = 0;
  DAT_00495c71 = 0;
  DAT_00495c79 = 0;
  DAT_00495c7a = 0;
  DAT_00495c8a = 0;
  return;
}



/* ================= FUN_0041e610 @ 0041e610 ================= */

void FUN_0041e610(void)

{
  FUN_00412b70(0x24,0x36,0xe4,0xd6);
  return;
}



/* ================= FUN_0041e630 @ 0041e630 ================= */

void FUN_0041e630(void)

{
  if ((DAT_00481de8 & 0x20) == 0) {
    if ((DAT_00481de8 & 0x40) == 0) {
      if ((DAT_00481de8 & 1) != 0) {
        DAT_00495c76 = DAT_00495c76 - 1;
        if ((DAT_00495c76 & 0x80) != 0) {
          DAT_00495c76 = 4;
        }
        FUN_0041e700();
        FUN_0041f110();
        return;
      }
      if ((DAT_00481de8 & 2) == 0) {
        FUN_0041e710();
        return;
      }
      DAT_00495c76 = DAT_00495c76 + 1;
      if (DAT_00495c76 == '\x05') {
        DAT_00495c76 = '\0';
      }
      FUN_0041e700();
      FUN_0041f110();
      return;
    }
  }
  else {
    if (DAT_00495c76 == 0) {
      FUN_0041f150();
      *(undefined1 **)(DAT_00482330 + 8) = &LAB_00407a80;
      return;
    }
    if (4 < DAT_00495c76) {
      return;
    }
    if (DAT_00495c76 == 1) {
      *(undefined1 **)(DAT_00482330 + 8) = &LAB_0041e750;
      return;
    }
    if (DAT_00495c76 < 4) {
      FUN_0041f150();
      *(undefined1 **)(DAT_00482330 + 8) = &LAB_0041e960;
      return;
    }
  }
  FUN_00412890(0x7c);
  DAT_00495c53 = 1;
  return;
}



/* ================= FUN_0041e700 @ 0041e700 ================= */

void FUN_0041e700(void)

{
  return;
}



/* ================= FUN_0041e710 @ 0041e710 ================= */

void FUN_0041e710(void)

{
  char cVar1;
  
  if (((DAT_0048349f == '\x01') || (DAT_004814fc == '\0')) ||
     (cVar1 = (char)((int)(DAT_00480efc + -0x36 + (DAT_00480efc + -0x36 >> 0x1f & 0x1fU)) >> 5),
     DAT_00495c76 == cVar1)) {
    return;
  }
  DAT_00495c76 = cVar1;
  FUN_0041f110();
  return;
}



/* ================= FUN_0041e9e0 @ 0041e9e0 ================= */

void FUN_0041e9e0(void)

{
  *(undefined2 *)(DAT_00481ddc + 0x20) = 9;
  FUN_0041ed20(&DAT_0046bc48);
  FUN_0041ed20(&DAT_0046bc40);
  FUN_0041ed20(&DAT_0046bc50);
  FUN_0041ed20(&DAT_0046bc58);
  return;
}



/* ================= FUN_0041ea80 @ 0041ea80 ================= */

void FUN_0041ea80(void)

{
  *(undefined2 *)(DAT_00481ddc + 0x20) = 0xc;
  FUN_0041ed20(&DAT_0046bc60);
  FUN_0041ed20(&DAT_0046bc68);
  return;
}



/* ================= FUN_0041eb40 @ 0041eb40 ================= */

void FUN_0041eb40(void)

{
  *(undefined2 *)(DAT_00481ddc + 0x20) = 7;
  FUN_0041ed20(&DAT_0046bc70);
  return;
}



/* ================= FUN_0041ebf0 @ 0041ebf0 ================= */

void FUN_0041ebf0(void)

{
  *(undefined2 *)(DAT_00481ddc + 0x20) = 4;
  FUN_0041ed20(&DAT_0046bc78);
  FUN_0041ed20(&DAT_0046bc80);
  FUN_0041ed20(&DAT_0046bc88);
  FUN_0041ed20(&DAT_0046bc90);
  FUN_0041ed20(&DAT_0046bc98);
  FUN_0041ed20(&DAT_0046bca0);
  FUN_0041ed20(&DAT_0046bca8);
  FUN_0041ed20(&DAT_0046bcb0);
  return;
}



/* ================= FUN_0041ecb0 @ 0041ecb0 ================= */

void FUN_0041ecb0(void)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_00482087;
  *(undefined2 *)(&DAT_00482c3c + uVar1 * 0x18) = 0x34;
  *(undefined2 *)(&DAT_00482c40 + uVar1 * 0x18) = 0x58;
  FUN_0041ed20(&DAT_0046bcb8);
  FUN_0041ed20(&DAT_0046bcc0);
  FUN_0041ed20(&DAT_0046bcc8);
  FUN_0041ed20(&DAT_0046bcd0);
  FUN_0041ed20(&DAT_0046bcd8);
  FUN_0041ed20(&DAT_0046bce0);
  return;
}



/* ================= FUN_0041ed20 @ 0041ed20 ================= */

void __cdecl FUN_0041ed20(byte *param_1)

{
  FUN_00412dc0(0x3c);
  FUN_0041c020(DAT_00482c20,*param_1);
  *(byte *)(DAT_00482c20 + 9) = param_1[1];
  *(undefined1 *)(DAT_00482c20 + 10) = 0;
  *(ushort *)(DAT_00482c20 + 0xc) = (ushort)param_1[2];
  *(ushort *)(DAT_00482c20 + 0x10) = (ushort)param_1[3];
  (&DAT_00481dd0)[param_1[4]] = (char)((DAT_00482c20 + -0x482c30) / 0x18);
  return;
}



/* ================= FUN_0041eda0 @ 0041eda0 ================= */

void FUN_0041eda0(void)

{
  *(undefined1 **)(DAT_00481ddc + 8) = &LAB_0041ff20;
  return;
}



/* ================= FUN_0041edc0 @ 0041edc0 ================= */

void FUN_0041edc0(void)

{
  int iVar1;
  
  iVar1 = DAT_00482330;
  if ((DAT_00481de8 & 0x40) == 0) {
    if (DAT_00495c79 == '\0') {
      FUN_00409c50();
    }
    else {
      FUN_00409cb0();
    }
    iVar1 = DAT_00482330;
    if (DAT_00495b3b != '\0') {
      if ((DAT_00481de8 & 0x20) == 0) {
        return;
      }
      DAT_00482330 = DAT_00495b34;
      FUN_00404a00();
      DAT_00482330 = iVar1;
      FUN_00406a90();
      DAT_00495b3b = 0;
      return;
    }
    if (DAT_00495c78 == '\0') {
      FUN_0041f0e0();
      if ((DAT_00481de8 & 0x20) == 0) {
        return;
      }
      DAT_00495c87 = 1;
    }
    else {
      FUN_0041f0e0();
      if ((DAT_00481de8 & 0x20) == 0) {
        return;
      }
    }
    FUN_00412890(3);
    DAT_00495c70 = DAT_00495c70 + 1;
    DAT_00495c72 = 1;
    DAT_00495c71 = (&DAT_0046bce8)[DAT_00495c70];
    if (DAT_00495c71 < 0x80) {
      DAT_00495c75 = (&DAT_0046bd30)[DAT_00495c70];
      if (DAT_00495c75 == 0) {
        FUN_00406d70();
      }
      else {
        FUN_00406d00();
        FUN_00420210(DAT_00495c75);
      }
      FUN_0041f090();
      FUN_0041eda0();
      return;
    }
  }
  else {
    if (DAT_00495b3b != '\0') {
      DAT_00482330 = DAT_00495b34;
      FUN_00404a00();
      DAT_00495b3b = '\0';
    }
    DAT_00495c71 = 0xfe;
    DAT_00482330 = iVar1;
    FUN_00412890(0xc);
  }
  *(undefined **)(DAT_00481ddc + 8) = &DAT_0041e5b0;
  DAT_0048208f = 0xff;
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_0041ef30;
  return;
}



/* ================= FUN_0041f040 @ 0041f040 ================= */

void FUN_0041f040(void)

{
  DAT_00495c70 = 0;
  FUN_0041f050();
  return;
}



/* ================= FUN_0041f050 @ 0041f050 ================= */

void FUN_0041f050(void)

{
  DAT_00495c89 = 2;
  DAT_00495c8c = 1;
  DAT_00495c78 = 1;
  DAT_00495c72 = 0;
  DAT_00495c7b = 0;
  DAT_00495c71 = 0;
  DAT_00495c79 = 0;
  DAT_00495c7a = 0;
  return;
}



/* ================= FUN_0041f090 @ 0041f090 ================= */

void FUN_0041f090(void)

{
  FUN_0041f200();
  FUN_00406850((&DAT_0046bd78)[DAT_00495c70],1,0x15);
  return;
}



/* ================= FUN_0041f0c0 @ 0041f0c0 ================= */

void FUN_0041f0c0(void)

{
  DAT_00495c77 = DAT_00495c77 + '\x01';
  if (DAT_00495c77 == '\x0e') {
    DAT_00495c77 = '\0';
    *(byte *)(DAT_00481dc0 + 9) = *(byte *)(DAT_00481dc0 + 9) ^ 0xa7;
  }
  return;
}



/* ================= FUN_0041f0e0 @ 0041f0e0 ================= */

void FUN_0041f0e0(void)

{
  DAT_00495c77 = DAT_00495c77 + '\x01';
  if (DAT_00495c77 == '\x0e') {
    DAT_00495c77 = '\0';
    *(byte *)(DAT_00481de0 + 9) = *(byte *)(DAT_00481de0 + 9) ^ 0xb9;
  }
  DAT_00495c88 = 1;
  return;
}



/* ================= FUN_0041f110 @ 0041f110 ================= */

void FUN_0041f110(void)

{
  FUN_00412890(3);
  FUN_0041f120();
  return;
}



/* ================= FUN_0041f120 @ 0041f120 ================= */

void FUN_0041f120(void)

{
  DAT_00495c77 = 0;
  *(undefined2 *)(DAT_00481dc0 + 0x10) = *(undefined2 *)(&DAT_0046bdc0 + (uint)DAT_00495c76 * 2);
  *(undefined1 *)(DAT_00481dc0 + 9) = 0xa7;
  return;
}



/* ================= FUN_0041f150 @ 0041f150 ================= */

void FUN_0041f150(void)

{
  FUN_0041bf20();
  FUN_00412cd0();
  return;
}



/* ================= FUN_0041f160 @ 0041f160 ================= */

void FUN_0041f160(void)

{
  FUN_00412dc0(10);
  FUN_0041c020(DAT_00482c20,0x10);
  *(undefined1 *)(DAT_00482c20 + 9) = 0xa7;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0x4c;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x86;
  DAT_00481dc0 = DAT_00482c20;
  return;
}



/* ================= FUN_0041f1b0 @ 0041f1b0 ================= */

void FUN_0041f1b0(void)

{
  FUN_00412dc0(10);
  FUN_0041c020(DAT_00482c20,0x16);
  *(undefined1 *)(DAT_00482c20 + 9) = 0xb9;
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0x116;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x116;
  DAT_00481de0 = DAT_00482c20;
  FUN_0041f200();
  return;
}



/* ================= FUN_0041f200 @ 0041f200 ================= */

void FUN_0041f200(void)

{
  *(undefined1 *)(DAT_00481de0 + 9) = 0;
  DAT_00495c88 = 0;
  return;
}



/* ================= FUN_004200e0 @ 004200e0 ================= */

void FUN_004200e0(void)

{
  int iVar1;
  byte *pbVar2;
  
  iVar1 = *(int *)(DAT_00481ddc + 0x30);
  pbVar2 = (byte *)(iVar1 + 2);
  *(byte **)(DAT_00481ddc + 0x30) = pbVar2;
  *(ushort *)(DAT_00481ddc + 0x10) = (ushort)*pbVar2;
  if (*pbVar2 == 0xff) {
    thunk_FUN_00409a90();
    return;
  }
  *(ushort *)(DAT_00481ddc + 0x12) = (ushort)*(byte *)(iVar1 + 3);
  return;
}



/* ================= thunk_FUN_00409a90 @ 00420120 ================= */

void thunk_FUN_00409a90(void)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  
  bVar2 = DAT_00482087;
  if ((DAT_004834bc & 7) != 0) {
    return;
  }
  if (DAT_0048208f != '\n') {
    FUN_00409590();
    return;
  }
  if (DAT_00482086 == '\0') {
    DAT_0048208f = '\0';
    goto LAB_00409bad;
  }
  bVar4 = (byte)*(ushort *)(DAT_00482330 + 0x10);
  bVar3 = bVar4 & 0xf;
  if ((*(ushort *)(DAT_00482330 + 0x10) & 0xf) == 0) {
    return;
  }
  if (bVar3 == 2) {
    DAT_0048233c = 0;
    DAT_00482338 = 10;
  }
  else {
    if ((bVar4 & 0xf) < 2) {
      DAT_0048233c = 10;
    }
    else {
      if (bVar3 != 4) {
        if (3 < bVar3) {
          return;
        }
        DAT_0048233c = 0;
        DAT_00482338 = 0xfff6;
        goto LAB_00409b4d;
      }
      DAT_0048233c = 0xfff6;
    }
    bVar4 = (&DAT_00482150)[DAT_00482087] + 1;
    (&DAT_00482150)[DAT_00482087] = bVar4;
    if ((bVar4 & 1) == 0) {
      DAT_00482338 = 0xfffb;
    }
    else {
      DAT_00482338 = 5;
    }
  }
LAB_00409b4d:
  iVar1 = (uint)bVar2 * 0x18;
  *(short *)(&DAT_00482c3c + iVar1) = *(short *)(&DAT_00482c3c + iVar1) + (short)DAT_0048233c;
  *(short *)(&DAT_00482c40 + iVar1) = *(short *)(&DAT_00482c40 + iVar1) + (short)DAT_00482338;
  *(short *)(DAT_00482330 + 0x12) = *(short *)(DAT_00482330 + 0x12) + -1;
  if (*(short *)(DAT_00482330 + 0x12) == 0) {
    DAT_0048208f = '\0';
  }
  if (DAT_00482086 != '\0') {
    (&DAT_00482c39)[iVar1] = 0xb6;
    return;
  }
LAB_00409bad:
  (&DAT_00482c39)[(uint)bVar2 * 0x18] = 0;
  return;
}



/* ================= FUN_00420130 @ 00420130 ================= */

void FUN_00420130(void)

{
  byte bVar1;
  ushort uVar2;
  byte *pbVar3;
  short sVar4;
  byte bVar5;
  short sVar6;
  
  do {
    do {
      pbVar3 = *(byte **)(DAT_00481ddc + 0x34);
      *(ushort *)(DAT_00481ddc + 0x10) = (ushort)*pbVar3;
      if (*pbVar3 == 0) {
        DAT_0048208e = 0;
        *(int *)(DAT_00481ddc + 0x34) = *(int *)(DAT_00481ddc + 0x34) + 2;
        *(undefined1 **)(DAT_00482330 + 8) = &LAB_00420010;
        FUN_004200e0();
        return;
      }
      *(ushort *)(DAT_00481ddc + 0x12) = (ushort)pbVar3[1];
      *(int *)(DAT_00481ddc + 0x34) = *(int *)(DAT_00481ddc + 0x34) + 2;
      *(undefined4 *)(DAT_00482330 + 8) = 0x4201a0;
      uVar2 = *(ushort *)(DAT_00481ddc + 0x10);
      bVar5 = (byte)uVar2 & 0xf;
      bVar1 = (&DAT_00481dd0)[(short)uVar2 >> 4];
    } while ((uVar2 & 0xf) == 0);
    if (bVar5 == 2) {
      sVar4 = -2;
      sVar6 = 1;
    }
    else {
      if (bVar5 < 2) {
        sVar4 = -2;
      }
      else {
        sVar4 = 0;
      }
      sVar6 = -1;
    }
    *(short *)(&DAT_00482c3c + (uint)bVar1 * 0x18) =
         *(short *)(&DAT_00482c3c + (uint)bVar1 * 0x18) + sVar4;
    *(short *)(&DAT_00482c40 + (uint)bVar1 * 0x18) =
         *(short *)(&DAT_00482c40 + (uint)bVar1 * 0x18) + sVar6;
    *(short *)(DAT_00481ddc + 0x12) = *(short *)(DAT_00481ddc + 0x12) + -1;
  } while (*(short *)(DAT_00481ddc + 0x12) == 0);
  return;
}



/* ================= FUN_00420210 @ 00420210 ================= */

void __cdecl FUN_00420210(byte param_1)

{
  FUN_00406830(param_1,1,0x13);
  return;
}



/* ================= FUN_00420230 @ 00420230 ================= */

void FUN_00420230(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_00406830(s_lmnop_0046c2a0[uVar2],s__0046c2b0[uVar2],(&DAT_0046c2b8)[uVar2]);
    uVar2 = (uint)bVar1;
  } while (bVar1 < 5);
  return;
}



/* ================= FUN_00420270 @ 00420270 ================= */

void FUN_00420270(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_00406830(s_qrstu_0046c2a8[uVar2],s__0046c2b0[uVar2],(&DAT_0046c2b8)[uVar2]);
    uVar2 = (uint)bVar1;
  } while (bVar1 < 5);
  return;
}



/* ================= FUN_004202b0 @ 004202b0 ================= */

void FUN_004202b0(void)

{
  uint uVar1;
  byte bVar2;
  char cVar3;
  
  FUN_0040da00(0x149);
  uVar1 = 0;
  do {
    bVar2 = (char)uVar1 + 1;
    FUN_0040daf0((byte)(&DAT_0046c2c0)[uVar1] | 0x500);
    uVar1 = (uint)bVar2;
  } while (bVar2 < 0xe);
  bVar2 = 0;
  FUN_0040da00(0x182);
  do {
    uVar1 = (uint)bVar2;
    bVar2 = bVar2 + 1;
    FUN_0040daf0((byte)(&DAT_0046c2d0)[uVar1] | 0x500);
  } while (bVar2 < 0x1d);
  DAT_0048236c = 0x1c2;
  cVar3 = '\x13';
  do {
    FUN_0040da00((short)DAT_0048236c);
    FUN_0040daf0(0x501);
    FUN_0040da00((short)DAT_0048236c + 0x1c);
    FUN_0040daf0(0x526);
    DAT_0048236c = DAT_0048236c + 0x40;
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  bVar2 = 0;
  FUN_0040da00((short)DAT_0048236c);
  do {
    uVar1 = (uint)bVar2;
    bVar2 = bVar2 + 1;
    FUN_0040daf0((byte)s_______________________________0046c2f0[uVar1] | 0x500);
  } while (bVar2 < 0x1d);
  return;
}



/* ================= FUN_004203a0 @ 004203a0 ================= */

void FUN_004203a0(void)

{
  byte bVar1;
  char cVar2;
  char local_1;
  
  DAT_00482370 = 0;
  DAT_00481f94 = &DAT_0046c310;
  FUN_00418bb0();
  local_1 = '\x1c';
  do {
    cVar2 = ' ';
    FUN_0040da00((short)DAT_00482370);
    do {
      bVar1 = FUN_00418bf0();
      DAT_0048236c = (uint)bVar1;
      bVar1 = FUN_00418bf0();
      DAT_0048236c = DAT_0048236c + (uint)bVar1 * 0x100;
      FUN_0040daf0((short)DAT_0048236c + 0x600);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  FUN_00409db0();
  return;
}



/* ================= FUN_00420430 @ 00420430 ================= */

undefined4 FUN_00420430(void *param_1)

{
  int iVar1;
  tagMSG local_1c;
  
  *(undefined4 *)((int)param_1 + 0x24) = 1;
  iVar1 = GetMessageA(&local_1c,(HWND)0x0,0,0);
  while (iVar1 != 0) {
    if (local_1c.message == 0x400) {
      FUN_00420650(param_1,local_1c.wParam,local_1c.lParam);
    }
    else if (local_1c.message == 0x401) {
      FUN_00420750(param_1,local_1c.wParam);
    }
    else if (local_1c.message == 0x402) {
      FUN_00420820(param_1,local_1c.wParam);
    }
    else {
      DispatchMessageA(&local_1c);
    }
    iVar1 = GetMessageA(&local_1c,(HWND)0x0,0,0);
  }
  *(undefined4 *)((int)param_1 + 0x24) = 0;
  return 0;
}



/* ================= FUN_00420590 @ 00420590 ================= */

undefined4 __thiscall FUN_00420590(void *this,WPARAM param_1,CHAR *param_2)

{
  int iVar1;
  HFILE HVar2;
  undefined4 uVar3;
  _OFSTRUCT local_88;
  
  if (*(int *)((int)this + 0x18) != 0) {
    FUN_00420870(this);
  }
  if ((param_2 != (CHAR *)0x0) && (HVar2 = OpenFile(param_2,&local_88,0x4000), HVar2 == -1)) {
    lstrcpyA(local_88.szPathName,param_2);
    param_2 = local_88.szPathName;
  }
  if (*(int *)((int)this + 0x24) == 0) {
    uVar3 = FUN_00420650(this,param_1,(int)param_2);
  }
  else {
    iVar1 = *(int *)((int)this + 0x28);
    while (iVar1 == 0) {
      Sleep(0);
      iVar1 = *(int *)((int)this + 0x28);
    }
    *(undefined4 *)((int)this + 0x28) = 0;
    PostThreadMessageA(*(DWORD *)((int)this + 0x20),0x400,param_1,(LPARAM)param_2);
    iVar1 = *(int *)((int)this + 0x28);
    while (iVar1 == 0) {
      Sleep(0);
      iVar1 = *(int *)((int)this + 0x28);
    }
    uVar3 = *(undefined4 *)((int)this + 0x2c);
  }
  return uVar3;
}



/* ================= FUN_00420650 @ 00420650 ================= */

void __thiscall FUN_00420650(void *this,undefined4 param_1,int param_2)

{
  MCIERROR MVar1;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  int local_8;
  
  local_14 = *(undefined4 *)this;
  local_c = param_1;
  local_8 = param_2;
  MVar1 = mciSendCommandA(0,0x803,(param_2 == 0) - 1 & 0x200 | *(uint *)((int)this + 0x14) | 0x2002,
                          (DWORD_PTR)&local_14);
  *(undefined4 *)((int)this + 0x14) = 0;
  if (MVar1 == 0) {
    *(undefined4 *)((int)this + 0x18) = local_10;
  }
  *(undefined4 *)((int)this + 0x28) = 1;
  *(MCIERROR *)((int)this + 0x2c) = MVar1;
  return;
}



/* ================= FUN_004206c0 @ 004206c0 ================= */

undefined4 __thiscall FUN_004206c0(void *this,UINT param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  if (*(int *)((int)this + 0x24) == 0) {
    puVar3 = this;
    puVar4 = (undefined4 *)((int)this + 0x30);
    for (iVar2 = 5; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
    *(undefined4 *)((int)this + 0x44) = *(undefined4 *)((int)this + 0x14);
    *(undefined4 *)((int)this + 0x14) = 0;
    uVar1 = FUN_00420750(this,param_1);
    return uVar1;
  }
  iVar2 = *(int *)((int)this + 0x28);
  while (iVar2 == 0) {
    Sleep(0);
    iVar2 = *(int *)((int)this + 0x28);
  }
  puVar3 = this;
  puVar4 = (undefined4 *)((int)this + 0x30);
  for (iVar2 = 5; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar4 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar4 = puVar4 + 1;
  }
  *(undefined4 *)((int)this + 0x44) = *(undefined4 *)((int)this + 0x14);
  *(undefined4 *)((int)this + 0x14) = 0;
  *(undefined4 *)((int)this + 0x28) = 0;
  PostThreadMessageA(*(DWORD *)((int)this + 0x20),0x401,param_1,0);
  return 0;
}



/* ================= FUN_00420750 @ 00420750 ================= */

void __thiscall FUN_00420750(void *this,UINT param_1)

{
  MCIERROR MVar1;
  
  if (*(int *)((int)this + 0x18) != 0) {
    do {
      MVar1 = mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x18),param_1,
                              *(DWORD_PTR *)((int)this + 0x44),(int)this + 0x30);
    } while (MVar1 == 0x151);
    if (param_1 == 0x804) {
      *(undefined4 *)((int)this + 0x18) = 0;
    }
  }
  *(undefined4 *)((int)this + 0x28) = 1;
  return;
}



/* ================= FUN_00420820 @ 00420820 ================= */

undefined4 __thiscall FUN_00420820(void *this,DWORD_PTR param_1)

{
  MCIERROR MVar1;
  
  *(undefined4 *)((int)this + 0x2c) = 0;
  if (*(int *)((int)this + 0x18) != 0) {
    do {
      MVar1 = mciSendCommandA(*(MCIDEVICEID *)((int)this + 0x18),0x814,param_1,(int)this + 0x48);
    } while (MVar1 == 0x151);
    if (MVar1 == 0) {
      *(undefined4 *)((int)this + 0x2c) = *(undefined4 *)((int)this + 0x4c);
    }
  }
  *(undefined4 *)((int)this + 0x28) = 1;
  return *(undefined4 *)((int)this + 0x2c);
}



/* ================= FUN_00420870 @ 00420870 ================= */

void __fastcall FUN_00420870(void *param_1)

{
  *(uint *)((int)param_1 + 0x14) = *(uint *)((int)param_1 + 0x14) | 2;
  FUN_004206c0(param_1,0x808);
  *(uint *)((int)param_1 + 0x14) = *(uint *)((int)param_1 + 0x14) | 2;
  FUN_004206c0(param_1,0x804);
  return;
}



/* ================= FUN_004208a0 @ 004208a0 ================= */

void FUN_004208a0(void)

{
  do {
    thunk_FUN_00412060();
    if (DAT_00481dbe == DAT_00495c05) {
      if (DAT_00495c06 == DAT_00481dbf) {
        return;
      }
      if (DAT_00495c06 < DAT_00481dbf) {
        if (DAT_00484701 == '\0') {
          DAT_00495c45 = DAT_00495c45 + -1;
          DAT_00481e73 = 1;
          DAT_00481e69 = DAT_00481e69 + -1;
          DAT_00484701 = '\x01';
        }
        else {
          DAT_00495c45 = DAT_00495c45 + -1;
          DAT_00481dbf = DAT_00481dbf - 1;
          DAT_00484701 = '\0';
        }
      }
      else {
        if (DAT_00484701 == '\0') {
          DAT_00481e73 = 0;
          DAT_00495c45 = DAT_00495c45 + '\x01';
          goto LAB_004209ba;
        }
        DAT_00495c45 = DAT_00495c45 + '\x01';
        DAT_00481e69 = DAT_00481e69 + '\x01';
        DAT_00481dbf = DAT_00481dbf + 1;
        DAT_00484701 = '\0';
      }
    }
    else if (DAT_00495c05 < DAT_00481dbe) {
      if (DAT_00484701 == '\0') {
        DAT_00495c44 = DAT_00495c44 + -1;
        DAT_00481e68 = DAT_00481e68 + -1;
        DAT_00481e73 = 2;
        goto LAB_004209ba;
      }
      DAT_00495c44 = DAT_00495c44 + -1;
      DAT_00481dbe = DAT_00481dbe - 1;
      DAT_00484701 = '\0';
    }
    else if (DAT_00484701 == '\0') {
      DAT_00481e73 = 3;
      DAT_00495c44 = DAT_00495c44 + '\x01';
LAB_004209ba:
      DAT_00484701 = '\x01';
    }
    else {
      DAT_00495c44 = DAT_00495c44 + '\x01';
      DAT_00481dbe = DAT_00481dbe + 1;
      DAT_00481e68 = DAT_00481e68 + '\x01';
      DAT_00484701 = '\0';
    }
    FUN_00419e90();
    FUN_0040a0f0();
    FUN_00412e90();
  } while( true );
}



/* ================= FUN_004209e0 @ 004209e0 ================= */

void FUN_004209e0(void)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 unaff_EDI;
  undefined4 *puVar6;
  
  if (DAT_0048327b == -1) {
    FUN_00401a60(7,unaff_EDI);
    FUN_004211e0();
    return;
  }
  if ((DAT_00483278 & 0x40) != 0) {
    FUN_00401a60(0,unaff_EDI);
    FUN_004211e0();
    return;
  }
  if ((DAT_00483278 & 0x1f) == 0) {
    FUN_00401a60(6,unaff_EDI);
    FUN_004211e0();
    return;
  }
  if ((DAT_0048327a & 0x20) != 0) {
    FUN_00401a60(0xd,unaff_EDI);
    FUN_004211e0();
    return;
  }
  if (DAT_00495c42 != '\0') {
    DAT_00495f04 = (&DAT_0042b218)[DAT_0048327c];
  }
  puVar6 = (undefined4 *)&DAT_00495c70;
  for (iVar3 = 0xa0; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  DAT_00481954 = '\0';
  uVar2 = (uint)DAT_0048327c;
  bVar1 = (&DAT_0042b1e8)[uVar2];
  uVar4 = (uint)(byte)(&DAT_0042b200)[uVar2];
  if ((bVar1 == 1) && ((&DAT_0042b1a0)[uVar2] != '\0')) {
    FUN_00421220((uint)DAT_0048327d,(uint)DAT_0048327e,1,0);
  }
  if ((uVar4 == 1) && ((&DAT_0042b1b8)[DAT_0048327c] != '\0')) {
    FUN_00421220((uint)DAT_0048327d,(uint)DAT_0048327e,1,1);
  }
  if ((DAT_00495c42 != '\0') || (((&DAT_0042b288)[DAT_0048327c] & 0x14) == 0x14)) {
    if ((1 < bVar1) && (((&DAT_0042b1a0)[DAT_0048327c] != '\0' && (iVar3 = 2, 1 < bVar1)))) {
      do {
        iVar5 = iVar3 + 1;
        FUN_00421220((uint)DAT_0048327d,(uint)DAT_0048327e,iVar3,0);
        iVar3 = iVar5;
      } while (iVar5 <= (int)(uint)bVar1);
    }
    if (((1 < uVar4) && ((&DAT_0042b1b8)[DAT_0048327c] != '\0')) && (iVar3 = 2, 1 < uVar4)) {
      do {
        iVar5 = iVar3 + 1;
        FUN_00421220((uint)DAT_0048327d,(uint)DAT_0048327e,iVar3,1);
        iVar3 = iVar5;
      } while (iVar5 <= (int)uVar4);
    }
  }
  if (DAT_00481954 == '\0') {
    if (DAT_00495c42 != '\0') {
      FUN_00401a60(5,unaff_EDI);
    }
    FUN_004211e0();
    return;
  }
  if (DAT_00495c42 == '\0') {
    DAT_00495c46 = 1;
    FUN_0041df10();
    FUN_0040a400();
  }
  DAT_00495c6d = 1;
  DAT_00481a68 = 1;
  DAT_00483270 = &LAB_00420c10;
  return;
}



/* ================= FUN_00420cd0 @ 00420cd0 ================= */

void FUN_00420cd0(void)

{
  int iVar1;
  
  FUN_0041b810((uint)DAT_0048327b,(uint)DAT_00483274,(uint)DAT_00481dbc);
  iVar1 = FUN_00421170();
  DAT_00481e52 = (short)((iVar1 * DAT_00481e52) / 10);
  iVar1 = FUN_00421170();
  DAT_00481e60 = (short)((iVar1 * DAT_00481e60) / 10);
  iVar1 = 0x32;
  if (DAT_00481e5f < '\x02') {
    iVar1 = 0;
  }
  DAT_00481db6 = (byte)(((iVar1 + DAT_00481e5f * 100) - (int)DAT_00481e60) / 100);
  iVar1 = 0x32;
  if (DAT_00481e5e < '\x02') {
    iVar1 = 0;
  }
  if (8 < DAT_00481db6) {
    DAT_00481db6 = 0;
  }
  DAT_00481db2 = (byte)(((iVar1 + DAT_00481e5e * 100) - (int)DAT_00481e52) / 100);
  if (8 < DAT_00481db2) {
    DAT_00481db2 = 0;
  }
  DAT_00481db9 = *(byte *)((int)&DAT_00481a80 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20) &
                 0xf;
  DAT_00481dba = *(byte *)((int)&DAT_00481a80 + (uint)DAT_00483276 + (uint)DAT_00483277 * 0x20) &
                 0xf;
  DAT_00481db3 = DAT_00481db9;
  DAT_00481db4 = DAT_00481dba;
  if (DAT_00495c63 != '\0') {
    DAT_00481db3 = DAT_00481dba;
    DAT_00481db4 = DAT_00481db9;
  }
  FUN_00403b00();
  DAT_00483270 = &LAB_00420e20;
  return;
}



/* ================= FUN_00420ff0 @ 00420ff0 ================= */

int __cdecl FUN_00420ff0(uint param_1,int param_2,int param_3)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = 0;
  if ((param_3 != param_2) && (((&DAT_00495af0)[param_1] & 0x20) != 0)) {
    iVar4 = 0;
    do {
      bVar1 = (&DAT_00495af0)[iVar4];
      if (((bVar1 != 0) && ((bVar1 & 0x40) != 0)) && ((bVar1 & 0x3f) == param_1)) break;
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x38);
    iVar3 = (*(byte *)((int)&DAT_004957e0 + iVar4) & 7) + 1;
    if (param_3 < iVar3) {
      iVar2 = iVar3 - param_3;
      if (param_3 != 0) {
        *(byte *)((int)&DAT_004957e0 + iVar4) =
             (char)param_3 - 1U | *(byte *)((int)&DAT_004957e0 + iVar4) & 0xf8;
        return iVar2;
      }
      (&DAT_00495af0)[iVar4] = 0xff;
      *(undefined1 *)((int)&DAT_00495ab0 + iVar4) = 0xff;
      *(undefined1 *)((int)&DAT_004957e0 + iVar4) = 0xff;
      *(undefined1 *)((int)&DAT_004957a0 + iVar4) = 0xff;
    }
  }
  return iVar2;
}



/* ================= FUN_00421170 @ 00421170 ================= */

int FUN_00421170(void)

{
  int iVar1;
  
  iVar1 = FUN_0040c770();
  if ((0x13 < iVar1) && (iVar1 < 0x19)) {
    return 0xf;
  }
  if ((0x3b < iVar1) && (iVar1 < 0x41)) {
    return 0x14;
  }
  if ((0x4f < iVar1) && (iVar1 < 0x53)) {
    return 2;
  }
  if ((iVar1 < 0x62) || (99 < iVar1)) {
    return (0x96 - iVar1) / 10;
  }
  return 0x28;
}



/* ================= FUN_004211d0 @ 004211d0 ================= */

void __cdecl FUN_004211d0(undefined1 param_1)

{
  DAT_0048235a = param_1;
  FUN_0040a260();
  return;
}



/* ================= FUN_004211e0 @ 004211e0 ================= */

void FUN_004211e0(void)

{
  DAT_00495c6d = 0;
  FUN_0041ae00();
  if (DAT_00495c42 == '\0') {
    FUN_0040a440();
  }
  else {
    FUN_0040a420();
  }
  FUN_0041df00();
  DAT_00483270 = &LAB_004165b0;
  return;
}



/* ================= FUN_00421220 @ 00421220 ================= */

void __cdecl FUN_00421220(uint param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined1 uVar2;
  
  iVar1 = param_3;
  if ((DAT_00483278 & 0x80) != DAT_00495c63) {
    FUN_00421410(param_1,param_2,param_3);
    return;
  }
  for (; iVar1 != 0; iVar1 = iVar1 + -1) {
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 - 1;
  }
  uVar2 = (undefined1)param_3;
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    param_2 = param_2 + -1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    if ((param_1 & 1) == 0) {
      param_2 = param_2 + -1;
    }
    param_1 = param_1 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    param_2 = param_2 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 - 1;
  }
  for (; param_3 != 0; param_3 = param_3 + -1) {
    FUN_00421350(param_1,param_2,uVar2,param_4);
    if ((param_1 & 1) == 0) {
      param_2 = param_2 + -1;
    }
    param_1 = param_1 - 1;
  }
  return;
}



/* ================= FUN_00421350 @ 00421350 ================= */

void __cdecl FUN_00421350(int param_1,int param_2,undefined1 param_3,int param_4)

{
  byte bVar1;
  
  if ((((-1 < param_1) && (-1 < param_2)) && (param_1 < (int)(uint)DAT_00495c02)) &&
     (param_2 < (int)(uint)DAT_00495c03)) {
    bVar1 = *(byte *)((int)&DAT_00495820 + param_1 + param_2 * 0x20);
    if ((bVar1 != 0xff) && ((bVar1 & 0x80) != DAT_00495c63)) {
      bVar1 = *(byte *)((int)&DAT_004957a0 + (uint)(bVar1 & 0x3f)) & 0x1f;
      if (param_4 == 0) {
        if ((&DAT_0042b230)[bVar1] == '\x04') {
          return;
        }
      }
      else if ((&DAT_0042b230)[bVar1] != '\x04') {
        return;
      }
      DAT_00481e6c = (char)param_1 - DAT_00495c05;
      DAT_00481e6d = (char)param_2 - DAT_00495c06;
      (&DAT_00495c70)[param_2 * 0x20 + param_1] = param_3;
      DAT_00481954 = DAT_00481954 + '\x01';
      FUN_0041a710();
    }
  }
  return;
}



/* ================= FUN_00421410 @ 00421410 ================= */

void __cdecl FUN_00421410(uint param_1,int param_2,int param_3)

{
  int iVar1;
  undefined1 uVar2;
  
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 - 1;
  }
  uVar2 = (undefined1)param_3;
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    param_2 = param_2 + -1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    if ((param_1 & 1) == 0) {
      param_2 = param_2 + -1;
    }
    param_1 = param_1 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    param_2 = param_2 + 1;
  }
  for (iVar1 = param_3; iVar1 != 0; iVar1 = iVar1 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    if ((param_1 & 1) != 0) {
      param_2 = param_2 + 1;
    }
    param_1 = param_1 - 1;
  }
  for (; param_3 != 0; param_3 = param_3 + -1) {
    FUN_004214e0(param_1,param_2,uVar2);
    if ((param_1 & 1) == 0) {
      param_2 = param_2 + -1;
    }
    param_1 = param_1 - 1;
  }
  return;
}



/* ================= FUN_004214e0 @ 004214e0 ================= */

void __cdecl FUN_004214e0(int param_1,int param_2,undefined1 param_3)

{
  if ((((-1 < param_1) && (-1 < param_2)) && (param_1 < (int)(uint)DAT_00495c02)) &&
     (param_2 < (int)(uint)DAT_00495c03)) {
    DAT_00481e6d = (char)param_2 - DAT_00495c06;
    DAT_00481954 = DAT_00481954 + '\x01';
    (&DAT_00495c70)[param_1 + param_2 * 0x20] = param_3;
    DAT_00481e6c = (char)param_1 - DAT_00495c05;
    FUN_0041a710();
  }
  return;
}



/* ================= FUN_00421540 @ 00421540 ================= */

void __cdecl FUN_00421540(byte param_1)

{
  short sVar1;
  byte bVar2;
  
  sVar1 = DAT_00481dac;
  if (param_1 < 0x20) goto LAB_00421597;
  if (param_1 < 0x30) {
    bVar2 = (&DAT_0046cc98)[param_1];
  }
  else if (param_1 < 0x3a) {
    bVar2 = param_1 + 0x61;
  }
  else if (param_1 < 0x40) {
    bVar2 = (&DAT_0046cc8e)[param_1];
  }
  else if (param_1 < 0xa6) {
    bVar2 = param_1 | 0x20;
    if ((bVar2 < 0x61) || (0x7a < bVar2)) {
LAB_00421597:
      bVar2 = 0x90;
    }
    else {
      bVar2 = bVar2 + 0x3a;
    }
  }
  else {
    if (0xdf < param_1) goto LAB_00421597;
    bVar2 = param_1 + 0xf;
  }
  if (0xec < bVar2) {
    if (bVar2 == 0xed) {
      if (DAT_00481da5 != '\0') {
LAB_004215fc:
        sVar1 = DAT_00481dac + DAT_004834c4 + -0x41;
        goto LAB_004215c7;
      }
      bVar2 = 0xbf;
    }
    else if (bVar2 == 0xee) {
      if (DAT_00481da5 != '\0') goto LAB_004215fc;
      bVar2 = 0xf1;
    }
  }
  DAT_00481dac = DAT_00481dac + 1;
  sVar1 = DAT_004834c4 + sVar1;
LAB_004215c7:
  FUN_0040da00(sVar1);
  FUN_0040daf0(CONCAT11(DAT_00481da4,bVar2));
  return;
}



/* ================= FUN_00421620 @ 00421620 ================= */

void FUN_00421620(void)

{
  byte bVar1;
  byte *pbVar2;
  
  bVar1 = *DAT_00481da8;
  pbVar2 = DAT_00481da8;
  while (bVar1 != 0) {
    DAT_00481da8 = pbVar2 + 1;
    FUN_00421540(*pbVar2);
    pbVar2 = DAT_00481da8;
    bVar1 = *DAT_00481da8;
  }
  DAT_00481da8 = pbVar2;
  return;
}



/* ================= FUN_00421650 @ 00421650 ================= */

void __cdecl FUN_00421650(byte param_1)

{
  short sVar1;
  
  sVar1 = DAT_00481dac;
  DAT_00481dac = DAT_00481dac + 1;
  FUN_0040da00(sVar1);
  if (param_1 == 0xff) {
    FUN_0040daf0((ushort)CONCAT31((uint3)DAT_00481da4,0x90));
    return;
  }
  FUN_0040daf0((ushort)param_1 + DAT_00481da2 | (ushort)DAT_00481da4 << 8);
  return;
}



/* ================= FUN_00421810 @ 00421810 ================= */

void FUN_00421810(void)

{
  DAT_00481d9b = 0;
  FUN_004163e0();
  FUN_00404940(&LAB_00421830,100);
  return;
}



/* ================= FUN_004218c0 @ 004218c0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_004218c0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 unaff_retaddr;
  
  FUN_0040db70();
  FUN_00412cf0();
  FUN_0041bf20();
  DAT_004834ae = 1;
  FUN_0041bf80();
  FUN_0040d990();
  _DAT_00481e32 = 0x16d;
  FUN_0041be10();
  FUN_0041bf50(6);
  DAT_004834ba = 0;
  DAT_004834c0 = 0x100;
  FUN_0041def0();
  FUN_004224f0();
  DAT_00495b74 = 0x3800;
  DAT_004834c4 = 0;
  FUN_004068b0();
  FUN_00421b60();
  FUN_0040cd80(0x12,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0x16,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0xd,unaff_retaddr,param_1,param_2,param_3);
  FUN_0040cd80(0xe,unaff_retaddr,param_1,param_2,param_3);
  FUN_00422270();
  DAT_00481d80 = 1;
  DAT_00481d81 = 0;
  if (DAT_00495c70 == -1) {
    FUN_004223a0();
    FUN_00421b30();
  }
  else {
    FUN_00422230();
    FUN_00421df0();
    FUN_00421e50();
    FUN_004223a0();
    FUN_004222d0();
    FUN_00422210();
    FUN_00422040();
    FUN_00422310();
  }
  DAT_00495f38 = 0;
  FUN_00416320();
  return;
}



/* ================= FUN_00421a80 @ 00421a80 ================= */

void FUN_00421a80(void)

{
  FUN_00412890(3);
  FUN_004223a0();
  FUN_00421aa0();
  return;
}



/* ================= FUN_00421aa0 @ 00421aa0 ================= */

void FUN_00421aa0(void)

{
  DAT_00481da0 = DAT_00481d1e * '\x04' + DAT_00481d1d;
  FUN_00422040();
  *(undefined1 *)(DAT_00481d90 + 9) = 2;
  DAT_00481d9a = 0;
  if ((uint)(*(byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[DAT_00481da0]) & 0x1f) -
      (uint)DAT_00481d9e == -8) {
    return;
  }
  FUN_00406d70();
  FUN_004221f0();
  FUN_00422210();
  FUN_00422310();
  return;
}



/* ================= FUN_00421b10 @ 00421b10 ================= */

void FUN_00421b10(void)

{
  DAT_00481d9a = DAT_00481d9a + '\x01';
  if (DAT_00481d9a == '\r') {
    DAT_00481d9a = '\0';
    *(byte *)(DAT_00481d90 + 9) = *(byte *)(DAT_00481d90 + 9) ^ 2;
  }
  return;
}



/* ================= FUN_00421b30 @ 00421b30 ================= */

void FUN_00421b30(void)

{
  FUN_0040dba0(2,0x12,0x22,7,0);
  DAT_00481d8f = 1;
  FUN_00406830(0x66,2,0x17);
  return;
}



/* ================= FUN_00421b60 @ 00421b60 ================= */

void FUN_00421b60(void)

{
  DAT_00481da0 = 0;
  DAT_00481d1d = 0;
  DAT_00481d98 = 0;
  DAT_00481d1e = 0;
  DAT_00481d8f = 0;
  DAT_00481d9a = 0;
  DAT_00481d9f = (*(byte *)((int)&DAT_004957a0 + (uint)DAT_00495c70) & 0x1f) + 8;
  FUN_00421ba0();
  FUN_00421bc0();
  return;
}



/* ================= FUN_00421ba0 @ 00421ba0 ================= */

void FUN_00421ba0(void)

{
  DAT_00481d95 = 0;
  do {
    if ((&DAT_00495c70)[DAT_00481d95] == -1) {
      return;
    }
    DAT_00481d95 = DAT_00481d95 + 1;
  } while (DAT_00481d95 != 0x38);
  return;
}



/* ================= FUN_00421bc0 @ 00421bc0 ================= */

void FUN_00421bc0(void)

{
  if (DAT_00481d95 < 0xd) {
    DAT_00481d9c = 0;
    return;
  }
  DAT_00481d9c = (char)((int)(DAT_00481d95 - 0xd) >> 2) + '\x01';
  return;
}



/* ================= FUN_00421bf0 @ 00421bf0 ================= */

void FUN_00421bf0(void)

{
  byte bVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    bVar1 = (char)uVar2 + 1;
    FUN_0041c040(*(byte *)(*(int *)(&DAT_00481d20 + uVar2 * 4) + 0x14));
    FUN_00412e60(*(int **)(&DAT_00481d20 + uVar2 * 4));
    uVar2 = (uint)bVar1;
  } while (bVar1 != DAT_00481da1);
  return;
}



/* ================= FUN_00421c30 @ 00421c30 ================= */

void FUN_00421c30(void)

{
  if ((DAT_00481de8 & 1) == 0) {
    if ((DAT_00481de8 & 2) == 0) {
      if ((DAT_00481de8 & 8) == 0) {
        if (((DAT_00481de8 & 4) != 0) && ((DAT_00481d1d & 3) != 0)) {
          DAT_00481d1d = DAT_00481d1d - 1;
          FUN_00421d70();
          FUN_00421aa0();
          FUN_00422510();
          return;
        }
      }
      else if (((DAT_00481d1d & 3) != 3) && (DAT_00481da0 + 1 < (uint)DAT_00481d95)) {
        DAT_00481d1d = DAT_00481d1d + 1;
        FUN_00421d70();
        FUN_00421aa0();
        FUN_00422510();
        return;
      }
    }
    else {
      if ((DAT_00481da0 + 4 < (uint)DAT_00481d95) && (DAT_00481d1d + 4 < 0xc)) {
        DAT_00481d1d = DAT_00481d1d + 4;
        FUN_00421d70();
        FUN_00421aa0();
        FUN_00422510();
        return;
      }
      if (DAT_00481d1e != DAT_00481d9c) {
        DAT_00481d1e = DAT_00481d1e + '\x01';
        if (DAT_00481d1e == DAT_00481d9c) {
          FUN_00421db0();
          FUN_00421d70();
        }
        FUN_00421bf0();
        FUN_00421e50();
        FUN_00421a80();
        FUN_00422510();
        return;
      }
    }
  }
  else {
    if (3 < DAT_00481d1d) {
      DAT_00481d1d = DAT_00481d1d - 4;
      FUN_00421d70();
      FUN_00421aa0();
      FUN_00422510();
      return;
    }
    if (DAT_00481d1e != '\0') {
      DAT_00481d1e = DAT_00481d1e + -1;
      FUN_00421bf0();
      FUN_00421e50();
      FUN_00421a80();
      FUN_00422510();
      return;
    }
  }
  return;
}



/* ================= FUN_00421d70 @ 00421d70 ================= */

void FUN_00421d70(void)

{
  FUN_00412890(3);
  *(undefined2 *)(DAT_00481d90 + 0xc) = *(undefined2 *)(&DAT_0047e7d8 + (uint)DAT_00481d1d * 2);
  *(undefined2 *)(DAT_00481d90 + 0x10) = *(undefined2 *)(&DAT_0047e7f0 + (uint)DAT_00481d1d * 2);
  return;
}



/* ================= FUN_00421db0 @ 00421db0 ================= */

void FUN_00421db0(void)

{
  int iVar1;
  
  iVar1 = (uint)DAT_00481da0 - (uint)DAT_00481d95;
  if (iVar1 == -4) {
    DAT_00481d1d = DAT_00481d1d + -1;
    return;
  }
  if (iVar1 == -3) {
    DAT_00481d1d = DAT_00481d1d + -2;
    return;
  }
  if (iVar1 == -2) {
    DAT_00481d1d = DAT_00481d1d + -3;
  }
  return;
}



/* ================= FUN_00421df0 @ 00421df0 ================= */

void FUN_00421df0(void)

{
  FUN_00412dc0(10);
  *(undefined1 *)(DAT_00482c20 + 9) = 2;
  *(undefined1 *)(DAT_00482c20 + 10) = 0;
  FUN_0041c020(DAT_00482c20,5);
  *(undefined2 *)(DAT_00482c20 + 0xc) = 0x28;
  *(undefined2 *)(DAT_00482c20 + 0x10) = 0x50;
  DAT_00481d90 = DAT_00482c20;
  *(int *)(DAT_00482330 + 0xc) = DAT_00482c20;
  return;
}



/* ================= FUN_00421e50 @ 00421e50 ================= */

void FUN_00421e50(void)

{
  byte *pbVar1;
  byte bVar2;
  byte bVar3;
  
  bVar3 = DAT_00481d1e << 2;
  DAT_00481d98 = '\0';
  DAT_00481d94 = 0;
  DAT_00481da1 = 0;
  do {
    if ((&DAT_00495c70)[bVar3] == 0xff) {
      return;
    }
    pbVar1 = (byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[bVar3]);
    if ((*pbVar1 & 0x40) == 0) {
      FUN_00422000();
    }
    else {
      FUN_00422030();
    }
    bVar2 = *pbVar1;
    bVar3 = bVar3 + 1;
    FUN_00421ef0(*(short *)(&DAT_0047e808 + (uint)DAT_00481d94 * 2),
                 *(short *)(&DAT_0047e820 + (uint)DAT_00481d94 * 2),
                 (&DAT_0042b170)[(bVar2 & 0x1f) * 2],bVar2 & 0x1f);
    DAT_00481d94 = DAT_00481d94 + 1;
    DAT_00481d98 = DAT_00481d98 + '\x01';
  } while (DAT_00481d94 != 0xc);
  return;
}



/* ================= FUN_00421ef0 @ 00421ef0 ================= */

void __cdecl FUN_00421ef0(short param_1,short param_2,undefined1 param_3,byte param_4)

{
  undefined1 uVar1;
  uint uVar2;
  
  FUN_00412dc0(10);
  FUN_0041c020(DAT_00482c20,DAT_00481d1c);
  *(undefined1 *)(DAT_00482c20 + 9) = param_3;
  *(undefined1 *)(DAT_00482c20 + 10) = 0;
  *(short *)(DAT_00482c20 + 0xc) = param_1;
  *(short *)(DAT_00482c20 + 0x10) = param_2;
  uVar2 = (uint)DAT_00481da1;
  DAT_00481da1 = DAT_00481da1 + 1;
  *(int *)(&DAT_00481d20 + uVar2 * 4) = DAT_00482c20;
  uVar1 = (&DAT_0047e838)[param_4];
  if (param_4 == 9) {
    param_2 = param_2 + -8;
  }
  else if (param_4 == 0x11) {
    param_2 = param_2 + -3;
  }
  FUN_00412dc0(10);
  FUN_0041c020(DAT_00482c20,DAT_00481d99);
  *(undefined1 *)(DAT_00482c20 + 9) = uVar1;
  *(undefined1 *)(DAT_00482c20 + 10) = 1;
  *(short *)(DAT_00482c20 + 0xc) = param_1 + -0x20;
  *(short *)(DAT_00482c20 + 0x10) = param_2 + 0xd;
  uVar2 = (uint)DAT_00481da1;
  DAT_00481da1 = DAT_00481da1 + 1;
  *(int *)(&DAT_00481d20 + uVar2 * 4) = DAT_00482c20;
  return;
}



/* ================= FUN_00422000 @ 00422000 ================= */

void FUN_00422000(void)

{
  if (DAT_00481d9d == '\0') {
    DAT_00481d1c = 3;
    DAT_00481d99 = 8;
    return;
  }
  if (DAT_00481d9d == -0x80) {
    DAT_00481d1c = 4;
    DAT_00481d99 = 10;
    return;
  }
  FUN_00422030();
  return;
}



/* ================= FUN_00422030 @ 00422030 ================= */

void FUN_00422030(void)

{
  DAT_00481d1c = 7;
  DAT_00481d99 = 0xd;
  return;
}



/* ================= FUN_00422040 @ 00422040 ================= */

void FUN_00422040(void)

{
  uint uVar1;
  byte bVar2;
  byte local_1;
  
  FUN_004220d0();
  DAT_00481d97 = 0;
  DAT_00481d96 = DAT_00481da0 + '\x01';
  FUN_00422140();
  local_1 = 1;
  FUN_00422170(DAT_0047e850);
  uVar1 = 0;
  do {
    bVar2 = (char)uVar1 + 1;
    DAT_00481d97 = 0;
    DAT_00481d96 = (&DAT_00481d88)[uVar1];
    FUN_00422140();
    uVar1 = (uint)local_1;
    local_1 = local_1 + 1;
    FUN_00422170((&DAT_0047e850)[uVar1]);
    uVar1 = (uint)bVar2;
  } while (bVar2 != 6);
  FUN_004222d0();
  return;
}



/* ================= FUN_004220d0 @ 004220d0 ================= */

void FUN_004220d0(void)

{
  byte bVar1;
  
  bVar1 = *(byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[DAT_00481da0]) & 0x1f;
  DAT_00481d88 = (&DAT_0042b1a0)[bVar1];
  DAT_00481d89 = 0;
  if ((&DAT_0042b1a0)[bVar1] != '\0') {
    DAT_00481d89 = (&DAT_0042b1e8)[bVar1];
  }
  DAT_00481d8a = (&DAT_0042b218)[bVar1];
  DAT_00481d8b = (&DAT_0042b1b8)[bVar1];
  DAT_00481d8c = 0;
  if ((&DAT_0042b1b8)[bVar1] != '\0') {
    DAT_00481d8c = (&DAT_0042b200)[bVar1];
  }
  DAT_00481d8d = (&DAT_0042b1d0)[bVar1];
  return;
}



/* ================= FUN_00422140 @ 00422140 ================= */

void FUN_00422140(void)

{
  for (; 9 < DAT_00481d96; DAT_00481d96 = DAT_00481d96 - 10) {
    DAT_00481d97 = DAT_00481d97 + '\x01';
  }
  return;
}



/* ================= FUN_00422170 @ 00422170 ================= */

void __cdecl FUN_00422170(undefined2 param_1)

{
  byte local_1;
  
  FUN_0040da00(param_1);
  local_1 = 1;
  if (DAT_00481d97 != '\0') goto LAB_004221b7;
  FUN_0040daf0(0x7090);
  if (DAT_00481d96 == '\0') {
    FUN_0040daf0(0x70fe);
    return;
  }
  while (local_1 = local_1 - 1, (local_1 & 0x80) == 0) {
LAB_004221b7:
    FUN_0040daf0((byte)(&DAT_00481d96)[local_1] + 0x91 | 0x7000);
  }
  return;
}



/* ================= FUN_004221f0 @ 004221f0 ================= */

void FUN_004221f0(void)

{
  DAT_00481d9f = (*(byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[DAT_00481da0]) & 0x1f)
                 + 8;
  return;
}



/* ================= FUN_00422210 @ 00422210 ================= */

void FUN_00422210(void)

{
  DAT_00481d9e = DAT_00481d9f;
  FUN_00406830(DAT_00481d9f,2,0x17);
  return;
}



/* ================= FUN_00422230 @ 00422230 ================= */

void FUN_00422230(void)

{
  byte bVar1;
  uint uVar2;
  
  DAT_00481da4 = 0x70;
  DAT_00481da5 = 1;
  uVar2 = 5;
  do {
    bVar1 = (char)uVar2 - 1;
    DAT_00481da8 = (&PTR_DAT_0047e860)[uVar2];
    DAT_00481dac = *(undefined2 *)(uVar2 * 2 + 0x47e878);
    FUN_00421620();
    uVar2 = (uint)bVar1;
  } while ((bVar1 & 0x80) == 0);
  return;
}



/* ================= FUN_00422270 @ 00422270 ================= */

void FUN_00422270(void)

{
  char cVar1;
  undefined3 extraout_var;
  char cVar2;
  char local_1;
  
  DAT_00482370 = 0;
  DAT_00481f94 = &DAT_0047e888;
  FUN_00418bb0();
  local_1 = '\x1e';
  do {
    cVar2 = '(';
    FUN_0040da00((short)DAT_00482370);
    do {
      cVar1 = FUN_00418bf0();
      FUN_0040daf0((short)CONCAT31(extraout_var,cVar1) + 0x480);
      cVar2 = cVar2 + -1;
    } while (cVar2 != '\0');
    DAT_00482370 = DAT_00482370 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_004222d0 @ 004222d0 ================= */

void FUN_004222d0(void)

{
  DAT_00481da4 = 0x10;
  DAT_0048235a = (&DAT_00495c70)[DAT_00481da0];
  DAT_00481da2 = (undefined2)CONCAT31((uint3)(byte)((ushort)DAT_00481da2 >> 8),4);
  FUN_0041d4f0(0x4cd,-0x3e);
  return;
}



/* ================= FUN_00422310 @ 00422310 ================= */

void FUN_00422310(void)

{
  FUN_0040dba0(4,0x14,7,2,0);
  DAT_00481da4 = 0x70;
  DAT_00481da5 = 0;
  DAT_00481dac = 0x504;
  DAT_00481da8 = s_M_77_0042b2a0 +
                 (uint)(*(byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[DAT_00481da0]) &
                       0x1f) * 8;
  FUN_00421620();
  DAT_00481dac = 0x544;
  DAT_00481da8 = &DAT_0042b358 +
                 (uint)(*(byte *)((int)&DAT_004957a0 + (uint)(byte)(&DAT_00495c70)[DAT_00481da0]) &
                       0x1f) * 8;
  FUN_00421620();
  return;
}



/* ================= FUN_004223a0 @ 004223a0 ================= */

void FUN_004223a0(void)

{
  byte bVar1;
  byte bStack_1;
  
  if (DAT_00481d98 != DAT_00481d81) {
    bVar1 = DAT_00481d81;
    if (DAT_00481d81 < DAT_00481d98) {
      do {
        FUN_00422400(bVar1);
        bVar1 = bVar1 + 1;
      } while (DAT_00481d98 != bVar1);
    }
    else {
      bStack_1 = DAT_00481d98;
      do {
        FUN_00422460(bStack_1);
        bStack_1 = bStack_1 + 1;
      } while (DAT_00481d81 != bStack_1);
    }
    DAT_00481d81 = DAT_00481d98;
  }
  return;
}



/* ================= FUN_00422400 @ 00422400 ================= */

void __cdecl FUN_00422400(byte param_1)

{
  char cVar1;
  byte *pbVar2;
  byte *pbVar3;
  short sVar4;
  char local_1;
  
  local_1 = '\x03';
  sVar4 = *(short *)(&DAT_0047e968 + (uint)param_1 * 2);
  pbVar2 = &DAT_0047e980;
  do {
    cVar1 = '\x04';
    FUN_0040da00(sVar4);
    pbVar3 = pbVar2;
    do {
      pbVar2 = pbVar3 + 1;
      FUN_0040daf0(*pbVar3 | 0x400);
      cVar1 = cVar1 + -1;
      pbVar3 = pbVar2;
    } while (cVar1 != '\0');
    sVar4 = sVar4 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_00422460 @ 00422460 ================= */

void __cdecl FUN_00422460(byte param_1)

{
  char cVar1;
  byte *pbVar2;
  byte *pbVar3;
  short sVar4;
  char local_1;
  
  local_1 = '\x03';
  sVar4 = *(short *)(&DAT_0047e968 + (uint)param_1 * 2);
  pbVar2 = &DAT_0047e990;
  do {
    cVar1 = '\x04';
    FUN_0040da00(sVar4);
    pbVar3 = pbVar2;
    do {
      pbVar2 = pbVar3 + 1;
      FUN_0040daf0(*pbVar3 | 0x400);
      cVar1 = cVar1 + -1;
      pbVar3 = pbVar2;
    } while (cVar1 != '\0');
    sVar4 = sVar4 + 0x40;
    local_1 = local_1 + -1;
  } while (local_1 != '\0');
  return;
}



/* ================= FUN_004224f0 @ 004224f0 ================= */

void FUN_004224f0(void)

{
  FUN_00412b70(8,0xffffffe8,0x128,0xb0);
  return;
}



/* ================= FUN_00422510 @ 00422510 ================= */

void FUN_00422510(void)

{
  return;
}



/* ================= FUN_00422520 @ 00422520 ================= */

void FUN_00422520(void)

{
  uint uVar1;
  char cVar2;
  
  if ((&DAT_004834a8)[DAT_00495c63 >> 7] == '\0' && DAT_004814fc != '\0') {
    uVar1 = (DAT_00480ef8 + -8) / 0x48 + ((DAT_00480efc + -0x11) / 0x28) * 4;
    if (DAT_00480efc < 0x10) {
      if (DAT_00481d1e != 0) {
        DAT_00481d1e = DAT_00481d1e - 1;
        FUN_00421bf0();
        FUN_00421e50();
        FUN_00421a80();
        FUN_00412ab0(DAT_00480ef8,DAT_00480efc + 0x28);
      }
      FUN_00422510();
      return;
    }
    if (0x87 < DAT_00480efc) {
      if (DAT_00481d1e != DAT_00481d9c) {
        DAT_00481d1e = DAT_00481d1e + 1;
        if (DAT_00481d1e == DAT_00481d9c) {
          FUN_00421db0();
          FUN_00421d70();
        }
        FUN_00421bf0();
        FUN_00421e50();
        FUN_00421a80();
        FUN_00412ab0(DAT_00480ef8,DAT_00480efc + -0x28);
      }
      FUN_00422510();
      return;
    }
    cVar2 = (char)uVar1;
    if (DAT_00481d1d != cVar2) {
      if ((uVar1 & 0xff) + (uint)DAT_00481d1e * 4 < (uint)DAT_00481d95) {
        DAT_00481d1d = cVar2;
        FUN_00421d70();
        FUN_00421aa0();
        return;
      }
      FUN_00422510();
    }
  }
  return;
}



/* ================= FUN_00422660 @ 00422660 ================= */

void FUN_00422660(void)

{
  byte bVar1;
  
  DAT_00481d18 = 1;
  bVar1 = FUN_00404940(&LAB_00422690,100);
  if (bVar1 != 0xff) {
    *(undefined2 *)(DAT_00482334 + 0x10) = 0;
  }
  return;
}



/* ================= FUN_00422710 @ 00422710 ================= */

void __cdecl FUN_00422710(uint param_1)

{
  FUN_00422750((short)param_1 + 0x81,0xb,param_1);
  return;
}



/* ================= FUN_00422730 @ 00422730 ================= */

void __cdecl FUN_00422730(uint param_1)

{
  FUN_00422750((short)param_1 + 0xc1,10,param_1);
  return;
}



/* ================= FUN_00422750 @ 00422750 ================= */

void __cdecl FUN_00422750(short param_1,int param_2,uint param_3)

{
  if (((param_3 & 0x80) == 0) && ((int)param_3 < 0x1f)) {
    do {
      FUN_0040da00(param_1);
      param_1 = param_1 + 0x80;
      FUN_0040daf0(0x7090);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}



/* ================= FUN_00422790 @ 00422790 ================= */

void FUN_00422790(void)

{
  FUN_004227b0(1);
  return;
}



/* ================= FUN_004227a0 @ 004227a0 ================= */

void FUN_004227a0(void)

{
  FUN_004227b0(0);
  return;
}



/* ================= FUN_004227b0 @ 004227b0 ================= */

void __cdecl FUN_004227b0(byte param_1)

{
  byte bVar1;
  
  DAT_00481d18 = 1;
  bVar1 = FUN_00404940(&LAB_004227f0,100);
  if (bVar1 != 0xff) {
    *(undefined2 *)(DAT_00482334 + 0x10) = 0;
    *(ushort *)(DAT_00482334 + 0x12) = (ushort)param_1;
  }
  return;
}



/* ================= FUN_00422870 @ 00422870 ================= */

void __cdecl FUN_00422870(uint param_1)

{
  FUN_004228f0((short)param_1 + 0x81,DAT_00481d1a + (short)param_1,0xb,param_1);
  return;
}



/* ================= FUN_004228a0 @ 004228a0 ================= */

void __cdecl FUN_004228a0(uint param_1)

{
  if (*(short *)(DAT_00482330 + 0x12) == 0) {
    DAT_0048233c = DAT_00481d1a + param_1 + 0x40;
  }
  else {
    DAT_0048233c = DAT_00481d1a + param_1 + 0x1f;
  }
  FUN_004228f0((short)param_1 + 0xc1,(short)DAT_0048233c,10,param_1);
  return;
}



/* ================= FUN_004228f0 @ 004228f0 ================= */

void __cdecl FUN_004228f0(short param_1,short param_2,int param_3,uint param_4)

{
  ushort uVar1;
  
  if (((param_4 & 0x80) == 0) && ((int)param_4 < 0x1f)) {
    if (*(short *)(DAT_00482330 + 0x12) == 0) {
      param_2 = param_2 + 0x81;
    }
    FUN_0040da00(param_1);
    FUN_0040da10(param_2);
    do {
      FUN_0040da00(param_1);
      param_1 = param_1 + 0x80;
      FUN_0040da10(param_2);
      uVar1 = FUN_0040da80();
      FUN_0040daf0(uVar1);
      if (*(short *)(DAT_00482330 + 0x12) == 0) {
        param_2 = param_2 + 0x80;
      }
      else {
        param_2 = param_2 + 0x3e;
      }
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  return;
}



/* ================= FUN_00422980 @ 00422980 ================= */

void __cdecl
FUN_00422980(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
            undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12)

{
  undefined4 unaff_retaddr;
  char *pcVar1;
  
  DAT_00481d14 = 0;
  DAT_00481d04 = 0;
  DAT_00481d00 = 0;
  DAT_00481d0c = &DAT_00495bd0;
  DAT_00481d10 = &DAT_00495c10;
  if (DAT_00495c51 != 0xffffffff) {
    do {
      DAT_00481d14 = DAT_00481d14 + (uint)*DAT_00481d0c;
      DAT_00481d0c = DAT_00481d0c + 1;
      DAT_00481d04 = DAT_00481d04 + (uint)*DAT_00481d10;
      DAT_00481d10 = DAT_00481d10 + 1;
      DAT_00481d00 = DAT_00481d00 + 1;
    } while ((uint)DAT_00481d00 < DAT_00495c51 + 1);
  }
  if (DAT_00495c51 < 0x1a) {
    DAT_00481d08 = 2;
    if (DAT_00495c51 < 0xd) {
      DAT_00481d08 = 1;
    }
  }
  else {
    DAT_00481d08 = 4;
  }
  DAT_00481da4 = 0x60;
  FUN_0040c850(2,2,(byte *)s_map_2d_<_>_USER__01d_WIN__0047e9e4,
               (uint)DAT_00495c6a * 0x10 + (uint)DAT_00495c52 + 1,DAT_00495c48 + 1,unaff_retaddr,
               param_1,param_2,param_3,param_4,param_5,param_6,param_7);
  if (DAT_0042fb74 == '\0') {
    pcVar1 = s_REVOLT_0042a9c8 + ((uint)DAT_00495c6a * 0x10 + (uint)DAT_00495c52) * 8;
  }
  else {
    pcVar1 = &DAT_004814e0;
  }
  FUN_0040c820(10,2,pcVar1);
  DAT_00481da4 = 0x70;
  FUN_0040c850(2,0xf,&DAT_0047e9e0,param_3,param_4,param_5,param_6,param_7,param_8,param_9,param_10,
               param_11,param_12);
  FUN_0040c850(10,4,(byte *)s_UNION_0047e9d8,param_3,param_4,param_5,param_6,param_7,param_8,param_9
               ,param_10,param_11,param_12);
  FUN_0040c850(10,5,(byte *)s_GUICY_0047e9d0,param_3,param_4,param_5,param_6,param_7,param_8,param_9
               ,param_10,param_11,param_12);
  FUN_0040c850(8,6,(byte *)s_s__1__1d__0047e9c4,(uint)DAT_00481d08,param_3,param_4,param_5,param_6,
               param_7,param_8,param_9,param_10,param_11);
  FUN_0040c850(0x11,4,&DAT_0047e9b4,DAT_00495c51 + 1,param_3,param_4,param_5,param_6,param_7,param_8
               ,param_9,param_10,param_11);
  FUN_0040c850(0x11,5,&DAT_0047e9a8,DAT_00481d14,param_3,param_4,param_5,param_6,param_7,param_8,
               param_9,param_10,param_11);
  FUN_0040c850(0x11,6,&DAT_0047e99c,DAT_00481d04,param_3,param_4,param_5,param_6,param_7,param_8,
               param_9,param_10,param_11);
  return;
}



/* ================= FUN_00423210 @ 00423210 ================= */

short __cdecl FUN_00423210(char param_1)

{
  return (short)param_1 << 7;
}



/* ================= FUN_00423470 @ 00423470 ================= */

void __cdecl FUN_00423470(byte param_1)

{
  byte *pbVar1;
  short sVar2;
  
  *(short *)(DAT_00482330 + 0x16) = *(short *)(DAT_00482330 + 0x16) + -1;
  if (*(ushort *)(DAT_00482330 + 0x16) != 0) {
    FUN_00423510(param_1);
    return;
  }
  pbVar1 = *(byte **)(DAT_00482330 + 0x30);
  *(ushort *)(DAT_00482330 + 0x16) = (ushort)*pbVar1;
  if (*pbVar1 == 0xff) {
    FUN_00423810();
    return;
  }
  *(ushort *)(DAT_00482330 + 0x10) = (ushort)pbVar1[1] + (*(ushort *)(DAT_00482330 + 0x10) & 0x80);
  sVar2 = FUN_00423210(pbVar1[2]);
  *(short *)(DAT_00482330 + 0x18) = sVar2;
  sVar2 = FUN_00423210(pbVar1[3]);
  *(short *)(DAT_00482330 + 0x1c) = sVar2;
  *(int *)(DAT_00482330 + 0x30) = *(int *)(DAT_00482330 + 0x30) + 4;
  return;
}



/* ================= FUN_00423510 @ 00423510 ================= */

void FUN_00423510(byte param_1)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  byte bVar4;
  short sVar5;
  
  if ((((param_1 < 0x7f) && (*(char *)(*(int *)(DAT_00482330 + 0xc) + 9) != '\0')) &&
      ((*(char *)(DAT_00482330 + 0x19) != '\0' || (*(char *)(DAT_00482330 + 0x1d) != '\0')))) &&
     ((DAT_004834bc & 3) == 0)) {
    if (*(short *)(DAT_00482330 + 0x2c) < 0x80) {
      bVar1 = DAT_004822d0 & 0xf;
    }
    else {
      bVar1 = DAT_004822d0 >> 4;
    }
    DAT_00482348 = (uint)bVar1;
    if (bVar1 == 0) {
      if (DAT_004822d2 != 1) {
        DAT_004822d2 = 0;
      }
    }
    else if (bVar1 == 2) {
      if (1 < DAT_004822d2) {
        bVar4 = 0x19;
LAB_00423634:
        FUN_00412890(bVar4);
      }
    }
    else {
      if (bVar1 < 2) {
        bVar4 = 0x15;
        DAT_004822d2 = bVar1;
        goto LAB_00423634;
      }
      if (bVar1 < 4) {
        if ((DAT_00482348 <= DAT_004822d2) && (*(char *)(DAT_00482330 + 0x19) != '\0')) {
          DAT_004822d3 = DAT_004822d3 + 1;
          if (DAT_004822d3 < 0x50) {
            DAT_004822d2 = 3;
            bVar4 = 0x1a;
            goto LAB_00423634;
          }
          if (DAT_004822d3 == 0x50) {
            if ((DAT_004822d0 & 0xf) == 3) {
              DAT_004822d0 = DAT_004822d0 & 0xf0;
            }
            if ((DAT_004822d0 & 0xf0) == 0x30) {
              DAT_004822d0 = DAT_004822d0 & 0xf;
            }
            DAT_004822d2 = 0;
            DAT_004822d1 = DAT_004822d1 + -1;
            FUN_00423a40();
          }
        }
      }
      else if (DAT_00482348 <= DAT_004822d2) {
        bVar4 = 0x1b;
        goto LAB_00423634;
      }
    }
  }
  DAT_00482360 = 0;
  if (*(short *)(DAT_00482330 + 0x18) != 0) {
    FUN_004237b0(*(int *)(DAT_00482330 + 0xc),*(short *)(DAT_00482330 + 0x18));
    DAT_00482360 = DAT_00482360 + 1;
  }
  sVar5 = *(short *)(DAT_00482330 + 0x1c);
  if (sVar5 != 0) goto LAB_004236a9;
  if (DAT_00482360 == 0) goto LAB_0042376a;
  bVar1 = (byte)*(undefined2 *)(DAT_00482330 + 0x16) & 7;
  if (bVar1 == 3) {
    sVar5 = 0x80;
LAB_004236a9:
    FUN_004237e0(*(int *)(DAT_00482330 + 0xc),sVar5);
  }
  else if (bVar1 == 4) {
    sVar5 = -0x80;
    goto LAB_004236a9;
  }
  uVar3 = *(ushort *)(DAT_00482330 + 0x10);
  if ((byte)uVar3 < 0x7f) {
    uVar3 = *(ushort *)(DAT_00482330 + 0x16) & 7;
    if (uVar3 == 3) goto LAB_0042378d;
    if (uVar3 == 7) {
      FUN_00423810();
      return;
    }
    goto LAB_00423738;
  }
  bVar1 = (byte)uVar3 & 0x7f;
  if (bVar1 < 6) {
LAB_0042374f:
    if ((((byte)*(undefined2 *)(DAT_00482330 + 0x16) & 0xf) == 0xf) && ((uVar3 & 0x7f) != 0)) {
      *(short *)(DAT_00482330 + 0x10) = uVar3 + 5;
    }
  }
  else {
    if (9 < bVar1) {
      if (bVar1 < 0x1d) goto LAB_0042376a;
      if (bVar1 < 0x22) goto LAB_0042374f;
      if (0x25 < bVar1) {
        if (bVar1 < 0x39) goto LAB_0042376a;
        if (bVar1 < 0x3e) goto LAB_0042374f;
        if (0x41 < bVar1) {
          if (bVar1 < 0x55) goto LAB_0042376a;
          if (bVar1 < 0x5a) goto LAB_0042374f;
          if (0x5d < bVar1) goto LAB_0042376a;
        }
      }
    }
    uVar2 = *(ushort *)(DAT_00482330 + 0x16) & 0x1f;
    if (((uVar2 == 7) || (uVar2 == 0x17)) && ((uVar3 & 0x7f) != 0)) {
      *(short *)(DAT_00482330 + 0x10) = uVar3 - 5;
      if (((byte)*(undefined2 *)(DAT_00482330 + 0x16) & 0xf) != 3) {
LAB_0042378d:
        *(char *)(*(int *)(DAT_00482330 + 0xc) + 0x14) =
             (char)(*(short *)(DAT_00482330 + 0x20) >> 4);
        FUN_00423810();
        return;
      }
LAB_00423738:
      *(byte *)(*(int *)(DAT_00482330 + 0xc) + 0x14) =
           (byte)*(undefined2 *)(DAT_00482330 + 0x20) & 0xf;
      FUN_00423810();
      return;
    }
  }
LAB_0042376a:
  FUN_00423810();
  return;
}



/* ================= FUN_004237b0 @ 004237b0 ================= */

void __cdecl FUN_004237b0(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0xe) +
          (*(short *)(param_1 + 0xc) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0xe) = (short)iVar1;
  *(short *)(param_1 + 0xc) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_004237e0 @ 004237e0 ================= */

void __cdecl FUN_004237e0(int param_1,short param_2)

{
  int iVar1;
  
  iVar1 = (uint)*(ushort *)(param_1 + 0x12) +
          (*(short *)(param_1 + 0x10) * 0x100 + (int)param_2) * 0x100;
  *(short *)(param_1 + 0x12) = (short)iVar1;
  *(short *)(param_1 + 0x10) = (short)((uint)iVar1 >> 0x10);
  return;
}



/* ================= FUN_00423810 @ 00423810 ================= */

void FUN_00423810(void)

{
  *(byte *)(*(int *)(DAT_00482330 + 0xc) + 9) = (byte)*(undefined2 *)(DAT_00482330 + 0x10) & 0x7f;
  if (*(short *)(DAT_00482330 + 0x22) < 0x7f) {
    return;
  }
  FUN_00423990();
  return;
}



/* ================= FUN_004238d0 @ 004238d0 ================= */

void FUN_004238d0(void)

{
  if (DAT_00481dbc == '\0') {
    *(undefined2 *)(DAT_00482330 + 0x1e) = 0xa0;
  }
  else {
    *(undefined2 *)(DAT_00482330 + 0x1e) = 0xf8;
  }
  FUN_00423a40();
  *(undefined1 **)(DAT_00482330 + 8) = &LAB_00423960;
  *(short *)(DAT_00482330 + 0x2e) = *(short *)(DAT_00482330 + 0x2e) + 1;
  FUN_00423990();
  return;
}



/* ================= FUN_00423990 @ 00423990 ================= */

void FUN_00423990(void)

{
  byte bVar1;
  uint uVar2;
  
  if (*(short *)(DAT_00482330 + 0x24) == 0) {
    uVar2 = *(ushort *)(DAT_00482330 + 0x22) & 0x7f;
    if (*(char *)((int)&DAT_00482050 + uVar2) != '\0') {
      FUN_0040e5b0((byte)uVar2);
    }
    return;
  }
  FUN_0040e5b0((byte)*(undefined2 *)(DAT_00482330 + 0x22) & 0x7f);
  bVar1 = 0x21;
  if (DAT_00482071 != '\0') {
    return;
  }
  do {
    if (bVar1 == 0) {
      FUN_00423a30();
      return;
    }
    bVar1 = bVar1 - 1;
  } while (*(char *)((int)&DAT_00482050 + (uint)bVar1) == '\0');
  return;
}



/* ================= FUN_00423a30 @ 00423a30 ================= */

void FUN_00423a30(void)

{
  *(undefined **)(DAT_00482330 + 8) = &DAT_00423a20;
  return;
}



/* ================= FUN_00423a40 @ 00423a40 ================= */

void FUN_00423a40(void)

{
  if (DAT_004822d1 != '\0') {
    FUN_00412890(0x7e);
    DAT_004822d1 = '\0';
  }
  return;
}



/* ================= FUN_00423a80 @ 00423a80 ================= */

void FUN_00423a80(void)

{
  byte bVar1;
  byte bVar2;
  undefined4 *puVar3;
  uint uVar4;
  char cVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  byte bVar9;
  byte local_7;
  
  puVar3 = &DAT_00481a80;
  cVar5 = '\x14';
  do {
    bVar7 = 0x1d;
    do {
      uVar4 = (uint)bVar7;
      bVar7 = bVar7 - 1;
      *(byte *)(uVar4 + (int)puVar3) = *(byte *)(uVar4 + (int)puVar3) & 0x3f;
    } while ((bVar7 & 0x80) == 0);
    puVar3 = puVar3 + 8;
    cVar5 = cVar5 + -1;
  } while (cVar5 != '\0');
  local_7 = 0x37;
  do {
    uVar4 = (uint)local_7;
    bVar7 = (&DAT_00495af0)[uVar4];
    if ((bVar7 & 0x80) == 0) {
      if (((*(byte *)((int)&DAT_00495ab0 + uVar4) & 0xe0) == 0) && ((bVar7 & 0x40) == 0)) {
        bVar8 = *(byte *)((int)&DAT_00495ab0 + uVar4) & 0x1f;
        bVar6 = bVar7 & 0x1f;
        bVar2 = (-((*(byte *)((int)&DAT_004957a0 + uVar4) & 0x80) == DAT_00495c63) & 0xc0U) + 0x80;
        bVar9 = bVar8 - 1;
        FUN_00423bf0(bVar8,bVar7 & 0x1f,bVar2);
        bVar1 = bVar6 - 1;
        FUN_00423bf0(bVar9,bVar7 & 0x1f,bVar2);
        if ((bVar9 & 1) != 0) {
          FUN_00423bf0(bVar9,bVar1,bVar2);
        }
        bVar7 = bVar8 + 1;
        FUN_00423bf0(bVar8,bVar1,bVar2);
        if ((bVar7 & 1) != 0) {
          FUN_00423bf0(bVar7,bVar1,bVar2);
        }
        bVar1 = bVar6 + 1;
        FUN_00423bf0(bVar7,bVar6,bVar2);
        if ((bVar7 & 1) == 0) {
          FUN_00423bf0(bVar7,bVar1,bVar2);
        }
        FUN_00423bf0(bVar8,bVar1,bVar2);
        if ((bVar8 - 1 & 1) == 0) {
          FUN_00423bf0(bVar8 - 1,bVar1,bVar2);
        }
      }
    }
    local_7 = local_7 - 1;
  } while ((local_7 & 0x80) == 0);
  return;
}



/* ================= FUN_00423bf0 @ 00423bf0 ================= */

void __cdecl FUN_00423bf0(byte param_1,byte param_2,byte param_3)

{
  byte *pbVar1;
  
  if ((param_1 < DAT_00495c02) && (param_2 < DAT_00495c03)) {
    pbVar1 = (byte *)((int)&DAT_00481a80 + ((uint)param_2 << 5 | (uint)param_1));
    *pbVar1 = *pbVar1 | param_3;
  }
  return;
}



/* ================= FUN_00423c30 @ 00423c30 ================= */

void FUN_00423c30(void)

{
  char cVar1;
  int iVar2;
  byte bVar3;
  uint uVar4;
  undefined4 *puVar5;
  
  if (DAT_0042a4e0 == '\0') {
    puVar5 = &DAT_00481a80;
    for (iVar2 = 0xa0; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    }
    puVar5 = &DAT_00481a80;
    DAT_00481f94 = (&PTR_DAT_0047f750)[DAT_00495c52];
    FUN_00418bb0();
    DAT_00482374 = (uint)DAT_00495c03;
    do {
      uVar4 = 0;
      do {
        cVar1 = FUN_00418bf0();
        bVar3 = (char)uVar4 + 1;
        *(char *)(uVar4 + (int)puVar5) = cVar1;
        uVar4 = (uint)bVar3;
      } while (DAT_00495c02 != bVar3);
      puVar5 = puVar5 + 8;
      DAT_00482374 = DAT_00482374 - 1;
    } while (DAT_00482374 != 0);
  }
  return;
}



/* ================= FUN_00423d40 @ 00423d40 ================= */

void FUN_00423d40(void)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  uint uVar4;
  byte local_1;
  
  DAT_00481f99 = 0;
  local_1 = 0x37;
  while( true ) {
    uVar4 = (uint)local_1;
    iVar2 = FUN_00417ee0(local_1);
    if ((((iVar2 == 0) && (*(char *)((int)&DAT_00481fa0 + uVar4) == '\0')) &&
        (((&DAT_00495af0)[uVar4] & 0x40) == 0)) &&
       (((*(byte *)((int)&DAT_004957a0 + uVar4) & 0x40) == 0 &&
        (((&DAT_0042b288)[*(byte *)((int)&DAT_004957a0 + uVar4) & 0x1f] & 0x90) == 0x80)))) break;
    local_1 = local_1 - 1;
    if (0x7f < local_1) {
      thunk_FUN_00417af0();
      DAT_00484710 = 0xff;
      return;
    }
  }
  DAT_0048327b = local_1;
  FUN_00417f20();
  cVar3 = DAT_00495c52 * '\x02';
  if (DAT_00495c63 == '\0') {
    cVar3 = cVar3 + '\x01';
  }
  DAT_00483276 = (&DAT_0047f790)[(byte)(cVar3 * '\x02')];
  DAT_00483277 = (&DAT_0047f791)[(byte)(cVar3 * '\x02')];
  DAT_00482393 = FUN_004179f0(DAT_00483276,DAT_00483277);
  DAT_00482374 = 8;
  local_1 = DAT_00495c52 << 4;
  do {
    uVar4 = (uint)local_1;
    bVar1 = (&DAT_0047f7d0)[uVar4];
    if (0x7f < bVar1) break;
    cVar3 = *(char *)((int)&DAT_00481a80 + (uint)bVar1 + (uint)(byte)(&DAT_0047f7d1)[uVar4] * 0x20);
    if ((cVar3 == DAT_00495c43) || (cVar3 == '\f')) {
      bVar1 = FUN_004179f0(bVar1,(&DAT_0047f7d1)[uVar4]);
      if (DAT_00482393 <= bVar1) {
        DAT_00483276 = (&DAT_0047f7d0)[uVar4];
        DAT_00483277 = (&DAT_0047f7d1)[uVar4];
        DAT_00482393 = bVar1;
      }
    }
    local_1 = local_1 + 2;
    DAT_00482374 = DAT_00482374 + -1;
  } while (DAT_00482374 != 0);
  FUN_00423f80();
  return;
}



/* ================= FUN_00423eb0 @ 00423eb0 ================= */

void __cdecl FUN_00423eb0(undefined1 param_1)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  byte local_1;
  
  DAT_00481f99 = param_1;
  local_1 = 0x37;
  while( true ) {
    iVar2 = FUN_00417ee0(local_1);
    if ((iVar2 == 0) && (*(char *)((int)&DAT_00481fa0 + (uint)local_1) == '\x01')) break;
    local_1 = local_1 - 1;
    if (0x7f < local_1) {
      thunk_FUN_00417af0();
      DAT_00484710 = 0xff;
      return;
    }
  }
  DAT_0048327b = local_1;
  FUN_00417f20();
  bVar1 = *(byte *)((int)&DAT_00481ff0 + (uint)DAT_0048327b) & 7;
  if (bVar1 != 7) {
    bVar1 = (bVar1 + DAT_00495c52 * '\b') * '\x02';
    DAT_00483276 = (&DAT_0047f7d0)[bVar1];
    DAT_00483277 = (&DAT_0047f7d1)[bVar1];
    FUN_00423f80();
    return;
  }
  cVar3 = DAT_00495c52 * '\x02';
  if (DAT_00495c63 == '\0') {
    cVar3 = cVar3 + '\x01';
  }
  DAT_00483276 = (&DAT_0047f790)[(byte)(cVar3 * '\x02')];
  DAT_00483277 = (&DAT_0047f791)[(byte)(cVar3 * '\x02')];
  FUN_00423f80();
  return;
}



/* ================= FUN_00423f80 @ 00423f80 ================= */

void FUN_00423f80(void)

{
  byte bVar1;
  undefined1 uVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  char cVar6;
  bool bVar7;
  undefined3 extraout_var;
  int iVar8;
  uint uVar9;
  byte bVar10;
  undefined4 *puVar11;
  
  DAT_00482399 = '\0';
  cVar6 = DAT_00495c52 * '\x02';
  if (DAT_00495c63 == 0) {
    cVar6 = cVar6 + '\x01';
  }
  if (((&DAT_0047f790)[(byte)(cVar6 * '\x02')] == DAT_00483276) &&
     ((&DAT_0047f791)[(byte)(cVar6 * '\x02')] == DAT_00483277)) {
    DAT_00482399 = '\x01';
  }
  DAT_00482396 = '\x01';
  if ((&DAT_00495c70)[(uint)DAT_00483276 + (uint)DAT_00483277 * 0x20] == '\0') {
    DAT_00481fda = DAT_00483276;
    DAT_00481fdb = DAT_00483277;
    DAT_00482396 = '\0';
    FUN_00419810();
    DAT_00482374 = 6;
    do {
      cVar6 = 'x';
      do {
        bVar7 = FUN_00419830();
        if ((CONCAT31(extraout_var,bVar7) != 1) &&
           ((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] != '\0')) {
          DAT_00483276 = DAT_00481fda;
          DAT_00483277 = DAT_00481fdb;
          goto LAB_00424054;
        }
        cVar6 = cVar6 + -1;
      } while (cVar6 != '\0');
      DAT_00482374 = DAT_00482374 + -1;
    } while (DAT_00482374 != 0);
    goto LAB_004244b3;
  }
LAB_00424054:
  iVar8 = FUN_00419620();
  if ((iVar8 == 0) && (DAT_00482396 != '\0')) {
    bVar10 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20);
    if (bVar10 != 0xff) {
      if (((bVar10 & 0x80) == DAT_00495c63) && (DAT_00482399 != '\0')) {
        DAT_00481fde = DAT_00481fde | 0x80;
      }
      goto LAB_004240a6;
    }
    DAT_00481f99 = '\0';
    if (DAT_00482399 != '\0') {
      DAT_00481fde = DAT_00481fde | 0x80;
    }
  }
  else {
LAB_004240a6:
    if ((((&DAT_0042b288)[DAT_0048327c] & 0x20) == 0) &&
       (bVar10 = DAT_00495f30 -
                 ((byte)(&DAT_00495c70)[(uint)DAT_00483276 + (uint)DAT_00483277 * 0x20] >> 4),
       2 < bVar10)) {
      DAT_00482393 = bVar10;
      iVar8 = FUN_004199c0();
      bVar5 = DAT_00495f30;
      bVar4 = DAT_0048327e;
      bVar3 = DAT_0048327d;
      bVar10 = DAT_0048327b;
      if (iVar8 == 0) {
        if (((*(byte *)((int)&DAT_004957a0 +
                       (*(byte *)((int)&DAT_00495820 +
                                 (uint)DAT_0048235a + (uint)DAT_00482358 * 0x20) & 0x3f)) & 0x40) !=
             0) && (DAT_0048327b = bVar10, DAT_0048327d = bVar3, DAT_0048327e = bVar4,
                   DAT_00495f30 = bVar5, DAT_00482393 < 4)) goto LAB_0042431d;
        DAT_00482397 = DAT_0048235a;
        DAT_00482348 = (uint)DAT_00482358 * 0x20 + (uint)DAT_0048235a;
        DAT_00482398 = DAT_00482358;
        if ((byte)(&DAT_00495c70)[DAT_00482348] >> 4 == DAT_00495f30) {
          DAT_0048327b = *(byte *)((int)&DAT_00495820 + DAT_00482348) & 0x3f;
          uVar9 = *(byte *)((int)&DAT_00495820 + DAT_00482348) & 0x3f;
          DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar9) & 0x1f;
          DAT_0048327e = (&DAT_00495af0)[uVar9] & 0x1f;
          DAT_00495c6e = 0;
          DAT_00495ef8 = 0;
          DAT_00495f30 = (&DAT_00436558)[*(byte *)((int)&DAT_004957a0 + uVar9) & 0x1f];
          puVar11 = &DAT_00482050;
          for (iVar8 = 0xa0; iVar8 != 0; iVar8 = iVar8 + -1) {
            *puVar11 = 0;
            puVar11 = puVar11 + 1;
          }
          FUN_004015c0();
          iVar8 = (uint)DAT_00483277 * 0x20 + (uint)DAT_00483276;
          bVar1 = *(byte *)((int)&DAT_00482050 + iVar8);
          DAT_00482348 = (uint)bVar1;
          DAT_0048234c = (uint)DAT_00495f30 - (uint)(bVar1 >> 4);
          if ((DAT_00482348 != 0) &&
             (DAT_0048234c < (byte)(((byte)(&DAT_00495c70)[iVar8] >> 4) - bVar5))) {
            DAT_00481fda = DAT_00482397;
            DAT_00481fdb = DAT_00482398;
            uVar9 = *(byte *)((int)&DAT_00495820 + (uint)DAT_00482397 + (uint)DAT_00482398 * 0x20) &
                    0x3f;
            DAT_0048327b = bVar10;
            DAT_0048327d = bVar3;
            DAT_0048327e = bVar4;
            DAT_00495f30 = bVar5;
            if ((*(byte *)((int)&DAT_004957a0 + uVar9) & 0x40) == 0) {
              uVar2 = *(undefined1 *)((int)&DAT_00481ff0 + (uint)bVar10);
              *(undefined1 *)((int)&DAT_00481fa0 + uVar9) =
                   *(undefined1 *)((int)&DAT_00481fa0 + (uint)bVar10);
              *(undefined1 *)((int)&DAT_00481ff0 + uVar9) = uVar2;
            }
            goto LAB_004242c3;
          }
        }
      }
      bVar1 = *(byte *)((int)&DAT_00481ff0 + (uint)bVar10);
      DAT_0048327b = bVar10;
      DAT_0048327d = bVar3;
      DAT_0048327e = bVar4;
      DAT_00495f30 = bVar5;
      if ((bVar1 & 8) == 0) {
        *(byte *)((int)&DAT_00481ff0 + (uint)bVar10) = bVar1 | 8;
        DAT_0048239f = bVar4;
        DAT_0048239e = DAT_0048327d;
        FUN_00402cd0();
        return;
      }
    }
  }
LAB_0042431d:
  DAT_00481fdd = 0xff;
  if (((&DAT_00495af0)[DAT_0048327b] & 0x60) == 0x20) {
    DAT_00481fdd = 0;
  }
  DAT_00481dbc = 0;
  iVar8 = FUN_004198b0();
  bVar3 = DAT_00481fdb;
  bVar10 = DAT_00481fda;
  if (iVar8 != 0) {
    if (DAT_00482030 != '\x01') {
      DAT_0048239a = 0;
      if (DAT_00481f99 != '\0') {
        DAT_00481fda = DAT_0048327d;
        DAT_00481fdb = DAT_0048327e;
        FUN_0041da80();
        DAT_0048239a = DAT_00481df5;
      }
      DAT_00482390 = 1;
      DAT_00482393 = 0;
      DAT_00481fda = bVar10;
      DAT_00481fdb = bVar3;
      do {
        FUN_004179b0(4);
        DAT_00482392 = 0;
        do {
          DAT_00482391 = DAT_00482390;
          do {
            iVar8 = FUN_004198b0();
            if (((iVar8 == 0) ||
                ((DAT_00481df5 != 0 &&
                 ((DAT_00481df5 < DAT_0048239a || (DAT_0048239a == DAT_00481df5)))))) &&
               (DAT_00482393 <=
                ((&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] & 0xf))) {
              DAT_00482394 = DAT_00481fda;
              DAT_00482395 = DAT_00481fdb;
              DAT_00482393 = (&DAT_00495c70)[(uint)DAT_00481fda + (uint)DAT_00481fdb * 0x20] & 0xf;
            }
            FUN_004179b0(DAT_00482392);
            DAT_00482391 = DAT_00482391 - 1;
          } while (DAT_00482391 != 0);
          DAT_00482392 = DAT_00482392 + 1;
        } while (DAT_00482392 < 6);
        if (DAT_00482393 != 0) {
          DAT_00481fda = DAT_00482394;
          DAT_00481fdb = DAT_00482395;
          goto LAB_004242c3;
        }
        DAT_00482390 = DAT_00482390 + 1;
      } while (DAT_00482390 < 4);
    }
LAB_004244b3:
    DAT_00484710 = 0xff;
    *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) =
         *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) | 0x80;
    return;
  }
LAB_004242c3:
  bVar10 = 0;
  DAT_00481fdd = 0;
  iVar8 = FUN_00419ae0();
  if (iVar8 != 1) {
    DAT_00484711 = DAT_00481fda;
    DAT_00484712 = DAT_00481fdb;
    DAT_00484714._0_1_ = DAT_0048235a;
    DAT_00484714._1_1_ = DAT_00482358;
    DAT_00484710 = 0;
    DAT_00484713 = 10;
    uVar9 = (uint)DAT_0048327b;
    DAT_00484714._2_1_ = 0xff;
    *(byte *)((int)&DAT_00481fa0 + (uint)DAT_00483274) = *(byte *)((int)&DAT_00481fa0 + uVar9) | 8;
    *(byte *)((int)&DAT_00481fa0 + uVar9) = 8;
    return;
  }
  if ((DAT_00481fda != DAT_0048327d) || (DAT_00481fdb != DAT_0048327e)) {
    bVar10 = 3;
    DAT_00484711 = DAT_00481fda;
    DAT_00484710 = 0;
    DAT_00484712 = DAT_00481fdb;
  }
  (&DAT_00484710)[bVar10] = 0xff;
  *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) =
       *(byte *)((int)&DAT_00481fa0 + (uint)DAT_0048327b) | 8;
  return;
}



/* ================= FUN_004246e0 @ 004246e0 ================= */

void FUN_004246e0(void)

{
  byte bVar1;
  uint uVar2;
  
  bVar1 = FUN_004048e0(&LAB_004247f0,10);
  if (bVar1 != 0xff) {
    uVar2 = FUN_0041bfb0(0x25);
    *(char *)(DAT_00482c20 + 0x14) = (char)uVar2;
    DAT_00481a73 = *(char *)(DAT_00482c20 + 0x14) << 4 | 5;
    *(undefined1 *)(DAT_00482c20 + 10) = 0;
    DAT_00481a6c = DAT_00482334;
    *(undefined2 *)(DAT_00482334 + 0x10) = 0x1e;
    *(undefined2 *)(DAT_00482334 + 0x14) = 0xff;
    *(undefined2 *)(DAT_00482334 + 0x16) = 0xff;
    DAT_00481a70 = 1;
    DAT_00481a68 = 1;
    FUN_00412b70(8,0x10,0xf8,0xb0);
    DAT_00480ef8 = ((uint)DAT_00481a72 - (uint)DAT_00495c05) * 0x10 + 8;
    DAT_00480efc = (((uint)DAT_00481a71 - (uint)DAT_00495c06) + 1) * 0x10 +
                   ((uint)DAT_00495c44 + ((uint)DAT_00481a72 - (uint)DAT_00495c05) * 2 & 2) * 4;
  }
  return;
}



/* ================= FUN_004247c0 @ 004247c0 ================= */

void FUN_004247c0(void)

{
  *(undefined1 *)(*(int *)(DAT_00481a6c + 0xc) + 9) = 0xe8;
  *(undefined2 *)(DAT_00481a6c + 0x14) = 0xff;
  *(undefined2 *)(DAT_00481a6c + 0x10) = 0x1e;
  FUN_00412e90();
  return;
}



/* ================= FUN_00424ce0 @ 00424ce0 ================= */

void __cdecl FUN_00424ce0(byte param_1)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  byte bVar5;
  byte bVar6;
  byte local_1;
  
  uVar4 = (uint)param_1;
  DAT_00482391 = (&DAT_0047f8d0)[uVar4];
  DAT_00482392 = (&DAT_0047f8d4)[uVar4];
  DAT_00482390 = (&DAT_0047f8d8)[uVar4];
  DAT_00481f99 = (&DAT_0047f8dc)[uVar4];
  bVar5 = 0x37;
  DAT_00483274 = 0xff;
  DAT_00481f9a = DAT_00482391;
  do {
    uVar4 = (uint)bVar5;
    if ((((((byte)(&DAT_00495af0)[uVar4] < 0x80) &&
          ((*(byte *)((int)&DAT_004957a0 + uVar4) & 0x80) != DAT_00495c63)) &&
         (*(char *)((int)&DAT_00481ff0 + uVar4) == '\0')) &&
        (((*(byte *)((int)&DAT_00495ab0 + uVar4) & 0xe0) == 0 &&
         (((&DAT_00495af0)[uVar4] & 0x40) == 0)))) &&
       (DAT_00481f9a <= *(byte *)((int)&DAT_00481fa0 + uVar4))) {
      DAT_00481f9a = *(byte *)((int)&DAT_00481fa0 + uVar4);
      DAT_00483274 = bVar5;
    }
    bVar5 = bVar5 - 1;
  } while (bVar5 < 0x80);
  if (DAT_00483274 < 0x80) {
    uVar4 = (uint)DAT_00483274;
    DAT_00483276 = *(byte *)((int)&DAT_00495ab0 + uVar4) & 0x1f;
    DAT_00483277 = (&DAT_00495af0)[uVar4] & 0x1f;
    DAT_00483275 = *(byte *)((int)&DAT_004957a0 + uVar4) & 0x1f;
    DAT_00482393 = 0xff;
    local_1 = 0x37;
    DAT_00495c6e = 0;
    DAT_00495ef8 = 0;
    DAT_00495f30 = 1;
    DAT_00482396 = ((&DAT_0042b288)[DAT_00483275] & 0x40) >> 6;
    DAT_00481f9a = DAT_00482392;
    do {
      uVar4 = (uint)local_1;
      iVar3 = FUN_00417ee0(local_1);
      if (((iVar3 == 0) && (((&DAT_00495af0)[uVar4] & 0x60) == 0)) &&
         (*(char *)((int)&DAT_00481fa0 + uVar4) == DAT_00482390)) {
        bVar5 = *(byte *)((int)&DAT_004957a0 + uVar4) & 0x1f;
        if (DAT_00482396 == 0) {
          cVar1 = (&DAT_0042b1a0)[bVar5];
        }
        else {
          cVar1 = (&DAT_0042b1b8)[bVar5];
        }
        if (cVar1 != '\0') {
          DAT_0048327b = local_1;
          DAT_0048327d = *(byte *)((int)&DAT_00495ab0 + uVar4) & 0x1f;
          DAT_0048327e = (&DAT_00495af0)[uVar4] & 0x1f;
          if (DAT_00482396 == 0) {
            bVar2 = (&DAT_0042b1e8)[bVar5];
          }
          else {
            bVar2 = (&DAT_0042b200)[bVar5];
          }
          DAT_0048327c = bVar5;
          if (bVar2 < 2) {
            FUN_00424f70();
          }
          else {
            FUN_004251d0();
          }
        }
      }
      local_1 = local_1 - 1;
    } while (local_1 < 0x80);
    if (DAT_00482393 < 0x80) {
      bVar6 = 0;
      DAT_0048327b = DAT_00482393;
      uVar4 = (uint)DAT_00482393;
      *(byte *)((int)&DAT_00481fa0 + uVar4) = *(byte *)((int)&DAT_00481fa0 + uVar4) | 8;
      bVar2 = DAT_00483277;
      bVar5 = DAT_00483276;
      if (((*(byte *)((int)&DAT_00495ab0 + uVar4) & 0x1f) != DAT_00482394) ||
         (((&DAT_00495af0)[uVar4] & 0x1f) != DAT_00482395)) {
        bVar6 = 3;
        DAT_00484711 = DAT_00482394;
        DAT_00484710 = 0;
        DAT_00484712 = DAT_00482395;
      }
      (&DAT_00484710)[bVar6] = 1;
      (&DAT_00484711)[bVar6] = bVar5;
      (&DAT_00484712)[bVar6] = bVar2;
      (&DAT_00484713)[bVar6] = 0xff;
      return;
    }
    *(byte *)((int)&DAT_00481ff0 + (uint)DAT_00483274) =
         *(byte *)((int)&DAT_00481ff0 + (uint)DAT_00483274) | 0x80;
  }
  else {
    thunk_FUN_00417af0();
  }
  DAT_00484710 = 0xff;
  return;
}



/* ================= FUN_00424f70 @ 00424f70 ================= */

void FUN_00424f70(void)

{
  int iVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  byte bVar5;
  byte bVar6;
  char cVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  byte bVar11;
  undefined4 *puVar12;
  
  cVar7 = FUN_004179f0(DAT_00483276,DAT_00483277);
  if (((byte)(cVar7 - 1U) != (&DAT_0042b218)[DAT_0048327c]) &&
     ((byte)(&DAT_0042b218)[DAT_0048327c] <= (byte)(cVar7 - 1U))) {
    return;
  }
  iVar1 = (uint)DAT_0048327e * 0x20 + (uint)DAT_0048327d;
  uVar2 = *(undefined1 *)((int)&DAT_00495820 + iVar1);
  DAT_00482348 = iVar1;
  *(undefined1 *)((int)&DAT_00495820 + iVar1) = 0xff;
  uVar3 = *(undefined1 *)((int)&DAT_00495ab0 + (uint)DAT_0048327b);
  uVar4 = (&DAT_00495af0)[DAT_0048327b];
  if (DAT_00482390 == '\x03') {
    puVar12 = (undefined4 *)&DAT_00495c70;
    DAT_00482348 = iVar1;
    for (iVar9 = 0xa0; iVar9 != 0; iVar9 = iVar9 + -1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
    (&DAT_00495c70)[iVar1] = 1;
  }
  else {
    FUN_004015d0();
  }
  DAT_00481fda = DAT_00483276;
  DAT_00481fdb = DAT_00483277;
  FUN_004179b0(4);
  DAT_00482397 = 0;
  do {
    bVar6 = DAT_00481fdb;
    bVar5 = DAT_00481fda;
    if ((((DAT_00481fda < DAT_00495c02) && (DAT_00481fdb < DAT_00495c03)) &&
        (DAT_00482348 = (uint)DAT_00481fdb * 0x20 + (uint)DAT_00481fda,
        (&DAT_00495c70)[DAT_00482348] != '\0')) &&
       (((bVar11 = *(byte *)((int)&DAT_00481a80 + DAT_00482348) & 0xf, bVar11 < 0xb ||
         (0xd < bVar11)) && (*(char *)((int)&DAT_00495820 + DAT_00482348) == -1)))) {
      *(byte *)((int)&DAT_00495820 + DAT_00482348) = DAT_00495c63 | DAT_0048327b;
      uVar8 = (uint)DAT_0048327b;
      *(byte *)((int)&DAT_00495ab0 + uVar8) = bVar5;
      (&DAT_00495af0)[uVar8] = bVar6;
      FUN_00423a80();
      if (DAT_00481f99 == 0) {
LAB_00425105:
        DAT_00481dbc = 0;
        FUN_0041d810();
        uVar8 = (uint)DAT_00481fda;
        uVar10 = (uint)DAT_00481fdb;
        if (DAT_00481f9a <= DAT_00481dee) {
          DAT_00481f9a = DAT_00481dee;
          DAT_00482394 = DAT_00481fda;
          DAT_00482395 = DAT_00481fdb;
          DAT_00482393 = DAT_0048327b;
        }
      }
      else {
        DAT_00481dbc = 1;
        FUN_0041da80();
        uVar10 = (uint)DAT_00481fdb;
        uVar8 = (uint)DAT_00481fda;
        if (DAT_00481df5 < DAT_00481f99) goto LAB_00425105;
      }
      *(undefined1 *)((int)&DAT_00495820 + uVar8 + uVar10 * 0x20) = 0xff;
    }
    bVar5 = DAT_00482397;
    if (5 < DAT_00482397) {
      *(undefined1 *)((int)&DAT_00495820 + (uint)DAT_0048327d + (uint)DAT_0048327e * 0x20) = uVar2;
      uVar8 = (uint)DAT_0048327b;
      *(undefined1 *)((int)&DAT_00495ab0 + uVar8) = uVar3;
      (&DAT_00495af0)[uVar8] = uVar4;
      FUN_00423a80();
      return;
    }
    DAT_00482397 = DAT_00482397 + 1;
    FUN_004179b0(bVar5);
  } while( true );
}



/* ================= FUN_004251d0 @ 004251d0 ================= */

void FUN_004251d0(void)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = FUN_004179f0(DAT_00483276,DAT_00483277);
  if (1 < iVar2) {
    if (((&DAT_0042b288)[DAT_00483275] & 0x40) == 0) {
      bVar1 = (&DAT_0042b1e8)[DAT_0048327c];
    }
    else {
      bVar1 = (&DAT_0042b200)[DAT_0048327c];
    }
    if (DAT_00482348 <= bVar1) {
      DAT_00481dbc = 1;
      FUN_0041d810();
      if ((DAT_00481f9a <= DAT_00481dee) && (DAT_00481dee != DAT_00481f9a)) {
        DAT_00481f9a = DAT_00481dee;
        DAT_00482394 = DAT_0048327d;
        DAT_00482395 = DAT_0048327e;
        DAT_00482393 = DAT_0048327b;
      }
    }
  }
  return;
}



/* ================= GetOpenFileNameA @ 00425450 ================= */

BOOL GetOpenFileNameA(LPOPENFILENAMEA param_1)

{
  BOOL BVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00425450. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  BVar1 = GetOpenFileNameA(param_1);
  return BVar1;
}



/* ================= GetSaveFileNameA @ 00425456 ================= */

BOOL GetSaveFileNameA(LPOPENFILENAMEA param_1)

{
  BOOL BVar1;
  
                    /* WARNING: Could not recover jumptable at 0x00425456. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  BVar1 = GetSaveFileNameA(param_1);
  return BVar1;
}



/* ================= FUN_004254b0 @ 004254b0 ================= */

void __cdecl FUN_004254b0(byte *param_1,uchar *param_2,uchar *param_3,uchar *param_4,uchar *param_5)

{
  byte bVar1;
  byte *pbVar2;
  size_t sVar3;
  byte *pbVar4;
  byte *local_4;
  
  local_4 = (byte *)0x0;
  if (param_1[1] == 0x3a) {
    if (param_2 != (uchar *)0x0) {
      __mbsnbcpy(param_2,param_1,2);
      param_2[2] = '\0';
    }
    param_1 = param_1 + 2;
  }
  else if (param_2 != (uchar *)0x0) {
    *param_2 = '\0';
  }
  pbVar2 = (byte *)0x0;
  bVar1 = *param_1;
  pbVar4 = param_1;
  while (bVar1 != 0) {
    bVar1 = *pbVar4;
    if ((*(byte *)((int)&DAT_0047f8f8 + bVar1 + 1) & 4) == 0) {
      if ((bVar1 == 0x2f) || (bVar1 == 0x5c)) {
        pbVar2 = pbVar4 + 1;
      }
      else if (bVar1 == 0x2e) {
        local_4 = pbVar4;
      }
    }
    else {
      pbVar4 = pbVar4 + 1;
    }
    pbVar4 = pbVar4 + 1;
    bVar1 = *pbVar4;
  }
  if (pbVar2 == (byte *)0x0) {
    if (param_3 != (uchar *)0x0) {
      *param_3 = '\0';
    }
  }
  else {
    if (param_3 != (uchar *)0x0) {
      sVar3 = (int)pbVar2 - (int)param_1;
      if (0xfe < sVar3) {
        sVar3 = 0xff;
      }
      __mbsnbcpy(param_3,param_1,sVar3);
      param_3[sVar3] = '\0';
    }
    param_1 = pbVar2;
  }
  if ((local_4 == (byte *)0x0) || (local_4 < param_1)) {
    if (param_4 != (uchar *)0x0) {
      sVar3 = (int)pbVar4 - (int)param_1;
      if (0xfe < sVar3) {
        sVar3 = 0xff;
      }
      __mbsnbcpy(param_4,param_1,sVar3);
      param_4[sVar3] = '\0';
    }
    if (param_5 != (uchar *)0x0) {
      *param_5 = '\0';
    }
  }
  else {
    if (param_4 != (uchar *)0x0) {
      sVar3 = (int)local_4 - (int)param_1;
      if (0xfe < sVar3) {
        sVar3 = 0xff;
      }
      __mbsnbcpy(param_4,param_1,sVar3);
      param_4[sVar3] = '\0';
    }
    if (param_5 != (uchar *)0x0) {
      sVar3 = (int)pbVar4 - (int)local_4;
      if (0xfe < sVar3) {
        sVar3 = 0xff;
      }
      __mbsnbcpy(param_5,local_4,sVar3);
      param_5[sVar3] = '\0';
      return;
    }
  }
  return;
}



/* ================= _strncmp @ 00425610 ================= */

/* Library Function - Single Match
    _strncmp
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release */

int __cdecl _strncmp(char *_Str1,char *_Str2,size_t _MaxCount)

{
  char cVar1;
  char cVar2;
  size_t sVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  
  uVar5 = 0;
  if (_MaxCount != 0) {
    sVar3 = _MaxCount;
    pcVar6 = _Str1;
    do {
      if (sVar3 == 0) break;
      sVar3 = sVar3 - 1;
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    iVar4 = _MaxCount - sVar3;
    do {
      pcVar6 = _Str2;
      pcVar7 = _Str1;
      if (iVar4 == 0) break;
      iVar4 = iVar4 + -1;
      pcVar7 = _Str1 + 1;
      pcVar6 = _Str2 + 1;
      cVar2 = *_Str1;
      cVar1 = *_Str2;
      _Str2 = pcVar6;
      _Str1 = pcVar7;
    } while (cVar1 == cVar2);
    uVar5 = 0;
    if ((byte)pcVar6[-1] <= (byte)pcVar7[-1]) {
      if (pcVar6[-1] == pcVar7[-1]) {
        return 0;
      }
      uVar5 = 0xfffffffe;
    }
    uVar5 = ~uVar5;
  }
  return uVar5;
}



/* ================= _strchr @ 00425660 ================= */

/* Library Function - Single Match
    _strchr
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release */

char * __cdecl _strchr(char *_Str,int _Val)

{
  uint uVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  
  while (((uint)_Str & 3) != 0) {
    uVar1 = *(uint *)_Str;
    if ((char)uVar1 == (char)_Val) {
      return (char *)(uint *)_Str;
    }
    _Str = (char *)((int)_Str + 1);
    if ((char)uVar1 == '\0') {
      return (char *)0x0;
    }
  }
  while( true ) {
    while( true ) {
      uVar1 = *(uint *)_Str;
      uVar4 = uVar1 ^ CONCAT22(CONCAT11((char)_Val,(char)_Val),CONCAT11((char)_Val,(char)_Val));
      uVar3 = uVar1 ^ 0xffffffff ^ uVar1 + 0x7efefeff;
      puVar5 = (uint *)((int)_Str + 4);
      if (((uVar4 ^ 0xffffffff ^ uVar4 + 0x7efefeff) & 0x81010100) != 0) break;
      _Str = (char *)puVar5;
      if ((uVar3 & 0x81010100) != 0) {
        if ((uVar3 & 0x1010100) != 0) {
          return (char *)0x0;
        }
        if ((uVar1 + 0x7efefeff & 0x80000000) == 0) {
          return (char *)0x0;
        }
      }
    }
    uVar1 = *(uint *)_Str;
    if ((char)uVar1 == (char)_Val) {
      return (char *)(uint *)_Str;
    }
    if ((char)uVar1 == '\0') {
      return (char *)0x0;
    }
    cVar2 = (char)(uVar1 >> 8);
    if (cVar2 == (char)_Val) {
      return (char *)((int)_Str + 1);
    }
    if (cVar2 == '\0') {
      return (char *)0x0;
    }
    cVar2 = (char)(uVar1 >> 0x10);
    if (cVar2 == (char)_Val) {
      return (char *)((int)_Str + 2);
    }
    if (cVar2 == '\0') break;
    cVar2 = (char)(uVar1 >> 0x18);
    if (cVar2 == (char)_Val) {
      return (char *)((int)_Str + 3);
    }
    _Str = (char *)puVar5;
    if (cVar2 == '\0') {
      return (char *)0x0;
    }
  }
  return (char *)0x0;
}



/* ================= FUN_00425720 @ 00425720 ================= */

/* WARNING: Unable to track spacebase fully for stack */

void __cdecl FUN_00425720(undefined1 param_1)

{
  uint in_EAX;
  undefined1 *puVar1;
  undefined4 unaff_retaddr;
  
  puVar1 = &param_1;
  for (; 0xfff < in_EAX; in_EAX = in_EAX - 0x1000) {
    puVar1 = puVar1 + -0x1000;
  }
  *(undefined4 *)(puVar1 + (-4 - in_EAX)) = unaff_retaddr;
  return;
}



/* ================= __chdir @ 00425830 ================= */

/* Library Function - Single Match
    __chdir
   
   Library: Visual Studio 1998 Release */

int __cdecl __chdir(char *_Path)

{
  BOOL BVar1;
  DWORD DVar2;
  uint uVar3;
  CHAR local_10c;
  undefined1 local_10b;
  undefined1 local_10a;
  undefined1 local_109;
  byte local_108;
  byte local_107;
  
  BVar1 = SetCurrentDirectoryA(_Path);
  if (BVar1 != 0) {
    DVar2 = GetCurrentDirectoryA(0x105,(LPSTR)&local_108);
    if (DVar2 != 0) {
      if (((local_108 == 0x5c) || (local_108 == 0x2f)) && (local_108 == local_107)) {
        return 0;
      }
      local_10c = '=';
      uVar3 = __mbctoupper((uint)local_108);
      local_10b = (undefined1)uVar3;
      local_10a = 0x3a;
      local_109 = 0;
      BVar1 = SetEnvironmentVariableA(&local_10c,(LPCSTR)&local_108);
      if (BVar1 != 0) {
        return 0;
      }
    }
  }
  DVar2 = GetLastError();
  FUN_004262d0(DVar2);
  return -1;
}



/* ================= __chdrive @ 00425920 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __chdrive
   
   Library: Visual Studio 1998 Release */

int __cdecl __chdrive(int _Drive)

{
  BOOL BVar1;
  DWORD DVar2;
  char local_4 [4];
  
  if ((_Drive < 1) || (0x1f < _Drive)) {
    _DAT_0047fb10 = 0xd;
    _DAT_0047fb14 = 0xf;
    return -1;
  }
  local_4[1] = 0x3a;
  local_4[2] = 0;
  local_4[0] = (char)_Drive + '@';
  BVar1 = SetCurrentDirectoryA(local_4);
  if (BVar1 == 0) {
    DVar2 = GetLastError();
    FUN_004262d0(DVar2);
    return -1;
  }
  return 0;
}



/* ================= FUN_00425990 @ 00425990 ================= */

void __cdecl FUN_00425990(char *param_1,SIZE_T param_2)

{
  FUN_004259b0(0,param_1,param_2);
  return;
}



/* ================= FUN_004259b0 @ 004259b0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char * __cdecl FUN_004259b0(uint param_1,char *param_2,SIZE_T param_3)

{
  char cVar1;
  int iVar2;
  DWORD DVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  char local_10c [4];
  LPSTR local_108;
  CHAR local_104 [260];
  
  if (param_1 == 0) {
    DVar3 = GetCurrentDirectoryA(0x104,local_104);
  }
  else {
    iVar2 = __validdrive(param_1);
    if (iVar2 == 0) {
      _DAT_0047fb14 = 0xf;
      _DAT_0047fb10 = 0xd;
      return (char *)0x0;
    }
    local_10c[0] = (char)param_1 + '@';
    local_10c[1] = 0x3a;
    local_10c[2] = 0x2e;
    local_10c[3] = 0;
    DVar3 = GetFullPathNameA(local_10c,0x104,local_104,&local_108);
  }
  if ((DVar3 == 0) || (uVar4 = DVar3 + 1, 0x104 < uVar4)) {
    return (char *)0x0;
  }
  if (param_2 == (char *)0x0) {
    if ((int)uVar4 <= (int)param_3) {
      uVar4 = param_3;
    }
    param_2 = (char *)FUN_00426250(uVar4);
    if (param_2 == (char *)0x0) {
      _DAT_0047fb10 = 0xc;
      return (char *)0x0;
    }
  }
  else if ((int)param_3 < (int)uVar4) {
    _DAT_0047fb10 = 0x22;
    return (char *)0x0;
  }
  uVar4 = 0xffffffff;
  pcVar6 = local_104;
  do {
    pcVar7 = pcVar6;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar7 = pcVar6 + 1;
    cVar1 = *pcVar6;
    pcVar6 = pcVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar6 = pcVar7 + -uVar4;
  pcVar7 = param_2;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar7 = *pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar7 = pcVar7 + 1;
  }
  return param_2;
}



/* ================= __validdrive @ 00425ae0 ================= */

/* Library Function - Single Match
    __validdrive
   
   Library: Visual Studio 1998 Release */

int __cdecl __validdrive(uint param_1)

{
  UINT UVar1;
  char local_4 [4];
  
  if (param_1 == 0) {
    return 1;
  }
  local_4[1] = 0x3a;
  local_4[2] = 0x5c;
  local_4[3] = 0;
  local_4[0] = (char)param_1 + '@';
  UVar1 = GetDriveTypeA(local_4);
  if ((UVar1 == 0) || (UVar1 == 1)) {
    return 0;
  }
  return 1;
}



/* ================= entry @ 00425b30 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void entry(void)

{
  byte bVar1;
  DWORD DVar2;
  int iVar3;
  uint uVar4;
  HMODULE pHVar5;
  UINT UVar6;
  byte *pbVar7;
  int unaff_EDI;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uVar8;
  _STARTUPINFOA local_74;
  undefined1 *local_1c;
  undefined4 local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_14 = *unaff_FS_OFFSET;
  local_8 = 0xffffffff;
  puStack_c = &DAT_00429080;
  puStack_10 = &LAB_00427248;
  *unaff_FS_OFFSET = &local_14;
  local_1c = (undefined1 *)((int)register0x00000010 + -0x80);
  DVar2 = GetVersion();
  _DAT_0047fb28 = DVar2 >> 8 & 0xff;
  DAT_0047fb1c = DVar2 >> 0x10;
  _DAT_0047fb24 = DVar2 & 0xff;
  _DAT_0047fb20 = _DAT_0047fb24 * 0x100 + _DAT_0047fb28;
  FUN_00427210();
  local_8 = 0;
  FUN_00427000();
  ___initmbctable();
  DAT_00496060 = (byte *)GetCommandLineA();
  DAT_0047f8e0 = FUN_00426e60();
  if ((DAT_0047f8e0 == (LPSTR)0x0) || (DAT_00496060 == (byte *)0x0)) {
    FUN_00426090(0xffffffff);
  }
  __setargv();
  FUN_00426900();
  __cinit(unaff_EDI);
  bVar1 = *DAT_00496060;
  pbVar7 = DAT_00496060;
  if (bVar1 == 0x22) {
    pbVar7 = DAT_00496060 + 1;
    if (*pbVar7 != 0x22) {
      do {
        if (*pbVar7 == 0) break;
        iVar3 = __ismbblead((uint)*pbVar7);
        if (iVar3 != 0) {
          pbVar7 = pbVar7 + 1;
        }
        pbVar7 = pbVar7 + 1;
      } while (*pbVar7 != 0x22);
      if (*pbVar7 != 0x22) goto LAB_00425c27;
    }
    pbVar7 = pbVar7 + 1;
  }
  else {
    while (0x20 < bVar1) {
      bVar1 = pbVar7[1];
      pbVar7 = pbVar7 + 1;
    }
  }
LAB_00425c27:
  bVar1 = *pbVar7;
  while ((bVar1 != 0 && (*pbVar7 < 0x21))) {
    pbVar7 = pbVar7 + 1;
    bVar1 = *pbVar7;
  }
  local_74.dwFlags = 0;
  GetStartupInfoA(&local_74);
  uVar4 = 10;
  if ((local_74.dwFlags & 1) != 0) {
    uVar4 = (uint)local_74.wShowWindow;
  }
  uVar8 = 0;
  pHVar5 = GetModuleHandleA((LPCSTR)0x0);
  UVar6 = FUN_00411210(pHVar5,uVar8,pbVar7,uVar4);
  FUN_00426090(UVar6);
  *unaff_FS_OFFSET = local_14;
  return;
}



/* ================= __amsg_exit @ 00425cc0 ================= */

/* Library Function - Single Match
    __amsg_exit
   
   Library: Visual Studio 1998 Release */

void __cdecl __amsg_exit(int param_1)

{
  if (DAT_0047f8ec == 1) {
    __FF_MSGBANNER();
  }
  FUN_00427360(param_1);
  (*(code *)PTR___exit_0047f8e8)(0xff);
  return;
}



/* ================= __mbsnbcpy @ 00425cf0 ================= */

/* Library Function - Single Match
    __mbsnbcpy
   
   Library: Visual Studio 1998 Release */

uchar * __cdecl __mbsnbcpy(uchar *_Dest,uchar *_Source,size_t _Count)

{
  byte bVar1;
  uchar *puVar2;
  byte *pbVar3;
  byte *pbVar4;
  size_t sVar5;
  
  if (DAT_0047f9fc == 0) {
    puVar2 = (uchar *)_strncpy((char *)_Dest,(char *)_Source,_Count);
    return puVar2;
  }
  pbVar4 = _Dest;
  do {
    while( true ) {
      pbVar3 = pbVar4;
      pbVar4 = pbVar3;
      sVar5 = _Count;
      if (_Count == 0) goto joined_r0x00425d53;
      sVar5 = _Count - 1;
      bVar1 = *_Source;
      if ((*(byte *)((int)&DAT_0047f8f8 + bVar1 + 1) & 4) == 0) break;
      *pbVar3 = bVar1;
      pbVar4 = pbVar3 + 1;
      if (sVar5 == 0) {
        *pbVar3 = 0;
        goto joined_r0x00425d53;
      }
      bVar1 = _Source[1];
      sVar5 = _Count - 2;
      *pbVar4 = bVar1;
      pbVar4 = pbVar3 + 2;
      _Source = _Source + 2;
      _Count = sVar5;
      if (bVar1 == 0) {
        *pbVar3 = 0;
        goto joined_r0x00425d53;
      }
    }
    *pbVar3 = bVar1;
    pbVar4 = pbVar3 + 1;
    _Source = _Source + 1;
    _Count = sVar5;
  } while (bVar1 != 0);
joined_r0x00425d53:
  for (; sVar5 != 0; sVar5 = sVar5 - 1) {
    *pbVar4 = 0;
    pbVar4 = pbVar4 + 1;
  }
  return _Dest;
}



/* ================= __setmbcp @ 00425d80 ================= */

/* Library Function - Single Match
    __setmbcp
   
   Library: Visual Studio 1998 Release */

int __cdecl __setmbcp(int _CodePage)

{
  byte *pbVar1;
  byte bVar2;
  UINT CodePage;
  UINT *pUVar3;
  BOOL BVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  BYTE *pBVar8;
  byte *pbVar9;
  undefined4 *puVar10;
  int local_18;
  _cpinfo local_14;
  
  CodePage = getSystemCP(_CodePage);
  if (CodePage == DAT_0047f9fc) {
    return 0;
  }
  if (CodePage == 0) {
    setSBCS();
    return 0;
  }
  local_18 = 0;
  pUVar3 = &DAT_0047fa20;
  do {
    if (*pUVar3 == CodePage) {
      uVar6 = 0;
      puVar10 = &DAT_0047f8f8;
      for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
        *puVar10 = 0;
        puVar10 = puVar10 + 1;
      }
      *(undefined1 *)puVar10 = 0;
      do {
        pbVar9 = &DAT_0047fa30 + (local_18 * 6 + uVar6) * 8;
        bVar2 = *pbVar9;
        while ((bVar2 != 0 && (pbVar9[1] != 0))) {
          uVar7 = (uint)*pbVar9;
          if (uVar7 <= pbVar9[1]) {
            bVar2 = (&DAT_0047fa18)[uVar6];
            do {
              pbVar1 = (byte *)((int)&DAT_0047f8f8 + uVar7 + 1);
              *pbVar1 = *pbVar1 | bVar2;
              uVar7 = uVar7 + 1;
            } while (uVar7 <= pbVar9[1]);
          }
          pbVar9 = pbVar9 + 2;
          bVar2 = *pbVar9;
        }
        uVar6 = uVar6 + 1;
      } while (uVar6 < 4);
      DAT_0047f9fc = CodePage;
      DAT_0047fa00 = _CPtoLCID(CodePage);
      DAT_0047fa08 = *(undefined4 *)(&DAT_0047fa24 + local_18 * 0x30);
      DAT_0047fa0c = *(undefined4 *)(&DAT_0047fa28 + local_18 * 0x30);
      DAT_0047fa10 = *(undefined4 *)(local_18 * 0x30 + 0x47fa2c);
      return 0;
    }
    pUVar3 = pUVar3 + 0xc;
    local_18 = local_18 + 1;
  } while (pUVar3 < &DAT_0047fb10);
  BVar4 = GetCPInfo(CodePage,&local_14);
  if (BVar4 != 1) {
    if (DAT_0047fa14 == 0) {
      return -1;
    }
    setSBCS();
    return 0;
  }
  puVar10 = &DAT_0047f8f8;
  for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar10 = 0;
    puVar10 = puVar10 + 1;
  }
  *(undefined1 *)puVar10 = 0;
  if (local_14.MaxCharSize < 2) {
    DAT_0047fa00 = 0;
    DAT_0047f9fc = 0;
  }
  else {
    pBVar8 = local_14.LeadByte;
    while ((local_14.LeadByte[0] != 0 && (pBVar8[1] != 0))) {
      uVar6 = (uint)*pBVar8;
      if (uVar6 <= pBVar8[1]) {
        do {
          pbVar9 = (byte *)((int)&DAT_0047f8f8 + uVar6 + 1);
          *pbVar9 = *pbVar9 | 4;
          uVar6 = uVar6 + 1;
        } while (uVar6 <= pBVar8[1]);
      }
      pBVar8 = pBVar8 + 2;
      local_14.LeadByte[0] = *pBVar8;
    }
    uVar6 = 1;
    do {
      pbVar9 = (byte *)((int)&DAT_0047f8f8 + uVar6 + 1);
      *pbVar9 = *pbVar9 | 8;
      uVar6 = uVar6 + 1;
    } while (uVar6 < 0xff);
    DAT_0047f9fc = CodePage;
    DAT_0047fa00 = _CPtoLCID(CodePage);
  }
  DAT_0047fa08 = 0;
  DAT_0047fa0c = 0;
  DAT_0047fa10 = 0;
  return 0;
}



/* ================= getSystemCP @ 00425f60 ================= */

/* Library Function - Single Match
    _getSystemCP
   
   Library: Visual Studio 1998 Release */

int __cdecl getSystemCP(int param_1)

{
  int iVar1;
  bool bVar2;
  
  if (param_1 == -2) {
    DAT_0047fa14 = 1;
                    /* WARNING: Could not recover jumptable at 0x00425f7d. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetOEMCP();
    return iVar1;
  }
  if (param_1 == -3) {
    DAT_0047fa14 = 1;
                    /* WARNING: Could not recover jumptable at 0x00425f92. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    iVar1 = GetACP();
    return iVar1;
  }
  bVar2 = param_1 == -4;
  if (bVar2) {
    param_1 = DAT_0047fe28;
  }
  DAT_0047fa14 = (uint)bVar2;
  return param_1;
}



/* ================= _CPtoLCID @ 00425fb0 ================= */

/* Library Function - Single Match
    _CPtoLCID
   
   Library: Visual Studio 1998 Release */

undefined4 __cdecl _CPtoLCID(undefined4 param_1)

{
  switch(param_1) {
  case 0x3a4:
    return 0x411;
  default:
    return 0;
  case 0x3a8:
    return 0x804;
  case 0x3b5:
    return 0x412;
  case 0x3b6:
    return 0x404;
  }
}



/* ================= setSBCS @ 00426010 ================= */

/* Library Function - Single Match
    _setSBCS
   
   Library: Visual Studio 1998 Release */

void __cdecl setSBCS(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_0047f8f8;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined1 *)puVar2 = 0;
  DAT_0047fa08 = 0;
  DAT_0047f9fc = 0;
  DAT_0047fa00 = 0;
  DAT_0047fa0c = 0;
  DAT_0047fa10 = 0;
  return;
}



/* ================= ___initmbctable @ 00426050 ================= */

/* Library Function - Single Match
    ___initmbctable
   
   Library: Visual Studio 1998 Release */

void ___initmbctable(void)

{
  __setmbcp(-3);
  return;
}



/* ================= __cinit @ 00426060 ================= */

/* Library Function - Single Match
    __cinit
   
   Library: Visual Studio 1998 Release */

int __cdecl __cinit(int param_1)

{
  int iVar1;
  
  if (DAT_0049605c != (code *)0x0) {
    (*DAT_0049605c)();
  }
  __initterm((undefined4 *)&DAT_0042a00c,(undefined4 *)&DAT_0042a014);
  iVar1 = __initterm((undefined4 *)&DAT_0042a000,(undefined4 *)&DAT_0042a008);
  return iVar1;
}



/* ================= FUN_00426090 @ 00426090 ================= */

void __cdecl FUN_00426090(UINT param_1)

{
  FUN_004260f0(param_1,0,0);
  return;
}



/* ================= __exit @ 004260b0 ================= */

/* Library Function - Single Match
    __exit
   
   Library: Visual Studio 1998 Release */

void __cdecl __exit(int _Code)

{
  FUN_004260f0(_Code,1,0);
  return;
}



/* ================= FUN_004260f0 @ 004260f0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_004260f0(UINT param_1,int param_2,int param_3)

{
  HANDLE hProcess;
  undefined4 *puVar1;
  UINT uExitCode;
  
  if (DAT_0047fb58 == 1) {
    uExitCode = param_1;
    hProcess = GetCurrentProcess();
    TerminateProcess(hProcess,uExitCode);
  }
  _DAT_0047fb54 = 1;
  DAT_0047fb50 = (undefined1)param_3;
  if (param_2 == 0) {
    if ((DAT_00496058 != (undefined4 *)0x0) &&
       (puVar1 = (undefined4 *)(DAT_00496054 + -4), DAT_00496058 <= puVar1)) {
      do {
        if ((code *)*puVar1 != (code *)0x0) {
          (*(code *)*puVar1)();
        }
        puVar1 = puVar1 + -1;
      } while (DAT_00496058 <= puVar1);
    }
    __initterm((undefined4 *)&DAT_0042a018,(undefined4 *)&DAT_0042a01c);
  }
  __initterm((undefined4 *)&DAT_0042a020,(undefined4 *)&DAT_0042a024);
  if (param_3 == 0) {
    DAT_0047fb58 = 1;
                    /* WARNING: Subroutine does not return */
    ExitProcess(param_1);
  }
  return;
}



/* ================= __initterm @ 004261a0 ================= */

/* Library Function - Single Match
    __initterm
   
   Library: Visual Studio 1998 Release */

void __cdecl __initterm(undefined4 *param_1,undefined4 *param_2)

{
  for (; param_1 < param_2; param_1 = param_1 + 1) {
    if ((code *)*param_1 != (code *)0x0) {
      (*(code *)*param_1)();
    }
  }
  return;
}



/* ================= FUN_00426250 @ 00426250 ================= */

void __cdecl FUN_00426250(SIZE_T param_1)

{
  FUN_00426270(param_1,DAT_0047fe2c);
  return;
}



/* ================= FUN_00426270 @ 00426270 ================= */

int __cdecl FUN_00426270(SIZE_T param_1,int param_2)

{
  int iVar1;
  
  if (0xffffffe0 < param_1) {
    return 0;
  }
  if (param_1 == 0) {
    param_1 = 1;
  }
  while( true ) {
    iVar1 = FUN_004262b0(param_1);
    if (iVar1 != 0) {
      return iVar1;
    }
    if (param_2 == 0) break;
    iVar1 = FUN_004276a0(param_1);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return 0;
}



/* ================= FUN_004262b0 @ 004262b0 ================= */

void __cdecl FUN_004262b0(SIZE_T param_1)

{
  HeapAlloc(DAT_00495f40,0,param_1);
  return;
}



/* ================= FUN_004262d0 @ 004262d0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_004262d0(uint param_1)

{
  uint *puVar1;
  int iVar2;
  
  iVar2 = 0;
  puVar1 = &DAT_0047fb60;
  _DAT_0047fb14 = param_1;
  do {
    if (*puVar1 == param_1) {
      _DAT_0047fb10 = *(undefined4 *)(iVar2 * 8 + 0x47fb64);
      return;
    }
    puVar1 = puVar1 + 2;
    iVar2 = iVar2 + 1;
  } while (puVar1 < &DAT_0047fcc8);
  if ((0x12 < param_1) && (param_1 < 0x25)) {
    _DAT_0047fb10 = 0xd;
    return;
  }
  if ((param_1 < 0xbc) || (0xca < param_1)) {
    _DAT_0047fb10 = 0x16;
    return;
  }
  _DAT_0047fb10 = 8;
  return;
}



/* ================= __mbctoupper @ 00426340 ================= */

/* Library Function - Single Match
    __mbctoupper
   
   Library: Visual Studio 1998 Release */

uint __cdecl __mbctoupper(uint _Ch)

{
  int iVar1;
  byte local_6;
  undefined1 local_5;
  undefined1 local_4;
  undefined1 local_3;
  
  if (_Ch < 0x100) {
    if (((int)_Ch < 0x61) || (0x7a < (int)_Ch)) {
      return _Ch;
    }
    return _Ch - 0x20;
  }
  local_6 = (byte)(_Ch >> 8);
  local_5 = (undefined1)_Ch;
  if ((*(byte *)((int)&DAT_0047f8f8 + local_6 + 1) & 4) == 0) {
    return _Ch;
  }
  iVar1 = FUN_00427930(DAT_0047fa00,0x200,(char *)&local_6,2,(LPWSTR)&local_4,2,DAT_0047f9fc);
  if (iVar1 == 0) {
    return _Ch;
  }
  return (uint)CONCAT11(local_4,local_3);
}



/* ================= FID_conflict:__toupper_lk @ 004263e0 ================= */

/* Library Function - Multiple Matches With Different Base Names
    __toupper_lk
    _toupper
   
   Library: Visual Studio 1998 Release */

int __cdecl FID_conflict___toupper_lk(int _C)

{
  uint uVar1;
  size_t sVar2;
  int iVar3;
  WCHAR local_8 [2];
  byte local_4;
  byte local_3;
  undefined1 local_2;
  
  if (DAT_0047fe18 == 0) {
    if ((0x60 < _C) && (_C < 0x7b)) {
      _C = _C + -0x20;
    }
    return _C;
  }
  if (_C < 0x100) {
    if (DAT_00480044 < 2) {
      uVar1 = *(ushort *)(PTR_DAT_0047fe38 + _C * 2) & 2;
    }
    else {
      uVar1 = __isctype(_C,2);
    }
    if (uVar1 == 0) {
      return _C;
    }
  }
  local_4 = (byte)((uint)_C >> 8);
  if ((PTR_DAT_0047fe38[(uint)local_4 * 2 + 1] & 0x80) == 0) {
    sVar2 = 1;
    local_3 = 0;
    local_4 = (byte)_C;
  }
  else {
    sVar2 = 2;
    local_2 = 0;
    local_3 = (byte)_C;
  }
  iVar3 = FUN_00427930(DAT_0047fe18,0x200,(char *)&local_4,sVar2,local_8,3,0);
  if (iVar3 == 0) {
    return _C;
  }
  if (iVar3 == 1) {
    return (uint)(byte)local_8[0];
  }
  return (uint)(ushort)local_8[0];
}



/* ================= __global_unwind2 @ 004264c8 ================= */

/* Library Function - Single Match
    __global_unwind2
   
   Library: Visual Studio */

void __cdecl __global_unwind2(PVOID param_1)

{
  RtlUnwind(param_1,(PVOID)0x4264e0,(PEXCEPTION_RECORD)0x0,(PVOID)0x0);
  return;
}



/* ================= __local_unwind2 @ 0042650a ================= */

/* Library Function - Single Match
    __local_unwind2
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release, Visual Studio 2003 Debug, Visual
   Studio 2003 Release */

void __cdecl __local_unwind2(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_1c;
  undefined1 *puStack_18;
  undefined4 local_14;
  int iStack_10;
  
  iStack_10 = param_1;
  puStack_18 = &LAB_004264e8;
  uStack_1c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_1c;
  while( true ) {
    iVar1 = *(int *)(param_1 + 8);
    iVar2 = *(int *)(param_1 + 0xc);
    if ((iVar2 == -1) || (iVar2 == param_2)) break;
    local_14 = *(undefined4 *)(iVar1 + iVar2 * 0xc);
    *(undefined4 *)(param_1 + 0xc) = local_14;
    if (*(int *)(iVar1 + 4 + iVar2 * 0xc) == 0) {
      FUN_0042659e();
      (**(code **)(iVar1 + 8 + iVar2 * 0xc))();
    }
  }
  *unaff_FS_OFFSET = uStack_1c;
  return;
}



/* ================= FUN_0042659e @ 0042659e ================= */

void FUN_0042659e(void)

{
  undefined4 in_EAX;
  int unaff_EBP;
  
  DAT_0047fcd0 = *(undefined4 *)(unaff_EBP + 8);
  DAT_0047fccc = in_EAX;
  DAT_0047fcd4 = unaff_EBP;
  return;
}



/* ================= __ismbblead @ 00426850 ================= */

/* Library Function - Single Match
    __ismbblead
   
   Library: Visual Studio 1998 Release */

int __cdecl __ismbblead(uint _C)

{
  int iVar1;
  
  iVar1 = x_ismbbtype((byte)_C,0,4);
  return iVar1;
}



/* ================= x_ismbbtype @ 004268c0 ================= */

/* Library Function - Single Match
    _x_ismbbtype
   
   Library: Visual Studio 1998 Release */

undefined4 __cdecl x_ismbbtype(byte param_1,uint param_2,byte param_3)

{
  uint uVar1;
  
  if ((param_3 & *(byte *)((int)&DAT_0047f8f8 + param_1 + 1)) == 0) {
    uVar1 = 0;
    if (param_2 != 0) {
      uVar1 = *(ushort *)(&DAT_0047fe42 + (uint)param_1 * 2) & param_2;
    }
    if (uVar1 == 0) {
      return 0;
    }
  }
  return 1;
}



/* ================= FUN_00426900 @ 00426900 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00426900(void)

{
  char cVar1;
  int *piVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  
  iVar7 = 0;
  cVar1 = *DAT_0047f8e0;
  pcVar6 = DAT_0047f8e0;
  while (cVar1 != '\0') {
    if (*pcVar6 != '=') {
      iVar7 = iVar7 + 1;
    }
    uVar3 = 0xffffffff;
    pcVar9 = pcVar6;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    pcVar6 = pcVar6 + ~uVar3;
    cVar1 = *pcVar6;
  }
  piVar2 = (int *)FUN_00426250(iVar7 * 4 + 4);
  _DAT_0047fb38 = piVar2;
  if (piVar2 == (int *)0x0) {
    __amsg_exit(9);
  }
  cVar1 = *DAT_0047f8e0;
  pcVar6 = DAT_0047f8e0;
  do {
    if (cVar1 == '\0') {
      FUN_004276c0(DAT_0047f8e0);
      *piVar2 = 0;
      return;
    }
    uVar3 = 0xffffffff;
    pcVar9 = pcVar6;
    do {
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    if (*pcVar6 != '=') {
      iVar7 = FUN_00426250(~uVar3);
      *piVar2 = iVar7;
      if (iVar7 == 0) {
        __amsg_exit(9);
      }
      uVar4 = 0xffffffff;
      pcVar9 = pcVar6;
      do {
        pcVar8 = pcVar9;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar8 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar8;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pcVar9 = (char *)*piVar2;
      piVar2 = piVar2 + 1;
      pcVar8 = pcVar8 + -uVar4;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar9 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      }
    }
    pcVar6 = pcVar6 + ~uVar3;
    cVar1 = *pcVar6;
  } while( true );
}



/* ================= __setargv @ 004269e0 ================= */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Library Function - Single Match
    __setargv
   
   Library: Visual Studio 1998 Release */

int __cdecl __setargv(void)

{
  undefined4 *puVar1;
  byte *pbVar2;
  int local_8;
  int local_4;
  
  GetModuleFileNameA((HMODULE)0x0,&DAT_00481960,0x104);
  _DAT_0047fb48 = &DAT_00481960;
  pbVar2 = &DAT_00481960;
  if (*DAT_00496060 != 0) {
    pbVar2 = DAT_00496060;
  }
  parse_cmdline(pbVar2,(undefined4 *)0x0,(byte *)0x0,&local_8,&local_4);
  puVar1 = (undefined4 *)FUN_00426250(local_8 * 4 + local_4);
  if (puVar1 == (undefined4 *)0x0) {
    __amsg_exit(8);
  }
  parse_cmdline(pbVar2,puVar1,(byte *)(puVar1 + local_8),&local_8,&local_4);
  _DAT_0047fb30 = puVar1;
  _DAT_0047fb2c = local_8 + -1;
  return local_8 + -1;
}



/* ================= parse_cmdline @ 00426a80 ================= */

/* Library Function - Single Match
    _parse_cmdline
   
   Library: Visual Studio 1998 Release */

void __cdecl
parse_cmdline(byte *param_1,undefined4 *param_2,byte *param_3,int *param_4,int *param_5)

{
  byte bVar1;
  bool bVar2;
  bool bVar3;
  byte *pbVar4;
  uint uVar5;
  byte *pbVar6;
  
  *param_5 = 0;
  *param_4 = 1;
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = param_3;
    param_2 = param_2 + 1;
  }
  if (*param_1 == 0x22) {
    pbVar6 = param_1 + 1;
    bVar1 = *pbVar6;
    while ((bVar1 != 0x22 && (*pbVar6 != 0))) {
      if (((*(byte *)((int)&DAT_0047f8f8 + *pbVar6 + 1) & 4) != 0) &&
         (*param_5 = *param_5 + 1, param_3 != (byte *)0x0)) {
        bVar1 = *pbVar6;
        pbVar6 = pbVar6 + 1;
        *param_3 = bVar1;
        param_3 = param_3 + 1;
      }
      *param_5 = *param_5 + 1;
      if (param_3 != (byte *)0x0) {
        *param_3 = *pbVar6;
        param_3 = param_3 + 1;
      }
      pbVar6 = pbVar6 + 1;
      bVar1 = *pbVar6;
    }
    *param_5 = *param_5 + 1;
    if (param_3 != (byte *)0x0) {
      *param_3 = 0;
      param_3 = param_3 + 1;
    }
    if (*pbVar6 == 0x22) {
      pbVar6 = pbVar6 + 1;
    }
  }
  else {
    do {
      *param_5 = *param_5 + 1;
      if (param_3 != (byte *)0x0) {
        *param_3 = *param_1;
        param_3 = param_3 + 1;
      }
      bVar1 = *param_1;
      pbVar6 = param_1 + 1;
      if ((*(byte *)((int)&DAT_0047f8f8 + bVar1 + 1) & 4) != 0) {
        *param_5 = *param_5 + 1;
        if (param_3 != (byte *)0x0) {
          *param_3 = *pbVar6;
          param_3 = param_3 + 1;
        }
        pbVar6 = param_1 + 2;
      }
      if (bVar1 == 0x20) break;
      if (bVar1 == 0) goto LAB_00426af0;
      param_1 = pbVar6;
    } while (bVar1 != 9);
    if (bVar1 == 0) {
LAB_00426af0:
      pbVar6 = pbVar6 + -1;
    }
    else if (param_3 != (byte *)0x0) {
      param_3[-1] = 0;
    }
  }
  bVar3 = false;
  while (*pbVar6 != 0) {
    for (; (*pbVar6 == 0x20 || (*pbVar6 == 9)); pbVar6 = pbVar6 + 1) {
    }
    if (*pbVar6 == 0) break;
    if (param_2 != (undefined4 *)0x0) {
      *param_2 = param_3;
      param_2 = param_2 + 1;
    }
    *param_4 = *param_4 + 1;
    while( true ) {
      bVar2 = true;
      uVar5 = 0;
      bVar1 = *pbVar6;
      while (bVar1 == 0x5c) {
        pbVar6 = pbVar6 + 1;
        uVar5 = uVar5 + 1;
        bVar1 = *pbVar6;
      }
      if (*pbVar6 == 0x22) {
        if ((uVar5 & 1) == 0) {
          if ((bVar3) && (pbVar6[1] == 0x22)) {
            pbVar6 = pbVar6 + 1;
          }
          else {
            bVar2 = false;
          }
          bVar3 = !bVar3;
        }
        uVar5 = uVar5 >> 1;
      }
      while (uVar5 != 0) {
        uVar5 = uVar5 - 1;
        if (param_3 != (byte *)0x0) {
          *param_3 = 0x5c;
          param_3 = param_3 + 1;
        }
        *param_5 = *param_5 + 1;
      }
      bVar1 = *pbVar6;
      if ((bVar1 == 0) || ((!bVar3 && ((bVar1 == 0x20 || (bVar1 == 9)))))) break;
      if (bVar2) {
        if (param_3 == (byte *)0x0) {
          if ((*(byte *)((int)&DAT_0047f8f8 + bVar1 + 1) & 4) != 0) {
            pbVar6 = pbVar6 + 1;
            *param_5 = *param_5 + 1;
          }
          *param_5 = *param_5 + 1;
          goto LAB_00426c21;
        }
        pbVar4 = param_3;
        if ((*(byte *)((int)&DAT_0047f8f8 + bVar1 + 1) & 4) != 0) {
          *param_3 = bVar1;
          pbVar6 = pbVar6 + 1;
          pbVar4 = param_3 + 1;
          *param_5 = *param_5 + 1;
        }
        bVar1 = *pbVar6;
        param_3 = pbVar4 + 1;
        pbVar6 = pbVar6 + 1;
        *pbVar4 = bVar1;
        *param_5 = *param_5 + 1;
      }
      else {
LAB_00426c21:
        pbVar6 = pbVar6 + 1;
      }
    }
    if (param_3 != (byte *)0x0) {
      *param_3 = 0;
      param_3 = param_3 + 1;
    }
    *param_5 = *param_5 + 1;
  }
  if (param_2 != (undefined4 *)0x0) {
    *param_2 = 0;
  }
  *param_4 = *param_4 + 1;
  return;
}



/* ================= ___crtGetEnvironmentStringsW @ 00426c60 ================= */

/* Library Function - Single Match
    ___crtGetEnvironmentStringsW
   
   Library: Visual Studio 1998 Release */

LPVOID __cdecl ___crtGetEnvironmentStringsW(void)

{
  char cVar1;
  WCHAR WVar2;
  LPWSTR in_EAX;
  LPWSTR pWVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  WCHAR *pWVar8;
  WCHAR *pWVar7;
  uint uVar9;
  LPWCH pWVar10;
  LPWSTR pWVar11;
  LPWSTR pWVar12;
  undefined4 *puVar13;
  LPWSTR pWVar14;
  int local_4;
  
  local_4 = 0;
  pWVar3 = (LPWSTR)0x0;
  if (DAT_0047fd64 == 0) {
    in_EAX = GetEnvironmentStringsW();
    if (in_EAX == (LPWCH)0x0) {
      in_EAX = (LPWSTR)GetEnvironmentStrings();
      if (in_EAX == (LPWSTR)0x0) {
        return (LPVOID)0x0;
      }
      DAT_0047fd64 = 2;
      pWVar3 = in_EAX;
    }
    else {
      DAT_0047fd64 = 1;
      pWVar3 = in_EAX;
    }
  }
  if (DAT_0047fd64 == 1) {
    if ((pWVar3 == (LPWSTR)0x0) && (pWVar3 = GetEnvironmentStringsW(), pWVar3 == (LPWCH)0x0)) {
      return (LPVOID)0x0;
    }
    WVar2 = *pWVar3;
    pWVar7 = pWVar3;
    while (WVar2 != L'\0') {
      do {
        pWVar8 = pWVar7;
        pWVar7 = pWVar8 + 1;
      } while (*pWVar7 != L'\0');
      pWVar7 = pWVar8 + 2;
      WVar2 = *pWVar7;
    }
    uVar9 = (int)pWVar7 + (2 - (int)pWVar3);
    puVar4 = (undefined4 *)FUN_00426250(uVar9);
    if (puVar4 == (undefined4 *)0x0) {
      FreeEnvironmentStringsW(pWVar3);
      return (LPVOID)0x0;
    }
    pWVar10 = pWVar3;
    puVar13 = puVar4;
    for (uVar6 = uVar9 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *puVar13 = *(undefined4 *)pWVar10;
      pWVar10 = pWVar10 + 2;
      puVar13 = puVar13 + 1;
    }
    for (uVar9 = uVar9 & 3; uVar9 != 0; uVar9 = uVar9 - 1) {
      *(char *)puVar13 = (char)*pWVar10;
      pWVar10 = (LPWCH)((int)pWVar10 + 1);
      puVar13 = (undefined4 *)((int)puVar13 + 1);
    }
    FreeEnvironmentStringsW(pWVar3);
    return puVar4;
  }
  if (DAT_0047fd64 == 2) {
    if ((pWVar3 == (LPWSTR)0x0) && (pWVar3 = (LPWSTR)GetEnvironmentStrings(), pWVar3 == (LPWSTR)0x0)
       ) {
      return (LPVOID)0x0;
    }
    cVar1 = (char)*pWVar3;
    pWVar11 = pWVar3;
    while (cVar1 != '\0') {
      iVar5 = MultiByteToWideChar(DAT_0047fe28,1,(LPCSTR)pWVar11,-1,(LPWSTR)0x0,0);
      if (iVar5 == 0) {
        return (LPVOID)0x0;
      }
      uVar9 = 0xffffffff;
      local_4 = local_4 + iVar5;
      pWVar12 = pWVar11;
      do {
        if (uVar9 == 0) break;
        uVar9 = uVar9 - 1;
        WVar2 = *pWVar12;
        pWVar12 = (LPWSTR)((int)pWVar12 + 1);
      } while ((char)WVar2 != '\0');
      pWVar11 = (LPWSTR)((int)pWVar11 + ~uVar9);
      cVar1 = (char)*pWVar11;
    }
    in_EAX = (LPWSTR)FUN_00426250((local_4 + 1) * 2);
    if (in_EAX == (LPWSTR)0x0) {
      FreeEnvironmentStringsA((LPCH)pWVar3);
      return (LPVOID)0x0;
    }
    cVar1 = (char)*pWVar3;
    pWVar11 = in_EAX;
    pWVar12 = pWVar3;
    while (cVar1 != '\0') {
      iVar5 = MultiByteToWideChar(DAT_0047fe28,1,(LPCSTR)pWVar12,-1,pWVar11,
                                  (local_4 + 1) - ((int)pWVar11 - (int)in_EAX >> 1));
      if (iVar5 == 0) {
        FUN_004276c0(in_EAX);
        FreeEnvironmentStringsA((LPCH)pWVar3);
        return (LPVOID)0x0;
      }
      uVar9 = 0xffffffff;
      pWVar14 = pWVar12;
      do {
        if (uVar9 == 0) break;
        uVar9 = uVar9 - 1;
        WVar2 = *pWVar14;
        pWVar14 = (LPWSTR)((int)pWVar14 + 1);
      } while ((char)WVar2 != '\0');
      pWVar12 = (LPWSTR)((int)pWVar12 + ~uVar9);
      iVar5 = FUN_00427c30(pWVar11);
      pWVar11 = pWVar11 + iVar5 + 1;
      cVar1 = (char)*pWVar12;
    }
    *pWVar11 = L'\0';
    FreeEnvironmentStringsA((LPCH)pWVar3);
  }
  return in_EAX;
}



/* ================= FUN_00426e60 @ 00426e60 ================= */

LPSTR FUN_00426e60(void)

{
  char cVar1;
  WCHAR WVar2;
  LPWCH pWVar3;
  SIZE_T cbMultiByte;
  LPSTR pCVar4;
  uint uVar5;
  LPWCH pWVar6;
  char *pcVar7;
  WCHAR *pWVar9;
  WCHAR *pWVar8;
  int iVar10;
  LPSTR pCVar11;
  LPWCH local_4;
  
  if (DAT_0047fd68 == 0) {
    pWVar3 = GetEnvironmentStringsW();
    if (pWVar3 == (LPWCH)0x0) {
      local_4 = (LPWCH)GetEnvironmentStrings();
      if (local_4 == (LPWCH)0x0) {
        return (LPSTR)0x0;
      }
      DAT_0047fd68 = 2;
    }
    else {
      DAT_0047fd68 = 1;
    }
  }
  else {
    pWVar3 = local_4;
  }
  if (DAT_0047fd68 != 1) {
    if (DAT_0047fd68 != 2) {
      return (LPSTR)0x0;
    }
    if ((local_4 == (LPWCH)0x0) && (local_4 = (LPWCH)GetEnvironmentStrings(), local_4 == (LPWCH)0x0)
       ) {
      return (LPSTR)0x0;
    }
    cVar1 = (char)*local_4;
    pWVar3 = local_4;
    while (cVar1 != '\0') {
      do {
        pWVar6 = pWVar3;
        pWVar3 = (LPWCH)((int)pWVar6 + 1);
      } while (*(char *)pWVar3 != '\0');
      pWVar3 = pWVar6 + 1;
      cVar1 = (char)*pWVar3;
    }
    pcVar7 = (char *)((int)pWVar3 + (1 - (int)local_4));
    pCVar4 = (LPSTR)FUN_00426250((SIZE_T)pcVar7);
    if (pCVar4 == (LPSTR)0x0) {
      FreeEnvironmentStringsA((LPCH)local_4);
      return (LPSTR)0x0;
    }
    pWVar3 = local_4;
    pCVar11 = pCVar4;
    for (uVar5 = (uint)pcVar7 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pCVar11 = *(undefined4 *)pWVar3;
      pWVar3 = pWVar3 + 2;
      pCVar11 = pCVar11 + 4;
    }
    for (uVar5 = (uint)pcVar7 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pCVar11 = (CHAR)*pWVar3;
      pWVar3 = (LPWCH)((int)pWVar3 + 1);
      pCVar11 = pCVar11 + 1;
    }
    FreeEnvironmentStringsA((LPCH)local_4);
    return pCVar4;
  }
  if ((pWVar3 == (LPWCH)0x0) && (pWVar3 = GetEnvironmentStringsW(), pWVar3 == (LPWCH)0x0)) {
    return (LPSTR)0x0;
  }
  WVar2 = *pWVar3;
  pWVar8 = pWVar3;
  while (WVar2 != L'\0') {
    do {
      pWVar9 = pWVar8;
      pWVar8 = pWVar9 + 1;
    } while (*pWVar8 != L'\0');
    pWVar8 = pWVar9 + 2;
    WVar2 = *pWVar8;
  }
  iVar10 = ((int)pWVar8 - (int)pWVar3 >> 1) + 1;
  cbMultiByte = WideCharToMultiByte(0,0,pWVar3,iVar10,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
  if ((cbMultiByte == 0) || (pCVar4 = (LPSTR)FUN_00426250(cbMultiByte), pCVar4 == (LPSTR)0x0)) {
    FreeEnvironmentStringsW(pWVar3);
    return (LPSTR)0x0;
  }
  iVar10 = WideCharToMultiByte(0,0,pWVar3,iVar10,pCVar4,cbMultiByte,(LPCSTR)0x0,(LPBOOL)0x0);
  if (iVar10 == 0) {
    FUN_004276c0(pCVar4);
    pCVar4 = (LPSTR)0x0;
  }
  FreeEnvironmentStringsW(pWVar3);
  return pCVar4;
}



/* ================= FUN_00427000 @ 00427000 ================= */

void FUN_00427000(void)

{
  undefined4 *puVar1;
  DWORD DVar2;
  HANDLE hFile;
  byte *pbVar3;
  int *piVar4;
  uint uVar5;
  undefined4 *puVar6;
  UINT UVar7;
  UINT *pUVar8;
  int iVar9;
  _STARTUPINFOA local_44;
  
  puVar1 = (undefined4 *)FUN_00426250(0x100);
  if (puVar1 == (undefined4 *)0x0) {
    __amsg_exit(0x1b);
  }
  DAT_00496050 = 0x20;
  DAT_00495f50 = puVar1;
  if (puVar1 < puVar1 + 0x40) {
    do {
      *(undefined1 *)(puVar1 + 1) = 0;
      puVar6 = puVar1 + 2;
      *puVar1 = 0xffffffff;
      *(undefined1 *)((int)puVar1 + 5) = 10;
      puVar1 = puVar6;
    } while (puVar6 < DAT_00495f50 + 0x40);
  }
  GetStartupInfoA(&local_44);
  if ((local_44.cbReserved2 != 0) && ((UINT *)local_44.lpReserved2 != (UINT *)0x0)) {
    UVar7 = *(UINT *)local_44.lpReserved2;
    pUVar8 = (UINT *)((int)local_44.lpReserved2 + 4);
    pbVar3 = (byte *)((int)pUVar8 + UVar7);
    if (0x7ff < (int)UVar7) {
      UVar7 = 0x800;
    }
    if ((int)DAT_00496050 < (int)UVar7) {
      piVar4 = &DAT_00495f54;
      do {
        puVar1 = (undefined4 *)FUN_00426250(0x100);
        if (puVar1 == (undefined4 *)0x0) {
          UVar7 = DAT_00496050;
          break;
        }
        *piVar4 = (int)puVar1;
        DAT_00496050 = DAT_00496050 + 0x20;
        if (puVar1 < puVar1 + 0x40) {
          do {
            *(undefined1 *)(puVar1 + 1) = 0;
            puVar6 = puVar1 + 2;
            *puVar1 = 0xffffffff;
            *(undefined1 *)((int)puVar1 + 5) = 10;
            puVar1 = puVar6;
          } while (puVar6 < (undefined4 *)(*piVar4 + 0x100));
        }
        piVar4 = piVar4 + 1;
      } while ((int)DAT_00496050 < (int)UVar7);
    }
    uVar5 = 0;
    if (0 < (int)UVar7) {
      do {
        if (((*(HANDLE *)pbVar3 != (HANDLE)0xffffffff) && ((*pUVar8 & 1) != 0)) &&
           (DVar2 = GetFileType(*(HANDLE *)pbVar3), DVar2 != 0)) {
          puVar1 = (undefined4 *)
                   (*(int *)((int)&DAT_00495f50 + ((int)(uVar5 & 0xffffffe7) >> 3)) +
                   (uVar5 & 0x1f) * 8);
          *puVar1 = *(undefined4 *)pbVar3;
          *(byte *)(puVar1 + 1) = (byte)*pUVar8;
        }
        uVar5 = uVar5 + 1;
        pUVar8 = (UINT *)((int)pUVar8 + 1);
        pbVar3 = pbVar3 + 4;
      } while ((int)uVar5 < (int)UVar7);
    }
  }
  iVar9 = 0;
  do {
    piVar4 = DAT_00495f50 + iVar9 * 2;
    if (*piVar4 == -1) {
      DVar2 = 0xfffffff6;
      *(undefined1 *)(piVar4 + 1) = 0x81;
      if (iVar9 != 0) {
        DVar2 = (iVar9 == 1) - 0xc;
      }
      hFile = GetStdHandle(DVar2);
      if ((hFile == (HANDLE)0xffffffff) || (DVar2 = GetFileType(hFile), DVar2 == 0)) {
        *(byte *)(piVar4 + 1) = *(byte *)(piVar4 + 1) | 0x40;
      }
      else {
        *piVar4 = (int)hFile;
        if ((DVar2 & 0xff) == 2) {
          *(byte *)(piVar4 + 1) = *(byte *)(piVar4 + 1) | 0x40;
        }
        else if ((DVar2 & 0xff) == 3) {
          *(byte *)(piVar4 + 1) = *(byte *)(piVar4 + 1) | 8;
        }
      }
    }
    else {
      *(byte *)(piVar4 + 1) = *(byte *)(piVar4 + 1) | 0x80;
    }
    iVar9 = iVar9 + 1;
  } while (iVar9 < 3);
  SetHandleCount(DAT_00496050);
  return;
}



/* ================= FUN_00427210 @ 00427210 ================= */

void FUN_00427210(void)

{
  DAT_00495f40 = HeapCreate(1,0x1000,0);
  return;
}



/* ================= FUN_00427305 @ 00427305 ================= */

void FUN_00427305(int param_1)

{
  __local_unwind2(*(int *)(param_1 + 0x18),*(int *)(param_1 + 0x1c));
  return;
}



/* ================= __FF_MSGBANNER @ 00427320 ================= */

/* Library Function - Single Match
    __FF_MSGBANNER
   
   Library: Visual Studio 1998 Release */

void __cdecl __FF_MSGBANNER(void)

{
  if ((DAT_0047f8ec == 1) || ((DAT_0047f8ec == 0 && (DAT_0047f8f0 == 1)))) {
    FUN_00427360(0xfc);
    if (DAT_0047fe08 != (code *)0x0) {
      (*DAT_0047fe08)();
    }
    FUN_00427360(0xff);
  }
  return;
}



/* ================= FUN_00427360 @ 00427360 ================= */

void __cdecl FUN_00427360(int param_1)

{
  char cVar1;
  int *piVar2;
  DWORD DVar3;
  HANDLE hFile;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  char *pcVar9;
  CHAR *pCVar10;
  char *pcVar11;
  DWORD local_1a8;
  char local_1a4 [100];
  char acStack_140 [60];
  CHAR local_104 [260];
  
  iVar4 = 0;
  piVar2 = &DAT_0047fd80;
  do {
    if (*piVar2 == param_1) break;
    piVar2 = piVar2 + 2;
    iVar4 = iVar4 + 1;
  } while (piVar2 < &DAT_0047fe08);
  if ((&DAT_0047fd80)[iVar4 * 2] == param_1) {
    if ((DAT_0047f8ec == 1) || ((DAT_0047f8ec == 0 && (DAT_0047f8f0 == 1)))) {
      hFile = *(HANDLE *)(DAT_00495f50 + 0x10);
      if (hFile == (HANDLE)0xffffffff) {
        hFile = GetStdHandle(0xfffffff4);
      }
      pcVar8 = *(char **)(iVar4 * 8 + 0x47fd84);
      uVar6 = 0xffffffff;
      pcVar9 = pcVar8;
      do {
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar9 + 1;
      } while (cVar1 != '\0');
      WriteFile(hFile,pcVar8,~uVar6 - 1,&local_1a8,(LPOVERLAPPED)0x0);
    }
    else if (param_1 != 0xfc) {
      DVar3 = GetModuleFileNameA((HMODULE)0x0,local_104,0x104);
      if (DVar3 == 0) {
        pcVar8 = "<program name unknown>";
        pCVar10 = local_104;
        for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
          *(undefined4 *)pCVar10 = *(undefined4 *)pcVar8;
          pcVar8 = pcVar8 + 4;
          pCVar10 = pCVar10 + 4;
        }
        *(undefined2 *)pCVar10 = *(undefined2 *)pcVar8;
        pCVar10[2] = pcVar8[2];
      }
      pcVar8 = local_104;
      uVar6 = 0xffffffff;
      pcVar9 = local_104;
      do {
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar9 + 1;
      } while (cVar1 != '\0');
      if (0x3c < ~uVar6) {
        uVar6 = 0xffffffff;
        pcVar8 = local_104;
        do {
          if (uVar6 == 0) break;
          uVar6 = uVar6 - 1;
          cVar1 = *pcVar8;
          pcVar8 = pcVar8 + 1;
        } while (cVar1 != '\0');
        pcVar8 = acStack_140 + ~uVar6;
        _strncpy(pcVar8,"...",3);
      }
      pcVar9 = "Runtime Error!\n\nProgram: ";
      pcVar11 = local_1a4;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar9;
        pcVar9 = pcVar9 + 4;
        pcVar11 = pcVar11 + 4;
      }
      *(undefined2 *)pcVar11 = *(undefined2 *)pcVar9;
      uVar6 = 0xffffffff;
      do {
        pcVar9 = pcVar8;
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        pcVar9 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar9;
      } while (cVar1 != '\0');
      uVar6 = ~uVar6;
      iVar5 = -1;
      pcVar8 = local_1a4;
      do {
        pcVar11 = pcVar8;
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        pcVar11 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar11;
      } while (cVar1 != '\0');
      pcVar8 = pcVar9 + -uVar6;
      pcVar9 = pcVar11 + -1;
      for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pcVar9 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      }
      uVar6 = 0xffffffff;
      pcVar8 = "\n\n";
      do {
        pcVar9 = pcVar8;
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        pcVar9 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar9;
      } while (cVar1 != '\0');
      uVar6 = ~uVar6;
      iVar5 = -1;
      pcVar8 = local_1a4;
      do {
        pcVar11 = pcVar8;
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        pcVar11 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar11;
      } while (cVar1 != '\0');
      pcVar8 = pcVar9 + -uVar6;
      pcVar9 = pcVar11 + -1;
      for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pcVar9 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      }
      uVar6 = 0xffffffff;
      pcVar8 = *(char **)(iVar4 * 8 + 0x47fd84);
      do {
        pcVar9 = pcVar8;
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        pcVar9 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar9;
      } while (cVar1 != '\0');
      uVar6 = ~uVar6;
      iVar4 = -1;
      pcVar8 = local_1a4;
      do {
        pcVar11 = pcVar8;
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        pcVar11 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar11;
      } while (cVar1 != '\0');
      pcVar8 = pcVar9 + -uVar6;
      pcVar9 = pcVar11 + -1;
      for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pcVar9 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      }
      ___crtMessageBoxA(local_1a4,"Microsoft Visual C++ Runtime Library",0x12010);
      return;
    }
  }
  return;
}



/* ================= _strncpy @ 00427580 ================= */

/* Library Function - Single Match
    _strncpy
   
   Library: Visual Studio 1998 Release */

char * __cdecl _strncpy(char *_Dest,char *_Source,size_t _Count)

{
  uint uVar1;
  uint uVar2;
  char cVar3;
  uint uVar4;
  uint *puVar5;
  
  if (_Count == 0) {
    return _Dest;
  }
  puVar5 = (uint *)_Dest;
  if (((uint)_Source & 3) != 0) {
    while( true ) {
      uVar4 = *(uint *)_Source;
      _Source = (char *)((int)_Source + 1);
      *(char *)puVar5 = (char)uVar4;
      puVar5 = (uint *)((int)puVar5 + 1);
      _Count = _Count - 1;
      if (_Count == 0) {
        return _Dest;
      }
      if ((char)uVar4 == '\0') break;
      if (((uint)_Source & 3) == 0) {
        uVar4 = _Count >> 2;
        goto joined_r0x004275be;
      }
    }
    do {
      if (((uint)puVar5 & 3) == 0) {
        uVar4 = _Count >> 2;
        cVar3 = '\0';
        if (uVar4 == 0) goto LAB_004275fb;
        goto LAB_00427669;
      }
      *(char *)puVar5 = '\0';
      puVar5 = (uint *)((int)puVar5 + 1);
      _Count = _Count - 1;
    } while (_Count != 0);
    return _Dest;
  }
  uVar4 = _Count >> 2;
  if (uVar4 != 0) {
    do {
      uVar1 = *(uint *)_Source;
      uVar2 = *(uint *)_Source;
      _Source = (char *)((int)_Source + 4);
      if (((uVar1 ^ 0xffffffff ^ uVar1 + 0x7efefeff) & 0x81010100) != 0) {
        if ((char)uVar2 == '\0') {
          *puVar5 = 0;
joined_r0x00427665:
          while( true ) {
            uVar4 = uVar4 - 1;
            puVar5 = puVar5 + 1;
            if (uVar4 == 0) break;
LAB_00427669:
            *puVar5 = 0;
          }
          cVar3 = '\0';
          _Count = _Count & 3;
          if (_Count != 0) goto LAB_004275fb;
          return _Dest;
        }
        if ((char)(uVar2 >> 8) == '\0') {
          *puVar5 = uVar2 & 0xff;
          goto joined_r0x00427665;
        }
        if ((uVar2 & 0xff0000) == 0) {
          *puVar5 = uVar2 & 0xffff;
          goto joined_r0x00427665;
        }
        if ((uVar2 & 0xff000000) == 0) {
          *puVar5 = uVar2;
          goto joined_r0x00427665;
        }
      }
      *puVar5 = uVar2;
      puVar5 = puVar5 + 1;
      uVar4 = uVar4 - 1;
joined_r0x004275be:
    } while (uVar4 != 0);
    _Count = _Count & 3;
    if (_Count == 0) {
      return _Dest;
    }
  }
  do {
    cVar3 = (char)*(uint *)_Source;
    _Source = (char *)((int)_Source + 1);
    *(char *)puVar5 = cVar3;
    puVar5 = (uint *)((int)puVar5 + 1);
    if (cVar3 == '\0') {
      while (_Count = _Count - 1, _Count != 0) {
LAB_004275fb:
        *(char *)puVar5 = cVar3;
        puVar5 = (uint *)((int)puVar5 + 1);
      }
      return _Dest;
    }
    _Count = _Count - 1;
  } while (_Count != 0);
  return _Dest;
}



/* ================= FUN_004276a0 @ 004276a0 ================= */

undefined4 __cdecl FUN_004276a0(undefined4 param_1)

{
  int iVar1;
  
  if (DAT_00481a64 != (code *)0x0) {
    iVar1 = (*DAT_00481a64)(param_1);
    if (iVar1 != 0) {
      return 1;
    }
  }
  return 0;
}



/* ================= FUN_004276c0 @ 004276c0 ================= */

void __cdecl FUN_004276c0(LPVOID param_1)

{
  if (param_1 != (LPVOID)0x0) {
    HeapFree(DAT_00495f40,0,param_1);
  }
  return;
}



/* ================= FUN_00427930 @ 00427930 ================= */

int __cdecl
FUN_00427930(LCID param_1,DWORD param_2,char *param_3,size_t param_4,LPWSTR param_5,int param_6,
            UINT param_7)

{
  int iVar1;
  LPWSTR lpWideCharStr;
  int iVar2;
  LPWSTR lpDestStr;
  
  if (DAT_0047fe34 == 0) {
    iVar1 = LCMapStringA(0,0x100,"",1,(LPSTR)0x0,0);
    if (iVar1 == 0) {
      iVar1 = LCMapStringW(0,0x100,L"",1,(LPWSTR)0x0,0);
      if (iVar1 == 0) {
        return 0;
      }
      DAT_0047fe34 = 1;
    }
    else {
      DAT_0047fe34 = 2;
    }
  }
  if (0 < (int)param_4) {
    param_4 = _strncnt(param_3,param_4);
  }
  if (DAT_0047fe34 == 2) {
    iVar1 = LCMapStringA(param_1,param_2,param_3,param_4,(LPSTR)param_5,param_6);
    return iVar1;
  }
  if (DAT_0047fe34 != 1) {
    return DAT_0047fe34;
  }
  lpDestStr = (LPWSTR)0x0;
  if (param_7 == 0) {
    param_7 = DAT_0047fe28;
  }
  iVar1 = MultiByteToWideChar(param_7,9,param_3,param_4,(LPWSTR)0x0,0);
  if (iVar1 == 0) {
    return 0;
  }
  lpWideCharStr = (LPWSTR)FUN_00426250(iVar1 * 2);
  if (lpWideCharStr == (LPWSTR)0x0) {
    return 0;
  }
  iVar2 = MultiByteToWideChar(param_7,1,param_3,param_4,lpWideCharStr,iVar1);
  if ((iVar2 != 0) &&
     (iVar2 = LCMapStringW(param_1,param_2,lpWideCharStr,iVar1,(LPWSTR)0x0,0), iVar2 != 0)) {
    if ((param_2 & 0x400) == 0) {
      lpDestStr = (LPWSTR)FUN_00426250(iVar2 * 2);
      if ((lpDestStr == (LPWSTR)0x0) ||
         (iVar1 = LCMapStringW(param_1,param_2,lpWideCharStr,iVar1,lpDestStr,iVar2), iVar1 == 0))
      goto LAB_00427aab;
      if (param_6 == 0) {
        iVar2 = WideCharToMultiByte(param_7,0x220,lpDestStr,iVar2,(LPSTR)0x0,0,(LPCSTR)0x0,
                                    (LPBOOL)0x0);
        iVar1 = iVar2;
      }
      else {
        iVar2 = WideCharToMultiByte(param_7,0x220,lpDestStr,iVar2,(LPSTR)param_5,param_6,(LPCSTR)0x0
                                    ,(LPBOOL)0x0);
        iVar1 = iVar2;
      }
    }
    else {
      if (param_6 == 0) goto LAB_00427b42;
      if (param_6 < iVar2) goto LAB_00427aab;
      iVar1 = LCMapStringW(param_1,param_2,lpWideCharStr,iVar1,param_5,param_6);
    }
    if (iVar1 != 0) {
LAB_00427b42:
      FUN_004276c0(lpWideCharStr);
      FUN_004276c0(lpDestStr);
      return iVar2;
    }
  }
LAB_00427aab:
  FUN_004276c0(lpWideCharStr);
  FUN_004276c0(lpDestStr);
  return 0;
}



/* ================= _strncnt @ 00427b60 ================= */

/* Library Function - Single Match
    _strncnt
   
   Library: Visual Studio 1998 Release */

size_t __cdecl _strncnt(char *_String,size_t _Cnt)

{
  size_t sVar1;
  char *pcVar2;
  
  pcVar2 = _String;
  sVar1 = _Cnt;
  while (sVar1 != 0) {
    sVar1 = sVar1 - 1;
    if (*pcVar2 == '\0') goto LAB_00427b85;
    pcVar2 = pcVar2 + 1;
  }
  if (*pcVar2 == '\0') {
LAB_00427b85:
    _Cnt = (int)pcVar2 - (int)_String;
  }
  return _Cnt;
}



/* ================= __isctype @ 00427b90 ================= */

/* Library Function - Single Match
    __isctype
   
   Library: Visual Studio 1998 Release */

int __cdecl __isctype(int _C,int _Type)

{
  LPCSTR _LpSrcStr;
  BOOL BVar1;
  byte bVar2;
  BOOL unaff_EBX;
  undefined4 local_6;
  undefined1 local_2;
  
  if (_C + 1U < 0x101) {
    return (uint)*(ushort *)(PTR_DAT_0047fe38 + _C * 2) & _Type;
  }
  bVar2 = (byte)((uint)_C >> 8);
  if ((PTR_DAT_0047fe38[(uint)bVar2 * 2 + 1] & 0x80) == 0) {
    _LpSrcStr = (LPCSTR)0x1;
    local_6._0_3_ = CONCAT12((char)_C,(undefined2)local_6);
    local_6 = (uint)(uint3)local_6;
  }
  else {
    _LpSrcStr = (LPCSTR)0x2;
    local_6._0_3_ = CONCAT12(bVar2,(undefined2)local_6);
    local_2 = 0;
    local_6 = CONCAT13((char)_C,(uint3)local_6);
  }
  BVar1 = ___crtGetStringTypeA
                    ((_locale_t)0x1,(int)&local_6 + 2,_LpSrcStr,(int)&local_6,(LPWORD)0x0,0,
                     unaff_EBX);
  if (BVar1 == 0) {
    return 0;
  }
  return local_6 & 0xffff & _Type;
}



/* ================= FUN_00427c30 @ 00427c30 ================= */

int __cdecl FUN_00427c30(short *param_1)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  psVar2 = param_1;
  do {
    psVar3 = psVar2 + 1;
    sVar1 = *psVar2;
    psVar2 = psVar3;
  } while (sVar1 != 0);
  return ((int)psVar3 - (int)param_1 >> 1) + -1;
}



/* ================= ___crtMessageBoxA @ 00427c50 ================= */

/* Library Function - Single Match
    ___crtMessageBoxA
   
   Library: Visual Studio 1998 Release */

int __cdecl ___crtMessageBoxA(LPCSTR _LpText,LPCSTR _LpCaption,UINT _UType)

{
  HMODULE hModule;
  int iVar1;
  
  iVar1 = 0;
  if (DAT_00480050 != (FARPROC)0x0) {
LAB_00427c9f:
    if (DAT_00480054 != (FARPROC)0x0) {
      iVar1 = (*DAT_00480054)();
    }
    if ((iVar1 != 0) && (DAT_00480058 != (FARPROC)0x0)) {
      iVar1 = (*DAT_00480058)(iVar1);
    }
    iVar1 = (*DAT_00480050)(iVar1,_LpText,_LpCaption,_UType);
    return iVar1;
  }
  hModule = LoadLibraryA("user32.dll");
  if (hModule != (HMODULE)0x0) {
    DAT_00480050 = GetProcAddress(hModule,"MessageBoxA");
    if (DAT_00480050 != (FARPROC)0x0) {
      DAT_00480054 = GetProcAddress(hModule,"GetActiveWindow");
      DAT_00480058 = GetProcAddress(hModule,"GetLastActivePopup");
      goto LAB_00427c9f;
    }
  }
  return 0;
}



/* ================= ___crtGetStringTypeW @ 00427cf0 ================= */

/* Library Function - Single Match
    ___crtGetStringTypeW
   
   Library: Visual Studio 1998 Release */

BOOL __cdecl
___crtGetStringTypeW
          (DWORD param_1,LPCWSTR param_2,int param_3,LPWORD param_4,UINT param_5,LCID param_6)

{
  LPWORD pWVar1;
  BOOL in_EAX;
  BOOL BVar2;
  int cbMultiByte;
  LPSTR lpMultiByteStr;
  int iVar3;
  LPWORD lpCharType;
  BOOL local_4;
  
  local_4 = in_EAX;
  if (DAT_0048005c == 0) {
    local_4 = GetStringTypeW(1,L"",1,(LPWORD)&local_4);
    if (local_4 == 0) {
      local_4 = GetStringTypeA(0,1,"",1,(LPWORD)&local_4);
      if (local_4 == 0) {
        return 0;
      }
      DAT_0048005c = 2;
    }
    else {
      DAT_0048005c = 1;
    }
  }
  if (DAT_0048005c == 1) {
    BVar2 = GetStringTypeW(param_1,param_2,param_3,param_4);
    return BVar2;
  }
  if (DAT_0048005c == 2) {
    lpCharType = (LPWORD)0x0;
    local_4 = 0;
    if (param_5 == 0) {
      param_5 = DAT_0047fe28;
    }
    cbMultiByte = WideCharToMultiByte(param_5,0x220,param_2,param_3,(LPSTR)0x0,0,(LPCSTR)0x0,
                                      (LPBOOL)0x0);
    if (cbMultiByte == 0) {
      return 0;
    }
    lpMultiByteStr = FUN_00428130(1,cbMultiByte);
    if (lpMultiByteStr == (LPSTR)0x0) {
      return 0;
    }
    iVar3 = WideCharToMultiByte(param_5,0x220,param_2,param_3,lpMultiByteStr,cbMultiByte,(LPCSTR)0x0
                                ,(LPBOOL)0x0);
    if ((iVar3 != 0) &&
       (lpCharType = (LPWORD)FUN_00426250(cbMultiByte * 2 + 2), lpCharType != (LPWORD)0x0)) {
      if (param_6 == 0) {
        param_6 = DAT_0047fe18;
      }
      pWVar1 = lpCharType + param_3;
      *pWVar1 = 0xffff;
      pWVar1[-1] = 0xffff;
      local_4 = GetStringTypeA(param_6,param_1,lpMultiByteStr,cbMultiByte,lpCharType);
      if ((pWVar1[-1] == 0xffff) || (*pWVar1 != 0xffff)) {
        local_4 = 0;
      }
      else {
        FID_conflict__memcpy(param_4,lpCharType,param_3 * 2);
      }
    }
    FUN_004276c0(lpMultiByteStr);
    FUN_004276c0(lpCharType);
  }
  return local_4;
}



/* ================= ___crtGetStringTypeA @ 00427eb0 ================= */

/* Library Function - Single Match
    ___crtGetStringTypeA
   
   Library: Visual Studio 1998 Release */

BOOL __cdecl
___crtGetStringTypeA
          (_locale_t _Plocinfo,DWORD _DWInfoType,LPCSTR _LpSrcStr,int _CchSrc,LPWORD _LpCharType,
          int _Code_page,BOOL _BError)

{
  BOOL BVar1;
  int iVar2;
  int iVar3;
  LPWSTR lpWideCharStr;
  WORD local_2;
  
  iVar3 = DAT_00480060;
  if (DAT_00480060 == 0) {
    BVar1 = GetStringTypeA(0,1,"",1,&local_2);
    if (BVar1 == 0) {
      BVar1 = GetStringTypeW(1,L"",1,&local_2);
      if (BVar1 == 0) {
        return 0;
      }
      iVar3 = 1;
    }
    else {
      iVar3 = 2;
    }
  }
  DAT_00480060 = iVar3;
  if (iVar3 == 2) {
    if (_Code_page == 0) {
      _Code_page = DAT_0047fe18;
    }
    BVar1 = GetStringTypeA(_Code_page,(DWORD)_Plocinfo,(LPCSTR)_DWInfoType,(int)_LpSrcStr,
                           (LPWORD)_CchSrc);
    return BVar1;
  }
  if (iVar3 == 1) {
    BVar1 = 0;
    lpWideCharStr = (LPWSTR)0x0;
    if (_LpCharType == (LPWORD)0x0) {
      _LpCharType = DAT_0047fe28;
    }
    iVar2 = MultiByteToWideChar((UINT)_LpCharType,9,(LPCSTR)_DWInfoType,(int)_LpSrcStr,(LPWSTR)0x0,0
                               );
    iVar3 = BVar1;
    if (((iVar2 != 0) && (lpWideCharStr = FUN_00428130(2,iVar2), lpWideCharStr != (LPWSTR)0x0)) &&
       (iVar2 = MultiByteToWideChar((UINT)_LpCharType,1,(LPCSTR)_DWInfoType,(int)_LpSrcStr,
                                    lpWideCharStr,iVar2), iVar2 != 0)) {
      iVar3 = GetStringTypeW((DWORD)_Plocinfo,lpWideCharStr,iVar2,(LPWORD)_CchSrc);
    }
    FUN_004276c0(lpWideCharStr);
  }
  return iVar3;
}



/* ================= FID_conflict:_memcpy @ 00427fe0 ================= */

/* Library Function - Multiple Matches With Different Base Names
    _memcpy
    _memmove
   
   Libraries: Visual Studio 1998 Debug, Visual Studio 1998 Release */

void * __cdecl FID_conflict__memcpy(void *_Dst,void *_Src,size_t _Size)

{
  uint uVar1;
  int in_EDX;
  uint uVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  undefined1 *puVar6;
  
  if ((_Src < _Dst) && (_Dst < (void *)((int)_Src + _Size))) {
    puVar3 = (undefined4 *)((int)_Src + _Size);
    puVar5 = (undefined4 *)((int)_Dst + _Size);
    if (((uint)puVar5 & 3) == 0) {
      uVar1 = _Size >> 2;
      while( true ) {
        puVar5 = puVar5 + -1;
        puVar3 = puVar3 + -1;
        if (uVar1 == 0) break;
        uVar1 = uVar1 - 1;
        *puVar5 = *puVar3;
      }
      switch(_Size & 3) {
      case 1:
switchD_004280a9_caseD_1:
        *(undefined1 *)((int)puVar5 + 3) = *(undefined1 *)((int)puVar3 + 3);
        return _Dst;
      case 2:
switchD_004280a9_caseD_2:
        *(undefined2 *)((int)puVar5 + 2) = *(undefined2 *)((int)puVar3 + 2);
        return _Dst;
      case 3:
switchD_004280a9_caseD_3:
        *(undefined2 *)((int)puVar5 + 2) = *(undefined2 *)((int)puVar3 + 2);
        *(undefined1 *)((int)puVar5 + 1) = *(undefined1 *)((int)puVar3 + 1);
        return _Dst;
      }
    }
    else {
      puVar4 = (undefined1 *)((int)puVar3 + -1);
      puVar6 = (undefined1 *)((int)puVar5 + -1);
      if (_Size < 0xd) {
        for (; _Size != 0; _Size = _Size - 1) {
          *puVar6 = *puVar4;
          puVar4 = puVar4 + -1;
          puVar6 = puVar6 + -1;
        }
        return _Dst;
      }
      uVar2 = -in_EDX & 3;
      uVar1 = _Size - uVar2;
      for (; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puVar6 = *puVar4;
        puVar4 = puVar4 + -1;
        puVar6 = puVar6 + -1;
      }
      puVar3 = (undefined4 *)(puVar4 + -3);
      puVar5 = (undefined4 *)(puVar6 + -3);
      for (uVar2 = uVar1 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + -1;
        puVar5 = puVar5 + -1;
      }
      switch(uVar1 & 3) {
      case 1:
        goto switchD_004280a9_caseD_1;
      case 2:
        goto switchD_004280a9_caseD_2;
      case 3:
        goto switchD_004280a9_caseD_3;
      }
    }
    return _Dst;
  }
  puVar3 = _Dst;
  if (((uint)_Dst & 3) == 0) {
    for (uVar1 = _Size >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
      *puVar3 = *(undefined4 *)_Src;
      _Src = (undefined4 *)((int)_Src + 4);
      puVar3 = puVar3 + 1;
    }
    switch(_Size & 3) {
    case 1:
switchD_00428010_caseD_1:
      *(undefined1 *)puVar3 = *(undefined1 *)_Src;
      return _Dst;
    case 2:
switchD_00428010_caseD_2:
      *(undefined2 *)puVar3 = *(undefined2 *)_Src;
      return _Dst;
    case 3:
switchD_00428010_caseD_3:
      *(undefined2 *)puVar3 = *(undefined2 *)_Src;
      *(undefined1 *)((int)puVar3 + 2) = *(undefined1 *)((int)_Src + 2);
      return _Dst;
    }
  }
  else {
    puVar4 = _Dst;
    if (_Size < 0xd) {
      for (; _Size != 0; _Size = _Size - 1) {
        *puVar4 = *(undefined1 *)_Src;
        _Src = (undefined1 *)((int)_Src + 1);
        puVar4 = puVar4 + 1;
      }
      return _Dst;
    }
    uVar2 = -(int)_Dst & 3;
    uVar1 = _Size - uVar2;
    for (; uVar2 != 0; uVar2 = uVar2 - 1) {
      *(undefined1 *)puVar3 = *(undefined1 *)_Src;
      _Src = (undefined4 *)((int)_Src + 1);
      puVar3 = (undefined4 *)((int)puVar3 + 1);
    }
    for (uVar2 = uVar1 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
      *puVar3 = *(undefined4 *)_Src;
      _Src = (undefined4 *)((int)_Src + 4);
      puVar3 = puVar3 + 1;
    }
    switch(uVar1 & 3) {
    case 1:
      goto switchD_00428010_caseD_1;
    case 2:
      goto switchD_00428010_caseD_2;
    case 3:
      goto switchD_00428010_caseD_3;
    }
  }
  return _Dst;
}



/* ================= FUN_00428130 @ 00428130 ================= */

LPVOID __cdecl FUN_00428130(int param_1,int param_2)

{
  LPVOID pvVar1;
  int iVar2;
  uint dwBytes;
  
  dwBytes = param_2 * param_1;
  if (dwBytes == 0) {
    dwBytes = 1;
  }
  do {
    if (dwBytes < 0xffffffe1) {
      pvVar1 = HeapAlloc(DAT_00495f40,8,dwBytes);
    }
    else {
      pvVar1 = (LPVOID)0x0;
    }
    if (pvVar1 != (LPVOID)0x0) {
      return pvVar1;
    }
    if (DAT_0047fe2c == 0) {
      return (LPVOID)0x0;
    }
    iVar2 = FUN_004276a0(dwBytes);
  } while (iVar2 != 0);
  return (LPVOID)0x0;
}



/* ================= RtlUnwind @ 004281f6 ================= */

void RtlUnwind(PVOID TargetFrame,PVOID TargetIp,PEXCEPTION_RECORD ExceptionRecord,PVOID ReturnValue)

{
                    /* WARNING: Could not recover jumptable at 0x004281f6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  RtlUnwind(TargetFrame,TargetIp,ExceptionRecord,ReturnValue);
  return;
}



/* ================= __strupr @ 00428270 ================= */

/* Library Function - Single Match
    __strupr
   
   Library: Visual Studio 1998 Release */

char * __cdecl __strupr(char *_String)

{
  char cVar1;
  WCHAR WVar2;
  char *pcVar3;
  SIZE_T SVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  LPWSTR pWVar8;
  LPWSTR pWVar9;
  LPWSTR pWVar10;
  char *pcVar11;
  
  pWVar8 = (LPWSTR)0x0;
  if (DAT_0047fe18 == 0) {
    DAT_0047fe18 = 0;
    cVar1 = *_String;
    pcVar3 = _String;
    while (cVar1 != '\0') {
      cVar1 = *pcVar3;
      if (('`' < cVar1) && (cVar1 < '{')) {
        *pcVar3 = cVar1 + -0x20;
      }
      pcVar3 = pcVar3 + 1;
      cVar1 = *pcVar3;
    }
    return _String;
  }
  SVar4 = FUN_00427930(DAT_0047fe18,0x200,_String,0xffffffff,(LPWSTR)0x0,0,0);
  if (((SVar4 != 0) && (pWVar8 = (LPWSTR)FUN_00426250(SVar4), pWVar8 != (LPWSTR)0x0)) &&
     (iVar5 = FUN_00427930(DAT_0047fe18,0x200,_String,0xffffffff,pWVar8,SVar4,0), iVar5 != 0)) {
    uVar6 = 0xffffffff;
    pWVar9 = pWVar8;
    do {
      pWVar10 = pWVar9;
      if (uVar6 == 0) break;
      uVar6 = uVar6 - 1;
      pWVar10 = (LPWSTR)((int)pWVar9 + 1);
      WVar2 = *pWVar9;
      pWVar9 = pWVar10;
    } while ((char)WVar2 != '\0');
    uVar6 = ~uVar6;
    pcVar3 = (char *)((int)pWVar10 - uVar6);
    pcVar11 = _String;
    for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined4 *)pcVar11 = *(undefined4 *)pcVar3;
      pcVar3 = pcVar3 + 4;
      pcVar11 = pcVar11 + 4;
    }
    for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
      *pcVar11 = *pcVar3;
      pcVar3 = pcVar3 + 1;
      pcVar11 = pcVar11 + 1;
    }
  }
  FUN_004276c0(pWVar8);
  return _String;
}



