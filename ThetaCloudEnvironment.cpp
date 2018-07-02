#include "ThetaCloudEnvironment.h"
#include "ThetaCloud.h"
#include "Arduino.h"
#include "I2CHelpers.h"
#include "Utils.h"

const uint8_t MICS_VZ_89_ADDRESS = 0x70;
const uint8_t GET_STATUS_COMMAND = 0b00001001;

ThetaCloudEnvironment::ThetaCloudEnvironment() :
	enabled(false)
{
}

void ThetaCloudEnvironment::init()
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
	co2Token = thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		auto co2MeasurementResult = GetAirReadings();
		if (!co2MeasurementResult.error)
		{
			emit(SensorData{std::string("co2"), to_string(co2MeasurementResult.co2Value)});
			emit(SensorData{std::string("voc"), to_string(co2MeasurementResult.vocValue)});
		}
	});
}

ThetaCloudEnvironment::AirSensorValue ThetaCloudEnvironment::GetAirReadings()
{
	AirSensorValue result = {0, 0, false};
	WriteToI2C(MICS_VZ_89_ADDRESS).write(GET_STATUS_COMMAND);
	auto dataFromSensor = ReadFromI2C<6>(MICS_VZ_89_ADDRESS);
	if (dataFromSensor.second) return AIR_ERROR;
    uint8_t raw_co2 = dataFromSensor.first[0];
    uint8_t raw_voc = dataFromSensor.first[2];
    // calculations from Preliminary Datasheet for MiCS-VZ-86/89
    result.co2Value = (raw_co2 - 13) * (1600 / 229) + 400;
    result.vocValue = (raw_voc - 13) * (1000 / 229);
	return result;
}

ThetaCloudEnvironment thetaCloudEnvironment;
