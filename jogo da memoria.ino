// Pinos dos LEDs
const int ledPins[] = {2, 3, 4, 5};
// Pinos dos botões
const int buttonPins[] = {6, 7, 8, 9};
// Pino do buzzer
const int buzzerPin = 10;

// Variáveis do jogo
int sequence[100];         // Sequência gerada
int playerInput[100];      // Sequência do jogador
int sequenceLength = 1;    // Comprimento da sequência atual
bool gameOver = false;

void setup() {
  // Configuração dos pinos dos LEDs e botões
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  // Configuração do pino do buzzer
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (!gameOver) {
    generateSequence();     // Gera a sequência
    playSequence();         // Mostra a sequência ao jogador
    if (!checkPlayerInput()) { // Verifica se o jogador acertou
      gameOver = true;
      showGameOver();       // Mostra "Game Over" em caso de erro
    } else {
      tone(buzzerPin, 1200, 300); // Som de progresso ao acertar
      delay(300);
      sequenceLength++;     // Aumenta o comprimento da sequência
    }
  }
}

// Função para gerar uma sequência aleatória
void generateSequence() {
  sequence[sequenceLength - 1] = random(0, 4); // Gera um número aleatório (0 a 3)
}

// Função para mostrar a sequência com LEDs e som
void playSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    int led = sequence[i];
    digitalWrite(ledPins[led], HIGH);       // Acende o LED
    tone(buzzerPin, 500 + led * 100, 500); // Emite som correspondente ao LED
    delay(500);                            // Aguarda enquanto o LED está aceso
    digitalWrite(ledPins[led], LOW);       // Apaga o LED
    delay(250);                            // Pausa entre os LEDs
  }
}

// Função para verificar a entrada do jogador
bool checkPlayerInput() {
  for (int i = 0; i < sequenceLength; i++) {
    bool buttonPressed = false;
    while (!buttonPressed) {
      for (int j = 0; j < 4; j++) {
        if (digitalRead(buttonPins[j]) == LOW) { // Botão pressionado
          buttonPressed = true;
          tone(buzzerPin, 1000, 100); // Som curto de confirmação
          playerInput[i] = j;
          if (playerInput[i] != sequence[i]) {
            return false; // Jogador errou
          }
          delay(250); // Evita múltiplas leituras
        }
      }
    }
  }
  return true;
}

// Função para mostrar o "Game Over"
void showGameOver() {
  for (int i = 0; i < 3; i++) { // Pisca todos os LEDs 3 vezes
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    tone(buzzerPin, 200, 200); // Emite som grave de erro
    delay(200);
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(200);
  }
  noTone(buzzerPin); // Para o som do buzzer
  sequenceLength = 1; // Reinicia o comprimento da sequência
  gameOver = false;   // Reinicia o jogo
}

