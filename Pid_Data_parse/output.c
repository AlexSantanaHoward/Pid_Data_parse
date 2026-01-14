#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "output.h"
#include "arg_handle.h"
#include "can_ids.h"
#include "file.h"
#include "ansi.h"


#pragma warning(disable : 4996)


static int can_filter_index = 0;
static uint16_t can_filter[11];

static int fct_filter_index = 0;
static uint8_t fct_filter[11];

static int lsg_filter_index = 0;
static uint8_t lsg_filter[11];

static int dif_index = 0;
static uint16_t dif_values[23];

void print_table_header(void)
{

    if (nc_state())
    {
        //ansi_bold();
        ansi_clear();
        ansi_invert();
        ansi_cursor_y_x(0,0);

        if (bap_state())
        {
            printf("\n___________________________________________________________________________________________________________________________|\n");
            printf(" Msg | CAN  |      CAN      |           BAP           | Lng | Frame | in |    | head | ls | fc | op |        Payload       |\n");
            printf(" Len |  ID  |     Name      |         Message         | frm | type  | dx | ln | -er  | ID | ID | cd |                      |\n");
            printf("-----|------|---------------|-------------------------|-----|-------|----|----|------|----|----|----|----------------------|\n");
        }
        else
        {
            #if 0

            printf("\n_______________________________________________________\n");
            printf(" Msg | CAN  |      CAN      |           BAP           |\n");
            printf(" Len |  ID  |     Name      |         Message         |\n");
            printf("-----|------|---------------|-------------------------|\n");

            #else
            //printf("\n Msg \33(0x\33(B CAN  \33(0x\33(B      CAN      \33(0x\33(B           BAP           \33(0x\33(B\n");
            //printf(  " Len \33(0x\33(B  ID  \33(0x\33(B     Name      \33(0x\33(B         Message         \33(0x\33(B\n");
            draw_box(0,0,2,55);

            ansi_cursor_y_x(0, 6);
            printf("\33(0"); // Set terminal display mode to graphic
            printf("x\33[B\33[Dx");

            ansi_cursor_y_x(0, 13);
            printf("x\33[B\33[Dx");
            ansi_cursor_y_x(0, 29);
            printf("x\33[B\33[Dx");
            ansi_cursor_y_x(0, 55);
            printf("x\33[B\33[Dx");

            printf("\33(B");
            ansi_cursor_y_x(0, 0);
;

            #endif
        }
        ansi_reset();
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

    int value_found = 0;

    if (filter_check(data) == 1)
    {
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

        if (nc_state())
        {

            if (dif_state())
            {

                for(int i = 0; i <= dif_index; i++)
                {
                    if(dif_values[i] == CAN_ID)
                    {
                        value_found = 1;
                        ansi_cursor_y_x(i + 2, 0);
                        break;
                    }
                    else
                    {
                        value_found = 0;
                    }
                }

                if(value_found == 0)
                {
                    dif_index++;
                    dif_values[dif_index] = CAN_ID;
                    ansi_cursor_y_x(dif_index + 2, 0);
                }

            }
            // Print data length 
            // Should this be an int
            printf("  %02x |", msg_length);

            // Print CAN ID
            printf(" %04x |", CAN_ID);

            // Print CAN ID Name
            printf(" %s| ", check_id_name(CAN_ID));

            // Print Message
            printf("%s", format_message_data(data));

            if (bap_state())
            {
                bap_parse(data, msg_length);
            }
            if (wip_state())
            {
                if (CAN_ID == 0x5b03)
                {
                    if (data[3] <= 0x08)
                    {
                        printf(" ignition  ON |");
                        printf(" rpm = %04i |", (256 * (data[4] + data[5])) / 20);
                        printf(" Temp1 = %i |", data[6] - 40);
                    }
                    else {
                        printf(" ignition OFF |");
                    }

                }
            }
            printf("\n");

            if (dif_state())
            {
                ansi_cursor_y_x(dif_index + 8,0);
            }

        }
        if (no_state())
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
}


void filter_add(int type, char* id)
{

    switch(type)
    {
    
        case(CAN):

            can_filter[can_filter_index] = str_to_hex(id);
            printf("added %04x to CAN ID filter\n", can_filter[can_filter_index]);
            can_filter_index = can_filter_index + 1;
            
        break;

        case(FCT):

            fct_filter[fct_filter_index] = (str_to_02_hex(id) & 0x3f);
            printf("added %02x to Function ID filter\n", fct_filter[fct_filter_index]);
            fct_filter_index = fct_filter_index + 1;

        break;

        case(LSG):

            lsg_filter[lsg_filter_index] = (str_to_02_hex(id) & 0x3F);
            printf("added %02x to Logical Device filter\n", lsg_filter[lsg_filter_index]);
            lsg_filter_index = lsg_filter_index + 1;

        break;
    
    }
}

// TODO:Should really incorporate str_to_hex and this function
static int str_to_hex(char* str)
{
    int x;

    if (sscanf(str, "%04x", &x) != EOF)
    {
        return x;
    }
    else
    {
        return 0;
    }
}

// TODO:Should really incorporate str_to_hex and this function
static int str_to_02_hex(char* str)
{
    int x;

    if (sscanf(str, "%02x", &x) != EOF)
    {
        return x;
    }
    else
    {
        return 0;
    }
}

int filter_check(uint8_t* data)
{
    // This will count up the amount of filter TRUEs.
    int verify = 0;

    uint16_t can_id = 0;
    uint8_t  fct_id = 0;
    uint8_t  lsg_id = 0;

    // CAN Check
    // If filter has been specified.
    if (can_filter_index != 0)
    {
        can_id = (data[1] << 8) | data[2];

        for (int i = 0; i <= can_filter_index; i++)
        {
            if (can_id == can_filter[i])
            {
                verify++;
                break;
            }
        }
    }
    else 
    {
        verify++;
    }

    // FCT Check
    // If filter has been specified.
    if (fct_filter_index != 0)
    {
        if ((data[3] & 0xC0) == 0x80)
        {
           fct_id = (data[6] >> 0) & 0x3F;
        }
        else if ((data[3] & 0xC0) == 0xC0)
        {
            // It must be a long BAP message, thus no function ID
            fct_id = 0xFF;
        }
        else
        {
            fct_id = (data[4] >> 0) & 0x3F;
        }

        for (int i = 0; i < fct_filter_index; i++)
        {
            if (fct_id == fct_filter[i])
            {
                verify++;
                break;
            }
        }
    }
    else
    {
        verify++;
    }

    // LSG Check
    // If filter has been specified.
    if (lsg_filter_index != 0)
    {
        if ((data[3] & 0xC0) == 0x80)
        {
            lsg_id = ((data[5] << 2 ) & 0x3C) | ((data[6] >> 6) & 0x03);
        }
        else if ((data[3] & 0xC0) == 0xC0)
        {
            // It must be a long BAP message, thus no logical device ID
            lsg_id = 0xFF;
        }
        else
        {
            lsg_id = ((data[3] << 2) & 0x3C) | ((data[4] >> 6) & 0x03);
        }

        for (int i = 0; i < lsg_filter_index; i++)
        {
            if (lsg_id == lsg_filter[i])
            {
                verify++;
                break;
            }
        }
    }
    else
    {
        verify++;
    }

    //printf("Verify = %i\n", verify);
    if (verify == 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}