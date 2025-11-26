#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // capacidade fixa da fila
#define MAX_PILHA 3  // capacidade fixa da pilha

// Struct que representa uma peça do Tetris Stack
typedef struct {
    char nome;  // tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // identificador único
} Peca;

// Struct para fila circular
typedef struct {
    Peca elementos[MAX_FILA];
    int inicio;     // posição do primeiro elemento
    int fim;        // posição de inserção
    int quantidade; // número de elementos armazenados
} Fila;

// Struct para pilha
typedef struct {
    Peca elementos[MAX_PILHA];
    int topo; // índice do topo
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca();
int enqueue(Fila *fila, Peca nova);
int dequeue(Fila *fila);
int push(Pilha *pilha, Peca nova);
int pop(Pilha *pilha);
void jogarPeca(Fila *fila);
void reservarPeca(Fila *fila, Pilha *pilha);
void usarReserva(Pilha *pilha);
void trocarAtual(Fila *fila, Pilha *pilha);
void trocaMultipla(Fila *fila, Pilha *pilha);
void exibirEstado(Fila fila, Pilha pilha);

// Função principal
int main() {
    Fila filaPecas;
    Pilha pilhaReserva;
    int opcao;

    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);

    printf("===== TETRIS STACK — GERENCIAMENTO AVANÇADO =====\n");

    do {
        exibirEstado(filaPecas, pilhaReserva);

        printf("\nOpções:\n");
        printf("1 — Jogar peça da fila\n");
        printf("2 — Reservar peça (fila -> pilha)\n");
        printf("3 — Usar peça reservada (pilha)\n");
        printf("4 — Trocar peça da fila com topo da pilha\n");
        printf("5 — Trocar 3 da fila com 3 da pilha\n");
        printf("0 — Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&filaPecas);
                enqueue(&filaPecas, gerarPeca());
                break;

            case 2:
                reservarPeca(&filaPecas, &pilhaReserva);
                enqueue(&filaPecas, gerarPeca());
                break;

            case 3:
                usarReserva(&pilhaReserva);
                enqueue(&filaPecas, gerarPeca());
                break;

            case 4:
                trocarAtual(&filaPecas, &pilhaReserva);
                break;

            case 5:
                trocaMultipla(&filaPecas, &pilhaReserva);
                break;

            case 0:
                printf("Encerrando o jogo... Obrigado por jogar!\n");
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa fila com 5 peças iniciais
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
    srand(time(NULL));

    for (int i = 0; i < MAX_FILA; i++)
        enqueue(fila, gerarPeca());
}

// Inicializa pilha vazia
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Gera nova peça com tipo aleatório
Peca gerarPeca() {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    static int contadorID = 0;

    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// Inserção na fila circular
int enqueue(Fila *fila, Peca nova) {
    if (fila->quantidade == MAX_FILA)
        return 0;

    fila->elementos[fila->fim] = nova;
    fila->fim = (fila->fim + 1) % MAX_FILA;
    fila->quantidade++;
    return 1;
}

// Remoção da fila
int dequeue(Fila *fila) {
    if (fila->quantidade == 0)
        return 0;

    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    fila->quantidade--;
    return 1;
}

// Push na pilha
int push(Pilha *pilha, Peca nova) {
    if (pilha->topo == MAX_PILHA - 1)
        return 0;

    pilha->elementos[++pilha->topo] = nova;
    return 1;
}

// Pop na pilha
int pop(Pilha *pilha) {
    if (pilha->topo == -1)
        return 0;

    pilha->topo--;
    return 1;
}

// Jogar peça da fila
void jogarPeca(Fila *fila) {
    if (fila->quantidade == 0) {
        printf("A fila está vazia!\n");
        return;
    }
    Peca jogada = fila->elementos[fila->inicio];
    dequeue(fila);
    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
}

// Enviar peça da fila para a pilha
void reservarPeca(Fila *fila, Pilha *pilha) {
    if (fila->quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }
    if (pilha->topo == MAX_PILHA - 1) {
        printf("Pilha cheia!\n");
        return;
    }
    Peca p = fila->elementos[fila->inicio];
    dequeue(fila);
    push(pilha, p);
    printf("Peça reservada: [%c %d]\n", p.nome, p.id);
}

// Usar peça reservada
void usarReserva(Pilha *pilha) {
    if (pilha->topo == -1) {
        printf("Nenhuma peça reservada!\n");
        return;
    }
    Peca usada = pilha->elementos[pilha->topo];
    pop(pilha);
    printf("Peça usada: [%c %d]\n", usada.nome, usada.id);
}

// Trocar peça da frente da fila com topo da pilha
void trocarAtual(Fila *fila, Pilha *pilha) {
    if (fila->quantidade == 0 || pilha->topo == -1) {
        printf("Impossível trocar: fila ou pilha vazia!\n");
        return;
    }
    Peca temp = fila->elementos[fila->inicio];
    fila->elementos[fila->inicio] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;
    printf("Troca realizada entre fila e pilha!\n");
}

// Trocar 3 peças entre fila e pilha
void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("Troca múltipla impossível! Ambas precisam ter ao menos 3 peças.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int index = (fila->inicio + i) % MAX_FILA;
        Peca temp = fila->elementos[index];
        fila->elementos[index] = pilha->elementos[pilha->topo - i];
        pilha->elementos[pilha->topo - i] = temp;
    }

    printf("Troca múltipla realizada com sucesso!\n");
