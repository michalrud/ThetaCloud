#include "Utils.h"
#include <Arduino.h>

std::string to_string(float val)
{
	static char dtostrfbuf[15];		// not thread safe - cool while esp8266 is not multithreaded
	return std::string(dtostrf(val, 8, 2, dtostrfbuf));
}

std::string to_string(uint8_t val, int radix)
{
	static char itoabuf[4];		// not thread safe - cool while esp8266 is not multithreaded
	return std::string(itoa(val, itoabuf, radix));
}
