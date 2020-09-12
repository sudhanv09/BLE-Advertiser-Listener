#include "beacon_uart.hpp"

void BeaconUart::vcpCallback(void)
{
    _vcp_called = true;
}

BeaconUart::BeaconUart(BLE &ble, events::EventQueue &event_queue)
: _vcp(true, 0x1f00, 0x2012, 0x0001),
_vcp_called(false),
_led_connected(LED1, 1),
_led_command(LED2, 0),
_ble(ble),
_event_queue(event_queue),
_adv_data_builder(_adv_buffer)
{
    _vcp.attach(this, &BeaconUart::vcpCallback);
    _vcp.init();
    bleStart();
    _event_queue.call_every(1s, this, &BeaconUart::vcpHandler);
    _event_queue.dispatch_forever();
}

void BeaconUart::ledBlink(DigitalOut &led)
{
    led = !led;
}

void BeaconUart::ledFlash(DigitalOut &led, std::chrono::milliseconds interval)
{
    led = 1;
    ThisThread::sleep_for(interval);
    led = 0;
}

void BeaconUart::vcpHandler()
{
    while (!_vcp.connected())
    {
        printVcpState(WAIT_CONNECTING);
        ledBlink(_led_connected);
        ThisThread::sleep_for(1s);
    }
    printVcpState(CONNECTED);
    _led_connected = 1;
    if (_vcp_called)
    {
        uint8_t buf_size = _vcp.available();
        printVcpState(RECEIVED_DATA);
        uint16_t building = 0;
        uint16_t dest_floor = 0;
        bool read_building = true;
        uint8_t shift = 0x00;
        for (uint8_t i=0; i < buf_size; i++)
        {
            char c = _vcp.getc();
            if (c == '\r' || c == '\n') {continue;}
            if (c == ',')
            {
                read_building = false;
                shift = 0x00;
                continue;
            }
            if (read_building)
            {
                building << shift;
                building |= c-48;
                shift += 8;
            }
            else
            {
                dest_floor << shift;
                dest_floor |= c-48;
                shift += 8;
            }
        }
        printf("building: %d, floor: %d\r\n", building, dest_floor);
        ledFlash(_led_command, 50ms);
        _vcp_called = false;
        startAdvertising(building, dest_floor);
    }
}

void BeaconUart::bleStart()
{
    _ble.gap().setEventHandler(this);
    _ble.init(this, &BeaconUart::onInitComplete);
}

void BeaconUart::onInitComplete(
    BLE::InitializationCompleteCallbackContext *params)
{
    if (params->error != BLE_ERROR_NONE) {
        printf("Ble initialization failed.");
        return;
    }
    startAdvertising(0, 0);
    printf("BLE start advertising\r\n");
}

void BeaconUart::startAdvertising(
    const uint16_t building, 
    const uint16_t dest_floor)
{
    ble::AdvertisingParameters adv_parameters(
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t(ble::millisecond_t(100))
    );
    _adv_data_builder.setFlags();
    static const uint8_t uuid[] = { 0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4,
                                    0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61 };
    uint16_t major_number = building;
    uint16_t minor_number = dest_floor;
    uint16_t tx_power     = 0xC8;
    uint16_t comp_id      = 0x004C;

    Payload ibeacon(uuid, major_number, minor_number, tx_power, comp_id);

    _adv_data_builder.setManufacturerSpecificData(ibeacon.raw);

    ble_error_t error = _ble.gap().setAdvertisingParameters(
        ble::LEGACY_ADVERTISING_HANDLE,
        adv_parameters
    );

    if (error) {
        print_error(error, "_ble.gap().setAdvertisingParameters() failed");
        return;
    }

    error = _ble.gap().setAdvertisingPayload(
        ble::LEGACY_ADVERTISING_HANDLE,
        _adv_data_builder.getAdvertisingData()
    );

    if (error) {
        print_error(error, "_ble.gap().setAdvertisingPayload() failed");
        return;
    }

    /* Start advertising */

    error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error) {
        print_error(error, "_ble.gap().startAdvertising() failed");
        return;
    }
}