// define error code (included from display.c)
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

#define ERROR_NUMBER_MIN \
  (int)(-(sizeof(Human_error_message) / sizeof(Human_error_message[0])))

static char *Human_error_message[] = {
    "File exists", /* -80 */
    "Invalid function call number",
    "No such file or directory",
    "No such directory",
    "Too many opened file",
    "Cannot access to directory or volume label",
    "Handle not opened",
    "Memory block broken",
    "No enough memory to execute",
    "Invalid memory block pointer",
    "Invalid environment",
    "Execute file format error",
    "Invalid access mode in open",
    "Invalid filename",
    "Invalid parameter",
    "Invalid drive",
    "Cannot remove current directory",
    "Device cannot ioctrl",
    "File not found more",
    "Write protected",
    "Directory already exists",
    "Directory not empty",
    "File already exists",
    "Disk full",
    "Directory full",
    "Cannot seek",
    "Already supervisor mode",
    "Undefined",
    "Process already exists",
    "Message not received",
    "Invalid process number",
    "Undefined",
    "Share file over",
    "File locking error",
    "Active device",           /* -34 */
    "Too many symbolic links", /* -35 */
};
