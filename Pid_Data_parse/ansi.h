#pragma once

#ifndef INC_ANSI_H_
#define INC_ANSI_H_

int terminal_init(void);

void terminal_fin(void);

void ansi_reset(void);

void ansi_bold(void);

void ansi_invert(void);

void ansi_cursor_y_x(int y, int x);

void ansi_clear(void);

#endif