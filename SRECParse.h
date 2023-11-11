#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<stdlib.h>

#define HEX_INVALID 0xFF

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

uint8_t Char2Hex(uint8_t c);
SREC_Status_Type Str2Hex(uint8_t *pStr, uint8_t len, uint32_t *result);
FILE* getFilePtr(void);                                                 // to test manually
SREC_Status_Type SREC_OpenFile(const uint8_t* fileName);
void SREC_CloseFile(void);
void SREC_InitLine(SREC_Type *srec_line);
SREC_Status_Type SREC_StartSrecFile(void);
SREC_Status_Type SREC_ReadLine(void);
SREC_Status_Type SREC_CheckSum(void);
Bool_Type SREC_isEOF();