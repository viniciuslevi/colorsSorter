#include <Servo.h>

#define pinoServo1 5 // Pinos dos servo-Motores
#define pinoServo2 6
#define S0 8       // Pino do módulo s0
#define S1 9       // Pino do módulo s1
#define S2 10      // Pino do módulo s2
#define S3 11      // Pino do módulo s3
#define out 12     // Pino do módulo out 
#define speed 1
#define LONG 1000 // Definindo intervalos de tempo
#define SHORT 100

struct Pair {
  int min;
  int max;
}; //Criando a estutura de pares
Pair matrix[4][3]; //Declarando a matriz de espectro de cores

  
int data = 0;      // Variável para armazenar os valores lidos
Servo servo1, servo2; // Declaração do tipo servo

const int initialServo2Position = 0; //Angulo inicial do servomotor 1
const int initialServo1Position = 108; //Angulo inicial do servomotor 2
const int sensorPosition = 57; //angulo onde se escontra o sensor de cores
  
int servo2Position; //Declaração de indices das posições dos servos
int servo1Position;

int counter=0; //Declaracao de variaveis utilizadas na ETAPA 2
int color;
int frequency;

void setup() {  

  //MATRIZ DE CLASSIFICAÇÃO DE CORES
  //matrix[X][Y].min, diz respeito ao parametro Y minimo que a cor X precisa ter. Enquanto matrix[X][Y].max, é o máximo.
  // 0:Vermelho, 1: Verde, 2: Azul
  //Por exemplo, "matrix[0][2].max = 34" diz que o 34 é o máximo do do parametro Blue que se precisa ter para ser considerado vermelho;

   //configuração cor vermelho
  matrix[0][0].min = 36; matrix[0][0].max = 46; //minimo e maximo do parametro Red
  matrix[0][1].min = 39; matrix[0][1].max = 50; //minimo e maximo do parametro Green
  matrix[0][2].min = 25; matrix[0][2].max = 34; //minimo e maximo do parametro blue

   //configuração cor verde
  matrix[1][0].min = 32; matrix[1][0].max = 38; //configuração cor verde
  matrix[1][1].min = 24; matrix[1][1].max = 31;
  matrix[1][2].min = 18; matrix[1][2].max = 21;

   //configuração cor azul
  matrix[2][0].min = 47; matrix[2][0].max = 53; //configuração cor azul
  matrix[2][1].min = 37; matrix[2][1].max = 45;
  matrix[2][2].min = 25; matrix[2][2].max = 30;

   //configuração cor branco
  matrix[3][0].min = 21; matrix[3][0].max = 31; // configuração cor branca
  matrix[3][1].min = 18; matrix[3][1].max = 28;
  matrix[3][2].min = 11; matrix[3][2].max = 21;

  servo1.attach(pinoServo1); // Associação do pino digital ao objeto do tipo servo
  servo1.write(initialServo1Position); // Inicia o motor na posição 0º
  delay(speed*250); 

  servo2.attach(pinoServo2); // Associação do pino digital ao objeto do tipo servo
  servo2.write(initialServo2Position); // Inicia o motor na posição 0º
  
  pinMode(S0, OUTPUT); // Define o modo dos pinos s0, s1, s2, s3 como saída
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(out, INPUT); // Define o modo do pino out como entrada
  //pinMode(4, INPUT);

  Serial.begin(9600); // Inicializa a comunicação serial com baud rate de 9600
  
  digitalWrite(S0, HIGH); // Configura a escala de frequência de saída do sensor para 100%
  digitalWrite(S1, HIGH);
}

void loop() {
  ETAPA1();
  int color = ETAPA2();
  ETAPA3(color);
  ETAPA4();
}

void ETAPA4() {
  for (int servo1Position = sensorPosition; servo1Position >= 0; servo1Position--) { // Para "pos" igual a posicao do sensor, enquanto "pos" maior que 0, decrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(speed*10); // Intervalo de 15 milissegundos
  }
  delay(speed*LONG); // Intervalo de 1 segundo, para que dê tempo da peça cair na rampa
  
  for (int servo1Position = 0; servo1Position <= initialServo1Position; servo1Position++) { // Vai de 0 graus para 108 graus, retornando para a posição inicial
    servo1.write(servo1Position); // Diz ao servo para ir para a posição na variável 'pos'
    delay(speed*10); // Espera 15ms para o servo alcançar a posição
  }
  delay(speed*SHORT);
}

