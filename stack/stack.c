#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LIVRE 0
#define PAREDE 1
#define VISITADA 2
#define BECO 3
#define SAIDA 4
#define MAPA_LINHAS 30
#define MAPA_COLUNAS 30

typedef struct Nodo {
    int coord;
    struct Nodo *prox;
} Nodo;

typedef struct Pilha {
    Nodo *topo;
    int tamanho;
} Pilha;

Pilha* criaPilha();
Nodo* criaNodo();
void push(Pilha* pilha, int coord);
int pop();
int criaLabirinto (int matriz[MAPA_LINHAS][MAPA_COLUNAS]);
void jogaRecursivo(Pilha* pilha, int matriz[MAPA_LINHAS][MAPA_COLUNAS], int coordRato, int acabou);
void exibeLabirinto(int matriz[MAPA_LINHAS][MAPA_COLUNAS], int linRato, int colRato);
void exibeRelatorio(Pilha *pilha, int matriz[MAPA_LINHAS][MAPA_COLUNAS]);
int buildCoord(int lin, int col);
int getLinhaByCoord(int coord);
int getColunaByCoord(int coord);

int main() {
    int labirinto[MAPA_LINHAS][MAPA_COLUNAS] = {0};
    Pilha *pilha = criaPilha();

    int coordRato = criaLabirinto(labirinto);

    // empilha posição inicial do rato
    push(pilha, coordRato);

    printf("\033[2J");
    jogaRecursivo(pilha, labirinto, coordRato, 0);
    return 0;
}

Pilha* criaPilha() {
    Pilha* pilha = (Pilha *) malloc(sizeof(Pilha));
    if (!pilha) {
        printf("Problemas na Alocação da pilha!!!");
        exit(0);
    }
    pilha->tamanho = 0;
    pilha->topo = NULL;
    return pilha;
}

Nodo* criaNodo() {
    Nodo* nodo = (Nodo *) malloc(sizeof(Nodo));
    if (!nodo) {
        printf("Problemas na Alocação da pilha!!!");
        exit(0);
    }
    return nodo;
}

void push(Pilha* pilha, int coord) {
    Nodo *nodo = criaNodo();

    nodo->coord = coord;
    nodo->prox = pilha->topo;
    pilha->topo = nodo;
    pilha->tamanho++;
}

int pop(Pilha* pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        return -1;
    }

    Nodo *removido = pilha->topo;
    int valor = removido->coord;
    pilha->topo = pilha->topo->prox;
    free(removido);
    pilha->tamanho--;

    return valor;
}

void liberaPilha(Pilha* pilha) {
    if (pilha == NULL) {
        return;
    }

    while (pilha->topo != NULL) {
        Nodo *elem = pilha->topo;
        pilha->topo = pilha->topo->prox;
        free(elem);
    }

    pilha->tamanho = 0;
}

int criaLabirinto(int matriz[MAPA_LINHAS][MAPA_COLUNAS]) {
    // preenche paredes nas bordas
    for (int i = 0; i < MAPA_LINHAS; i++) {
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            if (
                i == 0 ||
                j == 0 ||
                i == MAPA_LINHAS - 1 ||
                j == MAPA_COLUNAS -1
                ) {
                matriz[i][j] = PAREDE;
            }
        }
    }

    // gerar saída
    int linSaida = rand() % MAPA_LINHAS;
    matriz[linSaida][MAPA_COLUNAS - 1] = SAIDA;

    // gerar posicao inicial
    // qualquer linha (exceto bordas) e colunas (exceto bordas) entre 1/7 da matriz
    int linRato = rand() % (MAPA_LINHAS - 2) + 1;
    int maxCol = (MAPA_COLUNAS + 7 - 1) / 7;
    int colRato = rand() % maxCol + 1;


    // gerar paredes espalhadas e preenche espaços como livre
    for (int i = 1; i < MAPA_LINHAS - 1; i++) {
        for (int j = 1; j < MAPA_COLUNAS - 1; j++) {
            if (i == linRato && j == colRato) continue;

            float chanceParede;
            if (j < 6) {
                chanceParede = 0.2;
            } else if (j < 16) {
                chanceParede = 0.35;
            } else {
                chanceParede = 0.45;
            }

            float random = (float) rand() / RAND_MAX;

            // zona segura do rato
            int maxLinPermitidoRato = linRato + 2;
            int minLinPermitidoRato = linRato - 2;
            int maxColPermitidoRato = colRato + 2;
            int minColPermitidoRato = colRato - 2;

            // zona segura da saída
            int maxLinPermitidoSaida = linSaida + 4;
            int minLinPermitidoSaida = linSaida - 4;
            int minColPermitidoSaida = MAPA_COLUNAS - 2;


            if (
            random < chanceParede &&
            !((i >= minLinPermitidoRato && i <= maxLinPermitidoRato &&
               j >= minColPermitidoRato && j <= maxColPermitidoRato) ||
              (i >= minLinPermitidoSaida && i <= maxLinPermitidoSaida &&
               j >= minColPermitidoSaida))
           ) {
                matriz[i][j] = PAREDE;
            } else {
                matriz[i][j] = LIVRE;
            }
        }
    }

    return buildCoord(linRato, colRato);
}

