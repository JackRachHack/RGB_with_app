#include <Arduino.h>

// Definindo os pinos dos LEDs RGB
#define PIN_RED    9  // GPIO9
#define PIN_GREEN  10 // GPIO10
#define PIN_BLUE   11 // GPIO11

// Definindo os pinos dos potenciômetros
#define NOB_RED    A0 // GPIOA0
#define NOB_GREEN  A1 // GPIOA1

// Variáveis de pinos do hardware
int nobPins[2] = {NOB_RED, NOB_GREEN}; // Pinos dos potenciômetros
int ledPins[3] = {PIN_RED, PIN_GREEN, PIN_BLUE}; // Pinos dos LEDs

// Variáveis globais para armazenar valores
int rgbValues[3] = {0, 0, 0};
int nobValues[2] = {0, 0};
int sliderValues[3] = {0, 0, 0};

// Variável para escolher entre slider e nob
bool tog[2] = {false, false};
 
// Declaração de funções
void updateValuesSlider(int, int);
void updateValuesNob();
void updateValuesRGB();
void sendData(int, int);



void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps

  // Configurando os LEDs como saídas
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Configurando os potenciômetros como entradas
  for (int i = 0; i < 2; i++) {
    pinMode(nobPins[i], INPUT);
  }

  pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {
  delay(50);
  // Código principal do loop
  updateValuesNob();
  updateValuesRGB();
  Serial.println(rgbValues[0]);
  Serial.println(rgbValues[1]);
  Serial.println(rgbValues[2]);
  
}

void serialEvent() {
  static byte data[2];
  static int index = 0;
  static bool receiving = false;

  while (Serial.available()) {
    byte receivedByte = Serial.read();
    
    if (receivedByte == 'i') {  // Caractere 'i' encontrado
      receiving = true;         // Inicia a recepção dos dados
      index = 0;                // Reseta o índice
    } else if (receiving) {
      data[index] = receivedByte;
      index++;

      if (index == 2) { // Quando dois bytes são lidos, processar os dados
        // Armazenando valores lidos
        updateValuesSlider (data[0], data[1]);

        // Serial.print("Interrupção: Dados recebidos - ");
        // Serial.println(sliderValues[0]);
        // Serial.println(sliderValues[1]);
        // Serial.println(sliderValues[2]);

        // Reseta para a próxima leitura
        receiving = false;
      }
    }
  }
}

// Function to deal with receiving data
void updateValuesSlider(int identifier, int data) {

   // Depending on what is received, one global variable is updated
  switch (identifier) {
    case 0: // R data
      sliderValues[0] = data; // Storing slider value sent
      tog[0] = true; // Activating the use of the R slider
      break;

    case 1: // G data
      sliderValues[1] = data; // Storing slider value sent
      tog[1] = true; // Activating the use of the G slider
      break;

    case 2: // B data
      sliderValues[2] = data; // Storing slider value sent
      break;
   
    default:
      break;

  }
}

int oldValueRed = 0;
int oldValueGreen = 0;

// Função para lidar com os valores dos potenciômetros
void updateValuesNob() {
  // Definindo a tolerância para a mudança significativa
  const int gap = 6;
  
  // Variáveis para identificar movimento real do pot
  int newValueRed = 0;
  int newValueGreen = 0;

  
  newValueRed = map(analogRead(NOB_RED), 0, 1023, 0, 255);    // Valores iniciais de pot para considerar uma mudança
  newValueGreen = map(analogRead(NOB_GREEN), 0, 1023, 0, 255); // no futuro
  
  // Verifica a mudança nos valores e realiza a ação baseada nisso
  if (abs(newValueRed - oldValueRed) > gap) {
    tog[0] = false; // Ativa ação se houver mudança significativa no potenciômetro vermelho
    oldValueRed = newValueRed;
  }

  if (abs(newValueGreen - oldValueGreen) > gap) {
    tog[1] = false; // Ativa ação se houver mudança significativa no potenciômetro verde
    oldValueGreen = newValueGreen;
  }

  nobValues[0] = newValueRed;
  nobValues[1] = newValueGreen;
}


void updateValuesRGB(){
  // Atualizando valores dos LEDs no RGB
  for (int i = 0; i < 2; i++){
    // Operador ternário para atualizar o valor do led de acordo com a var tog
    rgbValues[i] = (tog[i]) ? sliderValues[i] : nobValues[i];
    if (!tog[i]) sendData(i, rgbValues[i]);
    analogWrite(ledPins[i], rgbValues[i]);
  }
  // Operador ternário para atualizar o valor do led de acordo com a var tog
  rgbValues[2] = sliderValues[2];
  analogWrite(ledPins[2], rgbValues[2]);
}

// Envia uma sequência de dados com o idenficador e o valor
void sendData(int identifier, int data){
  Serial.write('i');
  Serial.write(identifier);
  Serial.write(data);  
}
