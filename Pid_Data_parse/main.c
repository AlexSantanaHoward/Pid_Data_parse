#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "arg_handle.h"
#include "bap.h"
#include "input.h"
#include "output.h"
#include "file.h"
#include "ansi.h"
#include "serial.h"

#pragma warning(disable : 4996)

char message[100];

char* msg = "Message";
char* clean_txt = "_clean.txt";

char Fout_name[20];

FILE* fw;


int main(int argc, char* argv[])
{
    if(terminal_init() != 0)
    {
        exit(0);
    }

    arg_handle(argc, argv);

	char value;

    open_files();
    
    if (com_port_status == 1)
    {
        serial_init();
    }

    // Output header
    print_table_header();


    while (1)
	{
        if(com_port_status == 1)
        {
            value = serial_getc();
        }
        else
        {
            value = fgetc(input_p());
        }


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

    terminal_fin();
    
    if (com_port_status == 1)
    {
        serial_end();
    }
}


