#include <Servo.h>

#define pinoServo1 5 // Pinos dos servo-Motores
#define pinoServo2 6
#define s0 8       // Pino do módulo s0
#define s1 9       // Pino do módulo s1
#define s2 10      // Pino do módulo s2
#define s3 11      // Pino do módulo s3
#define out 12     // Pino do módulo out 

#define LONG 1000 // Definindo intervalos de tempo
#define SHORT 100

int data = 0;      // Variável para armazenar os valores lidos
Servo servo1, servo2; // Declaração do tipo servo

int servo2Position = 0;
int servo1Position = 0;

void setup() {
  servo1.attach(pinoServo1); // Associação do pino digital ao objeto do tipo servo
  servo1.write(0); // Inicia o motor na posição 0º
  
  servo2.attach(pinoServo2); // Associação do pino digital ao objeto do tipo servo
  servo2.write(0); // Inicia o motor na posição 0º
  
  pinMode(s0, OUTPUT); // Define o modo dos pinos s0, s1, s2, s3 como saída
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT); // Define o modo do pino out como entrada

  Serial.begin(9600); // Inicializa a comunicação serial com baud rate de 9600
  
  digitalWrite(s0, HIGH); // Configura a escala de frequência de saída do sensor para 100%
  digitalWrite(s1, HIGH);
}

void loop() {
  ETAPA1();
  int color = ETAPA2();
  ETAPA3(color);
  ETAPA4();
}

void ETAPA4() {
  for (int servo1Position = 60; servo1Position < 90; servo1Position++) { // Para "pos" igual a 60, enquanto "pos" menor que 180, incrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(15); // Intervalo de 15 milissegundos
  }
  delay(1000); // Intervalo de 1 segundo
  
  for (int servo1Position = 90; servo1Position >= 0; servo1Position--) { // Vai de 180 graus para 0 graus
    servo1.write(servo1Position); // Diz ao servo para ir para a posição na variável 'pos'
    delay(15); // Espera 15ms para o servo alcançar a posição
  }
  delay(SHORT);
}

void ETAPA3(int color) { // Função responsável por rotacionar a rampa
  while (servo2Position > 30 * color) {
    servo2.write(servo2Position);
    delay(15);
    servo2Position--;
  }
  while (servo2Position < 30 * color) {
    servo2.write(servo2Position);
    delay(15);
    servo2Position++;
  }
  delay(SHORT);
}

int ETAPA2() {    
  int Red, Blue, Green;
  int color = 0; // 1:vermelho; 2:azul, 3:verde
  
  // Leitura dos valores RGB
  digitalWrite(s2, LOW); // Seleciona os fotodiodos vermelhos
  digitalWrite(s3, LOW);
  Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); // Mede a duração do pulso LOW
  delay(20);  
   
  digitalWrite(s3, HIGH); // Seleciona os fotodiodos azuis
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
   
  digitalWrite(s2, HIGH); // Seleciona os fotodiodos verdes
  Green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  

  // Identificação da cor com base nos valores RGB lidos
  if (Red <= 15 && Green <= 15 && Blue <= 15) {
    Serial.println("White"); // Se todos os valores forem baixos, é provável que seja branco
    color = 0;
  } else if (Red < Blue && Red <= Green && Red < 23) {
    Serial.println("Red"); // Se o valor de vermelho for o mais baixo e menor que 23, é vermelho
    color = 1;
  } else if (Blue < Green && Blue < Red && Blue < 20) {
    Serial.println("Blue"); // Se o valor de azul for o mais baixo e menor que 20, é azul
    color = 2;
  } else if (Green < Red && (Green - Blue) <= 8) {
    Serial.println("Green"); // Se o verde for menor que o vermelho e a diferença entre verde e azul for aceitável, é verde
    color = 3;
  } else {
    Serial.println("Unknown"); // Se não for reconhecida, imprime "Unknown"
    ETAPA2(); // Enquanto não identificar a cor, continua a leitura até encontrar
  }
   
  return color;
}

void ETAPA1() {
  for (int servo1Position = 0; servo1Position < 60; servo1Position++) { // Para "pos" igual a 0, enquanto "pos" menor que 60, incrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(15); // Intervalo de 15 milissegundos
  }
  delay(SHORT); // Intervalo de 100ms
}
