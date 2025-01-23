# Jogo de Reflexos para Matriz de LEDs e OLED

Este projeto consiste em um jogo simples de reflexos e coordenação motora desenvolvido para sistemas embarcados. Ele foi criado como parte dos estudos durante o curso de sistemas embarcados do **EmbarcaTech**, utilizando a plataforma Raspberry Pi Pico W, em conjunto com uma matriz de LEDs RGB 5x5 e um display OLED.

## Objetivo do Jogo

O objetivo é pressionar o botão correto (A ou B) de acordo com a direção exibida na matriz de LEDs o mais rápido possível, enquanto um temporizador registra o tempo total sem erros. Caso um botão incorreto seja pressionado, o jogo para e exibe o tempo decorrido alternando com uma mensagem final. É possível reiniciar o jogo pressionando ambos os botões simultaneamente.

## Funcionalidades

- Exibição de contagem regressiva inicial na matriz de LEDs.
- Direções (setas) exibidas na matriz para indicar qual botão pressionar.
- Timer absoluto exibido no display OLED com 1 casa decimal de precisão.
- Feedback visual com a matriz de LEDs sendo apagada enquanto os botões são pressionados.
- Indicação de erro com o padrão "X" na matriz de LEDs e a pausa do timer.
- Alternância de mensagens no display OLED após um erro, incluindo o tempo final sem erros.
- Reinício imediato do jogo ao pressionar os botões A e B simultaneamente.

## Ferramentas e Tecnologias Utilizadas

- **Microcontrolador**: Raspberry Pi Pico W.
- **Linguagem de Programação**: C.
- **Periféricos**:
  - Matriz de LEDs RGB 5x5 para exibição de padrões.
  - Display OLED para mensagens e timer.
  - Botões de entrada (GPIOs).
- **Bibliotecas**:
  - `pico/stdlib` para controle dos GPIOs e temporização.
  - Biblioteca personalizada para controle da matriz de LEDs.
  - Biblioteca para exibição de mensagens no display OLED.

## Como Funciona o Jogo

1. Ao iniciar, o jogo exibe uma contagem regressiva de 3 segundos na matriz de LEDs.
2. Uma seta (← ou →) é exibida, indicando qual botão pressionar.
3. O jogador deve pressionar o botão correspondente antes que a direção mude.
4. O tempo é registrado e exibido no display OLED.
5. Caso um botão incorreto seja pressionado, o jogo pausa e exibe o tempo decorrido até o erro.
6. O jogador pode reiniciar pressionando os botões A e B simultaneamente.

## Aprendizados e Desafios

Durante o desenvolvimento deste projeto, foram explorados conceitos fundamentais de sistemas embarcados, como:

- Configuração e uso de GPIOs para leitura de botões e controle de periféricos.
- Implementação de debouncing de botões via software.
- Controle de uma matriz de LEDs RGB e exibição de padrões dinâmicos.
- Manipulação de tempos absolutos e diferenciais para o funcionamento do timer.
- Integração de periféricos de saída para feedback visual ao jogador.

## Como Executar o Projeto

1. Configure um Raspberry Pi Pico W com as bibliotecas necessárias para controle de matriz de LEDs e display OLED.
2. Conecte os periféricos (matriz de LEDs, display OLED e botões) conforme o layout do projeto.
3. Compile o código utilizando o SDK do Raspberry Pi Pico.
4. Envie o binário para o microcontrolador.
5. Divirta-se jogando e treinando seus reflexos!

## Requisitos de Hardware

- 1x Raspberry Pi Pico W.
- 1x Matriz de LEDs RGB 5x5.
- 1x Display OLED.
- 2x Botões para entrada.
- Resistores de pull-up (se necessário).

## Possíveis Melhorias Futuras

- Adição de modos de dificuldade com tempos limites para respostas.
- Implementação de sons para feedback auditivo.
- Inclusão de análise de desempenho com pontuação baseada no tempo de reação.
- Integração com um aplicativo para registro de pontuações via Bluetooth.

## Licença

Este projeto foi desenvolvido para fins educacionais e pessoais. Sinta-se à vontade para explorar, modificar e utilizar como base para seus próprios projetos!
