#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_JOGADORES 4
#define BUFFER_SIZE 1024
int cliente_sockets[MAX_JOGADORES];
#ifdef _WIN32
    #define strcasecmp _stricmp
#endif

// ================= CONSTANTES E DEFINIÇÕES =================

#define MAX_VIZINHOS 8
#define MAX_TERRITORIOS_CONTINENTE 16

#define CONT_AMERICA_SUL 0
#define CONT_AMERICA_NORTE 1
#define CONT_AFRICA 2
#define CONT_OCEANIA 3
#define CONT_ASIA 4
#define CONT_EUROPA 5
#define TOTAL_CONTINENTES 6

#define AS_BRASIL     0
#define AS_ARGENTINA  1
#define AS_CHILE      2
#define AS_COLOMBIA   3

#define AN_ALASKA 4
#define AN_VANCOUVER 5
#define AN_MACKENZIE 6
#define AN_OTTAWA 7
#define AN_LABRADOR 8
#define AN_GROELANDIA 9
#define AN_CALIFORNIA 10
#define AN_NOVA_YORK 11
#define AN_MEXICO 12

#define AF_NIGERIA 13
#define AF_EGITO 14
#define AF_SUDAO 15
#define AF_CONGO 16
#define AF_AFRICA_SUL 17
#define AF_MADAGASCAR 18

#define OC_SUMATRA 19
#define OC_BORNEO 20
#define OC_NOVA_GUINE 21
#define OC_AUSTRALIA 22

#define ASI_DUDINKA 23
#define ASI_SIBERIA 24
#define ASI_VLADVOSTOK 25
#define ASI_OMSK 26
#define ASI_TCHITA 27
#define ASI_ARAL 28
#define ASI_MONGOLIA 29
#define ASI_CHINA 30
#define ASI_JAPAO 31
#define ASI_ORIENTE_M 32
#define ASI_INDIA 33
#define ASI_VIETNA 34

#define UE_ISLANDIA 35
#define UE_INGLATERRA 36
#define UE_PORTUGAL 37
#define UE_SUECIA 38
#define UE_ALEMANHA 39
#define UE_POLONIA 40
#define UE_MOSCOU 41

#define TOTAL_TERRITORIOS 42

#define OBJ_1 0
#define OBJ_2 1
#define OBJ_3 2
#define OBJ_4 3
#define OBJ_5 4
#define OBJ_6 5
#define OBJ_7 6
#define OBJ_8 7
#define OBJ_9 8
#define OBJ_10 9
#define OBJ_11 10
#define OBJ_12 11

#define TOTAL_OBJETIVOS 12

void enviar_msg(int jogador_id, const char *msg);
void broadcast(const char *msg);
void receber_msg(int jogador_id, char *buffer);
void mostrar_meus_territorios_rede(int jogador_id);
void conquistar_territorio_rede(int vencedor_id, int id_origem, int id_destino);

// ================= STRUCTS (FORMATADAS) =================

typedef struct {
    int id;
    char nome[32];
    int continente_id;
    int dono;        
    int tropas;
    int vizinhos[MAX_VIZINHOS];
    int num_vizinhos;
    bool eh_bonus; 
} Territorio;

typedef struct {
    int id;
    char nome[32];
    int territorios[MAX_TERRITORIOS_CONTINENTE];
    int num_territorios;
    int bonus_completo;
} Continente;

typedef struct {
    int id;
    char texto[300];
    int dono;
    bool status_code;
    bool status_win;
} Objetivo;

// ================= VARIÁVEIS GLOBAIS DE ESTADO =================

int mao_cartas[MAX_JOGADORES] = {0, 0, 0, 0};
bool ganhou_bonus_nesta_rodada = false;

// ================= INICIALIZAÇÃO DE DADOS =================

Objetivo Objetivos[TOTAL_OBJETIVOS] = {
    {
        .id = OBJ_1,
        .texto = "Destruir os EXÉRCITOS BRANCOS",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_2,
        .texto = "Destruir os EXÉRCITOS AMARELOS",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_3,
        .texto = "Destruir os EXÉRCITOS PRETOS",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_4,
        .texto = "Destruir os EXÉRCITOS VERDES",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_5,
        .texto = "Conquistar AMERICA DO NORTE e OCEANIA",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_6,
        .texto = "Conquistar ASIA e AMERICA DO SUL",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_7,
        .texto = "Conquistar ASIA e AFRICA",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_8,
        .texto = "Conquistar AMÉRICA DO NORTE e AFRICA",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_9,
        .texto = "20 tropas no JAPÃO e NOVA IORQUE",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_10,
        .texto = "20 tropas na CHINA e BRASIL",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_11,
        .texto = "20 tropas na CHINA e NOVA IORQUE",
        .dono = -1,
        .status_code = false,
        .status_win = false
    },
    {
        .id = OBJ_12,
        .texto = "20 tropas na JAPÃO e BRASIL",
        .dono = -1,
        .status_code = false,
        .status_win = false
    }
};

