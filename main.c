#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #define strcasecmp _stricmp
#endif

// ================= CONSTANTES E DEFINIÇÕES =================

#define MAX_VIZINHOS 8
#define MAX_TERRITORIOS_CONTINENTE 16
#define MAX_JOGADORES 4

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

// Lista os territórios do jogador e seus vizinhos
void mostrar_meus_territorios(int jogador_id) {
    printf("\n=== SEUS TERRITORIOS E FRONTEIRAS (Jogador %d) ===\n", jogador_id);
    bool tem_territorio = false;
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        if (territorios[i].dono == jogador_id) {
            tem_territorio = true;
            printf("[%d] %s (Tropas: %d) | Vizinhos: ", i, territorios[i].nome, territorios[i].tropas);
            for (int j = 0; j < territorios[i].num_vizinhos; j++) {
                int v_id = territorios[i].vizinhos[j];
                printf("%s (Dono: %d)%s", territorios[v_id].nome, territorios[v_id].dono, (j < territorios[i].num_vizinhos - 1) ? ", " : "");
            }
            printf("\n");
        }
    }
    if (!tem_territorio) printf("Voce nao possui territorios no momento.\n");
}

bool sao_vizinhos(int id_origem, int id_destino) {
    for (int i = 0; i < territorios[id_origem].num_vizinhos; i++) {
        if (territorios[id_origem].vizinhos[i] == id_destino) return true;
    }
    return false;
}

