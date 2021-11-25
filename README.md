# digital HMI scale with rfid tagging and sending data to thikspeak server.

It consists of a nodemcu microcontroller, which takes weight measurements taking into account rfid tags to add a 
certain quantity of a material to the inventory, the nodmcu control uses an HMI interface and also the data taken 
is stored in a thinkspeak server (https://thingspeak.com/channels/1546302).

### Pre-requisitos 📋

1.Nodemcu V3 (1).
```
Any microcontroller can be used, only the pin layout must be taken into account.
```
2.Matriz led 8x8 (2).

3.Pushbuttons(6).

### Installation 🔧

Save the .ino file on the arduino.

## Built with 🛠️

* [LedControl](https://github.com/wayoda/LedControl) - library for the control of led arrays


## Authors ✒️

* **Richard Hernández** - * Working together * - (https://github.com/Richardshm/)
* **Dayber Mogollon** - * Working together * - (dmj.driver@hotmail.com)


## License 📄

This project is licensed under the GNU avr-gcc toolchain, GCC ARM Embedded toolchain, avr-libc, avrdude, bossac, openOCD and code from Processing and Wiring.
