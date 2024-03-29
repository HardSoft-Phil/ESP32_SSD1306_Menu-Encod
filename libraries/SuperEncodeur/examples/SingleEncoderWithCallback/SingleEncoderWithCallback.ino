#include "Arduino.h"
#include "SuperEncodeur.h"

void ESP_ISR callBack(SuperEncodeur *encPtr, const volatile SuperEncodeur::EncoderState *state, void *uPtr);

// Pins 2 and 3 should work for many processors, including Uno. See README for meaning of constructor arguments.
// Use FULL_PULSE for encoders that produce one complete quadrature pulse per detnet, such as: https://www.adafruit.com/product/377
// Use HALF_PULSE for endoders that produce one complete quadrature pulse for every two detents, such as: https://www.mouser.com/ProductDetail/alps/ec11e15244g1/?qs=YMSFtX0bdJDiV4LBO61anw==&countrycode=US&currencycode=USD
SuperEncodeur encoder(2, 3, -20, 20, 0, FULL_PULSE);

int16_t prevEncoderValue;
volatile SuperEncodeur::EncoderState newState;
volatile bool newValue = false;

void setup() {
  SuperEncodeur::EncoderState state;

  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting");
  if (!encoder.begin()) {
    Serial.println("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting.");
    while (1) {
      yield();
    }
  } else {
    encoder.getState(state);
    Serial.print("Encoder Successfully Started at value = ");
    prevEncoderValue = state.currentValue;
    Serial.println(prevEncoderValue);
  }
  encoder.attachCallback(callBack);
}

void loop() {
  int16_t currentValue;
  SuperEncodeur::EncoderClick currentClick;

  if (newValue) {
    noInterrupts();
    currentValue = newState.currentValue;
    currentClick = newState.currentClick;
    newValue = false;
    interrupts();
    Serial.print("Encoder: ");
    if (currentValue != prevEncoderValue) {
      Serial.println(currentValue);
      prevEncoderValue = currentValue;
    } else {
      switch (currentClick) {
        case SuperEncodeur::UpClick:
          Serial.println("at upper limit.");
          break;

        case SuperEncodeur::DownClick:
          Serial.println("at lower limit.");
          break;

        default:
          break;
      }
    }
  }
}

void ESP_ISR callBack(SuperEncodeur *encPtr, const volatile SuperEncodeur::EncoderState *state, void *uPtr) {
  (void) encPtr;
  (void) uPtr;
  memcpy((void *)&newState, (void *)state, sizeof(SuperEncodeur::EncoderState));
  newValue = true;
}
