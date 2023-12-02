// VARIABLE DECLARATIONS
int buttons[4];
bool isButtonsPressed[4];
int greenLED = 11;
int redLED = 12;
int whiteLED = 13;
bool isSetCombination = true;
int combination[4];
int currentDigit = 0;
int totalCorrect = 0;
int totalAttempts = 0;

void setup() {
  // put your setup code here, to run once:
  // the buttons and correlating variable values are set up.
  for (int i = 0; i < 4; i++) {
    buttons[i] = i + 2;
    pinMode(buttons[i], INPUT_PULLUP);
    isButtonsPressed[i] = false;
  }
  
  // LED setup
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // The program opens with a combination initializer.
  // After each value of the combination array is filled, this if statement will no longer be executed.
  if (isSetCombination) {
    if (digitalRead(buttons[0]) == LOW && !isButtonsPressed[0]) isButtonsPressed[0] = true;
    else if (digitalRead(buttons[0]) == HIGH && isButtonsPressed[0]) setCombinationDigit(0);

    if (digitalRead(buttons[1]) == LOW && !isButtonsPressed[1]) isButtonsPressed[1] = true;
    else if (digitalRead(buttons[1]) == HIGH && isButtonsPressed[1]) setCombinationDigit(1);

    if (digitalRead(buttons[2]) == LOW && !isButtonsPressed[2]) isButtonsPressed[2] = true;
    else if (digitalRead(buttons[2]) == HIGH && isButtonsPressed[2]) setCombinationDigit(2);

    if (digitalRead(buttons[3]) == LOW && !isButtonsPressed[3]) isButtonsPressed[3] = true;
    else if (digitalRead(buttons[3]) == HIGH && isButtonsPressed[3]) setCombinationDigit(3);

    if (currentDigit == 4) {
      for (int i = 0; i < 3; i++) {
        digitalWrite(whiteLED, HIGH);
        delay(250);
        digitalWrite(whiteLED, LOW);
        delay(250);
      }
      isSetCombination = false;
      currentDigit = 0;
    }
  }
  // After any button is pressed, the program will check to see if the button matches the stored combination.
  // User has 3 attempts to guess correctly. After third attempt, the user will be locked out.
  else {
    if (currentDigit < 4) {
      if (digitalRead(buttons[0]) == LOW && !isButtonsPressed[0]) isButtonsPressed[0] = true;
      else if (digitalRead(buttons[0]) == HIGH && isButtonsPressed[0]) assessChoice(0);

      if (digitalRead(buttons[1]) == LOW && !isButtonsPressed[1]) isButtonsPressed[1] = true;
      else if (digitalRead(buttons[1]) == HIGH && isButtonsPressed[1]) assessChoice(1);

      if (digitalRead(buttons[2]) == LOW && !isButtonsPressed[2]) isButtonsPressed[2] = true;
      else if (digitalRead(buttons[2]) == HIGH && isButtonsPressed[2]) assessChoice(2);

      if (digitalRead(buttons[3]) == LOW && !isButtonsPressed[3]) isButtonsPressed[3] = true;
      else if (digitalRead(buttons[3]) == HIGH && isButtonsPressed[3]) assessChoice(3);
    }
    else {
      if (totalCorrect == 4) digitalWrite(greenLED, HIGH);
      else {
        if (totalAttempts < 2) {
          digitalWrite(redLED, HIGH);
          delay(500);
          digitalWrite(redLED, LOW);
          delay(500);

          for (int i = 0; i < 3; i++) {
            digitalWrite(whiteLED, HIGH);
            delay(250);
            digitalWrite(whiteLED, LOW);
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

// This function assesses the chosen button with the stored combination.
// If the two values match, the total correct value is incremented.
void assessChoice(int button) {
  isButtonsPressed[button] = false;
  if (combination[currentDigit] == button + 1) totalCorrect++;
  currentDigit++;
}

// This function stores the selected button as a combination element.
void setCombinationDigit(int button) {
  isButtonsPressed[button] = false;
  combination[currentDigit] = button + 1;
  currentDigit++;
}