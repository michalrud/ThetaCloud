Registering write handlers
==========================

In the :doc:`previous lesson <01-registering-read-handler>` we've learned how to make our module publish data on every :cpp:func:`ThetaCloud::tick()`.

Now it's time to teach our module on how to react to data incoming from the outside.

Let's assume that our module will react to data named ``Example``, and with every message having the value equal to ``Ping`` our module will emit a
message with the same name, and the value of ``Pong``.

All other messages will be ignored.

Implementing the write handler
------------------------------

.. code-block:: cpp
	:linenos:
	:emphasize-lines: 10-17
	:caption: ThetaCloudExample.cpp

	#include "ThetaCloudExample.h"
	#include <ThetaCloud.h>
	#include <SensorData.h>

	void ThetaCloudExample::init()
	{
		readToken = thetaCloud.addReadHandler([](const ThetaCloud::Emit &emit) {
			emit(SensorData{"Example", "Heartbeat"});
		});
		writeToken = thetaCloud.addWriteHandler("Example",
			[](const SensorData &data, const ThetaCloud::Emit &emit)
			{
				if (data.value == std::string("Ping"))
				{
					emit(SensorValue{data.name, "Pong"});
				}
			});
	}

	ThetaCloudExample thetaCloudExample;

Here, just like in the read handler example, we need to store the token safely, so :cpp:class:`ThetaCloud` will not deregister our handler. As a first parameter to :cpp:func:`ThetaCloud::addWriteHandler()` we provide the expected name of the data - :cpp:class:`ThetaCloud` will call our handler only when the
data with this name would be published using :cpp:class:`ThetaCloud::write()`.

The second parameter is a :cpp:type:`ThetaCloud::DeviceWriteHandler` lambda
function. It takes two parameters - one is the :cpp:class:`SensorData` received, and the other one is a :cpp:type:`ThetaCloud::Emit` callback, just
like in :cpp:type:`ThetaCloud::DeviceReadHandler`.

So, in the function body we read the received :cpp:class:`SensorData`, and
call the :cpp:type:`ThetaCloud::Emit` with the response.

That's almost everything you need to know while writing support for your device
in ThetaCloud - the other one is how to actually talk to your device, but this
is not covered in this tutorial.

Have fun!
