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

// going forward: subtract readings from each other and set as turning speed

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
int sensor_threshold_left = 900;
int sensor_threshold_right = 860;

// stores motor speeds
int max_speed = 20;
int turning_speed = 100;
int left_motor_speed = 10;
int right_motor_speed = 10;

// last seen tape direction
// -1 left, 0 not seen, 1 right
byte tape_direction = 0;

// stores command from serial
String command_string = "";
bool estop = true;

String message = "";

int long baudRate = 115200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  // create with default frequency 1.6KHz
  AFMS.begin();
  // left motor
  left_motor->setSpeed(0);
  left_motor->run(FORWARD);
  left_motor->run(BACKWARD);
  // turn on motor
  left_motor->run(RELEASE);
  // right motor
  right_motor->setSpeed(0);
  right_motor->run(FORWARD);
  right_motor->run(BACKWARD);
  // turn on motor
  right_motor->run(RELEASE);
}

void loop() {
  //read commands from serial
  while(Serial.available()){ //If there is something waiting in the serial buffer, convert it to the series of arrays to flash to spell the text
    char cur = Serial.read();
    if(cur != '\n'){
      command_string += cur;
      delay(10);
    }
  }
  // echo the command
  if (command_string!=""){
    Serial.println(command_string);
  }
  // if the command says to stop
  if (command_string == "stop"){
    // stop robot
    /*left_motor_speed = 0;
    right_motor_speed = 0;*/
    left_motor->run(RELEASE);
    right_motor->run(RELEASE);
    estop = true;
    Serial.println("Emergency Stop!");
  }
  else if (command_string == "go"){
    // start robot again!
    estop = false;
    Serial.println("Whoopie!");
  }
  else{
    if (estop == false){
      read_sensors();
      check_action_required();
      write_motor_speeds();
    }
  }
  //delay(10); //necessary to keep data clean

  // reset command string
  command_string = "";
}

void read_sensors(){
  // get sensor values:
  IR_left = analogRead(IR_pin1);
  IR_right = analogRead(IR_pin2);
  // print sensor values to serial
  Serial.print(IR_left); Serial.print(",");
  Serial.println(IR_right);
}

void write_motor_speeds(){
  left_motor->setSpeed(left_motor_speed);
  right_motor->setSpeed(right_motor_speed);
  left_motor->run(FORWARD);
  right_motor->run(FORWARD);
}

void check_action_required(){
  // if neither sensor detects, keep forward
  if (!check_left_tape() && !check_right_tape()){
    left_motor_speed = max_speed;
    right_motor_speed = max_speed;
    message = "chuggin along";
  }
  // only left, turn left
  else if (check_left_tape() && !check_right_tape()){
    left_motor_speed = 0;
    right_motor_speed = turning_speed;
    tape_direction = -1;
    message = "turning left";
  }
  // only right, turn right
  else if (!check_left_tape() && check_right_tape()){
    left_motor_speed = turning_speed;
    right_motor_speed = 0;
    tape_direction = 1;
    message = "turning right";
  }
  // else, stop
  else{
    left_motor_speed = 0;
    right_motor_speed = 0;
    tape_direction = 0;
    message = "Stop! Im confused :(";
  }
  //Serial.println(message);
}

// Two functions for checking tape
// above threshold means tape, below means floor
bool check_left_tape(){
  if (IR_left > sensor_threshold_left){
    return true;
  }
  else{
    return false;
  }
}
bool check_right_tape(){
  if (IR_right > sensor_threshold_right){
    return true;
  }
  else{
    return false;
  }
}
