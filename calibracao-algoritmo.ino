// Este algoritmo é apenas para "calibragem" da matriz de classificação.
// antes de rodar, abra o serial monitor na porta 9600, adicione
// as cores vermelho, verde, azul e branco (nessas sequencia) e aguarde
// até o termino das leituras. 
// Minimize ao máximo interferencias na luz ambiente que incide sobre o sensor,
// como sombras, por exemplo. Evite mexer na estrutura enquanto as leituras são feitas.

#include <Servo.h>

#define pinoServo1 5 // Pinos dos servo-Motores
#define pinoServo2 6
#define S0 8       // Pino do módulo s0
#define S1 9       // Pino do módulo s1
#define S2 10      // Pino do módulo s2
#define S3 11      // Pino do módulo s3
#define out 12     // Pino do módulo out 

#define NUM_READINGS 50

Servo servo1, servo2;
const int initialServo1Position = 108;
const int initialServo2Position = 0;
const int sensorPosition = 57;
int servo1Position, servo2Position=0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(out, INPUT);

  Serial.begin(9600);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  Serial.println("Calibracao de Cores");
  
  servo1.attach(pinoServo1); // Associação do pino digital ao objeto do tipo servo
  servo1.write(initialServo1Position); // Inicia o motor na posição 0º
  
  servo1.attach(pinoServo2); // Associação do pino digital ao objeto do tipo servo
  servo1.write(initialServo2Position); // Inicia o motor na posição 0º
  delay(250); 
}

void loop() {
  toSensorPosition();
  calibrateColor("Red", LOW, LOW);
  dispensar(1);
  toSensorPosition();
  calibrateColor("Green", HIGH, HIGH);
  dispensar(3);
  toSensorPosition();
  calibrateColor("Blue", LOW, HIGH);
  dispensar(2);
  toSensorPosition();
  calibrateColor("White", HIGH, LOW);
  dispensar(0);
  while (true); // Parar após a calibração
}
void dispensar(int color){
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
  delay(200);

  for (int servo1Position = sensorPosition; servo1Position >= 0; servo1Position--) { // Para "pos" igual a posicao do sensor, enquanto "pos" maior que 0, decrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(15); // Intervalo de 15 milissegundos
  }
  delay(1000); // Intervalo de 1 segundo, para que dê tempo da peça cair na rampa
  
  for (int serv1Position = 0; servo1Position <= initialServo1Position; servo1Position++) { // Vai de 0 graus para 108 graus, retornando para a posição inicial
    servo1.write(servo1Position); // Diz ao servo para ir para a posição na variável 'pos'
    delay(15); // Espera 15ms para o servo alcançar a posição
  }
  delay(200);

}
void toSensorPosition() {
  for (int servo1Position = initialServo1Position; servo1Position > sensorPosition; servo1Position--) { // Para "pos" igual a posição inicial, enquanto "pos" menor que a posicao do sensor, decrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(15);
  }
  delay(200);
}
void toInicialPosition(){
  for (int servo1Position = 57; servo1Position <= initialServo1Position; servo1Position++) {
     servo1.write(servo1Position); // Movendo servo1
     delay(200); // Espera para alcançar a posição
  }
}

void calibrateColor(String colorName, int s2State, int s3State) {
  int redReadings[NUM_READINGS];
  int greenReadings[NUM_READINGS];
  int blueReadings[NUM_READINGS];

  // Capturar leituras
  for (int i = 0; i < NUM_READINGS; i++) {
    // Ler valor vermelho
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redReadings[i] = pulseIn(out, LOW);
    delay(100);

    // Ler valor verde
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    greenReadings[i] = pulseIn(out, LOW);
    delay(100);

    // Ler valor azul
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blueReadings[i] = pulseIn(out, LOW);
    delay(100);
  }

  // Calcular mínimos e máximos
  int redMin = redReadings[0], redMax = redReadings[0];
  int greenMin = greenReadings[0], greenMax = greenReadings[0];
  int blueMin = blueReadings[0], blueMax = blueReadings[0];

  for (int i = 1; i < NUM_READINGS; i++) {
    if (redReadings[i] < redMin) redMin = redReadings[i];
    if (redReadings[i] > redMax) redMax = redReadings[i];
    if (greenReadings[i] < greenMin) greenMin = greenReadings[i];
    if (greenReadings[i] > greenMax) greenMax = greenReadings[i];
    if (blueReadings[i] < blueMin) blueMin = blueReadings[i];
    if (blueReadings[i] > blueMax) blueMax = blueReadings[i];
  }

  // Imprimir resultados
  Serial.print(colorName);
  Serial.print(" - Red Min: ");
  Serial.print(redMin);
  Serial.print(", Red Max: ");
  Serial.print(redMax);
  Serial.print(", Green Min: ");
  Serial.print(greenMin);
  Serial.print(", Green Max: ");
  Serial.print(greenMax);
  Serial.print(", Blue Min: ");
  Serial.print(blueMin);
  Serial.print(", Blue Max: ");
  Serial.println(blueMax);
}
