#ifndef	SUBSCRIBER_H
#define	SUBSCRIBER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/camera_gimbal/AngularCommand.hpp>
#include "phoenix_can_shield.h"

using namespace uavcan;
using namespace equipment;
using namespace camera_gimbal;

Subscriber<AngularCommand> *AngularCommandSubscriber;


void AngularCmd_callback(const AngularCommand& AngularCommand_msg)
{
        angleR = 
            AngularCommand_msg.quaternion_xyzw[0];
        angleP =
            AngularCommand_msg.quaternion_xyzw[1];
}


// initialize all publisher
void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  AngularCommandSubscriber = new Subscriber<AngularCommand>(*node);

  if(AngularCommandSubscriber->start(AngularCmd_callback) < 0)
  {
    Serial.println("Unable to start subscriber!");
  } else {
    Serial.println("Subscriber Started");
  }

}

#endif
