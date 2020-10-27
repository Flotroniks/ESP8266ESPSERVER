#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
std::string page =
#include <homepage.html>
;

const char *ssid = "Nom du réseau auquel vous voulez vous connecter";
const char *password = "Mot de passe du réseau auquel vous voulez vous connecter";
ESP8266WebServer server(80);

const int led = 2;
bool etatLed = 0;
char texteEtatLed[2][10] = {"ÉTEINTE!","ALLUMÉE!"};

void handleRoot()
{
    String page = "<!DOCTYPE html>";

    

    server.setContentLength(page.length());
    server.send(200, "text/html", page);
}

void handleOn()
{
    etatLed = 1;
    digitalWrite(led, HIGH);
    server.sendHeader("Location","/");
    server.send(303);
}

void handleOff()
{
    etatLed = 0;
    digitalWrite(led, LOW);
    server.sendHeader("Location","/");
    server.send(303);
}

void handleNotFound()
{
    server.send(404, "text/plain", "404: Not found");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n");

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);

    WiFi.persistent(false);
    WiFi.begin(ssid, password);
    Serial.print("Tentative de connexion...");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Serial.println("\n");
    Serial.println("Connexion etablie!");
    Serial.print("Adresse IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("Serveur web actif!");
}

void loop()
{
    server.handleClient();
}