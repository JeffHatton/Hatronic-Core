/**
 * @file StatusCodes.h
 * @author Jeff Hatton
 * @brief 
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef StatusCodes_H
#define StatusCodes_H

// Exported Types ************************************

#define STATUS_CORE_START 0x001
#define STATUS_MEMORY_START 0x010
#define STATUS_STATE_START 0x020
#define STATUS_PROCESS_START 0x030
#define STATUS_COMS_START 0x040
#define STATUS_HARDWARE_START 0x050
#define STATUS_DATA_START 0x060
#define STATUS_CUSTOM_START 0x800

typedef enum 
{
    // Status logic requires good status be equal to 0
    // so leave good status in its own section
    Status_Ok = 0,

    // Core Statuses
    Status_NotInitialized = STATUS_CORE_START,
    Status_AlreadyInitialized, 
    Status_NullPtr,
    Status_ParamError,
    Status_NotImplemented,

    // Memory Statuses
    Status_BufferFull = STATUS_MEMORY_START,
    Status_BufferEmpty,
    Status_BufferTooSmall,
    Status_NotEnoughData,
    Status_AlreadyInBuffer,
    Status_IndexOOR, 

    // State Statuses
    Status_InvalidState = STATUS_STATE_START,

    // Data Statuses
    Status_NotFound = STATUS_DATA_START,
    Status_EOF,    

} Statuses_t;

// Exported Constants ************************************

// Exported Objects **************************************

// Exported Macros ***************************************

// Exported Functions ************************************

// END StatusCodes_H
#endif