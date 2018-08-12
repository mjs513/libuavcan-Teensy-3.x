#ifndef	SUBSCRIBER_HPP
#define	SUBSCRIBER_HPP

#define printf Serial.printf 

#include <uavcan/uavcan.hpp>
#include <uavcan/dsdlc/equipment/gnss/Fix.hpp>
#include <uavcan/helpers/ostream.hpp>

using namespace uavcan;
using namespace equipment;
using namespace gnss;


void initSubscriber(Node<NodeMemoryPoolSize> *node)
{
  // create a subscriber
   static Subscriber<Fix> Fix_data_Subscriber(*node);

    const int res = Fix_data_Subscriber.start(
        [&](const ReceivedDataStructure<Fix>& msg)
        {
            /*
             * The message will be streamed in YAML format.
             */
            //std::cout << msg << std::endl;
            /*
             * If the standard iostreams are not available (they rarely available in embedded environments),
             * use the helper class uavcan::OStream defined in the header file <uavcan/helpers/ostream.hpp>.
             */
            //OStream::instance() << msg.longitude_deg_1e8/1e8 << OStream::endl; 
            OStream::instance() << msg << OStream::endl << OStream::endl;
        });

  if(res < 0)
  {
    Serial.print("Unable to start subscriber!  ");
    Serial.println(res);
  } else {
    Serial.println("Subscriber Started");
  }
}

#endif
