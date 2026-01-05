#pragma once

#ifndef INC_FILE_H_
#define INC_FILE_H_

FILE* input_p(void);

FILE* output_p(void);

//FILE* input_file_init();

void input_file_init();

void output_file_init();

void input_close(void);

void output_close(void);

void open_files(void);

void close_files(void);

#endif