String input = "";

void setup(){
    Serial.begin(9600); //Open the serial port
    pinMode(LED_BUILTIN, OUTPUT); //Set the digital pin as output
    //sendmsg("louisready");
}

void loop(){
    while(Serial.available() > 0){
        //sendmsg("recibido_crack! "+input);
        input = Serial.readString(); //read the incoming byte
        if(input == "led_on"){
            digitalWrite(LED_BUILTIN, HIGH); //turn the LED on
             sendmsg("Prendido");
            delay(200);
        }else if(input == "led_off"){
            sendmsg("Apagado");
            digitalWrite(LED_BUILTIN, LOW); //turn the LED off
        }else if(input == "greetingsLouis"){
            sendmsg("greetingsMadProgrammer");
            digitalWrite(LED_BUILTIN, HIGH); //turn the LED off
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
            digitalWrite(LED_BUILTIN, HIGH); //turn the LED off
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
        }else{
          if(input.startsWith("+")){
            digitalWrite(LED_BUILTIN, HIGH); //turn the LED off
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
            digitalWrite(LED_BUILTIN, HIGH); //turn the LED off
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
            sendmsg("done.");
          }else{
            sendmsg("Comando_desconocido_"+input);
          }
          
        }
    }
}
void sendmsg(String msg){
  String outi="<<"+msg+">>";
  Serial.print(outi);
}
