//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaBinaryUtils.c
//!
//! @author Tobias Fetzer
//!
//! @brief
//! See related header
//!

// related header
#include "BaBinaryUtils.h"

#include <string.h>

void ba_binUtilsSetUInt8Bit(uint32_t bitIdx, uint8_t* bitfield)
{
    if ((bitfield != NULL) && (bitIdx < 8U))
    {
        uint8_t bitValue = (uint8_t)(((uint32_t)1U) << bitIdx);
        (*bitfield) |= bitValue;
    }
}

void ba_binUtilsResetUInt8Bit(uint32_t bitIdx, uint8_t* bitfield)
{
    if ((bitfield != NULL) && (bitIdx < 8U))
    {
        uint8_t bitValue = (uint8_t)(((uint32_t)1U) << bitIdx);
        (*bitfield) &= ~bitValue;
    }
}

bool ba_binUtilsGetUInt8Bit(uint32_t bitIdx, uint8_t bitfield)
{
    bool res = false;
    if (bitIdx < 8U)
    {
        res = ( (bitfield & ((uint8_t)(1U<<bitIdx))) != 0U );
    }
    return res;
}

void ba_binUtilsSetUInt16Bit(uint32_t bitIdx, uint16_t* bitfield)
{
    if ((bitfield != NULL) && (bitIdx < 16U))
    {
        uint16_t bitValue = (uint16_t)(((uint32_t)1U) << bitIdx);
        (*bitfield) |= bitValue;
    }
}

bool ba_binUtilsIsUInt16BitSet(uint32_t bitIdx, uint16_t bitfield)
{
    bool res = false;
    if (bitIdx < 16U)
    {
        res = ( (bitfield & (((uint32_t)1U)<<bitIdx)) != 0U );
    }
    return res;
}

uint32_t ba_binUtilsGetUInt16BitValue(uint32_t bitIdx, uint16_t bitfield)
{
    uint32_t res = 0U;
    if ((bitIdx < 16U) && ((bitfield & (((uint32_t)1U)<<bitIdx)) != 0U))
    {
        res = 1U;
    }
    return res;
}

void ba_binUtilsSetUInt32Bit(uint32_t bitIdx, uint32_t* bitfield)
{
    if ((bitfield != NULL) && (bitIdx < 32U))
    {
        uint32_t bitValue = (uint32_t)(((uint32_t)1U) << bitIdx);
        (*bitfield) |= bitValue;
    }
}

void ba_binUtilsResetUInt32Bit(uint32_t bitIdx, uint32_t* bitfield)
{
    if ((bitfield != NULL) && (bitIdx < 32U))
    {
        uint32_t bitValue = (uint32_t)(((uint32_t)1U) << bitIdx);
        (*bitfield) &= ~bitValue;
    }
}

bool ba_binUtilsIsUInt32BitSet(uint32_t bitIdx, uint32_t bitfield)
{
    bool res = false;
    if (bitIdx < 32U)
    {
        res = ( (bitfield & (((uint32_t)1U)<<bitIdx)) != 0U );
    }
    return res;
}

uint32_t ba_binUtilsGetUInt32BitValue(uint32_t bitIdx, uint32_t bitfield)
{
    uint32_t res = 0U;
    if ((bitIdx < 32U) && ((bitfield & (((uint32_t)1U)<<bitIdx)) != 0U))
    {
        res = 1U;
    }
    return res;
}

bool ba_binUtilsConvertUInt32ToByteArray(uint32_t value, uint8_t* dataPtr, uint32_t numBytes)
{
    bool res = false;

    if ( (dataPtr != NULL) && (numBytes >= 4U) )
    {
        res = true;

        dataPtr[0] = (uint8_t)((value >> 24U) & 0x000000FFU);
        dataPtr[1] = (uint8_t)((value >> 16U) & 0x000000FFU);
        dataPtr[2] = (uint8_t)((value >> 8U) & 0x000000FFU);
        dataPtr[3] = (uint8_t)(value & 0x000000FFU);
    }
    return res;
}

bool ba_binUtilsConvertUInt32ArrayToByteArray(const uint32_t* wordValues, uint32_t wordValuesSize, uint8_t* dataPtr, uint32_t numBytes)
{
    bool res = false;

    if ( (wordValues != NULL) && (wordValuesSize > 0U) && (dataPtr != NULL) && (numBytes >= (wordValuesSize*4U)) )
    {
        res = true;
        for (uint32_t i = 0U; i < wordValuesSize; ++i)
        {
            uint32_t startIdx = i*4U;
            dataPtr[startIdx] = (uint8_t)((wordValues[i] >> 24U) & 0x000000FFU);
            dataPtr[startIdx + 1U] = (uint8_t)((wordValues[i] >> 16U) & 0x000000FFU);
            dataPtr[startIdx + 2U] = (uint8_t)((wordValues[i] >> 8U) & 0x000000FFU);
            dataPtr[startIdx + 3U] = (uint8_t)(wordValues[i] & 0x000000FFU);
        }

    }
    return res;
}

static inline void _switchUIntByteOrderInPlace(uint8_t* dataPtr, uint32_t numBytes)
{
    if (numBytes == 2U)
    {
        uint8_t tmp[2] = { dataPtr[1], dataPtr[0] };
        memcpy(dataPtr, tmp, 2U);
    }
    else if (numBytes == 4U)
    {
        uint8_t tmp[4] = { dataPtr[3], dataPtr[2], dataPtr[1], dataPtr[0] };
        memcpy(dataPtr, tmp, 4U);
    }
    else
    {
        // should never happen
    }
}

void ba_binUtilsSwitchUIntByteOrderInPlace(uint8_t* dataPtr, uint32_t numBytes)
{
    if (dataPtr != NULL)
    {
        _switchUIntByteOrderInPlace(dataPtr, numBytes);
    }
}

uint16_t ba_binUtilsSwitchUInt16ByteOrder(uint16_t data)
{
    uint16_t res = data;
    _switchUIntByteOrderInPlace((uint8_t*)&res, 2U);
    return res;
}

uint32_t ba_binUtilsSwitchUInt32ByteOrder(uint32_t data)
{
    uint32_t res = data;
    _switchUIntByteOrderInPlace((uint8_t*)&res, 4U);
    return res;
}

uint16_t ba_binUtilsGetUInt16FromBigEndian(const uint8_t* bigEndian, uint32_t numBytes)
{
    uint16_t res = 0U;
    if ((bigEndian != NULL) && (numBytes == 2U))
    {
        memcpy(&res, bigEndian, 2U);
        _switchUIntByteOrderInPlace((uint8_t*)&res, 2U);
    }
    return res;
}

uint32_t ba_binUtilsGetUInt32FromBigEndian(const uint8_t* bigEndian, uint32_t numBytes)
{
    uint32_t res = 0U;
    if ((bigEndian != NULL) && (numBytes == 4U))
    {
        memcpy(&res, bigEndian, 4U);
        _switchUIntByteOrderInPlace((uint8_t*)&res, 4U);
    }
    return res;
}
