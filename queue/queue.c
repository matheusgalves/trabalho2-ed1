#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contadorVoos = 0;

typedef struct Aviao {
    int id;
    char modelo[50];
    char nome[50];
    char origem[50];
    char destino[50];
} Aviao;

typedef struct Nodo {
    Aviao aviao;
    struct Nodo *prox;
} Nodo;

typedef struct Fila {
    Nodo *inicio;
    Nodo *fim;
    int tamanho;
} Fila;

Fila* criaFila();
void liberaFila(Fila* fila);
Nodo* criaNodo();
void insereFila(Fila* fila, Aviao aviao);
void removeFila(Fila* fila);

void imprimeFila(Fila* fila);
void imprimeAviao(Aviao* aviao);

void exibeMenu(Fila* filaDecolagem, Fila* filaPouso);
void adicionarAviaoNaFila(Fila* fila, char operacao);
void autorizarOperacao(Fila* fila, char operacao);
// void decolar(Aviao* aviao);
// void pousar(Aviao* aviao);

int main() {
    Fila* filaDecolagem = criaFila();
    Fila* filaPouso = criaFila();
    int opcao;

    do {
        exibeMenu(filaDecolagem, filaPouso);
        printf("  Opcao: ");
        scanf("%d", &opcao);
        getchar(); /* limpa o '\n' do buffer */

        switch (opcao) {
            case 1:  adicionarAviaoNaFila(filaDecolagem, 'D');  break;
            case 2:  adicionarAviaoNaFila(filaPouso, 'P');      break;
            case 3:  autorizarOperacao(filaDecolagem, 'D');  break;
            case 4:  autorizarOperacao(filaPouso, 'P');      break;
            case 5:  imprimeFila(filaDecolagem);     break;
            case 6:  imprimeFila(filaPouso);        break;
            case 7:
                printf("\n  Avioes aguardando decolagem : %d\n", filaDecolagem->tamanho);
                printf("  Avioes aguardando pouso     : %d\n\n", filaPouso->tamanho);
                break;
            case 0:
                printf("\n  Encerrando sistema. Bom voo!\n\n");
                break;
            default:
                printf("\n  Opcao invalida. Tente novamente.\n\n");
        }

    } while (opcao != 0);


    liberaFila(filaDecolagem);
    liberaFila(filaPouso);
}

Fila* criaFila() {
    Fila* fila = (Fila*) malloc(sizeof(Fila));
    if (!fila) {
        printf("Problemas na Alocação da fila!!!");
        exit(0);
    }

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;

    return fila;
}

void liberaFila(Fila* fila) {
    if (!fila) {
        printf("Fila não foi passada corretamente!!!");
        exit(0);
    }

    while (fila->inicio != NULL) {
        Nodo * nodo = fila->inicio;
        fila->inicio = fila->inicio->prox;
        free(nodo);
    }
    free(fila);
}

Nodo* criaNodo() {
    Nodo* nodo = (Nodo *) malloc(sizeof(Nodo));
    if (!nodo) {
        printf("Problemas na Alocação do nodo!!!");
        exit(0);
    }

    nodo->prox = NULL;
    return nodo;
}

void insereFila(Fila* fila, Aviao aviao) {
    Nodo* nodo = criaNodo();
    nodo->aviao = aviao;
    nodo->prox = NULL;

    if (fila->fim == NULL) {
        fila->inicio = nodo;
        fila->fim = nodo;
        fila->tamanho++;
        return;
    }

    Nodo* elemFim = fila->fim;
    elemFim->prox = nodo;

    fila->fim = nodo;
    fila->tamanho++;
}

void removeFila(Fila* fila) {
    if (!fila->inicio) {
        return;
    }

    Nodo* elemInicio = fila->inicio;
    fila->inicio = elemInicio->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(elemInicio);
    fila->tamanho--;
}

void imprimeFila(Fila* fila) {
    if (!fila->inicio) {
        printf("A pista está livre");
    }

    Nodo* nodo = fila->inicio;
    while (nodo != NULL) {
        imprimeAviao(&nodo->aviao);
        nodo = nodo->prox;
    }
}

