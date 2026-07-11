//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaValueMapping.c
//!
//! @author Tobias Fetzer
//!
//! @brief
//! See related header
//!

// related header
#include "BaValueMapping.h"

uint8_t ba_valueMappingUIntValueToPercent(uint8_t value, const BaUIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t valuePercent = map[mapSize - 1U].percentValue;
    for (uint32_t i = 0; i < mapSize; ++i)
    {
        if (map[i].value >= value)
        {
            valuePercent = map[i].percentValue;
            break;
        }
    }
    return valuePercent;
}

uint8_t ba_valueMappingPercentToUIntValue(uint8_t valuePercent, const BaUIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t value = map[mapSize - 1U].value;
    if (map[mapSize - 1U].percentValue > map[0].percentValue)
    {
        for (uint32_t i = 0; i < mapSize; ++i)
        {
            if (map[i].percentValue >= valuePercent)
            {
                value = map[i].value;
                break;
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < mapSize; ++i)
        {
            if (map[i].percentValue <= valuePercent)
            {
                value = map[i].value;
                break;
            }
        }
    }
    return value;
}

uint8_t ba_valueMappingIncrPercValForUIntMap(uint8_t valuePercent, uint8_t increment, const BaUIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t value = 100U;
    uint8_t minTargetValue = valuePercent + increment;
    if (minTargetValue < 100U)
    {
        if (map[mapSize - 1U].percentValue > map[0].percentValue)
        {
            for (uint32_t i = 0; i < mapSize; ++i)
            {
                if (map[i].percentValue >= minTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
        else
        {
            for (uint32_t i = (mapSize - 1U); i > 0U; --i)
            {
                if (map[i].percentValue >= minTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
    }
    return value;
}

uint8_t ba_valueMappingDecrPercValForUIntMap(uint8_t valuePercent, uint8_t decrement, const BaUIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t value = 0U;
    if (valuePercent > decrement)
    {
        uint8_t maxTargetValue = valuePercent - decrement;
        if (map[mapSize - 1U].percentValue > map[0].percentValue)
        {
            for (uint32_t i = (mapSize - 1U); i > 0U; --i)
            {
                if (map[i].percentValue <= maxTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
        else
        {
            for (uint32_t i = 0; i < mapSize; ++i)
            {
                if (map[i].percentValue <= maxTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
    }
    return value;
}

void ba_valueMapIncDecValPercForUIntMap(uint8_t* valuePercent, int8_t increment, const BaUIntValueToPercentMapItem* map, uint32_t mapSize)
{
    if ((valuePercent != NULL) && (increment != 0))
    {
        if (*valuePercent > 100U)
        {
            *valuePercent = 100U;
        }
        else if (increment > 0)
        {
            *valuePercent = ba_valueMappingIncrPercValForUIntMap(*valuePercent, (uint8_t)increment, map, mapSize);
        }
        else // increment is negative here
        {
            if (increment > -100)
            {
                // -100 < increment <= 0
                int32_t absIncrement = ((int32_t)increment) * (-1);
                *valuePercent = ba_valueMappingDecrPercValForUIntMap(*valuePercent, ((uint8_t)absIncrement), map, mapSize);
            }
            else
            {
                *valuePercent = 0U;
            }
        }
    }
}

uint8_t ba_valueMappingIntValueToPercent(int8_t value, const BaIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t valuePercent = map[mapSize - 1U].percentValue;
    for (uint32_t i = 0; i < mapSize; ++i)
    {
        if (map[i].value >= value)
        {
            valuePercent = map[i].percentValue;
            break;
        }
    }
    return valuePercent;
}

int8_t ba_valueMappingPercentToIntValue(uint8_t valuePercent, const BaIntValueToPercentMapItem* map, uint32_t mapSize)
{
    int8_t value = map[mapSize - 1U].value;
    if (map[mapSize - 1U].percentValue > map[0].percentValue)
    {
        for (uint32_t i = 0; i < mapSize; ++i)
        {
            if (map[i].percentValue >= valuePercent)
            {
                value = map[i].value;
                break;
            }
        }
    }
    else
    {
        for (uint32_t i = 0; i < mapSize; ++i)
        {
            if (map[i].percentValue <= valuePercent)
            {
                value = map[i].value;
                break;
            }
        }
    }
    return value;
}

uint8_t ba_valueMappingIncrPercValForIntMap(uint8_t valuePercent, uint8_t increment, const BaIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t value = 100U;
    uint8_t minTargetValue = valuePercent + increment;
    if (minTargetValue < 100U)
    {
        if (map[mapSize - 1U].percentValue > map[0].percentValue)
        {
            for (uint32_t i = 0; i < mapSize; ++i)
            {
                if (map[i].percentValue >= minTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
        else
        {
            for (uint32_t i = (mapSize - 1U); i > 0U; --i)
            {
                if (map[i].percentValue >= minTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
    }
    return value;
}

uint8_t ba_valueMappingDecrPercValForIntMap(uint8_t valuePercent, uint8_t decrement, const BaIntValueToPercentMapItem* map, uint32_t mapSize)
{
    uint8_t value = 0U;
    if (valuePercent > decrement)
    {
        uint8_t maxTargetValue = valuePercent - decrement;
        if (map[mapSize - 1U].percentValue > map[0].percentValue)
        {
            for (uint32_t i = (mapSize - 1U); i > 0U; --i)
            {
                if (map[i].percentValue <= maxTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
        else
        {
            for (uint32_t i = 0; i < mapSize; ++i)
            {
                if (map[i].percentValue <= maxTargetValue)
                {
                    value = map[i].percentValue;
                    break;
                }
            }
        }
    }
    return value;
}

void ba_valueMapIncDecValPercForIntMap(uint8_t* valuePercent, int8_t increment, const BaIntValueToPercentMapItem* map, uint32_t mapSize)
{
    if ((valuePercent != NULL) && (increment != 0))
    {
        if (*valuePercent > 100U)
        {
            *valuePercent = 100U;
        }
        else if (increment > 0)
        {
            *valuePercent = ba_valueMappingIncrPercValForIntMap(*valuePercent, (uint8_t)increment, map, mapSize);
        }
        else // increment is negative here
        {
            if (increment > -100)
            {
                // -100 < increment <= 0
                int32_t absIncrement = ((int32_t)increment) * (-1);
                *valuePercent = ba_valueMappingDecrPercValForIntMap(*valuePercent, ((uint8_t)absIncrement), map, mapSize);
            }
            else
            {
                *valuePercent = 0U;
            }
        }
    }
}

