#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#pragma warning(disable : 4996)

#include "arg_handle.h"

FILE *input;
FILE *output;

/* Returns pointer to input file.
* @returns input file pointer.
*/
FILE* input_p()
{
    return input;
}

/* Returns pointer to output file.
* @returns output file pointer.
*/
FILE* output_p()
{
    return output;
}

/*Initialises input file.
*/
void input_file_init()
{
    input = fopen(file_in(), "r");

    if (input == NULL)
    {
        printf("\n Error Opening %s : File not found \n", file_in());
        exit(0);
    }
    else
    {
        printf("\nInput file: %s\n", file_in());
    }
}

/**Initialises output file.
*/
void output_file_init()
{
    output = fopen(file_out(), "w");

    if (output == NULL)
    {
        printf("\n Error Opening % s : Unable to access \n", file_out());
        exit(0);
    }
    else
    {
        printf("Writing to: %s\n", file_out());
    }
}

/* Closes input file
*/
void input_close(void)
{
    fclose(input);
}

/* Closes output file
*/
void output_close(void)
{
    fclose(output);
}

void open_files(void)
{
    input_file_init();

    if(no_state())
    {
        output_file_init();
    }
}

void close_files(void)
{
    input_close();

    if (no_state())
    {
        output_close();
    }
}