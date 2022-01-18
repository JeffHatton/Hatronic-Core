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

// Macros ***********************************************

/**
 * @brief Return the max capacity of the FIFO
 * 
 */
#define CAP(fifo) (fifo->Size-1)

// Functions Prototypes *********************************

// Private Variables ************************************

// Private Functions *************************************

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

    uint16_t free;
    Status_t ret = Fifo_GetFree(fifo, &free);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    //TODO LOCK

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

    uint16_t head = fifo->Head;
    uint16_t counter = 0;
    while (counter < size)
    {    
        fifo->Buf[head] = data[counter];

        counter++;
        head++;
        if (head == fifo->Size) head = 0;
    }

    fifo->Head = head;

    //TODO unlock

    return Status_Ok;
}

Status_t Fifo_Get(Fifo_t* fifo, uint16_t size, uint8_t* data)
{
    if (fifo == NULL) return Status_NullPtr;
    if (data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (used < size) return Status_NotEnoughData;

    //TODO LOCK

    uint16_t tail = fifo->Tail;
    uint16_t counter = 0;
    while (counter < size)
    {
        data[counter] = fifo->Buf[tail];

        counter++;
        tail++;
        if (tail == fifo->Size) tail = 0;
    }

    fifo->Tail = tail;

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

    uint16_t tail = fifo->Tail;
    uint16_t counter = 0;
    while (counter < size)
    {
        counter++;
        tail++;
        if (tail == fifo->Size) tail = 0;
    }

    fifo->Tail = tail;

    //TODO unlock

    return Status_Ok;     
}

Status_t Fifo_Peek(Fifo_t* fifo, uint16_t offset, uint16_t size, uint8_t* data)
{
    if (fifo == NULL) return Status_NullPtr;
    if (data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(fifo->Status)) return STATUS_PASS_UP(fifo->Status);

    uint16_t used;
    Status_t ret = Fifo_GetUsed(fifo, &used);
    if (STATUS_CHECK(ret)) return STATUS_PASS_UP(ret);

    if (used < size + offset) return Status_NotEnoughData;

    uint16_t counter = 0;
    uint16_t tail = fifo->Tail + offset;
    while (counter < size)
    {        
        data[counter] = fifo->Buf[tail];

        counter++;
        tail++;
        if (tail == fifo->Size) tail = 0;
    }

    return Status_Ok;
}

Status_t Fifo_PutObj(Fifo_t* fifo, void* data, uint16_t size);
Status_t Fifo_GetObj(Fifo_t* fifo, uint16_t* bufferSize, uint16_t* size, void* data);
Status_t Fifo_PeekObj(Fifo_t* fifo, uint16_t offset, uint16_t* bufferSize, uint16_t* size, void* data);

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