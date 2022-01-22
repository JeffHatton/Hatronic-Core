/**
 * @file Fifo.h
 * @author Jeff Hatton
 * @brief Module that defines a First In First Out buffer. 
 *        Supports both byte and object mode. Only a single mode is allowed
 *        and is chosen at the first put operation.
 * @date 2022-01-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef Fifo_H
#define Fifo_H

#include "Status.h"
#include "stdbool.h"

// Exported Types ************************************

typedef struct 
{
    /**
     * @brief The memory buffer to use for the fifo
     * 
     */
    uint8_t* Buf;

    /**
     * @brief Size of the memory buffer
     * 
     */
    uint16_t Size;

    /**
     * @brief The index into the buffer to write the next byte
     * 
     */
    uint16_t Head;

    /**
     * @brief The index into the buffer to read the next byte
     * 
     */
    uint16_t Tail;

    /**
     * @brief If to allow puts to clear out old data
     * 
     */
    bool AllowWrap;
    
    /**
     * @brief If object mode is enabled
     * 
     */
    bool ObjectMode;

    /**
     * @brief If byte mode is enabled
     * 
     */
    bool ByteMode;

    /**
     * @brief Current status of the fifo
     * 
     */
    Status_t Status;

    //TODO setup lock/unlock
    /**
     * @brief Pointer to the lock object for the fifo
     * 
     */
    void* LockObj;
} Fifo_t;

// Exported Constants ************************************

// Exported Objects **************************************

// Exported Macros ***************************************

// Exported Functions ************************************

/**
 * @brief Init the fifo
 * 
 * @param fifo The fifo to be intited
 * @param buffer The pointer to the buffer to use with the fifo
 * @param size The size of the buffer for the buffer
 * @return Status_t 
 */
Status_t Fifo_Init(Fifo_t* fifo, uint8_t* buffer, uint16_t size);

/**
 * @brief Set if put operations can remove data to fit new data
 * 
 * @param fifo The fifo
 * @param allowWrap If to allow wraps
 * @return Status_t 
 */
Status_t Fifo_SetAllowWrap(Fifo_t* fifo, bool allowWrap);

/**
 * @brief Get the amount of free bytes in the fifo
 * 
 * @param fifo The fifo
 * @param free Buffer for the number of free bytes
 * @return Status_t 
 */
Status_t Fifo_GetFree(Fifo_t* fifo, uint16_t* free);

/**
 * @brief Get the number of used bytes in the fifo
 * 
 * @param fifo The fifo
 * @param used Buffer for the number of fused bytes
 * @return Status_t 
 */
Status_t Fifo_GetUsed(Fifo_t* fifo, uint16_t* used);

/**
 * @brief Get if the fifo is full
 * 
 * @param fifo The fifo
 * @param full Buffer for is the fifo is full
 * @return Status_t 
 */
Status_t Fifo_IsFull(Fifo_t* fifo, bool* full);

/**
 * @brief Get if the fifo is empty
 * 
 * @param fifo The fifo
 * @param empty Buffer for if the fifo is empty
 * @return Status_t 
 */
Status_t Fifo_IsEmpty(Fifo_t* fifo, bool* empty);

/**
 * @brief Put bytes into the fifo. DO NOT use with Fifo_*Obj methods
 * 
 * @param fifo The fifo
 * @param data Pointer to the data to store in the fifo
 * @param size The amount of bytes to put into the fifo
 * @return Status_t 
 */
Status_t Fifo_Put(Fifo_t* fifo, uint8_t* data, uint16_t size);

/**
 * @brief Get a number of bytes from the fifo. The fifo must have all 
 * of the bytes in its buffer for a successful get.
 * 
 * @param fifo The fifo
 * @param size The number of bytes to get
 * @param data Buffer for the bytes
 * @return Status_t 
 */
Status_t Fifo_Get(Fifo_t* fifo, uint16_t size, uint8_t* data);

/**
 * @brief Remove a number of bytes/objects from the fifo without returning the values.
 * If there is not enough data in the buffer than requested the fifo will be cleared with
 * no error. If in object mode it will pop the number of objects not bytes.
 * 
 * @param fifo The fifo
 * @param size The number of bytes to pop in byte mode and number of objects to pop in object mode
 * @return Status_t 
 */
Status_t Fifo_Pop(Fifo_t* fifo, uint16_t size);

/**
 * @brief Get a number of bytes out of the fifo without pulling them out of the fifo.
 * Fifo must contain at least the number of bytes requested
 * 
 * @param fifo The fifo
 * @param offset The offset in bytes to peek at.
 * @param size The number of bytes to peek from the fifo
 * @param data The buffer for the data
 * @return Status_t 
 */
Status_t Fifo_Peek(Fifo_t* fifo, uint16_t offset, uint16_t size, uint8_t* data);

/**
 * @brief Put an object into the fifo.
 * 
 * @param fifo The fifo
 * @param data Pointer to object to put in
 * @param size The size of the object in bytes
 * @return Status_t 
 */
Status_t Fifo_PutObj(Fifo_t* fifo, uint8_t* data, uint16_t size);

/**
 * @brief Get an object from the fifo.
 * 
 * @param fifo The fifo
 * @param size Must be filled with the size of data. If the size is too small the number
 * of bytes needs is returned in this parameter
 * @param data The buffer for the data
 * @return Status_t 
 */
Status_t Fifo_GetObj(Fifo_t* fifo, uint16_t* size, uint8_t* data);

/**
 * @brief Get on object from the fifo with out removing it from the fifo.
 * 
 * @param fifo The fifo
 * @param offset The number of objects to skip before peaking.
 * @param size Must be filled with the size of data. If the size is too small the number
 * of bytes needs is returned in this parameter
 * @param data The buffer for the data
 * @return Status_t 
 */
Status_t Fifo_PeekObj(Fifo_t* fifo, uint16_t offset, uint16_t* size, uint8_t* data);

/**
 * @brief Get the number of objects in the fifo
 * 
 * @param fifo The fifo
 * @param count The buffer for the number of objects
 * @return Status_t 
 */
Status_t Fifo_GetNumObjs(Fifo_t* fifo, uint16_t* count);

/**
 * @brief Clear the buffer. Should by called by modules consuming from the
 * fifo for thread safe operation.
 * 
 * @param fifo The fifo
 * @return Status_t 
 */
Status_t Fifo_ResetByConsumer(Fifo_t* fifo);

/**
 * @brief Clear the buffer. Should by called by modules placing data into the
 * fifo for thread safe operation.
 * 
 * @param fifo The fifo
 * @return Status_t 
 */
Status_t Fifo_ResetByProducer(Fifo_t* fifo);

// END Fifo_H
#endif