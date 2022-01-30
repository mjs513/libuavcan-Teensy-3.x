/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2022 Bolder Flight Systems Inc
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

/* Simple UAV CAN subscriber to receive and print true airspeed */

#include "Arduino.h"
#include "uavcan.h"
#include "uavcan/uavcan_teensy/uavcan_teensy.hpp"
#include "uavcan/transport/can_acceptance_filter_configurator.hpp"
#include "uavcan/dsdlc/equipment/air_data/TrueAirspeed.hpp"

/* Defines needed for restart */
#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

/* Node constants */
static constexpr uint32_t NODE_ID = 102;
static constexpr uint8_t SW_VER = 1;
static constexpr uint8_t HW_VER = 1;
static const char* NODE_NAME = "TEST";
static const uint32_t NODE_MEM = 8192;  // size of node memory

/* System clock and CAN driver */
ISystemClock *clock = &uavcan_teensy::SystemClock::instance();
ICanDriver *can;

/* Node and subscriber */
Node<NODE_MEM> *node;
Subscriber<equipment::air_data::TrueAirspeed> *sub;

/* Function to restart Teensy, inherits from UAVCAN */
class : public IRestartRequestHandler {
  bool handleRestartRequest(NodeID request_source) override {
    Serial.println("Got a remote restart request!");
    WRITE_RESTART(0x5FA0004);
    return true;
  }
} restart_request_handler;

void DataCallback(const equipment::air_data::TrueAirspeed &ref) {
  Serial.println(ref.true_airspeed);
}

void setup() {
  /* Serial to print feedback */
  Serial.begin(115200);
  while (!Serial) {}
  Serial.println("Demo started");
  /* Init CAN transceivers - this is HW config dependent */
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  digitalWriteFast(26, LOW);
  digitalWriteFast(27, LOW);
  /* Init CAN driver */
  uavcan_teensy::IfaceParams iface_param[] = {
    /* Config CAN0 */
    {
      .bitrate=1000000,               // bit rate of can bus
      .tx_buff_size=64,               // TX ring buffer size
      .rx_buff_size=64,               // RX ring buffer size
      .use_alt_tx_pin = false,        // use alternative TX pin
      .use_alt_rx_pin = false,        // use alternative RX pin
      .dis_all_RX_by_default = false  // disables all RX mailboxes
    }
  };
  uavcan_teensy::CanDriver::instance().init(iface_param);
  can = &uavcan_teensy::CanDriver::instance();
  /* Init Node */
  node = new Node<NODE_MEM>(*can, *clock);
  protocol::SoftwareVersion sw_ver;
  protocol::HardwareVersion hw_ver;
  sw_ver.major = SW_VER;
  sw_ver.minor = 0;
  hw_ver.major = HW_VER;
  hw_ver.minor = 0;
  node->setNodeID(NODE_ID);
  node->setName(NODE_NAME);
  node->setSoftwareVersion(sw_ver);
  node->setHardwareVersion(hw_ver);
  node->setRestartRequestHandler(&restart_request_handler);
  if (node->start() < 0) {
    Serial.println("ERROR starting node");
    while (1) {}
  }
  Serial.println("Node initialized");
  /* Init subscriber */
  sub = new Subscriber<equipment::air_data::TrueAirspeed>(*node);
  if (sub->start(DataCallback) < 0) {
    Serial.println("ERROR initializing publisher");
    while (1) {}
  }
  Serial.println("Subscriber initialized");
  /* CAN acceptance filters */
  configureCanAcceptanceFilters(*node);
  /* Set Node mode to operational */
  node->setModeOperational();
  Serial.println("Setup complete");
}

void loop() {
  /* Check the node */
  if (node->spinOnce() < 0) {
    Serial.println("WARNING issue spinning node");
  }
}
