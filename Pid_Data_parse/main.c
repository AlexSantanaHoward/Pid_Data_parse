#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "pid_id_data.h"
#include "arg_handle.h"

#pragma warning(disable : 4996)

char message[100];

char* msg = "Message";
char* clean_txt = "_clean.txt";

char Fout_name[20];

FILE* fw;

static void string_replace_char(char* str, char current_c, char new_c)
{
	for (int i = 0; i <= 1000; i++)
	{
		if (str[i] == current_c)
		{
			str[i] = new_c;
		}
		else if (str[i] == '\0')
		{
			break;
		}
	}
}

static int str_to_hex(char* str)
{
	int x;

	if (sscanf(str, "%x", &x) != EOF) 
	{
		return x;
	}
	else
	{
		return 0;
	}
}

static void clear_str(char* str, int len)
{
	for (int i = 0; i <= len; i++)
	{
		str[i] = '\0';
	}
}


static void build_message(char c)
{

	static int  p;

	static char message[250];

	char ack[]      = "c6";
	char len[3]     = { 0 };

    // Dont belive this is opid, more like CAN_ID
	char opid[5]    = { 0 };

	char data[1000] = { 0 };

	uint16_t opid_uint = 0;

	message[p] = c;

	if (c == '\n')
	{

		if (strstr(message, msg) != NULL)
		{

			message[p] = '\0';

			strncpy(len, message + 21, 2);

			strncpy(opid, message + 24, 5);
			opid[2] = opid[3];
			opid[3] = opid[4];
			opid[4] = '\0';

            // TODO: Should this be here..
			int data_len = str_to_hex(len);

			opid_uint = str_to_hex(opid);

			if (strncmp(len, ack, 2) != 0)
			{
                
                // Carries ordinary data
                if (data_len >= 2)
				{
					strncpy(data, message + 30, (((data_len * 2) + data_len) - 7));
					string_replace_char(data, ',', ' ');

					printf(     "  %02i | %s |", data_len - 2, opid);
					fprintf(fw, "  %02i | %s |", data_len - 2, opid);
				}
                // Weird scenario, suspect it carries shutdown message
				else if (data_len == 1)
				{
					strncpy(data, message + 30, 2);

					printf(		"  %02i | %s |", data_len, opid);
					fprintf(fw, "  %02i | %s |", data_len, opid);

				}
                // No Data
				else
				{
					printf(     "  %02i | %s |", data_len, opid);
					fprintf(fw, "  %02i | %s |", data_len, opid);
				}


                // CAN ID Name checker -----------------------------
				for (int i = 0, r = 0; i <= 22; i++)
				{

					if (opid_uint == id_value[i])
					{

						printf(     " %s", id_name[i]);
						fprintf(fw, " %s", id_name[i]);
						r = 1;
					}

					if (i == 22 && r != 1)
					{

						printf(     "               ");
						fprintf(fw, "               ");

					}
				}
                //-----------------------------------------------      

                printf(     "| ");
                fprintf(fw, "| ");

                int div_by_8 = (data_len - 2) % 8;
                
                for (int i = 0; i <= (data_len - 3); i++)
                {
                    // TODO: nake this better!!
                    if (i == 8 || i == 16 || i == 24 || i == 32 || i == 40 || i == 48 || i == 56)
                    //if (i % 8 == 0 && i != 1 && (data_len - 3) > i)
                    {
                        printf(     "|\n     |      |               | ");
                        fprintf(fw, "|\n     |      |               | ");
                    }

                    printf(    "%.*s ", 2, data + (i * 3));
                    fprintf(fw,"%.*s ", 2, data + (i * 3));

                }

                // Empty space filler.
                if (data_len == 0)
                {
                    printf(     "                        ");
                    fprintf(fw, "                        ");
                }
                else if (data_len == 1)
                {
                    printf(     "                        ");
                    fprintf(fw, "                        ");
                }
                else
                {
                    if (div_by_8 != 0)
                    {
                    //printf("div_by_8 = %i", div_by_8);
                    for (int i = 0; i <= (7 - div_by_8); i++)
                    {
                        printf(     "   ");
                        fprintf(fw, "   ");

                    }
                    }
                }
                //----------------------------

                //bap_parse();

                printf(     "|\n");
                fprintf(fw, "|\n");
                
			}
		}

		p = 0;
		clear_str(message, 250);
	}
	else
	{
		p++;
	}
}

