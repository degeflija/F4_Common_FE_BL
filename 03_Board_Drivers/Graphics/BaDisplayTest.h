//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaDisplayTest.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//! ...
//!

#ifndef BA_SHARED_GRAPHIC_BADISPLAYTEST_H_
  #define BA_SHARED_GRAPHIC_BADISPLAYTEST_H_

	// project headers
	
	#include "BaDisplay.h"
	#include "BaGraphicFunctions.h"

  #ifdef __cplusplus
    extern "C" {
  #endif

  void ba_displayTestDrawCrosstalkPattern(uint8_t colA, uint8_t colB, uint8_t colC);
  void ba_displayTestDrawGrayScalePattern(void);

  #ifdef __cplusplus
    }
  #endif

#endif // BA_SHARED_GRAPHIC_BADISPLAYTEST_H_
