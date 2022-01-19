/**
 * @file Fifo.h
 * @author Jeff Hatton
 * @brief Module that defines a First In First Out buffer. 
 *        Supports both byte and object mode.
 * @date 2022-01-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef Fifo
#define Fifo

#include "Status.h"
#include "stdbool.h"

// Exported Types ************************************

typedef struct 
{
    uint8_t* Buf;
    uint16_t Size;
    uint16_t Head;
    uint16_t Tail;
    bool AllowWrap;
    bool ObjectMode;
    bool ByteMode;
    Status_t Status;
    void* LockObj;
} Fifo_t;

// Exported Constants ************************************

// Exported Objects **************************************

// Exported Macros ***************************************

// Exported Functions ************************************

Status_t Fifo_Init(Fifo_t* fifo, uint8_t* buffer, uint16_t size);

Status_t Fifo_SetAllowWrap(Fifo_t* fifo, bool allowWrap);

Status_t Fifo_GetFree(Fifo_t* fifo, uint16_t* free);
Status_t Fifo_GetUsed(Fifo_t* fifo, uint16_t* used);
Status_t Fifo_IsFull(Fifo_t* fifo, bool* full);
Status_t Fifo_IsEmpty(Fifo_t* fifo, bool* empty);

Status_t Fifo_Put(Fifo_t* fifo, uint8_t* data, uint16_t size);
Status_t Fifo_Get(Fifo_t* fifo, uint16_t size, uint8_t* data);
Status_t Fifo_Pop(Fifo_t* fifo, uint16_t size);
Status_t Fifo_Peek(Fifo_t* fifo, uint16_t offset, uint16_t size, uint8_t* data);

Status_t Fifo_PutObj(Fifo_t* fifo, uint8_t* data, uint16_t size);
Status_t Fifo_GetObj(Fifo_t* fifo, uint16_t* size, uint8_t* data);
Status_t Fifo_PeekObj(Fifo_t* fifo, uint16_t offset, uint16_t* size, uint8_t* data);

Status_t Fifo_ResetByConsumer(Fifo_t* fifo);
Status_t Fifo_ResetByProducer(Fifo_t* fifo);

// END Fifo
#endif