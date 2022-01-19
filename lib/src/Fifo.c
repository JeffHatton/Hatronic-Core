/**
 * @file Fifo.c
 * @author Jeff Hatton
 * @brief 
 * @date 2022-01-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Fifo.h"
#include "StatusCodes.h"

// Private Types ****************************************

// Constants ********************************************

#define OBJ_HEADER_SIZE 2

// Macros ***********************************************

/**
 * @brief Return the max capacity of the FIFO
 * 
 */
#define CAP(fifo) (fifo->Size-1)

// Functions Prototypes *********************************

// Private Variables ************************************

// Private Functions *************************************

static inline void PutByte(Fifo_t* fifo, uint8_t byte)
{
    fifo->Buf[fifo->Head] = byte;
    fifo->Head++;
    if (fifo->Head == fifo->Size) fifo->Head = 0;
}

static inline uint8_t GetByte(Fifo_t* fifo)
{
    uint8_t data = fifo->Buf[fifo->Tail];
    fifo->Tail++;
    if (fifo->Tail == fifo->Size) fifo->Tail = 0;

    return data;
}

// Public Functions *************************************

Status_t Fifo_Init(Fifo_t* fifo, uint8_t* buffer, uint16_t size)
{
    if (fifo == NULL) return Status_NullPtr;
    if (buffer == NULL) return Status_NullPtr;

    if (!STATUS_IS_CODE(fifo->Status, Status_NotInitialized))
    {
        return Status_AlreadyInitialized;
    }

    fifo->Buf = buffer;
    fifo->Size = size;
    fifo->Status = Status_Ok;
    fifo->AllowWrap = false;
    fifo->ObjectMode = false;
    fifo->ByteMode = false;

    return Status_Ok;
}

