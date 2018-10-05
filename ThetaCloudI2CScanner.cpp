#include "ThetaCloudI2CScanner.h"
#include "ThetaCloud.h"

#include <set>
#include <Wire.h>
#include "Utils.h"

constexpr uint8_t MIN_ADDRESS = 0x01;
constexpr uint8_t MAX_ADDRESS = 0x7F;

void ThetaCloudI2CScanner::init(ThetaCloud& thetaCloud)
{
    token = thetaCloud.addReadHandler([this](const ThetaCloud::Emit &emit) {
        emit(SensorData{std::string("i2c-devices"), scan()});
    });
}

std::string ThetaCloudI2CScanner::scan()
{
    std::set<uint8_t> detectedDevices;
    for (uint8_t address = MIN_ADDRESS; address < MAX_ADDRESS; ++address) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();
        if (0 == error)
            detectedDevices.insert(address);
    }
    std::string result;
    bool first = true;
    for (const auto &address : detectedDevices) {
        if (!first) result += ", ";
        first = false;
        result += "0x" + to_string(address, 16);
    }
    return result;
}
