# colorsSorter
O Arduino Color Sorter é um projeto que utiliza um sensor de cores (TCS3200/TCS230) para detectar e classificar objetos com base em sua cor. O projeto envolve o uso de servos para direcionar os objetos detectados para diferentes locais, dependendo da cor identificada.

## Instalação

### Conexão de Hardware:
1. Conecte o sensor de cor GY-31/TCS3200/TCS230 aos pinos digitais do Arduino conforme as definições dos pinos no código.
2. Conecte os servos motores aos pinos digitais definidos no código.
3. Certifique-se de que todas as conexões estejam seguras e corretas.

### Configuração de Software:
1. Baixe e instale a IDE do Arduino se ainda não tiver.
2. Abra a IDE do Arduino e crie um novo esboço (sketch).
3. Copie e cole o código disponível no link: [colorsSorter](colorsSorter.ino)
4. Carregue o código no Arduino.

## Dependências

### Hardware Necessário:
- Arduino Uno
- Sensor de cor TCS3200/TCS230
- 2 Servo motores
- Fonte de alimentação
- Cabos e jumpers

### Bibliotecas de Software:
- Biblioteca Servo.h (inclusa na IDE do Arduino)

## Requisitos funcionais do software
O software do projeto deve ser capaz de deslocar a peça para o leitor de cores fazendo uso do servomotor 1. Este deve girar a 60° deslocando a peça para o sensor fazer a leitura da cor e aguarda o comando para completar os 30° restantes (são 90° desde o ponto inicial até a rampa). De acordo com a cor identificada pelo sensor o servomotor 2 responsável pela rotação da rampa deve girar para a respectiva posição. Após isso o servomotor 1 deve receber o comando para completar os 30° restantes jogando a peça na rampa. O servomotor 1 deve aguardar um certo instante de tempo para que a peça caia na rampa e então finalmente deve retornar para a posição inicial girando 90° no sentido inverso.

## Descrição do algoritmo
A ideia principal é dividir o algoritmo em 4 etapas (ETAPA 1, ETAPA 2, ETAPA 3 e ETAPA 4). Cada etapa é responsável por desempenhar uma função em relação aos servo-motores e ao sensor de cores. O sistema opera com as 4 etapas agindo de maneira sequencial e ininterrupta.

### ETAPA 1: 
**Objetivo:** Posicionar o primeiro servo (servo1) para permitir a leitura do objeto a ser classificado.

**Operação:** 
- O servo1 é girado de 0° a 60° em incrementos de 1° com um pequeno atraso de 15 milissegundos entre cada incremento para suavizar o movimento.
- Após atingir a posição final, há um atraso curto de 100 milissegundos para o início da segunda etapa.

### ETAPA 2: 
**Objetivo:** Detectar e identificar a cor do objeto utilizando o sensor de cor GY-31.

**Operação:**
- Os valores de intensidade de luz refletida são lidos para as cores vermelha, azul e verde ajustando os pinos s2 e s3 para selecionar os diferentes fotodiodos do sensor.
- Com base nos valores lidos, o código determina se a cor é branca, vermelha, azul, verde ou desconhecida. A cor é determinada comparando os valores lidos contra limites predefinidos. No caso de cor desconhecida, o sensor é obrigado a continuar a leitura até que se identifique alguma das cores listadas.
- A função retorna um valor que representa a cor detectada (0 para branco, 1 para vermelho, 2 para azul, 3 para verde).

### ETAPA 3:
**Objetivo:** Posicionar o segundo servo (servo2) para direcionar o objeto detectado para o local correto de acordo com a cor identificada.

**Operação:**
- O servo2 é movimentado para uma posição específica baseada na cor detectada (multiplicador de 30° para cada cor).
- Se a posição atual do servo2 for maior que a desejada, ele será decrementado até atingir a posição correta. Se for menor, será incrementado até atingir a posição correta. Esse método é adotado para que a rampa somente se movimente quando necessário, isto é, quando houver a identificação de alguma cor. Caso contrário, permanece na posição onde se encontra.
- Um pequeno atraso de 15 milissegundos é adicionado entre cada movimento para suavizar a rotação do servo. Após atingir a posição final, há um curto atraso de 100 milissegundos para o início da ETAPA 4.

### ETAPA 4: 
**Objetivo:** Completar o giro do primeiro servo (servo1) até a rampa e então retornar à posição inicial após a queda do objeto.

**Operação:**
- O servo1 é girado de 60° a 90° em incrementos de 1° com um pequeno atraso de 15 milissegundos entre cada incremento.
- Após atingir a posição final (90°), há um atraso de 1 segundo para permitir que o objeto tenha tempo de se mover completamente para a rampa.
- Em seguida, o servo1 é retornado de 90° a 0° em decrementos de 1°, novamente com um pequeno atraso de 15 milissegundos entre cada decremento, voltando para a posição inicial.
- Após retornar à posição inicial, há um curto atraso de 100 milissegundos para a repetição do próximo ciclo com a ETAPA 1.

## Descrição do Código

### Inclusões, Definições e Variáveis Globais:
- Incluímos a biblioteca Servo.h para controlar os servos motores.
- Definimos os pinos do sensor de cor e dos servos motores.
- Definimos intervalos de tempo (LONG e SHORT) para uso no código.
- `int data`: Armazena os valores lidos do sensor.
- `Servo servo1, servo2`: Objetos do tipo Servo para controlar os servos motores.
- `int servo2Position, servo1Position`: Variáveis para armazenar as posições dos servos.

### Setup:
- Associamos os pinos dos servos motores aos objetos servo1 e servo2 e inicializamos na posição 0°.
- Configuramos os pinos do sensor de cor como saídas ou entradas.
- Inicializamos a comunicação serial.
- Configuramos a escala de frequência de saída do sensor para 100%.

### Loop:
- Chamamos as funções ETAPA1, ETAPA2, ETAPA3 e ETAPA4 em sequência para realizar as operações de detecção e movimentação.

### Função ETAPA1():
- Movimenta o servo1 de 0° a 60° com um atraso de 15ms entre cada incremento de 1°.
- Aguarda um curto intervalo de tempo (SHORT) após o movimento.

### Função ETAPA2():
- Lê os valores dos fotodiodos do sensor de cor para vermelho, azul e verde.
- Identifica a cor com base nos valores lidos e retorna um código de cor (0 para branco, 1 para vermelho, 2 para azul, 3 para verde).

### Função ETAPA3(int color):
- Movimenta o servo2 para uma posição específica com base no código de cor retornado pela ETAPA2.
- Aguarda um curto intervalo de tempo (SHORT) após o movimento.

### Função ETAPA4():
- Movimenta o servo1 de 60° a 90° e depois retorna a 0° com um atraso de 15ms entre cada incremento ou decremento de 1°.
- Aguarda um intervalo de tempo de 1 segundo após o movimento para repetição do ciclo.
