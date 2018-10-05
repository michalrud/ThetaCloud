#ifndef THETA_CLOUD_I2C_HELPERS_H
#define THETA_CLOUD_I2C_HELPERS_H

#include <Arduino.h>
#include <Wire.h>
#include <array>
#include <utility>

constexpr unsigned int READ_RETRIES = 10;

class WriteToI2C
{
public:
    WriteToI2C(uint8_t address)
    {
        Wire.beginTransmission(address);
    }

    template<typename T>
    WriteToI2C &write(T data)
    {
        Wire.write(data);
        return *this;
    }

    ~WriteToI2C()
    {
        Wire.endTransmission();
    }
};

template<uint8_t N>
std::pair<std::array<uint8_t, N>, bool> ReadFromI2C(uint8_t address)
{
    Wire.requestFrom(address, N);
    {
        unsigned int attempt = 0;
        while (Wire.available() < N) {
            if (++attempt > READ_RETRIES)
                return {
                           std::array<uint8_t, N>(), true
                };                                          // error
            delay(10);
        }
    }
    std::array<uint8_t, N> retval;
    for (uint8_t i = 0; i < N; ++i)
        retval[i] = Wire.read();
    return {
               retval, false
    };
}

#endif  // THETA_CLOUD_I2C_HELPERS_H
