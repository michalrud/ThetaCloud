Flashing ThetaCloud HW
======================

ThetaCloud can be programmed using Arduino IDE. As ESP8266 is the base of ThetaCloud, a lot of useful information
can be found in `the ESP8266 Arduino Core documentation <https://arduino-esp8266.readthedocs.io/en/latest/>`_.

Quick environment setup
-----------------------

#. You need Arduino IDE v. 1.6.8 or newer.
#. Add new Additional Board Manager URL in Arduino IDE:
	::

		http://arduino.esp8266.com/stable/package_esp8266com_index.json

#. Install ``esp8266`` platform in the Board Manager,
#. Select *Generic ESP8266 Module* as the current board.

.. _jumper-setup:

Jumper setup
------------

Main ThetaCloud board is equipped with a set of pins which can be shorted using
jumpers to change the boot behavior of the board.

.. image:: /_static/pics/theta-main-jumpers.png

.. _flash-mode:

Booting ThetaCloud into Flash Mode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ThetaCloud main board needs to be booted into the *flash mode* in order for you to be able to flash an application
on it. To do so, three jumpers need to be used on pins marked as **1**, **2** and **3** on the :ref:`jumper-setup` diagram.

.. note:: When ThetaCloud is booted into the flash mode, it will not execute the stored program.

Booting ThetaCloud with UART
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Access to UART is useful when debugging an application using a tool like
Serial Monitor provided with the Arduino IDE. To enable UART and boot the
application flashed on the board, use jumpers on pins marked **2** and **3**
on the :ref:`jumper-setup` diagram.

.. note:: Remember to leave pins **1** unshorted, as shorting them will cause
	ThetaCloud to boot into the :ref:`Flash mode <flash-mode>`.
