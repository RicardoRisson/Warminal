#include <stdio.h>

#define MAX_VIZINHOS 8
#define MAX_TERRITORIOS_CONTINENTE 16
#define MAX_JOGADORES 4

// ================= CONTINENTES =================
#define CONT_AMERICA_SUL 0
#define CONT_AMERICA_NORTE 1
#define CONT_AFRICA 2
#define CONT_OCEANIA 3
#define CONT_ASIA 4
#define CONT_EUROPA 5

#define TOTAL_CONTINENTES 6

// ================= TERRITORIOS (PAISES) =================
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

// ================= STRUCTS =================

typedef struct {
    int id;
    char nome[32];

    int continente_id;
    int dono;        // -1 = ninguém
    int tropas;

    int vizinhos[MAX_VIZINHOS];
    int num_vizinhos;
} Territorio;

typedef struct {
    int id;
    char nome[32];

    int territorios[MAX_TERRITORIOS_CONTINENTE];
    int num_territorios;

    int bonus_completo;
} Continente;

// ================= DADOS FIXOS DO MAPA =================

Continente continentes[TOTAL_CONTINENTES] = {
    {
        .id = CONT_AMERICA_SUL,
        .nome = "America do Sul",
        .territorios = { AS_BRASIL, AS_ARGENTINA, AS_CHILE, AS_COLOMBIA },
        .num_territorios = 4,
        .bonus_completo = 2
    },
    {
        .id = CONT_AMERICA_NORTE,
        .nome = "America do Norte",
        .territorios = { AN_ALASKA, AN_VANCOUVER, AN_MACKENZIE, AN_OTTAWA, AN_LABRADOR, AN_GROELANDIA, AN_CALIFORNIA, AN_NOVA_YORK, AN_MEXICO },
        .num_territorios = 9,
        .bonus_completo = 5
    },
    {
        .id = CONT_AFRICA,
        .nome = "Africa",
        .territorios = { AF_NIGERIA, AF_EGITO, AF_SUDAO, AF_CONGO, AF_AFRICA_SUL, AF_MADAGASCAR },
        .num_territorios = 6,
        .bonus_completo = 3
    },
    {
        .id = CONT_OCEANIA,
        .nome = "Oceania",
        .territorios = { OC_SUMATRA, OC_BORNEO, OC_NOVA_GUINE, OC_AUSTRALIA },
        .num_territorios = 4,
        .bonus_completo = 2
    },
    {
        .id = CONT_ASIA,
        .nome = "Asia",
        .territorios = { ASI_DUDINKA, ASI_SIBERIA, ASI_VLADVOSTOK, ASI_OMSK, ASI_TCHITA, ASI_ARAL, ASI_MONGOLIA, ASI_CHINA, ASI_JAPAO, ASI_ORIENTE_M, ASI_INDIA, ASI_VIETNA },
        .num_territorios = 12,
        .bonus_completo = 7
    },
    {
        .id = CONT_EUROPA,
        .nome = "Europa",
        .territorios = { UE_ISLANDIA, UE_INGLATERRA, UE_PORTUGAL, UE_SUECIA, UE_ALEMANHA, UE_POLONIA, UE_MOSCOU },
        .num_territorios = 7,
        .bonus_completo = 5
    }
};

