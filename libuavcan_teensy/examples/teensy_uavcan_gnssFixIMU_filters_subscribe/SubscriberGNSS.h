#ifndef	SUBSCRIBERGNSS_HPP
#define	SUBSCRIBERGNSS_HPP

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/gnss/Fix.hpp>

using namespace uavcan;
using namespace equipment;
using namespace gnss;

Subscriber<Fix> *Fix_data_Subscriber;


void Fix_data_callback(const Fix& msg)
{
  // TODO
  Serial.print("GNSS Fix: ");
  Serial.print(msg.gnss_time_standard); Serial.print(", ");
  Serial.print(msg.num_leap_seconds); Serial.print(", ");
  Serial.print(msg.longitude_deg_1e8/1e8); Serial.print(", "); 
  Serial.println(msg.latitude_deg_1e8/1e8);
     
  Serial.print(msg.height_ellipsoid_mm);Serial.print(", ");
  Serial.print(msg.height_msl_mm);Serial.print(", ");
  Serial.print(msg.ned_velocity[0]);Serial.print(", ");
  Serial.print(msg.ned_velocity[1]);Serial.print(", ");
  Serial.println(msg.ned_velocity[2]);
  
  Serial.print(msg.sats_used);Serial.print(", ");
  Serial.print(msg.status);Serial.print(", ");
  Serial.print(msg.pdop);
  Serial.println();

}


void initSubscriberGNSS(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
  Fix_data_Subscriber = new Subscriber<Fix>(*node);

  if(Fix_data_Subscriber->start(Fix_data_callback) < 0)
  {
    Serial.println("Unable to start GNSS subscriber!");
  } else {
    Serial.println("GNSS Subscriber Started");
  }
}

#endif
