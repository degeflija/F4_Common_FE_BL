//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaBinaryUtils.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//! Size and signedness indicating types for a 32-bit machine.
//!

#ifndef BA_SHARED_CORE_BABINARYUTILS_H_
#define BA_SHARED_CORE_BABINARYUTILS_H_

// project headers
#include "BaDataTypes.h"

extern void ba_binUtilsSetUInt8Bit(uint32_t bitIdx, uint8_t* bitfield);
extern void ba_binUtilsResetUInt8Bit(uint32_t bitIdx, uint8_t* bitfield);
extern bool ba_binUtilsGetUInt8Bit(uint32_t bitIdx, uint8_t bitfield);

extern void ba_binUtilsSetUInt16Bit(uint32_t bitIdx, uint16_t* bitfield);
extern bool ba_binUtilsIsUInt16BitSet(uint32_t bitIdx, uint16_t bitfield);
extern uint32_t ba_binUtilsGetUInt16BitValue(uint32_t bitIdx, uint16_t bitfield);

extern void ba_binUtilsSetUInt32Bit(uint32_t bitIdx, uint32_t* bitfield);
extern void ba_binUtilsResetUInt32Bit(uint32_t bitIdx, uint32_t* bitfield);
extern bool ba_binUtilsIsUInt32BitSet(uint32_t bitIdx, uint32_t bitfield);
extern uint32_t ba_binUtilsGetUInt32BitValue(uint32_t bitIdx, uint32_t bitfield);

// numBytes must be >= 4, array is encoded BigEndian i.e. the MSB of the UInt32 is in dataPtr[0]
extern bool ba_binUtilsConvertUInt32ToByteArray(uint32_t value, uint8_t* dataPtr, uint32_t numBytes);
// array is encoded BigEndian, i.e. the MSB of the UInt32 comes first
extern bool ba_binUtilsConvertUInt32ArrayToByteArray(const uint32_t* wordValues, uint32_t wordValuesSize, uint8_t* dataPtr, uint32_t numBytes);

// numBytes must be 2 or 4
extern void ba_binUtilsSwitchUIntByteOrderInPlace(uint8_t* dataPtr, uint32_t numBytes);

extern uint16_t ba_binUtilsSwitchUInt16ByteOrder(uint16_t data);
extern uint32_t ba_binUtilsSwitchUInt32ByteOrder(uint32_t data);

// numBytes must be 2
extern uint16_t ba_binUtilsGetUInt16FromBigEndian(const uint8_t* bigEndian, uint32_t numBytes);
// numBytes must be 4
extern uint32_t ba_binUtilsGetUInt32FromBigEndian(const uint8_t* bigEndian, uint32_t numBytes);

#endif // BA_SHARED_CORE_BABINARYUTILS_H_
