#include <Servo.h>

// COMPONENT VARIABLES
int buttons[4];
bool isButtonPressed[4];
int lockButton = 6;
bool isLockButtonPressed = false;
int yellowLED = 7;
int redLED = 8;
int greenLED = 9;
Servo servo;

// MISCELLANEOUS VARIABLES
bool isSetCombination = true;
int currentDigit = 0;
int combination[4];
int totalCorrect = 0;
int totalAttempts = 0;
bool isUnlocked = false;

void setup() {
  // put your setup code here, to run once:

  // SET UP BUTTONS
  for (int i = 0; i < 4; i++) {
    buttons[i] = i + 2;
    pinMode(buttons[i],INPUT_PULLUP);
    isButtonPressed[i] = false;
  }
  pinMode(lockButton,INPUT_PULLUP);

  // SET UP LED'S
  pinMode(yellowLED,OUTPUT);
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  digitalWrite(yellowLED,LOW);
  digitalWrite(redLED,LOW);
  digitalWrite(greenLED,LOW);

  // SET UP MINI SERVO
  servo.attach(10);
  servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Run this block if the lock has been successfully entered correctly. All variables are reset to original values but comination does not change until the Arduino is reset.
  if (isUnlocked) {
    if (digitalRead(lockButton) == LOW && !isLockButtonPressed) isLockButtonPressed = true;
    else if (digitalRead(lockButton) == HIGH && isLockButtonPressed) {
      isLockButtonPressed = false;
      servo.write(0);
      isUnlocked = false;
      currentDigit = 0;
      totalCorrect = 0;
      totalAttempts = 0;
      digitalWrite(greenLED,LOW);
    }
  }
  // User is prompted to enter a 4-digit security code. Once entered, the yellow LED will blink to indicate the system is armed.
  else {
    if (isSetCombination) {
      if (digitalRead(buttons[0]) == LOW && !isButtonPressed[0]) isButtonPressed[0] = true;
      else if (digitalRead(buttons[0]) == HIGH && isButtonPressed[0]) setCombinationDigit(0);

      if (digitalRead(buttons[1]) == LOW && !isButtonPressed[1]) isButtonPressed[1] = true;
      else if (digitalRead(buttons[1]) == HIGH && isButtonPressed[1]) setCombinationDigit(1);

      if (digitalRead(buttons[2]) == LOW && !isButtonPressed[2]) isButtonPressed[2] = true;
      else if (digitalRead(buttons[2]) == HIGH && isButtonPressed[2]) setCombinationDigit(2);

      if (digitalRead(buttons[3]) == LOW && !isButtonPressed[3]) isButtonPressed[3] = true;
      else if (digitalRead(buttons[3]) == HIGH && isButtonPressed[3]) setCombinationDigit(3);

      if (currentDigit == 4) {
        for (int i = 0; i < 3; i++) {
          digitalWrite(yellowLED,HIGH);
          delay(250);
          digitalWrite(yellowLED,LOW);
          delay(250);
        }
        isSetCombination = false;
        currentDigit = 0;
      }
    }
    else {
      if (currentDigit < 4) {
        if (digitalRead(buttons[0]) == LOW && !isButtonPressed[0]) isButtonPressed[0] = true;
        else if (digitalRead(buttons[0]) == HIGH && isButtonPressed[0]) assessChoice(0);

        if (digitalRead(buttons[1]) == LOW && !isButtonPressed[1]) isButtonPressed[1] = true;
        else if (digitalRead(buttons[1]) == HIGH && isButtonPressed[1]) assessChoice(1);

        if (digitalRead(buttons[2]) == LOW && !isButtonPressed[2]) isButtonPressed[2] = true;
        else if (digitalRead(buttons[2]) == HIGH && isButtonPressed[2]) assessChoice(2);

        if (digitalRead(buttons[3]) == LOW && !isButtonPressed[3]) isButtonPressed[3] = true;
        else if (digitalRead(buttons[3]) == HIGH && isButtonPressed[3]) assessChoice(3);
      }
      // After 4 digits have been entered, an evaluation is made. If all choices were correct, the system will unlock.
      // If incorrect, the red LED will turn on and the system will remain locked.
      // If three attempts were made, the system will remain locked indefinitely until the Arduino is manually reset.
      else {
        if (totalCorrect == 4) {
          digitalWrite(greenLED, HIGH);
          servo.write(90);
          isUnlocked = true;
        }
        else {
          if (totalAttempts < 2) {
            digitalWrite(redLED, HIGH);
            delay(500);
            digitalWrite(redLED, LOW);
            delay(500);

            for (int i = 0; i < 3; i++) {
              digitalWrite(yellowLED, HIGH);
              delay(250);
              digitalWrite(yellowLED, LOW);
              delay(250);
            }
          
            totalAttempts++;
            totalCorrect = 0;
            currentDigit = 0;
          }
          else digitalWrite(redLED, HIGH);
        }
      }
    }
  }
}

// Run this function when user presses a button. If the button matches the corresponding element of the combination array, the total correct choices is incremented.
void assessChoice(int button) {
  isButtonPressed[button] = false;
  if (combination[currentDigit] == button + 1) totalCorrect++;
  currentDigit++;
}

// A value is taken corresponding to the button choice and is stored in the combination array.
void setCombinationDigit(int button) {
  isButtonPressed[button] = false;
  combination[currentDigit] = button + 1;
  currentDigit++;
}
