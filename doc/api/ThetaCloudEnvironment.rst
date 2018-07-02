ThetaCloudEnvironment
=====================

ThetaCloudEnvironment is an optional, external board equipped with
``MICS-VZ-89F`` and `MPL3115A2`` sensors, allowing it to detect CO2, VOC, altitude, temperature and pressure.

.. _MPL3115A2:

Optional usage of MPL3115A2 sensor
----------------------------------

If the `Adafruit MPL3115A2 Library <https://github.com/adafruit/Adafruit_MPL3115A2_Library>`_
is present, board can provide additional readings of Altitude, Pressure and Temperature.
To enable this functionality:

#. Install the Adafruit MPL3115A2 Library in Arduino IDE,
#. Uncomment the ``USE_MPL3115A2`` feature flag in ``ThetaCloudEnvironment.h``.

Publishes
---------

:co2:
	Current level of CO2 as detected by ``MICS-VZ-89F``.

	:units:	``[ppm]``
	:format: ``float``

:voc:
	Current level of Volatile Organic Compounds (VOCs), as detected by
	``MICS-VZ-89F``.

	:units:	``[ppb]``
	:format: ``float``

.. note::
	Following readings may not always be available. See :ref:`mpl3115A2`

:altitude:
	Detected altitude.

	:units:	``[m]``
	:format:	``float``

:pressure:
	Pressure.

	:units:	``[Pa]``
	:format: ``float``

:envTemp:
	Temperature detected by the extension board.

	:units:	``[C]``
	:format: ``float``

Consumes
--------

This module does not consume any data.
