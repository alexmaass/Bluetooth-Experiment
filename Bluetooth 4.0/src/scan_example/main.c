
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cmd_def.h"


volatile HANDLE serial_handle;

void output(uint8 len1,uint8* data1,uint16 len2,uint8* data2)
{
    DWORD written;

    if(!WriteFile (serial_handle,
                   data1,
                   len1,
                   &written,
                   NULL
                   ))
        {
            printf("ERROR: Writing data. %d\n",(int)GetLastError());
            exit(-1);
        }

    if(!WriteFile (serial_handle,
                   data2,
                   len2,
                   &written,
                   NULL
                   ))
        {
            printf("ERROR: Writing data. %d\n",(int)GetLastError());
            exit(-1);
        }
}
int read_message()
{
    DWORD rread;
    const struct ble_msg *apimsg;
    struct ble_header apihdr;
    unsigned char data[256];//enough for BLE
    //read header

    if(!ReadFile(serial_handle,
               (unsigned char*)&apihdr,
               4,
               &rread,
               NULL))
        {
            return GetLastError();
        }
    if(!rread)return 0;
    //read rest if needed
    if(apihdr.lolen)
    {
        if(!ReadFile(serial_handle,
                   data,
                   apihdr.lolen,
                   &rread,
                   NULL))
            {
                return GetLastError();
            }
    }
    apimsg=ble_get_msg_hdr(apihdr);
    if(!apimsg)
    {
        printf("ERROR: Message not found:%d:%d\n",(int)apihdr.cls,(int)apihdr.command);
        return -1;
    }
    apimsg->handler(data);

    return 0;
}

void print_help()
{
    printf("Demo application to scan devices\n");
    printf("\tscan_example\tCOM-port\n");
}

int main(int argc, char *argv[] )
{    
    char str[80];

    if(argc<2)
    {
        print_help();
        exit(-1);
    }
    snprintf(str,sizeof(str)-1,"\\\\.\\%s",argv[1]);
    serial_handle = CreateFileA(str,
                                   GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   NULL,
                                   OPEN_EXISTING,
                                   0,
                                   NULL);


    if (serial_handle == INVALID_HANDLE_VALUE)
    {
        printf("Error opening serialport %s. %d\n",argv[1],(int)GetLastError());
        return -1;
    }

    bglib_output=output;


    //stop previous operation
    ble_cmd_gap_end_procedure();
    //get connection status,current command will be handled in response
    ble_cmd_connection_get_status(0);

    //Message loop
    while(1)
    {
        if(read_message())
        {
            printf("Error reading message\n");
            break;
        }
    }

    return 0;
}
