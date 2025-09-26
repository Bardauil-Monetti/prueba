//hecho por Monnetti y Ortiz
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Telecentro-da0d";
const char* password = "H4JTRX4NGL9L";

WebServer server(80);
int leds [5] = {14,27,26,25,33};  
int animacion1 [5] = {0,1,0,1,1}; 
int animacion2 [5][5] = {
  {0,0,1,0,0},
  {0,1,0,0,0},
  {0,0,0,1,0},
  {0,1,0,0,1},
  {1,0,1,0,0}
}; 
int apagarleds [5] = {0,0,0,0,0}; 

// HTML principal
const char pagina[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>Animaciones ESP32</title>
  <style>
    body { text-align: center; font-family: Arial; }
    h1 { color: #333; }
    button {
      width: 150px; height: 50px;
      font-size: 18px;
      margin: 10px;
      border: none;
      border-radius: 8px;
      cursor: pointer;
    }
    .animacion1 { background-color: red; color: white; }
    .animacion2 { background-color: yellow; color: black; }
    .apagar { background-color: magenta; color: white; }
  </style>
</head>
<body>
  <h1>Animaciones ESP32</h1>
  <a href="/animacion1"><button class="animacion1">Animación 1</button></a>
  <a href="/animacion2"><button class="animacion2">Animación 2</button></a>
  <a href="/apagar"><button class="apagar">Apagar</button></a>
</body>
</html>
)rawliteral";

// Funciones
void animacion1_funcion() {
  Serial.println("Request: Animación 1");
  for (int i=0;i<5;i++) {
    digitalWrite(leds[i], animacion1[i]);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void animacion2_funcion() {
  Serial.println("Request: Animación 2");
  for (int i=0;i<5;i++) {
    for (int j=0;j<5;j++) {
      digitalWrite(leds[j], animacion2[i][j]);
    }
    delay(500);  
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void apagar_funcion() {
  Serial.println("Request: Apagar LEDs");
  for (int i=0;i<5;i++) {
    digitalWrite(leds[i], apagarleds[i]);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  for (int i=0;i<5;i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());

  // Rutas
  server.on("/", []() {
    server.send(200, "text/html", pagina);
  });
  server.on("/animacion1", animacion1_funcion);
  server.on("/animacion2", animacion2_funcion);
  server.on("/apagar", apagar_funcion);

  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
}
