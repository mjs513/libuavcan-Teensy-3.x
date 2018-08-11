# libuavcan-Teensy 3.x

Portable reference implementation of the [UAVCAN protocol stack](http://uavcan.org) in C++ designed for use with Teensy 3.x series MCUs using the Arduino IDE with Teensduino.

UAVCAN is a lightweight protocol designed for reliable communication in aerospace and robotic applications via CAN bus.

## Documentation

* [UAVCAN website](http://uavcan.org)
* [UAVCAN discussion group](https://groups.google.com/forum/#!forum/uavcan)
* [Libuavcan overview](http://uavcan.org/Implementations/Libuavcan/)
* [List of platforms officially supported by libuavcan](http://uavcan.org/Implementations/Libuavcan/Platforms/)
* [Libuavcan tutorials](http://uavcan.org/Implementations/Libuavcan/Tutorials/)

## Library usage

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
