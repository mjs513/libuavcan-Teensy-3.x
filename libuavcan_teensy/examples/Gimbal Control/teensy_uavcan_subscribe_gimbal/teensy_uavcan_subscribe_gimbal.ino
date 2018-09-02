#include "Arduino.h"

// CAN Node settings
static constexpr uint32_t nodeID = 101;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.uavcan.gimbal.subscriber";

// application settings
static constexpr float framerate = 100;
int angle, angleP, angleR;
#define telem Serial  // Serial1 for 3dr Radio

#include "phoenix_can_shield.h"
#include "Subscriber.h"
#include "Publish.h"

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
  //telem.begin(115200);
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
  initPublisher(node);
  
  // set up filters
  configureCanAcceptanceFilters(*node);

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
  
    if (telem.available() > 0)
    {
        int val = telem.read();	//read telem input commands
        angle = telem.parseInt();
        switch(val)
        {
          case 'P' :
            angleP = angle;
            break;
          case 'R' :
            angleR = angle;
            break;
          case 'Z' :
            angleP = 0;
            angleR = 0;
            break;
          case 'L' :
            angleP = -17;
            angleR = 0;
            break;
        }
    }
        

  // get RC data, high level commands, motor telemetry rear motors
  cycleNode(node);

  cyclePublisher();
  
  // toggle heartbeat
  //toggleHeartBeat();
}
