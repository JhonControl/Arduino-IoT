
/////////////////////////////
////// Envio de variable de ADC 0 a Twitter 
/////// mas videos - more videos ......https://www.youtube.com/channel/UCv1D6zrC0ZL0PSgM6tdEpPg/videos
///////no olvides suscribirse

#include <SoftwareSerial.h>
 
#define DEBUG true
SoftwareSerial esp8266(10,11);      /////libreria software serial


#define APIKEY "XXXXXXXXXXXXXXXXXX" //  api key your twitter in thingspeak

#define TARGET_IP "184.106.153.149"///         //direccion IP thingspeak

#define TARGET_PORT "80"          ///  puerto 80

#define ID "XXXXXXXXX"  //name of wireless access point to connect to   - Red de inhalambrica wifi 
#define PASS "XXXXXXXXX"  //wifi password                          - Contraseña de Wifi 

                             
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
   
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

  /////////////////////////////////////////////////////////////////////////
    delay(1000);  //3 seg
    
  //////////////////////////////////////////////////////////////////////
   // read the input on analog pin 1:
  int sensorValue_batt = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage_Batt = (sensorValue_batt * (3.37 / 722.0));
   
     
   ///CONVERT  FLOAT TO CHAR  + CHAR TO STRING 
       
   char outstr1[15];
   dtostrf(voltage_Batt,4, 2, outstr1);   //float to char  4 numero de caracteres  3 cifras sin espacio
   String valor1 = outstr1;   // char to string
      
   //  String con tweet
   String  updateTwitterStatus = ("Test @thingspeak ThingTweet #IoT Arduino + ESP8266 Read ADC Arduino Nano " + valor1 + "Volt");      
      
     
    String webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";         
    sendData(webpage,1000,DEBUG);        

    // Create HTTP POST Data   
               
    String tsData = "api_key=" APIKEY "&status=" + updateTwitterStatus ;    
     
    String webpage1 = "POST /apps/thingtweet/1/statuses/update HTTP/1.1\n";
    webpage1+="Host: api.thingspeak.com\n";
    webpage1+="Connection: close\n";
    webpage1+="Content-Type: application/x-www-form-urlencoded\n";
    webpage1+="Content-Length: ";
    webpage1+=tsData.length();
    webpage1+="\n\n";
    webpage1+=tsData;
       
     //// envia   comando AT + cantidad de caracteres   
      
     String cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   

      //// comando AT de cierre de conexion            
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         

              /// DELAY para reenvio de datos a twitter
     delay(10000);  //3 seg

 
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
