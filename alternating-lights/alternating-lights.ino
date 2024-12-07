class LED {
  private:
    int pin;
    int pwmValue = 0;
    bool goingUp = true;
    int step = 30;
    unsigned long previousMillis = 0;
    bool active = false;
    unsigned long startTime = 0;
    int maxPwmValue = 120;

  public:
    LED(int pinNumber) {
      pin = pinNumber;
      pinMode(pin, OUTPUT);
    }

    void incrementLight() {
      pwmValue += step;
    }

    void decrementLight() {
      pwmValue -= step;
    }

    bool isGoingUp() {
      return goingUp;
    }

    int getPwmValue() {
      return pwmValue;
    }

    void switchDirection() {
      goingUp = !goingUp;
    }

    void off() {
      analogWrite(pin, 0);
    }

    void on() {
      analogWrite(pin, pwmValue);
    }

    unsigned long getPreviousMillis() {
      return previousMillis;
    }

    void setPreviousMillis(unsigned long milliseconds) {
      previousMillis = milliseconds;
    }

    bool isActive() {
      return active;
    }

    void setIsActive(bool isActive) {
      active = isActive;
    }

    void setStartTime(unsigned long ms) {
      startTime = ms;
    }

    unsigned long getStartTime() {
      return startTime;
    }

    int getStep() {
      return step;
    }

    int getMaxPwmValue() {
      return maxPwmValue;
    }
};

int buttonPin = 2;
LED led1(5);
LED led2(3);
LED led3(6);
LED led4(10);
LED led5(11);
LED led6(9);
int onLed = 12;
bool hasPressed = false;
unsigned long startTime = 0; 

unsigned long lastOn = 0;

LED* leds[] = {&led1, &led2, &led3, &led4, &led5, &led6};
unsigned long delays[] = {0, 20, 40, 60, 80, 100};

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(onLed, OUTPUT);
  digitalWrite(onLed, HIGH);
  led1.setIsActive(true);
  Serial.begin(9600);
}

void executeLED(LED& led) {
  if (led.isActive() && millis() - led.getPreviousMillis() >= led.getStep()) {
    led.setPreviousMillis(millis());

    if (led.isGoingUp()) {
      if (led.getPwmValue() < led.getMaxPwmValue()) {
        led.incrementLight();
      } else {
        led.switchDirection();
      }
    } else {
      if (led.getPwmValue() > 0) {
        led.decrementLight();
      } else {
        led.switchDirection();
      }
    }
    led.on();
  }
}

void loop() {
  if(millis() - lastOn >= 250) {
    int state = digitalRead(onLed);
    if(state == 0) {
      digitalWrite(onLed, HIGH);
    }
    else {
      digitalWrite(onLed, LOW);
    }
    lastOn = millis();
  }




  
  if (digitalRead(buttonPin) == HIGH) {
    hasPressed = true;
    led1.setStartTime(millis()); 
  }

  if (hasPressed) {
    executeLED(led1);

    for (int i = 0; i < 6; ++i) {
      if (!leds[i]->isActive() && millis() - leds[i-1]->getStartTime() >= delays[i] && leds[i-1]->getStartTime() > 0) {
        leds[i]->setIsActive(true);
        leds[i]->setPreviousMillis(millis());
        leds[i]->setStartTime(millis());
      }
      if (leds[i-1]->getStartTime() > 0) {
        executeLED(*leds[i]);
      }
    }
  }
}
