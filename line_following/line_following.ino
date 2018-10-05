
// analog pins:
int IR_pin1 = 0;
int IR_pin2 = 1;
// stores readings
int IR_value1 = 0;

int long baudRate = 9600;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudRate);
}

void loop() {
  // put your main code here, to run repeatedly:
  IR_value1 = analogRead(IR_pin1);
  Serial.println(IR_value1);
}
