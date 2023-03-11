#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>



//Settings
const bool debug_enabled = true;
const int debug_nbm = 1;
const int v1s = 530; //520
const int v1ul = 550; //550
const int v2s = 300; //280
const int v2ul = 392; //392


//Constants
const int r1 = A0; // Photoresistor at Arduino analog pin A0
const int r2 = A1;
const int led1 = 2;       // Led pin at Arduino pin 2
const int led2 = 4;
const int b1 = 6;

//Variables
int v1;				  // Store value from photoresistor (0-1023)
int v2;
int v3;
bool l2 = false;
bool l1 = false;
int p1 = 0;
int p2 = 0;
char p[10] = "P1:P2";
bool fr = true;
bool rst = false;

//Matrix
#define CS_PIN    10
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
MD_Parola M = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);



void setup_pi(){
  pinMode(led1, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(r1, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(led2, 1);
  pinMode(r2, INPUT);
  pinMode(b1, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(led1, 1);
  digitalWrite(led2, 1);
}

void setup(){
  if (debug_enabled){ debug(); }
  setup_pi();
  setup_m();
}

void debug(){
  Serial.begin(9600);
}

void setup_m(){
  M.begin();
  M.setTextAlignment(PA_CENTER);
  M.print("INIT");
  delay(1000);
}

void update_p(int x, int y){
  if(x < 10){
    if(y < 10){
      sprintf(p, "0%d:0%d", x, y);
      M.print(p);
    }else{
      sprintf(p, "0%d:%d", x, y);
      M.print(p);
    }
  }else if(y < 10){
    if(x < 10){
      sprintf(p, "0%d:0%d", x, y);
      M.print(p);
    }else{
      sprintf(p, "%d:0%d", x, y);
      M.print(p);
    }
  }else{
    sprintf(p, "%d:%d", x, y);
    M.print(p);
  }
}

void inc_p(int player, int by){
  if(p1 == 99){
    while(!rst){
      chk_rst(true); 
    }
    rst = false;    
  }else if(p2 == 99){
    while(!rst){
      chk_rst(true); 
    }
    rst = false;
  }else{
  if (player == 1){
    p1 = p1 + by;    
    update_p(p1, p2);
  }else if ( player == 2){
    p2 = p2 + by;
    update_p(p1, p2);
  }}
}


void chk_rst(bool ws){
  v3 = digitalRead(b1);
  if(v3 == 0){
    l1 = 0;
    l2 = 0;
    if(p1 > p2){
      M.print("P1 WON");
    }else if(p2 > p1){
      M.print("P2 WON");
    }else{
      M.print("DRAW");
    }
    p1 = 0;
    p2 = 0;
    if(ws){
      rst = true; 
    }
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop(){
  chk_rst(false);
  if(fr){
    fr = false;
    M.print("P1:P2");
    delay(2000);
  }
  v1 = analogRead(r1);
  v2 = analogRead(r2);
  if(v1 < v1s && !l1){
    l1 = true;
    inc_p(1,1);
  }else if(v1 >= v1ul && l1){
    l1 = false;
  }
  if(v2 < v2s && !l2){
    l2 = true;
    inc_p(2,1);
  }else if(v2 >= v2ul && l2){
    l2 = false;
  }
  if (debug_enabled){
    if(debug_nbm == 1){
      Serial.println(v1);
    }else{
      Serial.println(v2);
    }
  }
}
