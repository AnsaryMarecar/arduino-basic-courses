float temperature_celcius = 0.0;
int value_brut = 0;


void setup(){
  // On démarre la voie série pour déboguer
  Serial.begin(9600);
}

void loop(){

  // Read pin A0
  value_brut = analogRead(A0);
  
  // Transform value
  temperature_celcius = value_brut * (5.0 / 1023.0 * 100.0);
  // Seep 250ms
  delay(25000);
  client.stop();
  Serial.println(temperature_celcius);
}