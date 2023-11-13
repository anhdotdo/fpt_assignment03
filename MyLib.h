#ifndef _MYLIB_H_
#define _MYLIB_H_

typedef struct
{
    uint8_t SrecType[3];
    uint8_t ByteCount;
    uint32_t Address;
    uint8_t * Data; // Point to heap (storage Data)
    uint8_t Checksum;
} SREC_Type;

typedef enum
{
    SREC_LINE_TRUE = 0x00u,                     // a valid line?
    SREC_ERROR_FORMAT = 0x01u,                  // ...
    SREC_ERROR_BYTECOUNT = 0x02u,               // ...
    SREC_ERROR_CHECKSUM = 0x03u,                // ...
    SREC_FILE_NOT_EXIST = 0x04u,                // a exist file?
    SREC_FILE_EXIST = 0x05u,                    // ...
    SREC_FILE_START = 0x06u,                    // a valid file?
    SREC_FILE_END = 0x07u,
    SREC_FILE_FALSE = 0x08u,
} SREC_Status_Type;

typedef enum
{
    FALSE,
    TRUE,
}Bool_Type;

#endif