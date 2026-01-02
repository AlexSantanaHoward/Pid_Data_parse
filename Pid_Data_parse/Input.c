#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "output.h"
#include "file.h"

#pragma warning(disable : 4996)

static int str_to_hex(char* str)
{
    int x;

    if (sscanf(str, "%x", &x) != EOF)
    {
        return x;
    }
    else
    {
        return 0;
    }
}

static void clear_str(char* str, int len)
{
    for (int i = 0; i <= len; i++)
    {
        str[i] = '\0';
    }
}

static void clear_buff(uint8_t* buff, int len)
{
    for (int i = 0; i <= len; i++)
    {
        buff[i] = 0;
    }
}

void pulseview_build_message(char c)
{
    #define Hunting		0
    #define In_Frame	1
    #define C0_Escape	2

    static int  p;
    static int state = 0;

    uint8_t crnt_byte = 0;

    static uint8_t message_buffer[60] = { 0 };
    static int message_index;

    static char Rx_Buff[250];

    char crnt_byte_s[2] = { 0 };

    Rx_Buff[p] = c;

    if (c == '\n')
    {
        // Line will be longer if it contains Start/Stop byte
        if (p <= 35)
        {
            // Extract byte from string
            strncpy(crnt_byte_s, Rx_Buff + (p - 2), 2);
            crnt_byte = str_to_hex(crnt_byte_s);

            switch (state)
            {

            case(Hunting):

                if (crnt_byte == 0xc2 || crnt_byte == 0xc3)
                {
                    state = In_Frame;
                    message_index = 0;
                }

                break;

            case(In_Frame):

                // EOF
                if (crnt_byte == 0xc1)
                {
                    // Execute message send 
                    output_message(message_buffer);

                    clear_str(Rx_Buff, 250);
                    clear_buff(message_buffer, 60);
                    message_index = 0;
                    state = Hunting;
                }
                // C0 Escape
                else if (crnt_byte == 0xc0)
                {
                    state = C0_Escape;
                }
                // Data
                else
                {
                    message_buffer[message_index] = crnt_byte;
                    message_index++;
                }

                break;

            case(C0_Escape):

                message_buffer[message_index] = (crnt_byte | 0xc0);
                message_index++;
                state = In_Frame;

                break;
            }
        }
        else
        {
            p = 0;
            clear_str(Rx_Buff, 250);
        }
    }
    else
    {
        p++;
    }
}

