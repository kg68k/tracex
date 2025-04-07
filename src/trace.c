// System call tracer
//
// Copyright (C)1991 K.Abe

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

#include "trace.h"

#include <process.h>
#include <stdio.h>
#include <stdlib.h>

static int usage(void);

FILE* Stream = stdout;
int Count;
char Option_A_flag;

static int trace(const char* command_name, const char* command_argv[]) {
  int rc;

  setup_doscall_handler();
  rc = spawnvp(P_WAIT, command_name, (void*)command_argv);
  restore_doscall_handler();

  if (rc == -1) {
    perror("spawnvp");
    return 1;
  }

  fprintf(Stream, "\nExit code %d (%d System Calls)\n", rc, Count);
  return 0;
}

int main(int argc, char* argv[]) {
  char* command_name;
  char** command_argv;
  char* ofile = getenv("TRACE_LOG");
  int i;

  if (argc <= 1) return usage();

  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    switch (argv[i][1]) {
      case 'o':
        if (argv[i][2])
          ofile = argv[i] + 2;
        else
          ofile = argv[++i];
        break;
      case 'a':
        Option_A_flag = -1;
        if (argv[i][2] == 'o') {
          if (argv[i][3])
            ofile = argv[i] + 3;
          else
            ofile = argv[++i];
        }
        break;
      default:
        return usage();
    }
  }
  if (argc <= i) return usage();

  check_human_version();

  if (ofile && !(ofile[0] == (char)'-' && ofile[1] == (char)'\0')) {
    if ((Stream = fopen(ofile, "w")) == NULL) {
      perror("fopen");
      return 1;
    }
  }

  Initialize_argument_information();
  command_name = argv[i];
  command_argv = &argv[i];
  return trace(command_name, (void*)command_argv);
}

static int usage(void) {
  printf(
      "%s\n"
      "Usage: trace [-a] [-o file] command arg ...",
      Title);

  return EXIT_FAILURE;
}
