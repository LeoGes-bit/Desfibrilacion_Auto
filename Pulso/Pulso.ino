#include <ESP8266WiFi.h>
extern "C"
{
#include <espnow.h>
}

struct ESTRUCTURA_DATOS   //PIR
{
    uint16_t PULSO = 0;
}ED;

int PulseSensorPurplePin = 0;       
int LED13 = 13;   

int Signal;                
int Threshold = 550;           
int contador = 0;
int bpm = 0;
int promedio = 0;
int latido = 0;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
unsigned long tiempoSegundos = 0;

void setup() {
  //delay(3000);
         
  Serial.begin(115200);
  pinMode(LED13,OUTPUT); 
  // Inicializando protocolo ESP-NOW
  if (esp_now_init() != 0)
  {
    Serial.println("Error al Iniciar el Protocolo esp_now_init() ... reconectando.");
    ESP.restart();
    delay(1);
  }
  
 // Imprime la Direccion MAC del NODO
  Serial.print("SoftAP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
  Serial.print("STATION MAC: ");
  Serial.println(WiFi.macAddress());
  
  // Definir el rol del NODO
  // 0=OCIOSO, 1=MASTER, 2=SLAVE y 3=MASTER+SLAVE
  esp_now_set_self_role(1);
  
  //Añadimos NODO LUCES ESCLAVO
  uint8_t mac_addr[6] = {0x40, 0xF5, 0x20, 0x26, 0xB2, 0x6B};
  uint8_t role = 2;
  uint8_t channel = 3;
  uint8_t key[0] = {};
  uint8_t key_len = sizeof(key);
  
  esp_now_add_peer(mac_addr, role, channel, key, key_len);

 
}

void medir(){

  while(millis()<tiempo1+10000){
  if(analogRead(PulseSensorPurplePin) > Threshold)                        
     digitalWrite(LED13,LOW);
  else
     digitalWrite(LED13,HIGH);

     delay(65);
  }
}
void loop() {
  /*Signal = analogRead(PulseSensorPurplePin);                                      
  Serial.println(Signal);  */                 
  //ED.PULSO = Signal; 

  tiempo1 = millis();
  while(millis()<tiempo1+10000){
    Serial.println("...♥...♥...");
    if(analogRead(PulseSensorPurplePin) > Threshold){                          
     digitalWrite(LED13,LOW);
     latido = 0; 
     //esp_now_send(da, data, len);
     
   }else{
     digitalWrite(LED13,HIGH);         
     latido = 1;     
     //esp_now_send(da, data, len);
      medir();
   }
   //tiempo2 = millis();
   delay(65);    
  }
  
  if(latido == 1){
    ED.PULSO = 1;
    Serial.println("¡Enviando Latido...♥...♥...");    
  }
  else{
    ED.PULSO = 0;
    Serial.println("⚠ Iniciando descarga ⚠ ");
      
  }
  /*if(contador>100){
    ED.PULSO = 0;
    Serial.println("☠ Descargas Excedidas ☠ ");
  }
  contador = contador + 1;*/
  
  uint8_t da[6] = {0x40, 0xF5, 0x20, 0x26, 0xB2, 0x6B};
  uint8_t data[sizeof(ED)];
  memcpy(data, &ED, sizeof(ED));
  uint8_t len = sizeof(data);
  esp_now_send(da, data, len);
  delay(2000);
}
  