Territorio territorios[TOTAL_TERRITORIOS] = {
    {
        .id = AS_BRASIL,
        .nome = "Brasil",
        .continente_id = CONT_AMERICA_SUL,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AS_ARGENTINA, AS_COLOMBIA, AS_CHILE, AF_NIGERIA },
        .num_vizinhos = 4
    },
    {
        .id = AS_ARGENTINA,
        .nome = "Argentina",
        .continente_id = CONT_AMERICA_SUL,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AS_BRASIL, AS_CHILE },
        .num_vizinhos = 2
    },
    {
        .id = AS_CHILE,
        .nome = "Chile",
        .continente_id = CONT_AMERICA_SUL,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AS_ARGENTINA, AS_BRASIL, AS_COLOMBIA },
        .num_vizinhos = 3
    },
    {
        .id = AS_COLOMBIA,
        .nome = "Colombia",
        .continente_id = CONT_AMERICA_SUL,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AS_BRASIL, AS_CHILE, AN_MEXICO },
        .num_vizinhos = 3
    },
    {
        .id = AN_ALASKA,
        .nome = "Alaska",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AN_MACKENZIE, ASI_VLADVOSTOK },
        .num_vizinhos = 2
    },
    {
        .id = AN_VANCOUVER,
        .nome = "Vancouver",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AN_MACKENZIE, AN_OTTAWA, AN_CALIFORNIA  },
        .num_vizinhos = 3
    },
    {
        .id = AN_MACKENZIE,
        .nome = "Mackenzie",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AN_ALASKA, AN_VANCOUVER, AN_OTTAWA, AN_GROELANDIA  },
        .num_vizinhos = 4
    },
    {
        .id = AN_OTTAWA,
        .nome = "Ottawa",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AN_LABRADOR, AN_MACKENZIE, AN_VANCOUVER, AN_CALIFORNIA, AN_NOVA_YORK  },
        .num_vizinhos = 5
    },
    {
        .id = AN_LABRADOR,
        .nome = "Labrador",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AN_OTTAWA, AN_NOVA_YORK, AN_GROELANDIA  },
        .num_vizinhos = 3
    },
    {
        .id = AN_GROELANDIA,
        .nome = "Groelândia",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AN_MACKENZIE, AN_LABRADOR, UE_ISLANDIA},
        .num_vizinhos = 3
    },
    {
        .id = AN_CALIFORNIA,
        .nome = "Califórnia",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AN_OTTAWA, AN_VANCOUVER, AN_MEXICO, AN_NOVA_YORK},
        .num_vizinhos = 4
    },
    {
        .id = AN_NOVA_YORK,
        .nome = "Nova Iorque",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AN_OTTAWA, AN_LABRADOR, AN_MEXICO, AN_CALIFORNIA},
        .num_vizinhos = 4
    },
    {
        .id = AN_MEXICO,
        .nome = "México",
        .continente_id = CONT_AMERICA_NORTE,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AN_NOVA_YORK, AN_CALIFORNIA, AS_COLOMBIA},
        .num_vizinhos = 3
    },
    {
        .id = AF_NIGERIA,
        .nome = "Nigeria",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AF_EGITO, AF_CONGO, AF_SUDAO, AS_BRASIL, UE_PORTUGAL},
        .num_vizinhos = 5
    },
    {
        .id = AF_EGITO,
        .nome = "Egito",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AF_NIGERIA, AF_SUDAO, ASI_ORIENTE_M, UE_POLONIA, UE_PORTUGAL},
        .num_vizinhos = 5
    },
    {
        .id = AF_SUDAO,
        .nome = "Sudão",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AF_EGITO, AF_CONGO, AF_AFRICA_SUL, AF_NIGERIA, AF_MADAGASCAR},
        .num_vizinhos = 5
    },
    {
        .id = AF_CONGO,
        .nome = "Congo",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AF_SUDAO, AF_AFRICA_SUL, AF_NIGERIA},
        .num_vizinhos = 3
    },
    {
        .id = AF_AFRICA_SUL,
        .nome = "África do Sul",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AF_SUDAO, AF_CONGO, AF_MADAGASCAR },
        .num_vizinhos = 3
    },
    {
        .id = AF_MADAGASCAR,
        .nome = "Madagascar",
        .continente_id = CONT_AFRICA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = {AF_AFRICA_SUL, AF_SUDAO},
        .num_vizinhos = 2
    },
    {
        .id = OC_AUSTRALIA,
        .nome = "Austrália",
        .continente_id = CONT_OCEANIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { OC_SUMATRA, OC_BORNEO, OC_NOVA_GUINE},
        .num_vizinhos = 3
    },
    {
        .id = OC_BORNEO,
        .nome = "Borneo",
        .continente_id = CONT_OCEANIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { OC_AUSTRALIA, OC_NOVA_GUINE, ASI_VIETNA},
        .num_vizinhos = 3
    },
    {
        .id = OC_NOVA_GUINE,
        .nome = "Nova Guiné",
        .continente_id = CONT_OCEANIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { OC_AUSTRALIA, OC_BORNEO},
        .num_vizinhos = 2
    },
    {
        .id = OC_SUMATRA,
        .nome = "Sumatra",
        .continente_id = CONT_OCEANIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { OC_AUSTRALIA, ASI_INDIA, ASI_VIETNA},
        .num_vizinhos = 3
    },
    {
        .id = ASI_ARAL,
        .nome = "Aral",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_OMSK, ASI_CHINA, ASI_INDIA, ASI_ORIENTE_M, UE_MOSCOU },
        .num_vizinhos = 5
    },
    {
        .id = ASI_ORIENTE_M,
        .nome = "Oriente Médio",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AF_EGITO, ASI_INDIA, ASI_ARAL, UE_MOSCOU, UE_POLONIA},
        .num_vizinhos = 5
    },
    {
        .id = ASI_OMSK,
        .nome = "OmsK",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_MONGOLIA, ASI_CHINA, ASI_DUDINKA, ASI_ARAL, UE_MOSCOU},
        .num_vizinhos = 5
    },
    {
        .id = ASI_DUDINKA,
        .nome = "Dudinka",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_OMSK, ASI_MONGOLIA, ASI_TCHITA, ASI_SIBERIA},
        .num_vizinhos = 4
    },
    {
        .id = ASI_SIBERIA,
        .nome = "Sibéria",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_DUDINKA, ASI_VLADVOSTOK, ASI_TCHITA},
        .num_vizinhos = 3
    },
    {
        .id = ASI_TCHITA,
        .nome = "Tchita",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_VLADVOSTOK, ASI_MONGOLIA, ASI_DUDINKA, ASI_SIBERIA, ASI_CHINA},
        .num_vizinhos = 5
    },
    {
        .id = ASI_MONGOLIA,
        .nome = "Mongólia",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_TCHITA, ASI_DUDINKA, ASI_OMSK, ASI_CHINA},
        .num_vizinhos = 4
    },
    {
        .id = ASI_CHINA,
        .nome = "China",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_TCHITA, ASI_MONGOLIA, ASI_OMSK, ASI_ARAL, ASI_INDIA, ASI_VIETNA, ASI_JAPAO, ASI_VLADVOSTOK},
        .num_vizinhos = 8
    },
    {
        .id = ASI_INDIA,
        .nome = "India",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_CHINA, ASI_ARAL, ASI_ORIENTE_M, ASI_VIETNA, OC_SUMATRA},
        .num_vizinhos = 5
    },
    {
        .id = ASI_VIETNA,
        .nome = "Vietnã",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_CHINA, ASI_INDIA, ASI_JAPAO, OC_SUMATRA, OC_BORNEO},
        .num_vizinhos = 5
    },
    {
        .id = ASI_JAPAO,
        .nome = "Japão",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_CHINA, ASI_VLADVOSTOK, ASI_VIETNA},
        .num_vizinhos = 3
    },
    {
        .id = ASI_VLADVOSTOK,
        .nome = "Vladvostok",
        .continente_id = CONT_ASIA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_CHINA, ASI_JAPAO, ASI_TCHITA, ASI_SIBERIA, AN_ALASKA},
        .num_vizinhos = 5
    },
    {
        .id = UE_MOSCOU,
        .nome = "Moscou",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { ASI_OMSK, ASI_ARAL, ASI_ORIENTE_M, UE_POLONIA, UE_SUECIA },
        .num_vizinhos = 5
    },
    {
        .id = UE_POLONIA,
        .nome = "Polônia",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { UE_PORTUGAL, UE_ALEMANHA, ASI_ORIENTE_M, UE_MOSCOU, AF_EGITO },
        .num_vizinhos = 5
    },
    {
        .id = UE_SUECIA,
        .nome = "Suécia",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { UE_INGLATERRA, UE_ISLANDIA, UE_ALEMANHA, UE_MOSCOU },
        .num_vizinhos = 4
    },
    {
        .id = UE_ALEMANHA,
        .nome = "Alemanha",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { UE_POLONIA, UE_PORTUGAL, UE_SUECIA, UE_INGLATERRA },
        .num_vizinhos = 4
    },
    {
        .id = UE_INGLATERRA,
        .nome = "Inglaterra",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { UE_PORTUGAL, UE_SUECIA, UE_ALEMANHA, UE_ISLANDIA },
        .num_vizinhos = 4
    },
    {
        .id = UE_ISLANDIA,
        .nome = "Islândia",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { UE_INGLATERRA, UE_SUECIA, AN_GROELANDIA },
        .num_vizinhos = 3
    },
    {
        .id = UE_PORTUGAL,
        .nome = "Portugal",
        .continente_id = CONT_EUROPA,
        .dono = -1,
        .tropas = 0,
        .vizinhos = { AF_EGITO, AF_NIGERIA, UE_ALEMANHA, UE_POLONIA, UE_INGLATERRA},
        .num_vizinhos = 5
    },
};

