Simplest ThetaCloud application
-------------------------------

A simplest application that starts up the ThetaCloud infrastructure can be
written like this:

.. code-block:: cpp
	:linenos:
	
	#include <ThetaCloud.h>

	ThetaCloud thetaCloud;

	void setup()
	{
		thetaCloud.init();
	}

	void loop()
	{
		thetaCloud.tick();
	}

That's all what's needed - the library is initialized and does its work
in a loop for as long as the board is working.

The only issue is that it doesn't really do anything at this point - so
let's expand this program a bit :doc:`in the next section <02-enabling-modules>`.

.. seealso:: See also: :cpp:func:`ThetaCloud::init()`, :cpp:func:`ThetaCloud::tick()`
