#include "ThetaCloudHumidity.h"
#include "Arduino.h"
#include <cmath>
#include "SensorData.h"
#include "ThetaCloud.h"
#include "I2CHelpers.h"
#include "Utils.h"

const uint8_t SHT21_ADDRESS = 0x40;

const uint8_t GET_TEMPERATURE_CMD = 0xF3;
const uint8_t GET_HUMIDITY_CMD = 0xF5;

ThetaCloudHumidity::ThetaCloudHumidity()
{
}

void ThetaCloudHumidity::init()
{
	humidityToken = thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		emit(SensorData{std::string("humidity"), to_string(this->GetHumidity())});
	});
	temperatureToken = thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		emit(SensorData{std::string("temperature"), to_string(this->GetTemperature())});
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

	WriteToI2C(SHT21_ADDRESS).write(command);

	delay(100);

	auto dataFromSensor = ReadFromI2C<3>(SHT21_ADDRESS);
	if (dataFromSensor.second) return ERROR;

    result = dataFromSensor.first[0] << 8;
    result += dataFromSensor.first[1];
    result &= ~0x0003;   // clear two low bits (status bits)

    return {result, false};
}

ThetaCloudHumidity thetaCloudHumidity;