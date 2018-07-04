#include "ThetaCloudRelay.h"
#include "ThetaCloud.h"

#include "Arduino.h"

const char* ThetaCloudRelay::RELAY1 = "Relay1";
const char* ThetaCloudRelay::RELAY2 = "Relay2";
const std::string ThetaCloudRelay::ON = "ON";
const std::string ThetaCloudRelay::OFF = "OFF";

ThetaCloudRelay::ThetaCloudRelay()
{
}

void ThetaCloudRelay::init()
{
	// is this board connected?
	auto adcValue = analogRead(A0);
	if (adcValue < 250 || adcValue > 350)
	{
		return;
	}
	// actual initialization if board is detected
	for (const auto& pin : SWITCH_GPIO)
	{
		  pinMode(pin.second, OUTPUT);
	}
	switch1Token = thetaCloud.addWriteHandler(RELAY1,
		[this](const SensorData& data, const ThetaCloud::Emit& emit) {
			trySetSwitch(Switch::one, data);
	});
	switch2Token = thetaCloud.addWriteHandler(RELAY2,
		[this](const SensorData& data, const ThetaCloud::Emit& emit) {
			trySetSwitch(Switch::two, data);
	});
}

void ThetaCloudRelay::trySetSwitch(Switch sw, const SensorData& data) const
{
	if (ThetaCloudRelay::ON == data.value)
		setSwitch(sw, true);
	else if (ThetaCloudRelay::OFF == data.value)
		setSwitch(sw, false);
}

void ThetaCloudRelay::setSwitch(Switch whichOne, bool enabled) const
{
	digitalWrite(SWITCH_GPIO.at(whichOne), enabled ? HIGH : LOW);
}

ThetaCloudRelay thetaCloudRelay;

