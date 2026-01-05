#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "arg_handle.h"
#include "bap.h"
#include "input.h"
#include "output.h"
#include "file.h"

#pragma warning(disable : 4996)

char message[100];

char* msg = "Message";
char* clean_txt = "_clean.txt";

char Fout_name[20];

FILE* fw;
/*
static void string_replace_char(char* str, char current_c, char new_c)
{
	for (int i = 0; i <= 1000; i++)
	{
		if (str[i] == current_c)
		{
			str[i] = new_c;
		}
		else if (str[i] == '\0')
		{
			break;
		}
	}
}

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

static void clear_buff(uint8_t *buff, int len)
{
    for (int i = 0; i <= len; i++)
    {
        buff[i] = 0;
    }
}
*/
int main(int argc, char* argv[])
{

    arg_handle(argc, argv);

	char value;

    open_files();

    // Output header
    print_table_header();


    while (1)
	{
        value = fgetc(input_p());


		if (value == EOF)
		{
			close_files();
            break;
		}
		else
		{
            if(pv_state())
            {
                pulseview_build_message(value);
            }
            else
            {
			    build_message(value);
            }

		}
	}
}


