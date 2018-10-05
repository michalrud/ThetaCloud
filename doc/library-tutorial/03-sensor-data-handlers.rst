Handling data from sensors
==========================

As a next step, let's print out to the serial console all the
data from our sensors. To do so, we need to implement
:cpp:type:`ThetaCloud::Emit` and call :cpp:func:`ThetaCloud::whenDataAvailable()`
to register it:

.. code-block:: cpp
	:linenos:
	:emphasize-lines: 16-21,24
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>
	#include <ThetaCloudRelay.h>
	#include <ThetaCloudHumidity.h>
	#include <ThetaCloudEnvironment.h>
	#include <ThetaCloudI2CScanner.h>

	ThetaCloud thetaCloud;
	ThetaCloudLight thetaCloudLight;
	ThetaCloudRelay thetaCloudRelay;
	ThetaCloudHumidity thetaCloudHumidity;
	ThetaCloudEnvironment thetaCloudEnvironment;
	ThetaCloudI2CScanner thetaCloudI2CScanner;

	void handleNewData(const SensorData& data)
	{
		Serial.print(data.getName());
		Serial.print(" - ");
		Serial.println(data.getValue());
	}

	void setup()
	{
		thetaCloud.whenDataAvailable(handleNewData);
		thetaCloud.init(thetaCloud);
		thetaCloudLight.init(thetaCloud);
		thetaCloudRelay.init(thetaCloud);
		thetaCloudHumidity.init(thetaCloud);
		thetaCloudEnvironment.init(thetaCloud);
		thetaCloudI2CScanner.init(thetaCloud);
	}

	void loop()
	{
		thetaCloud.tick();
	}

In the next part of the tutorial, we'll learn
:doc:`how to publish data for the sensors <04-publishing-data-to-sensors>`.
