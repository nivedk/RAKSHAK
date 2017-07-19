

#include <Stepper.h>
#include <Servo.h>

Servo myServo; 

const int stepsPerRevolution = 200; // change this to fit the number of steps per revolution // for your motor

// initialize the stepper library on pins 8 through 11: 
Stepper hStepper(stepsPerRevolution, 4, 5, 6, 7);
Stepper vStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() { // set the speed at 60 rpm: 
  myServo.attach(3);
  hStepper.setSpeed(1); // initialize the serial port: 
  vStepper.setSpeed(1);
  Serial.begin(9600); 
  //pinMode(2,OUTPUT);
  //pinMode(2, OUTPUT);
  //pinMode(3, OUTPUT);
  //digitalWrite(3,LOW);
  //digitalWrite(2, LOW);
  myServo.write(55);
  //delay(
}

void getAngles(int *angles){ 
  int a = 0; 
  int b = 0;
  if (Serial.available()){ 
    //num = Serial.parseInt(); 
    byte x = Serial.read();
    Serial.println("  x  " +x);
    byte y = Serial.read();
    a = x-50;
    b = y - 50;
    Serial.println("a  " + a);
    Serial.println("b  " + b);
    
    //Serial.println
    //nums[i] = (int)chars[i] - 48; //Serial.println("hello"); 
    //Serial.println(nums[i]); //Serial.println(nums[i]); 
    //Serial.println((int)nums[i]-48); 
    //i++; 
 
  }

  angles[0] = a; 
  angles[1] = b; 
}


void moveStepper(int a){ 
  if(a>0){ 
    while(a>0){ 
      a--; 
      hStepper.step(1); 
      //delay(100); 
      //} 
      //}
    }
  }
}

//if(a<0){ while(a<0){ a++; hStepper.step(-1); //delay(100); } }



int angles[2];
int i = 0;
byte a, b;

void loop() { // step one revolution in one direction:
  if(Serial.available()){
    if (Serial.available()) { 
      //getAngles(angles); 
      //Serial.println("Clockwise"); 
      a = Serial.read();
      delay(2000);
    }
    if(Serial.available()){
      b = Serial.read();
    }
    Serial.write(angles[0]); 
    hStepper.step(1*(int(a)-50));
    Serial.println(int(a)); 
    delay(1000); 
    vStepper.step(-1*(int(b)-50));
    a = 0;
    b = 0;
    myServo.write(25);
    exit;
    while(true);
  }
  
  //Serial.println(angles[0]); 
  //Serial.println(angles[1]); 
  
  /*
  Serial.println("Anticlockwise"); 
  Serial.println(angles[1]); 
  vStepper.step(angles[1]); 
  delay(1000); //hStepper.step(30); //Serial.println(angle1); //hStepper.step(angle1); //delay(500);
  digitalWrite(2, HIGH);
  delay(3000);
  digitalWrite(2 ,LOW);
  digitalWrite(3, HIGH);
  delay(2000);
  digitalWrite(3, LOW);
  */
}

// step one revolution in the other direction: //Serial.println("counterclockwise"); //hStepper.step(25); //delay(500); }









