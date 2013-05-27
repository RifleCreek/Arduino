const int stepPin = 16;
const int dirPin = 17;

const int ledPin = 13;

int dir = HIGH;
int steps = 1500;
int wait = 1600;
int waitInc = -1;

void setup()
{  
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(ledPin, OUTPUT);

  digitalWrite(dirPin, LOW);
}

void loop()
{
  if (steps >= 3000) {
    steps = 0;
    if (dir == HIGH)
      dir = LOW;
    else
      dir = HIGH;
    digitalWrite(ledPin, dir);
    digitalWrite(dirPin, dir);
  }
  
  if (wait < 100 || wait > 1600) {
    waitInc *= -1;
  }
  
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(wait);

  digitalWrite(stepPin, LOW);
  delayMicroseconds(wait);
  
  steps++;
  wait += waitInc;
}

