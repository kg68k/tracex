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
#include <stdio.h>
#include <string.h>

#ifdef __HUMAN68K__
#include <jctype.h>
#else
// Human68k以外のIDEでjctype.hがないという警告を消す細工
extern int iskanji(int);
extern int iskanji2(int);
extern int isprkana(int);
#endif

#include "syscall.h"
#include "trace.h"

#ifdef ADDRESS_24BIT
#define ADDRESS_MASK 0x00ffffff
#else
#define ADDRESS_MASK 0x1fffffff /* 060turbo */
#endif

static void decode_argument_by_letter(char* buffer, const char* name,
                                      const char* argletter, const void* arg);

static SystemCallReturnType format_sub(const SystemCallInfo* sub,
                                       const void* arg, char* buffer,
                                       const char* name) {
  const unsigned int callno = sub->getSubCallNo(arg);
  const int isValid = (callno < sub->length);

  if (!isValid || sub->list[callno].name == NULL) {
    sprintf(buffer, "%s(%d){UNDEFINED}", name, callno);
    return isValid ? sub->list[callno].returnType : RET_INT;
  }

  decode_argument_by_letter(buffer, sub->list[callno].name,
                            sub->list[callno].argletter, arg);
  return sub->list[callno].returnType;
}

char* Format_output(unsigned int doscall, void* arg,
                    SystemCallReturnType* retType) {
  static char argbuf[256];
  char* writeptr = argbuf;

  const SystemCall* dos = &HumanInfo.list[doscall];
  const SystemCallInfo* sub;

  *retType = dos->returnType;

  if (dos->name == NULL) {
    sprintf(argbuf, "dos(0x%02x){UNDEFINED}", doscall);
    return argbuf;
  }

  if (0x50 <= doscall && doscall <= 0x7f) {
    // 0xff50～0xff7fのDOSコールは頭にv2_を付ける
    strcpy(writeptr, "v2_");
    writeptr += strlen(writeptr);
  }

  sub = GetSubCallInfo(doscall);
  if (sub) {
    // モードによって引数の型が変わるDOSコールの処理
    *retType = format_sub(sub, arg, writeptr, dos->name);
  } else {
    // 引数の型が一定なDOSコールの処理
    decode_argument_by_letter(writeptr, dos->name, dos->argletter, arg);
  }

  return argbuf;
}

static char* escapeContrlCode(unsigned char c, char* buf) {
  static const char escChars[] = {
      0, 0, 0, 0, 0, 0, 0, 'a', 'b', 't', 'n', 'v', 'f', 'r'  //
  };

  if (c < sizeof(escChars) && escChars[c]) {
    *buf++ = '\\';
    *buf++ = escChars[c];
  } else {
    buf += sprintf(buf, "\\x%02x", c);
  }

  return buf;
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
  for (count = 0; *p && count < 32; count++) {
    unsigned char c = *p++;

    if (isprkana(c)) {
      if (c == '\\' || c == '\"' || c == '\'') {
        *buffer++ = '\\';
      }
      *buffer++ = c;
    } else if (iscntrl(c)) {
      buffer = escapeContrlCode(c, buffer);
    } else if (iskanji(c) && iskanji2(*p)) {
      *buffer++ = c;
      *buffer++ = *p++;
      count++;
    } else {
      *buffer++ = '.';
    }
  }
  *buffer++ = '\"';

  if (*p) {
    *buffer++ = '.';
    *buffer++ = '.';
  }
  *buffer++ = '\0';
}

static const void* advance(const void* arg, int n) {
  return (const void*)((const char*)arg + n);
}

static int formatWord(char* buf, const void** argptr) {
  const void* arg = *argptr;
  int v = *(unsigned short*)arg;
  *argptr = advance(arg, 2);
  return (v < 10) ? sprintf(buf, "%d", v) : sprintf(buf, "%d(%#x)", v, v);
}

static int formatWordHex(char* buf, const void** argptr) {
  const void* arg = *argptr;
  int v = *(unsigned short*)arg;
  *argptr = advance(arg, 2);
  return (v < 10) ? sprintf(buf, "%x", v) : sprintf(buf, "%#x", v);
}

static int formatLong(char* buf, const void** argptr) {
  const void* arg = *argptr;
  unsigned int v = *(unsigned int*)arg;
  *argptr = advance(arg, 4);
  return (v < 10) ? sprintf(buf, "%d", (int)v)
                  : sprintf(buf, "%d(%#x)", (int)v, v);
}

static int formatLongHex(char* buf, const void** argptr) {
  const void* arg = *argptr;
  unsigned int v = *(unsigned int*)arg;
  *argptr = advance(arg, 4);
  return (v < 10) ? sprintf(buf, "%d", (int)v) : sprintf(buf, "%#x", v);
}

static void decode_argument_by_letter(char* buffer, const char* name,
                                      const char* argletter, const void* arg) {
  char temp[256];

  strcpy(buffer, name);
  buffer += strlen(buffer);

  *buffer++ = '(';

  if (argletter == NULL) argletter = "";

  while (*argletter) {
    switch (*argletter++) {
      case 'w':  // ワード値
        buffer += formatWord(buffer, &arg);
        break;
      case 'h':  // ワード値(16進数表記)
        buffer += formatWordHex(buffer, &arg);
        break;
      case 'l':  // ロングワード値
        buffer += formatLong(buffer, &arg);
        break;
      case 'x':  // ロングワード値(16進数表記)
        buffer += formatLongHex(buffer, &arg);
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
        arg = advance(arg, 4);
        break;
      case 's': /* 文字列 */
        escape((char*)*(long*)arg, temp);
        /* buffer += sprintf (buffer, "%s", temp); */
        strcpy(buffer, temp);
        buffer += strlen(buffer);
        arg = advance(arg, 4);
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
