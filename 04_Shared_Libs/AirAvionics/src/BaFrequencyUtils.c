//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaFrequencyUtils.c
//!
//! @author Tobias Fetzer
//!
//! @brief
//! See related header
//!

// related header
#include "BaFrequencyUtils.h"

static bool validateLastTwoDigitsOf833KHz(uint32_t lastTwoKHzDigits);

bool ba_freqUtilsValidateFreq(uint32_t frequencyKHz, bool raster25khz, bool raster833khz)
{
    bool isValid = true;
    uint32_t mhzValue = frequencyKHz / 1000U;

    if ((mhzValue > 136U) || (mhzValue < 118U))
    {
        isValid = false;
    }
    else
    {
        uint32_t khzValueToCheck = frequencyKHz % 100U;

        bool isValid25kHz = ((khzValueToCheck % 25U) == 0U);
        bool isValid833kHz = validateLastTwoDigitsOf833KHz(khzValueToCheck);

        isValid = (isValid25kHz && raster25khz) || (isValid833kHz && raster833khz);
    }

    return isValid;
}

bool ba_freqUtilsDecreaseMHz(uint32_t* frequencyMHz)
{
    bool freqDecreased = false;

    if (frequencyMHz == NULL)
    {
        // should not happen, nothing to do
    }
    else if (*frequencyMHz > 118U)
    {
        *frequencyMHz = (*frequencyMHz) - 1U;
        freqDecreased = true;
    }
    else
    {
        freqDecreased = false;
    }

    return freqDecreased;
}

bool ba_freqUtilsDecreaseKHz(uint32_t* frequencyKHz, bool raster25khz, bool raster833kHz, bool largeStep)
{
    bool frequencyDecreased = false;

    // validate input
    if (frequencyKHz == NULL)
    {
        // should never happen, nothing to do
    }
    else
    {
        uint32_t tmpFreqKHz = *frequencyKHz;

        if (raster25khz && raster833kHz)
        {
            // combined raster
            if (tmpFreqKHz <= 5U)
            {
                tmpFreqKHz = 0U;
            }
            else if (tmpFreqKHz > 990U)
            {
                // should never happen, fail safe
                tmpFreqKHz = 990U;
            }
            else if (largeStep)
            {
                // use 25kHz raster
                tmpFreqKHz = (tmpFreqKHz - 1U) / 25U;
                tmpFreqKHz *= 25U;
            }
            else
            {
                // safe way to get down to next modulo 5 value
                tmpFreqKHz = (tmpFreqKHz - 1U) / 5U;
                tmpFreqKHz *= 5U;
                // skip unused values
                bool isValid25KHz = ((tmpFreqKHz % 25U) == 0U);
                bool isValid833KHz = validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U);
                while ((!isValid25KHz)  && (!isValid833KHz))
                {
                    tmpFreqKHz -= 5U;
                    isValid25KHz = ((tmpFreqKHz % 25U) == 0U);
                    isValid833KHz = validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U);
                }
            }
        }
        else if (raster25khz)
        {
            if (tmpFreqKHz <= 25U)
            {
                tmpFreqKHz = 0U;
            }
            else if (tmpFreqKHz > 975U)
            {
                // should never happen, fail safe
                tmpFreqKHz = 975U;
            }
            else
            {
                tmpFreqKHz = (tmpFreqKHz - 1U) / 25U;
                tmpFreqKHz *= 25U;
            }
        }
        else if (raster833kHz)
        {
            if (tmpFreqKHz <= 10U)
            {
                tmpFreqKHz = 5U;
            }
            else if (tmpFreqKHz > 990U)
            {
                // should never happen, fail safe
                tmpFreqKHz = 990U;
            }
            else if (largeStep)
            {
                // tmpFreqKHz = (tmpFreqKHz - 6U) / 100U;
                // tmpFreqKHz *= 100U;
                // tmpFreqKHz += 5U;

                tmpFreqKHz = (tmpFreqKHz - 6U) / 25U;
                tmpFreqKHz *= 25U;
                tmpFreqKHz += 5U;
            }
            else
            {
                // safe way to get down to next modulo 5 value
                tmpFreqKHz = (tmpFreqKHz - 1U) / 5U;
                tmpFreqKHz *= 5U;
                // skip unused values
                while (!validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U))
                {
                    tmpFreqKHz -= 5U;
                }
            }
        }
        else
        {
            // nothing to do, no raster active, should never happen
        }

        if (tmpFreqKHz != (*frequencyKHz))
        {
            (*frequencyKHz) = tmpFreqKHz;
            frequencyDecreased = true;
        }
    }
    return frequencyDecreased;
}

bool ba_freqUtilsIncreaseMHz(uint32_t* frequencyMHz)
{
    bool frequencyIncreased = false;

    // validate input
    if (frequencyMHz == NULL)
    {
        // should never happen, nothing to do
    }
    else if (*frequencyMHz < 136U)
    {
        *frequencyMHz = (*frequencyMHz) + 1U;
        frequencyIncreased = true;
    }
    else
    {
        // nothing to do
    }

    return frequencyIncreased;
}

