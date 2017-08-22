//Braille Printer

/*
  +-----------+
  |   +++++   |
  2|   +++++   |3
  1|           |4
  +-----------+
*/
#include <Stepper.h>
String input = "";
int d_A = 25;
int d_B = 35;
int d_C = 25;
int d_D = 50;
int pun_out = 3;
int carrete = 0;
int stepCar_vel=90;
const int stepsPerRevolution = 400;//0.9° stepper
int steepsDelay = 100;//era 100
Stepper stepSheet(stepsPerRevolution, 8, 9, 10, 11);
Stepper stepCar(stepsPerRevolution, 7, 6, 5, 4);
void setup() {
  pinMode(pun_out, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  stepSheet.setSpeed(60);//era 60
  stepCar.setSpeed(90);//era 90
  Serial.begin(9600); //Open the serial port
  pinMode(LED_BUILTIN, OUTPUT); //Set the digital pin as output
}

void loop() {
  while (Serial.available() > 0) {
    //sendmsg("recibido_crack! "+input);
    input = Serial.readString();
    //BASICS
    if (input == "led_on") {
      digitalWrite(LED_BUILTIN, HIGH);
      sendmsg("Prendido");
      delay(200);
    } else if (input == "led_off") {
      sendmsg("Apagado");
      digitalWrite(LED_BUILTIN, LOW);
    } else if (input == "greetingsLouis") {
      blinke();
      sendmsg("greetingsMadProgrammer");

    } else {
      if (input.startsWith("-")) { //DIMENSIONES
          d_A = getValue(input, '_', 0).toInt();
          
          d_B = getValue(input, '_', 1).toInt();
          
          d_C = getValue(input, '_', 2).toInt();
          
          d_D = getValue(input, '_', 3).toInt();
          
          stepSheet.setSpeed(getValue(input, '_', 4).toInt());
          
          stepCar_vel =  getValue(input, '_', 5).toInt();
          stepCar.setSpeed(stepCar_vel);
          
          steepsDelay =  getValue(input, '_', 6).toInt();
          sendmsg("done.");

      } else if (input.startsWith("+")) { //PRINT COMMANDS
        //blinke();
        for (int index = 0; index < input.length() ; index++) {
          char actual = input.charAt(index);
          bool goBack = false;
          
          switch (actual) {
            case '1'://SOLENOIDE
              digitalWrite(pun_out, HIGH);
              delay(steepsDelay * 2);
              digitalWrite(pun_out, LOW);
              //sendmsg("punzon");
              break;
            case '2'://HORIZ DOTS
              stepCar.step(d_A);
              carrete += d_A;
              delay(steepsDelay);
              //sendmsg("DIST A");
              break;
            case '3'://HORIZ CHARS
              stepCar.step(d_B);
              carrete += d_B;
              delay(steepsDelay);
              //sendmsg("DIST B");
              break;
            case '4'://VERT DOTS
              stepSheet.step(d_C);
              goBack = true;
              delay(steepsDelay);
              //sendmsg("DIST C");
              break;
            case '5'://VERT RENG
              stepSheet.step(d_D);
              goBack = true;
              delay(steepsDelay);
              //sendmsg("DIST D");
              break;
            case '6'://END SHEET
              //sendmsg("END SHEET");
              break;
            case '7'://END PRINT
              //sendmsg("END PRINT");
              break;
            case '8'://HORIZ DOTS + HORIZ CHARS
              stepCar.step(d_A + d_B);
              carrete += d_A + d_B;
              delay(steepsDelay);
              //sendmsg("HORIZ DOTS + HORIZ CHARS");
              break;
          }
          stopSheet();
          stopCar();
          if (goBack) {
            returnCarrete();
            stopCar();
            goBack = false;
          }
        }
        sendmsg("done.");
      } else {
        sendmsg("Comando desconocido " + input);
      }

    }
  }
}
void sendmsg(String msg) {
  msg.replace(" ", "_");
  String outi = "<<" + msg + ">>";
  Serial.print(outi);
}
void blinke() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}
void stopCar() {
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
}
void stopSheet() {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}
void returnCarrete() {
  stepCar.setSpeed(120);
  stepCar.step(-carrete);
  carrete = 0;
  stepCar.setSpeed(stepCar_vel);
}
String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
