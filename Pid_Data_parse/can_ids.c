#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "can_ids.h"

#pragma warning(disable : 4996)


uint16_t id_value[] = { 0x5b03,			0x6703,			 0x6903,			0x7004,
                         0x2905,			0x7105,			 0x7205,			0x7505,
                         0x2106,			0x2906,			 0x2b06,			0x2d06,
                         0x5106,			0x5f06,			 0xd806,			0x6051,
                         0x01f0,			0x6c06,			 0x2705,			0x5505,
                         0x5306,			0x9105,			 0xB406 };

char id_name[23][15] = { " gw_motor     ", " GwAnt1       ", " GwAnt2       ", " bsg_kombi    ",
                         " mfa4         ", " bsg2         ", " gateway_kombi", " bsg3         ",
                         " kombi_k1     ", " mfa1         ", " mfa2         ", " mfa3         ",
                         " system_info1 ", " fzg_ident    ", "-Unknown 1    ", "-Unknown 2    ",
                         "-Unknown 3    ", "-Unknown 4    ", "-Unknown 5    ", "-Unknown 6    ",
                         "-Unknown 7    ", "-Unknown 8    ", "-Unknown 9    " };

/**Returns name of CAN ID.
* If no name is found, returns blank space.
* @param{ uint16_t } CAN ID.
* @returns { char* } CAN ID Name.
*/
char* check_id_name(uint16_t can_id)
{
    for (int i = 0, r = 0; i <= 22; i++)
    {
        // Name found
        if (can_id == id_value[i])
        {
            //printf("%s\n", id_name[i]);
            return id_name[i];
            r = 1;

        }

        // If no name is found, fill in the space
        if (i == 22 && r != 1)
        {
            return "              ";

        }
    }
}