bool ba_freqUtilsIncreaseKHz(uint32_t* frequencyKHz, bool raster25khz, bool raster833kHz, bool largeStep)
{
    bool frequencyIncreased = false;

    // validate input
    if (frequencyKHz == NULL)
    {
        // should never happen, nothing to do
    }
    else
    {
        uint32_t tmpFreqKHz = *frequencyKHz;

        if (raster25khz && raster833kHz)
        {
            if (tmpFreqKHz >= 985U)
            {
                // set to maximum
                if (largeStep)
                {
                    tmpFreqKHz = 975U;
                }
                else
                {
                    tmpFreqKHz = 990U;
                }
            }
            else if (largeStep)
            {
                // use 25kHz raster
                if (tmpFreqKHz >= 950U)
                {
                    tmpFreqKHz = 975U;
                }
                else
                {
                    tmpFreqKHz = (tmpFreqKHz / 25U) + 1U;
                    tmpFreqKHz *= 25U;
                }
            }
            else
            {
                // safe way to get up to next modulo 5 value
                tmpFreqKHz = (tmpFreqKHz / 5U) + 1U;
                tmpFreqKHz *= 5U;
                // skip unused values
                bool isValid25KHz = ((tmpFreqKHz % 25U) == 0U);
                bool isValid833KHz = validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U);
                while ((!isValid25KHz)  && (!isValid833KHz))
                {
                    tmpFreqKHz += 5U;
                    isValid25KHz = ((tmpFreqKHz % 25U) == 0U);
                    isValid833KHz = validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U);
                }
            }
        }
        else if (raster25khz)
        {
            if (tmpFreqKHz >= 950U)
            {
                tmpFreqKHz = 975U;
            }
            else
            {
                tmpFreqKHz = (tmpFreqKHz / 25U) + 1U;
                tmpFreqKHz *= 25U;
            }
        }
        else if (raster833kHz)
        {
            if (tmpFreqKHz < 5U)
            {
                tmpFreqKHz = 5U;
            }
            else if (tmpFreqKHz >= 985U)
            {
                tmpFreqKHz = 990U;
            }
            else if (largeStep)
            {
                tmpFreqKHz = (tmpFreqKHz / 25U) + 1U;
                tmpFreqKHz *= 25U;
                tmpFreqKHz += 5U;

                if (tmpFreqKHz > 990U)
                {
                    tmpFreqKHz = 990U;
                }
            }
            else
            {
                // safe way to get up to next modulo 5 value
                tmpFreqKHz = (tmpFreqKHz / 5U) + 1U;
                tmpFreqKHz *= 5U;
                // skip unused values
                while (!validateLastTwoDigitsOf833KHz(tmpFreqKHz % 100U))
                {
                    tmpFreqKHz += 5U;
                }
            }
        }
        else
        {
            // nothing to do, no raster active, should never happen
        }

        if (tmpFreqKHz != (*frequencyKHz))
        {
            (*frequencyKHz) = tmpFreqKHz;
            frequencyIncreased = true;
        }
        else
        {
            // nothing to do
        }
    }

    return frequencyIncreased;
}

uint32_t ba_freqUtilsPackFreqTo12Bit(uint32_t frequencyKHz)
{
    uint32_t compressedFrequency = 0;

    uint32_t value = frequencyKHz - 118000U;
    // save last digit (which can be either 0 or 5) in bit 0
    if ((value % 10U) == 5U)
    {
        // set last bit
        compressedFrequency |= 0x00000001U;
    }
    else
    {
        // nothing to do, last bit is already 0
    }

    // remove last digit from value
    value /= 10U;
    // from here: ((value >= 0) && (value < 1900)) => value needs 11 bits for storage
    // store value in bits 11-1
    compressedFrequency |= ((value & 0x7FFU) << 1);
    // add a one, to remove 0 from the values for valid frequencies
    compressedFrequency += 1U;

    return compressedFrequency;
}

uint32_t ba_freqUtilsUnpackFreqFrom12Bit(uint32_t compressedFrequency)
{
    uint32_t freqKhz = 0;

    if (compressedFrequency > 0U)
    {
        compressedFrequency -= 1U;

        // process the last bit state
        if ((compressedFrequency & 0x00000001U) == 0x00000001U)
        {
            freqKhz = 5U;
        }
        else
        {
            // nothing to do
        }

        freqKhz += ((compressedFrequency >> 1) * 10U);
        freqKhz += 118000U;

        if (!ba_freqUtilsValidateFreq(freqKhz, true, true))
        {
            freqKhz = 0;
        }
        else
        {
            // nothing to do
        }
    }
    else
    {
        // nothing to do, frequency is 0
    }

    return freqKhz;
}

static bool validateLastTwoDigitsOf833KHz(uint32_t lastTwoKHzDigits)
{
    bool valid = false;

    if ((lastTwoKHzDigits % 5U) != 0U)
    {
        // nothing to do, invalid
    }
    else if ( (lastTwoKHzDigits == 0U) ||
              (lastTwoKHzDigits == 20U) ||
              (lastTwoKHzDigits == 25U) ||
              (lastTwoKHzDigits == 45U) ||
              (lastTwoKHzDigits == 50U) ||
              (lastTwoKHzDigits == 70U) ||
              (lastTwoKHzDigits == 75U) ||
              (lastTwoKHzDigits == 95U) )
    {
        // nothing to do, invalid
    }
    else
    {
        valid = true;
    }

    return valid;
}
