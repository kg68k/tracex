/* -*-c-*-

   define error code
   ( included from display.c )
 
   Copyright (C) 1991 K.Abe

*/


#define ERROR_NUMBER_MIN\
    (-(sizeof( Human_error_message ) / sizeof( Human_error_message[0] )))
static char	*Human_error_message[] = {
    "----" ,
    "Invalid function call number" ,
    "No such file or directory" ,
    "No such directory" ,
    "Too many opened file" ,
    "Cannot access to directory or volume label" ,
    "Handle not opened" ,
    "Memory block broken" ,
    "No enough memory to execute" ,
    "Invalid memory block pointer" ,
    "Invalid environment" ,
    "Execute file format error" ,
    "Invalid access mode in open" ,
    "Invalid filename" ,
    "Invalid parameter" ,
    "Invalid drive" ,
    "Cannot remove current directory" ,
    "Device cannot ioctrl" ,
    "File not found more" ,
    "Write protected" ,
    "Directory already exists" ,
    "Directory not empty" ,
    "File already exists" ,
    "Disk full" ,
    "Directory full" ,
    "Cannot seek" ,
    "Already supervisor mode" ,
    "Undefined" ,
    "Process already exists" ,
    "Message not received" ,
    "Invalid process number" ,
    "Undefined" ,
    "Share file over" ,
    "File locking error" ,
};
