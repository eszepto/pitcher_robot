  #include <Servo.h>

  #define ir1_pin A3
  #define ir2_pin A4
  #define ir3_pin A5

  #define irRear_pin A6
  //// 345,678 for motor ||||  9,10,11 for select station |||| A0A1A2 for servo ///// A3A4A5

  int blackline = 600;
  int rear_backline = 650;
  int rear_whiteline = 350;

  int station;
  int line_count;
  boolean count_on = true;

  boolean mission = true;

  Servo base_Serv; //10
  Servo center_Serv; //11
  Servo pitcher_Serv; //9


  //[+][+][+][+][+][+][+] Function [+][+][+][+][+][+][+][+][+]
    void motor_run(){
      digitalWrite(8,1); //left
      digitalWrite(7,0);
      analogWrite(6,190);

      digitalWrite(3,1); //right
      digitalWrite(4,0);
      analogWrite(5,240);
    }

    void motor_run_mt(int speedleft,int speedright){
      digitalWrite(8,1); //left
      digitalWrite(7,0);
      analogWrite(6,speedleft);
      
      digitalWrite(3,1); //right
      digitalWrite(4,0);
      analogWrite(5,speedright);

    }

    void motor_stop(){
      digitalWrite(3,1);
      digitalWrite(4,1);
      analogWrite(5,255);

      digitalWrite(8,1);
      digitalWrite(7,1);
      analogWrite(6,255);
    }

    void motor_backward(){ //ถอย
      digitalWrite(3,0);
      digitalWrite(4,1);
      analogWrite(5,240);

      digitalWrite(8,0);
      digitalWrite(7,1);
      analogWrite(6,190);
    }

    void turn_left(){
      digitalWrite(3,1); 
      digitalWrite(4,0);
      analogWrite(5,200);

      digitalWrite(8,0);
      digitalWrite(7,1);
      analogWrite(6,200);
    }

    void normal(){ //หด
      base_Serv.write(90);
      center_Serv.write(180);
      delay(2000);
    }

    void change(){ //ยืด
      center_Serv.write(90);
      delay(10);
      base_Serv.write(170);
    }

    void pinch(){ //หนีบ
      pitcher_Serv.write(180);
    }

    void unpinch(){ //ปล่อย
      pitcher_Serv.write(100);
    }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //-----------------------------------OP------------------------------------------------------------
  void box_picking(){
    delay(1000);
    pinch();
    delay(1000);

  }

  void box_dropping(){
    change();
    delay(500);
    unpinch();
    normal();

  }

  void sensor_linetracking(){

        int ir1 = analogRead(ir1_pin);
        int ir2 = analogRead(ir2_pin);
        int ir3 = analogRead(ir3_pin);
        
        int irRear = analogRead(irRear_pin);

        if(count_on && irRear >= rear_backline){
          line_count += 1;
          count_on = false;
          return;
        }

        if(ir1 < blackline && ir2>=blackline && ir3 < blackline){
          motor_run();
          return;
        }
        else if(ir1 >= blackline && ir2 >= blackline && ir3 < blackline){
          motor_run_mt(150,250);
          return;
        }
        else if(ir1 >= blackline && ir2 < blackline && ir3 < blackline){
          motor_run_mt(150,250);
          return;
        }
        else if(ir1 < blackline && ir2 >= blackline && ir3 >= blackline){
          motor_run_mt(240,170);
          return;
        }
        else if(ir1 < blackline && ir2 < blackline && ir3 >= blackline){
          motor_run_mt(240,170);
          return;
        }

        if(!count_on && irRear <= rear_whiteline){
          count_on = true;
          return;
        }
  }

  int station_select(){
    if(digitalRead(9) == LOW){
      return 1;
    }
    if(digitalRead(10) == LOW){
      return 2;
    }
    if(digitalRead(11) == LOW){
      return 3;
    }
    if(digitalRead(12) == LOW){
      return 4;
    }
  }

  void testMode(){
    while(true){
      Serial.print("ir1 : ");
      Serial.print(analogRead(ir1_pin));
      Serial.print("\t");
      Serial.print("ir2 : ");
      Serial.print(analogRead(ir2_pin));
      Serial.print("\t");
      Serial.print("ir3 : ");
      Serial.print(analogRead(ir3_pin));
      Serial.print("\t");
      Serial.print("irRear : ");
      Serial.print(analogRead(irRear_pin));
      Serial.print("\t");
      Serial.print(digitalRead(9));
      Serial.print(digitalRead(10));
      Serial.print(digitalRead(11));
      Serial.print(digitalRead(12));
      Serial.println("/t");
      delay(50);
    }
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  void setup() {
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);

    pinMode(9,INPUT_PULLUP);
    pinMode(10,INPUT_PULLUP);
    pinMode(11,INPUT_PULLUP);
    pinMode(12,INPUT_PULLUP);

    base_Serv.attach(A0); //ตัวล่าง myservo1
    center_Serv.attach(A1); //ตัวบน myservo2
    pitcher_Serv.attach(A2);  //ตัวหนีบ myservo3

    normal();
    unpinch();

    station = station_select();

    if(station == 4){
      Serial.begin(115200);
      testMode();
    }

    box_picking();
  }


  void loop() { 
    sensor_linetracking();
    delay(10);

    if(mission && (line_count == station+1)){//ถึงสถานี
      motor_stop();
      delay(100);

      turn_left();
      delay(1500);

      motor_stop();
      delay(1000);
      
      box_dropping();

      turn_left();
      delay(1500);
      motor_stop();
      delay(1000);

      line_count = 0;
      delay(50);
      motor_run();
      delay(200);
      mission = false;
    }

    if(!mission && line_count == 1){//ถึงฐาน
      motor_stop();
      delay(10);

      turn_left();
      delay(1500);
      motor_stop();
      delay(1000);
      
      turn_left();
      delay(1500);
      motor_stop();
      delay(1000);

      while(true){
        
      }
    }
  }


