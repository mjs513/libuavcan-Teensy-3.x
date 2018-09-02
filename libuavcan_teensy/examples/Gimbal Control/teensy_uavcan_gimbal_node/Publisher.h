#ifndef	PUBLISHER_H
#define	PUBLISHER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/Status.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/Mode.hpp>
#include "phoenix_can_shield.h"

using namespace uavcan;
using namespace equipment;
using namespace camera_gimbal;


// publisher
Publisher<Status> *StatusPublisher;


// initialize all publisher
void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  StatusPublisher = new Publisher<Status>(*node);

  // initiliaze publishers
  if(StatusPublisher->init() < 0)
  {
    Serial.println("Unable to initialize FixPublisher!");
  } else {
    Serial.println("FixPublisher initialize!");
  }
  // set TX timeout
  StatusPublisher->setTxTimeout(MonotonicDuration::fromUSec(10000));

}

// cycle Fix publisher
void cyclePublisher()
{
  Status Status_msg;
  
  //Status_msg.mode = 0;
  Status_msg.camera_orientation_in_body_frame_xyzw[0] =  SBGC_ANGLE_TO_DEGREE_INT(rt_angle_data.angleData[ROLL].angle);//Roll angle
  Status_msg.camera_orientation_in_body_frame_xyzw[1] =  SBGC_ANGLE_TO_DEGREE_INT(rt_angle_data.angleData[PITCH].angle); //Pitch
  Status_msg.camera_orientation_in_body_frame_xyzw[2] =  0;  //yaw
  Status_msg.camera_orientation_in_body_frame_xyzw[3] =  0; //w - not used - using this message for dual purpose
    
  if (StatusPublisher->broadcast(Status_msg) < 0)
  {
    Serial.println(StatusPublisher->broadcast(Status_msg));
    Serial.print("Broadcastin Error: "); 

  }
}

#endif
