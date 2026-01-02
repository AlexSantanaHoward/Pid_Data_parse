#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#pragma warning(disable : 4996)

#include "arg_handle.h"

FILE *input;
FILE *output;


FILE* input_p()
{
    return input;
}

FILE* output_p()
{
    return output;
}

FILE* input_file_init()
{
    input = fopen(file_in(), "r");

    if (input == NULL)
    {
        printf("\n Error Opening %s : File not found \n", file_in());
        exit(0);
    }
    return input;
}

/**Initialises output file and returns pointer.
* @returns{ FILE* } Output file pointer.
*/
FILE* output_file_init()
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

    return output;
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

