
#ifndef IBEACON_SCANNER_H__
#define IBEACON_SCANNER_H__

#include "mbed.h"
#include "ble/BLE.h"
#include "Gap.h"
#include "AdvertisingDataParser.h"
#include "AdvertisingDataBuilder.h"

class Scanner : ble::Gap::EventHandler 
{
  public:
    struct iBeacon 
    {
        uint8_t uuid[16];
        uint16_t major;
        int16_t minor;
        int8_t tx_power;
        ble::rssi_t rssi;
    };

    Scanner(BLE &ble, events::EventQueue &event_queue);
    void start();
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params);
    void button_control(const iBeacon &ibeacon);
    static void printIBeacon(const iBeacon &ibeacon);

  private:
    BLE &_ble;
    events::EventQueue &_event_queue;
    int major_val, minor_val;
    DigitalOut floor_1, floor_2, floor_3, floor_4, floor_5, floor_6, floor_7, floor_8;
    DigitalOut _data_led, _scan_led;
    DigitalOut panelDownButton, panelUpButton, closeDoor, openDoor;
    bool parseIBeacon(ble::AdvertisingDataParser adv_data, iBeacon &ibeacon);

  protected:
    void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override;
    void onScanTimeout (const ble::ScanTimeoutEvent &event) override;
};
#endif /* IBEACON_SCANNER_H__ */