Continente continentes[TOTAL_CONTINENTES] = {
    {
        .id = CONT_AMERICA_SUL,
        .nome = "America do Sul",
        .territorios = { AS_BRASIL, AS_ARGENTINA, AS_CHILE, AS_COLOMBIA },
        .num_territorios = 4,
        .bonus_completo = 3
    },
    {
        .id = CONT_AMERICA_NORTE,
        .nome = "America do Norte",
        .territorios = { AN_ALASKA, AN_VANCOUVER, AN_MACKENZIE, AN_OTTAWA, AN_LABRADOR, AN_GROELANDIA, AN_CALIFORNIA, AN_NOVA_YORK, AN_MEXICO },
        .num_territorios = 9,
        .bonus_completo = 7
    },
    {
        .id = CONT_AFRICA,
        .nome = "Africa",
        .territorios = { AF_NIGERIA, AF_EGITO, AF_SUDAO, AF_CONGO, AF_AFRICA_SUL, AF_MADAGASCAR },
        .num_territorios = 6,
        .bonus_completo = 4
    },
    {
        .id = CONT_OCEANIA,
        .nome = "Oceania",
        .territorios = { OC_SUMATRA, OC_BORNEO, OC_NOVA_GUINE, OC_AUSTRALIA },
        .num_territorios = 4,
        .bonus_completo = 3
    },
    {
        .id = CONT_ASIA,
        .nome = "Asia",
        .territorios = { ASI_DUDINKA, ASI_SIBERIA, ASI_VLADVOSTOK, ASI_OMSK, ASI_TCHITA, ASI_ARAL, ASI_MONGOLIA, ASI_CHINA, ASI_JAPAO, ASI_ORIENTE_M, ASI_INDIA, ASI_VIETNA },
        .num_territorios = 12,
        .bonus_completo = 10
    },
    {
        .id = CONT_EUROPA,
        .nome = "Europa",
        .territorios = { UE_ISLANDIA, UE_INGLATERRA, UE_PORTUGAL, UE_SUECIA, UE_ALEMANHA, UE_POLONIA, UE_MOSCOU },
        .num_territorios = 7,
        .bonus_completo = 7
    }
};

int bonus_parcial[TOTAL_CONTINENTES] = { 1, 5, 2, 1, 7, 5 };

