/*
 * test with blink
 */
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

/*
 * I think this will just read the action without polluting the
 * data sending to python if I don't print back to the serial line.
 * are there multiple serial lines? one from python and one to it? is that how this works?
 */


// analog sensor pins:
int IR_pin1 = 0;
int IR_pin2 = 1;

// analog motor pins:

// stores readings
int IR_left = 0;
int IR_right = 0;

// stores received actions
char reading;
bool flash = false;

int long baudRate = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  read_sensors();
  delay(10);
  if (read_action()){
    test_blink();
  }
  delay(10); //necessary to keep data clean
  //test_blink();
}

bool read_action(){
  if (Serial.available() > 0) {
    reading = Serial.read();
  }
  if (reading>0){
    return true;
  }
  else{
    return false;
  }
}

void read_sensors(){
  // get sensor values:
  IR_left = 1111; //analogRead(IR_pin1);
  IR_right = 2222; //analogRead(IR_pin2);
  // print sensor values to serial
  Serial.print(IR_left); Serial.print(",");
  Serial.println(IR_right);
}

void test_blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
