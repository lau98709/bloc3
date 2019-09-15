#include "SerialCmd.h"
#include "FString.h"
#include "Stepper.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>


#define LED_PIN 13
#define STEPPER_PIN1 32
#define STEPPER_PIN2 33
#define STEPPER_PIN3 25
#define STEPPER_PIN4 26
#define FMAX_PIN 2
#define FMIN_PIN 15
#define BTN1_PIN 36
#define BTN2_PIN 39

Stepper4P *stepper;

int dir;
long dt;
int led;
long t, t2;

// Set these to your desired credentials.
const char *ssid = "ESP32";
const char *password = "";

WiFiServer server(80);

void Traitement(char *cmd) {
  FString s(cmd);
  s.ToUpper();
  if (s == "F") {
    dir = 1;
    Serial.println("FORWARD");
  }
  if (s == "R") {
    dir = -1;
    Serial.println("BACKWARD");
  }
  if (s == "S") {
    dir = 0;
    Serial.println("STOP");
  }
  if (s.BeginWith("D")) {
    dt = s.GetInt(2);
    Serial.print("DELAY=");
    Serial.println(dt);
  }
  if (s == "L") {
    led = (led == 0) ? 1 : 0;
    Serial.print("LED=");
    Serial.println(led);
  }
}


void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

  dir = 1;
  dt = 10;
  stepper = new Stepper4P(STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4, 16);

  led = 1;
  t = t2 = millis();

  pinMode(FMAX_PIN, INPUT);     // fin de course
  pinMode(FMIN_PIN, INPUT);     // fin de course

  pinMode(LED_PIN, OUTPUT);    // LED verte
  pinMode(BTN1_PIN, INPUT);     // bouton
  pinMode(BTN2_PIN, INPUT);      // bouton

  digitalWrite(LED_PIN, HIGH);
}

String currentLine = "";              // make a String to hold incoming data from the client

int HttpDaemon( WiFiClient client ) {
  int rval = 0;
  
  if (client.available()) {             // if there's bytes to read from the client,
    char c = client.read();             // read a byte, then
    Serial.write(c);                    // print it out the serial monitor
    if (c == '\n') {                    // if the byte is a newline character

      // if the current line is blank, you got two newline characters in a row.
      // that's the end of the client HTTP request, so send a response:
      if (currentLine.length() == 0) {
        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
        // and a content-type so the client knows what's coming, then a blank line:
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        // the content of the HTTP response follows the header:
        client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
        client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

        // The HTTP response ends with another blank line:
        client.println();
        // break out of the while loop:
        rval = 1;
      } else {    // if you got a newline, then clear currentLine:
        currentLine = "";
      }
    } else if (c != '\r') {  // if you got anything else but a carriage return character,
      currentLine += c;      // add it to the end of the currentLine
    }

    // Check to see if the client request was "GET /H" or "GET /L":
    if (currentLine.endsWith("GET /H")) {
      digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
    }
    if (currentLine.endsWith("GET /L")) {
      digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
    }

    return rval;
  }
}


void ServerLoop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    currentLine = "";
    while (client.connected()) {            // loop while the client's connected
      if (HttpDaemon(client) != 0) break;
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}


void loop() {
  if (dir != 0) {
    digitalWrite(LED_PIN, ((millis() % 1000) > 500));
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  if (millis() > t2) {
    if ((dir != 1) && (digitalRead(BTN1_PIN) == HIGH)) {
      Serial.println("FORWARD");
      dir = 1;
    }
    if ((dir != -1) && (digitalRead(BTN2_PIN) == HIGH)) {
      Serial.println("BACKWARD");
      dir = -1;
    }
    if ((dir != 0) && (digitalRead(BTN1_PIN) == HIGH) && (digitalRead(BTN2_PIN) == HIGH)) {
      Serial.println("STOP");
      dir = 0;
    }
    t2 = millis() + 200;
  }

  if ((dir == -1) && (digitalRead(FMAX_PIN) == HIGH)) dir = 0;
  if ((dir == 1) && (digitalRead(FMIN_PIN) == LOW)) dir = 0;

  if (dir != 0) stepper->Step(dir == 1);
  delayMicroseconds(((long)dt) * 100);

  if (millis() > t) {
    char txt[32];
    sprintf(txt, "dir=%d %d %d b1=%d b2=%d",
            dir,
            digitalRead(FMIN_PIN), digitalRead(FMAX_PIN),
            digitalRead(BTN1_PIN), digitalRead(BTN2_PIN));
    Serial.println(txt);
    t = millis() + 500;
  }

  ServerLoop();
}

