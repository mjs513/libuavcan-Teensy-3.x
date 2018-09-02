#ifndef	PUBLISHER_H
#define	PUBLISHER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/AngularCommand.hpp>
#include "phoenix_can_shield.h"

using namespace uavcan;
using namespace equipment;
using namespace camera_gimbal;

// publisher
Publisher<AngularCommand> *AngularCommandPublisher;

// initialize all publisher
void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  AngularCommandPublisher = new Publisher<AngularCommand>(*node);

  // initiliaze publishers
  if(AngularCommandPublisher->init() < 0)
  {
    Serial.println("Unable to initialize FixPublisher!");
  } else {
    Serial.println("FixPublisher initialize!");
  }
  // set TX timeout
  AngularCommandPublisher->setTxTimeout(MonotonicDuration::fromUSec(1000));

}

// cycle Fix publisher
void cyclePublisher()
{

    //Read acual GPS data here in place of test data
    AngularCommand AngularCommand_msg;
    AngularCommand_msg.gimbal_id = 1;
    AngularCommand_msg.quaternion_xyzw[0] = angleR;
    AngularCommand_msg.quaternion_xyzw[1] = angleP;
    AngularCommand_msg.quaternion_xyzw[2]  = 0;
    AngularCommand_msg.quaternion_xyzw[3]  = 0;
    
    if (AngularCommandPublisher->broadcast(AngularCommand_msg) < 0)
    {
      Serial.print("Error while broadcasting key message: ");
      Serial.println(AngularCommandPublisher->broadcast(AngularCommand_msg));
    }
}

#endif
