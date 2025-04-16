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
    {"exit", NULL, RET_VOID},    // 0x00
    {"getchar", NULL, RET_INT},  // 0x01
    {"putchar", "w", RET_INT},   // 0x02
    {"cominp", NULL, RET_INT},   // 0x03
    {"comout", "w", RET_INT},    // 0x04
    {"prnout", "w", RET_INT},    // 0x05
    {"inpout", "w", RET_INT},    // 0x06
    {"inkey", NULL, RET_INT},    // 0x07
    {"getc", NULL, RET_INT},     // 0x08
    {"print", "s", RET_INT},     // 0x09
    {"gets", "p", RET_INT},      // 0x0a
    {"keysns", NULL, RET_INT},   // 0x0b
    {"kflush", "w?", RET_INT},   // 0x0c
    {"fflush", NULL, RET_INT},   // 0x0d
    {"chgdrv", "w", RET_INT},    // 0x0e
    {"drvctrl", "w", RET_INT},   // 0x0f

    {"consns", NULL, RET_INT},    // 0x10
    {"prnsns", NULL, RET_INT},    // 0x11
    {"cinsns", NULL, RET_INT},    // 0x12
    {"coutsns", NULL, RET_INT},   // 0x13
    {NULL, NULL, RET_INT},        // 0x14
    {NULL, NULL, RET_INT},        // 0x15
    {NULL, NULL, RET_INT},        // 0x16
    {"fatchk", "sp?", RET_INT},   // 0x17
    {"hendsp", "w?", RET_INT},    // 0x18
    {"curdrv", NULL, RET_INT},    // 0x19
    {"getss", "p", RET_INT},      // 0x1a
    {"fgetc", "w", RET_INT},      // 0x1b
    {"fgets", "pw", RET_INT},     // 0x1c
    {"fputc", "ww", RET_INT},     // 0x1d
    {"fputs", "sw", RET_INT},     // 0x1e
    {"allclose", NULL, RET_INT},  // 0x1f

    {"super", "p", RET_PTR},     // 0x20
    {"fnckey", "wp", RET_INT},   // 0x21
    {"knjctrl", "l?", RET_INT},  // 0x22
    {"conctrl", "w?", RET_INT},  // 0x23
    {"keyctrl", "w?", RET_INT},  // 0x24
    {"intvcs", "wp", RET_PTR},   // 0x25
    {"pspset", "p", RET_INT},    // 0x26
    {"gettim2", NULL, RET_HEX},  // 0x27
    {"settim2", "l", RET_INT},   // 0x28
    {"namests", "sp", RET_INT},  // 0x29
    {"getdate", NULL, RET_HEX},  // 0x2a
    {"setdate", "w", RET_INT},   // 0x2b
    {"gettime", NULL, RET_HEX},  // 0x2c
    {"settime", "w", RET_INT},   // 0x2d
    {"verify", "w", RET_INT},    // 0x2e
    {"dup0", "ww", RET_INT},     // 0x2f

    {"vernum", NULL, RET_HEX},   // 0x30
    {"keeppr", "lw", RET_VOID},  // 0x31
    {"getdpb", "wp", RET_INT},   // 0x32
    {"breakck", "w", RET_INT},   // 0x33
    {"drvxchg", "ww", RET_INT},  // 0x34
    {"intvcg", "w", RET_PTR},    // 0x35
    {"dskfre", "wp", RET_INT},   // 0x36
    {"nameck", "sp", RET_INT},   // 0x37
    {NULL, NULL, RET_INT},       // 0x38
    {"mkdir", "s", RET_INT},     // 0x39
    {"rmdir", "s", RET_INT},     // 0x3a
    {"chdir", "s", RET_INT},     // 0x3b
    {"create", "sw", RET_INT},   // 0x3c
    {"open", "sw", RET_INT},     // 0x3d
    {"close", "w", RET_INT},     // 0x3e
    {"read", "wpl", RET_INT},    // 0x3f

    {"write", "wpl", RET_INT},    // 0x40
    {"delete", "s", RET_INT},     // 0x41
    {"seek", "wlw", RET_INT},     // 0x42
    {"chmod", "sw", RET_INT},     // 0x43
    {"ioctrl", "w?", RET_INT},    // 0x44
    {"dup", "w", RET_INT},        // 0x45
    {"dup2", "ww", RET_INT},      // 0x46
    {"curdir", "wp", RET_INT},    // 0x47
    {"malloc", "l", RET_PTR},     // 0x48
    {"mfree", "p", RET_INT},      // 0x49
    {"setblock", "pl", RET_HEX},  // 0x4a
    {"exec", "w?", RET_INT},      // 0x4b
    {"exit2", "w", RET_VOID},     // 0x4c
    {"wait", NULL, RET_INT},      // 0x4d
    {"files", "psw", RET_INT},    // 0x4e
    {"nfiles", "p", RET_INT},     // 0x4f

    {"setpdb", "p", RET_PTR},     // 0x50
    {"getpdb", NULL, RET_PTR},    // 0x51
    {"setenv", "sps", RET_INT},   // 0x52
    {"getenv", "spp", RET_INT},   // 0x53
    {"verifyg", NULL, RET_INT},   // 0x54
    {"common", "w?", RET_INT},    // 0x55
    {"rename", "ss", RET_INT},    // 0x56
    {"filedate", "wl", RET_HEX},  // 0x57
    {"malloc2", "wl?", RET_PTR},  // 0x58
    {NULL, NULL, RET_INT},        // 0x59
    {"maketmp", "sw", RET_INT},   // 0x5a
    {"newfile", "sw", RET_INT},   // 0x5b
    {"lock", "wwll", RET_INT},    // 0x5c
    {NULL, NULL, RET_INT},        // 0x5d
    {NULL, NULL, RET_INT},        // 0x5e
    {"assign", "w?", RET_INT},    // 0x5f

    {"malloc3", "l", RET_PTR},      // 0x60 (060turbo.sys)
    {"setblock2", "pl", RET_HEX},   // 0x61 (060turbo.sys)
    {"malloc4", "wl?", RET_PTR},    // 0x62 (060turbo.sys)
    {"s_malloc2", "wl?", RET_PTR},  // 0x63 (060turbo.sys)
    {NULL, NULL, RET_INT},          // 0x64
    {NULL, NULL, RET_INT},          // 0x65
    {NULL, NULL, RET_INT},          // 0x66
    {NULL, NULL, RET_INT},          // 0x67
    {NULL, NULL, RET_INT},          // 0x68
    {NULL, NULL, RET_INT},          // 0x69
    {NULL, NULL, RET_INT},          // 0x6a
    {NULL, NULL, RET_INT},          // 0x6b
    {NULL, NULL, RET_INT},          // 0x6c
    {NULL, NULL, RET_INT},          // 0x6d
    {NULL, NULL, RET_INT},          // 0x6e
    {NULL, NULL, RET_INT},          // 0x6f

    {NULL, NULL, RET_INT},           // 0x70
    {NULL, NULL, RET_INT},           // 0x71
    {NULL, NULL, RET_INT},           // 0x72
    {NULL, NULL, RET_INT},           // 0x73
    {NULL, NULL, RET_INT},           // 0x74
    {NULL, NULL, RET_INT},           // 0x75
    {NULL, NULL, RET_INT},           // 0x76
    {NULL, NULL, RET_INT},           // 0x77
    {NULL, NULL, RET_INT},           // 0x78
    {NULL, NULL, RET_INT},           // 0x79
    {"fflushflg", "w", RET_INT},     // 0x7a
    {"ospatch", "wp", RET_PTR},      // 0x7b
    {"getfcb", "w", RET_PTR},        // 0x7c
    {"s_malloc", "wl?", RET_PTR},    // 0x7d
    {"s_mfree", "p", RET_INT},       // 0x7e
    {"s_process", "wpll", RET_PTR},  // 0x7f

    {"setpdb", "p", RET_PTR},     // 0x80
    {"getpdb", NULL, RET_PTR},    // 0x81
    {"setenv", "sps", RET_INT},   // 0x82
    {"getenv", "spp", RET_INT},   // 0x83
    {"verifyg", NULL, RET_INT},   // 0x84
    {"common", "w?", RET_INT},    // 0x85
    {"rename", "ss", RET_INT},    // 0x86
    {"filedate", "wl", RET_HEX},  // 0x87
    {"malloc2", "wl?", RET_PTR},  // 0x88
    {NULL, NULL, RET_INT},        // 0x89
    {"maketmp", "sw", RET_INT},   // 0x8a
    {"newfile", "sw", RET_INT},   // 0x8b
    {"lock", "wwll", RET_INT},    // 0x8c
    {NULL, NULL, RET_INT},        // 0x8d
    {NULL, NULL, RET_INT},        // 0x8e
    {"assign", "w?", RET_INT},    // 0x8f

    {"malloc3", "l", RET_PTR},      // 0x90 (060turbo.sys)
    {"setblock2", "pl", RET_HEX},   // 0x91 (060turbo.sys)
    {"malloc4", "wl?", RET_PTR},    // 0x92 (060turbo.sys)
    {"s_malloc2", "wl?", RET_PTR},  // 0x93 (060turbo.sys)
    {NULL, NULL, RET_INT},          // 0x94
    {NULL, NULL, RET_INT},          // 0x95
    {NULL, NULL, RET_INT},          // 0x96
    {NULL, NULL, RET_INT},          // 0x97
    {NULL, NULL, RET_INT},          // 0x98
    {NULL, NULL, RET_INT},          // 0x99
    {NULL, NULL, RET_INT},          // 0x9a
    {NULL, NULL, RET_INT},          // 0x9b
    {NULL, NULL, RET_INT},          // 0x9c
    {NULL, NULL, RET_INT},          // 0x9d
    {NULL, NULL, RET_INT},          // 0x9e
    {NULL, NULL, RET_INT},          // 0x9f

    {NULL, NULL, RET_INT},           // 0xa0
    {NULL, NULL, RET_INT},           // 0xa1
    {NULL, NULL, RET_INT},           // 0xa2
    {NULL, NULL, RET_INT},           // 0xa3
    {NULL, NULL, RET_INT},           // 0xa4
    {NULL, NULL, RET_INT},           // 0xa5
    {NULL, NULL, RET_INT},           // 0xa6
    {NULL, NULL, RET_INT},           // 0xa7
    {NULL, NULL, RET_INT},           // 0xa8
    {NULL, NULL, RET_INT},           // 0xa9
    {"fflushflg", "w", RET_INT},     // 0xaa
    {"ospatch", "wp", RET_PTR},      // 0xab
    {"getfcb", "w", RET_PTR},        // 0xac
    {"s_malloc", "wl?", RET_PTR},    // 0xad
    {"s_mfree", "p", RET_INT},       // 0xae
    {"s_process", "wpll", RET_PTR},  // 0xaf

    {"twon", "w?", RET_INT},   // 0xb0 ((V)TwentyOne.sys)
    {"mvdir", "w?", RET_INT},  // 0xb1 (dos_mvdir.r)
    {NULL, NULL, RET_INT},     // 0xb2
    {NULL, NULL, RET_INT},     // 0xb3
    {NULL, NULL, RET_INT},     // 0xb4
    {NULL, NULL, RET_INT},     // 0xb5
    {NULL, NULL, RET_INT},     // 0xb6
    {NULL, NULL, RET_INT},     // 0xb7
    {NULL, NULL, RET_INT},     // 0xb8
    {NULL, NULL, RET_INT},     // 0xb9
    {NULL, NULL, RET_INT},     // 0xba
    {NULL, NULL, RET_INT},     // 0xbb
    {NULL, NULL, RET_INT},     // 0xbc
    {NULL, NULL, RET_INT},     // 0xbd
    {NULL, NULL, RET_INT},     // 0xbe
    {NULL, NULL, RET_INT},     // 0xbf

    {NULL, NULL, RET_INT},  // 0xc0
    {NULL, NULL, RET_INT},  // 0xc1
    {NULL, NULL, RET_INT},  // 0xc2
    {NULL, NULL, RET_INT},  // 0xc3
    {NULL, NULL, RET_INT},  // 0xc4
    {NULL, NULL, RET_INT},  // 0xc5
    {NULL, NULL, RET_INT},  // 0xc6
    {NULL, NULL, RET_INT},  // 0xc7
    {NULL, NULL, RET_INT},  // 0xc8
    {NULL, NULL, RET_INT},  // 0xc9
    {NULL, NULL, RET_INT},  // 0xca
    {NULL, NULL, RET_INT},  // 0xcb
    {NULL, NULL, RET_INT},  // 0xcc
    {NULL, NULL, RET_INT},  // 0xcd
    {NULL, NULL, RET_INT},  // 0xce
    {NULL, NULL, RET_INT},  // 0xcf

    {NULL, NULL, RET_INT},  // 0xd0
    {NULL, NULL, RET_INT},  // 0xd1
    {NULL, NULL, RET_INT},  // 0xd2
    {NULL, NULL, RET_INT},  // 0xd3
    {NULL, NULL, RET_INT},  // 0xd4
    {NULL, NULL, RET_INT},  // 0xd5
    {NULL, NULL, RET_INT},  // 0xd6
    {NULL, NULL, RET_INT},  // 0xd7
    {NULL, NULL, RET_INT},  // 0xd8
    {NULL, NULL, RET_INT},  // 0xd9
    {NULL, NULL, RET_INT},  // 0xda
    {NULL, NULL, RET_INT},  // 0xdb
    {NULL, NULL, RET_INT},  // 0xdc
    {NULL, NULL, RET_INT},  // 0xdd
    {NULL, NULL, RET_INT},  // 0xde
    {NULL, NULL, RET_INT},  // 0xdf

    {NULL, NULL, RET_INT},  // 0xe0
    {NULL, NULL, RET_INT},  // 0xe1
    {NULL, NULL, RET_INT},  // 0xe2
    {NULL, NULL, RET_INT},  // 0xe3
    {NULL, NULL, RET_INT},  // 0xe4
    {NULL, NULL, RET_INT},  // 0xe5
    {NULL, NULL, RET_INT},  // 0xe6
    {NULL, NULL, RET_INT},  // 0xe7
    {NULL, NULL, RET_INT},  // 0xe8
    {NULL, NULL, RET_INT},  // 0xe9
    {NULL, NULL, RET_INT},  // 0xea
    {NULL, NULL, RET_INT},  // 0xeb
    {NULL, NULL, RET_INT},  // 0xec
    {NULL, NULL, RET_INT},  // 0xed
    {NULL, NULL, RET_INT},  // 0xee
    {NULL, NULL, RET_INT},  // 0xef

    {NULL, NULL, RET_INT},             // 0xf0 exitvc
    {NULL, NULL, RET_INT},             // 0xf1 ctrlvc
    {NULL, NULL, RET_INT},             // 0xf2 errjvc
    {"diskred", "pw?", RET_INT},       // 0xf3
    {"diskwrt", "pw?", RET_INT},       // 0xf4
    {"indosflg", NULL, RET_PTR},       // 0xf5
    {"super_jsr", "p", RET_INT},       // 0xf6
    {"bus_err", "ppw", RET_INT},       // 0xf7
    {"open_pr", "swppwppl", RET_INT},  // 0xf8
    {"kill_pr", NULL, RET_VOID},       // 0xf9
    {"get_pr", "wp", RET_INT},         // 0xfa
    {"suspend_pr", "w", RET_INT},      // 0xfb
    {"sleep_pr", "l", RET_INT},        // 0xfc
    {"send_pr", "wwwpl", RET_INT},     // 0xfd
    {"time_pr", NULL, RET_INT},        // 0xfe
    {"change_pr", NULL, RET_INT},      // 0xff
};
SystemCallInfo HumanInfo = {C(HumanList), HumanList, NULL};

