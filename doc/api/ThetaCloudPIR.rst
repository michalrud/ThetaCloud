ThetaCloud PIR board
====================


.. cpp:class:: ThetaCloudPIR

	Board equipped with two motion sensors.

	.. note:: This class polls for motion events, therefore if the delay
		between calls to :cpp:func:`ThetaCloud::tick()` is too long some
		events may not be caught.

	:Publishes:
		:PIR1:
			Information about motion being detected or lost by the first
			motion detector.

			:format: ``Motion detected!`` when motion is detected,
				``Motion lost.`` when motion is lost.

		:PIR2:
			Information about motion being detected or lost by the first
			motion detector.

			:format: ``Motion detected!`` when motion is detected,
				``Motion lost.`` when motion is lost.

	:Consumes: This module does not consume any data.
