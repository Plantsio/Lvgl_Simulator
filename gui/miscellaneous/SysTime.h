//
// Created by Gordon on 2024/6/19.
//

#ifndef FIRMWARE_TIME_H
#define FIRMWARE_TIME_H

#include <cstdint>
#include <memory>
#include "Dump.h"
#include "Initializable.h"

#define DAY_SECONDS     3600 * 24

/**
 * should be instantiated and initialized before wifi connection
 */
class SysTime: public Initializable{
    //region Singleton
public:
    static SysTime &instance() {
        static std::shared_ptr<SysTime> instance(new SysTime());
        return *instance;
    }

    SysTime(const SysTime &) = delete;

    SysTime(SysTime &&) = delete;

    SysTime &operator=(const SysTime &) = delete;

    SysTime &operator=(SysTime &&) = delete;
    //endregion

public:
    typedef struct {
        uint16_t offset;
        uint16_t dst;
        bool isChina;
    } SysTimeConfig;

public:
    SysTime();

    bool _initialize() override;

    static void sync();

public:
    static uint32_t unix();

    static bool isUnix(uint32_t t);

    static uint32_t toSeconds(uint32_t unix);

    static uint32_t since(uint32_t t);

    static uint32_t sinceMillis(uint32_t t);

    static uint32_t unixOrSeconds();

    static uint32_t seconds();

    static uint32_t millis();

    static bool synced();

    static bool waitUntilSynced(uint32_t timeout);

    static void timeSyncCb(struct timeval *tv);

public:
    static void dumpSntpServers();
private:
    bool m_time_synced = false;
    Dump<SysTimeConfig> m_dump;
};

#endif //FIRMWARE_TIME_H
