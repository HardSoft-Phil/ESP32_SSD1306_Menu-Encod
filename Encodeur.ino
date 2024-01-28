/*
Fonctions publiques utilisables avec la bibliothèque SuperEncodeur:
  SuperEncodeur(uint8_t aPin, uint8_t bPin, bouton, int16_t minValue, int16_t maxValue, int16_t initalValue, uint8_t type = FULL_PULSE);
  SuperEncodeur();
  bool begin();
  void configure(uint8_t aPin, uint8_t bPin, bouton, int16_t minValue, int16_t maxValue, int16_t initalValue, uint8_t type = FULL_PULSE);
  void end();
  bool enabled();
  void attachCallback(EncoderCallBack cback, void *uPtr = nullptr);
  bool buttonPressed();
  bool upClick();
  bool downClick();
  bool getState(EncoderState &state);
  int16_t getValue();
  void setValue(int16_t val);
  bool getAndSet(int16_t val, EncoderState &Oldstate, EncoderState &Newstate);
  bool newSettings(int16_t newMin, int16_t newMax, int16_t newCurrent, EncoderState &state);

  SuperEncodeur(const SuperEncodeur&) = delete; // delete copy constructor. no copying allowed
  SuperEncodeur& operator=(const SuperEncodeur&) = delete; // delete operator=(). no assignment allowed
*/