static unsigned int GetModeWord(const void* arg) {
  return *(const unsigned short*)arg;
}

static const SystemCall KflushList[] = {
    {"kflush", "w", RET_INT},       //
    {"kflush{gp}", "w", RET_INT},   //
    {NULL, NULL, RET_INT},          //
    {NULL, NULL, RET_INT},          //
    {NULL, NULL, RET_INT},          //
    {NULL, NULL, RET_INT},          //
    {"kflush{io}", "ww", RET_INT},  //
    {"kflush{in}", "w", RET_INT},   //
    {"kflush{gc}", "w", RET_INT},   //
    {NULL, NULL, RET_INT},          //
    {"kflush{gs}", "wp", RET_INT},
};
static SystemCallInfo KflushInfo = {C(KflushList), KflushList, GetModeWord};

static unsigned int GetFatchkMode(const void* arg) {
  // BUFFER引数の最上位バイト
  const unsigned char* buf0 = (const unsigned char*)arg + 4;

  // 最上位ビットが1ならロングワードモード
  return (*buf0 & 0x80) ? 1 : 0;
}

static const SystemCall FatchkList[] = {
    {"fatchk", "sp", RET_INT},      // ワードモード
    {"fatchk{2}", "spw", RET_INT},  // ロングワードモード
};
static SystemCallInfo FatchkInfo = {C(FatchkList), FatchkList, GetFatchkMode};

