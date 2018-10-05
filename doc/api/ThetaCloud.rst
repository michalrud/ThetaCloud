ThetaCloud
==========

.. cpp:class:: ThetaCloud
	
	.. cpp:type:: std::function<void(const SensorData&)> Emit

		Function definition for callback that will be called by ThetaCloud every time a new data is available from
		devices. Can publish this data further or just print it using ``Serial``.

		Needs to be registered using :cpp:func:`ThetaCloud::whenDataAvailable()`. Only one function can be registered like
		that at one time - if :cpp:func:`ThetaCloud::whenDataAvailable()` is called again, previous function will be
		automatically unregistered.
		
		:Signature:
			.. code-block:: cpp

				using Emit = std::function<void(const SensorData&)>
		:Parameter SensorData: Data read from sensor.
		:Example implementation:
			.. code-block:: cpp

				void handleSensorData(const SensorData& data)
				{
					Serial.println(data.name);
					Serial.println(data.value);
				}

	.. cpp:type:: std::function<void(const Emit&)> DeviceReadHandler

			Function definition for callback that will be called each time ThetaCloud requests new data to be read,
			if available. Currently each registered handler will be called once when :cpp:func:`ThetaCloud::tick()` is called.

			Handler doesn't have to do anything if no data is available, or it cannot be read for any reason.

			If data is available, callback provided in argument should be called with :cpp:class:`SensorData` containing the
			data from sensor. Callback may be called multiple times, if multiple data is received (for example, sensor
			provides both temperature and humidity information).

			Callback should not be stored for longer than the duration of handler execution.

			:Signature:
				.. code-block:: cpp

					using DeviceReadHandler = std::function<void(const Emit&)>;
			:Parameter Emit: Callback which should be used to provide data from sensor to the rest of the ThetaCloud.
			:Example implementation:
				.. code-block:: cpp

					void handler(const ThetaCloud::Emit& emit)
					{
						emit(SensorData{"MySensor", "My value"});
					}

	.. cpp:type:: std::function<void(const SensorData&, const Emit&)> DeviceWriteHandler

		Function definition for callback that will be called, when a data is available for device to process.
		Data can be made available using :cpp:func:`ThetaCloud::write()` method. Data read from devices by ThetaCloud
		is not automatically published here to prevent infinite loops, but if it's required it can be easily done
		by calling :cpp:func:`ThetaCloud::write()` in implementation of :cpp:type:`ThetaCloud::Emit`.

		:Signature:
			.. code-block:: cpp

				using DeviceWriteHandler = std::function<void(const SensorData&, const Emit&)>;
		:Parameter SensorData: Data that became available for given device.
		:Parameter Emit: Callback, that can be called if any data needs to be published as a result of received data.
						 Does not have to called if no new data became available.
		:Example implementation:
			.. code-block:: cpp

				void writeHandler(const SensorData& sensor, const ThetaCloud::Emit& emit)
				{
					auto success = sendToDevice(sensor.value);
					if (success)
						emit(SensorData{"currentValue", sensor.value);
				}

	.. cpp:function:: void init()

		Initializes ThetaCloud. Usually should be called in the ``setup()`` function in the main file of Arduino's
		sketch.

		.. note::
			Should be called *before* any other ThetaCloud modules initialization, as it initializes
			basic libraries (at the moment just the ``Wire`` library) and any initializing actions performed by
			modules may fail without that.

		:Example:
			.. code-block:: cpp

				void setup()
				{
					thetaCloud.init();
					// initialization of other classes
				}

	.. cpp:function:: coid whenDataAvailable(const Emit& dataCallback)

		Method for registering an :cpp:type:`ThetaCloud::Emit` callback to ThetaCloud.

		In other words, the callback provided to this function will be called when a data from the device becomes
		available and can be sent further. See :cpp:type:`ThetaCloud::Emit` description for more information.

		:Example:
			.. code-block:: cpp

				void handleSensorData(const SensorData& data)
				{
					Serial.println(data.name);
					Serial.println(data.value);
				}

				void setup()
				{
					//... other initialization
					//...
					thetaCloud.whenDataAvailable(handleSensorData);
				}

	.. cpp:function:: DeviceHandlerTokenPtr addReadHandler(const DeviceReadHandler& handler)

		Method for registering an :cpp:type:`ThetaCloud::DeviceReadHandler` callback to ThetaCloud.

		In other words, callbacks provided using this method will be called when :cpp:func:`ThetaCloud::Tick()` is called
		to check if new data can be read from the device. See :cpp:type:`ThetaCloud::DeviceReadHandler` description for
		more information.

		.. note::
			Deletion of a :cpp:type:`DeviceHandlerToken` returned by this function will result in immediate unregistration
			of the registered callback. Therefore, **the returned value needs to be kept somewhere**. **The following code
			will not work** and the callback will *never be called*:

			.. code-block:: cpp

				class MyBrokenHandler
				{
				public:
					void handle(const ThetaCloud::Emit& emit)
					{
						// Will never be called because of the error!
						emit(SensorData{"Hello", "There"});
					}
					void init(ThetaCloud& thetaCloud)
					{
						// WRONG! WILL NOT DO ANYTHING as the return value is
						// is immediately destroyed
						thetaCloud.addReadHandler(handle);
					}
				};

			See the code example later on to see a proper way to do it.

		:Example: Warning - after `token` goes out of scope handler will be automatically unregistered.

			.. code-block:: cpp

				auto token = thetaCloud.addReadHandler(
					[](const ThetaCloud::Emit& emit)
					{
						emit("hello", "hi there");
					});
		:Example in a class:
			.. code-block:: cpp

				class MyHandler
				{
				public:
					static void handle(const ThetaCloud::Emit& emit)
					{
						emit(SensorData{"Hello", "There"});
					}
					void init(ThetaCloud& thetaCloud)
					{
						// Token is preserved - the handler will be automatically
						// unregistered when this instance of MyHandler gets deleted.
						token = thetaCloud.addReadHandler(handle);
					}
				private:
					DeviceHandlerTokenPtr token;
				};

	.. cpp:function:: DeviceHandlerTokenPtr addWriteHandler(const std::string& topic, const DeviceWriteHandler& handler)

		Method for registering an :cpp:type:`ThetaCloud::DeviceWriteHandler` callback to ThetaCloud.

		In other words, callbacks provided using this method will be called when a data will be
		available to the device for *consumption*.

		For data to be consumed by the device, it needs to be provided to ThetaCloud using the
		:cpp:func:`ThetaCloud::write()` method, and the ``name`` field of ``SensorData`` needs to be equal to the
		``topic`` argument passed during the registration.

		.. note::
			Only one handler can be subscribed for given topic. If another handler subscribes for the previously
			subscribed topic, the subscription will silently fail, and old subscription will still be in force.

			To make a new subscription to an already used topic, the old subscription needs to be removed by deleting a
			:cpp:type:`DeviceHandlerToken` received during subscription.

		.. note::
			Deletion of a :cpp:type:`DeviceHandlerToken` returned by this function will result in immediate unregistration
			of the registered callback. Therefore, **the returned value needs to be kept somewhere**. **The following code
			will not work** and the callback will *never be called*:

			.. code-block:: cpp

				class MyBrokenHandler
				{
				public:
					void handle(const SensorData& data, const ThetaCloud::Emit& emit)
					{
						// Will never be called because of the error!
						emit(SensorData{"Hello", "There"});
					}
					void init(ThetaCloud& thetaCloud)
					{
						// WRONG! WILL NOT DO ANYTHING as the return value is
						// is immediately destroyed
						thetaCloud.addWriteHandler("hello", handle);
					}
				};

			See the code example later on to see a proper way to do it.
		
		See :cpp:type:`ThetaCloud::DeviceWriteHandler` description for more information.

		:Example: Warning - after `token` goes out of scope, the handler will be automatically unregistered.

			.. code-block:: cpp

				auto token = thetaCloud.addWriteHandler("hello",
					[](const SensorData& data, const ThetaCloud::Emit& e)
					{
						Serial.println("Someone has greeted us!")
					});
				thetaCloud.write(SensorData{"hello", "there"}); // text will be written on Serial
				thetaCloud.write(SensorData{"bye", "friend"});  // our handler will not be called
		:Example in a class:
			.. code-block:: cpp

				class MyHandler
				{
				public:
					static void handle(const SensorData& data, const ThetaCloud::Emit& emit)
					{
						emit(SensorData{"Hello", "There"});
					}
					void init(ThetaCloud& thetaCloud)
					{
						// Token is preserved - the handler will be automatically
						// unregistered when this instance of MyHandler gets deleted.
						token = thetaCloud.addWriteHandler("hello", handle);
					}
				private:
					DeviceHandlerTokenPtr token;
				};

	.. cpp:function:: void write(const SensorData& data) const

		Method that can be used to publish data to modules registered using :cpp:func:`ThetaCloud::addWriteHandler()` so
		it can be *consumed*.

		Only handler registered to consume ``topic`` that is equal to published ``SensorData.name`` will be called.

		.. note::
			Publishing data using this function **will not** send it to :cpp:type:`ThetaCloud::Emit` function registered
			via :cpp:func:`ThetaCloud::whenDataAvailable()`.

			Use :cpp:func:`ThetaCloud::emit()` to send data to currently registered :cpp:type:`ThetaCloud::Emit`.

		:Example:
			.. code-block:: cpp

				thetaCloud.write(SensorData{"topic", "value"});

	.. cpp:function:: void emit(const SensorData& data) const

		Method that calls the currently registered :cpp:type:`ThetaCloud::Emit` callback.

		.. note::
			Publishing data using this function **will not** send it to any :cpp:type:`ThetaCloud::DeviceWriteHandler`
			for consumption. The data will be sent directly to currently registered :cpp:type:`ThetaCloud::Emit`.

			To send data to :cpp:type:`ThetaCloud::DeviceWriteHandler`, use :cpp:func:`ThetaCloud::write()`.

		:Example:
			.. code-block:: cpp

				thetaCloud.emit(SensorData{"topic", "value"});

	.. cpp:function:: void tick()

		Method that should be called in the application's main loop (in case of Arduino, in the ``loop()`` function).

		Currently calls all registered :cpp:type:`ThetaCloud::DeviceReadHandler` callbacks, but the exact functionality may be
		changed in the future.

		:Example:
			.. code-block:: cpp

				void loop()
				{
					thetaCloud.tick();
					delay(5000);
				}

.. cpp:class:: DeviceHandlerToken

	Token that ensures that given handler (either :cpp:type:`ThetaCloud::DeviceReadHandler` or :cpp:type:`ThetaCloud::DeviceWriteHandler`)
	is registered to ThetaCloud. When destroyed, automatically unregisters a corresponding handler from ThetaCloud,
	preventing calls to non-existing handlers, and - in effect - crashes.

	In practice - instance of this object (technically :cpp:type:`an unique pointer to it <DeviceHandlerTokenPtr>`) is
	returned by :cpp:func:`ThetaCloud::addReadHandler()` and :cpp:func:`ThetaCloud::addWriteHandler()`. Keep this instance
	for as long as your handler is valid and everything should be fine.

.. cpp:type:: std::unique_ptr<DeviceHandlerToken> DeviceHandlerTokenPtr

	Unique pointer to :cpp:type:`DeviceHandlerToken`. Automatically destroys the pointee when it goes out of scope.
	Alias for ``std::unique_ptr<DeviceHandlerToken>``.