Territorio territorios[TOTAL_TERRITORIOS] = {
    // AMERICA DO SUL
    { .id = AS_BRASIL, .nome = "Brasil", .continente_id = CONT_AMERICA_SUL, .dono = -1, .tropas = 0, .vizinhos = { AS_ARGENTINA, AS_COLOMBIA, AS_CHILE, AF_NIGERIA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = AS_ARGENTINA, .nome = "Argentina", .continente_id = CONT_AMERICA_SUL, .dono = -1, .tropas = 0, .vizinhos = { AS_BRASIL, AS_CHILE }, .num_vizinhos = 2, .eh_bonus = true },
    { .id = AS_CHILE, .nome = "Chile", .continente_id = CONT_AMERICA_SUL, .dono = -1, .tropas = 0, .vizinhos = { AS_ARGENTINA, AS_BRASIL, AS_COLOMBIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AS_COLOMBIA, .nome = "Colombia", .continente_id = CONT_AMERICA_SUL, .dono = -1, .tropas = 0, .vizinhos = { AS_BRASIL, AS_CHILE, AN_MEXICO }, .num_vizinhos = 3, .eh_bonus = true },
    // AMERICA DO NORTE
    { .id = AN_ALASKA, .nome = "Alaska", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_MACKENZIE, ASI_VLADVOSTOK }, .num_vizinhos = 2, .eh_bonus = true },
    { .id = AN_VANCOUVER, .nome = "Vancouver", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_MACKENZIE, AN_OTTAWA, AN_CALIFORNIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AN_MACKENZIE, .nome = "Mackenzie", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_ALASKA, AN_VANCOUVER, AN_OTTAWA, AN_GROELANDIA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = AN_OTTAWA, .nome = "Ottawa", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_LABRADOR, AN_MACKENZIE, AN_VANCOUVER, AN_CALIFORNIA, AN_NOVA_YORK }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = AN_LABRADOR, .nome = "Labrador", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_OTTAWA, AN_NOVA_YORK, AN_GROELANDIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AN_GROELANDIA, .nome = "Groelândia", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_MACKENZIE, AN_LABRADOR, UE_ISLANDIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AN_CALIFORNIA, .nome = "Califórnia", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_OTTAWA, AN_VANCOUVER, AN_MEXICO, AN_NOVA_YORK }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = AN_NOVA_YORK, .nome = "Nova Iorque", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_OTTAWA, AN_LABRADOR, AN_MEXICO, AN_CALIFORNIA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = AN_MEXICO, .nome = "México", .continente_id = CONT_AMERICA_NORTE, .dono = -1, .tropas = 0, .vizinhos = { AN_NOVA_YORK, AN_CALIFORNIA, AS_COLOMBIA }, .num_vizinhos = 3, .eh_bonus = true },
    // AFRICA
    { .id = AF_NIGERIA, .nome = "Nigeria", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_EGITO, AF_CONGO, AF_SUDAO, AS_BRASIL, UE_PORTUGAL }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = AF_EGITO, .nome = "Egito", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_NIGERIA, AF_SUDAO, ASI_ORIENTE_M, UE_POLONIA, UE_PORTUGAL }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = AF_SUDAO, .nome = "Sudão", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_EGITO, AF_CONGO, AF_AFRICA_SUL, AF_NIGERIA, AF_MADAGASCAR }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = AF_CONGO, .nome = "Congo", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_SUDAO, AF_AFRICA_SUL, AF_NIGERIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AF_AFRICA_SUL, .nome = "África do Sul", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_SUDAO, AF_CONGO, AF_MADAGASCAR }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = AF_MADAGASCAR, .nome = "Madagascar", .continente_id = CONT_AFRICA, .dono = -1, .tropas = 0, .vizinhos = { AF_AFRICA_SUL, AF_SUDAO }, .num_vizinhos = 2, .eh_bonus = true },
    // OCEANIA
    { .id = OC_SUMATRA, .nome = "Sumatra", .continente_id = CONT_OCEANIA, .dono = -1, .tropas = 0, .vizinhos = { OC_AUSTRALIA, ASI_INDIA, ASI_VIETNA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = OC_BORNEO, .nome = "Borneo", .continente_id = CONT_OCEANIA, .dono = -1, .tropas = 0, .vizinhos = { OC_AUSTRALIA, OC_NOVA_GUINE, ASI_VIETNA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = OC_NOVA_GUINE, .nome = "Nova Guiné", .continente_id = CONT_OCEANIA, .dono = -1, .tropas = 0, .vizinhos = { OC_AUSTRALIA, OC_BORNEO }, .num_vizinhos = 2, .eh_bonus = true },
    { .id = OC_AUSTRALIA, .nome = "Austrália", .continente_id = CONT_OCEANIA, .dono = -1, .tropas = 0, .vizinhos = { OC_SUMATRA, OC_BORNEO, OC_NOVA_GUINE }, .num_vizinhos = 3, .eh_bonus = true },
    // ASIA
    { .id = ASI_DUDINKA, .nome = "Dudinka", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_OMSK, ASI_MONGOLIA, ASI_TCHITA, ASI_SIBERIA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = ASI_SIBERIA, .nome = "Sibéria", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_DUDINKA, ASI_VLADVOSTOK, ASI_TCHITA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = ASI_VLADVOSTOK, .nome = "Vladvostok", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_CHINA, ASI_JAPAO, ASI_TCHITA, ASI_SIBERIA, AN_ALASKA }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_OMSK, .nome = "OmsK", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_MONGOLIA, ASI_CHINA, ASI_DUDINKA, ASI_ARAL, UE_MOSCOU }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_TCHITA, .nome = "Tchita", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_VLADVOSTOK, ASI_MONGOLIA, ASI_DUDINKA, ASI_SIBERIA, ASI_CHINA }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_ARAL, .nome = "Aral", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_OMSK, ASI_CHINA, ASI_INDIA, ASI_ORIENTE_M, UE_MOSCOU }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_MONGOLIA, .nome = "Mongólia", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_TCHITA, ASI_DUDINKA, ASI_OMSK, ASI_CHINA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = ASI_CHINA, .nome = "China", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_TCHITA, ASI_MONGOLIA, ASI_OMSK, ASI_ARAL, ASI_INDIA, ASI_VIETNA, ASI_JAPAO, ASI_VLADVOSTOK }, .num_vizinhos = 8, .eh_bonus = true },
    { .id = ASI_JAPAO, .nome = "Japão", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_CHINA, ASI_VLADVOSTOK, ASI_VIETNA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = ASI_ORIENTE_M, .nome = "Oriente Médio", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { AF_EGITO, ASI_INDIA, ASI_ARAL, UE_MOSCOU, UE_POLONIA }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_INDIA, .nome = "India", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_CHINA, ASI_ARAL, ASI_ORIENTE_M, ASI_VIETNA, OC_SUMATRA }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = ASI_VIETNA, .nome = "Vietnã", .continente_id = CONT_ASIA, .dono = -1, .tropas = 0, .vizinhos = { ASI_CHINA, ASI_INDIA, ASI_JAPAO, OC_SUMATRA, OC_BORNEO }, .num_vizinhos = 5, .eh_bonus = true },
    // EUROPA
    { .id = UE_ISLANDIA, .nome = "Islândia", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { UE_INGLATERRA, UE_SUECIA, AN_GROELANDIA }, .num_vizinhos = 3, .eh_bonus = true },
    { .id = UE_INGLATERRA, .nome = "Inglaterra", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { UE_PORTUGAL, UE_SUECIA, UE_ALEMANHA, UE_ISLANDIA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = UE_PORTUGAL, .nome = "Portugal", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { AF_EGITO, AF_NIGERIA, UE_ALEMANHA, UE_POLONIA, UE_INGLATERRA }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = UE_SUECIA, .nome = "Suécia", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { UE_INGLATERRA, UE_ISLANDIA, UE_ALEMANHA, UE_MOSCOU }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = UE_ALEMANHA, .nome = "Alemanha", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { UE_POLONIA, UE_PORTUGAL, UE_SUECIA, UE_INGLATERRA }, .num_vizinhos = 4, .eh_bonus = true },
    { .id = UE_POLONIA, .nome = "Polônia", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { UE_PORTUGAL, UE_ALEMANHA, ASI_ORIENTE_M, UE_MOSCOU, AF_EGITO }, .num_vizinhos = 5, .eh_bonus = true },
    { .id = UE_MOSCOU, .nome = "Moscou", .continente_id = CONT_EUROPA, .dono = -1, .tropas = 0, .vizinhos = { ASI_OMSK, ASI_ARAL, ASI_ORIENTE_M, UE_POLONIA, UE_SUECIA }, .num_vizinhos = 5, .eh_bonus = true }
};

