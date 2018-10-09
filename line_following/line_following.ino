/*
TODO:
- communicate with python to update motor behavior (2 dc motors independent)
- send IR data to python
- update 

    Design and build a removable attachment method for integrating your electronics with 
    the wheeled robotic platform. You may not modify the platform in any way that persists 
    after you remove your electronics. Write a controller that uses the optical reflectivity 
    sensors provided to guide your robotic platform along the “track.” 

    Integrate the Adafruit motor shield with the wheeled platform to drive two DC motors 
    independently. Using two IR reflectivity sensors, implement a controller that enables 
    your robot to follow a black tape line around a loop as fast as possible. 

    Create an interface to your Arduino code that allows you to change the behavior of
     your controller from your laptop (via the serial connection) without recompiling or reloading your Arduino code. 
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(2);

// analog sensor pins:
int IR_pin1 = 0;
int IR_pin2 = 1;

// analog motor pins:

// stores readings
int IR_left = 0;
int IR_right = 0;

// assuming binary readings
int sensor_threshold = 1000;

// stores motor speeds
int max_speed = 100;
int turning_speed = 50;
int left_motor_speed = 10;
int right_motor_speed = 20;

int long baudRate = 115200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  // create with default frequency 1.6KHz
  AFMS.begin();
  // may need to run motors here for setup?
  
  //pinMode(LED_BUILTIN, OUTPUT); // for blink test
  Serial.println("HELLO");
}

void loop() {
  read_sensors();
  delay(100);
  check_action_required();
  //delay(10); //necessary to keep data clean
  //test_blink();
}

void read_sensors(){
  // get sensor values:
  IR_left = 990; //analogRead(IR_pin1);
  IR_right = 990; //analogRead(IR_pin2);
  // print sensor values to serial
  /* Serial.print(IR_left); Serial.print(",");
  Serial.println(IR_right); */
}

void test_blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void write_motor_speeds(){
  
}

void check_action_required(){
  // if neither sensor detects, keep forward
  if (!check_left_tape() && !check_right_tape()){
    left_motor_speed = max_speed;
    right_motor_speed = max_speed;
    Serial.println("chuggin along");
  }
  // only left, turn left
  else if (check_left_tape() && !check_right_tape()){
    left_motor_speed = turning_speed;
    right_motor_speed = 0;
    Serial.println("turning left");
  }
  // only right, turn right
  else if (!check_left_tape() && check_right_tape()){
    left_motor_speed = 0;
    right_motor_speed = turning_speed;
    Serial.println("turning right");
  }
  // else, stop
  else{
    left_motor_speed = 0;
    right_motor_speed = 0;
    Serial.println("Stop! Im confused :(");
  }
}

// Two functions for checking tape
// above threshold means tape, below means floor
bool check_left_tape(){
  if (IR_left > sensor_threshold){
    return true;
  }
  else{
    return false;
  }
}
bool check_right_tape(){
  if (IR_right > sensor_threshold){
    return true;
  }
  else{
    return false;
  }
}

// scraps
/*
void read_action(){
  if (!Serial.available()) {
    return;
  }
  int command_stop = Serial.read();
  if (command_stop != 255){
    return;
  }
  left_motor_speed = Serial.read();
  right_motor_speed = Serial.read();
  Serial.print(left_motor_speed); Serial.print(",");
  Serial.println(right_motor_speed);
}
*/
