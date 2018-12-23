int loopIndex = 0;
int const kRunningRabbitPins[] = {7, 8, 9, 10, 11, 12, 13};
int const kRabbitLights = sizeof(kRunningRabbitPins) / sizeof(kRunningRabbitPins[0]);
int const kDivisions = kRabbitLights * 2; // number of times
int const kCyclePeriodMs = 1000;                        // 2 second period before repeats
int const kDivisionPeriodMs = kCyclePeriodMs / kDivisions;    // period milliseconds for each evaluation
int const kRunningRabbitLightsButton = 1; // momentary push button push-to-make connected to ground
bool rabbitLightsEnabled = true;
int const kRunwayLightsPin = 4;      // Active high to N-Channel MOSFET gate (low side switch for parallel connected runway LEDs)
int const kRunwayLightsButton = 0;   // momentary push button push-to-make connected to ground
bool runwayLightsEnabled = true;
int const kTaxiLightsPin = 5;        // Active high to N-Channel MOSFET gate (low side switch for parallel connected taxi LEDs)
int const kTaxiLightsButton = 2;     // momentary push button push-to-make connected to ground
bool taxiLightsEnabled = true;
int const kBeaconLightPin = 6;       // PWM analogue write on pins 3, 5, 6, 9, 10, and 11
int const kBeaconLightButton = 3;    // momentary push button push-to-make connected to ground
bool beaconLightEnabled = true;

void setup() {
  // setup running rabbit outputs (active high)
  for (int index = 0; index < kRabbitLights; ++index) {
    pinMode(kRunningRabbitPins[index], OUTPUT);
    digitalWrite(kRunningRabbitPins[index], LOW);
  }
  pinMode(kRunningRabbitLightsButton, INPUT_PULLUP);

  // setup MOSFET output for runway lights
  pinMode(kRunwayLightsPin, OUTPUT);
  pinMode(kRunwayLightsButton, INPUT_PULLUP);

  // setup pulsing PWM output for control tower lights
  // not needed, analogWrite() doesn't require a pinMode to be called
  pinMode(kBeaconLightButton, INPUT_PULLUP);

  // setup MOSFET output for taxiway lights
  pinMode(kTaxiLightsPin, OUTPUT);
  pinMode(kTaxiLightsButton, INPUT_PULLUP);
}

void loop() {
  // Evalute buttons
  if (LOW == digitalRead(kRunwayLightsButton)) runwayLightsEnabled = !runwayLightsEnabled;
  if (LOW == digitalRead(kRunningRabbitLightsButton)) rabbitLightsEnabled = !rabbitLightsEnabled;
  if (LOW == digitalRead(kTaxiLightsButton)) taxiLightsEnabled = !taxiLightsEnabled;
  if (LOW == digitalRead(kBeaconLightButton)) beaconLightEnabled = !beaconLightEnabled;
  
  // Cycle through the digital outputs and evaluate
  digitalWrite(kRunwayLightsPin, runwayLightsEnabled);
  digitalWrite(kTaxiLightsPin, taxiLightsEnabled);

  // Cycle through the analogue outputs
  analogWrite(kBeaconLightPin, 
    map(
      sin(((float)loopIndex / (float)kDivisions) * 2.0 * PI) * 127, 
      -127, 
      127, 
      0, 
      255));

  // disable all running rabbit lights
  for (int index = 0; index < kRabbitLights; ++index) {
    pinMode(kRunningRabbitPins[index], OUTPUT);
    digitalWrite(kRunningRabbitPins[index], LOW);
  }
  // potentially enable the correct running rabbit light
  if (loopIndex < kDivisions / 2) {
    digitalWrite(kRunningRabbitPins[loopIndex], HIGH);
  }
  
  // See if it's time to cycle back again
  if (loopIndex++ > kDivisions) {
    loopIndex = 0;
  }

  // Sleep until ready for the next division
  delay(kDivisionPeriodMs);
}
