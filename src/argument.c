// decode arguments
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

#include <ctype.h>
#include <jctype.h>
#include <stdio.h>
#include <string.h>

#include "syscall.h"
#include "trace.h"

#ifdef ADDRESS_24BIT
#define ADDRESS_MASK 0x00ffffff
#else
#define ADDRESS_MASK 0x1fffffff /* 060turbo */
#endif

static void decode_argument(int call, const void* arg, char* buffer,
                            const char* name, SystemCallSlice slice);
static void decode_argument_by_letter(char* buffer, const char* argletter,
                                      const void* arg);

static SystemCall System_call_info[256];

void Initialize_argument_information(void) {
  const RawSystemCallSlice slice = get_raw_doscall_slice();
  int i;

  for (i = 0; i < 256; i++) System_call_info[i].name = NULL;

  for (i = 0; i < slice.length; i++) {
    const RawSystemCall* call = &slice.array[i];
    const int j = call->number;

    System_call_info[j].name = call->name;
    System_call_info[j].argletter = call->argletter;

#define SKIP_LEN 3 /* strlen ("v2_") */

    if ((unsigned char)(j - 0x50) <= (0x7f - 0x50)) {
      System_call_info[j + 0x30].name = call->name + SKIP_LEN;
      System_call_info[j + 0x30].argletter = call->argletter;
    }
  }
}

// _countof()
#define C(array) (sizeof(array) / sizeof(array[0]))

SystemCallSlice get_doscall_slice(void) {
  SystemCallSlice slice = {C(System_call_info), System_call_info};
  return slice;
}

static void fatchk(int doscall, void* arg, char* argbuf) {
  char* argletter;

  strcpy(argbuf, System_call_info[doscall].name);
  argbuf += strlen(argbuf);

  argletter = "sp";
  if (*(short*)(arg + 4) < 0) {
    *argbuf++ = '{';
    *argbuf++ = '2';
    *argbuf++ = '}';
    argletter = "spw";
  }

  decode_argument_by_letter(argbuf, argletter, arg);
}

static void malloc2(int doscall, void* arg, char* argbuf) {
  char* argletter;

  strcpy(argbuf, System_call_info[doscall].name);
  argbuf += strlen(argbuf);

  argletter = "wl";
  if (*(short*)arg < 0) {
    *argbuf++ = '{';
    *argbuf++ = '2';
    *argbuf++ = '}';
    argletter = "wlp";
  }

  decode_argument_by_letter(argbuf, argletter, arg);
}

static void disk(int doscall, void* arg, char* argbuf) {
  char* argletter;

  strcpy(argbuf, System_call_info[doscall].name);
  argbuf += strlen(argbuf);

  argletter = "pwww";
  if (*(short*)arg < 0) {
    *argbuf++ = '{';
    *argbuf++ = '2';
    *argbuf++ = '}';
    argletter = "pwll";
  }

  decode_argument_by_letter(argbuf, argletter, arg);
}

