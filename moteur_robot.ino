#include <WiFi.h>
#include <WebServer.h>


const char* ap_ssid = "SmartCar"; // Nom du point d'accès WiFi créé par l'ESP32
const char* ap_password = "12345678"; // Mot de passe du point d'accès WiFi
IPAddress ap_ip(192, 168, 1, 10); // Adresse IP fixe pour le point d'accès WiFi

WebServer server(80);

int sensorPin = 18;
int roue_gauche_avant1 = 22;
int roue_gauche_avant2 = 23;
int roue_gauche_arriere1 = 4;
int roue_gauche_arriere2 = 5;
int roue_droit_avant1 = 12;
int roue_droit_avant2 = 13;
int roue_droit_arriere1 = 25;
int roue_droit_arriere2 = 27;

const int ledPin1 = 50; // Broche de la LED1
const int ledPin2 = 55; // Broche de la LED2
const int ledPin3 = 60; // Broche de la LED3
const int ledPin4 = 65; // Broche de la LED4

bool ledState1 = false;
bool ledState2 = false;
bool ledState3 = false;
bool ledState4 = false;

void handleRoot() {
  String htmlPage = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>SmartCar</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            background-color: #f0f0f0;
        }
        .container {
            text-align: center;
            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
            padding: 20px;
            border-radius: 10px;
            background-color: white;
        }
        button {
            margin: 10px;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            color: white;
            font-size: 16px;
            box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
        }
        button:hover {
            box-shadow: none;
        }
        #btn1 {
          background-color: #0000ff; /* Blue */
        }
        #btn2 {
          background-color: #0000ff; /* Blue */
        }
        #btn3 {
          background-color: #0000ff; /* Blue */
        }
        #btn4 {
          background-color: #0000ff; /* Blue */
        }
    </style>
</head>
<body>
<div class="container">
    <h1>SmartCar</h1>
<div class="details">
            <div class="col">
                <div id="sensorState" class="circle"></div>
            </div>
        </div>    
    <button id="btn1" onclick="toggleLED(1)">Avancer</button>
    <button id="btn2" onclick="toggleLED(2)">Gauche</button>
    <button id="btn3" onclick="toggleLED(3)">Droite</button>
    <button id="btn4" onclick="toggleLED(4)">Reculer</button>
</div>

<script>
function toggleLED(id) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/toggle" + id, true);
    xhr.send();
}

function updateData() {
          var xhr = new XMLHttpRequest();
          xhr.onreadystatechange = function() {
            if (xhr.readyState == 4 && xhr.status == 200) {
              var data = JSON.parse(xhr.responseText);
              document.getElementById("sensorState").style.backgroundColor = data.sensorState ? "red" : "green";
            }
          };
          xhr.open("GET", "/data", true);
          xhr.send();
        }

function handleSensorUpdate(sensorValue) {
    updateSensorCircle(sensorValue);
}

</script>

</body>
</html>

)=====";
server.send(200, "text/html", htmlPage);
}

void handleToggle1() {
ledState1 = !ledState1;
digitalWrite(ledPin1, ledState1 ? HIGH : LOW);
if (ledState1 == HIGH ){
  ledState2 = false;
  ledState3 = false;
  ledState4 = false;  
arret();
avancer();}
else {arret();}
server.send(200, "text/plain", String(ledState1));
}

void handleToggle2() {
ledState2 = !ledState2;
digitalWrite(ledPin2, ledState2 ? HIGH : LOW);
if (ledState2 == HIGH ){
  ledState1 = false;
  ledState3 = false;
  ledState4 = false;  
arret();
gauche();}
else {arret();}
server.send(200, "text/plain", String(ledState2));
}

void handleData() {
  bool sensorState = digitalRead(sensorPin) == LOW;

  String jsonData = "{\"sensorState\": " + String(sensorState) + "}";
  server.send(200, "application/json", jsonData);
}

void handleToggle3() {
ledState3 = !ledState3;
digitalWrite(ledPin3, ledState3 ? HIGH : LOW);
if (ledState3 == HIGH ){
  ledState2 = false;
  ledState1 = false;
  ledState4 = false;  
arret();
droite();}
else {arret();}
server.send(200, "text/plain", String(ledState3));
}

