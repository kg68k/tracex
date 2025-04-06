/* -*-c-*-

   define system call
   ( included from argument.c )

   Copyright (C) 1991 K.Abe

*/

typedef struct {
    char	*name;
    char	*argletter;
} System_call;


typedef struct {
    int		number;
    char	*name;
    char	*argletter;
} raw_system_call;


static raw_system_call Human_system_call[] = {
    0		, "exit"	, "" ,
    1		, "getchar"	, "" ,
    2		, "putchar"	, "w" ,
    3		, "cominp"	, "" ,
    4		, "comout"	, "w" ,
    5		, "prnout"	, "w" ,
    6		, "inpout"	, "w" ,
    7		, "inkey"	, "" ,
    8		, "getc"	, "" ,
    9		, "print"	, "s" ,
    0xa		, "gets"	, "p" ,
    0xb		, "keysns"	, "" ,
    0xc 	, "kflush"	, "wp" ,
    0xd 	, "fflush"	, "" ,
    0xe		, "chgdrv"	, "w" ,
    0xf		, "drvctrl"	, "w" ,
    0x10	, "consns"	, "" ,
    0x11	, "prnsns"	, "" ,
    0x12	, "cinsns"	, "" ,
    0x13	, "coutsns"	, "" ,
    0x14	, ""		, "" ,
    0x15	, ""		, "" ,
    0x16	, ""		, "" ,
    0x17	, "fatchk"	, "sp" ,
    0x18	, "hendsp"	, "w?" ,
    0x19	, "curdrv"	, "" ,
    0x1a	, "getss"	, "p" ,
    0x1b	, "fgetc"	, "w" ,
    0x1c	, "fgets"	, "pw" ,
    0x1d	, "fputc"	, "ww" ,
    0x1e	, "fputs"	, "sw" ,
    0x1f	, "allclose"	, "" ,
    0x20	, "super"	, "p" ,
    0x21	, "fnckey"	, "wp" ,
    0x22	, "knjctrl"	, "l?" ,
    0x23	, "conctrl"	, "w?" ,
    0x24	, "keyctrl"	, "w?" ,
    0x25	, "intvcs"	, "wp" ,
    0x26	, "pspset"	, "p" ,
    0x27	, "gettim2"	, "" ,
    0x28	, "settim2"	, "l" ,
    0x29	, "namests"	,  "sp" ,
    0x2a	, "getdate"	, "" ,
    0x2b	, "setdate"	, "w" ,
    0x2c	, "gettime"	, "" ,
    0x2d	, "settime"	, "w" ,
    0x2e	, "verify"	, "w" ,
    0x2f	, "dup0"	, "ww" ,
    0x30	, "vernum"	, "" ,
    0x31	, "keeppr"	, "lw" ,
    0x32	, "getdpb"	, "wp" ,
    0x33	, "breakck"	, "w" ,
    0x34	, "drvxchg"	, "ww" ,
    0x35	, "intvcg"	, "w" ,
    0x36	, "dskfre"	, "wp" ,
    0x37	, "nameck"	, "sp" ,
    0x38	, ""		, "" ,
    0x39	, "mkdir"	, "s" ,
    0x3a	, "rmdir"	, "s" ,
    0x3b	, "chdir"	, "s" ,
    0x3c	, "create"	, "sw" ,
    0x3d	, "open"	, "sw" ,
    0x3e	, "close"	, "w" ,
    0x3f	, "read"	, "wpl" ,
    0x40	, "write"	, "wpl" ,
    0x41	, "delete"	, "s" ,
    0x42	, "seek"	, "wlw" ,
    0x43	, "chmod"	, "sw" ,
    0x44	, "ioctrl"	, "w?" ,
    0x45	, "dup"		, "w" ,
    0x46	, "dup2"	, "ww" ,
    0x47	, "curdir"	, "wp" ,
    0x48	, "malloc"	, "l" ,
    0x49	, "mfree"	, "p" ,
    0x4a	, "setblock"	, "pl" ,
    0x4b	, "exec"	, "wsss" ,
    0x4c	, "exit2"	, "w" ,
    0x4d	, "wait"	, "" ,
    0x4e	, "files"	, "psw" ,
    0x4f	, "nfiles"	, "p" ,
    0x50	, "setpdb"	, "p" ,
    0x51	, "getpdb"	, "" ,
    0x52	, "setenv"	, "sps" ,
    0x53	, "getenv"	, "ssp" ,
    0x54	, "verifyg"	, "" ,
    0x55	, "common"	, "w?" ,
    0x56	, "rename"	, "ss" ,
    0x57	, "filedate"	, "wl" ,
    0x58	, "malloc2"	, "wl" ,
    0x59	, ""		, "" ,
    0x5a	, "maketmp"	, "sw" ,
    0x5b	, "newfile"	, "sw" ,
    0x5c	, "lock"	, "wwll" ,
    0x5d	, ""		, "" ,
    0x5e	, ""		, "" ,
    0x5f	, "getassign"	, "w?" ,
    0x7d	, "s_malloc"	, "wl" ,
    0x7e	, "s_mfree"	, "p" ,
    0x7f	, "s_process"	, "wlll" ,
    0xf3	, "diskred"	, "pw?" ,
    0xf4	, "diskwrt"	, "pw?" ,
    0xf5	, "indosflg"	, "" ,
    0xf6	, "super_jsr"	, "p" ,
    0xf7	, "bus_err"	, "ppw" ,
    0xf8	, "open_pr"	, "swllwlll" ,
    0xf9	, "kill_pr"	, "" ,
    0xfa	, "get_pr"	, "wp" ,
    0xfb	, "suspend_pr"	, "w" ,
    0xfc	, "sleep_pr"	, "l" ,
    0xfd	, "send_pr"	, "wwwpl" ,
    0xfe	, "time_pr"	, "" ,
    0xff	, "change_pr"	, "" ,
};


