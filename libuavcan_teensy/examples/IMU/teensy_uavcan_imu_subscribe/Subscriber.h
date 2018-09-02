#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/phoenix_msgs/ImuData.hpp>

using namespace uavcan;
using namespace phoenix_msgs;

Subscriber<ImuData> *imu_data_Subscriber;


typedef struct {
  float lin_acc[3];
  float gyro[3];
  float euler[3];
} imu_t;

imu_t imu;

void imu_data_callback(const ImuData& msg)
{
  // TODO
  Serial.println("Call back hit");
  imu.lin_acc[0] = msg.lin_acceleration[0];
  imu.lin_acc[1] = msg.lin_acceleration[1];
  imu.lin_acc[2] = msg.lin_acceleration[2];

  imu.gyro[0] = msg.rate_gyro[0];
  imu.gyro[1] = msg.rate_gyro[1];
  imu.gyro[2] = msg.rate_gyro[2];

  imu.euler[0] = msg.euler[0];
  imu.euler[1] = msg.euler[1];
  imu.euler[2] = msg.euler[2];
  
  Serial.print("Orientation: ");
  Serial.print(imu.euler[0]); Serial.print(", ");
  Serial.print(imu.euler[1]); Serial.print(", ");
  Serial.println(imu.euler[2]);
}


void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  imu_data_Subscriber = new Subscriber<ImuData>(*node);

  if(imu_data_Subscriber->start(imu_data_callback) < 0)
  {
    Serial.println("Unable to start subscriber!");
  } else {
    Serial.println("Subscriber Started");
    
  }
}

#endif
