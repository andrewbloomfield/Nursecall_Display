#include <LiquidCrystal.h>

const int rs = 14, en = 15, d4 = 16, d5 = 17, d6 = 18, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String vers="1.00";         //Version control

char inbyte;                //get serial byte
String intext;              //incomming serial string
int newline=0;              //find carriage return
int nextline=0;             //find comma
int hexval;                 //hex switch
String wake="D";            //wake charactor
String messages[5] {"Empty,Message", "Empty,Message","Empty,Message","Empty,Message","Empty,Message"};        //array for messages
int newmessage=0;           //slot for new message
int showmessage=0;          //count for show message
long LED;                   //LED Backlight time out
int debounce=50;            //switch debounce
bool sw1 = false;           //switch 1 status
bool sw2 = false;           //switch 2 status


void setup() {
  pinMode(7,OUTPUT);  //Buzzer
  
  pinMode(6,INPUT_PULLUP);  //Right Button
  pinMode(5,INPUT_PULLUP);  //Middle Button
  pinMode(4,INPUT_PULLUP);  //Left Button
  
  DDRB = B11110000;         //Set port B0-B3 for input
  PORTB = B00001111;        //Enable pullup resistors
  
  Serial.begin(9600);
  
  pinMode(3,OUTPUT);  //backlight
  digitalWrite(3,HIGH); //backlight
  LED=millis();
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  ver();
  //lcd.print("andrewbloomfield");

}

void loop() {

  while(Serial.available()){
    //intext=Serial.readString();
    inbyte=Serial.read();             //read serial byte
    intext=String(intext + inbyte);   //Add each byte to form a string
    
  }

  newline=intext.indexOf(13);         //find carriage return
  nextline=intext.indexOf(44);        //find comma

  while(newline>0){
    //Serial.println(int(wak.charAt(0)));

/*  Serial.println(intext);
  Serial.println(newline);
  Serial.println(intext.length());

  Serial.println(int(intext.charAt(0)));
  Serial.println(intext.charAt(0)); 
  //int n = atoi("4");
  //Serial.println(n);
  
  //Serial.println(String(intext.charAt(0));
  //Serial.println(atoi(F));
  //char myString = "1";
  //int myInt = atoi(myString);
  //Serial.print(myInt);

  
  readhex();
  //Serial.println(wake);
  //Serial.println(wake);
  if(intext.charAt(0)==(int(wake.charAt(0)))){
    Serial.println("Wake sent");
  }
  
Serial.println(hexval);
  if(intext.charAt(1)==hexval){
    Serial.println("HEX switch matched");
  }*/

  readhex();

  if((intext.charAt(0)==(int(wake.charAt(0))))&&(intext.charAt(2)==hexval)){
    //Serial.println("Both Matched");


    switch(intext.charAt(3)){
      case '1':
      lcd.clear();
      break;
      case '2':
      incomming();
      lcd.setCursor(0, 0);
      lcd.print("                ");              //clear line 
      lcd.setCursor(0, 0);
      lcd.print(intext.substring(4,newline));
      break;
      case '3':
      incomming();
      lcd.setCursor(0, 1);
      lcd.print("                ");             //clear line 
      lcd.setCursor(0, 1);
      lcd.print(intext.substring(4,newline));
      break;
      case '4':
      if(nextline>0){
      incomming();
      storemessage();  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(intext.substring(4,nextline));
      lcd.setCursor(0,1);
      lcd.print(intext.substring(nextline+1,newline));
      }
      break;
      case '5':
      ver();
      break;
      case '6':
      Serial.print("OK");
      break;
    }
    
  }
 //delay(1000);
  newline=0;
  intext="";

  }
  LEDOFF();
  Buttons();

  //ver();

  //My test Code
  /*Serial.print("Hex switch set to:     ");
  Serial.println(PINB ^B00001111,HEX);
  
  Serial.print(" Right Button set to:  ");
  Serial.println(digitalRead(6) ^B1,HEX);
  Serial.print(" Middle Button set to: ");
  Serial.println(digitalRead(5) ^B1,HEX);
  Serial.print(" Left Button set to:   ");
  Serial.println(digitalRead(4) ^B1,HEX);
  Serial.println();
  
  delay(1000);

  digitalWrite(7,HIGH);
  delay(100);
  digitalWrite(7,LOW);*/
//for(;;);
}

void beep(){
  digitalWrite(7,HIGH);
  delay(100);
  digitalWrite(7,LOW);
}

void ver(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Audio Monitor");
  lcd.setCursor(2, 1);
  lcd.print("Version ");lcd.print(vers);
}

void readhex(){
  hexval=(PINB ^B00001111);
  if(hexval<=9){hexval=hexval+48;}else{hexval=hexval+55;}
}

void LEDOFF(){
  if(millis() - LED > 10000){
    digitalWrite(3,LOW);            //Turn Off Backlight
  }
}

void Buttons(){
  if((digitalRead(5) ^B1)==1){
      digitalWrite(3,HIGH); //backlight on
      LED=millis();      
  }

  if(((digitalRead(4) ^B1)==1)&&(sw1==false)){        //left button
    delay(debounce);
    sw1=true;                                         //set switch flag
    showmessage--;                                    //decrease counter
    if(showmessage<0){
      showmessage=4;
    }
    previousmessage();                                //show message
  }

  if(((digitalRead(6) ^B1)==1)&&(sw2==false)){        //right button
    delay(debounce);
    sw2=true;                                         //set switch flag
    showmessage++;;                                   //increase counter
    if(showmessage>4){
      showmessage=0;
    }
    previousmessage();                                //show message
  }

    if((digitalRead(4) ^B1)==0){                      //check sw1 released
      delay(debounce);
      sw1=false;
    }

    if((digitalRead(6) ^B1)==0){                       //check sw2 released
      delay(debounce);
      sw2=false;
    }

  
}



void incomming(){
      beep();               //Beep on incomming message
      digitalWrite(3,HIGH); //backlight on
      LED=millis();  
}

void storemessage(){
  showmessage=newmessage;   //set showmessage to be the same as new message
  messages[newmessage]=intext.substring(4,newline);   //store message
  newmessage++;             //increase counter
  if(newmessage>4){         //reset new message count
    newmessage=0;
  }
}


void previousmessage(){
  incomming();
  nextline=messages[showmessage].indexOf(44);        //find comma
  lcd.clear();                                       //clear display
  lcd.setCursor(0,0);                                //set cursor position
  lcd.print(messages[showmessage].substring(0,nextline));
  lcd.setCursor(0,1);                              //next line
  lcd.print(messages[showmessage].substring(nextline+1,messages[showmessage].length()));
}
