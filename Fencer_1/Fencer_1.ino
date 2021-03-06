#include "WiFi.h"
#include <esp_now.h>
#include <WiFi.h>

const int LEDPIN = 25;///on adafruit esp32 pin A1 is gpio is 25
const int PushButton = 13;//pin the b-wire goes into

int timeout = 40;//touch timeout window

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
float last_received = 0;//time the last touch was received
float last_given = 0;//time the last touch was given


// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5;    // the debounce time; increase if the output flickers

// 40:F5:20:46:2A:8C : Fencer 2 mac address
//  40:F5:20:45:2D:80 : Fencer 1 mac address

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x40, 0xF5, 0x20, 0x46, 0x2A, 0x8C}; // 40:F5:20:46:2A:8C : Fencer 2 mac address
//uint8_t broadcastAddress[] = {0x40, 0xF5, 0x20, 0x45, 0x2D, 0x80}; //  40:F5:20:45:2D:80 : Fencer 1 mac address

typedef struct struct_message {
  char character[100];
  float floating_value;
} struct_message;

struct_message message;

void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus of Last Message Sent:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void data_receive(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(message.character);
  Serial.print("Float: ");
  Serial.println(message.floating_value);
  //above can be removed it is helpful for demonstrating it works while connected to a computer
  last_received = millis();
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(data_sent);

  esp_now_peer_info_t peerInfo;
  esp_now_register_recv_cb(data_receive);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(LEDPIN, OUTPUT);
  pinMode(PushButton, INPUT);

}

void loop() {

  int Push_button_state = digitalRead(PushButton);
  // if condition checks if push button is pressed
  // if pressed LED will turn on otherwise remain off
  int reading = digitalRead(PushButton);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if ( Push_button_state == HIGH)
      {
          last_given = millis();
          strcpy(message.character, "Fencer 1 hit");
          message.floating_value = millis();
          esp_err_t outcome = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));

          if (outcome == ESP_OK) {
            Serial.println("Mesage sent successfully!");
          }
          else {
            Serial.println("Error sending the message");
          }
        score();
      }
      else
      {
        digitalWrite(LEDPIN, LOW);
        ledState = LOW;

      }

    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

}

void score() {

  //if the touch has not been timed out
  if (abs(last_received - last_given) < timeout || abs(last_received - last_given) > 2000) {
      digitalWrite(LEDPIN, HIGH);
      Serial.println("Point scored");
      delay(1000);
      digitalWrite(LEDPIN, LOW);
  }

}
