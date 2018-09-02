#include "Arduino.h"

// CAN Node settings
static constexpr uint32_t nodeID = 104;
static constexpr uint8_t swVersion = 1;
static constexpr uint8_t hwVersion = 1;
static const char* nodeName = "org.uavcan.gimbal";

//Gimbal API Setup
#include <inttypes.h>
#include <SBGC.h>
#include <SBGC_Arduino.h>


////////////////////////////////////////////////
// Serial baud rate should match with the rate, configured for the SimpleBGC controller
#define SERIAL_SPEED 115200

// delay between commands, ms
#define SBGC_CMD_DELAY 10

// Set serial port where SBGC32 is connected
#define serial Serial1  // Connect Gimbal to Serial1
#define telem Serial

////////////////////////////////////////
static SBGC_cmd_realtime_data0_t rt_data0;
static SBGC_cmd_realtime_data_t rt_data;
static SBGC_cmd_getAngles_data_t rt_angle_data;

#define MAX_WAIT_TIME_MS 2000  // time to wait for incoming commands to be in CONNECTED state
#define REALTIME_DATA_REQUEST_INTERAL_MS 5 // interval between reatime data requests

static uint16_t cur_time_ms, last_cmd_time_ms; //last_bt_connect_ms;
static uint8_t is_connected = 0;

// application settings
static constexpr float framerate = 100;
int angle, angleP, angleR;

#include "phoenix_can_shield.h"
#include "Publisher.h"
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
  delay(3000);
  Serial.begin(115200);
  serial.begin(SERIAL_SPEED);
  Serial.println("Setup");
  
  // init LEDs
  initLeds();

  // create a node
  systemClock = &initSystemClock();
  canDriver = &initCanDriver();
  node = new Node<NodeMemoryPoolSize>(*canDriver, *systemClock);
  bool nodeStart = initNode(node, nodeID, nodeName, swVersion, hwVersion);
  
  // init publisher
  initPublisher(node);
  initSubscriber(node);
  
  // set up filters
  configureCanAcceptanceFilters(*node);

  // start up node
  node->setModeOperational();

  Serial.println("Node Setup Finished");

  //Gimbal
  Serial.println("Gimbal Initialization\n");
  SBGC_Demo_setup(&serial);
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

  setAngle();

  cyclePublisher();
  
  // toggle heartbeat
  //toggleHeartBeat();
}


void setAngle(){
    //mode; speedROLL; angleROLL; speedPITCH; anglePITCH; speedYAW; angleYAW;
    SBGC_cmd_control_t c = { 0, 0, 0, 0, 0, 0, 0 };
    c.mode = SBGC_CONTROL_MODE_ANGLE;
    c.speedROLL = c.speedPITCH = 100 * SBGC_SPEED_SCALE;
    c.angleROLL = SBGC_DEGREE_TO_ANGLE(angleR);
    c.anglePITCH = SBGC_DEGREE_TO_ANGLE(angleP);
    SBGC_cmd_control_send(c, sbgc_parser);
    delay(SBGC_CMD_DELAY);

    ////////// Request realtime data
    readAngles();
    delay(200);
}

void readAngles(){
      SerialCommand cmd;
      if(is_connected) {
        cmd.init(SBGC_CMD_GET_ANGLES);
      } else { // Set version request to init connection
        cmd.init(SBGC_CMD_BOARD_INFO);
      }
      sbgc_parser.send_cmd(cmd, 0);
    delay(REALTIME_DATA_REQUEST_INTERAL_MS);
    process_in_queue();
}

// Process incoming commands. Call it as frequently as possible, to prevent overrun of serial input buffer.
void process_in_queue() {
  while(sbgc_parser.read_cmd()) {
    SerialCommand &cmd = sbgc_parser.in_cmd;
    last_cmd_time_ms = cur_time_ms;
    if(!is_connected) set_connected();
    
    uint8_t error = 0;
    switch(cmd.id) {
    // Receive realtime data
    case SBGC_CMD_GET_ANGLES:
      error = SBGC_cmd_getangles_data_unpack(rt_angle_data, cmd);
      //Serial.print("ERROR: ");Serial.println(error);
      if(!error) {
        Serial.print("Pitch: "); 
        Serial.print(SBGC_ANGLE_TO_DEGREE_INT(rt_angle_data.angleData[PITCH].angle));
        Serial.print(", "); Serial.print("Roll: ");  
        Serial.println(SBGC_ANGLE_TO_DEGREE_INT(rt_angle_data.angleData[ROLL].angle));
       } else {
        sbgc_parser.onParseError(error);
      }
      break;         
    case SBGC_CMD_REALTIME_DATA:
      error = SBGC_cmd_realtime_data0_unpack(rt_data0, cmd);
      //Serial.print("ERROR: ");Serial.println(error);
      if(!error) {
        Serial.print("Pitch: "); 
        Serial.print(SBGC_ANGLE_TO_DEGREE_INT(rt_data0.camera_angle[PITCH]));
        Serial.print(", "); Serial.print("Roll: ");  
        Serial.println(SBGC_ANGLE_TO_DEGREE_INT(rt_data0.camera_angle[ROLL]));
      } else {
        sbgc_parser.onParseError(error);
      }
      break;    
      
    case SBGC_CMD_REALTIME_DATA_3:
    case SBGC_CMD_REALTIME_DATA_4:
      error = SBGC_cmd_realtime_data_unpack(rt_data, cmd);
      Serial.print("ERROR: ");Serial.println(error);
      if(!error) {
        // Extract some usefull data
        // Average stabilization error (0.001 degree)
      } else {
        sbgc_parser.onParseError(error);
      }
      break;    
    }
  }
  
  // If no commands for a long time, set connected state to false
  if(is_connected && (uint16_t)(cur_time_ms - last_cmd_time_ms) > MAX_WAIT_TIME_MS) {
    is_connected = 0;
  }
}

// Called once on a connection established
void set_connected() {
  is_connected = 1;
}
