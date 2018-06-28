#include "Utils.h"
#include <Arduino.h>

std::string floatToString(float val)
{
	static char dtostrfbuf[15];		// not thread safe - cool while esp8266 is not multithreaded
	return std::string(dtostrf(val, 8, 2, dtostrfbuf));
}
