# 🎖️ Warminal

**Warminal** é um motor de jogo de estratégia em turnos para terminal escrito em C. Ele simula as mecânicas clássicas de dominação territorial (inspirado em War/Risk) utilizando uma arquitetura **Cliente-Servidor** via Sockets TCP.

O jogo suporta até 4 jogadores simultâneos, possui sistema de missões secretas, cartas de bônus e ataques aéreos.

## 🚀 Funcionalidades Implementadas
* **Multiplayer em Rede:** Servidor robusto que gerencia até 4 conexões simultâneas.
* **Mapa Global:** 42 territórios divididos em 6 continentes com bônus de tropas.
* **Objetivos Secretos:** 12 missões diferentes sorteadas no início da partida.
* **Mecânicas de Combate:** Rola de dados (D6) sincronizada entre todos os jogadores.
* **Ataques Especiais:** Uso de cartas para reforços extras ou ataques de avião.

## 🛠️ Requisitos
* **Compilador:** GCC (padrão em sistemas Linux/Arch).
* **Ferramentas de Cliente:** `telnet` ou `nc` (Netcat).
  * No Arch Linux: `sudo pacman -S inetutils gnu-netcat`.

## 💻 Como Rodar

### 1. Iniciar o Servidor
Compile o código e execute o binário para abrir a porta de escuta (`12345`):
```bash
gcc main.c -o servidor_war
./servidor_war
```
### 2. Conectar os Jogadores

Cada jogador deve abrir um terminal e conectar-se ao IP do servidor (ou localhost se estiver na mesma máquina):
```bash
telnet localhost 12345
ou
nc localhost 12345
```