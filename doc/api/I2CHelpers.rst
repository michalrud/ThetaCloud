I2CHelpers
==========

	
.. cpp:class:: WriteToI2C

	Convenience class for writing data to I2C.

	Will automatically call ``Wire.beginTransmission()`` and ``Wire.endTransmission()``.

	.. note::
		This class doesn't prevent the programmer from creating multiple different
		instances of this class at the same time, it will essentially cause more than one
		``beginTransmission()`` call at the same time. While the Arduino reference doesn't seem
		to forbid it, this may cause problems.

	:Usage example: Simple write:

		.. literalinclude:: /doc/uml/simplewrite.utxt

		.. code-block:: cpp

			// Writes two bytes, 0x12 and 0x34, to I2C address 0x67,
			// and immediately end the transmission.
			WriteToI2C(0x67).write(0x12).write(0x34);

	:Usage example: Write with delayed ``endTransmission()``:

		.. literalinclude:: /doc/uml/delayedwrite.utxt

		.. code-block:: cpp

			{
				auto writer = WriteI2C(0x67);	// beginTransmission is called here
				delay(10);
				writer.write(0x12);
				delay(20);
				writer.write(0x34);
				// writer goes out of scope, endTransmission is called
			}

	.. cpp:function:: WriteToI2C(uint8_t address)

		Initiates an I2C transmission to a provided address.

	.. cpp:function:: ~WriteI2C()

		Calls ``endTransmission()``. Therefore, the transmission will be "opened" for as long
		as the object instance is existing.

	.. cpp:function:: WriteToI2C& write(uint8_t data)

		Writes a data using the currently opened transmission.

.. cpp:function:: template<uint8_t N>\
		std::array<uint8_t, N> ReadFromI2C(uint8_t address)

	Convenience method for reading data from I2C.

	Will try to read the ``N`` bytes for 100ms. If the read fails, returns an error value,
	otherwise returns an ``std::array`` with data that was read.

	Under the hood uses the Arduino's ``Wire`` library, especially ``Wire.requestFrom()`` and ``Wire.read()``.

	:Param N: Number of bytes (``uint8_t``) that will be read from I2C.
	:Param address: I2C address, from which bytes will be requested.
	:Return when success: ``std::pair``, where first element is an ``std::array`` of ``uint8_t`` values
						  that were received from the device. Second element will be ``false``.
	:Return when failure: ``std::pair``, where the second element is set to ``true``, and the first element should
						  be ignored.
	:Usage example:
		.. code-block:: cpp

			// Will read 4 bytes from 0x12 I2C address
			auto data = ReadFromI2C<4>(0x12);
			if (data.second)
				Serial.println("Error!");
			else
				Serial.print("Data read:");
				for (const auto& readByte : data.first)
					Serial.print(readByte);
				Serial.println();
