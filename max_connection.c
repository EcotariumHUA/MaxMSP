const int FSR_PINS[9] = {A0, A1, A2, A3, A4, 5, 6, 7, 8};
int new_package[9] = {0}, old_package[9] = {0};

const int THRESHOLD = 350;

const unsigned long SENSOR_INTERVAL = 50;
unsigned long previousMillis = 0;

const unsigned long DEBOUNCE_DELAY = 400;
unsigned long lastDebounceTime[9] = {0};

const int SENSOR_CAP = 1;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < SENSOR_CAP; i++) {
    pinMode(FSR_PINS[i], INPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
}

void loop() {

  unsigned long currentMillis = millis();
  
  for (int i = 0; i < SENSOR_CAP; i++) {
    old_package[i] = new_package[i];
  }

  if (currentMillis - previousMillis >= SENSOR_INTERVAL) {
    previousMillis = currentMillis;  // Reset timer

    for (int i = 0; i < SENSOR_CAP; i++) {
      int reading = analogRead(FSR_PINS[i]);  // Read each sensor
      int new_state = (reading > THRESHOLD) ? 1 : 0;

      if (new_state == new_package[i]) {
        lastDebounceTime[i] = currentMillis;
      }

      if ((currentMillis - lastDebounceTime[i]) >= DEBOUNCE_DELAY) {
        new_package[i] = new_state;
      }
    }
  }

  check_change(new_package, old_package);
}

void check_change(int new_package[9], int old_package[9]) {
  for (int i = 0; i < SENSOR_CAP; i++) {
    if (new_package[i] != old_package[i]) {
      if (new_package[i] == 1) {
        Serial.print(i + 1); // print 1-9 as changed value
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  }
}
