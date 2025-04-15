// define system call
// Copyright (C) 2025 TcbnErik
//
// This file is part of tracex.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "syscall.h"

#include <stdio.h>

// _countof()
#define C(array) (sizeof(array) / sizeof(array[0]))

static const SystemCall HumanList[256] = {
    {"exit", NULL},     // 0x00
    {"getchar", NULL},  // 0x01
    {"putchar", "w"},   // 0x02
    {"cominp", NULL},   // 0x03
    {"comout", "w"},    // 0x04
    {"prnout", "w"},    // 0x05
    {"inpout", "w"},    // 0x06
    {"inkey", NULL},    // 0x07
    {"getc", NULL},     // 0x08
    {"print", "s"},     // 0x09
    {"gets", "p"},      // 0x0a
    {"keysns", NULL},   // 0x0b
    {"kflush", "w?"},   // 0x0c
    {"fflush", NULL},   // 0x0d
    {"chgdrv", "w"},    // 0x0e
    {"drvctrl", "w"},   // 0x0f

    {"consns", NULL},    // 0x10
    {"prnsns", NULL},    // 0x11
    {"cinsns", NULL},    // 0x12
    {"coutsns", NULL},   // 0x13
    {NULL, NULL},        // 0x14
    {NULL, NULL},        // 0x15
    {NULL, NULL},        // 0x16
    {"fatchk", "sp?"},   // 0x17
    {"hendsp", "w?"},    // 0x18
    {"curdrv", NULL},    // 0x19
    {"getss", "p"},      // 0x1a
    {"fgetc", "w"},      // 0x1b
    {"fgets", "pw"},     // 0x1c
    {"fputc", "ww"},     // 0x1d
    {"fputs", "sw"},     // 0x1e
    {"allclose", NULL},  // 0x1f

    {"super", "p"},     // 0x20
    {"fnckey", "wp"},   // 0x21
    {"knjctrl", "l?"},  // 0x22
    {"conctrl", "w?"},  // 0x23
    {"keyctrl", "w?"},  // 0x24
    {"intvcs", "wp"},   // 0x25
    {"pspset", "p"},    // 0x26
    {"gettim2", NULL},  // 0x27
    {"settim2", "l"},   // 0x28
    {"namests", "sp"},  // 0x29
    {"getdate", NULL},  // 0x2a
    {"setdate", "w"},   // 0x2b
    {"gettime", NULL},  // 0x2c
    {"settime", "w"},   // 0x2d
    {"verify", "w"},    // 0x2e
    {"dup0", "ww"},     // 0x2f

    {"vernum", NULL},   // 0x30
    {"keeppr", "lw"},   // 0x31
    {"getdpb", "wp"},   // 0x32
    {"breakck", "w"},   // 0x33
    {"drvxchg", "ww"},  // 0x34
    {"intvcg", "w"},    // 0x35
    {"dskfre", "wp"},   // 0x36
    {"nameck", "sp"},   // 0x37
    {NULL, NULL},       // 0x38
    {"mkdir", "s"},     // 0x39
    {"rmdir", "s"},     // 0x3a
    {"chdir", "s"},     // 0x3b
    {"create", "sw"},   // 0x3c
    {"open", "sw"},     // 0x3d
    {"close", "w"},     // 0x3e
    {"read", "wpl"},    // 0x3f

    {"write", "wpl"},    // 0x40
    {"delete", "s"},     // 0x41
    {"seek", "wlw"},     // 0x42
    {"chmod", "sw"},     // 0x43
    {"ioctrl", "w?"},    // 0x44
    {"dup", "w"},        // 0x45
    {"dup2", "ww"},      // 0x46
    {"curdir", "wp"},    // 0x47
    {"malloc", "l"},     // 0x48
    {"mfree", "p"},      // 0x49
    {"setblock", "pl"},  // 0x4a
    {"exec", "w?"},      // 0x4b
    {"exit2", "w"},      // 0x4c
    {"wait", NULL},      // 0x4d
    {"files", "psw"},    // 0x4e
    {"nfiles", "p"},     // 0x4f

    {"setpdb", "p"},     // 0x50
    {"getpdb", NULL},    // 0x51
    {"setenv", "sps"},   // 0x52
    {"getenv", "spp"},   // 0x53
    {"verifyg", NULL},   // 0x54
    {"common", "w?"},    // 0x55
    {"rename", "ss"},    // 0x56
    {"filedate", "wl"},  // 0x57
    {"malloc2", "wl?"},  // 0x58
    {NULL, NULL},        // 0x59
    {"maketmp", "sw"},   // 0x5a
    {"newfile", "sw"},   // 0x5b
    {"lock", "wwll"},    // 0x5c
    {NULL, NULL},        // 0x5d
    {NULL, NULL},        // 0x5e
    {"assign", "w?"},    // 0x5f

    {"malloc3", "l"},     // 0x60 (060turbo.sys)
    {"setblock2", "pl"},  // 0x61 (060turbo.sys)
    {"malloc4", "wl?"},   // 0x62 (060turbo.sys)
    {NULL, NULL},         // 0x63
    {NULL, NULL},         // 0x64
    {NULL, NULL},         // 0x65
    {NULL, NULL},         // 0x66
    {NULL, NULL},         // 0x67
    {NULL, NULL},         // 0x68
    {NULL, NULL},         // 0x69
    {NULL, NULL},         // 0x6a
    {NULL, NULL},         // 0x6b
    {NULL, NULL},         // 0x6c
    {NULL, NULL},         // 0x6d
    {NULL, NULL},         // 0x6e
    {NULL, NULL},         // 0x6f

    {NULL, NULL},           // 0x70
    {NULL, NULL},           // 0x71
    {NULL, NULL},           // 0x72
    {NULL, NULL},           // 0x73
    {NULL, NULL},           // 0x74
    {NULL, NULL},           // 0x75
    {NULL, NULL},           // 0x76
    {NULL, NULL},           // 0x77
    {NULL, NULL},           // 0x78
    {NULL, NULL},           // 0x79
    {"fflushflg", "w"},     // 0x7a
    {"ospatch", "wp"},      // 0x7b
    {"getfcb", "w"},        // 0x7c
    {"s_malloc", "wl"},     // 0x7d
    {"s_mfree", "p"},       // 0x7e
    {"s_process", "wpll"},  // 0x7f

    {"setpdb", "p"},     // 0x80
    {"getpdb", NULL},    // 0x81
    {"setenv", "sps"},   // 0x82
    {"getenv", "spp"},   // 0x83
    {"verifyg", NULL},   // 0x84
    {"common", "w?"},    // 0x85
    {"rename", "ss"},    // 0x86
    {"filedate", "wl"},  // 0x87
    {"malloc2", "wl?"},  // 0x88
    {NULL, NULL},        // 0x89
    {"maketmp", "sw"},   // 0x8a
    {"newfile", "sw"},   // 0x8b
    {"lock", "wwll"},    // 0x8c
    {NULL, NULL},        // 0x8d
    {NULL, NULL},        // 0x8e
    {"assign", "w?"},    // 0x8f

    {"malloc3", "l"},     // 0x90 (060turbo.sys)
    {"setblock2", "pl"},  // 0x91 (060turbo.sys)
    {"malloc4", "wl?"},   // 0x92 (060turbo.sys)
    {NULL, NULL},         // 0x93
    {NULL, NULL},         // 0x94
    {NULL, NULL},         // 0x95
    {NULL, NULL},         // 0x96
    {NULL, NULL},         // 0x97
    {NULL, NULL},         // 0x98
    {NULL, NULL},         // 0x99
    {NULL, NULL},         // 0x9a
    {NULL, NULL},         // 0x9b
    {NULL, NULL},         // 0x9c
    {NULL, NULL},         // 0x9d
    {NULL, NULL},         // 0x9e
    {NULL, NULL},         // 0x9f

    {NULL, NULL},           // 0xa0
    {NULL, NULL},           // 0xa1
    {NULL, NULL},           // 0xa2
    {NULL, NULL},           // 0xa3
    {NULL, NULL},           // 0xa4
    {NULL, NULL},           // 0xa5
    {NULL, NULL},           // 0xa6
    {NULL, NULL},           // 0xa7
    {NULL, NULL},           // 0xa8
    {NULL, NULL},           // 0xa9
    {"fflushflg", "w"},     // 0xaa
    {"ospatch", "wp"},      // 0xab
    {"getfcb", "w"},        // 0xac
    {"s_malloc", "wl"},     // 0xad
    {"s_mfree", "p"},       // 0xae
    {"s_process", "wpll"},  // 0xaf

    {"twon", "w?"},   // 0xb0 ((V)TwentyOne.sys)
    {"mvdir", "w?"},  // 0xb1 (dos_mvdir.r)
    {NULL, NULL},     // 0xb2
    {NULL, NULL},     // 0xb3
    {NULL, NULL},     // 0xb4
    {NULL, NULL},     // 0xb5
    {NULL, NULL},     // 0xb6
    {NULL, NULL},     // 0xb7
    {NULL, NULL},     // 0xb8
    {NULL, NULL},     // 0xb9
    {NULL, NULL},     // 0xba
    {NULL, NULL},     // 0xbb
    {NULL, NULL},     // 0xbc
    {NULL, NULL},     // 0xbd
    {NULL, NULL},     // 0xbe
    {NULL, NULL},     // 0xbf

    {NULL, NULL},  // 0xc0
    {NULL, NULL},  // 0xc1
    {NULL, NULL},  // 0xc2
    {NULL, NULL},  // 0xc3
    {NULL, NULL},  // 0xc4
    {NULL, NULL},  // 0xc5
    {NULL, NULL},  // 0xc6
    {NULL, NULL},  // 0xc7
    {NULL, NULL},  // 0xc8
    {NULL, NULL},  // 0xc9
    {NULL, NULL},  // 0xca
    {NULL, NULL},  // 0xcb
    {NULL, NULL},  // 0xcc
    {NULL, NULL},  // 0xcd
    {NULL, NULL},  // 0xce
    {NULL, NULL},  // 0xcf

    {NULL, NULL},  // 0xd0
    {NULL, NULL},  // 0xd1
    {NULL, NULL},  // 0xd2
    {NULL, NULL},  // 0xd3
    {NULL, NULL},  // 0xd4
    {NULL, NULL},  // 0xd5
    {NULL, NULL},  // 0xd6
    {NULL, NULL},  // 0xd7
    {NULL, NULL},  // 0xd8
    {NULL, NULL},  // 0xd9
    {NULL, NULL},  // 0xda
    {NULL, NULL},  // 0xdb
    {NULL, NULL},  // 0xdc
    {NULL, NULL},  // 0xdd
    {NULL, NULL},  // 0xde
    {NULL, NULL},  // 0xdf

    {NULL, NULL},  // 0xe0
    {NULL, NULL},  // 0xe1
    {NULL, NULL},  // 0xe2
    {NULL, NULL},  // 0xe3
    {NULL, NULL},  // 0xe4
    {NULL, NULL},  // 0xe5
    {NULL, NULL},  // 0xe6
    {NULL, NULL},  // 0xe7
    {NULL, NULL},  // 0xe8
    {NULL, NULL},  // 0xe9
    {NULL, NULL},  // 0xea
    {NULL, NULL},  // 0xeb
    {NULL, NULL},  // 0xec
    {NULL, NULL},  // 0xed
    {NULL, NULL},  // 0xee
    {NULL, NULL},  // 0xef

    {NULL, NULL},             // 0xf0 exitvc
    {NULL, NULL},             // 0xf1 ctrlvc
    {NULL, NULL},             // 0xf2 errjvc
    {"diskred", "pw?"},       // 0xf3
    {"diskwrt", "pw?"},       // 0xf4
    {"indosflg", NULL},       // 0xf5
    {"super_jsr", "p"},       // 0xf6
    {"bus_err", "ppw"},       // 0xf7
    {"open_pr", "swppwppl"},  // 0xf8
    {"kill_pr", NULL},        // 0xf9
    {"get_pr", "wp"},         // 0xfa
    {"suspend_pr", "w"},      // 0xfb
    {"sleep_pr", "l"},        // 0xfc
    {"send_pr", "wwwpl"},     // 0xfd
    {"time_pr", NULL},        // 0xfe
    {"change_pr", NULL},      // 0xff
};
SystemCallInfo HumanInfo = {C(HumanList), HumanList, NULL};

