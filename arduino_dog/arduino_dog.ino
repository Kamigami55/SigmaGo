#include <Servo.h>
#include <SoftwareSerial.h>

#define FRONT_MOTOR_PIN 11
#define BACK_MOTOR_PWM 3
#define BACK_MOTOR_FORWARD 5
#define BACK_MOTOR_BACKWARD 6
#define STANDBY 4
#define BT_TX 8 // UNKNOWN
#define BT_RX 9 // UNKNOWN

#define FORWARD 0
#define BACKWARD 1
#define RIGHT 2
#define LEFT 3
#define SLOW 4
#define STOP 5

#define SPEED 150

Servo servo;
SoftwareSerial BT(BT_TX, BT_RX);

void setup()
{
    Serial.begin(9600);
    Serial.println(" ");
    pinMode(FRONT_MOTOR_PIN, OUTPUT);
    pinMode(BACK_MOTOR_PWM, OUTPUT);
    pinMode(BACK_MOTOR_FORWARD, OUTPUT);
    pinMode(BACK_MOTOR_BACKWARD, OUTPUT);
    pinMode(STANDBY, OUTPUT);
    /*pinMode(BT_TX, OUTPUT);
    pinMode(BT_RX, INPUT);*/
    analogWrite(FRONT_MOTOR_PIN, 90);
    analogWrite(BACK_MOTOR_PWM, 100);
    digitalWrite(BACK_MOTOR_FORWARD, HIGH);
    digitalWrite(BACK_MOTOR_BACKWARD, HIGH);
    digitalWrite(STANDBY, HIGH);

    servo.attach(FRONT_MOTOR_PIN);
    Serial.println("CAR is ready!");

    // 設定藍牙模組的連線速率
    // 如果是HC-05，請改成38400
    BT.begin(9600);
    Serial.println("Bluetooth is ready!");
    servo.write(90);
    delay(300);
    servo.write(180);
    delay(300);
    servo.write(0);
    delay(300);
    servo.write(360);
}

void loop()
{
    if (Serial.available() || BT.available()) 
    {
        char val;
        if(Serial.available())
        {
          val = Serial.read();
          BT.print(val);
        }
        else 
        {
          val = BT.read();
          Serial.print(val);
        }
        switch(val){
          case '0':
            servo.write(90);
            digitalWrite(BACK_MOTOR_FORWARD, 1);
            digitalWrite(BACK_MOTOR_BACKWARD, 0);
            analogWrite(BACK_MOTOR_PWM, SPEED);
            digitalWrite(STANDBY, HIGH);
            Serial.print("Forward");
            break;
          case '1':
            servo.write(90);
            digitalWrite(BACK_MOTOR_FORWARD, 0);
            digitalWrite(BACK_MOTOR_BACKWARD, 1);
            analogWrite(BACK_MOTOR_PWM, SPEED);
            digitalWrite(STANDBY, HIGH);
            Serial.print("Backward");
            break;
          case '2':
            servo.write(120);
            digitalWrite(BACK_MOTOR_FORWARD, 1);
            digitalWrite(BACK_MOTOR_BACKWARD, 0);
            analogWrite(BACK_MOTOR_PWM, SPEED);
            digitalWrite(STANDBY, HIGH);
            Serial.print("Right");
            break;
          case '3':
            servo.write(60);
            digitalWrite(BACK_MOTOR_FORWARD, 1);
            digitalWrite(BACK_MOTOR_BACKWARD, 0);
            analogWrite(BACK_MOTOR_PWM, SPEED);
            digitalWrite(STANDBY, HIGH);
            Serial.print("Left");
            break;
          case '4':
            servo.write(90);
            digitalWrite(BACK_MOTOR_FORWARD, 1);
            digitalWrite(BACK_MOTOR_BACKWARD, 0);
            analogWrite(BACK_MOTOR_PWM, 40);
            digitalWrite(STANDBY, HIGH);
            Serial.print("Slow");
            break;
          case '5':
            servo.write(90);
            digitalWrite(BACK_MOTOR_FORWARD, 1);
            digitalWrite(BACK_MOTOR_BACKWARD, 0);
            analogWrite(BACK_MOTOR_PWM, 0);
            digitalWrite(STANDBY, LOW);
            Serial.print("Stop");
            break;
        }
    }
}
