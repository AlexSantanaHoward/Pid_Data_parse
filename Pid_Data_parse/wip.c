#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "can_ids.h"

typedef enum
{
    rms_IDLE        = 0,
    rms_Text_build  = 1,
    rms_Text_end    = 2,

} RadioMessageState;

static RadioMessageState rms_state = {0};
static int iTextToRead;
static int iTextRead;

uint8_t sRInfo[128];

void wip_parse(uint16_t CAN_ID, uint8_t* data)
{

    int iShortTotalTimeOfTravelMin, iShortTimeOfTravelHours, iShortTimeOfTravelMin;
    int iLongTotalTimeOfTravelMin, iLongTimeOfTravelHours, iLongTimeOfTravelMin;



    switch(CAN_ID)
    {
    case (gw_motor):

        printf(" Byte 1 Unknown |");
        printf(" rpm = %04i |", ((256 * data[5]) + data[4]) / 4); //? This is wrong
        printf(" Eng T = %03i |", data[6] - 80); // This currently seems correct, need to verify with obd2

    break;

    case (gw_ant2):
        break;

    case (mfa1):
        
        printf(" Range: %03i     |",data[5]);
        printf(" Byte 1 Unk |");

        break;

    case (mfa2):

        iShortTotalTimeOfTravelMin = (data[7] + (data[8] * 256));
        iShortTimeOfTravelHours = iShortTotalTimeOfTravelMin / 60;
        iShortTimeOfTravelMin = iShortTotalTimeOfTravelMin - (iShortTimeOfTravelHours * 60);

        printf(" Sh MPG = %.1f  |", (float)(data[3] + 256) / 10 );

        //printf(" ignition  ON |"); //?
        printf(" Sh Dis = %04i  |", data[5]); // Short trip distance, this is correct

        printf(" Sh ToT = %02i:%02i |", iShortTimeOfTravelHours, iShortTimeOfTravelMin); // This is correct, verified with trip

        if (data[9] == 0xff) // No short average speed
        {
            printf(" Sh Speed = ---  |"); // Short trip distance, this is correct
        } else
        {
            printf(" Sh Speed = %03i  |", data[9]); // Short trip distance, this is correct
        }

        break;

    case (mfa3):

        iLongTotalTimeOfTravelMin = (data[7] + (data[8] * 256));
        iLongTimeOfTravelHours = iLongTotalTimeOfTravelMin / 60;
        iLongTimeOfTravelMin = iLongTotalTimeOfTravelMin - (iLongTimeOfTravelHours * 60);

        printf(" Ln MPG = %.1f  |", (float)(data[3] + 256) / 10);

        printf(" Ln Dis = %04i  |", data[5]); // Long trip distance, this is correct

        printf(" Ln ToT = %01i:%02i |", iLongTimeOfTravelHours, iLongTimeOfTravelMin); // This is correct, verified with trip

        if (data[9] == 0xff) // No Long average speed
        {
            printf(" Ln Speed = ---  |"); // Long trip distance, this is correct
        }
        else
        {
            printf(" Ln Speed = %03i  |", data[9]); // Long trip distance, this is correct
        }


        break;

    case (radio):

        switch(rms_state)
        {
            case(rms_IDLE):
            

                if (data[3] == 0xb0)  // Text transmission
                {
                    //TODO Deal with different message types i.e artist, song,station, station data

                    iTextToRead = data[4];
                    iTextRead = 0;
                    

                    if(data[4] >= 4)
                    {
                        sRInfo[0] = data[7];
                        sRInfo[1] = data[8];
                        sRInfo[2] = data[9];
                        sRInfo[3] = data[10];

                        iTextRead = 4;
                        iTextToRead = (iTextToRead - 4);
                    }
                    //TODO what if the text is only 3 chars?
                    rms_state = rms_Text_build;
                }

            break;

            case(rms_Text_build):

                if((data[3] & 0xf0) == 0xf0)
                {
                    if(iTextToRead >= 6)
                    {
                        for (int i = 0; i <= 6; i++, iTextRead++ )
                            {
                                sRInfo[iTextRead] = data[4 + i];
                                iTextToRead = iTextToRead -1;
                            }
                    }else
                    {
                        for (int i = 0, x = iTextToRead; i <= x; i++, iTextRead++ )
                            {
                                sRInfo[iTextRead] = data[4 + i];

                                if(iTextToRead == 0)
                                {
                                    break;
                                }

                                iTextToRead = iTextToRead -1;
                            }
                            iTextToRead = 0;
                    }

                    if (iTextToRead == 0)
                    {
                        //TODO should really be '<' but see if end char is consistent
                        for(int i = 0; i <= iTextRead;i++)
                        {
                            printf("%c",sRInfo[i]);
                        }
                        #if 0
                        for(int i = 0; i <= iTextRead;i++)
                        {
                            printf("%02x ",sRInfo[i]);
                        }
                        #endif

                        iTextRead = 0;
                        iTextToRead = 0;
                        rms_state = rms_IDLE;
                    } 
                }


            break;
        }

        
    break;

    case (bsg_kombi):

        if(data[3] & 0x20)
        {

        }

        printf(" ");

        if ((data[3] & 0x01) == 0x01 || (data[3] == 0x00)) // Passenger indicator
        {
            printf("\033[42m");   //Set background to green
        }
        printf("L\033[m ");

        if ((data[3] & 0x02) == 0x02) // Driver indicator
        {
            printf("\033[42m");   //Set background to green
        }
        printf("R\033[m ");

        if (data[3] & 0x20)         // reverse
        {
             printf("\033[41m");   //Set background to red
        }
        printf("Revrs\033[m ");

        if ((data[3] & 0x80) != 0x80)         // ignition/Engine
        {
            printf("\033[48;5;172m");   //Set background to orange
        }
        printf("Eng\033[m  |");

        if (data[4] & 0x10) // hood
        {
            printf("\033[41m");   //Set background to red
        }
        printf(" BN\033[m ");

        if ((data[4] & 0x60) == 0x60) // boot
        {
            printf("\033[41m");   //Set background to red
        }
        printf("BT\033[m ");

        if ((data[4] & 0x01) == 0x01) // Front drivers door
        {
            printf("\033[41m");   //Set background to red
        }
        printf("FD\033[m ");

        if ((data[4] & 0x02) == 0x02) // front passenger door
        {
            printf("\033[41m");   //Set background to red
        }
        printf("FP\033[m ");

        if ((data[4] & 0x04) == 0x04) // rear passenger door
        {
            printf("\033[41m");   //Set background to red
        }
        printf("RP\033[m ");

        if ((data[4] & 0x08) == 0x08) // rear driver door
        {
            printf("\033[41m");   //Set background to red
        }
        printf("RD\033[m ");

        if ((data[8] & 0x08) == 0x08)
        {
            printf("\033[41m");   //Set background to red
        }
        printf("Lights\033[m ");

        if ((data[8] & 0x10) == 0x10)
        {
            printf("\033[41m");   //Set background to red
        }
        printf("Fog\033[m ");        


        break;
        
        case(kombi_k1):

        printf(" temp = %03i.%01i",(data[6] - 6) / 2,((data[6] - 6) % 2));

        break;
    }

}

/*
char* sFuelConsumption(uint8_t data)
{
    char* cFuelConsumption[5];
    sprintf_s(cFuelConsumption,5,"%.2f", (float)((data + 256) / 10));
    return cFuelConsumption;
}
*/
   