// ================= FUNÇÕES DO JOGO =================

// Rola d6
int dado() {
    int valores[6] = {1, 2, 3, 4, 5, 6};
    int *p = valores;
    int indice = rand() % 6;
    return *(p + indice);
}

// Busca o ID de um território
int buscar_territorio_por_nome(char *nome) {
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (strcasecmp(territorios[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Distribui territórios: 1 fixo (dos 5 iniciais) + 1 aleatório
void distribui_cartas_inicio(int num_jogadores) {
    int paises_iniciais[] = {AN_NOVA_YORK, AS_BRASIL, ASI_CHINA, OC_AUSTRALIA, UE_ALEMANHA};
    bool base_usada[5] = {false, false, false, false, false};

    printf("\n[SISTEMA] Distribuindo territorios iniciais (2 tropas por regiao)...\n");

    for (int i = 0; i < num_jogadores; i++) {
        int r;
        do { r = rand() % 5; } while (base_usada[r]);
        base_usada[r] = true;
        int id_base = paises_iniciais[r];
        territorios[id_base].dono = i; 
        territorios[id_base].tropas = 2;
    }

    for (int i = 0; i < num_jogadores; i++) {
        int id_global;
        bool eh_principal;
        do {
            id_global = rand() % TOTAL_TERRITORIOS;
            eh_principal = false;
            for (int j = 0; j < 5; j++) {
                if (id_global == paises_iniciais[j]) { eh_principal = true; break; }
            }
        } while (territorios[id_global].dono != -1 || eh_principal); 

        territorios[id_global].dono = i;
        territorios[id_global].tropas = 2;
    }
}

// Verifica se o jogador domina todos os países de um continente
int jogador_controla_continente(int jogador_id, Continente *c) {
    for (int i = 0; i < c->num_territorios; i++) {
        int tid = c->territorios[i];
        if (territorios[tid].dono != jogador_id) {
            return 0;
        }
    }
    return 1;
}

bool sao_vizinhos(int id_origem, int id_destino) {
    for (int i = 0; i < territorios[id_origem].num_vizinhos; i++) {
        if (territorios[id_origem].vizinhos[i] == id_destino) return true;
    }
    return false;
}

// Finaliza a rodada com sorteio de 1 a 3 cartas
void finalizar_rodada(int jogador_id) {
    if (ganhou_bonus_nesta_rodada) {
        int espaco_na_mao = 5 - mao_cartas[jogador_id];
        if (espaco_na_mao > 0) {
            int sorteio = (rand() % 3) + 1;
            int a_receber = (sorteio > espaco_na_mao) ? espaco_na_mao : sorteio;

            mao_cartas[jogador_id] += a_receber;
            printf("\n[BÔNUS] Jogador %d ganhou %d cartas novas!\n", jogador_id, a_receber);
            printf("[MAO] Total de cartas: %d/5\n", mao_cartas[jogador_id]);
        }
    }
    ganhou_bonus_nesta_rodada = false;
}

// Ataque especial
void usar_bonus_aviao(int jogador_id, int id_alvo) {
    int avioes_restantes = 5;
    int tropas_abatidas = 0;
    int avioes_abatidos = 0;
    printf("\n--- ATAQUE AEREO contra %s ---\n", territorios[id_alvo].nome);

    while (avioes_restantes > 0) {
        int d_defesa = dado();
        if (d_defesa <= 2) {
            avioes_abatidos++;
            avioes_restantes--; 
            continue; 
        } else if (d_defesa == 3) {
            avioes_abatidos += 2;
            avioes_restantes -= 2;
            continue;
        }

        int d_ataque = dado();
        if (d_ataque <= 3) tropas_abatidas += d_ataque;
        avioes_restantes--;
    }
    territorios[id_alvo].tropas -= tropas_abatidas;
    if (territorios[id_alvo].tropas < 0) territorios[id_alvo].tropas = 0;
    printf("Tropas destruidas: %d | Avioes perdidos: %d\n", tropas_abatidas, (avioes_abatidos > 5 ? 5 : avioes_abatidos));
}

// Uso de cartas
void usar_carta_bonus(int jogador_id, int territorio_id, int tipo_uso) {
    if (mao_cartas[jogador_id] <= 0) {
        printf("Voce nao possui cartas!\n");
        return;
    }
    
    if (tipo_uso == 1 && territorios[territorio_id].dono == jogador_id) {
        territorios[territorio_id].tropas += 5;
        mao_cartas[jogador_id]--;
        printf("Reforco de 5 tropas aplicado em %s!\n", territorios[territorio_id].nome);
    } else if (tipo_uso == 2) {
        usar_bonus_aviao(jogador_id, territorio_id);
        mao_cartas[jogador_id]--;
    }
}

// Realiza a conquista e remaneja tropas
void conquistar_territorio_rede(int vencedor_id, int id_origem, int id_destino) {
    char out[BUFFER_SIZE], in[BUFFER_SIZE];
    
    sprintf(out, "\n--- VITORIA! %s conquistado a partir de %s! ---\n", 
            territorios[id_destino].nome, territorios[id_origem].nome);
    broadcast(out);
    
    int tropas_disponiveis = territorios[id_origem].tropas - 1;
    int qtd_enviar;

    if (tropas_disponiveis <= 1) {
        qtd_enviar = 1;
        enviar_msg(vencedor_id, "Movendo automaticamente 1 tropa para ocupar.\n");
    } else {
        sprintf(out, "Quantas tropas mover de %s (Max: %d)? ", territorios[id_origem].nome, tropas_disponiveis);
        enviar_msg(vencedor_id, out);
        receber_msg(vencedor_id, in);
        qtd_enviar = atoi(in); // Converte string recebida para int
        if (qtd_enviar < 1 || qtd_enviar > tropas_disponiveis) qtd_enviar = 1;
    }

    territorios[id_destino].dono = vencedor_id;
    territorios[id_destino].tropas = qtd_enviar;
    territorios[id_origem].tropas -= qtd_enviar;

    if (territorios[id_destino].eh_bonus) ganhou_bonus_nesta_rodada = true;
}


// Cálculo de tropas por turno
int calcular_tropas_recebidas(int jogador_id) {
    int total_territorios_jogador = 0;
    int bonus_total = 0;
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (territorios[i].dono == jogador_id) total_territorios_jogador++;
    }
    int bonus_base = total_territorios_jogador / 2;

    for (int i = 0; i < TOTAL_CONTINENTES; i++) {
        int contagem = 0;
        Continente *c = &continentes[i];
        for (int j = 0; j < c->num_territorios; j++) {
            if (territorios[c->territorios[j]].dono == jogador_id) contagem++;
        }
        float porcentagem = (float)contagem / c->num_territorios;
        if (contagem == c->num_territorios) bonus_total += c->bonus_completo;
        else if (porcentagem >= 0.70f) bonus_total += bonus_parcial[i];
    }
    return (bonus_base < 3 ? 3 : bonus_base) + bonus_total; // Mínimo de 3 tropas como no jogo real
}

int luta_tropas_rede(int jogador_atacado, int jogador_atual, int *d1, int *d2){
    *d1 = dado(); // Dado do Defensor
    *d2 = dado(); // Dado do Atacante
    
    if(*d2 > *d1) return jogador_atual;
    if(*d2 < *d1) return jogador_atacado;
    return 67; // Empate (Defesa ganha no War)
}

void ocupar_mapa_manual(int num_jogadores) {
    int jogador_da_vez = 0;
    int territorios_ocupados = 0;

    // Conta quantos já foram ocupados na distribuição inicial
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (territorios[i].dono != -1) territorios_ocupados++;
    }

    while (territorios_ocupados < TOTAL_TERRITORIOS) {
        printf("\n=== STATUS DO MAPA (%d/%d) ===\n", territorios_ocupados, TOTAL_TERRITORIOS);
        
        for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
            if (territorios[i].dono != -1) {
                printf("[X] %-15s (Dono: %d) | ", territorios[i].nome, territorios[i].dono);
            } else {
                printf("[ ] %-15s (LIVRE)    | ", territorios[i].nome);
            }
            if ((i + 1) % 3 == 0) printf("\n");
        }

        char entrada[32];
        printf("\n\nJOGADOR %d, escolha um territorio (ou digite 'debug' para auto-completar): ", jogador_da_vez);
        scanf(" %[^\n]s", entrada);

        // --- TRECHO DE DEBUG ---
        if (strcasecmp(entrada, "debug") == 0) {
            printf("\n[DEBUG] Distribuindo territorios restantes automaticamente...\n");
            for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
                if (territorios[i].dono == -1) {
                    territorios[i].dono = jogador_da_vez;
                    territorios[i].tropas = 1;
                    territorios_ocupados++;
                    jogador_da_vez = (jogador_da_vez + 1) % num_jogadores;
                }
            }
            break; // Sai do while de ocupação
        }

        
        // -----------------------

        int id = buscar_territorio_por_nome(entrada);

        if (id != -1 && territorios[id].dono == -1) {
            territorios[id].dono = jogador_da_vez;
            territorios[id].tropas = 1;
            territorios_ocupados++;
            jogador_da_vez = (jogador_da_vez + 1) % num_jogadores;
            printf("\n>> Sucesso!\n");
        } else {
            printf("\n>> ERRO: Nome invalido ou ja ocupado! Tente novamente.\n");
        }
    }
    printf("\n=== MAPA PRONTO PARA A GUERRA ===\n");
}
void enviar_msg(int jogador_id, const char *msg) {
    send(cliente_sockets[jogador_id], msg, strlen(msg), 0);
}

