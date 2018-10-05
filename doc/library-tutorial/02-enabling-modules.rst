Enabling provided modules
=========================

ThetaCloud library comes with a set of modules, which allow for interaction
with hardware, either integrated with the base board or connected as an additional module. Let's enable :doc:`the light sensor </doc/api/ThetaCloudLight>`, which is integrated with
the base board by default:

.. code-block:: cpp
	:linenos:
	:emphasize-lines: 2,5,10
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>

	ThetaCloud thetaCloud;
	ThetaCloudLight thetaCloudLight;

	void setup()
	{
		thetaCloud.init();
		thetaCloudLight.init(thetaCloud);
	}

	void loop()
	{
		thetaCloud.tick();
	}

That's all what's needed - one ``#include`` and one call to the ``init()`` method.

.. note:: Remember to initialize ``thetaCloud`` *before* other ThetaCloud
	modules, as ``ThetaCloud`` may perform some common initialization required
	in other modules. If this rule will not be followed, other modules may not
	work correctly.

Since it's that easy, let's enable all provided modules:

.. code-block:: cpp
	:linenos:
	:emphasize-lines: 3,4,5,6,9,10,11,12,13,18,19,20,21,22
	
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

	void setup()
	{
		thetaCloud.init();
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

As you can see, some of the enabled modules are handling additional expansion
boards, which may not be connected at the moment. Don't worry - modules are
written in such way, that if the board will not be detected, the module will
not do anything.

Speaking of not doing anything - our current program will do all the
measurements, but we will not see any output anywhere - it's time to change
that in :doc:`next lesson <03-sensor-data-handlers>`.
