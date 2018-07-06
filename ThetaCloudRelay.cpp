#include "ThetaCloudRelay.h"
#include "ThetaCloud.h"

#include "Arduino.h"

ThetaCloudRelay::ThetaCloudRelay() :
	enabled(false)
{
}

void ThetaCloudRelay::init()
{
	// is this board connected?
	auto adcValue = analogRead(A0);
	if (adcValue > 250 && adcValue < 350)
	{
		enabled = true;
	}
	else
	{
		enabled = false;
		return;
	}
	// actual initialization if board is detected
	for (const auto& pin : SWITCH_GPIO)
	{
		  pinMode(pin.second, OUTPUT);
	}
	thetaCloud.addWriteHandler("Relay1", [this](const SensorData& data,
		const ThetaCloud::Emit& emit) {
		trySetSwitch(Switch::one, data);
	});
	thetaCloud.addWriteHandler("Relay2", [this](const SensorData& data,
		const ThetaCloud::Emit& emit) {
		trySetSwitch(Switch::two, data);
	});
}

void ThetaCloudRelay::trySetSwitch(Switch sw, const SensorData& data) const
{
	if (std::string("ON") == data.value)
		setSwitch(sw, true);
	else if (std::string("OFF") == data.value)
		setSwitch(sw, false);
	else
	{
		Serial.print("ThetaCloudRelay: Error: ");
		Serial.print(data.getName());
		Serial.print(" got unknown command: ");
		Serial.println(data.getValue());
	}
}

void ThetaCloudRelay::setSwitch(Switch whichOne, bool enabled) const
{
	digitalWrite(SWITCH_GPIO.at(whichOne), enabled ? HIGH : LOW);
}

ThetaCloudRelay thetaCloudRelay;
