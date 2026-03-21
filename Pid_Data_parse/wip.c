#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "can_ids.h"
/*
typedef enum
{
    rms_IDLE            = 0,
    rms_Desc_build      = 1,
    rms_Station_build   = 2,
    rms_Unk_build       = 3

} RadioMessageState;

static RadioMessageState rms_state = {0};

static int iTextToRead;
static int iTextRead;

static int iStationTextToRead;
static int iStationTextRead;

static int iUnkToRead;
static int iUnkRead;

static int iMaxTextRead;
static int iMaxStationRead;
static int iMaxUnkRead;

uint8_t sRInfo[64];
uint8_t sRStation[64];
uint8_t uRUnk[64];
*/

void clear_buffer(uint8_t* data, int iLen)
{
    for(int i = 0; i <= iLen; i++)
    {
        data[i] = 0;
    }
}

void radio_bap(uint8_t* data)
{
    // Data 0 = total length, data 1 + 2 is can message ID, Data 3 = bap type, Data 4 = Payload length

    static int iTextToRead;
    static int iTextRead;

    static int iStationTextToRead;
    static int iStationTextRead;

    static int iUnkToRead;
    static int iUnkRead;

    static int iMaxTextRead;
    static int iMaxStationRead;
    static int iMaxUnkRead;

    static uint8_t sRInfo[64];
    static uint8_t sRStation[64];
    static uint8_t uRUnk[64];

    typedef enum
    {
        rms_Unk_header          = 0x80,
        rms_Station_header      = 0xa0,
        rms_Description_header  = 0xb0,

        rms_Unk_Cont            = 0xc0,
        rms_Station_Cont        = 0xe0,
        rms_Description_Cont    = 0xf0,

    } RadioMessage;

    static RadioMessage radio_bap_type = {0};

    int iStation_x = 1;
    int iDescription_x = (iMaxStationRead + 4);
    int iUnk_x = (iMaxStationRead + iMaxTextRead + 8);

    uint8_t bap_value   = (data[3] & 0xf0);
    int iPayload_length = data[4];
    //uint8_t uPayload[]  = {0};



    switch(bap_value)
    {
        case (rms_Unk_header):
            
            // Reset all variables/ buffer as new header has priority.
            clear_buffer(uRUnk, 64);
            iUnkRead    = 0;
            iUnkToRead  = 0;

            iUnkToRead = iPayload_length;                   // Copy over payload length value

            if(iUnkToRead >= 4)                             // There can only be 3 bytes of data in the header
            {
                uRUnk[0] = data[7];
                uRUnk[1] = data[8];
                uRUnk[2] = data[9];
                uRUnk[3] = data[10];

                iUnkRead = 4;                               // Increment Unk read by 3
                iUnkToRead = (iUnkToRead - 4);              // Decrease Unk to read by 3
            }
            else
            {
                if (iUnkRead >= iMaxUnkRead)                // Update the Max Station read so that formatting can use it.
                {
                    iMaxUnkRead = iUnkRead;
                }

                printf("\033[%iC", iUnk_x);                 // Move cursor by Unk_x

                #if 0
                for(int i = 0; iUnkToRead != 0; i++)
                {
                    printf("%02x ",data[7 + i]);            // Just print the data buffer data directly

                    iUnkRead++;                             // Increment Unk read by +1
                    iUnkToRead = (iUnkToRead - 1);          // Decrease Unk to read by -1
                }
                #else
                // Test to print out BAP header as well incase any important info
                for(int i = 0; (iUnkToRead + 2) != 0; i++)
                {
                    printf("%02x ",data[5 + i]);            // Just print the data buffer data directly

                    iUnkRead++;                             // Increment Unk read by +1
                    iUnkToRead = (iUnkToRead - 1);          // Decrease Unk to read by -1
                }

                #endif
            }


        break;

        case (rms_Station_header):
            // Reset all variables/ buffer as new header has priority.
            clear_buffer(sRStation, 64);
            iStationTextRead    = 0;
            iStationTextToRead  = 0;

            //iStationTextToRead = (iPayload_length - 1);           // Copy over payload length value
            iStationTextToRead = data[7];                           // Copy over payload length value

            if(iStationTextToRead >= 4)                             // There can only be 3 bytes of data in the header
            {
                sRStation[0] = data[8];
                sRStation[1] = data[9];
                sRStation[2] = data[10];

                iStationTextRead = 3;                               // Increment Station text read by 3
                iStationTextToRead = (iStationTextToRead - 3);      // Decrease Station to text read by 3
            }
            else
            {

                if (iStationTextRead >= iMaxStationRead)            // Update the Max Station read so that formatting can use it.
                {
                    iMaxStationRead = iStationTextRead;
                }

                printf("\033[%iC", iStation_x);                     // Move cursor by Station_x

                for(int i = 0; iStationTextToRead != 0; i++)
                {
                    printf("%c",data[8 + i]);                       // Just print the data buffer data directly

                    iStationTextRead++;                             // Increment Station text read by +1
                    iStationTextToRead = (iStationTextToRead - 1);  // Decrease Station to text read by -1
                }
            }

        break;

        case (rms_Description_header):

            // Reset all variables/ buffer as new header has priority.
            clear_buffer(sRInfo, 64);
            iTextRead    = 0;
            iTextToRead  = 0;

            //iStationTextToRead = (iPayload_length - 1);           // Copy over payload length value
            iTextToRead = data[7];                                  // Copy over payload length value

            if(iTextToRead >= 4)                                    // There can only be 3 bytes of data in the header
            {
                sRInfo[0] = data[8];
                sRInfo[1] = data[9];
                sRInfo[2] = data[10];

                iTextRead = 3;                                      // Increment Station text read by 3
                iTextToRead = (iTextToRead - 3);                    // Decrease Station to text read by 3
            }
            else
            {

                if (iTextRead >= iMaxTextRead)                      // Update the Max Text read so that formatting can use it.
                {
                    iMaxTextRead = iTextRead;
                }

                printf("\033[%iC", iDescription_x);                 // Move cursor by Station_x

                for(int i = 0; iTextToRead != 0; i++)
                {
                    printf("%c",data[8 + i]);                       // Just print the data buffer data directly

                    iTextRead++;                                    // Increment text read by +1
                    iTextToRead = (iTextToRead - 1);                // Decrease Text to text read by -1
                }
            }

        break;

        case (rms_Unk_Cont):

            if(iUnkToRead > 7)                              // If there is data after this payload, just add to the buffer
            {
                for (int i = 0; i <= 6; i++, iUnkRead++ )
                {
                    uRUnk[iUnkRead] = data[4 + i];
                    iUnkToRead = iUnkToRead -1;
                }
            }
            else if(iUnkToRead <= 7 && iUnkToRead != 0)  // This should be the last payload
            {
                for (int i = 0; i <= iUnkToRead; i++, iUnkRead++ )
                {
                    uRUnk[iUnkRead] = data[4 + i];
                }

                if (iUnkRead >= iMaxUnkRead)
                {
                    iMaxUnkRead = iUnkRead;
                }

                printf("\033[%iC", iUnk_x);                     // Move cursor by Station_x
                for (int i = 0; i < iMaxUnkRead; i++)
                {
                    if(i < (iUnkRead - 1 ))
                    {
                        printf("%02x ",uRUnk[i]);
                        uRUnk[i] = '\0';
                    }else
                    {
                        printf(" ");
                    }
                }
                iUnkToRead = 0;
            }
            else if(iUnkToRead == 0)
            {
                // Do nothing as header hasn't been received
            }

        break;

        case (rms_Station_Cont):

            if(iStationTextToRead > 7)                              // If there is data after this payload, just add to the buffer
            {
                for (int i = 0; i <= 6; i++, iStationTextRead++ )
                {
                    sRStation[iStationTextRead] = data[4 + i];
                    iStationTextToRead = iStationTextToRead -1;
                }
            }
            else if(iStationTextToRead <= 7 && iStationTextToRead != 0)  // This should be the last payload
            {
                for (int i = 0; i <= iStationTextToRead; i++, iStationTextRead++ )
                {
                    sRStation[iStationTextRead] = data[4 + i];
                }

                if (iStationTextRead >= iMaxStationRead)
                {
                    iMaxStationRead = iStationTextRead;
                }

                printf("\033[%iC", iStation_x);                     // Move cursor by Station_x
                for (int i = 0; i < iMaxStationRead; i++)
                {
                    if(i < (iStationTextRead - 1 ))
                    {
                        printf("%c",sRStation[i]);
                        sRStation[i] = '\0';
                    }else
                    {
                        printf(" ");
                    }
                }
                iStationTextToRead = 0;
            }
            else if(iStationTextToRead == 0)
            {
                // Do nothing as header hasn't been received
            }

        break;

        case (rms_Description_Cont):

            if(iTextToRead > 7)                              // If there is data after this payload, just add to the buffer
            {
                for (int i = 0; i <= 6; i++, iTextRead++ )
                {
                    sRInfo[iTextRead] = data[4 + i];
                    iTextToRead = iTextToRead -1;
                }
            }
            else if(iTextToRead <= 7 && iTextToRead != 0)  // This should be the last payload
            {
                for (int i = 0; i <= iTextToRead; i++, iTextRead++ )
                {
                    sRInfo[iTextRead] = data[4 + i];
                }

                if (iTextRead >= iMaxTextRead)
                {
                    iMaxTextRead = iTextRead;
                }

                printf("\033[%iC", iDescription_x);                     // Move cursor by Station_x
                for (int i = 0; i < iMaxTextRead; i++)
                {
                    if(i < (iTextRead - 1))
                    {
                        printf("%c",sRInfo[i]);
                        sRInfo[i] = '\0';
                    }else
                    {
                        printf(" ");
                    }
                }
                iStationTextToRead = 0;
            }
            else if(iStationTextToRead == 0)
            {
                // Do nothing as header hasn't been received
            }

        break;

    }

}