static unsigned int GetModeWord(const void* arg) {
  return *(const unsigned short*)arg;
}

static const SystemCall KflushList[] = {
    {"kflush", "w"},       //
    {"kflush{gp}", "w"},   //
    {NULL, NULL},          //
    {NULL, NULL},          //
    {NULL, NULL},          //
    {NULL, NULL},          //
    {"kflush{io}", "ww"},  //
    {"kflush{in}", "w"},   //
    {"kflush{gc}", "w"},   //
    {NULL, NULL},          //
    {"kflush{gs}", "wp"},
};
static SystemCallInfo KflushInfo = {C(KflushList), KflushList, GetModeWord};

static unsigned int GetFatchkMode(const void* arg) {
  // BUFFER引数の最上位バイト
  const unsigned char* buf0 = (const unsigned char*)arg + 4;

  // 最上位ビットが1ならロングワードモード
  return (*buf0 & 0x80) ? 1 : 0;
}

static const SystemCall FatchkList[] = {
    {"fatchk", "sp"},      // ワードモード
    {"fatchk{2}", "spw"},  // ロングワードモード
};
static SystemCallInfo FatchkInfo = {C(FatchkList), FatchkList, GetFatchkMode};

static const SystemCall HendspList[] = {
    {"hendsp{mo}", "wwp"},  //
    {"hendsp{mp}", "wwp"},  //
    {"hendsp{mr}", "wwp"},  //
    {"hendsp{mc}", "w"},    //
    {"hendsp{io}", "w"},    //
    {"hendsp{ip}", "wwp"},  //
    {"hendsp{ir}", "wwp"},  //
    {"hendsp{ic}", "ww"},   //
    {"hendsp{so}", "w"},    //
    {"hendsp{sp}", "wwp"},  //
    {"hendsp{sr}", "wwp"},  //
    {"hendsp{sc}", "w"},
};
static SystemCallInfo HendspInfo = {C(HendspList), HendspList, GetModeWord};

