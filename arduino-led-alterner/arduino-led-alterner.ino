// A l'initialisation (début de programme)
void setup() {                
  // Donnée de la broche 2 de l'arduino (Broche 2 = Sortie numérique [Digital])
  pinMode(2, OUTPUT);    
}

// Réalisation en continue
void loop() {
  delay(1000);              // Pause de 1 seconde
  digitalWrite(2, HIGH);    // Allumer la LED
  delay(1000);              // Pause de 1 seconde
  digitalWrite(2, LOW);    // Eteindre la LED
}