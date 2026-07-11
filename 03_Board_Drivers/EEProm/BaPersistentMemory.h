//!
//! (c) Copyright Butterfly Avionics GmbH 2015
//!
//! @file BaPersistentMemory.h
//!
//! @author Tobias Fetzer
//!
//! @brief
//! A persistent memory module. The functions defined in this header are not to be
//! used directly within application. BaPersistentMemoryInterface shall be used instead.
//!

#ifndef BA_DRIVERS_MEMORY_BAPERSISTENTMEMORY_H_
#define BA_DRIVERS_MEMORY_BAPERSISTENTMEMORY_H_

// project headers
#include "BaDataTypes.h"

#define     EE_OK                   0


typedef struct
{
    uint32_t size;
    uint32_t pageSize;
    uint32_t pageCount;
}
BaPersistentMemoryDescription;

extern bool ba_persistentMemoryInit(uint32_t id);
extern bool ba_persistentMemoryDeInit(uint32_t id);

extern const BaPersistentMemoryDescription* ba_persistentMemoryDescription(uint32_t id);

extern bool ba_persistentMemoryEraseAll(uint32_t id, uint32_t numTries);
extern bool ba_persistentMemoryErasePage(uint32_t id, uint32_t page, uint32_t numTries);

extern bool ba_persistentMemoryWriteBytes(uint32_t id, uint32_t page, uint32_t addressInPage, uint32_t numTries, uint8_t* dataArray, uint32_t numBytes);

extern bool ba_persistentMemoryReadBytes(uint32_t id, uint32_t address, uint32_t numTries, uint8_t* dataArray, uint32_t numBytes);
extern bool ba_persistentMemoryReadPageBytes(uint32_t id, uint32_t page, uint32_t addressInPage, uint32_t numTries, uint8_t* dataArray, uint32_t numBytes);

#endif // BA_DRIVERS_MEMORY_BAPERSISTENTMEMORY_H_
