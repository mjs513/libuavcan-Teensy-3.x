#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// CAN Node settings
static constexpr uint32_t nodeID = 102;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.phoenix.ImuData";

// application settings
static constexpr float framerate = 100;

#include "phoenix_can_shield.h"
#include "Publisher.h"

// BNO055 imu
Adafruit_BNO055 bno055 = Adafruit_BNO055();

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
  Serial.begin(115200);
  Serial.println("Setup");
  
  // init LEDs
  //initLeds();

  // create a node
  systemClock = &initSystemClock();
  canDriver = &initCanDriver();
  node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
  bool nodeStart = initNode(node, nodeID, nodeName, swVersion, hwVersion);
  
  // init publisher
  initPublisher(node);


  // set up filters
  configureCanAcceptanceFilters(*node);


  // start up node
  node->setModeOperational();

  Serial.println("Node Setup Finished");

  // set up BNO055 IMU Adafruit_Sensor
  /* Initialise the sensor */
  if(!bno055.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);
  bno055.setExtCrystalUse(true);

  Serial.println("BNO55 setup completed..");

}

imu_t bno_data;

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

  // BNO055 data aquisition
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  bno_data.lin_acc = bno055.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno_data.gyro = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno_data.euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);
  
  // cycle publisher
  cyclePublisherBNO(bno_data);

  cycleWait(1000);
  // toggle heartbeat
  toggleHeartBeat();
}