void ETAPA3(int color) { // Função responsável por rotacionar a rampa
  while (servo2Position > 30 * color) {
    servo2.write(servo2Position);
    delay(speed*10);
    servo2Position--;
  }
  while (servo2Position < 30 * color) {
    servo2.write(servo2Position);
    delay(speed*10);
    servo2Position++;
  }
  delay(speed*SHORT);
}



int ETAPA2() {    
  // Configurando fotodiodos filtrados para vermelho
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Lendo a frequência de saída
  int red = frequency = pulseIn(out, LOW);
  // Imprimindo o valor no monitor serial
  Serial.print("R= ");
  Serial.print(frequency); // imprimindo frequência da cor vermelha
  Serial.print("  ");
  delay(speed*100);

  // Configurando fotodiodos filtrados para verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Lendo a frequência de saída
  int green = frequency = pulseIn(out, LOW);
  // Imprimindo o valor no monitor serial
  Serial.print("G= ");
  Serial.print(frequency); // imprimindo frequência da cor verde
  Serial.print("  ");
  delay(speed*100);

  // Configurando fotodiodos filtrados para azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Lendo a frequência de saída
  int blue = frequency = pulseIn(out, LOW);
  // Imprimindo o valor no monitor serial
  Serial.print("B= ");
  Serial.print(frequency); // imprimindo frequência da cor azul
  Serial.println("  ");
  delay(speed*100);

  // Verificando se a cor é vermelha
  if(red >= matrix[0][0].min && red <= matrix[0][0].max
    && green >= matrix[0][1].min && green <= matrix[0][1].max
    && blue >= matrix[0][2].min && blue <= matrix[0][2].max) {
      Serial.println("Red"); // Identificado como vermelho
      color = 1;
    }
  // Verificando se a cor é verde
  else if(red >= matrix[1][0].min && red <= matrix[1][0].max
    && green >= matrix[1][1].min && green <= matrix[1][1].max
    && blue >= matrix[1][2].min && blue <= matrix[1][2].max) {
      Serial.println("Green"); // Identificado como verde
      color = 3;
    }
  // Verificando se a cor é azul
  else if(red >= matrix[2][0].min && red <= matrix[2][0].max
    && green >= matrix[2][1].min && green <= matrix[2][1].max
    && blue >= matrix[2][2].min && blue <= matrix[2][2].max) {
      Serial.println("Blue"); // Identificado como azul
      color = 2;
    }
  // Verificando se a cor é branca
  else if(red >= matrix[3][0].min && red <= matrix[3][0].max
    && green >= matrix[3][1].min && green <= matrix[3][1].max
    && blue >= matrix[3][2].min && blue <= matrix[3][2].max) 
    {
      Serial.println("White"); // Identificado como branco
      color = 0; 
    }
  // Se a cor for desconhecida
  else {
      Serial.println("Desconhecida");
      if((counter++) >= 50) {
        counter = 0;
        // Movendo servo1 para posição inicial se não for identificada cor após 50 tentativas
        for (int servo1Position = sensorPosition; servo1Position <= initialServo1Position; servo1Position++) {
          servo1.write(servo1Position); // Movendo servo1
          delay(speed*10); // Espera para alcançar a posição
        }
        while(true); // Parando o programa
      }
      ETAPA2(); // Repetindo a leitura até encontrar a cor
    }
    return color;
}

void ETAPA1() {
  for (int servo1Position = initialServo1Position; servo1Position >= sensorPosition; servo1Position--) { // Para "pos" igual a posição inicial, enquanto "pos" menor que a posicao do sensor, decrementa "pos"
    servo1.write(servo1Position); // Escreve o valor da posição que o servo deve girar
    delay(speed*10); // Intervalo de 15 milissegundos
  }
  delay(speed*SHORT); // Intervalo de 100ms
}
