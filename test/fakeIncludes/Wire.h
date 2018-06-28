#pragma once
#include <memory>
#include "gmock/gmock.h"

struct WireMock
{
	MOCK_METHOD2(begin, void(int, int));
};

struct WireProxy
{
	void begin(int a, int b) {mock->begin(a, b);}
	std::unique_ptr<WireMock> mock;
};

extern WireProxy Wire;