static unsigned int GetFpCallNo(const void* arg) {
  unsigned int callno = *(const unsigned int*)arg;
  return callno & 0xffff;  // ASK68Kでは上位ワードは無視される
}

static const SystemCall KnjctrlList[] = {
    {NULL, NULL},           // 0
    {"knjctrl", "ll"},      // 1
    {"knjctrl", "l"},       // 2
    {"knjctrl", "ll"},      // 3
    {"knjctrl", "l"},       // 4
    {"knjctrl", "ll"},      // 5
    {"knjctrl", "l"},       // 6
    {"knjctrl", "ll"},      // 7
    {"knjctrl", "l"},       // 8
    {"knjctrl", "ll"},      // 9
    {"knjctrl", "l"},       // 10
    {"knjctrl", "ll"},      // 11
    {"knjctrl", "l"},       // 12
    {"knjctrl", "lsl"},     // 13
    {"knjctrl", "lsl"},     // 14
    {"knjctrl", "lsl"},     // 15
    {"knjctrl", "lsl"},     // 16
    {"knjctrl", "lsl"},     // 17
    {"knjctrl", "lsl"},     // 18
    {"knjctrl", "lsll"},    // 19
    {"knjctrl", "lll"},     // 20
    {"knjctrl", "lll"},     // 21
    {"knjctrl", "lll"},     // 22
    {"knjctrl", "lll"},     // 23
    {"knjctrl", "ll"},      // 24
    {"knjctrl", "llllll"},  // 25
    {"knjctrl", "lsll"},    // 26
    {"knjctrl", "llll"},    // 27
    {"knjctrl", "l"},       // 28
    {"knjctrl", "l"},       // 29
    {"knjctrl", "llssl"},   // 30
    {"knjctrl", "llssl"},   // 31
    {"knjctrl", "lss"},     // 32
    {"knjctrl", "lll"},     // 33
    {"knjctrl", "lll"},     // 34
    {"knjctrl", "lll"},     // 35
    {"knjctrl", "lll"},     // 36
    {"knjctrl", "lll"},     // 37
    {"knjctrl", "lll"},     // 38
    {"knjctrl", "lll"},     // 39
    {"knjctrl", "lll"},     // 40
    {"knjctrl", "lll"},     // 41
    {"knjctrl", "l"},       // 42
    {NULL, NULL},           // 43
    {NULL, NULL},           // 44
    {NULL, NULL},           // 45
    {NULL, NULL},           // 46
    {NULL, NULL},           // 47
    {NULL, NULL},           // 48
    {NULL, NULL},           // 49
    {"knjctrl", "l"},       // 50
    {"knjctrl", "l"},       // 51
    {"knjctrl", "ls"},      // 52
    {"knjctrl", "l"},       // 53
    {"knjctrl", "ll"},      // 54
    {"knjctrl", "l"},       // 55
    {"knjctrl", "ll"},      // 56
    {NULL, NULL},           // 57
    {NULL, NULL},           // 58
    {NULL, NULL},           // 59
    {"knjctrl", "lp"},      // 60
    {"knjctrl", "l"},       // 61
    {"knjctrl", "lpsl"},    // 62
    {"knjctrl", "lpll"},    // 63
};
static SystemCallInfo KnjctrlInfo = {C(KnjctrlList), KnjctrlList, GetFpCallNo};

