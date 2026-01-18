#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "serial.h"

#include <windows.h>
#include <tchar.h>

//#include "output.h"
//#include "arg_handle.h"
//#include "can_ids.h"
//#include "file.h"

#pragma warning(disable : 4996)

// https://learn.microsoft.com/en-us/windows/win32/devio/configuring-a-communications-resource

//char* COM_PORT[] = "COM15";

static DCB dcb;
static HANDLE hCom;
static BOOL fSuccess;
//TCHAR *pcCommPort = TEXT("\\\\.\\COM15"); //  Most systems have a COM1 port   //works
//char* pcCommPort = TEXT("\\\\.\\COM15"); //  Most systems have a COM1 port    //works
char* pcCommPort = L"\\\\.\\COM15"; //  Most systems have a COM1 port            // Does not work


//static char* pcCommPort;
// *pcCommPort = TEXT("COM1");

DWORD dwBytesRead = 0;

void PrintCommState(DCB dcb)
{
    //  Print some of the DCB structure values
    printf("\nBaudRate = %d, ByteSize = %d, Parity = %d, StopBits = %d\n",
        dcb.BaudRate,
        dcb.ByteSize,
        dcb.Parity,
        dcb.StopBits);
}


void serial_init(void)
{
    //  Open a handle to the specified com port.
    //hCom = CreateFile(TEXT("\\\\.\\COM15"),
    hCom = CreateFile(pcCommPort,
        GENERIC_READ | GENERIC_WRITE,
        0,      //  must be opened with exclusive-access
        NULL,   //  default security attributes
        OPEN_EXISTING, //  must use OPEN_EXISTING
        0,      //  not overlapped I/O
        NULL); //  hTemplate must be NULL for comm devices

    //printf("here");


    if (hCom == INVALID_HANDLE_VALUE)
    {
        //  Handle the error.
        //printf("\n\33[31mCreateFile failed with error %d.\n\n\33[m", GetLastError());
        printf("\n\33[31mUnable to open %s\n\n\33[m", pcCommPort);
        exit(0);
    }

    //  Initialize the DCB structure.
    SecureZeroMemory(&dcb, sizeof(DCB));
    dcb.DCBlength = sizeof(DCB);

    //  Build on the current configuration by first retrieving all current
    //  settings.
    fSuccess = GetCommState(hCom, &dcb);

    if (!fSuccess)
    {
        //  Handle the error.
        printf("\n\33[31mGetCommState failed with error %d.\n\33[m", GetLastError());
        exit(0);
    }

    PrintCommState(dcb);       //  Output to console

    //  Fill in some DCB values and set the com state: 
   //  2,200,000 bps, 8 data bits, no parity, and 1 stop bit.
    //dcb.BaudRate = CBR_57600;     // baud rate
    dcb.BaudRate = 2200000;         // Baud rate
    dcb.ByteSize = 8;               // data size, xmit and rcv
    dcb.Parity   = NOPARITY;        // parity bit
    dcb.StopBits = ONESTOPBIT;      // stop bit

    fSuccess = SetCommState(hCom, &dcb);

    if (!fSuccess)
    {
        //  Handle the error.
        printf("\n\33[31mSetCommState failed with error %d.\n\33[m", GetLastError());
        exit(0);
    }

    //  Get the comm config again.
    fSuccess = GetCommState(hCom, &dcb);

    if (!fSuccess)
    {
        //  Handle the error.
        printf("\n\33[31mGetCommState failed with error %d.\n\33[m", GetLastError());
        exit(0);
    }

    PrintCommState(dcb);
    printf("Serial port %s successfully configured.\n", pcCommPort);
}

char serial_getc(void)
{
    char Rx_buff[1] = {0};

    if (!ReadFile(hCom, Rx_buff, 1, &dwBytesRead, NULL)) {
        printf("SetCommState failed. Error: %d.\n", GetLastError());
        CloseHandle(hCom);
        exit(0);
    }
    else {
        printf("Bytes read %d -> %s\n", dwBytesRead, Rx_buff);
    }
}

void serial_end(void)
{
    CloseHandle(hCom);
    printf("Serial port %s successfully closed.\n", pcCommPort);
}
