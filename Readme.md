# Bluetooth-Fencing-Scoring-System

Notes before we start:
1. This system DOES NOT GROUND, bell guard hits will register as will touches on the strip obviously
2. No guarantee of timing accuracy, should be about right but I have not tested how accurate it is

Suggested materials:

  1. [Adafruit ESP32](https://www.adafruit.com/product/3269) - $15 x2 (need one per box)
  2. [Solderable breadboard](https://www.adafruit.com/product/1609) - $5 x2 (need one per box)
  3. [Piezo buzzer](https://www.adafruit.com/product/160) - $1.50 x2 (need one per box)
  4. [5mm LED](https://www.adafruit.com/product/4203) - $5
  5. 330 ohm resistor
  6. [German epee socket](https://www.absolutefencinggear.com/af-master-series-german-epee-socket.html) - $11 x2 (need one per box)
  7. Solder/Soldering iron
  8. Small portable charger with short(3") usb->micro usb cable x2
  9. Some sort of protective housing to put the circuit in, I modeled and 3d printed one but a cardboard box would work just as well. Make sure the Micro usb port is visible from the outside.
## Building the circuit:
<pre>

<img src="./circuit_diagram.png" align="left" width="500px"/>

</pre>

1. Place ESP32 board into breadboard with usb connector at the edge, solder header pins to breadboard
2. Wire positive pin of LED to pin A1 or GPIO pin 25(same pin) ([pinout found here](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/pinouts)
3. Wire negative pin of LED to positive speaker pin and 330 ohm resistor, negative speaker pin and resistor should end at ground.
4. Wire 5v out pin on board to A line in the epee socket, Wire b line in socket to D9 pin on board


## Building the program:
1. Go to tools and set board to ESP32 dev module, set port to the port where you board is plugged in.
2. We first must find the MAC address of the ESP32 board, using the included get_mac.ino program. The MAC address will be output to the serial monitor, you must manually convert it to the correct format seen in Fencer1.ino and Fencer2.ino (0xYY where YY is the corresponding 2 characters in the mac address).
3. Set uint8_t broadcastAddress at line 27 in Fencer1.ino to the found MAC address of the board you are using for Fencer2.ino and set uint8_t broadcastAddress at line 27 in Fencer2.ino to the MAC address of the board for Fencer1.ino
4. Download Fencer1.ino and Fencer2.ino to there respective boards. Use body cord to connect to weapon and test. I use a small portable phone charger plugged into the arduino's mircro usb port for power while in use.

See this [tutorial](https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/) for explanation of code.

### Helpful links:
1. See this [tutorial](https://randomnerdtutorials.com/esp-now-esp32-arduino-ide/) for explanation of code.
2. [ESP32 board overview](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather)