void jogaRecursivo(Pilha* pilha, int matriz[MAPA_LINHAS][MAPA_COLUNAS], int coordRato, int acabou) {
    int linRato = getLinhaByCoord(coordRato);
    int colRato = getColunaByCoord(coordRato);

    exibeLabirinto(matriz, linRato, colRato);
    usleep(100000);

    if (acabou) {
        exibeRelatorio(pilha, matriz);
        return;
    }

    // sentido horario
    int posDireita = matriz[linRato][colRato + 1];
    int posEmbaixo = matriz[linRato + 1][colRato];
    int posEsquerda = matriz[linRato][colRato - 1];
    int posEmCima = matriz[linRato - 1][colRato];

    // Saída
    if (posDireita == SAIDA) {
        int coordProx = buildCoord(linRato, colRato + 1);
        push(pilha, coordProx);
        matriz[linRato][colRato] = VISITADA;
        jogaRecursivo(pilha, matriz, coordProx, 1);
        return;
    }

    // Livre
    if (posDireita == LIVRE) {
        int coordProx = buildCoord(linRato, colRato + 1);
        push(pilha, coordProx);
        matriz[linRato][colRato] = VISITADA;
        jogaRecursivo(pilha, matriz, coordProx, 0);
        return;
    }
    if (posEmbaixo == LIVRE) {
        int coordProx = buildCoord(linRato + 1, colRato);
        push(pilha, coordProx);
        matriz[linRato][colRato] = VISITADA;
        jogaRecursivo(pilha, matriz, coordProx, 0);
        return;
    }
    if (posEsquerda == LIVRE) {
        int coordProx = buildCoord(linRato, colRato - 1);
        push(pilha, coordProx);
        matriz[linRato][colRato] = VISITADA;
        jogaRecursivo(pilha, matriz, coordProx, 0);
        return;
    }
    if (posEmCima == LIVRE) {
        int coordProx = buildCoord(linRato - 1, colRato);
        push(pilha, coordProx);
        matriz[linRato][colRato] = VISITADA;
        jogaRecursivo(pilha, matriz, coordProx, 0);
        return;
    }

    int coordRemovida = pop(pilha);

    if (pilha->topo == NULL) {
        exibeRelatorio(pilha, matriz);
        return;
    }

    int posicaoAnterior = pilha->topo->coord;
    matriz[getLinhaByCoord(coordRemovida)][getColunaByCoord(coordRemovida)] = BECO;

    jogaRecursivo(pilha, matriz, posicaoAnterior, 0);
}

void exibeLabirinto(int matriz[MAPA_LINHAS][MAPA_COLUNAS], int linRato, int colRato) {
    const char *COR_RESET  = "\033[0m";
    const char *COR_PAREDE = "\033[48;5;235m\033[38;5;240m";
    const char *COR_LIVRE  = "\033[48;5;234m";
    const char *COR_VISITA = "\033[38;5;80m";
    const char *COR_BECO   = "\033[38;5;172m";
    const char *COR_SAIDA  = "\033[38;5;226m";
    const char *COR_RATO   = "\033[38;5;82m";

    printf("\033[H");

    for (int i = 0; i < MAPA_LINHAS; i++) {
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            if (i == linRato && j == colRato) {
                printf("%s \u263A %s", COR_RATO, COR_RESET);
                continue;
            }
            switch (matriz[i][j]) {
                case PAREDE:   printf("%s\u2588\u2588\u2588%s", COR_PAREDE, COR_RESET); break;
                case LIVRE:    printf("%s   %s",                COR_LIVRE,  COR_RESET); break;
                case VISITADA: printf("%s \u00B7 %s",           COR_VISITA, COR_RESET); break;
                case BECO:     printf("%s\u2591\u2591\u2591%s", COR_BECO,   COR_RESET); break;
                case SAIDA:    printf("%s \u2605 %s",           COR_SAIDA,  COR_RESET); break;
            }
        }
        printf("\n");
    }

    printf("\n%s███%s Parede  %s   %s Livre  %s · %s Visitada  %s░░░%s Beco  %s ★ %s Saída  %s ☺ %s Rato\n",
        COR_PAREDE, COR_RESET,
        COR_LIVRE,  COR_RESET,
        COR_VISITA, COR_RESET,
        COR_BECO,   COR_RESET,
        COR_SAIDA,  COR_RESET,
        COR_RATO,   COR_RESET
    );

    fflush(stdout);
}

void exibeRelatorio( Pilha *pilha, int matriz[MAPA_LINHAS][MAPA_COLUNAS]) {
    int visitadas = 0;
    int becos = 0;
    int paredes = 0;
    int livres = 0;

    for (int i = 0; i < MAPA_LINHAS; i++) {
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            switch (matriz[i][j]) {
                case VISITADA:
                    visitadas++;
                    break;
                case BECO:
                    becos++;
                    break;
                case PAREDE:
                    paredes++;
                    break;
                case LIVRE:
                    livres++;
                    break;
            }
        }
    }

    printf("\n");
    printf("=====================================\n");
    printf("      RELATORIO DE EXECUCAO\n");
    printf("=====================================\n");
    printf("Caminho encontrado: SIM\n");
    printf("Tamanho do caminho: %d passos\n", pilha->tamanho);
    printf("Posicoes visitadas: %d\n", visitadas);
    printf("Becos encontrados : %d\n", becos);
    printf("Paredes geradas   : %d\n", paredes);
    printf("Livres restantes  : %d\n", livres);
    printf("=====================================\n");
}

int buildCoord(int lin, int col) {
    return lin * 100 + col;
}

int getLinhaByCoord(int coord) {
    return coord / 100;
}
int getColunaByCoord(int coord) {
    return coord % 100;
}