// Realiza a conquista e remaneja tropas
void conquistar_territorio(int vencedor_id, int id_origem, int id_destino) {
    printf("\n--- VITORIA! %s foi conquistado a partir de %s! ---\n", 
            territorios[id_destino].nome, territorios[id_origem].nome);
    
    int tropas_disponiveis = territorios[id_origem].tropas - 1;
    int qtd_enviar;

    if (tropas_disponiveis <= 1) {
        qtd_enviar = 1;
        printf("Movendo automaticamente 1 tropa para ocupar.\n");
    } else {
        do {
            printf("Quantas tropas enviar de %s para %s? (Disponivel: %d): ", 
                    territorios[id_origem].nome, territorios[id_destino].nome, tropas_disponiveis);
            scanf("%d", &qtd_enviar);
        } while (qtd_enviar < 1 || qtd_enviar > tropas_disponiveis);
    }

    territorios[id_destino].dono = vencedor_id;
    territorios[id_destino].tropas = qtd_enviar;
    territorios[id_origem].tropas -= qtd_enviar;

    if (territorios[id_destino].eh_bonus) ganhou_bonus_nesta_rodada = true;
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

int luta_tropas(int jogador_atacado, int jogador_atual, int *joga_1, int *joga_2){
    int x = 0;
    int y = 0;
    int empate = 67;
    for(int i = 0; i < 2; i++){
       if(i == 0){
       x = dado();        
       *joga_1 = x;
    }
       else{
       y = dado();        
       *joga_2 = y;
       }
    }

    if(x > y){
        return jogador_atacado;
    }else if(x < y){
        return jogador_atual;
    }else{
        return empate;
    }
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

// ================= MAIN =================
int *joga_1;
int *joga_2;

int main() {
    srand(time(NULL));
    int jogador_atual = 0;
    int num_jogadores = 4;
    char alvo_nome[32];
    int d1, d2; // Dados da luta_tropas

    // Estado dos territórios
    for(int i=0; i<TOTAL_TERRITORIOS; i++) {
        territorios[i].dono = -1;
        territorios[i].tropas = 0;
    }
    
    distribui_cartas_inicio(num_jogadores);
    ocupar_mapa_manual(num_jogadores);
    while(true) {
        printf("\n\n========================================");
        printf("\n   TURNO DO JOGADOR %d", jogador_atual);
        printf("\n========================================");
        mostrar_meus_territorios(jogador_atual);
        // 1. FASE DE REFORÇO
        int novas_tropas = calcular_tropas_recebidas(jogador_atual);
        printf("\n[SISTEMA] Voce recebeu %d tropas de reforco.", novas_tropas);
        
        while(novas_tropas > 0) {
            char destino_reforco[32];
            printf("\nOnde colocar reforcos? (%d restantes): ", novas_tropas);
            scanf(" %[^\n]s", destino_reforco);
            int id_ref = buscar_territorio_por_nome(destino_reforco);
            
            if(id_ref != -1 && territorios[id_ref].dono == jogador_atual) {
                int qtd;
                printf("Quantas tropas em %s? ", territorios[id_ref].nome);
                scanf("%d", &qtd);
                if(qtd > 0 && qtd <= novas_tropas) {
                    territorios[id_ref].tropas += qtd;
                    novas_tropas -= qtd;
                } else {
                    printf("Quantidade invalida!");
                }
            } else {
                printf("Territorio invalido ou nao e seu!");
            }
        }

        // 2. FASE DE CARTAS
        if(mao_cartas[jogador_atual] > 0) {
            char usar;
            printf("\nVoce tem %d cartas. Deseja usar uma? (s/n): ", mao_cartas[jogador_atual]);
            scanf(" %c", &usar);
            if(usar == 's') {
                char t_nome[32];
                int t_tipo;
                printf("Nome do territorio para o bonus: ");
                scanf(" %[^\n]s", t_nome);
                int id_t = buscar_territorio_por_nome(t_nome);
                printf("Tipo (1: Reforco +5, 2: Aviao): ");
                scanf("%d", &t_tipo);
                if(id_t != -1) usar_carta_bonus(jogador_atual, id_t, t_tipo);
            }
        }

        // 3. FASE DE ATAQUE
        while(true) {
            mostrar_meus_territorios(jogador_atual);
            printf("\nComando: [NOME DO PAIS] para atacar ou 'FIM' para passar: ");
            scanf(" %[^\n]s", alvo_nome);

            if (strcasecmp(alvo_nome, "FIM") == 0) {
                finalizar_rodada(jogador_atual); 
                jogador_atual = (jogador_atual + 1) % num_jogadores; // Troca o turno
                break; 
            }

            int id_alvo = buscar_territorio_por_nome(alvo_nome);
            if (id_alvo == -1 || territorios[id_alvo].dono == jogador_atual) {
                printf("Alvo invalido ou ja e seu!\n");
                continue;
            }

            // --- LÓGICA PARA IDENTIFICAR ID_ORIGEM ---
            int id_origem = -1;
            int vizinhos_aptos[MAX_VIZINHOS];
            int cont = 0;

            for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
                if (territorios[i].dono == jogador_atual && sao_vizinhos(i, id_alvo)) {
                    if (territorios[i].tropas > 1) {
                        vizinhos_aptos[cont++] = i;
                    }
                }
            }

            if (cont == 0) {
                printf(">> Erro: Sem vizinhos com tropas suficientes.\n");
                continue;
            }

            if (cont == 1) {
                id_origem = vizinhos_aptos[0];
            } else {
                printf("Escolha a origem do ataque:\n");
                for (int i = 0; i < cont; i++) {
                    printf("[%d] %s (%d tropas)\n", vizinhos_aptos[i], territorios[vizinhos_aptos[i]].nome, territorios[vizinhos_aptos[i]].tropas);
                }
                printf("Digite o ID da origem: ");
                scanf("%d", &id_origem);
            }

            // --- EXECUÇÃO DO COMBATE ---
            // Usando sua função luta_tropas
            int resultado = luta_tropas(territorios[id_alvo].dono, jogador_atual, &d1, &d2);
            
            printf("\n--- RESULTADO DOS DADOS ---");
            printf("\nAtacante (Jogador %d): [%d]", jogador_atual, d2);
            printf("\nDefensor (Jogador %d): [%d]", territorios[id_alvo].dono, d1);

            if(resultado == jogador_atual) {
                printf("\n>> Vitoria no dado! Defesa perde 1 tropa.");
                territorios[id_alvo].tropas--;
                if(territorios[id_alvo].tropas <= 0) {
                    conquistar_territorio(jogador_atual, id_origem, id_alvo);
                }
            } else if(resultado == territorios[id_alvo].dono) {
                printf("\n>> Derrota no dado! Ataque perde 1 tropa.");
                territorios[id_origem].tropas--;
            } else {
                printf("\n>> Empate! Defesa ganha (Ataque perde 1 tropa).");
                territorios[id_origem].tropas--;
            }
        }
    }
    return 0;
}