// Lista os territórios do jogador e seus vizinhos
void mostrar_meus_territorios_rede(int jogador_id) {
    char out[BUFFER_SIZE];
    enviar_msg(jogador_id, "\n=== SEUS TERRITORIOS ===\n");
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (territorios[i].dono == jogador_id) {
            sprintf(out, "[%s] Tropas: %d\n", territorios[i].nome, territorios[i].tropas);
            enviar_msg(jogador_id, out);
        }
    }
}

void broadcast(const char *msg) {
    for(int i = 0; i < MAX_JOGADORES; i++) {
        if(cliente_sockets[i] > 0) send(cliente_sockets[i], msg, strlen(msg), 0);
    }
}

void receber_msg(int jogador_id, char *buffer) {
    memset(buffer, 0, BUFFER_SIZE);
    int n = recv(cliente_sockets[jogador_id], buffer, BUFFER_SIZE, 0);
    if (n <= 0) { printf("Jogador %d desconectou.\n", jogador_id); exit(1); }
    buffer[strcspn(buffer, "\n")] = 0;
    buffer[strcspn(buffer, "\r")] = 0;
}

bool verificar_vitoria(int jogador_id) {
    int meu_obj = -1;
    // Encontra qual objetivo pertence a este jogador
    for(int i=0; i<TOTAL_OBJETIVOS; i++) {
        if(Objetivos[i].dono == jogador_id) { meu_obj = i; break; }
    }

    switch(meu_obj) {
        case OBJ_5: // América do Norte e Oceania
            return (jogador_controla_continente(jogador_id, &continentes[CONT_AMERICA_NORTE]) && 
                    jogador_controla_continente(jogador_id, &continentes[CONT_OCEANIA]));
        case OBJ_6: // Ásia e América do Sul
            return (jogador_controla_continente(jogador_id, &continentes[CONT_ASIA]) && 
                    jogador_controla_continente(jogador_id, &continentes[CONT_AMERICA_SUL]));
        case OBJ_7: // Ásia e África
            return (jogador_controla_continente(jogador_id, &continentes[CONT_ASIA]) && 
                    jogador_controla_continente(jogador_id, &continentes[CONT_AFRICA]));
        case OBJ_8: // América do Norte e África
            return (jogador_controla_continente(jogador_id, &continentes[CONT_AMERICA_NORTE]) && 
                    jogador_controla_continente(jogador_id, &continentes[CONT_AFRICA]));
        case OBJ_9: // 20 tropas no Japão e NY
            return (territorios[ASI_JAPAO].dono == jogador_id && territorios[ASI_JAPAO].tropas >= 20 &&
                    territorios[AN_NOVA_YORK].dono == jogador_id && territorios[AN_NOVA_YORK].tropas >= 20);
        case OBJ_10: // 20 tropas China e Brasil
            return (territorios[ASI_CHINA].dono == jogador_id && territorios[ASI_CHINA].tropas >= 20 &&
                    territorios[AS_BRASIL].dono == jogador_id && territorios[AS_BRASIL].tropas >= 20);
        case OBJ_11: // 20 tropas China e NY
            return (territorios[ASI_CHINA].dono == jogador_id && territorios[ASI_CHINA].tropas >= 20 &&
                    territorios[AN_NOVA_YORK].dono == jogador_id && territorios[AN_NOVA_YORK].tropas >= 20);
        case OBJ_12: // 20 tropas Japão e Brasil
            return (territorios[ASI_JAPAO].dono == jogador_id && territorios[ASI_JAPAO].tropas >= 20 &&
                    territorios[AS_BRASIL].dono == jogador_id && territorios[AS_BRASIL].tropas >= 20);
        default:
            // Para os objetivos de "Destruir exército", como não temos cores fixas, 
            // uma regra comum de fallback é conquistar 24 territórios.
            int count = 0;
            for(int i=0; i<TOTAL_TERRITORIOS; i++) if(territorios[i].dono == jogador_id) count++;
            return (count >= 24);
    }
}

