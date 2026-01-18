#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "arg_handle.h"
#include "output.h"

#pragma warning(disable : 4996)

char* input_option      = "-i";
char* output_file_name  = "-o";
char* com_input_option  = "-com";
char* pulse_option      = "-pv";
char* console_option    = "-nc";
char* output_option     = "-no";
char* bap_parse_option  = "-bap";
char* wip_parse_option  = "-wip";
char* dif_parse_option  = "-dif";
char* can_filter_option = "-can";
char* fct_filter_option = "-fct";
char* lsg_filter_option = "-lsg";

char* out_file_end = "_clean.txt";

static char input_file[250];
static char output_file[250];

static int output_defined;
static int com_option;
static int pv_option;
static int nc_option;
static int no_option;
static int bap_option;
static int wip_option;
static int dif_option;



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

int nc_state(void)
{
    return nc_option;
}

int no_state(void)
{
    return no_option;
}

int bap_state(void)
{
    return bap_option;
}

int wip_state(void)
{
    return wip_option;
}

int dif_state(void)
{
    return dif_option;
}

int com_state(void)
{
    return com_option;
}



void arg_handle(int argc, char* argv[])
{
    // Initialise pv_option as 0 and allow arg to overide.
    pv_option = 0;

    // Intialize bap_option as 0 and allow arg to overide.
    bap_option = 0;

    // Intialize bap_option as 0 and allow arg to overide.
    wip_option = 0;

    // Intialize bap_option as 0 and allow arg to overide.
    dif_option = 0;

    // Intialize nc_option as 1 and allow arg to overide.
    nc_option = 1;

    // Intialize no_option as 1 and allow arg to overide.
    no_option = 1;


    int last_period = 0;

    if(argc <= 1)
    {
        // print help message
        printf("\n\33[31m"); // Set text to Red
        printf(" Error insufficient arguments\x1b[m\n\n");

        printf("\x1b[38;5;172m"); // Set Text to orange
        printf(" -i     <Input_File.txt>\n");
        printf(" -o     <Output_File.txt>\n");
        printf(" -com = assign com port input\n");
        printf(" -nc  = No console output\n");
        printf(" -nc  = No output file\n");
        printf(" -bap = Enable BAP parse\n");
        printf(" -wip = Enable WIP parse\n");
        printf(" -dif = Enable differential output\n");
        printf(" -can = Filter by CAN ID\n");
        printf(" -fct = Filter by Function ID\n");
        printf(" -lsg = Filter by logical device ID\n\n");

        printf("\x1b[m"); // Reset terminal colours
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

                if(output_defined == 0)
                {
                    // Set output file name
                    snprintf(output_file, strlen(argv[i]) + 1, "%s", argv[i]);

                    // Modify output file name
                    last_period = string_find_last_char(output_file, '.');
           
                    for(int r = 0; r <= strlen(out_file_end); r++)
                    {
                        output_file[r + last_period] = out_file_end[r];
                    }     
                }
            }
            else if (strcmp(output_file_name, argv[i]) == 0 && argc > i)
            {
                // Make sure that there is an argument after
                if (i < argc)
                {
                    snprintf(output_file, strlen(argv[i + 1]) + 1, "%s", argv[i + 1]);
                    output_defined = 1;
                }
                else
                {
                    printf("\n\33[31m\nError! insufficient arguments\x1b[m\n");
                    exit(0);
                }
            }
            else if (strcmp(com_input_option, argv[i]) == 0)
            {
                // Make sure that there is an argument after
                if (i < argc)
                {
                    if(sscanf(argv[i + 1], "%i", &com_option) == EOF)
                    {
                        printf("\n\33[31m\nError! incorrect COM arguments <%s>\x1b[m\n", argv[i + 1]);
                        exit(0);
                    }
                }
                else
                {
                    printf("\n\33[31m\nError! incorrect COM arguments\x1b[m\n");
                    exit(0);
                }
            }

            else if(strcmp(pulse_option, argv[i]) == 0)
            {
                pv_option = 1;
            }
            else if (strcmp(console_option, argv[i]) == 0)
            {
                nc_option = 0;
            }
            else if (strcmp(output_option, argv[i]) == 0)
            {
                no_option = 0;
            }
            else if (strcmp(bap_parse_option, argv[i]) == 0)
            {
                bap_option = 1;
            }
            else if (strcmp(wip_parse_option, argv[i]) == 0)
            {
                wip_option = 1;
            }
            else if (strcmp(dif_parse_option, argv[i]) == 0)
            {
                dif_option = 1;
            }
            else if (strcmp(can_filter_option, argv[i]) == 0)
            {
                // Make sure that there is an argument after
                if(i < argc)
                {
                    // Cycle through the args after -can to find IDs
                    for (int r = 1; (r + i) < argc ; r++)
                    {
                        // If next arg is argument'-xx' break
                        if (argv[i + r][0] != '-')
                        {                            
                            filter_add(CAN, argv[i + r]);
                        }
                        else
                        {                           
                            break;
                        }
                    }
                }
                else
                {
                    printf("\n\33[31m\nError! insufficient CAN filter arguments\x1b[m\n");
                    exit(0);
                }
            }
            else if (strcmp(fct_filter_option, argv[i]) == 0)
            {
                // Make sure that there is an argument after
                if (i < argc)
                {
                    // Cycle through the args after -can to find IDs
                    for (int r = 1; (r + i) < argc; r++)
                    {
                        // If next arg is argument'-xx' break
                        if (argv[i + r][0] != '-')
                        {
                            filter_add(FCT, argv[i + r]);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    printf("\n\33[31m\nError! insufficient Function filter arguments\x1b[m\n");
                    exit(0);
                }
            }
            else if (strcmp(lsg_filter_option, argv[i]) == 0)
            {
                // Make sure that there is an argument after
                if (i < argc)
                {
                    // Cycle through the args after -can to find IDs
                    for (int r = 1; (r + i) < argc; r++)
                    {
                        // If next arg is argument'-xx' break
                        if (argv[i + r][0] != '-')
                        {
                            filter_add(LSG, argv[i + r]);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    printf("\n\33[31m\nError! insufficient Logical device filter arguments\x1b[m\n");
                    exit(0);
                }
            }
        }
    }
}