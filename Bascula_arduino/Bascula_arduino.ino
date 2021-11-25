
#include "ListLib.h"

#define LED_BUILTIN 2 //pin de led

//Libreria para adc sensor de peso
#include "HX711.h" 

//Librerias para el RFID
#include <SPI.h>
#include <MFRC522.h>

//Librerias para el uso del wifi y envio de datos al servidor
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

//Pines ADC sensor de peso
const int DOUT=D1;
const int CLK=D0;

//Pines RC522
#define RST_PIN  D3    //Pin 3 para el reset del RC522
#define SS_PIN D4   //Pin 4 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

//Nombre y clave de red wifi
const char* ssid = "TECNOLOGIA-RH";//nombre de red
const char* password = "PanPer@2019$SaNi";//clave de red

WiFiClient  client;
const char * myWriteAPIKey = "EEO9WIWNRX546GPR";

//Variables string s recibir de la interfaz
String inputString1 = "";     //string donde guardar datos
bool stringComplete1 = false; //boolean para saber si se recibio el string

//Instanciamos balanza
HX711 balanza;

int peso = 0;
List<int> list_az;
List<int> list_ha;
List<int> list_ar;
int azucar_peso;
int harina_peso;
int arroz_peso;
 
void setup() {

  // El LED integrado está conectado al pin 2.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  //Inicio comunicacion serial a 9600 bps
  Serial.begin(9600);
  //Iniciamos la comunicacion serial del ADC        
  balanza.begin(DOUT, CLK);
  //Establecemos la escala de la balanza
  balanza.set_scale(404.97); 
  //El peso actual es considerado Tara.
  balanza.tare(20);

  //Iniciamos el Bus SPI para el RFID
  SPI.begin();
  //Iniciamos  el RC522
  mfrc522.PCD_Init();
  
  //Iniciamos wifi y server
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  //Iniciamos Thinkspeak

  //reservamos 200 bytes para los inputString:
  inputString1.reserve(200);
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte arroz[4]= {0x73, 0x13, 0x26, 0x03} ; //código del objeto 1
byte azucar[4]= {0x49, 0xD1, 0x25, 0xB3} ; //código del objeto 2
byte harina[7]= {0x04, 0xEF, 0xB8, 0x0A, 0x5D, 0x67, 0x80} ; //código del objeto 3
 
void loop() {
  
// Connectar o reconectar al wifi
  if(WiFi.status() != WL_CONNECTED){
       Serial.print("Attempting to connect");
       digitalWrite(LED_BUILTIN, LOW);
       while(WiFi.status() != WL_CONNECTED){
            WiFi.begin(ssid, password); 
            delay(5000);}
      digitalWrite(LED_BUILTIN, HIGH);     
      //Serial.println("\nConnected.");
      }

//imprime el string cuando esta completo:
if (stringComplete1){
  inputString1.trim();  // si llega una nueva linea carga un espacio vacio
  //Serial.println(inputString);
  
  if (inputString1.equals("#On")){
    int peso = balanza.get_units(20);
    delay(100);
    Serial.println(peso);
    Serial.flush();
    
    }
    
  if (inputString1.equals("#On1")){
    comproTarjeta();
    if(compareArray(ActualUID,arroz)){
         Serial.println("Arroz");
         arroz_peso = true;
         Serial.flush();}
         
    if(compareArray(ActualUID,azucar)){
         Serial.println("Azucar");
         azucar_peso = true;
         Serial.flush();}
         
    if(compareArray(ActualUID,harina)){
         Serial.println("Harina");
         harina_peso = true;
         Serial.flush();}
    }
    
  if (inputString1.equals("#On2")){
    int peso = balanza.get_units(20);
    comproTarjeta();
    
    if(compareArray(ActualUID,arroz)){
      list_ar.Add(peso);
      for (int i = 0; i < list_ar.Count(); i++){
        arroz_peso = list_ar[i] + arroz_peso;}
      ThingSpeak.writeField(1, 3, arroz_peso, myWriteAPIKey);}
         
    if(compareArray(ActualUID,azucar)){
      list_az.Add(peso);
      for (int i = 0; i < list_az.Count(); i++){
        azucar_peso = list_az[i] + azucar_peso;}
      ThingSpeak.writeField(1, 1, azucar_peso, myWriteAPIKey);}
         
    if(compareArray(ActualUID,harina)){
      list_ha.Add(peso);
      for (int i = 0; i < list_ha.Count(); i++){
        harina_peso = list_ha[i] + harina_peso;}
      ThingSpeak.writeField(1, 2, harina_peso, myWriteAPIKey);}
}
  
  //limpia el string
  inputString1 = "";
  stringComplete1 = false;}

}

void serialEvent(){
    while (Serial.available()){
        //Obtiene nuevo byte:
        char inChar = (char)Serial.read();
        //si lo recibido es una linea 
        if (inChar == '\n'){
            stringComplete1 = true;}
        else{
            //lo agrega a inputString1:
            inputString1 += inChar;}
    }
}


void comproTarjeta(){
    // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()){  
      //Seleccionamos una tarjeta
      if ( mfrc522.PICC_ReadCardSerial()){
        // Enviamos serialemente su UID
        //Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          //Serial.print(mfrc522.uid.uidByte[i], HEX);}
          ActualUID[i]=mfrc522.uid.uidByte[i];  
        
  //Terminamos la lectura de la tarjeta  actual
  mfrc522.PICC_HaltA();}}

}
}

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
