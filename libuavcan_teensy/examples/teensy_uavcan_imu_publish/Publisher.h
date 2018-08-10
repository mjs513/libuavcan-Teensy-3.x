#ifndef	PUBLISHER_H
#define	PUBLISHER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/phoenix_msgs/ImuData.hpp>
#include "phoenix_can_shield.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>


using namespace uavcan;
using namespace phoenix_msgs;

typedef struct {
  imu::Vector<3> lin_acc;
  imu::Vector<3> gyro;
  imu::Vector<3> euler;
} imu_t;

// publisher
Publisher<ImuData> *imuPublisher;


// initialize all publisher
void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  imuPublisher = new Publisher<ImuData>(*node);

  // initiliaze publishers
  if(imuPublisher->init() < 0)
  {
    Serial.println("Unable to initialize imuPublisher!");
  }
  // set TX timeout
  imuPublisher->setTxTimeout(MonotonicDuration::fromUSec(500));

}

// cycle BNO publisher
void cyclePublisherBNO(imu_t bno_data)
{
  ImuData msg;

  msg.timestamp = systemClock->getUtc();

  msg.lin_acceleration[0] = bno_data.lin_acc[0];
  msg.lin_acceleration[1] = bno_data.lin_acc[1];
  msg.lin_acceleration[2] = bno_data.lin_acc[3];

  msg.rate_gyro[0] = bno_data.gyro[0];
  msg.rate_gyro[1] = bno_data.gyro[1];
  msg.rate_gyro[2] = bno_data.gyro[2];

  msg.euler[0] = bno_data.euler[0];
  msg.euler[1] = bno_data.euler[1];
  msg.euler[2] = bno_data.euler[2];

  if (imuPublisher->broadcast(msg) < 0)
  {
    Serial.println("Error while broadcasting key message");
  }
}

#endif
