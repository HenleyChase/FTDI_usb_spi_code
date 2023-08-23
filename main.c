#include <stdio.h>
#include <stdlib.h>
#include <ftd2xx.h> // FTDI D2XX library

int main() {
    // Status and Handle used by all functions
    FT_STATUS ftStatus;
    FT_HANDLE ftHandle;
    // 
    // First Open the Device
    // I thought it might be easiest to open the device by description but what we use to open the device in FT_OpenEx can change
    // Look up FT_OpenEx in D2XX_Programmers_GuideFT_000071.pdf if you need to change how you open the device
    //
    long dwLoc;
    char deviceDescription[] = "USB Serial Converter"; // Update this with our device device description
    ftStatus = FT_OpenEx(deviceDescription,FT_OPEN_BY_DESCRIPTION,&ftHandle);
    if (ftStatus != FT_OK) {
        // device with device description "USB Serial Converter" failed to open
        printf("Error: Unable to open FTDI device\n");
        return 1;
    }

    //
    // Set the SPI interface parameters
    // 
    UCHAR Mask = 0xff;
    // FT_BITMODE_SYNC_FIFO = 0x40 which is for Single Channel Synchronous 245 FIFO Mode (FT2232H and FT232H devices only). 
    // That may need to change Idk. The other modes are listed in the documentation
    ftStatus = FT_SetBitMode(ftHandle, Mask, FT_BITMODE_SYNC_FIFO);
    if (ftStatus != FT_OK) {
        printf("Error: Unable to set bit mode\n");
        FT_Close(ftHandle);
        return 1;
    }

    // Send and receive data over SPI
    
    //
    // Write the data
    //
    unsigned char writeData[4] = {0x01, 0x02, 0x03, 0x04}; // Test data to write
    DWORD BytesWritten;
    ftStatus = FT_Write(ftHandle, writeData, sizeof(writeData), &BytesWritten);
    if (ftStatus != FT_OK) {
        printf("Error: Unable to write data\n");
        FT_Close(ftHandle);
        return 1;
    }

    //
    // Read the data
    //
    unsigned char readData[4] = {0}; // All elements are initialized to 0
    DWORD BytesRead;
    ftStatus = FT_Read(ftHandle, readData, sizeof(readData), &BytesRead);
    if (ftStatus != FT_OK) {
        printf("Error: Unable to read data\n");
        FT_Close(ftHandle);
        return 1;
    }

    // Close the FTDI device
    FT_Close(ftHandle);

    // Print the received data
    for (int i = 0; i < 4; i++) {
        printf("Read data[%d]: 0x%02X\n", i, readData[i]);
    }

    return 0;
}