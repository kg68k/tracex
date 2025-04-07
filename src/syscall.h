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

typedef struct {
  const int number;
  const char* name;
  const char* argletter;
} RawSystemCall;

typedef struct {
  const int length;
  const RawSystemCall* array;
} RawSystemCallSlice;

typedef struct {
  const char* name;
  const char* argletter;
} SystemCall;

typedef struct {
  const int length;
  const SystemCall* array;
} SystemCallSlice;

RawSystemCallSlice get_raw_doscall_slice(void);

#define DEFINE_GET_SLICE(name) SystemCallSlice get_##name##_call_slice(void);
DEFINE_GET_SLICE(Kflush);
DEFINE_GET_SLICE(Hendsp);
DEFINE_GET_SLICE(Knjctrl);
DEFINE_GET_SLICE(Conctrl);
DEFINE_GET_SLICE(Keyctrl);
DEFINE_GET_SLICE(Ioctrl);
DEFINE_GET_SLICE(Exec);
DEFINE_GET_SLICE(v2Common);
DEFINE_GET_SLICE(Common);
DEFINE_GET_SLICE(v2Assign);
DEFINE_GET_SLICE(Assign);
DEFINE_GET_SLICE(Twon);
DEFINE_GET_SLICE(Mvdir);
#undef DEFINE_GET_SLICE

#endif
