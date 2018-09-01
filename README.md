# libuavcan-Teensy 3.x

Portable reference implementation of the [UAVCAN protocol stack](http://uavcan.org) in C++ designed for use with Teensy 3.x series MCUs using the Arduino IDE with Teensduino.

UAVCAN is a lightweight protocol designed for reliable communication in aerospace and robotic applications via CAN bus.

The teensy driver and examples are taken from the UAVcan website and tum-phoenix (https://github.com/tum-phoenix/drive_teensy_main and https://github.com/tum-phoenix/drive_teensy_libuavcan) and modified appropriately.

## Documentation

* [UAVCAN website](http://uavcan.org)
* [UAVCAN discussion group](https://groups.google.com/forum/#!forum/uavcan)
* [Libuavcan overview](http://uavcan.org/Implementations/Libuavcan/)
* [List of platforms officially supported by libuavcan](http://uavcan.org/Implementations/Libuavcan/Platforms/)
* [Libuavcan tutorials](http://uavcan.org/Implementations/Libuavcan/Tutorials/)
* [Teensy UAVcan Forum Thread](https://forum.pjrc.com/threads/53394-UAVcan-for-Teensy-3-x-Proof-of-Concept)

## Library usage

Copy the library into the Arduino libraries folder and run examples as you would normally using the Arduino IDE.

### Dependencies

* Python 2.7 or 3.3 or newer

Note that this reporitory includes [Pyuavcan](http://uavcan.org/Implementations/Pyuavcan) as a submodule.
Such inclusion enables the library to be built even if pyuavcan is not installed in the system.
The following components will be installed:

* Libuavcan headers and the static library
* Generated DSDL headers
* Libuavcan DSDL compiler (a Python script named `libuavcan_dsdlc`)
* Libuavcan DSDL compiler's support library (a Python package named `libuavcan_dsdl_compiler`)

Note that Pyuavcan (an implementation of UAVCAN in Python) will not be installed.
You will need to install it separately if you intend to use the Libuavcan's DSDL compiler in your applications.

### Driver

The driver is based on the Collins80 Flexcan library as modified in the thread found on this thread: https://forum.pjrc.com/threads/45017...light=nmea2000.

### Issues resolved (history on getting this working)

* [Subscribing to vendor specific messages ](https://groups.google.com/forum/#!topic/uavcan/gK6vNSfmQG8)
* [Compiler errors on missing files in uavcan/protocol directory ](https://github.com/UAVCAN/libuavcan/issues/157)
* [ReceivedDataStructure - can't seem to figure out how to make it work  ](https://github.com/tum-phoenix/drive_teensy_main/issues/4)
* [Not reading Subscribed ImuData ](https://github.com/tum-phoenix/drive_teensy_main/issues/3)
* [Printf revisited? ](https://forum.pjrc.com/threads/53423-Printf-revisited)

### CHANGE LOG
9/1/2018 - Merged development Branch into Master.  The merge brings in tonton81's new and improbed IFCT CAN Library.  It also updates examples to include how to use built in functions to send messages at a specific rate.
