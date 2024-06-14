# Projeto Arduino Color Sorter

Este projeto implementa um classificador de cores usando Arduino, um sensor de cor TCS3200/TCS230 e dois servos motores. Ele foi inspirado no projeto encontrado em [How To Mechatronics](https://howtomechatronics.com/projects/arduino-color-sorter-project/).

## Instalação

### Conexão de Hardware

1. Conecte o sensor de cor GY-31/TCS3200/TCS230 aos pinos digitais do Arduino conforme as definições dos pinos no código.
2. Conecte os servos motores aos pinos digitais definidos no código.
3. Certifique-se de que todas as conexões estejam seguras e corretas.

### Configuração de Software

1. Baixe e instale a IDE do Arduino se ainda não tiver.
2. Abra a IDE do Arduino e crie um novo esboço (sketch).
3. Copie e cole o código disponível no link: [GitHub Colors Sorter](https://github.com/viniciuslevi/colorsSorter).
4. Carregue o código no Arduino.

## Dependências

### Hardware Necessário

- Arduino Uno
- Sensor de cor TCS3200/TCS230
- 2 Servo motores
- Fonte de alimentação
- Cabos e jumpers

### Bibliotecas de Software

- Biblioteca Servo.h (inclusa na IDE do Arduino)

## Requisitos funcionais do software

O software do projeto deve ser capaz de:

1. Deslocar a peça para o leitor de cores, fazendo uso do servomotor 1.
2. Detectar a cor da peça utilizando o sensor de cor.
3. Posicionar a rampa de acordo com a cor identificada utilizando o servomotor 2.
4. Movimentar a peça para a rampa e retornar à posição inicial.

## Descrição do algoritmo

O algoritmo é dividido em 4 etapas (ETAPA 1, ETAPA 2, ETAPA 3 e ETAPA 4), cada etapa desempenha uma função específica em relação aos servomotores e ao sensor de cores.

### Setup

Configuramos as ações padrão como a configuração dos pinos, inicialização da comunicação serial, e configuração da frequência do sensor de leitura de cores. Definimos também os parâmetros de seleção de cores através de uma matriz de classificação de cores.

### Loop

O loop executa ininterruptamente as etapas, iniciando o próximo ciclo após o término do anterior.

### Etapas

#### ETAPA 1

- **Objetivo:** Posicionar o servo 1 para permitir a leitura do objeto.
- **Operação:** O servo 1 é girado de 108° a 57° em decrementos de 1°, com um atraso de 15ms entre cada incremento. Após atingir a posição final, há um atraso curto de 100ms.

#### ETAPA 2

- **Objetivo:** Detectar e identificar a cor do objeto.
- **Operação:** Leitura dos valores de intensidade de luz refletida para vermelho, azul e verde ajustando os pinos s2 e s3. A cor é determinada comparando os valores lidos com os intervalos definidos na matriz de classificação. A função retorna um valor que representa a cor detectada (0 para branco, 1 para vermelho, 2 para azul, 3 para verde).

#### ETAPA 3

- **Objetivo:** Posicionar o servo 2 para direcionar o objeto para o local correto de acordo com a cor.
- **Operação:** O servo 2 é movimentado para a posição específica baseada na cor detectada. Um pequeno atraso de 15ms é adicionado entre cada movimento para suavizar a rotação. Após atingir a posição final, há um curto atraso de 100ms.

#### ETAPA 4

- **Objetivo:** Completar o giro do servo 1 até a rampa e retornar à posição inicial.
- **Operação:** O servo 1 é girado de 57° a 0° em incrementos de 1°, com um atraso de 15ms entre cada incremento. Após atingir a posição final, há um atraso de 1s para permitir que o objeto se mova para a rampa. Em seguida, o servo 1 retorna de 0° a 108°.

## Descrição do Código

### Inclusões, Definições e Variáveis Globais

- **Bibliotecas:** Biblioteca Servo.h para controlar os servos motores.
- **Definições:** Pinos do sensor de cor e dos servos motores, intervalos de tempo (LONG e SHORT).
- **Variáveis:** Variáveis para armazenar os valores lidos do sensor e as posições dos servos.

### Funções

#### Função `ETAPA1()`

- Movimenta o servo 1 de 108° a 55°, com um atraso de 15ms entre cada decremento. Aguarda um curto intervalo de tempo após o movimento.

#### Função `ETAPA2()`

- Lê os valores dos fotodiodos do sensor de cor para vermelho, azul e verde. Identifica a cor comparando os valores lidos com a matriz de classificação. Em caso de cor desconhecida, persiste na leitura até 50 tentativas.

#### Função `ETAPA3(int color)`

- Movimenta o servo 2 para uma posição específica com base na cor identificada. Aguarda um curto intervalo de tempo após o movimento.

#### Função `ETAPA4()`

- Movimenta o servo 1 de 55° a 0°, aguarda 1s e retorna a 108° com um atraso de 15ms entre cada incremento. Aguarda um intervalo de tempo de 1s após o movimento para repetição do ciclo.