Status_t Fifo_SetAllowWrap(Fifo_t* fifo, bool allowWrap)
{
    if (fifo == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    fifo->AllowWrap = allowWrap;

    return Status_Ok;
}

Status_t Fifo_GetFree(Fifo_t* fifo, uint16_t* free)
{
    if (fifo == NULL) return Status_NullPtr;
    if (free == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);
    
    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    *free = CAP(fifo) - used;

    return Status_Ok;
}

Status_t Fifo_GetUsed(Fifo_t* fifo, uint16_t* used)
{
    if (fifo == NULL) return Status_NullPtr;
    if (used == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);
    
    uint16_t head = fifo->Head;
    uint16_t tail = fifo->Tail;

    if (tail > head)
    {
        //TODO minus 1?
        *used = fifo->Size - (tail - head);
    }
    else if (tail < head)
    {
        *used = head - tail;
    }
    else
    {
        // Fifo is empty
        *used = 0;
    }

    return Status_Ok;
}

Status_t Fifo_IsFull(Fifo_t* fifo, bool* full)
{
    if (fifo == NULL) return Status_NullPtr;
    if (full == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    uint16_t free;
    Status_t ret = Fifo_GetFree(fifo, &free);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    *full = free == 0;

    return Status_Ok;
}

Status_t Fifo_IsEmpty(Fifo_t* fifo, bool* empty)
{
    if (fifo == NULL) return Status_NullPtr;
    if (empty == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    *empty = fifo->Head == fifo->Tail;

    return Status_Ok;
}

Status_t Fifo_Put(Fifo_t* fifo, uint8_t* data, uint16_t size)
{
    if (fifo == NULL) return Status_NullPtr;
    if (data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    if (fifo->ObjectMode) return Status_InvalidState;

    uint16_t free;
    Status_t ret = Fifo_GetFree(fifo, &free);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    //TODO LOCK

    fifo->ByteMode = true;

    if (free < size) {
        // If wrap is allowed free space up
        if (fifo->AllowWrap)
        {
            if (CAP(fifo) < size)
            {
                return Status_BufferFull;
            }

            ret = Fifo_Pop(fifo, size - free);
            if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);
        }        
        else
        {
            return Status_BufferFull;
        }
    }

    uint16_t counter = 0;
    while (counter < size)
    {    
        PutByte(fifo, data[counter]);
        counter++;
    }

    //TODO unlock

    return Status_Ok;
}

Status_t Fifo_Get(Fifo_t* fifo, uint16_t size, uint8_t* data)
{
    if (fifo == NULL) return Status_NullPtr;
    if (data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    if (fifo->ObjectMode) return Status_InvalidState;

    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (used < size) return Status_NotEnoughData;

    //TODO LOCK

    uint16_t counter = 0;
    while (counter < size)
    {
        data[counter] = GetByte(fifo);
        counter++;
    }

    //TODO unlock

    return Status_Ok;
}

Status_t Fifo_Pop(Fifo_t* fifo, uint16_t size)
{
    if (fifo == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status); 

    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    //TODO LOCK
    if (used < size) {
        fifo->Tail = fifo->Head;
        return Status_Ok;
    }

    if (fifo->ObjectMode)
    {
        for (size_t i = 0; i < size; i++)
        {
            uint16_t objSize = GetByte(fifo) | GetByte(fifo) << 8;
            uint16_t counter = 0;
            while (counter < objSize)
            {
                GetByte(fifo);
                counter++;
            }
        }
    }
    else
    {
        uint16_t counter = 0;
        while (counter < size)
        {
            GetByte(fifo);
            counter++;
        }
    }

    //TODO unlock

    return Status_Ok;     
}

Status_t Fifo_Peek(Fifo_t* fifo, uint16_t offset, uint16_t size, uint8_t* data)
{
    if (fifo == NULL) return Status_NullPtr;
    if (data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    // if (fifo->ObjectMode) return Status_InvalidState;

    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (used < size + offset) return Status_NotEnoughData;

    uint16_t counter = 0;
    uint16_t tail = fifo->Tail + offset;
    if (tail >= fifo->Size) tail -= fifo->Size;

    while (counter < size)
    {        
        data[counter] = fifo->Buf[tail];

        counter++;
        tail++;
        if (tail == fifo->Size) tail = 0;
    }

    return Status_Ok;
}

Status_t Fifo_PutObj(Fifo_t* fifo, uint8_t* data, uint16_t size)
{
    if (fifo == NULL || data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    if (fifo->ByteMode) return Status_InvalidState;

    uint16_t free;
    Status_t ret = Fifo_GetFree(fifo, &free);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    //TODO LOCK

    fifo->ObjectMode = true;

    if (free < (size + OBJ_HEADER_SIZE)) {
        // If wrap is allowed free space up
        if (fifo->AllowWrap)
        {
            if (CAP(fifo) < size)
            {
                return Status_BufferFull;
            }

            do 
            {
                ret = Fifo_Pop(fifo, 1);
                if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

                ret = Fifo_GetFree(fifo, &free);
                if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);
            } while(free < (size + OBJ_HEADER_SIZE));
        }        
        else
        {
            return Status_BufferFull;
        }
    }

    uint8_t byte = size & 0x00FF;
    PutByte(fifo, byte);
    byte = size >> 8;
    PutByte(fifo, byte);

    uint16_t counter = 0;
    while (counter < size)
    {    
        PutByte(fifo, data[counter]);
        counter++;
    }

    //TODO unlock

    return Status_Ok;
}

Status_t Fifo_GetObj(Fifo_t* fifo, uint16_t* size, uint8_t* data)
{
    if (fifo == NULL || data == NULL || size == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    if (!fifo->ObjectMode) return Status_InvalidState;

    bool empty;
    Status_t ret = Fifo_IsEmpty(fifo, &empty);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (empty) return Status_BufferEmpty;

    uint16_t objSize;
    ret = Fifo_Peek(fifo, 0, 2, (uint8_t*)&objSize);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);
    
    if (objSize > *size)
    {
        *size = objSize;
        return Status_BufferTooSmall;
    }

    GetByte(fifo);
    GetByte(fifo);

    *size = objSize;

    uint16_t counter = 0;
    while (counter < objSize)
    {
        data[counter] = GetByte(fifo);
        counter++;
    }

    return Status_Ok;
}


Status_t Fifo_PeekObj(Fifo_t* fifo, uint16_t offset, uint16_t* size, uint8_t* data)
{
    if (fifo == NULL || data == NULL || size == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    if (!fifo->ObjectMode) return Status_InvalidState;

    bool empty;
    Status_t ret = Fifo_IsEmpty(fifo, &empty);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (empty) return Status_BufferEmpty;

    uint16_t tail = fifo->Tail;
    uint16_t currentIdx = 0;
    uint16_t currentOffset = offset;

    while (currentOffset > 0)    
    {
        uint16_t objSize;
        ret = Fifo_Peek(fifo, currentIdx, 2, (uint8_t*)&objSize);
        if (STATUS_CHECK(ret)) return Status_NotEnoughData;

        uint16_t counter = 0;

        currentIdx++;
        currentIdx++;

        tail++;
        if (tail == fifo->Size) tail = 0;
        tail++;
        if (tail == fifo->Size) tail = 0;

        while (counter < *size)
        {        
            currentIdx++;
            counter++;
            tail++;
            if (tail == fifo->Size) tail = 0;
        }
        currentOffset--;
    }

    
    uint16_t objSize = 0;
    ret = Fifo_Peek(fifo, currentIdx, 2, (uint8_t*)&objSize);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);
    
    if (objSize > *size)
    {
        *size = objSize;
        return Status_BufferTooSmall;
    }

    *size = objSize;

    uint16_t counter = 0;
    tail++;
    if (tail == fifo->Size) tail = 0;
    tail++;
    if (tail == fifo->Size) tail = 0;

    while (counter < *size)
    {        
        data[counter] = fifo->Buf[tail];

        counter++;
        tail++;
        if (tail == fifo->Size) tail = 0;
    }

    return Status_Ok;
}

Status_t Fifo_ResetByConsumer(Fifo_t* fifo)
{
    if (fifo == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    //TODO LOCK

    fifo->Head = fifo->Tail;

    //TODO unlock

    return Status_Ok;    
}

Status_t Fifo_ResetByProducer(Fifo_t* fifo)
{
    if (fifo == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    //TODO LOCK

    fifo->Tail = fifo->Head;

    //TODO unlock

    return Status_Ok;
}