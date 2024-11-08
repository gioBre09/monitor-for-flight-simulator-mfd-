#include <Joystick.h>

// Configurazione del multiplexer
const int SIG_PIN = 2;  // Pin collegato a SIG del multiplexer
const int S0_PIN = 4;   // Pin collegato a S0 del multiplexer
const int S1_PIN = 5;   // Pin collegato a S1 del multiplexer
const int S2_PIN = 6;   // Pin collegato a S2 del multiplexer
const int S3_PIN = 7;   // Pin collegato a S3 del multiplexer

// Canali del multiplexer
const int numChannels = 16;  // 16 canali totali

// Stato precedente dei pulsanti collegati al multiplexer
int lastButtonStateMux[numChannels] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

// Configurazione dei pulsanti collegati direttamente ad Arduino
const int directButtonPins[] = {3, 8, 9, 10};
const int numDirectButtons = 4;
int lastButtonStateDirect[numDirectButtons] = {HIGH, HIGH, HIGH, HIGH};

// Pin del potenziometro
const int potPin = A0;  // Collegato a A0

// Inizializzazione della libreria Joystick
Joystick_ Joystick;

void setup() {
  // Configurazione pin digitali come output per selezionare i canali del multiplexer
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  // Configurazione pin SIG come input per leggere il valore dal multiplexer
  pinMode(SIG_PIN, INPUT_PULLUP);

  // Configurazione dei pin dei pulsanti collegati direttamente
  for (int i = 0; i < numDirectButtons; i++) {
    pinMode(directButtonPins[i], INPUT_PULLUP);
  }

  // Inizializzazione Joystick
  Joystick.begin();
}

void loop() {
  // Gestione dei pulsanti collegati al multiplexer
  for (int i = 0; i < numChannels; i++) {
    // Seleziona il canale del multiplexer
    selectMuxChannel(i);

    // Legge lo stato del pulsante
    int buttonState = digitalRead(SIG_PIN);

    // Se il pulsante è stato premuto e non era premuto prima
    if (buttonState == LOW && lastButtonStateMux[i] == HIGH) {
      Joystick.pressButton(i);  // Simula la pressione del pulsante i
    } 
    // Se il pulsante è stato rilasciato e non era rilasciato prima
    else if (buttonState == HIGH && lastButtonStateMux[i] == LOW) {
      Joystick.releaseButton(i);  // Rilascia il pulsante i
    }

    // Aggiorna lo stato precedente del pulsante
    lastButtonStateMux[i] = buttonState;
  }

  // Gestione dei pulsanti collegati direttamente ai pin digitali di Arduino
  for (int i = 0; i < numDirectButtons; i++) {
    int buttonState = digitalRead(directButtonPins[i]);

    // Se il pulsante è stato premuto e non era premuto prima
    if (buttonState == LOW && lastButtonStateDirect[i] == HIGH) {
      Joystick.pressButton(numChannels + i);  // Simula la pressione del pulsante (numChannels + i)
    } 
    // Se il pulsante è stato rilasciato e non era rilasciato prima
    else if (buttonState == HIGH && lastButtonStateDirect[i] == LOW) {
      Joystick.releaseButton(numChannels + i);  // Rilascia il pulsante (numChannels + i)
    }

    // Aggiorna lo stato precedente del pulsante
    lastButtonStateDirect[i] = buttonState;
  }

  // Lettura del potenziometro
  int potValue = analogRead(potPin);  // Legge il valore del potenziometro (0-1023)
  int mappedPotValue = map(potValue, 0, 1023, 0, 255);  // Mappa il valore a 0-255 (intervallo di un asse del joystick)
  Joystick.setThrottle(mappedPotValue);  // Imposta il valore dell'asse del joystick (es. throttle)
}

// Funzione per selezionare il canale del multiplexer
void selectMuxChannel(int channel) {
  digitalWrite(S0_PIN, bitRead(channel, 0));
  digitalWrite(S1_PIN, bitRead(channel, 1));
  digitalWrite(S2_PIN, bitRead(channel, 2));
  digitalWrite(S3_PIN, bitRead(channel, 3));
}