static const SystemCall HendspList[] = {
    {"hendsp{mo}", "wwp", RET_INT},  //
    {"hendsp{mp}", "wwp", RET_INT},  //
    {"hendsp{mr}", "wwp", RET_INT},  //
    {"hendsp{mc}", "w", RET_INT},    //
    {"hendsp{io}", "w", RET_INT},    //
    {"hendsp{ip}", "wwp", RET_INT},  //
    {"hendsp{ir}", "wwp", RET_INT},  //
    {"hendsp{ic}", "ww", RET_INT},   //
    {"hendsp{so}", "w", RET_INT},    //
    {"hendsp{sp}", "wwp", RET_INT},  //
    {"hendsp{sr}", "wwp", RET_INT},  //
    {"hendsp{sc}", "w", RET_INT},
};
static SystemCallInfo HendspInfo = {C(HendspList), HendspList, GetModeWord};

static unsigned int GetFpCallNo(const void* arg) {
  unsigned int callno = *(const unsigned int*)arg;
  return callno & 0xffff;  // ASK68Kでは上位ワードは無視される
}

static const SystemCall KnjctrlList[] = {
    {NULL, NULL, RET_INT},           // 0
    {"knjctrl", "ll", RET_INT},      // 1
    {"knjctrl", "l", RET_INT},       // 2
    {"knjctrl", "ll", RET_INT},      // 3
    {"knjctrl", "l", RET_INT},       // 4
    {"knjctrl", "ll", RET_INT},      // 5
    {"knjctrl", "l", RET_INT},       // 6
    {"knjctrl", "ll", RET_INT},      // 7
    {"knjctrl", "l", RET_INT},       // 8
    {"knjctrl", "ll", RET_INT},      // 9
    {"knjctrl", "l", RET_INT},       // 10
    {"knjctrl", "ll", RET_INT},      // 11
    {"knjctrl", "l", RET_INT},       // 12
    {"knjctrl", "lsl", RET_INT},     // 13
    {"knjctrl", "lsl", RET_INT},     // 14
    {"knjctrl", "lsl", RET_INT},     // 15
    {"knjctrl", "lsl", RET_INT},     // 16
    {"knjctrl", "lsl", RET_INT},     // 17
    {"knjctrl", "lsl", RET_INT},     // 18
    {"knjctrl", "lsll", RET_INT},    // 19
    {"knjctrl", "lll", RET_INT},     // 20
    {"knjctrl", "lll", RET_INT},     // 21
    {"knjctrl", "lll", RET_INT},     // 22
    {"knjctrl", "lll", RET_INT},     // 23
    {"knjctrl", "ll", RET_INT},      // 24
    {"knjctrl", "llllll", RET_INT},  // 25
    {"knjctrl", "lsll", RET_INT},    // 26
    {"knjctrl", "llll", RET_INT},    // 27
    {"knjctrl", "l", RET_INT},       // 28
    {"knjctrl", "l", RET_INT},       // 29
    {"knjctrl", "llssl", RET_INT},   // 30
    {"knjctrl", "llssl", RET_INT},   // 31
    {"knjctrl", "lss", RET_INT},     // 32
    {"knjctrl", "lll", RET_INT},     // 33
    {"knjctrl", "lll", RET_INT},     // 34
    {"knjctrl", "lll", RET_INT},     // 35
    {"knjctrl", "lll", RET_INT},     // 36
    {"knjctrl", "lll", RET_INT},     // 37
    {"knjctrl", "lll", RET_INT},     // 38
    {"knjctrl", "lll", RET_INT},     // 39
    {"knjctrl", "lll", RET_INT},     // 40
    {"knjctrl", "lll", RET_INT},     // 41
    {"knjctrl", "l", RET_INT},       // 42
    {NULL, NULL, RET_INT},           // 43
    {NULL, NULL, RET_INT},           // 44
    {NULL, NULL, RET_INT},           // 45
    {NULL, NULL, RET_INT},           // 46
    {NULL, NULL, RET_INT},           // 47
    {NULL, NULL, RET_INT},           // 48
    {NULL, NULL, RET_INT},           // 49
    {"knjctrl", "l", RET_INT},       // 50
    {"knjctrl", "l", RET_INT},       // 51
    {"knjctrl", "ls", RET_INT},      // 52
    {"knjctrl", "l", RET_INT},       // 53
    {"knjctrl", "ll", RET_INT},      // 54
    {"knjctrl", "l", RET_INT},       // 55
    {"knjctrl", "ll", RET_INT},      // 56
    {NULL, NULL, RET_INT},           // 57
    {NULL, NULL, RET_INT},           // 58
    {NULL, NULL, RET_INT},           // 59
    {"knjctrl", "lp", RET_INT},      // 60
    {"knjctrl", "l", RET_INT},       // 61
    {"knjctrl", "lpsl", RET_INT},    // 62
    {"knjctrl", "lpll", RET_INT},    // 63
};
static SystemCallInfo KnjctrlInfo = {C(KnjctrlList), KnjctrlList, GetFpCallNo};

