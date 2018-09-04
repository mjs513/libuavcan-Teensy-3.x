#include "Arduino.h"

// CAN Node settings
static constexpr uint32_t nodeID = 100;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.uavcan.subscriber";

// application settings
static constexpr float framerate = 100;

#include "phoenix_can_shield.h"
#include "SubscriberGNSS.h"
#include "SubscriberIMU.h"
#include <uavcan/helpers/ostream.hpp>

// this is important for UAVCAN to compile
namespace std {
  void __throw_bad_alloc()
  {
    Serial.println("Unable to allocate memory");
    for(;;){}
  }

  void __throw_length_error( char const*e )
  {
    Serial.print("Length Error :");
    Serial.println(e);
    for(;;){}
  }

  void __throw_bad_function_call()
  {
    Serial.println("Bad function call!");
    for(;;){}
  }
}

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("Setup");
  
  // init LEDs
  initLeds();

  // create a node
  systemClock = &initSystemClock();
  canDriver = &initCanDriver();
  node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
  bool nodeStart = initNode(node, nodeID, nodeName, swVersion, hwVersion);
  
    /**
     * The most common way to configure CAN hardware acceptance filters is
     * just to use configureCanAcceptanceFilters()
     * method with a node you want to configure as an input argument. This
     * method usually called after all the node's
     * subscribers have been declared and it calls computeConfiguration()
     * and applyConfiguration() subsequently (these
     * functions will be described later in this tutorial).
     * At this point your HW filters will be configured to accept 
     * anonymous messages, since we don't have any subscribers declared so
     *far. You may already
     * execute publisher_client which will constantly send messages to this
     *node.
     */
    configureCanAcceptanceFilters(*node);
    
    /**
     * Initializing a subscriber uavcan::equipment::gnss::Fix. Please refer
     * to the "Publishers and subscribers" tutorial for more details.
     * We are still not getting any messages from publisher_client node,
     * since the gnss::Fix was declared after acceptance filters have been
     * configured.
     */
    initSubscriberGNSS(node);
    
    /**
     * Will not receive air_data::Sideslip messages for another 3 seconds until we reconfigure acceptance filters and
     * include another configuration for the message. As soon as filters are reconfigured the node will start to
     * receive the air_data::Sideslip messages.
     */
    //node->spin(uavcan::MonotonicDuration::fromMSec(3000)); // Wait 3 seconds
    delay(3000);
    OStream::instance() << OStream::endl << "Reconfiguring acceptance filters ..." << OStream::endl;
    configureCanAcceptanceFilters(*node);

    /**
     * In case you need to create custom configuration of hardware
     * acceptance filters, or if you want to output your
     * computed configurations, the following approach should be used.
     *
     * The first step to configure CAN hardware acceptance filters is to
     * create an object of class uavcan::CanAcceptanceFilterConfigurator
     * with the desired node as an input argument.
     *
     * This object haven't configured anything yet, in order to make the
     * configuration proceed to the next step.
     */
    uavcan::CanAcceptanceFilterConfigurator anon_test_configuration(*node);    
  
    /*
     * Initializing another subscriber
     * custom message IMU.
     */  
    initSubscriberIMU(node);
  
    /**
     * The method computeConfiguration() gathers information from all
     * subscribers and service messages on the configurator's node. 
     * Also it creates a container with automatically calculated 
     * configurations of Masks and ID's and subsequently loads the
     * configurations to the CAN controller.
     *
     * It may or may not take an argument:
     * - IgnoreAnonymousMessages
     * - AcceptAnonymousMessages (default, if no input arguments)
     *
     * By default filter configurator accepts all anonymous messages. 
     * If you don't want them, you may specify the
     * IgnoreAnonymousMessages input argument. Let's make configurator 
     * that accepts anonymous messages.
     */
    anon_test_configuration.computeConfiguration();

    /**
     * At this point you have your configuration calculated and stored
     * within the class container multiset_configs_, but
     * it's not still applied. Let's take a look what is inside the
     *container of configurations using method
     * getConfiguration(). The output should consist of four configurations:
     * service, anonymous, GNSS and IMU messages.
     */
    auto& configure_array = anon_test_configuration.getConfiguration();
    uint16_t configure_array_size = configure_array.getSize();

    OStream::instance() << OStream::endl << "Configuration with AcceptAnonymousMessages input and two subscribers:" << OStream::endl;
    for (uint16_t i = 0; i < configure_array_size; i++)
    {
        OStream::instance() << "config.ID [" << i << "]= " << configure_array.getByIndex(i)->id << OStream::endl;
        OStream::instance() << "config.MK [" << i << "]= " << configure_array.getByIndex(i)->mask << OStream::endl;
    }
    OStream::instance() << OStream::endl;
    
    /**
     * Our node keeps getting GNSS messages since
     * configureCanAcceptanceFilters() was called previously
     * and service message file::BeginFirmwareUpdate since the service 
     * was declared (always accepted by UAVCAN, again).
     * But the IMU is still not being accepted, 
     * even though it's within the multiset_configs_container. 
     * In order to carry out the configuration let's call
     * applyConfiguration() in 3 seconds and start to
     * get another message.
     */
    //node->spin(MonotonicDuration::fromMSec(3000)); // Wait 3 seconds
    delay(3000);
    OStream::instance() << OStream::endl << "Applying new configuration, IMU is accepted now..." << OStream::endl;
    anon_test_configuration.applyConfiguration();

    //node->spin(MonotonicDuration::fromMSec(3000)); // Wait 3 seconds
    delay(3000);
    
  // start up node
  node->setModeOperational();

  Serial.println("Node Setup Finished");

}

uint32_t t_ = 0;
void loop() {
  // wait in cycle

  //uint32_t t = micros();
  //Serial.print("CPU Load: ");
  //Serial.println((float)(t-t_)/100);
  //cycleWait(framerate);
  //t_ = micros();

  // get RC data, high level commands, motor telemetry rear motors
  cycleNode(node);
  
  // toggle heartbeat
  //toggleHeartBeat();
}
