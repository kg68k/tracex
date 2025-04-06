// define system call (included from argument.c)
//
// Copyright (C) 1991 K.Abe

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

typedef struct {
  char* name;
  char* argletter;
} System_call;

typedef struct {
  int number;
  char* name;
  char* argletter;
} raw_system_call;

#define SYSTEM_CALL_TABLE_MAX \
  ((int)(sizeof(Human_system_call) / sizeof(Human_system_call[0])))

static raw_system_call Human_system_call[] = {
    {0x00, "exit", ""},
    {0x01, "getchar", ""},
    {0x02, "putchar", "w"},
    {0x03, "cominp", ""},
    {0x04, "comout", "w"},
    {0x05, "prnout", "w"},
    {0x06, "inpout", "w"},
    {0x07, "inkey", ""},
    {0x08, "getc", ""},
    {0x09, "print", "s"},
    {0x0a, "gets", "p"},
    {0x0b, "keysns", ""},
    {0x0c, "kflush", "w?"},
    {0x0d, "fflush", ""},
    {0x0e, "chgdrv", "w"},
    {0x0f, "drvctrl", "w"},
    {0x10, "consns", ""},
    {0x11, "prnsns", ""},
    {0x12, "cinsns", ""},
    {0x13, "coutsns", ""},
    /* { 0x14, "", "" }, */
    /* { 0x15, "", "" }, */
    /* { 0x16, "", "" }, */
    {0x17, "fatchk", "sp?"},
    {0x18, "hendsp", "w?"},
    {0x19, "curdrv", ""},
    {0x1a, "getss", "p"},
    {0x1b, "fgetc", "w"},
    {0x1c, "fgets", "pw"},
    {0x1d, "fputc", "ww"},
    {0x1e, "fputs", "sw"},
    {0x1f, "allclose", ""},
    {0x20, "super", "p"},
    {0x21, "fnckey", "wp"},
    {0x22, "knjctrl", "l?"},
    {0x23, "conctrl", "w?"},
    {0x24, "keyctrl", "w?"},
    {0x25, "intvcs", "wp"},
    {0x26, "pspset", "p"},
    {0x27, "gettim2", ""},
    {0x28, "settim2", "l"},
    {0x29, "namests", "sp"},
    {0x2a, "getdate", ""},
    {0x2b, "setdate", "w"},
    {0x2c, "gettime", ""},
    {0x2d, "settime", "w"},
    {0x2e, "verify", "w"},
    {0x2f, "dup0", "ww"},
    {0x30, "vernum", ""},
    {0x31, "keeppr", "lw"},
    {0x32, "getdpb", "wp"},
    {0x33, "breakck", "w"},
    {0x34, "drvxchg", "ww"},
    {0x35, "intvcg", "w"},
    {0x36, "dskfre", "wp"},
    {0x37, "nameck", "sp"},
    /* { 0x38, "", "" }, */
    {0x39, "mkdir", "s"},
    {0x3a, "rmdir", "s"},
    {0x3b, "chdir", "s"},
    {0x3c, "create", "sw"},
    {0x3d, "open", "sw"},
    {0x3e, "close", "w"},
    {0x3f, "read", "wpl"},
    {0x40, "write", "wpl"},
    {0x41, "delete", "s"},
    {0x42, "seek", "wlw"},
    {0x43, "chmod", "sw"},
    {0x44, "ioctrl", "w?"},
    {0x45, "dup", "w"},
    {0x46, "dup2", "ww"},
    {0x47, "curdir", "wp"},
    {0x48, "malloc", "l"},
    {0x49, "mfree", "p"},
    {0x4a, "setblock", "pl"},
    {0x4b, "exec", "w?"},
    {0x4c, "exit2", "w"},
    {0x4d, "wait", ""},
    {0x4e, "files", "psw"},
    {0x4f, "nfiles", "p"},

    {0x50, "v2_setpdb", "p"},
    {0x51, "v2_getpdb", ""},
    {0x52, "v2_setenv", "sps"},
    {0x53, "v2_getenv", "spp"},
    {0x54, "v2_verifyg", ""},
    {0x55, "v2_common", "w?"},
    {0x56, "v2_rename", "ss"},
    {0x57, "v2_filedate", "wl"},
    {0x58, "v2_malloc2", "wl?"},
    /* { 0x59, "", "" }, */
    {0x5a, "v2_maketmp", "sw"},
    {0x5b, "v2_newfile", "sw"},
    {0x5c, "v2_lock", "wwll"},
    /* { 0x5d, "", "" }, */
    /* { 0x5e, "", "" }, */
    {0x5f, "v2_assign", "w?"},

    {0x60, "v2_malloc3", "l"},    /* 060turbo.sys */
    {0x61, "v2_setblock2", "pl"}, /* 060turbo.sys */
    {0x62, "v2_malloc4", "wl?"},  /* 060turbo.sys */

    {0x7a, "v2_fflushflg", "w"},
    {0x7b, "v2_ospatch", "wp"},
    {0x7c, "v2_getfcb", "w"},
    {0x7d, "v2_s_malloc", "wl"},
    {0x7e, "v2_s_mfree", "p"},
    {0x7f, "v2_s_process", "wpll"},

    {0xb0, "twon", "w?"},  /* (V)TwentyOne.sys */
    {0xb1, "mvdir", "w?"}, /* dos_mvdir.r */

    /* { 0xf0, "exitvc", "" }, */
    /* { 0xf1, "ctrlvc", "" }, */
    /* { 0xf2, "errjvc", "" }, */
    {0xf3, "diskred", "pw?"},
    {0xf4, "diskwrt", "pw?"},
    {0xf5, "indosflg", ""},
    {0xf6, "super_jsr", "p"},
    {0xf7, "bus_err", "ppw"},
    {0xf8, "open_pr", "swppwppl"},
    {0xf9, "kill_pr", ""},
    {0xfa, "get_pr", "wp"},
    {0xfb, "suspend_pr", "w"},
    {0xfc, "sleep_pr", "l"},
    {0xfd, "send_pr", "wwwpl"},
    {0xfe, "time_pr", ""},
    {0xff, "change_pr", ""},
};