static const SystemCall ConctrlList[] = {
    {"conctrl{putc}", "ww", RET_HEX},     //
    {"conctrl{print}", "ws", RET_HEX},    //
    {"conctrl{color}", "ww", RET_HEX},    //
    {"conctrl{locate}", "www", RET_HEX},  //
    {"conctrl{down_s}", "w", RET_INT},    //
    {"conctrl{up_s}", "w", RET_INT},      //
    {"conctrl{up}", "ww", RET_INT},       //
    {"conctrl{down}", "ww", RET_INT},     //
    {"conctrl{right}", "ww", RET_INT},    //
    {"conctrl{left}", "ww", RET_INT},     //
    {"conctrl{cls}", "ww", RET_INT},      //
    {"conctrl{era}", "ww", RET_INT},      //
    {"conctrl{ins}", "ww", RET_INT},      //
    {"conctrl{del}", "ww", RET_INT},      //
    {"conctrl{fnkmod}", "ww", RET_INT},   //
    {"conctrl{window}", "www", RET_INT},  //
    {"conctrl{width}", "ww", RET_INT},    //
    {"conctrl{curon}", "w", RET_INT},     //
    {"conctrl{curoff}", "w", RET_INT},
};
static SystemCallInfo ConctrlInfo = {C(ConctrlList), ConctrlList, GetModeWord};