// ================= LOGICA =================

// Verifica se um jogador controla todo o continente
int jogador_controla_continente(int jogador_id, Continente *c) {
    for (int i = 0; i < c->num_territorios; i++) {
        int tid = c->territorios[i];
        if (territorios[tid].dono != jogador_id) {
            return 0;
        }
    }
    return 1;
}

// Calcula bonus total de continentes
int calcular_bonus_continentes(int jogador_id) {
    int bonus = 0;

    for (int i = 0; i < TOTAL_CONTINENTES; i++) {
        if (jogador_controla_continente(jogador_id, &continentes[i])) {
            bonus += continentes[i].bonus_completo;
        }
    }

    return bonus;
}

// ================= TESTE =================

void imprimir_mapa() {
    printf("=== MAPA ===\n");
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        printf(
            "%s | Dono: %d | Tropas: %d | Vizinhos:",
            territorios[i].nome,
            territorios[i].dono,
            territorios[i].tropas
        );

        for (int j = 0; j < territorios[i].num_vizinhos; j++) {
            printf(" %s", territorios[territorios[i].vizinhos[j]].nome);
        }
        printf("\n");
    }
}

int main() {
    // Simulação: jogador 0 domina tudo
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        territorios[i].dono = 0;
        territorios[i].tropas = 3;
    }

    imprimir_mapa();

    int bonus = calcular_bonus_continentes(0);
    printf("\nJogador 0 recebe bonus de continente: %d tropas\n", bonus);

    return 0;
}