// ================= MAIN =================
int *joga_1;
int *joga_2;

int main() {
    srand(time(NULL));
    int server_fd;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);
    char buffer_rede[BUFFER_SIZE];
    char msg_temp[BUFFER_SIZE];

    // Setup do Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erro no bind"); exit(EXIT_FAILURE);
    }
    listen(server_fd, MAX_JOGADORES);

    printf("[SERVIDOR] Aguardando %d jogadores...\n", MAX_JOGADORES);
    for (int i = 0; i < MAX_JOGADORES; i++) {
        cliente_sockets[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        enviar_msg(i, "Conectado! Aguardando inicio do jogo...\n");
        printf("Jogador %d conectado.\n", i);
    }

    // Inicialização do Estado
    // --- SORTEIO DE OBJETIVOS ---
    distribui_cartas_inicio(MAX_JOGADORES);
    bool obj_sorteado[TOTAL_OBJETIVOS] = {false};
    for (int i = 0; i < MAX_JOGADORES; i++) {
        int r;
        do { r = rand() % TOTAL_OBJETIVOS; } while (obj_sorteado[r]);
        obj_sorteado[r] = true;
        Objetivos[r].dono = i;
        
        sprintf(msg_temp, "\n[MISSÃO SECRETA] Seu objetivo é: %s\n", Objetivos[r].texto);
        enviar_msg(i, msg_temp);
    }

    int jogador_atual = 0;
    int d_def, d_atk;

    broadcast("\n--- O JOGO COMEÇOU! ---\n");

while(true) {
        sprintf(msg_temp, "\n>>> VEZ DO JOGADOR %d <<<\n", jogador_atual);
        broadcast(msg_temp);
        
        // --- 1. FASE DE REFORÇO ---
        int tropas_para_receber = calcular_tropas_recebidas(jogador_atual);
        sprintf(msg_temp, "[SISTEMA] Voce recebeu %d tropas de reforco!\n", tropas_para_receber);
        enviar_msg(jogador_atual, msg_temp);

        while (tropas_para_receber > 0) {
            mostrar_meus_territorios_rede(jogador_atual);
            sprintf(msg_temp, "Tropas restantes para posicionar: %d\nEscolha o territorio: ", tropas_para_receber);
            enviar_msg(jogador_atual, msg_temp);
            
            receber_msg(jogador_atual, buffer_rede);
            int id_alvo = buscar_territorio_por_nome(buffer_rede);

            if (id_alvo != -1 && territorios[id_alvo].dono == jogador_atual) {
                enviar_msg(jogador_atual, "Quantas tropas colocar aqui? ");
                receber_msg(jogador_atual, buffer_rede);
                int qtd = atoi(buffer_rede);

                if (qtd > 0 && qtd <= tropas_para_receber) {
                    territorios[id_alvo].tropas += qtd;
                    tropas_para_receber -= qtd;
                    enviar_msg(jogador_atual, "Tropas posicionadas!\n");
                } else {
                    enviar_msg(jogador_atual, "Quantidade invalida!\n");
                }
            } else {
                enviar_msg(jogador_atual, "Territorio invalido ou nao eh seu!\n");
            }
        }

        // --- 2. FASE DE ATAQUE ---
        bool atacando = true;
        while (atacando) {
            enviar_msg(jogador_atual, "\n--- ATAQUE --- Escolha ORIGEM (ou 'FIM'): ");
            receber_msg(jogador_atual, buffer_rede);

            if (strcasecmp(buffer_rede, "FIM") == 0) {
                atacando = false;
                break;
            }

            int id_origem = buscar_territorio_por_nome(buffer_rede);
            if (id_origem != -1 && territorios[id_origem].dono == jogador_atual && territorios[id_origem].tropas > 1) {
                enviar_msg(jogador_atual, "Escolha o ALVO: ");
                receber_msg(jogador_atual, buffer_rede);
                int id_alvo = buscar_territorio_por_nome(buffer_rede);

                if (id_alvo != -1 && territorios[id_alvo].dono != jogador_atual && sao_vizinhos(id_origem, id_alvo)) {
                    int resultado = luta_tropas_rede(territorios[id_alvo].dono, jogador_atual, &d_def, &d_atk);
                    sprintf(msg_temp, "\n[BATALHA] %s ataca %s! Atacante: %d | Defensor: %d\n", 
                            territorios[id_origem].nome, territorios[id_alvo].nome, d_atk, d_def);
                    broadcast(msg_temp);

                    if (resultado == jogador_atual) {
                        territorios[id_alvo].tropas--;
                        if (territorios[id_alvo].tropas <= 0) conquistar_territorio_rede(jogador_atual, id_origem, id_alvo);
                    } else {
                        territorios[id_origem].tropas--;
                        broadcast("Defesa venceu o round!\n");
                    }
                } else { enviar_msg(jogador_atual, "Alvo invalido ou nao vizinho!\n"); }
            } else { enviar_msg(jogador_atual, "Origem invalida ou pouca tropa!\n"); }
        }

        // --- 3. FASE DE FORTIFICAÇÃO (MOVIMENTAÇÃO) ---
        enviar_msg(jogador_atual, "\n--- FORTIFICACAO --- Deseja mover tropas entre seus territorios? (S/N): ");
        receber_msg(jogador_atual, buffer_rede);

        if (strcasecmp(buffer_rede, "S") == 0) {
            enviar_msg(jogador_atual, "Mover de (ORIGEM): ");
            receber_msg(jogador_atual, buffer_rede);
            int id_o = buscar_territorio_por_nome(buffer_rede);

            enviar_msg(jogador_atual, "Mover para (DESTINO): ");
            receber_msg(jogador_atual, buffer_rede);
            int id_d = buscar_territorio_por_nome(buffer_rede);

            if (id_o != -1 && id_d != -1 && territorios[id_o].dono == jogador_atual && 
                territorios[id_d].dono == jogador_atual && sao_vizinhos(id_o, id_d) && territorios[id_o].tropas > 1) {
                
                sprintf(msg_temp, "Quantas tropas mover (Max: %d)? ", territorios[id_o].tropas - 1);
                enviar_msg(jogador_atual, msg_temp);
                receber_msg(jogador_atual, buffer_rede);
                int qtd = atoi(buffer_rede);

                if (qtd > 0 && qtd < territorios[id_o].tropas) {
                    territorios[id_o].tropas -= qtd;
                    territorios[id_d].tropas += qtd;
                    enviar_msg(jogador_atual, "Tropas remanejadas!\n");
                }
            } else {
                enviar_msg(jogador_atual, "Movimento invalido! Verifique se sao vizinhos e se voce eh o dono.\n");
            }
        }

// --- 4. VERIFICAÇÃO DE VITÓRIA ---
        if (verificar_vitoria(jogador_atual)) {
            int obj_idx = -1;
            for(int k=0; k<TOTAL_OBJETIVOS; k++) if(Objetivos[k].dono == jogador_atual) obj_idx = k;
            
            sprintf(msg_temp, "\n==============================================\n"
                              "VITORIA! O JOGADOR %d VENCEU O JOGO!\n"
                              "OBJETIVO CONCLUIDO: %s\n"
                              "==============================================\n", 
                    jogador_atual, Objetivos[obj_idx].texto);
            broadcast(msg_temp);
            break; // Sai do loop e encerra o servidor
        }

        // --- 5. FINALIZAÇÃO ---
// --- 5. FINALIZAÇÃO ---
        finalizar_rodada(jogador_atual);
        
        // Adicione isso para o turno passar (ou resetar no 0 em caso de 1 jogador)
        jogador_atual = (jogador_atual + 1) % MAX_JOGADORES;
        sleep(1);
    }

    return 0;
}