void wip_parse(uint16_t CAN_ID, uint8_t* data)
{

    int iShortTotalTimeOfTravelMin, iShortTimeOfTravelHours, iShortTimeOfTravelMin;
    int iLongTotalTimeOfTravelMin, iLongTimeOfTravelHours, iLongTimeOfTravelMin;



    switch(CAN_ID)
    {
    case (gw_motor):

        printf(" Byte 1 Unknown |");
        printf(" rpm = %04i |", ((256 * data[5]) + data[4]) / 4); //? This seems correct
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

        //printf(" Sh MPG = %.1f  |", (float)(data[3] + 256) / 10 );
        if((data[4] & 0x0f) != 0x0f)
        {
            //printf(" Sh MPG = %.1f  |", (float)((data[3] / 10) + ((data[4] & 0x0f) * 25.6) ) );
            printf(" Sh MPG = %.1f  |", (float)(data[3] + ( (data[4] & 0x0f ) * 256)) / 10);
        } else
        {
            printf(" Sh MPG = ----  |");
        }

        //printf(" ignition  ON |"); //?
        printf(" Sh Dis = %04i  |", data[5]); // Short trip distance, this is correct

        printf(" Sh ToT = %02i:%02i |", iShortTimeOfTravelHours, iShortTimeOfTravelMin); // This is correct, verified with trip

        if ((data[9] & 0xf0) == 0xf0) // No short average speed
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

        //printf(" Ln MPG = %.1f  |", (float)(data[3] + 256) / 10);
        if((data[4] & 0x0f) != 0x0f)
        {
            //printf(" Sh MPG = %.1f  |", (float)((data[3] / 10) + ((data[4] & 0x0f) * 25.6) ) );
            printf(" Ln MPG = %.1f  |", (float)(data[3] + ((data[4] & 0x0f) * 256)) / 10);
        } else
        {
            printf(" Ln MPG = ----  |");
        }

        printf(" Ln Dis = %04i  |", data[5]); // Long trip distance, this is correct

        printf(" Ln ToT = %02i:%02i |", iLongTimeOfTravelHours, iLongTimeOfTravelMin); // This is correct, verified with trip

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

        #if 0
        switch(rms_state)
        {
            case(rms_IDLE):
            

                if (data[3] == 0xb0)        // Description transmission
                {
                    //TODO Deal with different message types i.e artist, song,station, station data

                    iTextToRead = data[7];
                    iTextRead = 0;
                    

                    if(data[7] >= 3)
                    {
                        sRInfo[0] = data[8];
                        sRInfo[1] = data[9];
                        sRInfo[2] = data[10];

                        iTextRead = 3;
                        iTextToRead = (iTextToRead - 3);
                    }
                    //TODO what if the text is only 3 chars?
                    rms_state = rms_Desc_build;
                }
                else if(data[3] == 0xa0)    // Station Transmission
                {
                    iStationTextToRead = data[7];

                    if(data[7] >= 3)
                    {
                        sRStation[0] = data[8];
                        sRStation[1] = data[9];
                        sRStation[2] = data[10];

                        iStationTextRead = 3;
                        iStationTextToRead = (iStationTextToRead - 3);
                    }
                    rms_state = rms_Station_build;
                }
                else if(data[3] == 0x80)    // Unknown transmission
                {
                    iUnkToRead = data[4];

                    if(data[4] >= 4)
                    {
                        uRUnk[0] = data[7];
                        uRUnk[1] = data[8];
                        uRUnk[2] = data[9];
                        uRUnk[3] = data[10];

                        iUnkRead = 4;
                        iUnkToRead = (iUnkToRead - 3);

                        rms_state = rms_Unk_build;
                    }
                    else
                    {
                        printf("\033[%iC",(iMaxStationRead + iMaxTextRead + 6) ); 

                        #if 0
                        for(int i = 0; i < (data[4] + 2); i++)
                        {

                            printf("%02x ", data[5] + i);
                            
                        }
                        #else
                        printf("%02x %02x %02x %02x %02x %02x ",data[5],data[6],data[7],data[8],data[9],data[10]);
                        #endif

                    }
                    
                }
                
                

            break;

            case(rms_Desc_build):

                if((data[3] & 0xf0) == 0xf0)
                {
                    if(iTextToRead >= 7)
                    {
                        for (int i = 0; i <= 6; i++, iTextRead++ )
                            {
                                sRInfo[iTextRead] = data[4 + i];
                                iTextToRead = iTextToRead -1;
                            }
                    }
                    else
                    {
                        for (int i = 0, x = iTextToRead; i <= x; i++, iTextRead++ )
                            {
                                if(iTextToRead == 0)
                                {
                                    break;
                                }

                                sRInfo[iTextRead] = data[4 + i];

                                iTextToRead = iTextToRead -1;
                            }
                            iTextToRead = 0;
                    }

                    if (iTextToRead == 0)
                    {
                        //TODO should really be '<' but see if end char is consistent

                        if (iTextRead >= iMaxTextRead)
                        {
                            iMaxTextRead = iTextRead;
                        }

                        printf("\033[%iC",(iMaxStationRead + 4) );
                        //for(int i = 0; i <= iTextRead;i++)
                        for (int i = 0; i <= iMaxTextRead;i++)
                        {
                            if(i <= iTextRead)
                            {
                                printf("%c",sRInfo[i]);
                                sRInfo[i] = '\0';
                            }else
                            {
                                printf(" ");
                            }
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

            case(rms_Station_build):

            if((data[3] & 0xe0) == 0xe0)
                {
                    if(iStationTextToRead >= 7)
                    {
                        for (int i = 0; i <= 6; i++, iStationTextRead++ )
                            {
                                sRStation[iStationTextRead] = data[4 + i];
                                iStationTextToRead = iStationTextToRead -1;
                            }
                    }
                    else
                    {
                        for (int i = 0, x = iStationTextToRead; i <= x; i++, iStationTextRead++ )
                            {
                                sRStation[iStationTextRead] = data[4 + i];

                                if(iStationTextToRead == 0)
                                {
                                    break;
                                }

                                iStationTextToRead = iStationTextToRead -1;
                            }
                            iStationTextToRead = 0;
                    }

                    if (iStationTextToRead == 0)
                    {
                        //TODO should really be '<' but see if end char is consistent

                        if (iStationTextRead >= iMaxStationRead)
                        {
                            iMaxStationRead = iStationTextRead;
                        }

                        //for(int i = 0; i <= iTextRead;i++)
                        printf("\033[1C");
                        for (int i = 0; i <= iMaxStationRead;i++)
                        {
                            if(i <= iStationTextRead)
                            {
                                printf("%c",sRStation[i]);
                                //sRStation[i] = '\0';
                            }else
                            {
                                printf(" ");
                            }
                        }
                        #if 0
                        for(int i = 0; i <= iStationTextRead;i++)
                        {
                            printf("%02x ",sRStation[i]);
                        }
                        #endif

                        iStationTextRead = 0;
                        iStationTextToRead = 0;
                        rms_state = rms_IDLE;
                    } 
                }



            break;


            case(rms_Unk_build):

            if((data[3] & 0xc0) == 0xc0)
                {
                    if(iUnkToRead >= 7)
                    {
                        for (int i = 0; i <= 6; i++, iUnkRead++ )
                            {
                                uRUnk[iUnkRead] = data[4 + i];
                                iUnkToRead = iUnkToRead -1;
                            }
                    }
                    else
                    {
                        for (int i = 0, x = iUnkToRead; i <= x; i++, iUnkRead++ )
                            {
                                uRUnk[iUnkRead] = data[4 + i];

                                if(iUnkToRead == 0)
                                {
                                    break;
                                }

                                iUnkRead = iUnkToRead -1;
                            }
                            iUnkToRead = 0;
                    }

                    if (iUnkToRead == 0)
                    {
                        //TODO should really be '<' but see if end char is consistent

                        if (iUnkRead >= iMaxUnkRead)
                        {
                            iMaxUnkRead = iUnkRead;
                        }

                        //for(int i = 0; i <= iTextRead;i++)
                        //printf("\033[1C");
                        //printf("\033[%iC",(iMaxStationRead + iMaxTextRead + 4) );
                        for (int i = 0; i <= iMaxUnkRead;i++)
                        {
                            if(i <= iUnkRead)
                            {
                                //("%02x ",sRStation[i]);
                                //sRStation[i] = '\0';
                            }else
                            {
                                //printf(" ");
                            }
                        }


                        iUnkRead = 0;
                        iUnkToRead = 0;
                        rms_state = rms_IDLE;
                    } 
                }

            break;
        }
        #else

        radio_bap(data);

        #endif

        
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
   

