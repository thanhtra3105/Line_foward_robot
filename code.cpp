
int sensor[5] = { 0, 0, 0, 0, 0 };
int se[5] = { 2, 8, 10, 11, 12 };
// Motor Variables
int ENA = 3;  //Motor Trái
int motorInput1 = 7;
int motorInput2 = 6;
int motorInput3 = 5;
int motorInput4 = 4;
int ENB = 9;  //Motor Phải

//Initial Speed of Motor
int initial_motor_speed = 80;  // 100   /150

const int trig = A1;
const int echo = A0;

unsigned long duration;  // biến đo thời gian
int distance;            // biến lưu khoảng cách
unsigned long time = 0;
// PID Constants
float Kp = 15;  // thi 20
float Ki = 0;
float Kd = 40;  // 40

float error = 0, P = 0, I = 0, D = 0, PID_value = 0;
float previous_error = 0, previous_I = 0;
int left_motor_speed, right_motor_speed;

int flag = 1;

void setup() {
  pinMode(se[0], INPUT);
  pinMode(se[1], INPUT);
  pinMode(se[2], INPUT);
  pinMode(se[3], INPUT);
  pinMode(se[4], INPUT);

  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);  //setting serial monitor at a default baund rate of 9600
  time = millis();
  // Serial.println("DO_LINE OKE");
}
void loop() {
  
  distance = do_distance();
  
  if (distance > 10) {
    read_sensor_values();
    if (error == 102) {
      forward();
    } else if (error == 0) {
      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
      forward();
    } else {
      calculate_pid();  // Tính giá trị PID
      motor_control();  // Điều chỉnh motor theo giá trị PID mới tính, cho xe chạy thẳng
    }

  }


  else {
    Serial.println(distance);
    stop_bot();
    flag = 0;
    delay(100);
  }
  
}


float read_sensor_values() {
  sensor[0] = digitalRead(se[0]);
  sensor[1] = digitalRead(se[1]);
  sensor[2] = digitalRead(se[2]);
  sensor[3] = digitalRead(se[3]);
  sensor[4] = digitalRead(se[4]);
/*
  if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 1)  // lệch phải nhiều
    error = -4;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 1)  // lệch phải nhiều
    error = -3;

  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 1)
    error = -2;

  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && sensor[4] == 1)
    error = -1;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 1) && sensor[4] == 1)
    error = 0;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && sensor[4] == 1)
    error = 1;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && sensor[4] == 1)
    error = 2;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && sensor[4] == 0)
    error = 3;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && sensor[3] == 1 && sensor[4] == 0)  // lệch trais nhiều
    error = 4;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 0)  // qua phai
    error = 100;

  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 1)  // qua ytai
    error = 101;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && sensor[4] == 1)  // Make U turn
    error = 102;

  /* else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && sensor[4] == 0)  // stop
    error = 103; */
*/
  // doi sang line trang
  
  if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))  // lệch phải nhiều
    error = -4;

  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))  // lệch phải nhiều
    error = -3;

  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -2;

  else if ((sensor[0] == 0) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = -1;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4] == 0))
    error = 0;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 1;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 0))
    error = 2;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 1) && (sensor[4] == 1))
    error = 3;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))  // lệch trái nhiều
    error = 4;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 1))  // qua phải
    error = 100;

  else if ((sensor[0] == 1) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))  // qua trái
    error = 101;

  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4] == 0))  // mat line
    error = 102;
    
  /* 
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4] == 1))  // stop
    error = 103; */

  return error;
}

void calculate_pid() {
  P = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * P) + (Ki * I) + (Kd * D);  // er =3; PID = 120 motor phải chay nhanh hơn

  previous_I = I;
  previous_error = error;
}

void motor_control() {
  // Calculating the effective motor speed:

  left_motor_speed = initial_motor_speed + PID_value;   //200
  right_motor_speed = initial_motor_speed - PID_value;  //0

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);    //0
  right_motor_speed = constrain(right_motor_speed, 0, 255);  //210
 
  analogWrite(ENB, right_motor_speed);  //phai Motor Speed    //0
  analogWrite(ENA, left_motor_speed);   //left Motor Speed  //120

  //following lines of code are to make the bot move forward
  forward();
}

void forward() {
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, HIGH);  // trai
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);  // phai
  digitalWrite(motorInput4, LOW);
}
void reverse() {
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void left() {                       // Trai chay
  digitalWrite(motorInput1, HIGH);  // Trai
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);  // Phai
  digitalWrite(motorInput4, LOW);
}
void right() {                     // Phai chay
  digitalWrite(motorInput1, LOW);  //Trai
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);  // Phai
  digitalWrite(motorInput4, LOW);
}
void quay_phai() {  // Quay trai --> Phai toi, trai lui
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);  // trai lui
  digitalWrite(motorInput3, HIGH);  // phai toi
  digitalWrite(motorInput4, LOW);
}
void quay_trai() {  //Quay phai --> Phai lui, trai toi
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void stop_bot() {
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}

int do_distance() {

  digitalWrite(trig, LOW);  // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);  // phát xung từ chân trig
  delayMicroseconds(5);      // xung có độ dài 5 microSeconds
  digitalWrite(trig, LOW);   // tắt chân trig

  duration = pulseIn(echo, HIGH);
  delay(50);
  return int(duration / 2 / 29.412);
}
