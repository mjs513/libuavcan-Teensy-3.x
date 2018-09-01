#include "Arduino.h"

// CAN Node settings
static constexpr uint32_t nodeID = 101;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.phoenix.subscriber";

// application settings
static constexpr float framerate = 100;

#include "phoenix_can_shield.h"
#include "Subscriber.h"

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
  //get EEPROM Parameters
  //readParamsFromEEPROM();
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
  
  // init subscriber
  initSubscriber(node);
  
  // set up filters
  configureCanAcceptanceFilters(*node);


  // start up node
  node->setModeOperational();

  Serial.println("Node Setup Finished");


}

uint32_t t_ = 0;
void loop() {
  // wait in cycle

  uint32_t t = micros();
  Serial.print("CPU Load: ");
  Serial.println((float)(t-t_)/100);
  cycleWait(framerate);
  t_ = micros();


  // get RC data, high level commands, motor telemetry rear motors
  cycleNode(node);
  
  // toggle heartbeat
  //toggleHeartBeat();
}
