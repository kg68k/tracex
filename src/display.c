// display doscall name, arguments, result code (called from New_handler)
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

#include <stdio.h>
#include <unistd.h>

#include "error.h"
#include "trace.h"

#define CSRX (unsigned short *)(0x974)
#define WAIT (int *)(0x1cae)

static void indent(void);

static char *Formatted_string;
static int exec_level = -1;
static int Doscall_nestlevel = 0;
static char Last_instruction_not_return = 0;
static char exec_flag = 0;
static char Flush_flag = 1;

extern void display1(int callnum, void *arg) {
  const unsigned int callno = callnum & 0xff;

  if (Flush_flag == 0) {
    /* 再帰か終了系コール直後の返値相当の文字は次のコールで表示する */

    if (Option_A_flag || 1 == exec_flag) {
      indent();
      fputs(Formatted_string, Stream);
    }

    if (Last_instruction_not_return) {
      fputs("\n", Stream);

      if (!Option_A_flag && 1 == exec_flag && 0 == Doscall_nestlevel)
        exec_flag = 2;
      else
        Doscall_nestlevel--;

      if (Option_A_flag || 1 == exec_flag) {
        int wait = *WAIT;

        indent();

        if ((unsigned long)wait < 10)
          fprintf(Stream, "}=%d\n", wait);
        else
          fprintf(Stream, "}=%d(%#x)\n", wait, wait);
      }
    } else {
      if (Option_A_flag || 1 == exec_flag) fputs("={\n", Stream);
      Doscall_nestlevel++;
    }
  }

  Formatted_string = Format_output(callno, arg);

  Last_instruction_not_return =
      (callno == 0 || callno == 0x4c || callno == 0x31);

  if (callno == 0x4b && (*(short *)arg == 0 || *(short *)arg == 4))
    exec_level = Doscall_nestlevel;

  if (Option_A_flag || 1 == exec_flag) Count++;

  if (isatty(fileno(Stream))) fflush(Stream);

  Flush_flag = 0; /* DOSコール実行中は 0 */
}

extern void display2(int result) {
  if (Flush_flag) Doscall_nestlevel--;

  if (Option_A_flag || 1 == exec_flag) {
    indent();

    if (Flush_flag)
      putc('}', Stream);
    else
      fputs(Formatted_string, Stream);

    fprintf(Stream, "=%d", result);
    if ((unsigned long)result >= 10) fprintf(Stream, "(%#x)", result);

    if (result < 0) {
      const char *msg = get_human_error_message(result);
      if (msg) {
        fprintf(Stream, "(%s)", msg);
      }
    }

    putc('\n', Stream);
  }

  if (exec_level == Doscall_nestlevel) {
    if (result >= 0) {
      if (Option_A_flag || 1 == exec_flag) {
        indent();
        fputs("EXEC()={\n", Stream);
      }
      if (!Option_A_flag && 0 == exec_flag)
        exec_flag = 1;
      else
        Doscall_nestlevel++;
    }

    exec_level = -1;
  }

  Flush_flag = 1;
}

static void indent() {
  int i;

  if (isatty(fileno(Stream))) {
    fflush(Stream);
    if (*CSRX != 0) putc('\n', Stream);
  }

  for (i = Doscall_nestlevel; i; i--) {
    putc(' ', Stream);
    putc(' ', Stream);
  }
  /*  fprintf( Stream , "<%d>" , Doscall_nestlevel ); */
}

/* EOF */
