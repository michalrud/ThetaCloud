#pragma once
#include <memory>
#include "gmock/gmock.h"

struct NiceWireMock
{
    MOCK_METHOD2(begin, void(uint8_t, uint8_t));
    MOCK_METHOD2(requestFrom, void(uint8_t, uint8_t));
    MOCK_METHOD1(beginTransmission, void(uint8_t));
    MOCK_METHOD1(write, void(uint8_t));
    MOCK_METHOD0(available, uint8_t());
    MOCK_METHOD0(read, uint8_t());
    MOCK_METHOD0(endTransmission, void());
};
using WireMock = ::testing::StrictMock<NiceWireMock>;

struct WireProxy
{
    void begin(uint8_t a, uint8_t b)
    {
        mock->begin(a, b);
    }

    void beginTransmission(uint8_t a)
    {
        mock->beginTransmission(a);
    }

    void endTransmission()
    {
        mock->endTransmission();
    }

    void write(uint8_t a)
    {
        mock->write(a);
    }

    void requestFrom(uint8_t address, uint8_t n)
    {
        mock->requestFrom(address, n);
    }

    uint8_t available()
    {
        return mock->available();
    }

    uint8_t read()
    {
        return mock->read();
    }

    std::unique_ptr<WireMock> mock;
};

extern WireProxy Wire;
