#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/Status.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/Mode.hpp>

using namespace uavcan;
using namespace equipment;
using namespace camera_gimbal;

Subscriber<Status> *camStatus_Subscriber;


void camStatus_callback(const Status& msg)
{
  // TODO
  Serial.print("Gimbal Status: ");
  Serial.print(msg.camera_orientation_in_body_frame_xyzw[0]);Serial.print(", ");  //Roll angle
  Serial.print(msg.camera_orientation_in_body_frame_xyzw[1]);Serial.print(", ");  //Pitch
  Serial.println();

}


void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  camStatus_Subscriber = new Subscriber<Status>(*node);

  if(camStatus_Subscriber->start(camStatus_callback) < 0)
  {
    Serial.println("Unable to start subscriber!");
  } else {
    Serial.println("Subscriber Started");
  }
}

#endif