static void pulseview_build_message(char c)
{
	static int  p;
	static int state = 0;

	uint8_t crnt_byte = 0;

	static char Rx_Buff[250];
	static char Tx_Buff[250];
	static char message[1000];

	char crnt_byte_s[4] = {0};

	#define Hunting		0
	#define In_Frame	1
	#define C0_Escape	2

	Rx_Buff[p] = c;

	if (c == '\n') 
	{
		// Line will be longer if it contains Start/Stop byte
		if (p <= 35) 
		{
			// Extract byte from string
			strncpy(crnt_byte_s, Rx_Buff + (p - 2), 2);
			crnt_byte = str_to_hex(crnt_byte_s);
			
			switch (state) 
			{

			case(Hunting):

				if (crnt_byte == 0xc2 || crnt_byte == 0xc3)
				{
					state = In_Frame;

					crnt_byte_s[2] = ',';
					//crnt_byte_s[3] = '\0';

					strncpy(message, crnt_byte_s, 3);
				}

				break;

			case(In_Frame):

				if (crnt_byte == 0xc1) 
				{
					crnt_byte_s[2] = '\0';
					strcat(message, crnt_byte_s);

					snprintf(Tx_Buff,(20 + strlen(message)),"Message arrived:  %s\n",message);

					for (int i = 0; i != 250; i++)
					{
						build_message(Tx_Buff[i]);

						if(Tx_Buff[i] == '\n')
						{
							break;
						}
					}
					
					clear_str(message, 250);
					clear_str(Rx_Buff, 250);
					clear_str(Tx_Buff, 250);

					state = Hunting;


					//state = End_Frame;
				}
				else if (crnt_byte == 0xc0)
				{
					state = C0_Escape;
				}
				else 
				{
					crnt_byte_s[2] = ',';
					//crnt_byte_s[3] = '\0';
					strcat(message, crnt_byte_s);
				}

				break;

			case(C0_Escape):

				crnt_byte_s[0] = 'c';

				crnt_byte_s[2] = ',';
				//crnt_byte_s[3] = '\0';
				strcat(message, crnt_byte_s);

				state = In_Frame;

				break;

			}
		}

		p = 0;
		clear_str(Rx_Buff, 250);
	}
	else 
	{
		p++;
	}

}


int main(int argc, char* argv[])
{

    arg_handle(argc, argv);

	// Read file
	FILE* fp;
	char value;
	fp = fopen(file_in(), "r");


	// Check if read file exsists.
	if (fp != NULL)
	{

        fw = fopen(file_out(), "w");

        // Check if write file has been opened
        if (fw == NULL)
        {
            printf("\n Error Opening %s : Unable to access \n", file_out());
            exit(0);
        }
        else
        {
            printf("Writing to: %s\n", file_out());
        }
        //----------------------------------------

		printf(     "\n Len |CANID |    CAN Name   |       BAP Message       |\n-----|------|---------------|-------------------------|\n");
		fprintf(fw, "\n Len |CANID |    CAN Name   |       BAP Message       |\n-----|------|---------------|-------------------------|\n");

		while (1)
		{

			value = fgetc(fp);

			if (value == EOF)
			{
				fclose(fp);
				fclose(fw);
				break;
			}
			else
			{
                if(pv_state())
                {
                    pulseview_build_message(value);
                }
                else
                {
				    build_message(value);
                }

			}
		}
	}
	else
	{
		printf("\n Error Opening %s : File not found \n", file_in());
		exit(0);
	}
}