static const SystemCall KeyctrlList[] = {
    {"keyctrl{keyinp}", "w", RET_INT},   //
    {"keyctrl{keysns}", "w", RET_INT},   //
    {"keyctrl{sftsns}", "w", RET_HEX},   //
    {"keyctrl{keybit}", "ww", RET_HEX},  //
    {"keyctrl{insmod}", "ww", RET_INT},
};
static SystemCallInfo KeyctrlInfo = {C(KeyctrlList), KeyctrlList, GetModeWord};

static const SystemCall IoctrlList[] = {
    {"ioctrl{gt}", "ww", RET_HEX},
    {"ioctrl{st}", "www", RET_HEX},
    {"ioctrl{rh}", "wwll", RET_INT},
    {"ioctrl{wh}", "wwll", RET_INT},
    {"ioctrl{rd}", "wwll", RET_INT},
    {"ioctrl{wd}", "wwll", RET_INT},
    {"ioctrl{is}", "ww", RET_INT},
    {"ioctrl{os}", "ww", RET_INT},
    {NULL, NULL, RET_INT},
    {"ioctrl{dvgt}", "ww", RET_HEX},
    {"ioctrl{fdgt}", "ww", RET_HEX},
    {"ioctrl{rtset}", "www", RET_INT},
    {"ioctrl{fdctl}", "wwwl", RET_INT},
    {"ioctrl{dvctl}", "wwwl", RET_INT},
};
static SystemCallInfo IoctrlInfo = {C(IoctrlList), IoctrlList, GetModeWord};

