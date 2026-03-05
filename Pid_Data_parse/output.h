#pragma once

#ifndef INC_OUTPUT_H_
#define INC_OUTPUT_H_

#define CAN 1
#define FCT 2
#define LSG 3

void print_table_header(void);

char* format_message_data(uint8_t* data);

void output_message(uint8_t* data);

void filter_add(int type, char* id);

int filter_check(uint8_t* data);

#endif