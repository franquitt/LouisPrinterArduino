/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

int finalDerecha=12;
int finalIzquierda=9;
int punzon = 10;
int enableMotorsPin=8;
int dirRolo=4;
int stepRolo=7;
int dirCarro=2;
int stepCarro=5;


//distancias
String input = "";
int d_A = 20;
int d_B = 35;
int d_C = 25;
int d_D = 50;
int sleepCarro=400;
int sleepRolo=400;
int d_IncioCarro = 0;

int steepsDelay = 100;//era 100
int carrete = 0;
bool enabledMotors=false;

void setup() {
  pinMode(enableMotorsPin, OUTPUT);
  digitalWrite(enableMotorsPin, HIGH);//DESACTIVA TODOS LOS MOTORES AL PRINCIPIO
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(finalDerecha, INPUT_PULLUP);
  pinMode(finalIzquierda, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(punzon, OUTPUT);
  pinMode(dirCarro, OUTPUT);
  pinMode(stepCarro, OUTPUT);
  digitalWrite(stepCarro, LOW);
  pinMode(dirRolo, OUTPUT);
  pinMode(stepRolo, OUTPUT);
  digitalWrite(stepRolo, LOW);
  Serial.begin(9600);
  carroHome();
}

// the loop routine runs over and over again forever:
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
    }else if (input == "limites") {
      if(limDer())
        sendmsg("derecho");
      if(limIzq())
        sendmsg("izquierdo");
      sendmsg("listo");

    } else {
      if (input.startsWith("-")) { //DIMENSIONES
          input=input.substring(1);
          d_A = getValue(input, '_', 0).toInt();
          
          d_B = getValue(input, '_', 1).toInt();
          
          d_C = getValue(input, '_', 2).toInt();
          
          d_D = getValue(input, '_', 3).toInt();
          
          sleepRolo = getValue(input, '_', 4).toInt();
          
          sleepCarro = getValue(input, '_', 5).toInt();

          d_IncioCarro = getValue(input, '_', 7).toInt();
          
          //stepSheet.setSpeed(getValue(input, '_', 4).toInt());
          
          //stepCar_vel =  getValue(input, '_', 5).toInt();
          //stepCar.setSpeed(stepCar_vel);
          
          steepsDelay =  getValue(input, '_', 6).toInt();
          sendmsg("done.");

      } else if (input.startsWith("+")) { //PRINT COMMANDS
        //blinke();
        for (int index = 0; index < input.length() ; index++) {
          char actual = input.charAt(index);
          bool goBack = false;
          
          switch (actual) {
            case '1'://SOLENOIDE
              punzar();
              //sendmsg("punzon");
              break;
            case '2'://HORIZ DOTS
              if(input.charAt(index+1)=='*'){
                stepCarroMove(d_A*((int)input.charAt(index+2)));
                carrete += d_A*((int)input.charAt(index+2));
                index+=2;
              }else{
                stepCarroMove(d_A);
                carrete += d_A;
              }
              delay(steepsDelay);
              break;
            case '3'://HORIZ CHARS
              stepCarroMove(d_B);
              carrete += d_B;
              delay(steepsDelay);
              //sendmsg("DIST B");
              break;
            case '4'://VERT DOTS
              stepRoloMove(d_C);
              goBack = true;
              delay(steepsDelay);
              //sendmsg("DIST C");
              break;
            case '5'://VERT RENG
              stepRoloMove(d_D);
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
              stepCarroMove(d_A + d_B);
              carrete += d_A + d_B;
              delay(steepsDelay);
              //sendmsg("HORIZ DOTS + HORIZ CHARS");
              break;
            case '9'://INICIO CARRO DIST
              stepCarroMove(d_IncioCarro);
              carrete += d_B;
              delay(steepsDelay);
              //sendmsg("DIST B");
              break;
          }
          if (goBack) {
            carroHome();
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
void stepCarroMove(int steps){
  actiMotors(true);
  for(int cantidad=0;cantidad<steps;cantidad++){
    digitalWrite(dirCarro, LOW);
    digitalWrite(stepCarro, HIGH);
    delayMicroseconds(sleepCarro);
    digitalWrite(stepCarro, LOW);
    delayMicroseconds(sleepCarro);
  }
  actiMotors(false);
}
void stepRoloMove(int steps){
  actiMotors(true);
  for(int cantidad=0;cantidad<steps;cantidad++){
    if(limDer())
      break;
    digitalWrite(dirRolo, LOW);
    digitalWrite(stepRolo, HIGH);
    delayMicroseconds(sleepRolo);
    digitalWrite(stepRolo, LOW);
    delayMicroseconds(sleepRolo);
  }
  actiMotors(false);
}
void carroHome(){
  actiMotors(true);
  digitalWrite(dirCarro, HIGH);
  while(!limIzq()){
    digitalWrite(stepCarro, HIGH);
    delayMicroseconds(sleepCarro);
    digitalWrite(stepCarro, LOW);
    delayMicroseconds(sleepCarro);
  }
  actiMotors(false);
}
void carroEnd(){
  actiMotors(true);
  digitalWrite(dirCarro, LOW);
  while(!limDer()){
    digitalWrite(stepCarro, HIGH);   
    delayMicroseconds(400);                     
    digitalWrite(stepCarro, LOW);    
    delayMicroseconds(400);
  }
  actiMotors(false);
}
void punzar(){
  delay(100);
  digitalWrite(punzon, HIGH);
  delay(20);//se empieza a fallar con menos                   
  digitalWrite(punzon, LOW);
  delay(100);
}
void actiMotors(bool state){
  digitalWrite(enableMotorsPin, !state);
}
bool limIzq(){
  return digitalRead(finalIzquierda)==LOW;
}
bool limDer(){
  return digitalRead(finalDerecha)==LOW;
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
