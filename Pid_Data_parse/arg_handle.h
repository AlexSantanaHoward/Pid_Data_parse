#pragma once

#ifndef INC_ARG_HANDLE_H_
#define INC_ARG_HANDLE_H_


static int string_find_last_char(char* str, char c);

char* file_in(void);

char* file_out(void);

int pv_state(void);

void arg_handle(int argc, char* argv[]);


#endif