static System_call Kflush_call_info[] = {
    {"kflush", "w"},
    {"kflush{gp}", "w"},
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"kflush{io}", "ww"},
    {"kflush{in}", "w"},
    {"kflush{gc}", "w"},
    {"", ""},
    {"kflush{gs}", "wp"},
};

static System_call Hendsp_call_info[] = {
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

static System_call Knjctrl_call_info[] = {
    {"", ""}, /* 0 */
    {"knjctrl", "ll"},
    {"knjctrl", "l"},
    {"knjctrl", "ll"},
    {"knjctrl", "l"},
    {"knjctrl", "ll"}, /* 5 */
    {"knjctrl", "l"},
    {"knjctrl", "ll"},
    {"knjctrl", "l"},
    {"knjctrl", "ll"},
    {"knjctrl", "l"}, /* 10 */
    {"knjctrl", "ll"},
    {"knjctrl", "l"},
    {"knjctrl", "lsl"},
    {"knjctrl", "lsl"},
    {"knjctrl", "lsl"}, /* 15 */
    {"knjctrl", "lsl"},
    {"knjctrl", "lsl"},
    {"knjctrl", "lsl"},
    {"knjctrl", "lsll"},
    {"knjctrl", "lll"}, /* 20 */
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "ll"},
    {"knjctrl", "llllll"}, /* 25 */
    {"knjctrl", "lsll"},
    {"knjctrl", "llll"},
    {"knjctrl", "l"},
    {"knjctrl", "l"},
    {"knjctrl", "llssl"}, /* 30 */
    {"knjctrl", "llssl"},
    {"knjctrl", "lss"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"}, /* 35 */
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"},
    {"knjctrl", "lll"}, /* 40 */
    {"knjctrl", "lll"},
    {"knjctrl", "l"},
    {"", ""},
    {"", ""},
    {"", ""}, /* 45 */
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
    {"knjctrl", "l"}, /* 50 */
    {"knjctrl", "l"},
    {"knjctrl", "ls"},
    {"knjctrl", "l"},
    {"knjctrl", "ll"},
    {"knjctrl", "l"}, /* 55 */
    {"knjctrl", "ll"},
    {"", ""},
    {"", ""},
    {"", ""},
    {"knjctrl", "lp"}, /* 60 */
    {"knjctrl", "l"},
    {"knjctrl", "lpsl"},
    {"knjctrl", "lpll"},
};

static System_call Conctrl_call_info[] = {
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

static System_call Keyctrl_call_info[] = {
    {"keyctrl{keyinp}", "w"},   //
    {"keyctrl{keysns}", "w"},   //
    {"keyctrl{sftsns}", "w"},   //
    {"keyctrl{keybit}", "ww"},  //
    {"keyctrl{insmod}", "ww"},
};

static System_call Ioctrl_call_info[] = {
    {"ioctrl{gt}", "ww"},
    {"ioctrl{st}", "www"},
    {"ioctrl{rh}", "wwll"},
    {"ioctrl{wh}", "wwll"},
    {"ioctrl{rd}", "wwll"},
    {"ioctrl{wd}", "wwll"},
    {"ioctrl{is}", "ww"},
    {"ioctrl{os}", "ww"},
    {"", ""},
    {"ioctrl{dvgt}", "ww"},
    {"ioctrl{fdgt}", "ww"},
    {"ioctrl{rtset}", "www"},
    {"ioctrl{dvctl}", "wwwl"},
    {"ioctrl{fdctl}", "wwwl"},
};

static System_call Exec_call_info[] = {
    {"exec{loadexec}", "wssp"},  //
    {"exec{load}", "wssp"},      //
    {"exec{pathchk}", "wspp"},   //
    {"exec{loadonly}", "wspp"},  //
    {"exec{execonly}", "wp"},    //
    {"exec{bindno}", "wss"},
};

static System_call v2Common_call_info[] = {
    {"v2_common{ck}", "ws"},      //
    {"v2_common{rd}", "wslll"},   //
    {"v2_common{wt}", "wslll"},   //
    {"v2_common{lk}", "wslll"},   //
    {"v2_common{fre}", "wslll"},  //
    {"v2_common{del}", "ws"},
};
static System_call Common_call_info[] = {
    {"common{ck}", "ws"},      //
    {"common{rd}", "wslll"},   //
    {"common{wt}", "wslll"},   //
    {"common{lk}", "wslll"},   //
    {"common{fre}", "wslll"},  //
    {"common{del}", "ws"},
};

static System_call v2Assign_call_info[] = {
    {"v2_assign{getassign}", "wsp"},
    {"v2_assign{makeassign}", "wssw"},
    {"", ""},
    {"", ""},
    {"v2_assign{rassign}", "ws"},
};
static System_call Assign_call_info[] = {
    {"assign{getassign}", "wsp"},
    {"assign{makeassign}", "wssw"},
    {"", ""},
    {"", ""},
    {"assign{rassign}", "ws"},
};

static System_call Twon_call_info[] = {
    {"twon{getid}", "w"},     //
    {"twon{getver}", "w"},    //
    {"twon{getadr}", "w"},    //
    {"twon{getopt}", "w"},    //
    {"twon{setopt}", "wl"},   //
    {"twon{getsysr}", "wp"},  //
    {"twon{setsysr}", "ws"},  //
    {"twon{u2s}", "ww"},
};

static System_call Mvdir_call_info[] = {
    {"mvdir{getid}", "w"},
    {"mvdir{getver}", "w"},
    {"mvdir{unhook}", "w"},
    {"mvdir{move}", "wss"},
};

/* EOF */
