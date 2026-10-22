#pragma once
#include <cstdint>
#include <cstdio>
#include <esp_timer.h>
#include <esp_system.h>

inline uint64_t pdo128_rx_uptime_ms()
{
    return static_cast<uint64_t>(esp_timer_get_time() / 1000ULL);
}
