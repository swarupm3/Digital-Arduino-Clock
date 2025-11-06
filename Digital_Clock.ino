#define segA 8
#define segB 7
#define segC 6
#define segD 5
#define segE 4
#define segF 3
#define segG 2
#define digitOne 9
#define digitTwo 10
#define digitThree 11
#define digitFour 12
#define potentiometerPinOne A0
#define potentiometerPinTwo A1


// segment encoding for digits 0-9
const int digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

const int segmentPins[7] = {segA, segB, segC, segD, segE, segF, segG};

bool setupMode = true;

int saved1, saved2, saved3, saved4, savedTotalmins;

unsigned long long lastIncrementTime = 0;

void setup() {
  // Set segment pins as outputs
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    digitalWrite(segmentPins[i], LOW); // Turn off initially
  }

  // Set digit control pins as outputs
  pinMode(digitOne, OUTPUT);
  pinMode(digitTwo, OUTPUT);
  digitalWrite(digitOne, HIGH); // Off initially
  digitalWrite(digitTwo, HIGH);

  pinMode(digitThree, OUTPUT);
  pinMode(digitFour, OUTPUT);
  digitalWrite(digitThree, HIGH); // Off initially
  digitalWrite(digitFour, HIGH);

  // Potentiometer input
  pinMode(potentiometerPinOne, INPUT);
  pinMode(potentiometerPinTwo, INPUT);

}

void displayDigit(int digit, int position) {
  // Turn off both digits before updating
  digitalWrite(digitOne, HIGH);
  digitalWrite(digitTwo, HIGH);
  digitalWrite(digitThree, HIGH);
  digitalWrite(digitFour, HIGH);

  // Set segment pins based on digit pattern (HIGH = ON for common cathode)
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], digits[digit][i] ? HIGH : LOW);
  }

  // Activate the correct digit (LOW = ON)
  if (position == 1) {
    digitalWrite(digitOne, LOW);
  } else if (position == 2) {
    digitalWrite(digitTwo, LOW);
  } else if (position == 3) {
    digitalWrite(digitThree, LOW);
  } else if (position == 4) {
    digitalWrite(digitFour, LOW);
  }
}

void loop() {
  // Start in setup mode to set the time
  if (setupMode) {                                               
      int SetupSwitch = analogRead(potentiometerPinTwo);
      int SetupSwitchMapped = map(SetupSwitch, 0, 1023, 0, 100);

      int potValueOne = analogRead(potentiometerPinOne);
      int totalmins = map(potValueOne, 0, 915, 0, 1439);

      // Break number into Hours and Minds
      int Hours = totalmins / 60;
      int Mins = totalmins % 60;
      int displayValue = Hours * 100 + Mins;

      int d1 = displayValue / 1000;           // Thousands place
      int d2 = (displayValue / 100) % 10;     // Hundreds
      int d3 = (displayValue / 10) % 10;      // Tens
      int d4 = displayValue % 10;             // Ones
      displayDigit(d1, 1);
      delay(4); 
      displayDigit(d2, 2);
      delay(4);
      displayDigit(d3, 3);
      delay(4); 
      displayDigit(d4, 4);
      delay(4);
      if (SetupSwitchMapped < 50) {  
          saved1 = d1;
          saved2 = d2;
          saved3 = d3;
          saved4 = d4;
          savedTotalmins = totalmins;
          setupMode = false;
      }
  } else {
      displayDigit(saved1, 1);
      delay(4); 
      displayDigit(saved2, 2);
      delay(4);
      displayDigit(saved3, 3);
      delay(4); 
      displayDigit(saved4, 4);
      delay(4);

      if (millis() - lastIncrementTime >= 60000) {
        lastIncrementTime = millis();  

        // Increment the time every minute
        savedTotalmins++;
        if (savedTotalmins >= 1440) {
          savedTotalmins = 0;
        }

        int newHours =  savedTotalmins / 60;
        int newMins = savedTotalmins % 60;
        int newDisplayVal = newHours * 100 + newMins;

        saved1 = newDisplayVal / 1000;       // Thousands place
        saved2 = (newDisplayVal / 100) % 10; // Hundreds place
        saved3 = (newDisplayVal / 10) % 10;  // Tens place
        saved4 = newDisplayVal % 10;         // Ones place
    }

  }
}