void imprimeAviao(Aviao* aviao) {
    printf("\n");
    printf("        .--.\n");
    printf("       /  o \\\n");
    printf("      |  ___|\n");
    printf("  ____|_/       ID: %-4d  Modelo: %s\n", aviao->id, aviao->modelo);
    printf(" /  ____|        Nome: %s\n", aviao->nome);
    printf("(______|         Origem:  %-20s -> Destino: %s\n", aviao->origem, aviao->destino);
    printf("  |  |  |______/\n");
    printf("  '--'--'\n");
    printf("\n");
}

void exibeMenu(Fila* filaDecolagem, Fila* filaPouso) {
    printf("\n");
    printf("  ================================================\n");
    printf("         SISTEMA DE CONTROLE DE VOO - ATC         \n");
    printf("  ================================================\n");
    printf("   [1]  Adicionar aviao na fila de DECOLAGEM\n");
    printf("   [2]  Adicionar aviao na fila de POUSO\n");
    printf("   [3]  Autorizar DECOLAGEM do proximo aviao\n");
    printf("   [4]  Autorizar POUSO do proximo aviao\n");
    printf("   [5]  Listar fila de DECOLAGEM\n");
    printf("   [6]  Listar fila de POUSO\n");
    printf("   [7]  Ver quantidade de avioes nas filas\n");
    printf("   [0]  Sair\n");
    printf("  ================================================\n");
    printf("   Decolagem: %d aviao(es)  |  Pouso: %d aviao(es)\n",
           filaDecolagem->tamanho, filaPouso->tamanho);
    printf("  ================================================\n");
}

void adicionarAviaoNaFila(Fila* fila, char operacao) {
    Aviao aviao;
    aviao.id = ++contadorVoos;

    if (operacao == 'D') {
        printf("\n  -- Cadastro de aviao para DECOLAGEM --\n");
    } else {
        printf("\n  -- Cadastro de aviao para POUSO --\n");
    }

    printf("  Nome do voo  : "); fgets(aviao.nome,    50, stdin); aviao.nome[strcspn(aviao.nome,    "\n")] = 0;
    printf("  Modelo       : "); fgets(aviao.modelo,  50, stdin); aviao.modelo[strcspn(aviao.modelo,  "\n")] = 0;
    printf("  Origem       : "); fgets(aviao.origem,  50, stdin); aviao.origem[strcspn(aviao.origem,  "\n")] = 0;
    printf("  Destino      : "); fgets(aviao.destino, 50, stdin); aviao.destino[strcspn(aviao.destino, "\n")] = 0;

    insereFila(fila, aviao);
    if (operacao == 'D') {
        printf("\n  Aviao ID %d adicionado a fila de decolagem. Posicao: %d\n\n",
           aviao.id, fila->tamanho);
    } else {
        printf("\n  Aviao ID %d adicionado a fila de pouso. Posicao: %d\n\n",
           aviao.id, fila->tamanho);
    }

}

void autorizarOperacao(Fila* fila, char operacao) {
    if (!fila->inicio) {
        if (operacao == 'D') {
            printf("\n  Nenhum aviao aguardando decolagem.\n\n");
        } else {
            printf("\n  Nenhum aviao aguardando pouso.\n\n");
        }

        return;
    }

    if (operacao == 'D') {
        printf("\n  -- Proximo aviao para DECOLAGEM --\n");
    } else {
        printf("\n  -- Proximo aviao para POUSO --\n");
    }

    imprimeAviao(&fila->inicio->aviao);

    printf("  Confirmar? (1 = Sim / 0 = Nao): ");
    int confirma;
    scanf("%d", &confirma);
    getchar();

    if (confirma == 1) {
        if (operacao == 'D') {
            //aqui
            printf("\n  Pista liberada! Aviao %s decolando...\n\n",
               fila->inicio->aviao.nome);
        } else {
            //aqui
            printf("\n  Pista liberada! Aviao %s pousando...\n\n",
               fila->inicio->aviao.nome);
        }

        removeFila(fila);
    } else {
        printf("\n  Operação cancelada.\n\n");
    }
}