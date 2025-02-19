# OpenGL Robô

## 🛠️ Compilação e Execução

Para compilar o programa, execute:

```bash
g++ main.cpp RgbImage.cpp -o main -lGL -lGLU -lglut
./main

🎮 Controles do Robô
🚶 Movimentos do Robô

    W: Avança o robô para frente na direção atual.
    S: Move o robô para trás na direção oposta.
    A: Gira o tronco do robô para a esquerda.
    D: Gira o tronco do robô para a direita.
    Q: Gira as pernas do robô para a esquerda.
    E: Gira as pernas do robô para a direita.

🤖 Controle dos Braços

    1: Aumenta o ângulo do braço esquerdo.
    2: Diminui o ângulo do braço esquerdo.
    3: Aumenta o ângulo do braço direito.
    4: Diminui o ângulo do braço direito.
    7: Flexiona o braço esquerdo para frente.
    8: Flexiona o braço esquerdo para trás.
    9: Flexiona o braço direito para frente.
    0: Flexiona o braço direito para trás.

🛠️ Outros Comandos

    5: Aumenta o ângulo de abertura da pinça no eixo Y.
    6: Diminui o ângulo de abertura da pinça no eixo Y.
    P: Alterna entre modos de perspectiva e ortogonal.
    ESC: Encerra o programa.

🖱️ Controles do Mouse

    Botão Esquerdo: Aproxima a câmera (Zoom-In).
    Botão Direito: Afasta a câmera (Zoom-Out).

🖱️ Movimento do Mouse

    Movimentos horizontais ajustam o azimute da câmera (gira ao redor do eixo vertical).

🎯 Controles das Setas Direcionais

    Seta para Cima: Inclina a cabeça do robô para cima (limite de 15 graus).
    Seta para Baixo: Inclina a cabeça do robô para baixo (limite de -15 graus).
    Seta para Esquerda: Gira a cabeça do robô para a esquerda.
    Seta para Direita: Gira a cabeça do robô para a direita.
