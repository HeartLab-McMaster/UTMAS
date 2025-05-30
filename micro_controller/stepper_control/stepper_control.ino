#define motor_pulse_1 2
#define motor_dir_1 3

#define motor_pulse_2 4
#define motor_dir_2 5

#define motor_pulse_3 6
#define motor_dir_3 7

#define motor_pulse_4 42
#define motor_dir_4 43

#define motor_pulse_5 52
#define motor_dir_5 53

#define motor_pulse_6 48
#define motor_dir_6 49

// #include <AccelStepper.h>

char buf[2];

int targetPosition = 0;
int selected_motor = 1;
// AccelStepper stepper(AccelStepper::DRIVER, motor_pulse_1, motor_dir_1);

void setup() {
  pinMode(motor_pulse_1, OUTPUT);
  pinMode(motor_dir_1, OUTPUT);

  pinMode(motor_pulse_2, OUTPUT);
  pinMode(motor_dir_2, OUTPUT);
  
  pinMode(motor_pulse_3, OUTPUT);
  pinMode(motor_dir_3, OUTPUT);
  
  pinMode(motor_pulse_4, OUTPUT);
  pinMode(motor_dir_4, OUTPUT);
  
  pinMode(motor_pulse_5, OUTPUT);
  pinMode(motor_dir_5, OUTPUT);
  
  pinMode(motor_pulse_6, OUTPUT);
  pinMode(motor_dir_6, OUTPUT);
  
  // stepper.setMaxSpeed(100);
  Serial.begin(9600);
}

void moveTo(int target, int pulse_pin, int dir_pin) {
  for (int i = 0; i < target; i++)  //Forward 5000 steps
  {
    digitalWrite(dir_pin, LOW);
    digitalWrite(pulse_pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulse_pin, LOW);
    delayMicroseconds(500);
  }
}

void loop() {

  if (Serial.available() > 0) {
    // Read the input as a string
    int res = Serial.readBytes(buf, 2);
    // String input = Serial.readStringUntil('\n');
    // input.trim();  // Remove any whitespace or newline characters

    // selected_motor = input.toInt();
    // Serial.println(selected_motor);

    selected_motor = buf[0];

    // Wait for target position input
    // while (!Serial.available()) {}
    // input = Serial.readStringUntil('\n');
    // targetPosition = input.toInt();
    targetPosition = buf[1];

    switch (selected_motor) {
      case 1:
        moveTo(targetPosition, motor_pulse_1, motor_dir_1);
        break;
      case 2:
        moveTo(targetPosition, motor_pulse_2, motor_dir_2);
        break;
      case 3:
        moveTo(targetPosition, motor_pulse_3, motor_dir_3);
        break;
      case 4:
        moveTo(targetPosition, motor_pulse_4, motor_dir_4);
        break;
      case 5:
        moveTo(targetPosition, motor_pulse_5, motor_dir_5);
        break;
      case 6:
        moveTo(targetPosition, motor_pulse_6, motor_dir_6);
        break;
      default:
        break;
    }
  }
}