This project showcases use of BLE Advertiser/Listener. We are using it to control elevator buttons by broadcasting through smartphones, the building and floor numbers.

STM32Wb microcontroller is used to listen to broadcast signals. 
The smartphone sends out 31-bit address which consists of: the beacon type, special identifier(UUID), Major number(Building), Minor(Floor) and RSSI(signal strength)

Minor data recieved is fed to the elevator relay.

