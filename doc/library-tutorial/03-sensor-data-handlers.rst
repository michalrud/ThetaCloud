Handling data from sensors
==========================

As a next step, let's print out to the serial console all the
data from our sensors. To do so, we need to implement
:term:`ThetaCloud::Emit` and call :term:`ThetaCloud::whenDataAvailable()`
to register it:

.. code-block:: cpp
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>
	#include <ThetaCloudRelay.h>
	#include <ThetaCloudHumidity.h>
	#include <ThetaCloudEnvironment.h>
	#include <ThetaCloudI2CScanner.h>

	void handleNewData(const SensorData& data)
	{
		Serial.print(data.getName());
		Serial.print(" - ");
		Serial.println(data.getValue());
	}

	void setup()
	{
		thetaCloud.whenDataAvailable(handleNewData);
		thetaCloud.init();
		thetaCloudLight.init();
		thetaCloudRelay.init();
		thetaCloudHumidity.init();
		thetaCloudEnvironment.init();
		thetaCloudI2CScanner.init();
	}

	void loop()
	{
		thetaCloud.tick();
	}

In the next part of the tutorial, we'll learn
:doc:`how to publish data for the sensors <04-publishing-data-to-sensors>`.