static const SystemCall ConctrlList[] = {
    {"conctrl{putc}", "ww"},     //
    {"conctrl{print}", "ws"},    //
    {"conctrl{color}", "ww"},    //
    {"conctrl{locate}", "www"},  //
    {"conctrl{down_s}", "w"},    //
    {"conctrl{up_s}", "w"},      //
    {"conctrl{up}", "ww"},       //
    {"conctrl{down}", "ww"},     //
    {"conctrl{right}", "ww"},    //
    {"conctrl{left}", "ww"},     //
    {"conctrl{cls}", "ww"},      //
    {"conctrl{era}", "ww"},      //
    {"conctrl{ins}", "ww"},      //
    {"conctrl{del}", "ww"},      //
    {"conctrl{fnkmod}", "ww"},   //
    {"conctrl{window}", "www"},  //
    {"conctrl{width}", "ww"},    //
    {"conctrl{curon}", "w"},     //
    {"conctrl{curoff}", "w"},
};
static SystemCallInfo ConctrlInfo = {C(ConctrlList), ConctrlList, GetModeWord};

static const SystemCall KeyctrlList[] = {
    {"keyctrl{keyinp}", "w"},   //
    {"keyctrl{keysns}", "w"},   //
    {"keyctrl{sftsns}", "w"},   //
    {"keyctrl{keybit}", "ww"},  //
    {"keyctrl{insmod}", "ww"},
};
static SystemCallInfo KeyctrlInfo = {C(KeyctrlList), KeyctrlList, GetModeWord};

