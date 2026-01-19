#pragma once

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

void serial_init(void);

int serial_getc(void);

void serial_end(void);

#endif