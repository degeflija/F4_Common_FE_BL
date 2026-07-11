//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaGeoUtils.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//!
//!

#ifndef BA_SHARED_CORE_BAGEOUTILS_H_
#define BA_SHARED_CORE_BAGEOUTILS_H_

// project headers
#include "BaMathUtils.h"

static inline float32_t geoUtilsAltMSLToHAE(float32_t altMslM, float32_t geoidHeightAboveWGSEllipsoidM) { return (altMslM + geoidHeightAboveWGSEllipsoidM); }
static inline float32_t geoUtilsHAEToAltMSL(float32_t haeM, float32_t geoidHeightAboveWGSEllipsoidM) { return (haeM - geoidHeightAboveWGSEllipsoidM); }

extern float32_t ba_geoUtilsGetDistanceM(const BaLocationCoordinate* loc1, const BaLocationCoordinate* loc2);

// fast distance estimation with no restrictions
// - with less mathematical functions
// - usually around 2 times faster than ba_geoUtilsGetDistanceM (on PC platform)
// - maximum error below
extern float32_t ba_geoUtilsGetEstimatedDistanceM(const BaLocationCoordinate* loc1, const BaLocationCoordinate* loc2);

// fast distance estimation for short distances, away from poles
// - without complex mathematical functions
// - around 8 times faster than ba_geoUtilsGetDistanceM (on PC platform)
// - should be used between +/-80° latitude only
// - should be used for relatively short distances only (max error around 5% for results less than 1200km)
extern float32_t ba_geoUtilsGetEstimatedShortDistanceM(const BaLocationCoordinate* loc1, const BaLocationCoordinate* loc2);

extern int32_t ba_geoUtilsRoundedLatitude(float32_t lat);
extern int32_t ba_geoUtilsRoundedLongitude(float32_t lon);
extern void ba_geoUtilsGetDestPoint(const BaLocationCoordinate* start, float32_t distM, float32_t directionDeg, BaLocationCoordinate* result);

extern int32_t ba_geoUtilsGetOfficialSunrise(int32_t day, int32_t month, int32_t year, const BaLocationCoordinate* loc);
extern int32_t ba_geoUtilsGetOfficialSunset(int32_t day, int32_t month, int32_t year, const BaLocationCoordinate* loc);
extern int32_t ba_geoUtilsGetCivilSunrise(int32_t day, int32_t month, int32_t year, const BaLocationCoordinate* loc);
extern int32_t ba_geoUtilsGetCivilSunset(int32_t day, int32_t month, int32_t year, const BaLocationCoordinate* loc);

#endif // BA_SHARED_CORE_BAGEOUTILS_H_
