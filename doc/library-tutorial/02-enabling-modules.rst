Enabling provided modules
=========================

ThetaCloud library comes with a set of modules, which allow for interaction
with hardware, either integrated with the base board or connected as an additional module. Let's enable :doc:`the light sensor </doc/api/ThetaCloudLight>`, which is integrated with
the base board by default:

.. code-block:: cpp
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>

	void setup()
	{
		thetaCloud.init();
		thetaCloudLight.init();
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
	
	#include <ThetaCloud.h>
	#include <ThetaCloudLight.h>
	#include <ThetaCloudRelay.h>
	#include <ThetaCloudHumidity.h>
	#include <ThetaCloudEnvironment.h>
	#include <ThetaCloudI2CScanner.h>

	void setup()
	{
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

As you can see, some of the enabled modules are handling additional expansion
boards, which may not be connected at the moment. Don't worry - modules are
written in such way, that if the board will not be detected, the module will
not do anything.

Speaking of not doing anything - our current program will do all the
measurements, but we will not see any output anywhere - it's time to change
that in :doc:`next lesson <03-sensor-data-handlers>`.
