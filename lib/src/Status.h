/**
 * @file Status.h
 * @author Jeff Hatton
 * @brief Basic module that defines return statuses format and basic statuses.
 * @date 2022-01-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//TODO documentation on how to use status

#ifndef STATUS
#define STATUS

#include <stdint.h>
#include <stdio.h>

// Exported Types ************************************

typedef uint32_t Status_t;

/**
 * @brief enum that defines the various serverities supported
          by status.
 * 
 */
typedef enum {
    /**
     * @brief Status represents an error. 
     * 
     */
    StatusServerity_Error = 0,

    /**
     * @brief Status represents a warning. Status is not
     *        means something is wrong but isn't an error
     */
    StatusServerity_Warn,

    /**
     * @brief Status is a fatal error. This must be handled.
     *        The software unit that generated this error most
     *        likely doesn't function anymore.
     * 
     */
    StatusServerity_Fatal,

    /**
     * @brief Serveritry left open for user use.
     * 
     */
    StatusServerity_CU,

    /**
     * @brief The count of the servertities
     * 
     */
    StatusServerity_Count,
} StatusServerity_t;

/**
 * @brief Struct the defines the bit structure of the status metadata
 * 
 */
typedef struct {
    /**
     * @brief If the function that returned this status was its orginator.
     * 
     */
    uint8_t Originating : 1;

    /**
     * @brief If the status has been handled
     * 
     */
    uint8_t Handled : 1;

    /**
     * @brief Used bits for future expansion
     * 
     */
    uint8_t Unused : 4;

    /**
     * @brief The severity of the status
     * 
     */
    uint8_t Severity : 2;
}  StatusMetadata_t;

typedef union {
    uint8_t value;
    StatusMetadata_t Meta;
} StatusMetadataMask_t;

// Exported Constants ************************************

/**
 * @brief Size in bits of the status portion
 * 
 */
#define STATUS_CODE_SIZE 12

/**
 * @brief Size in bits of the ID portion
 * 
 */
#define STATUS_ID_SIZE 12

/**
 * @brief Size in bits of the metadata portion
 * 
 */
#define STATUS_META_SIZE 8

/**
 * @brief Amount to left shift id values
 * 
 */
#define STATUS_ID_SHIFT_AMT (STATUS_CODE_SIZE)

/**
 * @brief Amount to left shift metadata values
 * 
 */
#define STATUS_META_SHIFT_AMT (STATUS_CODE_SIZE+STATUS_ID_SIZE)

/**
 * @brief Mask to get the status code from a status
 * 
 */
#define STATUS_CODE_MASK (0x00000FFF)

/**
 * @brief Mask to get the ID portion of a status
 * 
 */
#define STATUS_ID_MASK (0x00FFF000)

/**
 * @brief Mask to get the meta data portion of a status
 * 
 */
#define STATUS_META_MASK (0xFF000000)

/**
 * @brief Mask for the orginating bit after geting the metadata
          from the status
 * 
 */
#define STATUS_META_ORIGINATING_MASK (0x1)

/**
 * @brief Mask for the handled bit after geting the metadata
          from the status
 * 
 */
#define STATUS_META_HANDLED_MASK (0x2)

/**
 * @brief Mask for the severity bits after geting the metadata
          from the status
 * 
 */
#define STATUS_META_SEV_MASK (0xC0)

/**
 * @brief Mask for the handled bit in the packed status
 * 
 */
#define STATUS_META_HANDLED_MASK_PACKED (STATUS_META_HANDLED_MASK << STATUS_META_SHIFT_AMT)

/**
 * @brief Mask for the originating bit in the packed status
 * 
 */
#define STATUS_META_ORIGINATING_MASK_PACKED (STATUS_META_ORIGINATING_MASK << STATUS_META_SHIFT_AMT)

/**
 * @brief Mask for the severity bits in the packed status
 * 
 */
#define STATUS_META_SERV_MASK_PACKED (STATUS_META_SEV_MASK << STATUS_META_SHIFT_AMT)

// Exported Objects **************************************

// Exported Macros ***************************************

//
// Status Building Macros
//

/**
 * @brief Set the metadata bits in a status
 * 
 */
#define STATUS_SET_META(status, meta) ((status & (~STATUS_META_MASK)) | (meta << STATUS_META_SHIFT_AMT))

