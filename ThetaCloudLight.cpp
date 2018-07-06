#include "ThetaCloudLight.h"
#include "ThetaCloud.h"

#include <Arduino.h>
#include "I2CHelpers.h"
#include "Utils.h"

constexpr uint8_t ISL29023_ADDRESS = 0x44;

constexpr uint8_t COMMAND1_REGISTER = 0x00;
constexpr uint8_t COMMAND2_REGISTER = 0x01;
constexpr uint8_t DATALSB_REGISTER = 0x02;
constexpr uint8_t DATAMSB_REGISTER = 0x03;

constexpr uint8_t CONTINOUS_AMBIENT_LIGHT_MEAS = 0b10100000;
constexpr uint8_t CONTINOUS_INFRARED_MEAS = 0b11000000;

constexpr uint8_t FULL_SCALE_RANGE_1000 = 0b00000000;
constexpr uint8_t FULL_SCALE_RANGE_4000 = 0b00000001;
constexpr uint8_t FULL_SCALE_RANGE_16000 = 0b00000010;
constexpr uint8_t FULL_SCALE_RANGE_64000 = 0b00000011;

constexpr uint8_t ADC_RESOLUTION_16BIT = 0b00000000;
constexpr uint8_t ADC_RESOLUTION_12BIT = 0b00000100;
constexpr uint8_t ADC_RESOLUTION_8BIT = 0b00001000;
constexpr uint8_t ADC_RESOLUTION_4BIT = 0b00001100;

constexpr float USED_RESOLUTION = 65536;    // 2^16
constexpr float USED_RANGE = 64000;

constexpr float getLuxMultiplier(const float resolution, const float range)
{
    return range/resolution;
}

void ThetaCloudLight::init()
{
    WriteToI2C(ISL29023_ADDRESS)
    .write(COMMAND1_REGISTER).write(CONTINOUS_AMBIENT_LIGHT_MEAS);
    WriteToI2C(ISL29023_ADDRESS)
    .write(COMMAND2_REGISTER).write(FULL_SCALE_RANGE_64000);

    lightToken = thetaCloud.addReadHandler([this](const ThetaCloud::Emit &emit) {
        emit(SensorData{std::string("light"), to_string(read())});
    });
}

float ThetaCloudLight::read()
{
    WriteToI2C(ISL29023_ADDRESS).write(DATAMSB_REGISTER);
    auto dataMsb = ReadFromI2C<1>(ISL29023_ADDRESS);
    if (dataMsb.second) return NAN; // read error

    WriteToI2C(ISL29023_ADDRESS).write(DATALSB_REGISTER);
    auto dataLsb = ReadFromI2C<1>(ISL29023_ADDRESS);
    if (dataLsb.second) return NAN; // read error

    uint16_t light = dataMsb.first[0] << 8;
    light += dataLsb.first[0];

    return float(light) * getLuxMultiplier(USED_RESOLUTION, USED_RANGE);
}

ThetaCloudLight thetaCloudLight;
