ThetaCloud I2C Scanner
======================

Scans for all devices connected to the I2C bus on the main ThetaCloud board.

Publishes
---------

:i2c-devices:
	Result of the I2C device scan.

	:format: List of hexadecimal address list delimeted by commas.
		Example: ``0x12, 0x34``

Consumes
--------

This module does not consume any data.