static unsigned int GetExecMode(const void* arg) {
  unsigned short md = *(const unsigned short*)arg;
  return md & 0x00ff;  // 上位バイトはモジュール番号
}

static const SystemCall ExecList[] = {
    {"exec{loadexec}", "wssp", RET_INT},  //
    {"exec{load}", "wssp", RET_PTR},      //
    {"exec{pathchk}", "wspp", RET_INT},   //
    {"exec{loadonly}", "wspp", RET_INT},  //
    {"exec{execonly}", "wp", RET_INT},    //
    {"exec{bindno}", "wss", RET_HEX},
};
static SystemCallInfo ExecInfo = {C(ExecList), ExecList, GetExecMode};

static const SystemCall CommonList[] = {
    {"common{ck}", "ws", RET_INT},      //
    {"common{rd}", "wslll", RET_INT},   //
    {"common{wt}", "wslll", RET_INT},   //
    {"common{lk}", "wslll", RET_INT},   //
    {"common{fre}", "wslll", RET_INT},  //
    {"common{del}", "ws", RET_INT},
};
static SystemCallInfo CommonInfo = {C(CommonList), CommonList, GetModeWord};

static unsigned int GetMalloc2Mode(const void* arg) {
  // MD引数の上位バイト
  const unsigned char* md0 = (const unsigned char*)arg;

  // 最上位ビットが1ならプロセス管理ポインタ指定モード
  return (*md0 & 0x80) ? 1 : 0;
}

