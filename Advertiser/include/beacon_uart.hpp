#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "USBSerial.h"
#include "pretty_printer.h"

const static char DEVICE_NAME[] = "CAMPUS ROVER";

class BeaconUart : ble::Gap::EventHandler 
{
public:
    BeaconUart(BLE &ble, events::EventQueue &event_queue);
    void vcpCallback(void);
protected:
    
private:
    USBSerial _vcp;
    bool _vcp_called;
    events::EventQueue &_event_queue;
    DigitalOut _led_connected;
    DigitalOut _led_command;
    BLE &_ble;
    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;
    union Payload {
        // Raw data of the payload.
        uint8_t raw[25];
        struct {
            // Beacon manufacturer identifier.
            uint16_t companyID;
            // Packet ID; Equal to 2 for an iBeacon.
            uint8_t ID;
            // Length of the remaining data presents in the payload.
            uint8_t len;
            // Beacon UUID.
            uint8_t proximityUUID[16];
            // Beacon Major group ID.
            uint16_t majorNumber;
            // Beacon minor ID.
            uint16_t minorNumber;
            // Tx power received at 1 meter; in dBm.
            uint8_t txPower;
        };
        Payload(
            const uint8_t *uuid,
            uint16_t majNum,
            uint16_t minNum,
            uint8_t transmitPower,
            uint16_t companyIDIn
        ) : companyID(companyIDIn),
            ID(0x02),
            len(0x15),
            majorNumber(__REV16(majNum)),
            minorNumber(__REV16(minNum)),
            txPower(transmitPower)
        {
            memcpy(proximityUUID, uuid, 16);
        }
    };
    void ledBlink(DigitalOut &led);
    void ledFlash(DigitalOut &led, std::chrono::milliseconds interval);
    void vcpHandler();
    void bleStart();
    void onInitComplete(BLE::InitializationCompleteCallbackContext *params);
    void startAdvertising(const uint16_t building, const uint16_t dest_floor);
};