static const SystemCall IoctrlList[] = {
    {"ioctrl{gt}", "ww"},
    {"ioctrl{st}", "www"},
    {"ioctrl{rh}", "wwll"},
    {"ioctrl{wh}", "wwll"},
    {"ioctrl{rd}", "wwll"},
    {"ioctrl{wd}", "wwll"},
    {"ioctrl{is}", "ww"},
    {"ioctrl{os}", "ww"},
    {NULL, NULL},
    {"ioctrl{dvgt}", "ww"},
    {"ioctrl{fdgt}", "ww"},
    {"ioctrl{rtset}", "www"},
    {"ioctrl{fdctl}", "wwwl"},
    {"ioctrl{dvctl}", "wwwl"},
};
static SystemCallInfo IoctrlInfo = {C(IoctrlList), IoctrlList, GetModeWord};

static unsigned int GetExecMode(const void* arg) {
  unsigned short md = *(const unsigned short*)arg;
  return md & 0x00ff;  // 上位バイトはモジュール番号
}

static const SystemCall ExecList[] = {
    {"exec{loadexec}", "wssp"},  //
    {"exec{load}", "wssp"},      //
    {"exec{pathchk}", "wspp"},   //
    {"exec{loadonly}", "wspp"},  //
    {"exec{execonly}", "wp"},    //
    {"exec{bindno}", "wss"},
};
static SystemCallInfo ExecInfo = {C(ExecList), ExecList, GetExecMode};

