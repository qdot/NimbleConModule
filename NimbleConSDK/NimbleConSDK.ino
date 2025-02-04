// Use "ESP32 Dev Module" as board

#include "nimbleCon.h"

bool btLED = 0;

void setup() {
  // put your setup code here, to run once:
  initNimbleSDK();

  /*ledcWrite(8, 50);
  ledcWrite(9, 50);
  ledcWrite(10, 50);
  ledcWrite(11, 50);*/

}

void loop() {
  // put your main code here, to run repeatedly:

// ***************** Do stuff to the values to be sent below this line. Use no delays.
  
  // Check actuator and pendant serial ports for complete packets and update structs.
  readFromPend();
  if(readFromPend())  // Read values from pendant. If the function returns true, the values were updated so update the pass-through values.
  { // DEMO: Pass through data from pendant to actuator
    actuator.positionCommand = pendant.positionCommand;
    actuator.forceCommand = pendant.forceCommand;
    actuator.airIn = pendant.airIn;
    actuator.airOut = pendant.airOut;
    actuator.activated = pendant.activated;
  }
  
  readFromAct(); // Read values from actuator. If the function returns true, the values were updated. Otherwise there was nothing new.

  // This DEMO code pauses the actuator (in a very crude way) when the encoder button is pressed (it will jump to whatever position the pendant is commanding at the moment the button is released)
  if(digitalRead(ENC_BUTT)) // Encoder button reads low when pressed.
  {
    driveLEDs(encoder.getCount());  // Show LEDs as demo
  }else
  {
    driveLEDs(0);   // Blank LEDs when button is pressed
    actuator.forceCommand = 0;  // Set force command to 0 when button is pressed.
  }

// ***************** Do stuff to the values to be sent above this line. Use no delays.

  // Check if it's time to send a packet.
  if(checkTimer()) sendToAct();
  
  pendant.present ? ledcWrite(PEND_LED, 50) : ledcWrite(PEND_LED, 0);  // Display pendant connection status on LED.
  actuator.present ? ledcWrite(ACT_LED, 50) : ledcWrite(ACT_LED, 0);  // Display actuator connection status on LED.
}
