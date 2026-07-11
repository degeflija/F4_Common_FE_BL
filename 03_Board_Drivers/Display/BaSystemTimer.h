//!
//! (c) Copyright Butterfly Avionics GmbH
//!
//! @file BaSystemTimer.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//!
//!

#ifndef BA_DRIVERS_TIMER_BASYSTEMTIMER_H_
#define BA_DRIVERS_TIMER_BASYSTEMTIMER_H_

// project headers
#include "BaDataTypes.h"

extern void ba_systemTimerInit(void);

extern void ba_systemTimerDelayMs(uint32_t delay);
extern void ba_systemTimerDelayUs(uint64_t delay);

extern uint32_t ba_systemTimerGetMs(void);
extern uint64_t ba_systemTimerGetUs(void);

#endif // BA_DRIVERS_TIMER_BASYSTEMTIMER_H_
