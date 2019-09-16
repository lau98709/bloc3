//===============================================================
// Programme de gestion d'un store automatique
// avec interface web.
//
// Auteurs : LAU Wai Tong Christian, TAMATAI Tautu
// Dans le cadre du DIU EIL 2019.
// Université de la Polynésie Française
// Tahiti, Polynésie Française
// Septembre 2019
//
//===============================================================

#include "Stepper.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include "index.h"

// Les broches d'E/S
#define LED_PIN 13
#define STEPPER_PIN1 32
#define STEPPER_PIN2 33
#define STEPPER_PIN3 25
#define STEPPER_PIN4 26
#define FDC_OUV_PIN 2
#define FDC_FERM_PIN 15
#define BTN_FERM_PIN 36
#define BTN_OUV_PIN 39

// Objet gestion du moteur pas à pas
Stepper4P *stepper;

int dir;    // direction : dir = 1, -1, 0=arrêt
long dt;    // délai d'attente par pas
long t, t2;   // temps limites
int etat;     // etat : 0=arrêt, 1=ouverture, 2=fermeture

// Paramètres wifi
const char *ssid = "ESP32";
const char *password = "";

// Objet représentant le serveur web
WebServer server(80);

//===============================================================
// Fonctions gestionnaires d'événements
//===============================================================

// La page racine
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

// Récupération des valeurs des E/S et de l'état
void GetValues() {
  String s = "etat:"+String(etat)+"\n";
  s = s + "fo:"+String(digitalRead(FDC_OUV_PIN))+"\n";
  s = s + "ff:"+String(digitalRead(FDC_FERM_PIN))+"\n";
  s = s + "bo:"+String(digitalRead(BTN_OUV_PIN))+"\n";
  s = s + "bf:"+String(digitalRead(BTN_FERM_PIN))+"\n";
  server.send(200, "text/plane", s); //Send ADC value only to client ajax request
}

// Appui sur le bouton "Arrêter" sur la page web
void BtnArret() {
  //
  // A COMPLETER
  //
  
}

// Appui sur le bouton "Ouvrir" sur la page web
void BtnOuvrir() {
  //
  // A COMPLETER
  //
  
}

// Appui sur le bouton "Fermer" sur la page web
void BtnFermer() {
  //
  // A COMPLETER
  //
  
}

//===============================================================
// Initialisation
//===============================================================

void setup() {
  Serial.begin(115200);     // Initialisation sortie port série (=console)

  Serial.println();
  Serial.println("Configuring access point...");

  // Configuration du point d'accès wifi
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //----- Gestion des événements
  server.on("/", handleRoot);             // La page principale
  server.on("/GetValues", GetValues);     // Récupération des valeurs des E/S
  server.on("/BtnArret", BtnArret);       // Appui sur le bouton "Arrêter"
  server.on("/BtnOuvrir", BtnOuvrir);     // Appui sur le bouton "Ouvrir"
  server.on("/BtnFermer", BtnFermer);     // Appui sur le bouton "Fermer"

  // Démarrage du serveur web
  server.begin();
  Serial.println("Server started");

  // Initialisation des paramètre du moteur pas à pas
  dir = 1;
  dt = 10;
  stepper = new Stepper4P(STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4, 16);

  // Initialisation des temps limites (pour les délais)
  t = t2 = millis();

  // Initialisation des E/S
  pinMode(FDC_OUV_PIN, INPUT);      // fin de course
  pinMode(FDC_FERM_PIN, INPUT);     // fin de course

  pinMode(LED_PIN, OUTPUT);         // LED verte
  pinMode(BTN_FERM_PIN, INPUT);     // bouton
  pinMode(BTN_OUV_PIN, INPUT);      // bouton

  digitalWrite(LED_PIN, HIGH);      // Extinction de la LED

  etat = 0;     // Démarrage en état d'arrêt
}

//===============================================================
// Boucle principale
//===============================================================

void loop() {
  // Réalisation des actions selon l'état
  switch (etat) {
    case 0:     // Arrêt
      //
      // A COMPLETER
      //
      
      break;

    case 1:     // Ouverture
      digitalWrite(LED_PIN, ((millis() % 1000) > 500));
      stepper->Step(false);
      delayMicroseconds(((long)dt) * 100);
      break;

    case 2:     // Fermeture
      //
      // A COMPLETER
      //
      
      break;
  }

  // Gestion des transitions
  switch (etat) {
    case 0:     // Arrêt
      if (digitalRead(BTN_OUV_PIN) == HIGH) {     // Si appui sur bouton ouverture
        Serial.println("Ouverture");
        etat = 1;
        delay(200);
      }
      //
      // A COMPLETER
      //
      
      break;

    case 1:     // Ouverture
      // Si appui sur un bouton ou fin de course ouverture actif
      // alors aller à l'état 0=arrêt
      //
      // A COMPLETER
      //
      
      break;

    case 2:     // Fermeture
      // Si appui sur un bouton ou fin de course fermeture actif
      // alors aller à l'état 0=arrêt
      //
      // A COMPLETER
      //
      
      break;
  }

  // Gestion des événements du serveur
  server.handleClient();
}

