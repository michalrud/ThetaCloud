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

	class ThetaCloud;

	class ThetaCloudExample
	{
	public:
		void init(ThetaCloud& thetaCloud);
	protected:
		DeviceHandlerTokenPtr readToken;
		DeviceHandlerTokenPtr writeToken;
	};

	extern ThetaCloudExample thetaCloudExample;

	#endif  // THETA_CLOUD_EXAMPLE_H

Not much is happening here - ``init()`` will be our initialization function, where
we will subscribe to :cpp:class:`ThetaCloud` passed as an argument. The ``&`` here
is important to tell the compiler that we don't want to copy the whole
:cpp:class:`ThetaCloud` - we want to use the existing one.

In line 6 we tell the compiler that we will use the :cpp:class:`ThetaCloud` class
in our code. But since the compiler doesn't have to know the details about it at
this point, we just tell it that the class is existing, so it wouldn't complain
about it. We will include the header with actual class definition later in the
``.cpp`` file, where it will be really required.

In lines 13 and 14 we are creating two ``unique_ptr`` objects that will hold tokens
for read handler and write handler. While the ``unique_ptr`` is a very useful C++
feature, you don't have to worry about it at all - all you need to know is that this
is where you need to put the value returned by :cpp:func:`ThetaCloud::addReadHandler()`
and :cpp:func:`ThetaCloud::addWriteHandler()`. The idea is, that as long as those
values are not destroyed the handlers will be registered. That way, when your
class will be destroyed for whatever reason, :cpp:class:`ThetaCloud` will stop
calling your handlers and avoid crashing.

CPP file
--------

.. code-block:: cpp
	:linenos:
	:caption: ThetaCloudExample.cpp

	#include "ThetaCloudExample.h"
	#include <ThetaCloud.h>
	#include <SensorData.h>

	void ThetaCloudExample::init(ThetaCloud& thetaCloud)
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
of ``Heartbeat``. Remember - calling ``emit()`` is *entirely optional*. If your module
doesn't have anything to publish - handler function can return without calling
``emit()`` at all.

Also, if your function has more than one piece of data to publish, ``emit()`` can
be called multiple times.

.. note:: It's possible that the ``emit`` function will be changed during the lifetime
	of an application. Therefore, if you want your module to emit data even when
	:cpp:func:`ThetaCloud::tick()` is not called, use :cpp:func:`ThetaCloud::emit()`
	instead of storing the reference received in your handler.

So we have half of our functionality already implemented - let's continue to
the next lesson, where we will :doc:`handle the data sent to our module <02-registering-write-handler>`.
