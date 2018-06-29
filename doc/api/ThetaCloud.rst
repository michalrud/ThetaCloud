ThetaCloud
==========

.. glossary::
	
	ThetaCloud::Emit
		hello

	ThetaCloud::SensorReadHandler
		hello

	ThetaCloud::SensorWriteHandler
		hello

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

				SensorHandlerTokenPtr ThetaCloud::addReadHandler(const SensorReadHandler& handler)

		hello

	ThetaCloud::addWriteHandler()
		:Method signature:
			.. code-block:: cpp

				SensorHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const SensorWriteHandler& handler)

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

	SensorHandlerToken
	SensorHandlerTokenPtr
		hello