#define DECODE(first, info_name)                    \
  decode_argument(*(first*)arg & 0xff, arg, argbuf, \
                  System_call_info[doscall].name,   \
                  get_##info_name##_call_slice())

char* Format_output(int doscall, void* arg) {
  static char argbuf[256];

  if (doscall < 0 || 255 < doscall || System_call_info[doscall].name == NULL) {
    sprintf(argbuf, "dos(%#x){UNDEFINED}", doscall);
    return argbuf;
  }

  switch (doscall) {
    case 0x0c: /* kflush */
      DECODE(short, Kflush);
      break;
    case 0x17: /* fatchk */
      fatchk(doscall, arg, argbuf);
      break;
    case 0x18: /* hendsp */
      DECODE(short, Hendsp);
      break;
    case 0x44: /* ioctrl */
      DECODE(short, Ioctrl);
      break;
    case 0x22: /* knjctrl */
      DECODE(long, Knjctrl);
      break;
    case 0x23: /* conctrl */
      DECODE(short, Conctrl);
      break;
    case 0x24: /* keyctrl */
      DECODE(short, Keyctrl);
      break;
    case 0x4b: /* exec */
      DECODE(short, Exec);
      break;
    case 0x55: /* v2_common */
      DECODE(short, v2Common);
      break;
    case 0x58: /* v2_malloc2 */
    case 0x60: /* v2_malloc4 */
    case 0x88: /* malloc2 */
    case 0x90: /* malloc4 */
      malloc2(doscall, arg, argbuf);
      break;
    case 0x5f: /* v2_assign */
      DECODE(short, v2Assign);
      break;
    case 0x85: /* common */
      DECODE(short, Common);
      break;
    case 0x8f: /* assign */
      DECODE(short, Assign);
      break;
    case 0xb0: /* twon */
      DECODE(short, Twon);
      break;
    case 0xb1: /* mvdir */
      DECODE(short, Mvdir);
      break;
    case 0xf3: /* diskred */
    case 0xf4: /* diskwrt */
      disk(doscall, arg, argbuf);
      break;
    default:
      decode_argument(doscall, arg, argbuf, "dos", get_doscall_slice());
      break;
  }
  return argbuf;
}

static void decode_argument(int call, const void* arg, char* buffer,
                            const char* name, SystemCallSlice slice) {
  if (call >= slice.length || slice.array[call].name[0] == '\0') {
    sprintf(buffer, "%s(%d){UNDEFINED}", name, call);
    return;
  }

  strcpy(buffer, slice.array[call].name);
  decode_argument_by_letter(buffer + strlen(buffer),
                            slice.array[call].argletter, arg);
}

/* 文字列(またはアドレス)を表示する */
static void escape(char* str, char* buffer) {
  unsigned char* p = str;
  int count;

  if (p > (unsigned char*)ADDRESS_MASK) {
    buffer += sprintf(buffer, "(%#x)", (unsigned int)p);
    p = (unsigned char*)((unsigned long)p & ADDRESS_MASK);
  }

  *buffer++ = '\"';
  for (count = 0; *p && count < 32; p++, count++) {
    if (isprkana(*p))
      *buffer++ = *p;
    else if (iscntrl(*p))
      buffer += sprintf(buffer, "\\x%02x", *p);
    else if (iskanji(p[0]) && iskanji2(p[1])) {
      *buffer++ = *p++;
      *buffer++ = *p;
      count++;
    } else
      *buffer++ = '.';
  }
  *buffer++ = '\"';

  if (*p) {
    *buffer++ = '.';
    *buffer++ = '.';
  }
  *buffer++ = '\0';
}

static void decode_argument_by_letter(char* buffer, const char* argletter,
                                      const void* arg) {
  char temp[256];

  *buffer++ = '(';

  while (*argletter) {
    switch (*argletter++) {
      case 'w': /* ワード値 */
        buffer += sprintf(buffer, "%d", *(short*)arg);
        if (*(unsigned short*)arg >= 10)
          buffer += sprintf(buffer, "(%#x)", *(short*)arg & 0xffff);
        arg += 2;
        break;
      case 'l': /* ロングワード値 */
        buffer += sprintf(buffer, "%d", *(int*)arg);
        if (*(long*)arg >= 10) buffer += sprintf(buffer, "(%#x)", *(int*)arg);
        arg += 4;
        break;
      case 'p': /* ポインタ */
        if ((void*)*(long*)arg)
          buffer += sprintf(buffer, "%#x", *(int*)arg);
        else {
          *buffer++ = 'N'; /* "NULL" */
          *buffer++ = 'U';
          *buffer++ = 'L';
          *buffer++ = 'L';
        }
        arg += 4;
        break;
      case 's': /* 文字列 */
        escape((char*)*(long*)arg, temp);
        /* buffer += sprintf (buffer, "%s", temp); */
        strcpy(buffer, temp);
        buffer += strlen(buffer);
        arg += 4;
        break;
      default:
        goto exit;
    }

    if (*argletter) *buffer++ = ','; /* まだ引数がある */
  }
exit:
  *buffer++ = ')';
  *buffer = '\0';
}

/* EOF */
