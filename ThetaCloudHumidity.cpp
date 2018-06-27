#include "ThetaCloudHumidity.h"
#include "Arduino.h"
#include <Wire.h>
#include <cmath>
#include "SensorData.h"
#include "ThetaCloud.h"

const uint8_t SHT21_ADDRESS = 0x40;

const uint8_t GET_TEMPERATURE_CMD = 0xF3;
const uint8_t GET_HUMIDITY_CMD = 0xF5;

const unsigned int READ_RETRIES = 10;

ThetaCloudHumidity::ThetaCloudHumidity()
{
}

void ThetaCloudHumidity::init()
{
	thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		char dtostrfbuf[15];
		emit(SensorData{std::string("humidity"),
			std::string(dtostrf(this->GetHumidity(), 8, 2, dtostrfbuf))});
	});
	thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		char dtostrfbuf[15];
		emit(SensorData{std::string("temperature"),
			std::string(dtostrf(this->GetTemperature(), 8, 2, dtostrfbuf))});
	});
}

float ThetaCloudHumidity::GetHumidity()
{
	auto result = GetValue(GET_HUMIDITY_CMD);
	if (result.error) return NAN;

    return -6.0 + 125.0 / 65536.0 * result.value;
}

float ThetaCloudHumidity::GetTemperature()
{
	auto result = GetValue(GET_TEMPERATURE_CMD);
	if (result.error) return NAN;

    return -46.85 + 175.72 / 65536.0 * result.value;
}

ThetaCloudHumidity::SensorGetValue ThetaCloudHumidity::GetValue(uint8_t command)
{
	uint16_t result;

	Wire.beginTransmission(SHT21_ADDRESS);
	Wire.write(command);
	Wire.endTransmission();

	delay(100);

	Wire.requestFrom(SHT21_ADDRESS, (uint8_t) 3);
	delay(5);
	{
		unsigned int attempt = 0;
	    while (Wire.available() < 3) {
	        if (++attempt > READ_RETRIES) {
	            return {0, true};
	        }
	        delay(10);
	    }
	}

    result = Wire.read() << 8;
    result += Wire.read();
    result &= ~0x0003;   // clear two low bits (status bits)

    //Clear the final byte from the buffer
    Wire.read();

    return {result, false};
}

ThetaCloudHumidity thetaCloudHumidity;