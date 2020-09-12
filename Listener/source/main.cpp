#include "helper.hpp"

static EventQueue event_queue(8 * EVENTS_EVENT_SIZE);

void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
  event_queue.call(callback(&(context->ble), &BLE::processEvents));
}

int main(void){
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(schedule_ble_processing);
    
    Scanner scanner(ble, event_queue);
    scanner.start();
    return 0;
}
