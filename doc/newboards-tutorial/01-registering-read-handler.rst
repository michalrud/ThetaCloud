Registering read handlers
=========================

Services in ThetaCloud don't have to derive from any parent classes or to implement any specific interface. The only requirement is that they need to register their functions using :cpp:func:`ThetaCloud::addReadHandler()` and/or :cpp:func:`ThetaCloud::addWriteHandler()`.

Therefore, it's not even required to create new classes for the services - it would be enough to just call the appropriate method from anywhere and pass a lambda function to it. Wrapping it in separate classes makes it easier to separate from the rest of the code, while it doesn't require that much work, so we will go in that direction.

Header file
-----------

First - we need to create a class definition in a header file. This should be enough:

.. code-block:: cpp
	:linenos:
	:caption: ThetaCloudExample.h
	
	#ifndef THETA_CLOUD_EXAMPLE_H	// include guard - needs to be unique
	#define THETA_CLOUD_EXAMPLE_H

	#include <DeviceHandlerToken.h>

	class ThetaCloudExample
	{
	public:
		void init();
	protected:
		DeviceHandlerTokenPtr readToken;
		DeviceHandlerTokenPtr writeToken;
	};

	extern ThetaCloudExample thetaCloudExample;

	#endif  // THETA_CLOUD_EXAMPLE_H

Not much is happening here - ``init()`` will be our initialization function, where
we will subscribe to :cpp:class:`ThetaCloud`.

In lines 11 and 12 we are creating two ``unique_ptr`` objects that will hold tokens
for read handler and write handler. While the ``unique_ptr`` is a veru useful C++
feature, you don't have to worry about it at all - all you need to know is that this
is where you need to put the value returned by :cpp:func:`ThetaCloud::addReadHandler()`
and :cpp:func:`ThetaCloud::addWriteHandler()`. The idea is, that as long as those
values are not destroyed the handlers will be registered. That way, when your
class will be destroyed for whatever reason, :cpp:class:`ThetaCloud` will stop
calling your handlers and avoid crashing.

Line 15 is not really necessary, and to be honest is a bad
programming practice - but as most Arduino libraries seem to create
static variables of their own type, it's probably what most beginner
Arduino users grew used to. If you know that your library will not be used
by beginner Arduino users feel free to omit this.

CPP file
--------

.. code-block:: cpp
	:linenos:
	:caption: ThetaCloudExample.cpp

	#include "ThetaCloudExample.h"
	#include <ThetaCloud.h>
	#include <SensorData.h>

	void ThetaCloudExample::init()
	{
		readToken = thetaCloud.addReadHandler([](const ThetaCloud::Emit &emit) {
			emit(SensorData{"Example", "Heartbeat"});
		});
	}

	ThetaCloudExample thetaCloudExample;

As you can see, this is not too complicated either - the ``init()`` method calls
:cpp:func:`ThetaCloud::addReadHandler()` with a lambda function - a
:cpp:type:`ThetaCloud::DeviceReadHandler`. This function gets one parameter - a
reference to an :cpp:type:`ThetaCloud::Emit` function, that will publish the
data further (for where it goes see: :doc:`/doc/library-tutorial/03-sensor-data-handlers`).
Calling this function with a :cpp:class:`SensorData` structure publishes the data.

And so, in the 8th line we are publishing the data named ``Example`` with the value
of ``Heartbeat``.

.. note:: It's possible that the ``emit`` function will be changed during the lifetime
	of an application. Therefore, if you want your module to emit data even when
	:cpp:func:`ThetaCloud::tick()` is not called, use :cpp:func:`ThetaCloud::emit()`
	instead of storing the reference received in your handler.

So we have half of our functionality already implemented - let's continue to
the next lesson, where we will :doc:`handle the data sent to our module <02-registering-write-handler>`.
