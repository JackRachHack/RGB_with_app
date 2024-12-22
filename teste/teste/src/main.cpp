#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps
  // Outras configurações do setup...
}

void loop() {
  // Código principal do loop
  // Outras tarefas podem ser executadas aqui enquanto o Arduino aguarda dados
}

void serialEvent() {
  while (Serial.available()) {
    char dado = (char)Serial.read();
    // Processar o dado recebido
    Serial.print("Interrupção: Dado recebido - ");
    Serial.println(dado);
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}