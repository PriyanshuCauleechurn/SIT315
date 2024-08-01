#define GREEN_LED 13
#define RED_LED 12
#define TRIGGER_PIN 9
#define ECHO_PIN 10
#define TEMP_SENSOR_PIN A1

int restrictedZone = 100; // distance in centimeters
volatile long duration, distance;
volatile float temperatureCelsius;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  
  Serial.begin(9600);

  // Initialize Timer0 for temperature sensor
  startTimer0(0.001); 
  
  // Initialize Timer1 for ultrasonic sensor
  startTimer1(20.0); 
}

void loop() {
  // Main loop can remain empty or handle other tasks
}

void startTimer0(double timerFrequency){
  noInterrupts();
  
  // Clear Timer0 control registers
  TCCR0A = 0;
  TCCR0B = 0;
  
  // Initialize counter value to 0
  TCNT0 = 0;   

  // Set compare match register for the desired timer frequency
  OCR0A = (16000000 / (1024 * timerFrequency)) - 1;
  
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR0A |= (1 << WGM01);

  // Set CS02 and CS00 bits for 1024 prescaler
  TCCR0B |= (1 << CS02) | (1 << CS00);

  // Enable Timer0 compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  
  interrupts();
}

void startTimer1(double timerFrequency){
  noInterrupts();
  
  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;
  
  // Initialize counter value to 0
  TCNT1 = 0;   

  // Set compare match register for the desired timer frequency
  OCR1A = (16000000 / (1024 * timerFrequency)) - 1;
  
  // Set CTC mode (Clear Timer on Compare Match)
  TCCR1B |= (1 << WGM12);

  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts();
}

ISR(TIMER0_COMPA_vect) {
  // Check temperature sensor
  int tempReading = analogRead(TEMP_SENSOR_PIN);
  float voltage = tempReading * (5.0 / 1024.0); // TMP36 voltage calculation
  temperatureCelsius = (voltage - 0.5) * 100;
  
  // Print temperature for debugging
  Serial.print("Temperature: ");
  Serial.println(temperatureCelsius);
}

ISR(TIMER1_COMPA_vect) {
  // Check ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  // Read the echo pin
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;
  
  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.println(distance);

  // Control the LEDs based on the conditions
  if (distance < restrictedZone || temperatureCelsius < 20.0) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }
}
