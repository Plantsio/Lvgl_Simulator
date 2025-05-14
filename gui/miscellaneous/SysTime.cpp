//
// Created by Gordon on 2024/6/19.
//

#include "SysTime.h"
#include "filepath.h"
#include "log.h"
#include <windows.h>
#define INET6_ADDRSTRLEN    48

#define MILLIS() GetTickCount64()

extern const std::string CONFIG_DIR;

SysTime::SysTime() : m_dump(path({CONFIG_DIR, "timeConf"}), {.offset=8, .dst=0, .isChina=true}) {}

bool SysTime::_initialize() {
//    m_dump.load(true);
//    esp_sntp_config_t config;
//    if (m_dump.data().isChina) {
//        config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(4,
//                                                        ESP_SNTP_SERVER_LIST(
//                                                                CONFIG_SNTP_TIME_SERVER_CN_1,
//                                                                CONFIG_SNTP_TIME_SERVER_CN_2,
//                                                                CONFIG_SNTP_TIME_SERVER_US,
//                                                                CONFIG_SNTP_TIME_SERVER_1));
//    } else {
//        config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(4,
//                                                        ESP_SNTP_SERVER_LIST(
//                                                                CONFIG_SNTP_TIME_SERVER_US,
//                                                                CONFIG_SNTP_TIME_SERVER_CN_1,
//                                                                CONFIG_SNTP_TIME_SERVER_CN_2,
//                                                                CONFIG_SNTP_TIME_SERVER_1));
//
//    }
//    config.start = false;                       // start SNTP service explicitly (after connecting)
//    config.server_from_dhcp = false;             // accept NTP offers from DHCP server, if any (need to enable *before* connecting)
//    config.renew_servers_after_new_IP = true;   // let esp-netif update configured SNTP server(s) after receiving DHCP lease
//    config.index_of_first_server = 0;           // updates from server num 1, leaving server 0 (from DHCP) intact
//    config.ip_event_to_renew = IP_EVENT_STA_GOT_IP;
//    config.sync_cb = SysTime::timeSyncCb; // only if we need the notification function
//    auto ret = esp_netif_sntp_init(&config);
//    if (ret == ESP_OK) {
//        return true;
//    } else {
//        log_w("time init failed with ret: %d", ret);
//        return false;
//    }
}

void SysTime::timeSyncCb(struct timeval *tv) {
    log_i("SysTime synced");
    SysTime::instance().m_time_synced = true;
    SysTime::dumpSntpServers();
}

void SysTime::sync() {
    if (!SysTime::synced()) {
        log_d("syncing time...");
        //esp_netif_sntp_start();
    }
}

uint32_t SysTime::unix() {
    time_t now;
    time(&now);
    return now;
}

uint32_t SysTime::seconds() {
    return (unsigned long) (MILLIS() / 1000ULL);
}

uint32_t SysTime::unixOrSeconds() {
    if (synced()) {
        return unix();
    } else {
        return seconds();
    }
}

uint32_t SysTime::millis() {
    return MILLIS();
}

bool SysTime::synced() {
    return SysTime::instance().m_time_synced;
}

bool SysTime::waitUntilSynced(uint32_t timeout) {
    //return esp_netif_sntp_sync_wait(timeout) == ESP_OK;
}

void SysTime::dumpSntpServers() {
    log_d("List of configured NTP servers:");

//    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i) {
//        if (esp_sntp_getservername(i)) {
//            log_d("server %d: %s", i, esp_sntp_getservername(i));
//        } else {
//            // we have either IPv4 or IPv6 address, let's print it
//            char buff[INET6_ADDRSTRLEN];
//            ip_addr_t const *ip = esp_sntp_getserver(i);
//            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != nullptr)
//                log_d("server %d: %s", i, buff);
//        }
//    }
}

bool SysTime::isUnix(uint32_t t) {
    return t > 1721636988;
}

uint32_t SysTime::toSeconds(uint32_t unix) {
    auto currentUnix = SysTime::unix();
    auto currentSeconds = SysTime::seconds();
    if (currentUnix < unix) {
        return currentSeconds;
    }
    int diff = currentUnix - unix;
    if (currentSeconds < diff) {
        return 0;
    }
    return currentSeconds - diff;
}

uint32_t SysTime::since(uint32_t t) {
    auto currentUnix = SysTime::unix();
    if (currentUnix < t) {
        return 0;
    }
    return currentUnix - t;
}

uint32_t SysTime::sinceMillis(uint32_t t) {
    auto currentTime = millis();
    if (currentTime<t){
        return 0;
    }
    return currentTime - t;
}
