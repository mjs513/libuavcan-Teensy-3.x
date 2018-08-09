#ifndef	PUBLISHER_H
#define	PUBLISHER_H

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/air_data/sideslip.hpp>
#include "phoenix_can_shield.h"


using namespace uavcan;
using namespace equipment;
using namespace air_data;


// publisher
Publisher<Sideslip> *SideslipPublisher;


// initialize all publisher
void initPublisher(Node<NodeMemoryPoolSize> *node)
{
  // create publishers
  SideslipPublisher = new Publisher<Sideslip>(*node);

  // initiliaze publishers
  if(SideslipPublisher->init() < 0)
  {
    Serial.println("Unable to initialize imuPublisher!");
  }
  // set TX timeout
  SideslipPublisher->setTxTimeout(MonotonicDuration::fromUSec(500));

}

// cycle SS publisher
void cyclePublisherSS()
{
  equipment::air_data::Sideslip sideslip_msg;
  sideslip_msg.sideslip_angle = random(10, 20) / float(20);
  sideslip_msg.sideslip_angle_variance = random(1,4) / float(4);


  if (SideslipPublisher->broadcast(sideslip_msg) < 0)
  {
    Serial.println("Error while broadcasting key message");
  }
}

#endif
