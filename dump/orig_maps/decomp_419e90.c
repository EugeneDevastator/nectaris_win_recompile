// address 00419e90 function FUN_00419e90
// body 00419e90 .. 0041a0f5
// callers:
//   FUN_004208a0 @ 004208a0

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



// ---- disassembly ----
00419e50  PUSH EBX
00419e51  XOR AL,AL
00419e53  MOV BL,byte ptr [0x00495c52]
00419e59  SHL BL,0x4
00419e5c  XOR EDX,EDX
00419e5e  MOV CL,byte ptr [0x0048327d]
00419e64  MOV DL,BL
00419e66  CMP byte ptr [EDX + 0x47f7d0],CL
00419e6c  JNZ 0x00419e7c
00419e6e  MOV CL,byte ptr [0x0048327e]
00419e74  CMP byte ptr [EDX + 0x47f7d1],CL
00419e7a  JZ 0x00419e89
00419e7c  ADD BL,0x2
00419e7f  INC AL
00419e81  CMP AL,0x7
00419e83  JNZ 0x00419e5c
00419e85  MOV AL,0xff
00419e87  POP EBX
00419e88  RET
00419e89  INC AL
00419e8b  POP EBX
00419e8c  RET
00419e90  SUB ESP,0x4
00419e93  XOR EAX,EAX
00419e95  MOV ECX,0x8
00419e9a  PUSH EBX
00419e9b  PUSH EDI
00419e9c  MOV EDI,0x481e10
00419ea1  STOSD.REP ES:EDI
00419ea3  STOSW ES:EDI
00419ea5  MOV EDI,0x482050
00419eaa  MOV ECX,0x8
00419eaf  STOSD.REP ES:EDI
00419eb1  STOSW ES:EDI
00419eb3  MOV AL,[0x00481e73]
00419eb8  TEST AL,AL
00419eba  JZ 0x0041a073
00419ec0  CMP AL,0x1
00419ec2  JNZ 0x00419f4b
00419ec8  MOV AL,[0x00481e72]
00419ecd  DEC AL
00419ecf  MOV [0x00481e72],AL
00419ed4  CMP AL,0xff
00419ed6  JNZ 0x00419eda
00419ed8  MOV AL,0x15
00419eda  PUSH 0x7c1f
00419edf  MOV [0x00481e72],AL
00419ee4  MOV dword ptr [0x00482338],0x81
00419eee  MOVZX AX,AL
00419ef2  SHL AX,0x3
00419ef6  MOV [0x004834a2],AX
00419efc  CALL 0x0040da00
00419f01  MOV byte ptr [ESP + 0xf],0x14
00419f06  ADD ESP,0x4
00419f09  MOV EAX,[0x00482338]
00419f0e  MOV BL,0x1f
00419f10  PUSH EAX
00419f11  CALL 0x0040da10
00419f16  ADD ESP,0x4
00419f19  CALL 0x0040da80
00419f1e  MOVZX EAX,AX
00419f21  PUSH EAX
00419f22  CALL 0x0040da20
00419f27  ADD ESP,0x4
00419f2a  DEC BL
00419f2c  JNZ 0x00419f19
00419f2e  ADD dword ptr [0x00482338],0x40
00419f35  DEC byte ptr [ESP + 0xb]
00419f39  JNZ 0x00419f09
00419f3b  CALL 0x0041a100
00419f40  CALL 0x0041a6c0
00419f45  POP EDI
00419f46  POP EBX
00419f47  ADD ESP,0x4
00419f4a  RET
00419f4b  CMP AL,0x2
00419f4d  JNZ 0x00419fdf
00419f53  MOV AL,[0x00481e71]
00419f58  MOV dword ptr [0x00482338],0x81
00419f62  MOV dword ptr [0x0048233c],0x7c01
00419f6c  MOV byte ptr [ESP + 0xb],0x15
00419f71  DEC AL
00419f73  AND AL,0x1f
00419f75  MOV [0x00481e71],AL
00419f7a  MOVZX AX,AL
00419f7e  SHL AX,0x3
00419f82  MOV [0x004836e0],AX
00419f88  MOV EAX,[0x0048233c]
00419f8d  PUSH EAX
00419f8e  CALL 0x0040da00
00419f93  ADD ESP,0x4
00419f96  MOV EAX,[0x00482338]
00419f9b  MOV BL,0x1e
00419f9d  PUSH EAX
00419f9e  CALL 0x0040da10
00419fa3  ADD ESP,0x4
00419fa6  CALL 0x0040da80
00419fab  MOVZX EAX,AX
00419fae  PUSH EAX
00419faf  CALL 0x0040da20
00419fb4  ADD ESP,0x4
00419fb7  DEC BL
00419fb9  JNZ 0x00419fa6
00419fbb  ADD dword ptr [0x00482338],0x40
00419fc2  ADD dword ptr [0x0048233c],0x1f
00419fc9  DEC byte ptr [ESP + 0xb]
00419fcd  JNZ 0x00419f88
00419fcf  CALL 0x0041a270
00419fd4  CALL 0x0041a6c0
00419fd9  POP EDI
00419fda  POP EBX
00419fdb  ADD ESP,0x4
00419fde  RET
00419fdf  CMP AL,0x3
