Publishing data to sensors
==========================

We are currently able to read data from the sensors just
fine. But what about publishing data targeting the devices?

The only thing we need to do is to call the
:cpp:func:`ThetaCloud::write()` method with data in a
:cpp:class:`SensorData` structure. If there's a module
that handles the :cpp:member:`SensorData::name`, then
it will be notified of the data being published.

.. note:: If publishing the data seems to have no effect,
	check if there's any module loaded that handles the
	provided :cpp:member:`SensorData::name`.

Let's write an application, that will toggle the
:doc:`relays </doc/api/ThetaCloudRelay>` every 5 seconds:

.. code-block:: cpp
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>
	#include <ThetaCloudRelay.h>
	#include <ThetaCloudHumidity.h>
	#include <ThetaCloudEnvironment.h>
	#include <ThetaCloudI2CScanner.h>

	bool relayOn = false;

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
		for (const auto& name : {ThetaCloudRelay::RELAY1, ThetaCloudRelay::RELAY2})
		{
			thetaCloud.write(SensorData{
				name,
				relayOn ? ThetaCloudRelay::ON : ThetaCloudRelay::OFF
			});
		}
		relayOn = !relayOn;
		delay(5000);
	}

Another effect that this code will have is that the sensors
will be checked for new code every 5 seconds - as the :cpp:func:`ThetaCloud::tick()`
method is called every 5 seconds. Of course it's possible to change that -
the :cpp:func:`ThetaCloud::tick()` and :cpp:func:`ThetaCloud::write()` methods
are independent of each other.

This is all when it comes to basic library usage tutorial - you're now able to
turn certain modules on, and send and get data between your ThetaCloud board
and the devices.

If this is still not enough knowledge, go to the :doc:`/doc/api` reference or
:doc:`learn how to add support for your own hardware </doc/newboards-sw>`.
