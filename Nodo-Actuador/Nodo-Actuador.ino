#include <ESP8266WiFi.h>
extern "C"
{
#include <espnow.h>
}

/*
Estructura de datos para transmitir entre Maestro y Esclavo
Esta estructura debe ser la misma tanto en el Maestro y en el Esclavo
*/

struct ESTRUCTURA_DATOS
{
    uint16_t PULSO = 0;

};

#define LUZ_ROJA 13   //D7


void setup()
{
 
  Serial.begin(115200);
   pinMode(LUZ_ROJA,OUTPUT);  
  digitalWrite(LUZ_ROJA, HIGH);
  Serial.println();
  //digitalWrite(LUZ_ROJA, HIGH);
  // Inicializando protocolo ESP-NOW
  if (esp_now_init() != 0)
  {
      Serial.println("Error al Iniciar el Protocolo ESP NOW ... reconectando.");
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
  esp_now_set_self_role(2);


  pinMode(LUZ_ROJA, OUTPUT);

  // Recebendo mensagens de ESP-NOW MASTER
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {
     
  ESTRUCTURA_DATOS ED;
  memcpy(&ED, data, sizeof(ED));

  
  //Serial.print(ED.PULSO);
 
  if(ED.PULSO == 1){
      Serial.println("...♥...♥...Latido Recibido!"); 
      digitalWrite(LUZ_ROJA, HIGH);  
      //delay(3000);   
     // digitalWrite(LUZ_ROJA, HIGH);   
  }
  else{
      Serial.println("⚠ Iniciando Descarga ⚠ ");  
      digitalWrite(LUZ_ROJA, LOW);
  }
  });
}

void loop()
{
}
