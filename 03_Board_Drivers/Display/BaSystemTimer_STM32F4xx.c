//!
//! (c) Copyright Butterfly Avionics GmbH
//!
//! @file BaSystemTimer_STM32F4xx.c
//!
//! @author Tobias Fetzer
//!
//! @brief
//!
//!

// related header
#include "BaSystemTimer.h"
#include "stm32f4xx.h"

#if 0 // obsolete

// system counter (counting since device start)
static volatile uint64_t s_systemCounterUs = 0ULL;

void ba_systemTimerInit(void)
{
}

void ba_systemTimerDelayMs(uint32_t delay)
{
    ba_systemTimerDelayUs(((uint64_t)delay)*1000ULL);
}

void ba_systemTimerDelayUs(uint64_t delay)
{
    uint64_t expiration = s_systemCounterUs + delay;
    while (s_systemCounterUs < expiration);
}

uint32_t ba_systemTimerGetMs(void)
{
    return (uint32_t)((s_systemCounterUs/1000ULL) & 0xFFFFFFFFULL);
}

uint64_t ba_systemTimerGetUs(void)
{
    return s_systemCounterUs;
}

// SysTick interrupt handler
void SysTick_Handler(void)
{
    ++s_systemCounterUs;
}

#endif
