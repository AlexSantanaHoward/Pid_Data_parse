#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arg_handle.h"

#pragma warning(disable : 4996)

char* input_option = "-i";
char* pulse_option = "-pv";
//TODO: have option for diff out & no console output

char* out_file_end = "_clean.txt";

static char input_file[250];
static char output_file[250];

static int pv_option;


static int string_find_last_char(char* str, char c)
{
    int last_char_p = 0;

    for(int i = 0; i <= strlen(str); i++)
    {
        if(str[i] == c)
        {
            last_char_p = i;
        }
    }

    return last_char_p;
}


char* file_in(void)
{
    return input_file;
}

char* file_out(void)
{
    return output_file;
}

int pv_state(void)
{
    return pv_option;
}



void arg_handle(int argc, char* argv[])
{
    // Initialise pv_option as 0 and allow arg to overide.
    pv_option = 0;

    int last_period = 0;

    if(argc <= 1)
    {
        // print help message
        // TODO: print out args + example
        printf("Error");
        exit(0);
    }
    else
    {
        // Start at one to avoid application name
        for(int i = 1; i < argc; i++)
        {

            if(strcmp(input_option,argv[i]) == 0 && argc > i)
            {
                
                i++;

                // Set input file name
                snprintf(input_file, strlen(argv[i]) + 1, "%s", argv[i]);
                printf("\nInput file: %s\n", input_file);

                // Set output file name
                snprintf(output_file, strlen(argv[i]) + 1, "%s", argv[i]);

                // Modify output file name
                last_period = string_find_last_char(output_file, '.');
           
                for(int r = 0; r <= strlen(out_file_end); r++)
                {
                    output_file[r + last_period] = out_file_end[r];
                }              
            }
            else if(strcmp(pulse_option, argv[i]) == 0)
            {
                pv_option = 1;
            }
        }
    }
}