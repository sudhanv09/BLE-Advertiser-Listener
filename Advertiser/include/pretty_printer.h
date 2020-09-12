#include <mbed.h>
#include "ble/BLE.h"

enum UartState{INIT, WAIT_CONNECTING, CONNECTED, RECEIVED_DATA};

inline void printMacAddress()
{

}

inline void printVcpState(const UartState state)
{
    static UartState connect_state = INIT;
    static UartState data_state = INIT;
    switch(state){
    case WAIT_CONNECTING:
        if (connect_state != WAIT_CONNECTING)
        {
            printf("VCP disconneced\r\n");
            connect_state = WAIT_CONNECTING;
        }
        break;
    case CONNECTED:
        if (connect_state != CONNECTED)
        {
            printf("VCP conneced\r\n");
            connect_state = CONNECTED;
        }
        break;
    case RECEIVED_DATA:
        printf(" --> ");
        break;
    default:
        break;
    }
}

inline void print_error(ble_error_t error, const char* msg)
{
    printf("%s: ", msg);
    switch(error) {
        case BLE_ERROR_NONE:
            printf("BLE_ERROR_NONE: No error");
            break;
        case BLE_ERROR_BUFFER_OVERFLOW:
            printf("BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been aborted");
            break;
        case BLE_ERROR_NOT_IMPLEMENTED:
            printf("BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the target HW");
            break;
        case BLE_ERROR_PARAM_OUT_OF_RANGE:
            printf("BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range");
            break;
        case BLE_ERROR_INVALID_PARAM:
            printf("BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid");
            break;
        case BLE_STACK_BUSY:
            printf("BLE_STACK_BUSY: The stack is busy");
            break;
        case BLE_ERROR_INVALID_STATE:
            printf("BLE_ERROR_INVALID_STATE: Invalid state");
            break;
        case BLE_ERROR_NO_MEM:
            printf("BLE_ERROR_NO_MEM: Out of Memory");
            break;
        case BLE_ERROR_OPERATION_NOT_PERMITTED:
            printf("BLE_ERROR_OPERATION_NOT_PERMITTED");
            break;
        case BLE_ERROR_INITIALIZATION_INCOMPLETE:
            printf("BLE_ERROR_INITIALIZATION_INCOMPLETE");
            break;
        case BLE_ERROR_ALREADY_INITIALIZED:
            printf("BLE_ERROR_ALREADY_INITIALIZED");
            break;
        case BLE_ERROR_UNSPECIFIED:
            printf("BLE_ERROR_UNSPECIFIED: Unknown error");
            break;
        case BLE_ERROR_INTERNAL_STACK_FAILURE:
            printf("BLE_ERROR_INTERNAL_STACK_FAILURE: internal stack failure");
            break;
        case BLE_ERROR_NOT_FOUND:
            printf("BLE_ERROR_NOT_FOUND");
            break;
        default:
            printf("Unknown error");
    }
    printf("\r\n");
}
