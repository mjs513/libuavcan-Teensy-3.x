#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/air_data/sideslip.hpp>

using namespace uavcan;
using namespace equipment;
using namespace air_data;

Subscriber<Sideslip> *ss_data_Subscriber;


void ss_data_callback(const Sideslip& msg)
{
  // TODO
  Serial.println("Call back hit");
  Serial.print("SideSlip: ");
  Serial.print(msg.sideslip_angle); Serial.print(", ");
  Serial.println(msg.sideslip_angle_variance);
}


void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  ss_data_Subscriber = new Subscriber<Sideslip>(*node);

  if(ss_data_Subscriber->start(ss_data_callback) < 0)
  {
    Serial.println("Unable to start subscriber!");
  } else {
    Serial.println("Subscriber Started");
    
  }
}

#endif
