// define error code
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

#include "error.h"

#include <stdio.h>

static const char* Human_error_message[] = {
    NULL,
    "Invalid function call number",                // -1
    "No such file or directory",                   // -2
    "No such directory",                           // -3
    "Too many opened file",                        // -4
    "Cannot access to directory or volume label",  // -5
    "Handle not opened",                           // -6
    "Memory block broken",                         // -7
    "No enough memory to execute",                 // -8
    "Invalid memory block pointer",                // -9
    "Invalid environment",                         // -10
    "Execute file format error",                   // -11
    "Invalid access mode in open",                 // -12
    "Invalid filename",                            // -13
    "Invalid parameter",                           // -14
    "Invalid drive",                               // -15
    "Cannot remove current directory",             // -16
    "Device cannot ioctrl",                        // -17
    "File not found more",                         // -18
    "Write protected",                             // -19
    "Directory already exists",                    // -20
    "Directory not empty",                         // -21
    "File already exists",                         // -22
    "Disk full",                                   // -23
    "Directory full",                              // -24
    "Cannot seek",                                 // -25
    "Already supervisor mode",                     // -26
    "Process already exists",                      // -27
    "Message not received",                        // -28
    "Invalid process number",                      // -29
    NULL,                                          // -30
    NULL,                                          // -31
    "Share file over",                             // -32
    "File locking error",                          // -33
    "Active device",                               // -34
    "Too many symbolic links",                     // -35
};

const char* get_human_error_message(int error) {
  if (error < 0) {
    const int len =
        sizeof(Human_error_message) / sizeof(Human_error_message[0]);
    int e = -error;

    if (e < len) return Human_error_message[e];
    if (e == 80) return "File exists";
  }

  return NULL;
}
