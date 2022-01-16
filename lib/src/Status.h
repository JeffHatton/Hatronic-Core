/**
 * @file Status.h
 * @author Jeff Hatton
 * @brief Basic module that defines return statuses format and basic statuses.
 * @date 2022-01-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef Status
#define Status

#include <stdint.h>

// Exported Types ************************************

typedef uint32_t Status_t;

// Exported Constants ************************************

/**
 * @brief Mask to get the status code from a status
 * 
 */
#define STATUS_STATUS_MASK (0x0000FFFF)

/**
 * @brief Mask to get the ID portion of a status
 * 
 */
#define STATUS_ID_MASK (0x0FFF0000)

/**
 * @brief Mask to get the meta data portion of a status
 * 
 */
#define STATUS_META_DATA_MASK (0xF0000000)

// Exported Objects **************************************

// Exported Macros ***************************************


#define STATUS_GET_METADATA(status) ((status & STATUS_META_DATA_MASK) >> 24)

#define STATUS_GET_ID(status) ((status & STATUS_ID_MASK) >> 16)

#define STATUS_GET_STATUS(status) (status & STATUS_STATUS_MASK)

#define STATUS_SU_PACK(suId, status) (status | (suId << 16))

#define STATUS_PACK(status) (status | (SuId << 16))

#define STATUS_IS_STATUS(status, checkStatus) (STATUS_GET_STATUS())

// Exported Functions ************************************

// END Status
#endif