System_call	Ioctrl_call_info[] = 
{
    "ioctrl{gt}"	, "ww" ,
    "ioctrl{st}"	, "www" ,
    "ioctrl{rh}"	, "wwll" ,
    "ioctrl{wh}"	, "wwll" ,
    "ioctrl{rd}"	, "wwll" ,
    "ioctrl{wd}"	, "wwll" ,
    "ioctrl{is}"	, "ww" ,
    "ioctrl{os}"	, "ww" ,
    ""			, "" ,
    "ioctrl{dvgt}"	, "ww" ,
    "ioctrl{fdgt}"	, "ww" ,
    "ioctrl{rtset}"	, "www" ,
    "ioctrl{dvctl}"	, "wwwl" ,
    "ioctrl{fdctl}"	, "wwwl" ,
};


static System_call	Knjctrl_call_info[] = {
    ""		, "" ,		/* 0 */
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "ll" ,	/* 5 */
    "knjctrl"	, "l" ,
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,		/* 10 */
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "lsl" ,
    "knjctrl"	, "lsl" ,
    "knjctrl"	, "lsl" ,	/* 15 */
    "knjctrl"	, "lsl" ,
    "knjctrl"	, "lsl" ,
    "knjctrl"	, "lsl" ,
    "knjctrl"	, "lsll" ,
    "knjctrl"	, "lll" ,	/* 20 */
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "ll" ,
    "knjctrl"	, "llllll" ,	/* 25 */
    "knjctrl"	, "lsll" ,
    "knjctrl"	, "llll" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "llssl" ,	/* 30 */
    "knjctrl"	, "llssl" ,
    "knjctrl"	, "lss" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,	/* 35 */
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,
    "knjctrl"	, "lll" ,	/* 40 */
    "knjctrl"	, "lll" ,
    ""		, "" ,
    ""		, "" ,
    ""		, "" ,
    ""		, "" ,		/* 45 */
    ""		, "" ,
    ""		, "" ,
    ""		, "" ,
    ""		, "" ,
    "knjctrl"	, "l" ,		/* 50 */
    "knjctrl"	, "l" ,
    "knjctrl"	, "ls" ,
    "knjctrl"	, "l" ,
    "knjctrl"	, "ll" ,
    "knjctrl"	, "l" ,		/* 55 */
    "knjctrl"	, "ll" ,
};


static System_call	Exec_call_info[] = {
    "exec{loadexec}"	, "wssp" ,
    "exec{load}"	, "wssp" ,
    "exec{pathchk}"	, "wspp" ,
    "exec{loadonly}"	, "wspp" ,
    "exec{execonly}"	, "wpsp" ,
    "exec{bindno}"	, "wss" ,
};


static System_call	Keyctrl_call_info[] = {
    "keyctrl{keyinp}"	, "w" ,
    "keyctrl{keysns}"	, "w" ,
    "keyctrl{sftsns}"	, "w" ,
    "keyctrl{keybit}"	, "ww" ,
    "keyctrl{insmod}"	, "ww" ,
};


static System_call	Conctrl_call_info[] = {
    "conctrl{putc}"		, "ww" ,
    "conctrl{print}"		, "ws" ,
    "conctrl{color}"		, "ww" ,
    "conctrl{locate}"		, "www" ,
    "conctrl{down_s}"		, "w" ,
    "conctrl{up_s}"		, "w" ,
    "conctrl{up}"		, "ww" ,
    "conctrl{down}"		, "ww" ,
    "conctrl{right}"		, "ww" ,
    "conctrl{left}"		, "ww" ,
    "conctrl{cls}"		, "ww" ,
    "conctrl{era}"		, "ww" ,
    "conctrl{ins}"		, "ww" ,
    "conctrl{del}"		, "ww" ,
    "conctrl{fnkmod}"		, "ww" ,
    "conctrl{window}"		, "www" ,
    "conctrl{width}"		, "ww" ,
    "conctrl{curon}"		, "w" ,
    "conctrl{curoff}"		, "w" ,
};


static System_call	Common_call_info[] = {
    "common{ck}"		, "ws" ,
    "common{rd}"		, "wslll" ,
    "common{wt}"		, "wslll" ,
    "common{lk}"		, "wslll" ,
    "common{fre}"		, "wslll" ,
    "common{del}"		, "ws" ,
};


static System_call	Kflush_call_info[] = {
    "kflush"			, "w" ,
    "kflush{gp}"		, "w" ,
    ""				, "" ,
    ""				, "" ,
    ""				, "" ,
    ""				, "" ,
    "kflush{io}"		, "ww" ,
    "kflush{in}"		, "w" ,
    "kflush{gc}"		, "w" ,
    ""				, "" ,
    "kflush{gs}"		, "wp" ,
};


static System_call	Hendsp_call_info[] = {
    "hendsp{mo}"		, "wwp" ,
    "hendsp{mp}"		, "wwp" ,
    "hendsp{mr}"		, "wwp" ,
    "hendsp{mc}"		, "w" ,
    "hendsp{io}"		, "w" ,
    "hendsp{ip}"		, "wwp" ,
    "hendsp{ir}"		, "wwp" ,
    "hendsp{ic}"		, "ww" ,
    "hendsp{so}"		, "w" ,
    "hendsp{sp}"		, "wwp" ,
    "hendsp{sr}"		, "wwp" ,
    "hendsp{sc}"		, "w" ,
};


static System_call	Getassign_call_info[] = {
    "getassign{getassign}"	, "wsp" ,
    "getassign{makeassign}"	, "wssw" ,
    ""				, "" ,
    ""				, "" ,
    "getassign{rassign}"	, "ws" ,
};
