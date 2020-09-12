# NTUT-Rover-Mbed

This project showcases use of BLE Advertiser/Listener. We are using it to control elevator buttons by broadcasting through smartphones building and floor number.
We use the STM32Wb microcontroller to listen to broadcast signals. 
The smartphone sends out 31-bit address which consists of: the beacon type, special identifier(UUID), Major number(Building), Minor(Floor) and RSSI(signal strength)

We use the minor data recieved and feed it to the elevator relay.

