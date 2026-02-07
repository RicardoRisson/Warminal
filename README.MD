# Warminal

Projeto de jogo baseado em turnos para terminal, escrito em C, que simula as mecânicas de dominação territorial do War. O mapa conta com 42 territórios e 6 continentes, utilizando uma estrutura de dados baseada em grafos para gerir fronteiras e bônus.

O projeto ainda está em fase inicial de desenvolvimento, tecnicamente jogavel, mas apenas sozinho ou com um amigo jogando no mesmo computador que você
podendo jogar o jogo quase que funcionalmente, as outras regras parecidas com o jogo de tabuleiro original serao adicionadas posteriormente.

## Objetivo
O projeto é pensado para hospedar em um servidor doméstico, permitindo partidas entre amigos via rede local ou SSH.

## Como rodar
Para compilar o projeto, é necessário apenas um compilador GCC padrão:

1. **Clone o repositório**
2. **No terminal, execute:**
```bash
gcc main.c -o war