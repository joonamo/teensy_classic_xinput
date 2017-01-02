/* 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Use Teensy XInput library from here: https://github.com/zlittell/MSF-XINPUT
 * 
 * Adapted from http://havencking.blogspot.com
 *
 * For more info about the Wii Classic Controller, see:
 * http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
 */


// the improved Wire I2C library
// see:  https://www.pjrc.com/teensy/td_libs_Wire.html
#include "i2c_t3.h"
#include <xinput.h>
#define LEDPIN 13
#define ShoulderSwap 2
#define KeySwap 3
#define AnalogShoulders 4
#define ResetPin 5

XINPUT controller(LED_ENABLED, LEDPIN);

// an array to store the six bytes of controller status
int buttons[6];

// the analog controls
int LX = 32;
int LY = 32;
int LT;
int RX = 16;
int RY = 16;
int RT;
int trigger_deadzone = 6;

// the digital controls
bool BDR;
bool BDD;
bool BLT;
bool Bminus;
bool BH;
bool Bplus;
bool BRT;
bool BZL;
bool BB;
bool BY;
bool BA;
bool BX;
bool BZR;
bool BDL;
bool BDU;

bool shoulder_swap;

bool was_connected;

void feed_data_to_xinput()
{
  // Update buttons
  controller.buttonUpdate(BUTTON_A, !BB);
  controller.buttonUpdate(BUTTON_B, !BA);
  controller.buttonUpdate(BUTTON_X, !BY);
  controller.buttonUpdate(BUTTON_Y, !BX);
  // These buttons don't actually exist on classic controller, hope you don't need them
  //controller.buttonUpdate(BUTTON_L3, !);
  //controller.buttonUpdate(BUTTON_R3, !);
  controller.buttonUpdate(BUTTON_START, !Bplus);
  controller.buttonUpdate(BUTTON_BACK, !Bminus);
  controller.buttonUpdate(BUTTON_LOGO, !BH);

  // Most importantly, update DPAD (this is why I want to play with Classic Controller!)
  controller.dpadUpdate(!BDU, !BDD, !BDL, !BDR);

  // Digital triggers should be enough for everyone
  BLT = BLT && LT < trigger_deadzone;
  BRT = BRT && RT < trigger_deadzone;
  if (shoulder_swap)
  {
    controller.triggerUpdate((BZL ? 0 : 0xFF), (BZR ? 0 : 0xFF));
    controller.buttonUpdate(BUTTON_LB, !BLT);
    controller.buttonUpdate(BUTTON_RB, !BRT);
  }
  else
  {
    controller.triggerUpdate((BLT ? 0 : 0xFF), (BRT ? 0 : 0xFF));
    controller.buttonUpdate(BUTTON_LB, !BZL);
    controller.buttonUpdate(BUTTON_RB, !BZR);
  }

  // Update sticks, xinput allows whole lot more precision than the Classic Controller can afford
  controller.stickUpdate(STICK_LEFT, (LX - 32) * 1000, (LY - 32) * 1000);
  controller.stickUpdate(STICK_RIGHT, (RX - 16) * 2000, (RY - 16) * 2000);

  //Update the LED display
  controller.LEDUpdate();

  //Send data
  controller.sendXinput();

  //Receive data
  controller.receiveXinput();
}

void reset_values() {
  LX = 32;
  LY = 32;
  LT = 0;
  RX = 16;
  RY = 16;
  RT = 0;

  BDR = true;
  BDD = true;
  BLT = true;
  Bminus = true;
  BH = true;
  Bplus = true;
  BRT = true;
  BZL = true;
  BB = true;
  BY = true;
  BA = true;
  BX = true;
  BZR = true;
  BDL = true;
  BDU = true;

  was_connected = false;
}

void setup() {
  Wire.begin();

  pinMode(ShoulderSwap, INPUT_PULLUP);
  pinMode(KeySwap, INPUT_PULLUP);
  pinMode(AnalogShoulders, INPUT_PULLUP);
  pinMode(ResetPin, INPUT_PULLUP);

  reset_values();
}

void loop() {
  // Make sure the controller is connected.
  // I have the middle pin connected to teensy-lc pin 14 
  if (analogRead(A0) < 500)
  {
    // Controller not connected
    if (was_connected)
    {
      // Was connected previously, reset everything
      reset_values();
      /*digitalWrite(13, LOW);*/
    }

  }
  else 
  {
    // Something seems to be  connected
    if (was_connected)
    {
      // Talk to the controller, it should be available

      // request the 6 bytes of controller status and store in the buttons array
      Wire.requestFrom(0x52, 6);
      while (Wire.available()) {

        for (int i = 0; i < 6; i++) {
          buttons[i] = Wire.receive();
        }

        // parse the controller data
        // analog controls
        LX = buttons[0] & B00111111;
        LY = buttons[1] & B00111111;
        LT = ((buttons[2] >> 2) & B00011000) | ((buttons[3] >> 5) & B00000111);
        RX = ((buttons[0] >> 3) & B00011000) | ((buttons[1] >> 5) & B00000110) | ((buttons[2] >> 7) & B00000001);
        RY = buttons[2] & B00011111;
        RT = buttons[3] & B00011111;

        // digital controls
        BDR = (buttons[4] >> 7) & B00000001;
        BDD = (buttons[4] >> 6) & B00000001;
        BLT = (buttons[4] >> 5) & B00000001;
        Bminus = (buttons[4] >> 4) & B00000001;
        BH = (buttons[4] >> 3) & B00000001;
        Bplus = (buttons[4] >> 2) & B00000001;
        BRT = (buttons[4] >> 1) & B00000001;
        BZL = (buttons[5] >> 7) & B00000001;
        BB = (buttons[5] >> 6) & B00000001;
        BY = (buttons[5] >> 5) & B00000001;
        BA = (buttons[5] >> 4) & B00000001;
        BX = (buttons[5] >> 3) & B00000001;
        BZR = (buttons[5] >> 2) & B00000001;
        BDL = (buttons[5] >> 1) & B00000001;
        BDU = buttons[5] & B00000001;

        // restart at the beginning of the address space
        Wire.beginTransmission(0x52);
        Wire.write(0x00);
        Wire.endTransmission();

        shoulder_swap = digitalRead(ShoulderSwap);
      }

    }

    else
    {
      // Start Talking to controller
      // Short delay to allow Classic Controller to wake up
      delay(200);

      // initialize Wii I2C using "the new way"
      // see: http://wiibrew.org/wiki/Wiimote/Extension_Controllers
      Wire.beginTransmission(0x52); // Wii Classic 7-bit I2C address is 0x52
      Wire.write(0xF0);
      Wire.write(0x55);
      Wire.endTransmission();
      Wire.beginTransmission(0x52);
      Wire.write(0xFB);
      Wire.write(0x00);

      was_connected = Wire.endTransmission();
      /*if (was_connected)
      {
        digitalWrite(13, HIGH);
      }*/


    }
  }

  feed_data_to_xinput();

  if (digitalRead(ResetPin))
  {
    delay(2000);
    if (digitalRead(ResetPin))
      controller.bootloaderJump();
  }

  delay(1); // This seems to be necessary or the Classic Controller chokes
}
