const int analogPin = A0;
const int num_sensors = 6;
const int sensor_pins[] = {A0, A1, A2, A3, A4, A5};

int value;
float angle;

void setup() {
  Serial.begin(9600);

  for(int i=0;i<num_sensors;i++)
    pinMode(sensor_pins[i], INPUT);
}

void loop() {

  for(int i=0;i<num_sensors;i++){
    value = analogRead(sensor_pins[i]); // Read analog voltage
    angle = map(value, 0, 1023, 0, 360); // Convert to degrees
    Serial.print(angle);
  }

  delay(100);
}