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

#ifndef SYSCALL_H
#define SYSCALL_H

typedef enum {
  RET_INT,
  RET_HEX,
  RET_PTR,
  RET_VOID,
} SystemCallReturnType;

typedef struct {
  const char* name;
  const char* argletter;
  SystemCallReturnType returnType;
} SystemCall;

typedef struct {
  unsigned int length;
  const SystemCall* list;
  unsigned int (*getSubCallNo)(const void* arg);
} SystemCallInfo;

extern SystemCallInfo HumanInfo;
const SystemCallInfo* GetSubCallInfo(unsigned char callno);

#endif
