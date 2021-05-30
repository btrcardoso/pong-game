# pong-game

Pong-Game feito em C utilizando a biblioteca SDL2.
Este jogo foi criado seguindo [este Tutorial](https://www.youtube.com/watch?v=B4BXpiQASpA).

![Screenshot from 2021-05-30 00-37-53](https://user-images.githubusercontent.com/72050839/120091237-a72e4700-c0df-11eb-9f88-bf07b6bce33b.png)

## Como Jogar 

Tecla BACKSPACE para iniciar a partida.

Teclas W e S para mover o JOGADOR VERMELHO.

Teclas UP e DOWN para mover o JOGADOR AZUL.

## Instalação

Para jogar, baixe os arquivos deste repositório e abra o terminal Linux na pasta onde os arquivos se encontram.

Faça a instalação do gcc:
```bash
sudo apt install gcc
```

Faça a intalação do SDL2:
```bash
sudo apt-get install libsdl2-dev
```

Crie um arquivo executável para o programa:
```bash
gcc pong.c -o pong -lSDL2
```

Sempre que for executar o programa, digite no terminal onde os arquivos se encontram o seguinte comando:
```bash
./pong
```