static const SystemCall CommonList[] = {
    {"common{ck}", "ws"},      //
    {"common{rd}", "wslll"},   //
    {"common{wt}", "wslll"},   //
    {"common{lk}", "wslll"},   //
    {"common{fre}", "wslll"},  //
    {"common{del}", "ws"},
};
static SystemCallInfo CommonInfo = {C(CommonList), CommonList, GetModeWord};

static unsigned int GetMalloc2Mode(const void* arg) {
  // MD引数の上位バイト
  const unsigned char* md0 = (const unsigned char*)arg;

  // 最上位ビットが1ならプロセス管理ポインタ指定モード
  return (*md0 & 0x80) ? 1 : 0;
}

static const SystemCall Malloc2List[] = {
    {"malloc2", "wl"},      // 通常モード
    {"malloc2{2}", "wlp"},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo Malloc2Info = {  //
    C(Malloc2List), Malloc2List, GetMalloc2Mode};

static const SystemCall Malloc4List[] = {
    {"malloc4", "wl"},      // 通常モード
    {"malloc4{2}", "wlp"},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo Malloc4Info = {  //
    C(Malloc4List), Malloc4List, GetMalloc2Mode};

static const SystemCall AssignList[] = {
    {"assign{getassign}", "wsp"},
    {"assign{makeassign}", "wssw"},
    {NULL, NULL},
    {NULL, NULL},
    {"assign{rassign}", "ws"},
};
static SystemCallInfo AssignInfo = {C(AssignList), AssignList, GetModeWord};

static const SystemCall TwonList[] = {
    {"twon{getid}", "w"},     //
    {"twon{getver}", "w"},    //
    {"twon{getadr}", "w"},    //
    {"twon{getopt}", "w"},    //
    {"twon{setopt}", "wl"},   //
    {"twon{getsysr}", "wp"},  //
    {"twon{setsysr}", "ws"},  //
    {"twon{u2s}", "ww"},
};
static SystemCallInfo TwonInfo = {C(TwonList), TwonList, GetModeWord};

static const SystemCall MvdirList[] = {
    {"mvdir{getid}", "w"},
    {"mvdir{getver}", "w"},
    {"mvdir{unhook}", "w"},
    {"mvdir{move}", "wss"},
};
static SystemCallInfo MvdirInfo = {C(MvdirList), MvdirList, GetModeWord};

static unsigned int GetDiskRWMode(const void* arg) {
  // BUFFER引数の上位バイト
  const unsigned char* buf0 = (const unsigned char*)arg;

  // 最上位ビットが1ならロングワードモード
  return (*buf0 & 0x80) ? 1 : 0;
}

static const SystemCall DiskredList[] = {
    {"diskred", "pwww"},     // ワードモード
    {"diskred{2}", "pwll"},  // ロングワードモード
};
static SystemCallInfo DiskredInfo = {  //
    C(DiskredList), DiskredList, GetDiskRWMode};

static const SystemCall DiskwrtList[] = {
    {"diskwrt", "pwww"},     // ワードモード
    {"diskwrt{2}", "pwll"},  // ロングワードモード
};
static SystemCallInfo DiskwrtInfo = {  //
    C(DiskwrtList), DiskwrtList, GetDiskRWMode};

const SystemCallInfo* GetSubCallInfo(unsigned char callno) {
  switch (callno) {
    default:
      break;
    case 0x0c:
      return &KflushInfo;
    case 0x17:
      return &FatchkInfo;
    case 0x18:
      return &HendspInfo;
    case 0x22:
      return &KnjctrlInfo;
    case 0x23:
      return &ConctrlInfo;
    case 0x24:
      return &KeyctrlInfo;
    case 0x44:
      return &IoctrlInfo;
    case 0x4b:
      return &ExecInfo;
    case 0x55:
    case 0x85:
      return &CommonInfo;
    case 0x58:
    case 0x88:
      return &Malloc2Info;
    case 0x5f:
    case 0x8f:
      return &AssignInfo;
    case 0x62:
    case 0x92:
      return &Malloc4Info;
    case 0xb0:
      return &TwonInfo;
    case 0xb1:
      return &MvdirInfo;
    case 0xf3:
      return &DiskredInfo;
    case 0xf4:
      return &DiskwrtInfo;
  }
  return NULL;
}
