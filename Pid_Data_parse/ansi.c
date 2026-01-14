#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <windows.h>

//#include "output.h"
//#include "arg_handle.h"
//#include "can_ids.h"
//#include "file.h"

// https://learn.microsoft.com/en-gb/windows/console/clearing-the-screen
// https://learn.microsoft.com/en-gb/windows/console/console-virtual-terminal-sequences
// https://notes.burke.libbey.me/ansi-escape-codes/


#pragma warning(disable : 4996)

static HANDLE hStdOut;
static DWORD originalMode;


int terminal_init(void)
{
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Fetch existing console mode so we correctly add a flag and not turn off others
    DWORD mode = 0;
    if (!GetConsoleMode(hStdOut, &mode))
    {
        //return ::GetLastError();
        return -1;
    }

    // Hold original mode to restore on exit to be cooperative with other command-line apps.
    originalMode = mode;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    // Try to set the mode.
    if (!SetConsoleMode(hStdOut, mode))
    {
        //return ::GetLastError();
        return -1;
    }

    return 0;

}

void terminal_fin(void)
{
    // Restore the mode on the way out to be nice to other command-line applications.
    SetConsoleMode(hStdOut, originalMode);
}

void ansi_reset(void)
{
    printf("\33[0m");
}

void ansi_bold(void)
{
    printf("\33[1m");
}

void ansi_invert(void)
{
    printf("\33[7m");
}

void ansi_cursor_y_x(int y, int x)
{
    printf("\33[");
    printf("%i;%iH", y, x);
}

void ansi_verticle_line()
{
    printf("\33(0x\33(B");
}

char* ansi_verticle_line_str()
{
    return "\33(0x\33(B";
}

void draw_box(int y, int x, int rows, int column)
{
    ansi_cursor_y_x(y, x);

    for (int i = 1; i <= rows; i++)
    {
        //printf("%*c", column, ' ');
        ansi_cursor_y_x((y + i), x);
        printf("%*c", column, ' ');
        
        
    }
}

void ansi_clear(void)
{
    printf("\33[2J");
}

