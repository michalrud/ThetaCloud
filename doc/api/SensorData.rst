SensorData
==========

A simple structure that holds data either read from or provided to external devices.

.. glossary::
	
	SensorData::name
		Name of the sensor which provided the data.

		This should be the name of the sensor itself, board identification may be added later
		during data publication.

	SensorData::value
		Value read from the sensor. Format is not defined, so it depends on the implementation.
		While simplicity should be preferred, there's nothing stopping given sensor handler to provide
		a JSON or XML document.

	const char* SensorData::getName()
		Convenience method to get ``const char*`` of :term:`SensorData::name`.
		Essentially calls ``name.c_str()``.

	const char* SensorData::getValue()
		Convenience method to get ``const char*`` of :term:`SensorData::value`.
		Essentially calls ``value.c_str()``.
