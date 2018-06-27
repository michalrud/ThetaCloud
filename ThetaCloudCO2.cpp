#include "ThetaCloudCO2.h"
#include "ThetaCloud.h"
#include "Arduino.h"
#include "Wire.h"

const uint8_t MICS_VZ_89_ADDRESS = 0x70;
const uint8_t GET_STATUS_COMMAND = 0b00001001;
const unsigned int READ_RETRIES = 5;

ThetaCloudCO2::ThetaCloudCO2() :
	enabled(false)
{
}

void ThetaCloudCO2::init()
{
	// is this board connected?
	auto adcValue = analogRead(A0);
	if (adcValue > 150 && adcValue < 250)
	{
		enabled = true;
	}
	else
	{
		enabled = false;
		return;
	}
	// actual initialization if board is detected
	thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		auto measurementResult = GetCo2Level();
		if (measurementResult.error) return;
		char dtostrfbuf[15];
		emit(SensorData{std::string("co2"),
			std::string(dtostrf(measurementResult.co2Value, 8, 2, dtostrfbuf))});
		emit(SensorData{std::string("voc"), 
			std::string(dtostrf(measurementResult.vocValue, 8, 2, dtostrfbuf))});
	});
}

ThetaCloudCO2::SensorGetValue ThetaCloudCO2::GetCo2Level()
{
	SensorGetValue result = {0, 0, false};
	Wire.beginTransmission(MICS_VZ_89_ADDRESS);
	Wire.write(GET_STATUS_COMMAND);
	Wire.endTransmission();
	Wire.requestFrom(MICS_VZ_89_ADDRESS, (uint8_t) 6);
	delay(5);
	{
		unsigned int attempt = 0;
	    while (Wire.available() < 6) {
	        if (++attempt > READ_RETRIES) {
	            return {0, 0, true};	// error
	        }
	        delay(10);
	    }
	}
    uint8_t raw_co2 = Wire.read();
    Wire.read(); // voc_short (unused)
    uint8_t raw_voc = Wire.read();
    Wire.read(); Wire.read(); Wire.read(); // raw sensor data (unused)
    // calculations from Prelimenary Datasheet for MiCS-VZ-86/89
    result.co2Value = (raw_co2 - 13) * (1600 / 229) + 400;
    result.vocValue = (raw_voc - 13) * (1000 / 229);
	return result;
}

ThetaCloudCO2 thetaCloudCO2;
