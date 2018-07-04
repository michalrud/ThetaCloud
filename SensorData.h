#ifndef THETA_CLOUD_SENSOR_DATA_H
#define THETA_CLOUD_SENSOR_DATA_H

#include <string>

struct SensorData
{
    const std::string name;
    const std::string value;
    const char *getName() const
    {
        return name.c_str();
    }

    const char *getValue() const
    {
        return value.c_str();
    }
};

#endif  // THETA_CLOUD_SENSOR_DATA_H
