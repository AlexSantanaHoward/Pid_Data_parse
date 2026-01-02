#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "bap.h"

#pragma warning(disable : 4996)




static uint16_t bap_header_assemble(uint8_t* buff, int header_point)
{
    uint16_t header = 0;

    header = (buff[header_point] << 8u) & 0xFF00;
    header = (header | (uint16_t) buff[header_point + 1]);

    return header;
}


static void string_data_to_array(uint8_t *buff, char* data, int len)
{
    int data_string_len = strlen(data);
    char string_buff[2] = {0};

    for(int i = 0, p = 0, e = 0; i <= data_string_len; i++)
    {
        if(data[i] >= '0')
        {
            string_buff[e] = data[i];
            e++;
        }

        if(e == 2)
        {
            buff[p] = (int)strtol(string_buff, NULL, 16);
            p++;

            string_buff[0] = 0;
            string_buff[1] = 0;

            e = 0;
        }


        if (p == (len - 2))
        {
            break;
        }

    }
}


void bap_parse(char* data, int len)
{

    char return_str[100] = { 0 };

    int long_frame = 0;
    int op_code = 0;
    int logical_device_id = 0;
    int function_id = 0;
    int payload_pointer = 0;

    uint8_t data_buff[1000] = { 0 };

    uint16_t bap_header = 0;
    uint8_t long_frame_data_length = 0;
    uint8_t long_frame_index = 0;

    // make sure there is a payload to parse
    if( len >= 2)
    {
        string_data_to_array(data_buff,data, len);

        #if 0
        // Check if it is a initial long BAP message
        if (data_buff[0] == 0x80)
        {
            long_frame = 1;

            long_frame_data_length = data_buff[1];

            bap_header = bap_header_assemble(data_buff, 2);
            
            function_id         = (bap_header >> 0)  & 0x3F;
            logical_device_id   = (bap_header >> 6)  & 0x3F;
            op_code             = (bap_header >> 12) & 0x7;




            printf(" 1 ");
            printf("|header = %04x ", bap_header);
            printf("|lsg_id = %03x ", logical_device_id);
            printf("|Fct_ID = %03x ", function_id);
            printf("|Op_Code = %02x ", op_code);
            printf("|Payload = ");

            for (int i = 0; i <= 5; i++)
            {
                if (i <= ((len - 2) - 5))
                {
                    printf("%02x ", data_buff[i + 4]);
                }
                else
                {
                    printf("   ");
                }
            }
            //printf("\n");


        }
        else
        {
            bap_header = bap_header_assemble(data_buff, 0);

            function_id         = (bap_header >> 0)  & 0x3F;
            logical_device_id   = (bap_header >> 6)  & 0x3F;
            op_code             = (bap_header >> 12) & 0x7;

            printf(" 0 ");
            printf("|header = %04x ", bap_header);
            printf("|lsg_id = %03x ", logical_device_id);
            printf("|Fct_ID = %03x ", function_id);
            printf("|Op_Code = %02x ", op_code);
            printf("|Payload = ");

            for(int i = 0; i <= 5; i++)
            {
                if( i <= ((len - 2) - 3) )
                {
                    printf("%02x ", data_buff[i+2]);
                }
                else
                {
                    printf("   ");
                }
            }
            //printf("\n");

        }
        #else

        // Long bap message
        if ((data_buff[0] & 0xC0) == 0x80)
        {
            bap_header = bap_header_assemble(data_buff, 2);

            long_frame_index = data_buff[0] & 0x3F;
            long_frame_data_length = data_buff[1];

            function_id         = (bap_header >> 0) & 0x3F;
            logical_device_id   = (bap_header >> 6) & 0x3F;
            op_code             = (bap_header >> 12) & 0x7;

            //                    long   start   index  length  header lsID  fct_ID  op_cd
            //strncpy(return_str, " Yes | Start | %02x | %04x | %02x | %02x | %02x | ", long_frame_index, bap_header, logical_device_id, function_id, op_code, 42);
            //snprintf(return_str, 42, " Yes | Start | %02x | %04x | %02x | %02x | %02x | ", long_frame_index, bap_header, logical_device_id, function_id, op_code);
            printf(" Yes | Start | %02x | %02x | %04x | %02x | %02x | %02x | ", long_frame_index, long_frame_data_length, bap_header, logical_device_id, function_id, op_code);

            payload_pointer = 4;
            
        
        }
        // Long bap continuation message
        else if((data_buff[0] & 0xC0) == 0xC0)
        {
           long_frame_index = data_buff[0] & 0x3F;

           //strncpy(return_str, " Yes | Cont | %02x |  --  |  --  |  --  |  --  | ", long_frame_index, bap_header, logical_device_id, function_id, op_code, 37);
           // snprintf(return_str, 47, " Yes | Cont | %02x |  --  |  --  |  --  |  --  | ", long_frame_index);
           printf(" Yes | Contn | %02x | -- | ---- | -- | -- | -- | ", long_frame_index);

           payload_pointer = 1;
        }
        // Standard message
        else
        {
            bap_header = bap_header_assemble(data_buff, 0);

            function_id         = (bap_header >> 0) & 0x3F;
            logical_device_id   = (bap_header >> 6) & 0x3F;
            op_code             = (bap_header >> 12) & 0x7;

            //                    long  start  index header lsID  fct_ID  op_cd
           // strncpy(return_str, "  No | ----- | -- | %04x | %02x | %02x | %02x | ", long_frame_index, bap_header, logical_device_id, function_id, op_code, 42);
            //snprintf(return_str, 42, "  No | ----- | -- | %04x | %02x | %02x | %02x | ", bap_header, logical_device_id, function_id, op_code);
            printf("  No | ----- | -- | -- | %04x | %02x | %02x | %02x | ", bap_header, logical_device_id, function_id, op_code);

            payload_pointer = 2;
        
        }

        // Payload

        for (int i = 0; i <= 6; i++)
        {
            if (i <= ((len - 3) - payload_pointer))
            {
                //strcat(return_str, data_buff[i]);
                printf("%02x ", data_buff[i + payload_pointer]);
            }
            else
            {
                strcat(return_str, "   ");
                printf("   ");
            }
        }
        printf("|");
        //strcat(return_str, "\n");

        //return "\n";

        #endif
    }


}