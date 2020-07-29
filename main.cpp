

#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "pretty_printer.h"
#include "Gap.h"


void callback(const Gap::address_t                          peerAddr,
                           int8_t                           rssi,
                           bool                             isScanResponse,
                    GapAdvertisingParams::AdvertisingType_t type,
                           uint8_t                          advertisingDataLen,
                           const uint8_t                    *advertisingData) {
    printf("adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u\r\n",
           peerAddr[0], peerAddr[1], peerAddr[2], peerAddr[3], peerAddr[4], peerAddr[5], rssi, isScanResponse, type);
    printf("len = %u\r\n", advertisingDataLen);
    unsigned index = 0;
    for (; index < advertisingDataLen; index++) {
        printf("%02x ", advertisingData[index]);
    }
    printf("\r\n");
}



int main()
{

    ble_error_t ScanParameters(
        uint16_t interval = GapScanningParams::SCAN_INTERVAL_MAX,
        uint16_t window = GapScanningParams::SCAN_WINDOW_MAX,
        uint16_t timeout = 0,
        bool activeScanning = false
    );

    printf("Starting Bluetooth");

    ble_error_t setScanParameters(uint16_t interval);
    ble_error_t setScanParameters(uint16_t window);
    ble_error_t setScanParameters(uint16_t timeout);
    void setActiveScan(bool activeScanning);

    ble_error_t startScan(Gap::AdvertisementReportCallback_t callback);

    printf("Stopping scan");
    ble_error_t stopScan(void);
}
