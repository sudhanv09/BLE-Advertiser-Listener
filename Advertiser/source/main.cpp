#include <events/mbed_events.h>
#include "mbed.h"
#include "ble/BLE.h"
#include "beacon_uart.hpp"

static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

/** Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

// main() runs in its own thread in the OS
int main()
{
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_events);
    printf("Wait for VCP connect\r\n");
    BeaconUart bu(ble, event_queue);
    printf("end program\r\n");
    return 0;
}

