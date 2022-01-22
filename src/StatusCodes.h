/**
 * @file StatusCodes.h
 * @author Jeff Hatton
 * @brief 
 * @date 2022-01-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef StatusCodes
#define StatusCodes

// Exported Types ************************************

#define STATUS_CORE_START 0x001
#define STATUS_MEMORY_START 0x010
#define STATUS_STATE_START 0x020
#define STATUS_PROCESS_START 0x030
#define STATUS_COMS_START 0x040
#define STATUS_HARDWARE_START 0x050
#define STATUS_CUSTOM_START 0x800

typedef enum 
{
    Status_Ok = 0,

    // Core Statuses
    Status_NotInitialized = STATUS_CORE_START,
    Status_AlreadyInitialized, 
    Status_NullPtr,
    Status_ParamError,

    // Memory Statuses
    Status_BufferFull = STATUS_MEMORY_START,
    Status_BufferEmpty,
    Status_BufferTooSmall,
    Status_NotEnoughData,

    // State Statuses
    Status_InvalidState = STATUS_STATE_START,

} Statuses_t;

// Exported Constants ************************************

// Exported Objects **************************************

// Exported Macros ***************************************

// Exported Functions ************************************

// END StatusCodes
#endif