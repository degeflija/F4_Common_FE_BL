//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaMathUtils.c
//!
//! @author Tobias Fetzer
//!
//! @brief
//! See related header
//!

// related header
#include "BaMathUtils.h"

static const float32_t kCosTable[91];

uint32_t ba_mathUtilsPowUInt32(uint32_t base, uint32_t exponent)
{
    uint32_t result = 0U;
    if (exponent > 0U)
    {
        result = base;
        for (uint32_t i = 1; i < exponent; ++i)
        {
            uint32_t tmpResult = result * base;
            if (tmpResult >= result)
            {
                result = tmpResult;
            }
            else
            {
                // overflow
                result = 0U;
                break;
            }
        }
    }
    else if (base != 0U)
    {
        result = 1U;
    }
    else
    {
        // 0^0 is undefined
        result = 0U;
    }
    return result;
}

int32_t ba_mathUtilsPowInt32(int32_t base, int32_t exponent)
{
    int32_t result = 0;

    if (base == 0)
    {
        result = 0;
    }
    else if (base == 1)
    {
        result = 1;
    }
    else if (exponent > 0)
    {
        result = base;
        for (int32_t i = 1; i < exponent; ++i)
        {
            result *= base;
        }
    }
    else if (exponent == 0)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

uint16_t ba_mathUtilsSqrtUInt32(uint32_t value)
{
    uint32_t op = value;
    uint32_t res = 0U;

    // "one" starts at the highest power of four <= than the argument
    uint32_t one = 1U << 30U;  /* second-to-top bit set */
    while (one > op)
    {
        one >>= 2U;
    }

    while (one != 0U)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  2U * one;
        }
        res >>= 1U;
        one >>= 2U;
    }

    return (uint16_t)res;
}

float32_t ba_mathUtilsNormalizeAngleDeg(float32_t deg)
{
    float32_t res = deg;
    while (res >= 360.0f)
    {
        res -= 360.0f;
    }
    while (res < 0.0f)
    {
        res += 360.0f;
    }
    return res;
}

int32_t ba_mathUtilsNormalizeAngleDegInt32(int32_t deg)
{
    int32_t res = deg;
    while (res >= 360)
    {
        res -= 360;
    }
    while (res < 0)
    {
        res += 360;
    }
    return res;
}

int32_t ba_mathUtils360DegToRelativeDegInt32(int32_t deg)
{
    int32_t res = deg;
    while (res > 180)
    {
        res -= 360;
    }
    while (res < -180)
    {
        res += 360;
    }
    return res;
}

int8_t ba_mathUtilsRoundToInt8(float32_t value)
{
    int8_t res = 0;
    float32_t tmpRes = roundf(value);

    if (tmpRes >= (float32_t)BA_INT8_MAX)
    {
        res = BA_INT8_MAX;
    }
    else if (tmpRes <= (float32_t)BA_INT8_MIN)
    {
        res = BA_INT8_MIN;
    }
    else
    {
        res = (int8_t)tmpRes;
    }
    return res;
}

int16_t ba_mathUtilsRoundToInt16(float32_t value)
{
    int16_t res = 0;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)BA_INT16_MAX)
    {
        res = BA_INT16_MAX;
    }
    else if (tmpRes < (float32_t)BA_INT16_MIN)
    {
        res = BA_INT16_MIN;
    }
    else
    {
        res = (int16_t)tmpRes;
    }
    return res;
}

int32_t ba_mathUtilsRoundToInt32(float32_t value)
{
    int32_t res = 0;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)BA_INT32_MAX)
    {
        res = BA_INT32_MAX;
    }
    else if (tmpRes < (float32_t)BA_INT32_MIN)
    {
        res = BA_INT32_MIN;
    }
    else
    {
        res = (int32_t)tmpRes;
    }
    return res;
}

uint8_t ba_mathUtilsRoundToUInt8(float32_t value)
{
    uint8_t res = 0U;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)BA_UINT8_MAX)
    {
        res = BA_UINT8_MAX;
    }
    else if (tmpRes < 0.0f)
    {
        res = 0U;
    }
    else
    {
        res = (uint8_t)tmpRes;
    }
    return res;
}

uint16_t ba_mathUtilsRoundToUInt12(float32_t value)
{
    uint16_t res = 0;
    float32_t tmpRes = roundf(value);

    if (tmpRes >= 4094.5f)
    {
        res = 0xFFFU;
    }
    else if (tmpRes < 0.0f)
    {
        res = 0U;
    }
    else
    {
        res = (uint16_t)tmpRes;
    }
    return res;
}

uint16_t ba_mathUtilsRoundToUInt16(float32_t value)
{
    uint16_t res = 0;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)BA_UINT16_MAX)
    {
        res = BA_UINT16_MAX;
    }
    else if (tmpRes < 0.0f)
    {
        res = 0U;
    }
    else
    {
        res = (uint16_t)tmpRes;
    }
    return res;
}

