//Librerias

#include <WiFi.h>
#include <Servo.h>
#include <HTTPClient.h>
#include "ThingsBoard.h"
#include "CTBot.h"

//Credenciales para la conexión a la API de la visualización web
#define TOKEN "XCDjG1xsWwDL4EJ5C3Ob"
#define THINGSBOARD_SERVER "demo.thingsboard.io"

//Credenciales de mi internet de casa
const char* ssid = "LIB-3532968";
const char* password = "fyer4dakYNe8";

//Contadores declaradas variables locales
int vidrio = 0;
int carton = 0;
int metal = 0;
int plastico = 0;

String nombres[2] = {"Rances","Jose"};
int puntos[2] = {0,0};

String desecho;

//Configuración para el envío HTTP
WiFiClient client;
ThingsBoard tb(client);

//Declaración del objeto servo
Servo myserv;

CTBot miBot;

const String token = "5819638343:AAGEysU29uLDqNXTSH_LRtOfha7Zz0viJW4"; 

bool band = false;

const int pinB = 13;

void setup() {

  // Inicialización del baudio
  Serial.begin(115200);
  
  Serial.println("•❲🛰❳︶︶︶︶︶︶︶︶︶︶︶❲🛰❳•\n │ㅤㅤ──╯꒰☁︎ ꒱ ʟɪʟʏ ꒰☁︎ ꒱╰──\n │✄. . . . . . . .  . . . . . . . . . . . . . . . ↻° :   ﹢\n │ㅤ↬xㅤ✰;; _Conexión Exitosa_  ♡ʾʾ . . \n │✄. . . . . . . . . . . . . . . . . . .ⵓ‧˖˚࿔₊˚\n ☁︎ ♡ ʟɪʟʏ ☁︎ ♡ \n •❲🛰❳︶︶︶︶︶︶︶︶︶︶︶❲🛰❳•");

  //Iniciar conexión con el internet
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Iniciar conexión con la visualización web
  if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
    Serial.println("Error al conectarse a ThingsBoard por culpa del Fablab");
  } else {
    Serial.println("Conexión exitosa a ThingsBoard");
  }

  miBot.setTelegramToken(token);
  
  if (miBot.testConnection()) {
    Serial.println("\n Conectado");
  }
  else {
    Serial.println("\n Problemas Auxilio");
  }

  //Definimos el pin donde está conectado nuestro servomotor
  myserv.attach(2);
  //Declaramos que se mantenga al iniciar en la posición del °0
  myserv.write(0);

}

void loop() {

  //Condicional donde recibiremos el mensaje de la red neuronal por medio de la conexión serial
  if(Serial.available()){
    String data = Serial.readStringUntil('\n');
    data.trim();

    //Almacenamos el mensaje de la red neuronal en una variable entera
    int categoria = data.substring(data.indexOf("%") + 1, data.indexOf(";")).toInt();
    //Condicionales para declarar que tipo de desecho es, aumentar su contador, mover el servo en los grados de su contenedor y el envío del dato a la visualización web
    if(categoria == 0) {
      vidrio++;
      desecho = "Vidrio";
      myserv.write(45);
      tb.sendTelemetryFloat("Vidrio", vidrio); 
    }
    else if (categoria == 1) { 
      carton++;
      desecho = "Carton";
      myserv.write(90);
      tb.sendTelemetryFloat("Carton", carton); 
    }
    else if (categoria == 2) { 
      metal++;
      desecho = "Metal";
      myserv.write(135);
      tb.sendTelemetryFloat("Metal", metal);
    }
    else {
      plastico++;
      desecho = "Plastico";
      myserv.write(180);
      tb.sendTelemetryFloat("Plastico", plastico); 
    }
    band = true;
  }

  TBMessage msg;

  if (CTBotMessageText == miBot.getNewMessage(msg)) {
    Serial.println("Mensaje: "+String(msg.sender.firstName)+" - "+String(msg.text)+"\n");

    if (msg.text.equalsIgnoreCase("000333")) {
      Serial.println("1");
      puntos[0]++;
      miBot.sendMessage(msg.sender.id, "Log test \nNombre: "+String(nombres[0])+"\nPuntos: "+String(puntos[0])+"\nDesecho: "+String(desecho));
      band = false;
    }
    else if (msg.text.equalsIgnoreCase("000444")) {
      Serial.println("1");
      puntos[1]++;
      miBot.sendMessage(msg.sender.id, "Log test \nNombre: "+String(nombres[1])+"\nPuntos: "+String(puntos[1])+"\nDesecho: "+String(desecho));
      band = false;
    }
    else {
     miBot.sendMessage(msg.sender.id, "Bienbenido " + msg.sender.firstName + ",intenta usar: encender o apagar");
    }
  }

  if (digitalRead(pinB) == 0) { Serial.println("1"); }

  delay(250);

}
