//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaValueMapping.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//!
//!

#ifndef BA_SHARED_CORE_BAVALUEMAPPING_H_
#define BA_SHARED_CORE_BAVALUEMAPPING_H_

// project headers
#include "BaDataTypes.h"

//! Value mapping from values to percent and back.
//! the value member shall always be ascending within a map
//! the percent value may be ascending or descending within a map
//! if range is exceeded, the corresponding min or max value is used by the mapping fucntions

typedef struct
{
    uint8_t value;
    uint8_t percentValue;
} BaUIntValueToPercentMapItem;

typedef struct
{
    int8_t value;
    uint8_t percentValue;
} BaIntValueToPercentMapItem;

extern uint8_t ba_valueMappingUIntValueToPercent(uint8_t value, const BaUIntValueToPercentMapItem* map, uint32_t mapSize);
extern uint8_t ba_valueMappingPercentToUIntValue(uint8_t valuePercent, const BaUIntValueToPercentMapItem* map, uint32_t mapSize);
extern uint8_t ba_valueMappingIncrPercValForUIntMap(uint8_t valuePercent, uint8_t increment, const BaUIntValueToPercentMapItem* map, uint32_t mapSize);
extern uint8_t ba_valueMappingDecrPercValForUIntMap(uint8_t valuePercent, uint8_t decrement, const BaUIntValueToPercentMapItem* map, uint32_t mapSize);
extern void ba_valueMapIncDecValPercForUIntMap(uint8_t* valuePercent, int8_t increment, const BaUIntValueToPercentMapItem* map, uint32_t mapSize);

extern uint8_t ba_valueMappingIntValueToPercent(int8_t value, const BaIntValueToPercentMapItem* map, uint32_t mapSize);
extern int8_t ba_valueMappingPercentToIntValue(uint8_t valuePercent, const BaIntValueToPercentMapItem* map, uint32_t mapSize);
extern uint8_t ba_valueMappingIncrPercValForIntMap(uint8_t valuePercent, uint8_t increment, const BaIntValueToPercentMapItem* map, uint32_t mapSize);
extern uint8_t ba_valueMappingDecrPercValForIntMap(uint8_t valuePercent, uint8_t decrement, const BaIntValueToPercentMapItem* map, uint32_t mapSize);
extern void ba_valueMapIncDecValPercForIntMap(uint8_t* valuePercent, int8_t increment, const BaIntValueToPercentMapItem* map, uint32_t mapSize);

#endif // BA_SHARED_CORE_BAVALUEMAPPING_H_
