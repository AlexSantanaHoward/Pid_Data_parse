#pragma once

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

void print_table_header(void);

static char* concatenate_hex(uint8_t hex);

char* format_message_data(uint8_t* data);

void output_message(uint8_t* data);

void filter_add(int type, char* id);

static int str_to_hex(char* str);

int filter_check(uint8_t* data);

#endif