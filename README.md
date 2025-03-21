# Jogo de Reflexos para Matriz de LEDs e OLED

Este projeto consiste em um jogo simples de reflexos e coordenação motora desenvolvido para sistemas embarcados. Ele foi criado como parte dos estudos durante o curso de sistemas embarcados do **EmbarcaTech**, utilizando a plataforma Raspberry Pi Pico W, em conjunto com uma matriz de LEDs RGB 5x5 e um display OLED e um buzzer para feedback sonoro.

## Objetivo do Jogo

O objetivo é pressionar o botão correto (A ou B) de acordo com a direção exibida na matriz de LEDs o mais rápido possível, enquanto um temporizador registra o tempo total sem erros. Caso um botão incorreto seja pressionado, o jogo para e exibe o tempo decorrido alternando com uma mensagem final. É possível reiniciar o jogo pressionando ambos os botões simultaneamente.

## Funcionalidades
1. **Tela Inicial:**
   - Alterna entre setas na matriz de LEDs enquanto exibe uma mensagem no display OLED.
   - O jogo só inicia após o usuário pressionar os botões A e B simultaneamente.

2. **Fluxo Principal do Jogo:**
   - Após a tela inicial, o jogo começa com uma contagem regressiva exibida na matriz de LEDs e acompanhada por um som de alerta.
   - O jogador deve pressionar o botão correspondente à direção da seta exibida (esquerda ou direita) dentro de um limite de tempo.

3. **Controle de Tempo:**
   - O tempo disponível para reação começa em 1,5 segundos e é reduzido em 100 milissegundos a cada 5 segundos, até um limite mínimo de 0,5 segundos.
   - Se o jogador pressionar o botão incorreto ou não reagir dentro do tempo, um "X" vermelho é exibido na matriz de LEDs, acompanhado por um som de erro, indicando erro.

4. **Feedback Visual e Sonoro:**
   - O display OLED exibe mensagens como "Prepare-se!", o tempo decorrido e instruções para reiniciar o jogo após um erro.
   - A matriz de LEDs fornece feedback visual claro durante todo o jogo.
   - A contagem regressiva agora é acompanhada por sons para aumentar a imersão do jogador.
   - Um som de erro é emitido sempre que o jogador cometer um erro.

5. **Reinicialização Automática:**
   - Se o jogo permanecer ocioso por mais de 10 segundos na tela de "Game Over", ele retorna automaticamente para a animação inicial, incentivando novas tentativas.

## Ferramentas e Tecnologias Utilizadas

- **Microcontrolador**: Raspberry Pi Pico W.
- **Linguagem de Programação**: C.
- **Periféricos**:
  - Matriz de LEDs RGB 5x5 para exibição de padrões.
  - Display OLED para mensagens e timer.
  - Buzzer para sons de feedback.
  - Botões de entrada (GPIOs).
- **Bibliotecas**:
  - **SDK do Raspberry Pi Pico**: Para acesso às funções de GPIO, temporização e outras funcionalidades da placa.
- **Bibliotecas Personalizadas**:
  - `matrix_control.h`: Controle da matriz de LEDs RGB.
  - `matrices.h`: Padrões e formas exibidas na matriz (setas, números, "X", etc.).
  - `oled_display.h`: Controle do display OLED.
  - `buzzer.h`: Controle do buzzer para sons do jogo.
  - `sounds.h`: Definição dos sons utilizados para feedback sonoro.
- **CMake**: Para configuração e build do projeto.

## Como Jogar

1. Conecte o hardware conforme especificado.
2. Compile e carregue o código na Raspberry Pi Pico W.
3. Após a inicialização, a matriz exibirá setas alternadas e o display mostrará uma mensagem.
4. Pressione os botões A e B simultaneamente para iniciar o jogo.
5. Uma contagem regressiva sonora e visual será exibida antes do jogo começar.
6. Pressione o botão correto (A para esquerda ou B para direita) dentro do tempo limite indicado.
7. Se errar ou demorar demais, um som de erro será emitido e a tela de "Game Over" será exibida.
8. O jogo retornará automaticamente para a animação inicial após 10 segundos de inatividade na tela de "Game Over".

## Aprendizados e Desafios

Durante o desenvolvimento deste projeto, foram explorados conceitos fundamentais de sistemas embarcados, como:

- Programação de sistemas embarcados em C.
- Configuração e uso de GPIOs para leitura de botões e controle de periféricos.
- Implementação de debouncing de botões via software.
- Controle de uma matriz de LEDs RGB e exibição de padrões dinâmicos.
- Manipulação de tempos absolutos e diferenciais para o funcionamento do timer.
- Integração de periféricos de saída para feedback visual e sonoro ao jogador.
- Implementação de controle de áudio via buzzer para sons customizados.
- Gerenciamento de estado do jogo para permitir reinicialização automática.

## Como Executar o Projeto

1. Configure um Raspberry Pi Pico W com as bibliotecas necessárias para controle de matriz de LEDs, display OLED e buzzer.
2. Conecte os periféricos (matriz de LEDs, display OLED, buzzer e botões) conforme o layout do projeto.
3. Compile o código utilizando o SDK do Raspberry Pi Pico.
4. Envie o binário para o microcontrolador.
5. Divirta-se jogando e treinando seus reflexos!

## Requisitos de Hardware

- 1x Raspberry Pi Pico W.
- 1x Matriz de LEDs RGB 5x5.
- 1x Display OLED.
- 1x Buzzer.
- 2x Botões para entrada.
- Resistores de pull-up (se necessário).

## Possíveis Melhorias Futuras

- Adição de modos de dificuldade com tempos limites para respostas.
- Implementação de novos efeitos sonoros para diferentes eventos do jogo.
- Inclusão de análise de desempenho com pontuação baseada no tempo de reação.
- Integração com um aplicativo para registro de pontuações via Bluetooth.

## Autor
Desenvolvido por [Rafhael Gaspar da silva](https://github.com/Rafhael0069).

## Licença

Este projeto foi desenvolvido para fins educacionais e pessoais. Sinta-se à vontade para explorar, modificar e utilizar como base para seus próprios projetos!