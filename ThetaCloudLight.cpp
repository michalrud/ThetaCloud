#include "ThetaCloudLight.h"
#include "ThetaCloud.h"

#include "Arduino.h"
#include <isl29023.h>

ThetaCloudLight::ThetaCloudLight() : isl(new ISL29023())
{
}

void ThetaCloudLight::init()
{
	isl->init();
	lightToken = thetaCloud.addReadHandler([this](const ThetaCloud::Emit& emit) {
		emit(SensorData{std::string("light"),
			std::string(dtostrf(isl->read(), 8, 2, this->dtostrfbuf))});
	});
}

ThetaCloudLight thetaCloudLight;
