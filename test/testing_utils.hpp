#pragma once

#include <SensorData.h>

bool operator ==(const SensorData &a, const SensorData &b)
{
	return a.name == b.name && a.value == b.value;
}
