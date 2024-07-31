#define GREEN_LED 13
#define RED_LED 12
#define TRIGGER_PIN 9
#define ECHO_PIN 10
int restrictedZone = 100; // distance in centimeters

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  long duration, distance;
  
  // Clear the trigger
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Set the trigger high for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance < restrictedZone) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
  
  delay(500); // Wait for 1 second before next measurement
}

