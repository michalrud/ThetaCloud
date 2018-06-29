ThetaCloud
==========

.. glossary::
	
	ThetaCloud::Emit
		Function definition for callback that will be called by ThetaCloud every time a new data is available from
		devices. Can publish this data further or just print it using ``Serial``.
		
		:Signature:
			.. code-block:: cpp

				using Emit = std::function<void(const SensorData&)>
		:Parameter SensorData: Data read from sensor.
		:Example implementation:
			.. code-block:: cpp

				void handleSensorData(const SensorData& data)
				{
					Serial.println(data.name);
					Serial.println(data.value);
				}
		:Example implementation as a lambda function:
			.. code-block:: cpp

				auto handleSensorData = [](const SensorData& data)
				{
					Serial.println(data.name);
					Serial.println(data.value);
				};

	ThetaCloud::DeviceReadHandler
		Function definition for callback that will be called each time ThetaCloud requests new data to be read,
		if available. Currently each registered handler will be called once when :term:`ThetaCloud::tick()` is called.

		Handler doesn't have to do anything if no data is available, or it cannot be read for any reason.

		If data is available, callback provided in argument should be called with :term:`SensorData` containing the
		data from sensor. Callback may be called multiple times, if multiple data is received (for example, sensor
		provides both temperature and humidity information).

		Callback should not be stored for longer than the duration of handler execution.

		:Signature:
			.. code-block:: cpp

				using DeviceReadHandler = std::function<void(const Emit&)>;
		:Parameter Emit: Callback which should be used to provide data from sensor to the rest of the ThetaCloud.
		:Example implementation:
			.. code-block:: cpp

				void handler(const ThetaCloud::Emit& emit)
				{
					emit(SensorData{"MySensor", "My value"});
				}
		:Example implementation as a lambda function:
			.. code-block:: cpp

				auto handler = [](const ThetaCloud::Emit& emit)
				{
					emit(SensorData{"MySensor", "My value"});
				};

	ThetaCloud::DeviceWriteHandler
		Function definition for callback that will be called, when 

	ThetaCloud::init()
		:Method signature:
			.. code-block:: cpp

				void ThetaCloud::init()

		hello

	ThetaCloud::whenDataAvailable()
		:Method signature:
			.. code-block:: cpp

				void ThetaCloud::whenDataAvailable(const Emit& dataCallback)

		hello

	ThetaCloud::addReadHandler()
		:Method signature:
			.. code-block:: cpp

				DeviceHandlerTokenPtr ThetaCloud::addReadHandler(const DeviceReadHandler& handler)

		hello

	ThetaCloud::addWriteHandler()
		:Method signature:
			.. code-block:: cpp

				DeviceHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const DeviceWriteHandler& handler)

		hello

	ThetaCloud::write()
		:Method signature:
			.. code-block:: cpp

				void ThetaCloud::write(const SensorData& data) const

		hello

	ThetaCloud::tick()
		:Method signature:
			.. code-block:: cpp

				void ThetaCloud::tick()

		hello

	DeviceHandlerToken
	DeviceHandlerTokenPtr
		hello
