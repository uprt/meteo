#### What is it
This app is a receiver, parser and csv-saver for data from no-name Chinese weather station.

The weather station was bought somewhere in China market and presented to me by my friend.
It has no brand name, no model, no other signs or codes :) It was working well and measuring temperature and humidity, and when I disassembled it I found some chip and RX/TX lines on the board with soldered contacts. After connecting it to the serial port using the UART-RS232 level converter I saw some data on the port and tried to reverse-engineer the protocol. The protocol was a bit funny with some magic constants (or simply I didn't get what does it mean), really weird number encoding format and simple checksum.

This utility was made long time ago to receive, filter and save data to CSV-file, that can be opened in Excel or LibreOffice Calc to make plots, or display it on the webpage to monitor conditions remotely.
Not sure if it will be useful for anybody (because it is hard to detect the exact model or manufacturer of the station), but let it be here for educational purposes :)

#### How to build
```
cmake ./
make
```

#### Command line options
```
-v (--verbose): print additional log information to console
-p (--port): COM-port to communicate
-s (--speed): COM-port speed (e.g. 9600)
-b (--stopbits): set stopbits count (1 or 2)
-r (--parity): setparity mode:  0 - none, 1 - odd, 2 - even
-f (--file): file path to save data
-i (--protocol): communication protocol to use (0 = 'Type1')
-t (--timeout): 'silence timeout' in sec. to reopen port if no data
```
