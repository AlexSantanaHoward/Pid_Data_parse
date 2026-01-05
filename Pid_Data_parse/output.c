#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "output.h"
#include "arg_handle.h"
#include "can_ids.h"
#include "file.h"


#pragma warning(disable : 4996)


void print_table_header(void)
{
/*
    FILE *out;
    out = output_p();
    */
    if (nc_state())
    {
        if (bap_state())
        {
            printf("\n___________________________________________________________________________________________________________________________|\n");
            printf(" Msg | CAN  |      CAN      |           BAP           | Lng | Frame | in |    | head | ls | fc | op |        Payload       |\n");
            printf(" Len |  ID  |     Name      |         Message         | frm | type  | dx | ln | -er  | ID | ID | cd |                      |\n");
            printf("-----|------|---------------|-------------------------|-----|-------|----|----|------|----|----|----|----------------------|\n");
        }
        else
        {
            printf("\n_______________________________________________________\n");
            printf(" Msg | CAN  |      CAN      |           BAP           |\n");
            printf(" Len |  ID  |     Name      |         Message         |\n");
            printf("-----|------|---------------|-------------------------|\n");
        }
    }
    if (no_state())
    {
        if (bap_state())
        {
            fprintf(output_p(), "\n___________________________________________________________________________________________________________________________|\n");
            fprintf(output_p(), " Msg | CAN  |      CAN      |           BAP           | Lng | Frame | in |    | head | ls | fc | op |        Payload       |\n");
            fprintf(output_p(), " Len |  ID  |     Name      |         Message         | frm | type  | dx | ln | -er  | ID | ID | cd |                      |\n");
            fprintf(output_p(), "-----|------|---------------|-------------------------|-----|-------|----|----|------|----|----|----|----------------------|\n");
        }
        else
        {
            fprintf(output_p(), "\n_______________________________________________________\n");
            fprintf(output_p(), " Msg | CAN  |      CAN      |           BAP           |\n");
            fprintf(output_p(), " Len |  ID  |     Name      |         Message         |\n");
            fprintf(output_p(), "-----|------|---------------|-------------------------|\n");
        }
    }

}

static char *concatenate_hex(uint8_t hex)
{
    static char string[4] = {0};

    snprintf(string, 4, "%02x ", hex);

    return string;

}

char* format_message_data(uint8_t* data)
{
    uint8_t msg_length = 0;
    static char return_string[500] = {0};

    int div_by_8 = 0;

    //TODO implement clear string
    for (int i = 0; i <= 499; i++)
    {
        return_string[i] = 0;
    }

    // Deal with pesky non BAP format i.e less than 2.
    if ((data[0] - 2) >= 2)
    {
        msg_length = (data[0] - 2);
    }
    else if (data[0] == 1)
    {
        msg_length = 1;
    }
    else
    {
        return "                        |";
    }

    // Work out the remainder
    if(msg_length == 8)
    {
        div_by_8 = 0;
    }
    else
    {
    div_by_8 = 8 - (msg_length % 8);
    }

    for (int i = 1; i <= (msg_length + div_by_8); i++)
    {
        if (i <= msg_length)
        {
            strcat(return_string, concatenate_hex(data[i + 2]));
        }
        else
        {
            strcat(return_string, "   ");
        }

        // Long payload handler .. adds new line
        if (i % 8 == 0 && i != 1 && msg_length > i)
        {
            strcat(return_string, "|\n     |      |               | ");
        }
    }

    strcat(return_string, "|");
    
    return return_string;
}


void output_message(uint8_t* data)
{

    uint16_t CAN_ID = 0;
    uint16_t msg_length = 0;

    // Deal with pesky non BAP format i.e less than 2.
    if (data[0] >= 2)
    {
        msg_length = (data[0] - 2);
    }
    else
    {
        msg_length = data[0];
    }

    CAN_ID = (data[1] << 8u) | data[2];

    if(nc_state())
    {
        // Print data length 
        // Should this be an int
        printf("  %02x |", msg_length);

        // Print CAN ID
        printf(" %04x |", CAN_ID);

        // Print CAN ID Name
        printf(" %s| ", check_id_name(CAN_ID));

        // Print Message
        printf("%s", format_message_data(data));

        if(bap_state())
        {
            bap_parse(data, msg_length);
        }

        printf("\n");

    }
    if(no_state())
    {
        // Print data length 
        // Should this be an int
        fprintf(output_p(), "  %02x |", msg_length);

        // Print CAN ID
        fprintf(output_p(), " %04x |", CAN_ID);

        // Print CAN ID Name
        fprintf(output_p(), " %s| ", check_id_name(CAN_ID));

        // Print Message
        fprintf(output_p(), "%s", format_message_data(data));
        fprintf(output_p(), "\n");
    }

}