#include "helper.hpp"
#include <AdvertisingDataTypes.h>

#include "pretty_printer.h"
using namespace ble;

Scanner::Scanner(BLE &ble, events::EventQueue &event_queue)
: _ble(ble),
_data_led(LED3,1),
_scan_led(LED1,0),
floor_1(PB_11),
floor_2(PC_5),
floor_3(PB_2),
floor_4(PE_4),
floor_5(PA_13),
floor_6(PA_14),
floor_7(PC_14),
floor_8(PC_15),
panelUpButton(PC_4),
panelDownButton(PB_8),
closeDoor(PC_2),
openDoor(PC_3),
_event_queue(event_queue){}


// Setting up the BLE
void Scanner::start()
{
    _ble.gap().setEventHandler(this);
    _ble.init(this,&Scanner::on_init_complete); 

    _event_queue.dispatch_forever();
}
  

// Setting up scan parameters and scan duration
void Scanner::on_init_complete(BLE::InitializationCompleteCallbackContext *params)
{
    if(params->error != BLE_ERROR_NONE)
    {
        printf("Bluetooth Init Failed!\r\n");
        return;
    }

    _ble.gap().setScanParameters(
        ScanParameters(
            phy_t::LE_1M,   // scan on the 1M PHY
            scan_interval_t(160),
            scan_window_t(40),
            true      )
      );
    _ble.gap().startScan();
    printf("Starting Scan... \r\n");
}

bool Scanner::parseIBeacon(ble::AdvertisingDataParser adv_data, iBeacon &ibeacon)
{
    if (!adv_data.hasNext())  {return false;}
    AdvertisingDataParser::element_t flag = adv_data.next();
    if (flag.type != ble::adv_data_type_t::FLAGS) {return false;}
    if (flag.value[0] != 0x06) {return false;}
    if (!adv_data.hasNext()) {return false;}
    AdvertisingDataParser::element_t packet_data = adv_data.next();
    if (packet_data.type != ble::adv_data_type_t::MANUFACTURER_SPECIFIC_DATA)  {return false;}
    if (packet_data.value.size() != 25) {return false;}
    // Company ID 0x004C
    if (packet_data.value[0] != 0x4C) {return false;}
    if (packet_data.value[1] != 0x00) {return false;}
    // Beacon Type
    if (packet_data.value[2] != 0x02) {return false;}
    // Beacon Length
    if (packet_data.value[3] != 0x15) {return false;}
    // UUID
    for (uint8_t i = 0; i < 16; i++)
    {
        ibeacon.uuid[i] = packet_data.value[i+4];
    }
    // Major Number
    ibeacon.major = (packet_data.value[20] << 8) | packet_data.value[21];
    // Minor Number
    ibeacon.minor = (packet_data.value[22] << 8) | packet_data.value[23];
    // Tx Power
    ibeacon.tx_power = packet_data.value[24];
    return true;
}

// Getting the advertisement. 
void Scanner::onAdvertisingReport(const AdvertisingReportEvent &event)
{
    _scan_led = !_scan_led;
    AdvertisingDataParser adv_data(event.getPayload());
    iBeacon beacon;
    if (parseIBeacon(adv_data, beacon))
    {
        printf("Found Beacon!\r\n");
        beacon.rssi = event.getRssi();
        printIBeacon(beacon);
        button_control(beacon);
    }
}

void Scanner::onScanTimeout (const ble::ScanTimeoutEvent &event)
{
    printf("Scan Timeout!\r\n");
}

// Attaching the payload values to elevator buttons
void Scanner::button_control(const iBeacon &ibeacon)
{  
    if(ibeacon.major == 0x0) {floor_1 = 1;}
    // if(ibeacon.major == 2) {floor_2 = 1;}
    // if(ibeacon.major == 3) {floor_3 = 1;}
    // if(ibeacon.major == 4) {floor_4 = 1;}
    // if(ibeacon.major == 5) {floor_5 = 1;}
    // if(ibeacon.major == 6) {floor_6 = 1;}
    // if(ibeacon.major == 7) {floor_7 = 1;}
    // if(ibeacon.major == 8) {floor_8 = 1;}

    // if(ibeacon.uuid[0] == 0xe4) {panelDownButton = 1;}
    // if(ibeacon.uuid[0] == 0xe3) {panelUpButton = 1;}

    // if(ibeacon.uuid[0] == 0xe5){openDoor = 1;}
    // if(ibeacon.uuid[0] == 0xe6) {closeDoor = 1;}
}

void Scanner::printIBeacon(const iBeacon &ibeacon)
{
    printf("****iBeacon****\r\n");
    printf("RSSI: %d dBm\r\n", ibeacon.rssi);
    printf("UUID: %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\r\n",
    ibeacon.uuid[0], ibeacon.uuid[1], ibeacon.uuid[2],  ibeacon.uuid[3],  
    ibeacon.uuid[4], ibeacon.uuid[5], ibeacon.uuid[6],  ibeacon.uuid[7], 
    ibeacon.uuid[8], ibeacon.uuid[9], ibeacon.uuid[10], ibeacon.uuid[11], 
    ibeacon.uuid[12], ibeacon.uuid[13], ibeacon.uuid[14], ibeacon.uuid[15]);
    printf("Major: 0x%X \r\n", ibeacon.major);
    printf("Minor: %d \r\n", ibeacon.minor);
    printf("RSSI at 1m: %d dBm\r\n", ibeacon.tx_power);
}
