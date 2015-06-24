
#include <SoftwareSerial.h>
 
#define DEBUG true
SoftwareSerial esp8266(10,11);      /////libreria software serial

 /// remplazar   apikey y direccion tcp ip de host ThingSpeak
#define APIKEY "TXBSGLTWSRF63NPZ" //  api key ThingSpeak

#define TARGET_IP "184.106.153.149"///         //local  direccion IP o Hosting  ThingSpeak

#define TARGET_PORT "80"          ///  puerto 80


#define ID "MAIRA"  //name of wireless access point to connect to   - Red de inhalambrica wifi 
#define PASS "3113884544"  //wifi password                          - Contraseña de Wifi 
                             
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
  
    pinMode(13, OUTPUT);
   
      ///inicializacion y configuracion de modulo ESP8266

    sendData("AT+RST\r\n",1000,DEBUG); // 
     
    sendData("AT+CWMODE=1\r\n",1000,DEBUG); // 
    
      String cmd="AT+CWJAP=\"";  
      cmd+=ID;
      cmd+="\",\"";
      cmd+=PASS;
      cmd+="\"";      
     sendData( cmd+"\r\n",1000,DEBUG); //     
    
     sendData("AT+CIPMUX=0\r\n",1000,DEBUG); // 
 }
 
void loop()
{ 
       
  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)   
//////////////////////////////////////////////////////////////////////////
   // read the input on analog pin 0:
  int sensorValue_batt = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage_Batt = (sensorValue_batt * (5.0 / 1023.0)*2);
  // print out the value you read:
  
     
     ///CONVERT  FLOAT TO CHAR  + CHAR TO STRING 
     
  
      char outstr1[15];
      //dtostrf(voltage_Batt,4, 2, outstr1);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor1 = outstr1;   // char to string
      
      
      
      dtostrf(voltage_Batt,random(1, 10), 0, outstr1);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor2 = outstr1;   // char to string
      
     
      
     /// encabezado de envio a ThingSpeak       
    
    String webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        
     
     String webpage1 =   "GET /update?key=" APIKEY "&field1="+ valor2 + "   HTTP/1.0\r\n";
   
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
        
       //// envia   comando AT + cantidad de caracteres   
      
     String cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
               
        //// comando AT de cierre de conexion            
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
           
            
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW   





     /// DELAY para reenvio de datos a thingSpeak
     delay(5000);  //3 seg

}
  
 //// rutina de depuracion via puerto serial
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
