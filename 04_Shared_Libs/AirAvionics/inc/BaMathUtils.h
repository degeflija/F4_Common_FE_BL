//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaMathUtils.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//! Mathematical Audio functions.
//!

#ifndef BA_SHARED_CORE_BAMATHUTILS_H_
#define BA_SHARED_CORE_BAMATHUTILS_H_

// standard libs

// suppress multiple include warnings for stddef, which is included by multiple std libs
//lint -e537 -e451
#include <embedded_math.h>   // NULL-pointer type
//lint +e537 -e451

#include <stdlib.h>

// project headers
#include "BaDataTypes.h"

static inline uint32_t ba_mathUtilsMaxUInt(uint32_t a, uint32_t b) { return ( (a > b) ? a : b ); }
static inline uint64_t ba_mathUtilsMaxUInt64(uint64_t a, uint64_t b) { return ( (a > b) ? a : b ); }

static inline uint32_t ba_mathUtilsMinUInt(uint32_t a, uint32_t b) { return ( (a < b) ? a : b ); }
static inline uint64_t ba_mathUtilsMinUInt64(uint64_t a, uint64_t b) { return ( (a < b) ? a : b ); }

static inline int32_t ba_mathUtilsMaxInt(int32_t a, int32_t b) { return ( (a > b) ? a : b ); }
static inline int32_t ba_mathUtilsMinInt(int32_t a, int32_t b) { return ( (a < b) ? a : b ); }

static inline float32_t ba_mathUtilsMaxFloat(float32_t a, float32_t b) { return ( (a > b) ? a : b ); }
static inline float32_t ba_mathUtilsMinFloat(float32_t a, float32_t b) { return ( (a < b) ? a : b ); }

static inline uint64_t ba_mathUtilsSafeDiffUInt64(uint64_t greater, uint64_t smaller) { return ( (greater > smaller) ? (greater - smaller) : 0ULL ); }


static inline float32_t ba_mathUtilsLimitFloat(float32_t v, float32_t min, float32_t max)
{
    return ( (v < min) ? min : (v > max ? max : v) );
}

static inline int8_t ba_mathUtilsLimitToInt8(int32_t a)
{
    if (a < BA_INT8_MIN) return BA_INT8_MIN;
    else if (a > BA_INT8_MAX) return BA_INT8_MAX;
    else return (int8_t)a;
}

static inline uint8_t ba_mathUtilsLimitToUInt8(int32_t a)
{
    if (a < 0) return 0U;
    else if (a > BA_UINT8_MAX) return BA_UINT8_MAX;
    else return (uint8_t)a;
}

static inline int16_t ba_mathUtilsLimitToInt16(int32_t a)
{
    if (a < BA_INT16_MIN) return BA_INT16_MIN;
    else if (a > BA_INT16_MAX) return BA_INT16_MAX;
    else return (int16_t)a;
}

static inline uint16_t ba_mathUtilsLimitToUInt16(int32_t a)
{
    if (a < 0U) return 0U;
    else if (a > BA_UINT16_MAX) return BA_UINT16_MAX;
    else return (uint16_t)a;
}

static inline int32_t ba_mathUtilsLimitInt32(int32_t v, int32_t min, int32_t max)
{
    return ( (v < min) ? min : (v > max ? max : v) );
}

static inline float32_t ba_mathUtilsRad2deg(float32_t rad) { return (rad * 57.2957795f); }
static inline float32_t ba_mathUtilsDeg2rad(float32_t deg) { return (deg * 0.0174532925f); }
static inline bool ba_mathUtilsAlmostEqualFloat(float32_t v1, float32_t v2, float32_t epsilon) { return (fabsf(v1 - v2) <= epsilon); }

static inline uint32_t ba_mathUtilsAbsUIntDelta(uint32_t a, uint32_t b) { return ( (a > b) ? (a - b) : (b - a) ); }

static inline float32_t ba_mathUtilsSquare(const float32_t x) { return (x * x); }
static inline float32_t ba_mathUtilsInnerProd2(const float32_t* const x, const float32_t* const y) { return x[0] * y[0] + x[1] * y[1]; }

//!
//! \brief Integer based power function.
//!
//! \param base     The base for the power function.
//! \param exponent The exponent, which must be positive
//!
//! \return pow(base, exponent), or 0 if the value could not be computed due to invalid arguments or an overflow
//!
extern uint32_t ba_mathUtilsPowUInt32(uint32_t base, uint32_t exponent);
// may overflow, negative exponents results are 0
extern int32_t ba_mathUtilsPowInt32(int32_t base, int32_t exponent);

// fast integer implementation of sqrt
// 144 cycles against 2000 cycles of double implementation and 579 cycles of float implementation
extern uint16_t ba_mathUtilsSqrtUInt32(uint32_t value);

extern float32_t ba_mathUtilsNormalizeAngleDeg(float32_t deg);
extern int32_t ba_mathUtilsNormalizeAngleDegInt32(int32_t deg);
extern int32_t ba_mathUtils360DegToRelativeDegInt32(int32_t deg);

extern int8_t ba_mathUtilsRoundToInt8(float32_t value);
extern int16_t ba_mathUtilsRoundToInt16(float32_t value);
extern int32_t ba_mathUtilsRoundToInt32(float32_t value);
extern uint8_t ba_mathUtilsRoundToUInt8(float32_t value);
extern uint16_t ba_mathUtilsRoundToUInt12(float32_t value);
extern uint16_t ba_mathUtilsRoundToUInt16(float32_t value);
extern uint32_t ba_mathUtilsRoundToUInt24(float32_t value);
extern uint32_t ba_mathUtilsRoundToUInt32(float32_t value);

// factorSize must be >= 32
extern uint32_t ba_mathUtilsGetPrimeFactors(uint32_t value, uint32_t* factors, uint32_t factorsSize);

// lookup-table based fast cosinus/sinus calculation
// input value is rounded to full degrees
// maximum error is < 0.0088f
extern float32_t ba_mathUtilsFastCosDeg(float32_t valueDeg);
extern float32_t ba_mathUtilsFastSinDeg(float32_t valueDeg);

// fast acos calculation
// input value is between -1 and 1
// output value is between 0 and PI
extern float32_t ba_mathUtilsFastACos(float32_t value);

#endif // BA_SHARED_CORE_BAMATHUTILS_H_
