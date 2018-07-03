ThetaCloudRelay
===============

.. cpp:class:: ThetaCloudRelay

	Additional, optional board having two relays, allowing the user to
	switch external appliances on and off.

	:Publishes: This module does not publish any data.

	:Consumes:
		:Relay1:
			Possible values:

			:ON: Turns on the first appliance.
			:OFF: Turns off the first appliance.

			All other values are ignored.

		:Relay2:
			Possible values:

			:ON: Turns on the second appliance.
			:OFF: Turns off the second appliance.

			All other values are ignored.

	.. cpp:member:: static const std::string RELAY1

		Const value for which the service registers in
		:cpp:class:`ThetaCloud`, that changes the state
		of the first relay. Should be used as
		:cpp:member:`SensorData::name`.

	.. cpp:member:: static const std::string RELAY2

		Const value for which the service registers in
		:cpp:class:`ThetaCloud`, that changes the state
		of the second relay. Should be used as
		:cpp:member:`SensorData::name`.

	.. cpp:member:: static const std::string ON

		Const value which turns given relay on. Should be used as
		:cpp:member:`SensorData::value`.

	.. cpp:member:: static const std::string OFF

		Const value which turns given relay off. Should be used as
		:cpp:member:`SensorData::value`.
