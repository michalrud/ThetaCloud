ThetaCloud Introduction
=======================

What is ThetaCloud?
-------------------

ThetaCloud is an ESP8266 device for simple, DIY home automation.
It consists of:

* An ESP8266-based main board with extension socket, that can be programmed and powered over USB,
* Extension modules that can be plugged into the main board with possibility of designing new ones,
* Arduino library that can be used for designing actual systems using ThetaCloud, with example implementation basing on MQTT being provided.

.. _installing:

Basic Usage
-----------

Most of the time the user will want to use this code as an Arduino IDE library. To do so, take the .zip file containing
the code and add the library using the Arduino IDE: ``Sketch -> Include Library -> Add .ZIP Library``.

After that the library is ready to be included in your sketches using regular ``#include`` directives. 

Running the tests
-----------------

ThetaCloud library comes with the test suite. To run it perform the regular CMake build:

.. code-block:: sh
	
	mkdir build
	cd build
	cmake ..
	make -j4

``make`` will automatically run all tests during the build process.

Building the library using CMake is not required, as Arduino IDE builds its libraries on its own. Therefore, if you
don't plan on running the test suite, you don't need CMake.

Used libraries
--------------

 * Parts derived from `Sodaq_SHT2x <https://github.com/SodaqMoja/Sodaq_SHT2x>`_ library.
 * Parts derived from `isl29023 Arduino library <https://github.com/perelloc/isl29023>`_.
