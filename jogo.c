#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jogo.h"

static void limparEntrada(void) {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

static int lerCoordenada(const char *mensagem, int *coordenada) {
    printf("%s", mensagem);

    if (scanf("%d", coordenada) != 1) {
        printf("Entrada invalida. Digite um numero entre 0 e 2.\n");
        limparEntrada();
        return 0;
    }

    return 1;
}

void iniciarJogo(void) {
    Jogo jogo;

    srand((unsigned int)time(NULL));

    inicializarTabuleiro(&jogo);
    jogo.jogadorAtual = sortearJogadorInicial();

    exibirTabuleiro(&jogo);
    printf("Jogador inicial sorteado: %c\n", jogo.jogadorAtual);

    while (1) {
        printf("\nVez do jogador %c\n", jogo.jogadorAtual);

        if (!realizarJogada(&jogo)) {
            continue;
        }

        exibirTabuleiro(&jogo);

        if (verificarVitoria(&jogo, jogo.jogadorAtual)) {
            printf("\nJogador %c venceu!\n", jogo.jogadorAtual);
            break;
        }

        if (verificarEmpate(&jogo)) {
            printf("\nEmpate!\n");
            break;
        }

        alternarJogador(&jogo);
    }
}

void inicializarTabuleiro(Jogo *jogo) {
    int linha;
    int coluna;

    for (linha = 0; linha < TAMANHO; linha++) {
        for (coluna = 0; coluna < TAMANHO; coluna++) {
            jogo->tabuleiro[linha][coluna] = 0;
        }
    }

    jogo->quantidadeJogadas = 0;
}

void exibirTabuleiro(const Jogo *jogo) {
    int linha;
    int coluna;

    printf("\033[2J\033[H");
    printf("JOGO DA VELHA\n\n");
    printf("    0   1   2\n");
    printf("  +---+---+---+\n");

    for (linha = 0; linha < TAMANHO; linha++) {
        printf("%d |", linha);

        for (coluna = 0; coluna < TAMANHO; coluna++) {
            char simbolo = ' ';

            if (jogo->tabuleiro[linha][coluna] == 1) {
                simbolo = 'O';
            } else if (jogo->tabuleiro[linha][coluna] == 4) {
                simbolo = 'X';
            }

            printf(" %c |", simbolo);
        }

        printf("\n  +---+---+---+\n");
    }
}

char sortearJogadorInicial(void) {
    if (rand() % 2 == 0) {
        return 'X';
    }

    return 'O';
}

int realizarJogada(Jogo *jogo) {
    int linha;
    int coluna;

    if (!lerCoordenada("Linha (0 a 2): ", &linha)) {
        return 0;
    }

    if (!lerCoordenada("Coluna (0 a 2): ", &coluna)) {
        return 0;
    }

    if (linha < 0 || linha >= TAMANHO ||
        coluna < 0 || coluna >= TAMANHO) {
        printf("Coordenada invalida. Use valores de 0 a 2.\n");
        return 0;
    }

    if (jogo->tabuleiro[linha][coluna] != 0) {
        printf("Essa casa ja esta ocupada. Escolha outra.\n");
        return 0;
    }

    if (jogo->jogadorAtual == 'O') {
        jogo->tabuleiro[linha][coluna] = 1;
    } else {
        jogo->tabuleiro[linha][coluna] = 4;
    }

    jogo->quantidadeJogadas++;

    return 1;
}

int verificarVitoria(const Jogo *jogo, char jogador) {
    int marca = (jogador == 'O') ? 1 : 4;
    int linha;
    int coluna;
    int somaDiagonalPrincipal = 0;
    int somaDiagonalSecundaria = 0;

    for (linha = 0; linha < TAMANHO; linha++) {
        int somaLinha = 0;
        int somaColuna = 0;

        for (coluna = 0; coluna < TAMANHO; coluna++) {
            somaLinha += jogo->tabuleiro[linha][coluna];
            somaColuna += jogo->tabuleiro[coluna][linha];
        }

        if (somaLinha == marca * TAMANHO ||
            somaColuna == marca * TAMANHO) {
            return 1;
        }

        somaDiagonalPrincipal += jogo->tabuleiro[linha][linha];

        somaDiagonalSecundaria +=
            jogo->tabuleiro[linha][TAMANHO - 1 - linha];
    }

    if (somaDiagonalPrincipal == marca * TAMANHO ||
        somaDiagonalSecundaria == marca * TAMANHO) {
        return 1;
    }

    return 0;
}

int verificarEmpate(const Jogo *jogo) {
    return jogo->quantidadeJogadas == TAMANHO * TAMANHO;
}

void alternarJogador(Jogo *jogo) {
    if (jogo->jogadorAtual == 'X') {
        jogo->jogadorAtual = 'O';
    } else {
        jogo->jogadorAtual = 'X';
    }
}