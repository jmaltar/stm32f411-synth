// Platform-specific clock implementation for the STM32F411 target.
// Provides the ticks_by_far() symbol declared by the engine in synth/utils.h.
// Resolution: milliseconds since boot, sourced from the STM32 HAL SysTick.

#include "main.h"  // STM32 HAL — provides HAL_GetTick()

#include <cstdint>

uint64_t ticks_by_far() {
    return static_cast<uint64_t>(HAL_GetTick());
}