uint32_t ba_mathUtilsRoundToUInt32(float32_t value)
{
    uint32_t res = 0U;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)BA_UINT32_MAX)
    {
        res = BA_UINT32_MAX;
    }
    else if (tmpRes < 0.0f)
    {
        res = 0U;
    }
    else
    {
        res = (uint32_t)tmpRes;
    }
    return res;
}

uint32_t ba_mathUtilsRoundToUInt24(float32_t value)
{
    uint32_t res = 0U;
    float32_t tmpRes = roundf(value);

    if (tmpRes > (float32_t)0xFFFFFFU)
    {
        res = 0xFFFFFFU;
    }
    else if (tmpRes < 0.0f)
    {
        res = 0U;
    }
    else
    {
        res = (uint32_t)tmpRes;
    }
    return res;
}

uint32_t ba_mathUtilsGetPrimeFactors(uint32_t value, uint32_t* factors, uint32_t factorsSize)
{
    uint32_t numFactors = 0U;

    if ((factors != NULL) && (factorsSize >= 32U))
    {
        uint32_t remValue = value;
        uint32_t div = 2U;

        while (remValue != 0U)
        {
            if ((remValue % div) != 0U)
            {
                ++div;
            }
            else
            {
                factors[numFactors++] = div;
                remValue = remValue / div;
                if (remValue == 1U)
                {
                    if (numFactors == 1U)
                    {
                        factors[numFactors++] = 1U;
                    }
                    break;
                }
            }
        }
    }

    return numFactors;
}

float32_t ba_mathUtilsFastCosDeg(float32_t valueDeg)
{
    int32_t idx = abs(ba_mathUtilsRoundToInt32(valueDeg)) % 360;
    if (idx <= 90)
    {
        return kCosTable[idx];
    }
    else if (idx <= 180)
    {
        return (-1.0f * kCosTable[180 - idx]);
    }
    else if (idx <= 270)
    {
        return (-1.0f * kCosTable[idx - 180]);
    }
    else
    {
        return kCosTable[360 - idx];
    }
}
#if 0
float32_t ba_mathUtilsFastSinDeg(float32_t valueDeg)
{
    return ba_mathUtilsFastCosDeg(90.0f - valueDeg);
}

// http://developer.download.nvidia.com/cg/index_stdlib.html
float32_t ba_mathUtilsFastACos(float32_t value)
{
    float32_t negate = 0.0f;
    if (value < 0.0f)
    {
        value *= -1.0f;
        negate = 1.0f;
    }

    float32_t ret = -0.0187293f;
    ret *= value;
    ret += 0.0742610f;
    ret *= value;
    ret -= 0.2121144f;
    ret *= value;
    ret += 1.5707288f;
    ret *= sqrtf(1.0f-value);
    ret -= 2.0f * negate * ret;
    return negate * 3.14159265358979f + ret;
}
#endif
static const float32_t kCosTable[91] =
{
    1.0f,
    0.999847695f,
    0.999390827f,
    0.998629535f,
    0.99756405f,
    0.996194698f,
    0.994521895f,
    0.992546152f,
    0.990268069f,
    0.987688341f,
    0.984807753f,
    0.981627183f,
    0.978147601f,
    0.974370065f,
    0.970295726f,
    0.965925826f,
    0.961261696f,
    0.956304756f,
    0.951056516f,
    0.945518576f,
    0.939692621f,
    0.933580426f,
    0.927183855f,
    0.920504853f,
    0.913545458f,
    0.906307787f,
    0.898794046f,
    0.891006524f,
    0.882947593f,
    0.874619707f,
    0.866025404f,
    0.857167301f,
    0.848048096f,
    0.838670568f,
    0.829037573f,
    0.819152044f,
    0.809016994f,
    0.79863551f,
    0.788010754f,
    0.777145961f,
    0.766044443f,
    0.75470958f,
    0.743144825f,
    0.731353702f,
    0.7193398f,
    0.707106781f,
    0.69465837f,
    0.68199836f,
    0.669130606f,
    0.656059029f,
    0.64278761f,
    0.629320391f,
    0.615661475f,
    0.601815023f,
    0.587785252f,
    0.573576436f,
    0.559192903f,
    0.544639035f,
    0.529919264f,
    0.515038075f,
    0.5f,
    0.48480962f,
    0.469471563f,
    0.4539905f,
    0.438371147f,
    0.422618262f,
    0.406736643f,
    0.390731128f,
    0.374606593f,
    0.35836795f,
    0.342020143f,
    0.325568154f,
    0.309016994f,
    0.292371705f,
    0.275637356f,
    0.258819045f,
    0.241921896f,
    0.224951054f,
    0.207911691f,
    0.190808995f,
    0.173648178f,
    0.156434465f,
    0.139173101f,
    0.121869343f,
    0.104528463f,
    0.087155743f,
    0.069756474f,
    0.052335956f,
    0.034899497f,
    0.017452406f,
    0.0f
};