/**
 * @brief Set the ID bits in a status
 * 
 */
#define STATUS_SET_ID(status, suId) ((status & (~STATUS_ID_MASK)) | (suId << STATUS_ID_SHIFT_AMT))

/**
 * @brief Set the code bits in a status
 * 
 */
#define STATUS_SET_CODE(status, code) ((status & ~STATUS_CODE_MASK) | code)

/**
 * @brief Pack the passed code into the status. Requires SU to have SuId local variable.
 * 
 */
#define STATUS_PACK(code) (STATUS_SET_ID(code, SuId))

/**
 * @brief Packs the passed meta flags and code into the status. Requires SU to have SuId local variable.
 * 
 */
#define STATUS_PACK_META(code, meta) (STATUS_PACK((STATUS_SET_META(code, meta))))

/**
 * @brief Packs the passed meta flags, code and ID into the status.
 * 
 */
#define STATUS_PACK_META_SU(code, suId, meta) ((STATUS_SET_ID(STATUS_SET_META(code, meta), suId)))

//
// Status Decoding macros
//

/**
 * @brief Checks if a status' code is equal to the passed code
 * 
 */
#define STATUS_IS_CODE(status, code) (STATUS_GET_CODE(status) == code)

/**
 * @brief Get the metadata value from the status
 * 
 */
#define STATUS_GET_METADATA(status) ((status & STATUS_META_MASK) >> STATUS_META_SHIFT_AMT)

/**
 * @brief Populate a metadata object from the status
 * 
 */
#define STATUS_POP_METADATA(status, metaObj) (metaObj.value = STATUS_GET_METADATA(status))

/**
 * @brief Get the ID from the status
 * 
 */
#define STATUS_GET_ID(status) ((status & STATUS_ID_MASK) >> STATUS_ID_SHIFT_AMT)

/**
 * @brief Get the code from the status
 * 
 */
#define STATUS_GET_CODE(status) (status & STATUS_CODE_MASK)


//
// Metadata decode macros
//

/**
 * @brief Get the severity from the status
 * 
 */
#define STATUS_GET_SEV(status) ((STATUS_GET_METADATA(status) & STATUS_META_SEV_MASK) >> 6)

/**
 * @brief Check if the severity of the status is error
 * 
 */
#define STATUS_IS_ERROR(status) (STATUS_GET_SEV(status) == StatusServerity_Error)

/**
 * @brief Check if the severity of the status is warn
 * 
 */
#define STATUS_IS_WARN(status) (STATUS_GET_SEV(status) == StatusServerity_Warn)

/**
 * @brief Check if the severity of the status is fatal
 * 
 */
#define STATUS_IS_FATAL(status) (STATUS_GET_SEV(status) == StatusServerity_Fatal)

/**
 * @brief Check if the severity of the status is custom user
 * 
 */
#define STATUS_IS_CU(status) (STATUS_GET_SEV(status) == StatusServerity_CU)

/**
 * @brief Check if the status has been handled
 * 
 */
#define STATUS_IS_HANDLED(status) (STATUS_GET_METADATA(status) & STATUS_META_HANDLED_MASK)

/**
 * @brief Check if the status originates from the returned function
 * 
 */
#define STATUS_IS_ORGINATOR(status) (STATUS_GET_METADATA(status) & STATUS_META_ORIGINATING_MASK)

//
// Helper Macros
//

/**
 * @brief Check if the status is a bad return. If the status is not 0 or has had any data 
            packed into it this will return true. Use this for a quick check if the status
            needs further examination.
 * 
 */
#define STATUS_CHECK(status) (status != 0)

/**
 * @brief Check if the status is a bad return and its severity matches the passed value.
 * 
 */
#define STATUS_CHECK_SEV(status, sev) (status != 0 && STATUS_GET_SEV(status) == sev)

/**
 * @brief Mark the status has handled
 * 
 */
#define STATUS_HANDLED(status) (status | STATUS_META_HANDLED_MASK_PACKED)

/**
 * @brief Call this when allowing a status to filter up from function calls.
 * 
 */
#define STATUS_PASS_UP(status) (status | STATUS_META_ORIGINATING_MASK_PACKED)


//TODO handle checking for multiple severities

// Exported Functions ************************************

//TODO to string
//TODO CLI?

// END STATUS
#endif

