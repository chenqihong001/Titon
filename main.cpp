// test_absl.cpp
#include <iostream>
#include <chrono>
#include "data.pb.h"
#include <google/protobuf/util/time_util.h>

using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::util;

// 创建温度传感器数据
void createTemperatureData(SensorData& sensor) {
    auto* temp_data = sensor.mutable_temp_data();
    temp_data->set_celsius(25.5f);
    temp_data->set_fahrenheit(77.9f);
}

// 创建湿度传感器数据
void createHumidityData(SensorData& sensor) {
    auto* humidity_data = sensor.mutable_humidity_data();
    humidity_data->set_relative_humidity(65.2f);
}

// 创建运动传感器数据
void createMotionData(SensorData& sensor) {
    auto* motion_data = sensor.mutable_motion_data();
    motion_data->set_acceleration_x(0.1f);
    motion_data->set_acceleration_y(-0.2f);
    motion_data->set_acceleration_z(9.8f);
}

int main() {
    // 初始化 Protobuf 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 示例1: 创建温度传感器数据
    {
        SensorData sensor;
        sensor.set_sensor_id("temp_sensor_001");

        // 设置当前时间戳
        auto now = chrono::system_clock::now();
        *sensor.mutable_timestamp() = TimeUtil::TimeTToTimestamp(std::chrono::system_clock::to_time_t(now));

        createTemperatureData(sensor);

        // 设置采样时长 (10秒)
        *sensor.mutable_sampling_duration() = TimeUtil::SecondsToDuration(10);

        // 序列化为字符串
        string serialized_str;
        if (!sensor.SerializeToString(&serialized_str)) {
            cerr << "Failed to serialize sensor data." << endl;
            return -1;
        }

        cout << "Serialized temperature data size: " << serialized_str.size() << " bytes" << endl;

        // 反序列化
        SensorData parsed_sensor;
        if (!parsed_sensor.ParseFromString(serialized_str)) {
            cerr << "Failed to parse sensor data." << endl;
            return -1;
        }

        // 打印解析结果
        cout << "\nParsed Temperature Sensor Data:" << endl;
        cout << "Sensor ID: " << parsed_sensor.sensor_id() << endl;
        cout << "Timestamp: " << TimeUtil::ToString(parsed_sensor.timestamp()) << endl;
        cout << "Temperature: " << parsed_sensor.temp_data().celsius() << "°C ("
            << parsed_sensor.temp_data().fahrenheit() << "°F)" << endl;
        cout << "Sampling Duration: " << TimeUtil::DurationToSeconds(parsed_sensor.sampling_duration())
            << " seconds" << endl;
    }

    // 示例2: 创建湿度传感器数据
    {
        SensorData sensor;
        sensor.set_sensor_id("humidity_sensor_001");
        *sensor.mutable_timestamp() = TimeUtil::GetCurrentTime();
        createHumidityData(sensor);
        *sensor.mutable_sampling_duration() = TimeUtil::MillisecondsToDuration(500); // 500ms

        cout << "\nHumidity Sensor Data:" << endl;
        cout << "Relative Humidity: " << sensor.humidity_data().relative_humidity() << "%" << endl;
    }

    // 示例3: 创建运动传感器数据
    {
        SensorData sensor;
        sensor.set_sensor_id("motion_sensor_001");
        *sensor.mutable_timestamp() = TimeUtil::GetCurrentTime();
        createMotionData(sensor);
        *sensor.mutable_sampling_duration() = TimeUtil::MicrosecondsToDuration(100000); // 100ms

        cout << "\nMotion Sensor Data:" << endl;
        cout << "Acceleration X: " << sensor.motion_data().acceleration_x() << " g" << endl;
        cout << "Acceleration Y: " << sensor.motion_data().acceleration_y() << " g" << endl;
        cout << "Acceleration Z: " << sensor.motion_data().acceleration_z() << " g" << endl;
    }

    // 清理 Protobuf 库
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}