void handleToggle4() {
ledState4 = !ledState4;
digitalWrite(ledPin4, ledState4 ? HIGH : LOW);
if (ledState4 == HIGH ){
  ledState2 = false;
  ledState3 = false;
  ledState1 = false;  
arret();
reculer();}
else {arret();}
server.send(200, "text/plain", String(ledState4));
}

void setup() {
Serial.begin(115200);

pinMode(sensorPin, INPUT);

pinMode(roue_gauche_avant1, OUTPUT);
pinMode(roue_gauche_avant2, OUTPUT);
pinMode(roue_gauche_arriere1, OUTPUT);
pinMode(roue_gauche_arriere2, OUTPUT);
pinMode(roue_droit_avant1, OUTPUT);
pinMode(roue_droit_avant2, OUTPUT);
pinMode(roue_droit_arriere1, OUTPUT);
pinMode(roue_droit_arriere2, OUTPUT);

pinMode(ledPin1, OUTPUT);
pinMode(ledPin2, OUTPUT);
pinMode(ledPin3, OUTPUT);
pinMode(ledPin4, OUTPUT);

digitalWrite(ledPin1, LOW);
digitalWrite(ledPin2, LOW);
digitalWrite(ledPin3, LOW);
digitalWrite(ledPin4, LOW);

// Configurer l'ESP32 en tant que point d'accès WiFi avec l'adresse IP fixe
WiFi.softAPConfig(ap_ip, ap_ip, IPAddress(255, 255, 255, 0));
WiFi.softAP(ap_ssid, ap_password);

Serial.println("Point d'accès WiFi créé !");
Serial.print("Adresse IP du point d'accès WiFi: ");
Serial.println(ap_ip);

// Définir les gestionnaires d'URL pour le serveur
server.on("/", handleRoot);
server.on("/toggle1", handleToggle1);
server.on("/toggle2", handleToggle2);
server.on("/toggle3", handleToggle3);
server.on("/toggle4", handleToggle4);

server.begin();
Serial.println("Serveur Web démarré !");
}

void loop() {
server.handleClient();
}

void avancer () {
digitalWrite(roue_gauche_avant1,1);
digitalWrite(roue_gauche_avant2,0); 
digitalWrite(roue_gauche_arriere1,1);
digitalWrite(roue_gauche_arriere2,0);
digitalWrite(roue_droit_avant1,1);
digitalWrite(roue_droit_avant2,0);
digitalWrite(roue_droit_arriere1,1);
digitalWrite(roue_droit_arriere2,0);
}

void reculer () {
digitalWrite(roue_gauche_avant1,0);
digitalWrite(roue_gauche_avant2,1);
digitalWrite(roue_gauche_arriere1,0);
digitalWrite(roue_gauche_arriere2,1);
digitalWrite(roue_droit_avant1,0);
digitalWrite(roue_droit_avant2,1);
digitalWrite(roue_droit_arriere1,0);
digitalWrite(roue_droit_arriere2,1);
}

void arret () {
digitalWrite(roue_gauche_avant1,0);
digitalWrite(roue_gauche_avant2,0); 
digitalWrite(roue_gauche_arriere1,0);
digitalWrite(roue_gauche_arriere2,0);
digitalWrite(roue_droit_avant1,0);
digitalWrite(roue_droit_avant2,0);
digitalWrite(roue_droit_arriere1,0);
digitalWrite(roue_droit_arriere2,0);
}

void gauche () {
digitalWrite(roue_gauche_avant1,0);
digitalWrite(roue_gauche_avant2,0); 
digitalWrite(roue_gauche_arriere1,0);
digitalWrite(roue_gauche_arriere2,0);
digitalWrite(roue_droit_avant1,1);
digitalWrite(roue_droit_avant2,0);
digitalWrite(roue_droit_arriere1,1);
digitalWrite(roue_droit_arriere2,0);
}

void droite () {
digitalWrite(roue_gauche_avant1,1);
digitalWrite(roue_gauche_avant2,0); 
digitalWrite(roue_gauche_arriere1,1);
digitalWrite(roue_gauche_arriere2,0);
digitalWrite(roue_droit_avant1,0);
digitalWrite(roue_droit_avant2,0);
digitalWrite(roue_droit_arriere1,0);
digitalWrite(roue_droit_arriere2,0);
}