static const SystemCall Malloc2List[] = {
    {"malloc2", "wl", RET_PTR},      // 通常モード
    {"malloc2{2}", "wlp", RET_PTR},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo Malloc2Info = {  //
    C(Malloc2List), Malloc2List, GetMalloc2Mode};

static const SystemCall Malloc4List[] = {
    {"malloc4", "wl", RET_PTR},      // 通常モード
    {"malloc4{2}", "wlp", RET_PTR},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo Malloc4Info = {  //
    C(Malloc4List), Malloc4List, GetMalloc2Mode};

static const SystemCall SMalloc2List[] = {
    {"s_malloc2", "wl", RET_PTR},      // 通常モード
    {"s_malloc2{2}", "wlp", RET_PTR},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo SMalloc2Info = {  //
    C(SMalloc2List), SMalloc2List, GetMalloc2Mode};

static const SystemCall SMallocList[] = {
    {"s_malloc", "wl", RET_PTR},      // 通常モード
    {"s_malloc{2}", "wlp", RET_PTR},  // プロセス管理ポインタ指定モード
};
static SystemCallInfo SMallocInfo = {  //
    C(SMallocList), SMallocList, GetMalloc2Mode};

static const SystemCall AssignList[] = {
    {"assign{getassign}", "wsp", RET_HEX},
    {"assign{makeassign}", "wssw", RET_INT},
    {NULL, NULL, RET_INT},
    {NULL, NULL, RET_INT},
    {"assign{rassign}", "ws", RET_INT},
};
static SystemCallInfo AssignInfo = {C(AssignList), AssignList, GetModeWord};

static const SystemCall TwonList[] = {
    {"twon{getid}", "w", RET_HEX},     //
    {"twon{getver}", "w", RET_INT},    //
    {"twon{getadr}", "w", RET_PTR},    //
    {"twon{getopt}", "w", RET_HEX},    //
    {"twon{setopt}", "wl", RET_HEX},   //
    {"twon{getsysr}", "wp", RET_INT},  //
    {"twon{setsysr}", "ws", RET_INT},  //
    {"twon{u2s}", "ww", RET_HEX},
};
static SystemCallInfo TwonInfo = {C(TwonList), TwonList, GetModeWord};

static const SystemCall MvdirList[] = {
    {"mvdir{getid}", "w", RET_HEX},
    {"mvdir{getver}", "w", RET_HEX},
    {"mvdir{unhook}", "w", RET_PTR},
    {"mvdir{move}", "wss", RET_INT},
};
static SystemCallInfo MvdirInfo = {C(MvdirList), MvdirList, GetModeWord};

static unsigned int GetDiskRWMode(const void* arg) {
  // BUFFER引数の上位バイト
  const unsigned char* buf0 = (const unsigned char*)arg;

  // 最上位ビットが1ならロングワードモード
  return (*buf0 & 0x80) ? 1 : 0;
}

static const SystemCall DiskredList[] = {
    {"diskred", "pwww", RET_INT},     // ワードモード
    {"diskred{2}", "pwll", RET_INT},  // ロングワードモード
};
static SystemCallInfo DiskredInfo = {  //
    C(DiskredList), DiskredList, GetDiskRWMode};

static const SystemCall DiskwrtList[] = {
    {"diskwrt", "pwww", RET_INT},     // ワードモード
    {"diskwrt{2}", "pwll", RET_INT},  // ロングワードモード
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
    case 0x63:
    case 0x93:
      return &SMalloc2Info;
    case 0x7d:
    case 0xad:
      return &SMallocInfo;
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
