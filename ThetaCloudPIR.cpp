#include "ThetaCloudPIR.h"
#include "Arduino.h"

const std::string ThetaCloudPIR::MOTION_DETECTED = "Motion detected!";
const std::string ThetaCloudPIR::MOTION_LOST = "Motion lost.";

void ThetaCloudPIR::init()
{
    // is this board connected?
    auto adcValue = analogRead(A0);
    if (adcValue < 50 || adcValue > 150)
        return;
    // actual initialization if board is detected
    for (auto &pir : PIR_GPIO) {
        pinMode(pir.pin, INPUT);
        pir.token = thetaCloud.addReadHandler([&pir, this](const ThetaCloud::Emit &emit) mutable {
            handlePir(pir, emit);
        });
    }
}

void ThetaCloudPIR::handlePir(ThetaCloudPIR::PirInfo &pir, const ThetaCloud::Emit &emit)
{
    bool currentState = (digitalRead(pir.pin) == HIGH);
    if (pir.lastState != currentState) {
        pir.lastState = currentState;
        emit(SensorData{pir.name, currentState ? "Motion detected!" : "Motion lost."});
    }
}

ThetaCloudPIR thetaCloudPIR;
