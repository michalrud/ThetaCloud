SensorData
==========

.. cpp:class:: SensorData
		
		A simple structure that holds data either read from or provided to external devices.
	
	.. cpp:member:: std::string name

		Name of the sensor which provided the data.

		This should be the name of the sensor itself, board identification may be added later
		during data publication.

	.. cpp:member:: std::string value

		Value read from the sensor. Format is not defined, so it depends on the implementation.
		While simplicity should be preferred, there's nothing stopping given sensor handler to provide
		a JSON or XML document.

	.. cpp:function:: const char* getName() const

		Convenience method to get ``const char*`` of :cpp:member:`SensorData::name`.
		Essentially calls ``name.c_str()``.

	.. cpp:function:: const char* getValue() const

		Convenience method to get ``const char*`` of :cpp:member:`SensorData::value`.
		Essentially calls ``value.c_str()``.
