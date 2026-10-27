#include <stdio.h>
#include "jogo.h"

void inicializarJogo(Jogo *jogo) {
    int i, j;

    for (i = 0; i < TAMANHO; i++)
        for (j = 0; j < TAMANHO; j++)
            jogo->tabuleiro[i][j] = ' ';

    jogo->jogadorAtual = 'X';
    jogo->quantidadeJogadas = 0;
}

void mostrarTabuleiro(Jogo *jogo) {
    int i;

    printf("\n    0   1   2\n");
    for (i = 0; i < TAMANHO; i++) {
        printf("%d   %c | %c | %c\n", i, jogo->tabuleiro[i][0],
               jogo->tabuleiro[i][1], jogo->tabuleiro[i][2]);
        if (i < TAMANHO - 1)
            printf("   ---+---+---\n");
    }
}

int verificarVitoria(Jogo *jogo, char simbolo) {
    int i;

    for (i = 0; i < TAMANHO; i++) {
        if (jogo->tabuleiro[i][0] == simbolo &&
            jogo->tabuleiro[i][1] == simbolo &&
            jogo->tabuleiro[i][2] == simbolo)
            return 1;

        if (jogo->tabuleiro[0][i] == simbolo &&
            jogo->tabuleiro[1][i] == simbolo &&
            jogo->tabuleiro[2][i] == simbolo)
            return 1;
    }

    if (jogo->tabuleiro[0][0] == simbolo &&
        jogo->tabuleiro[1][1] == simbolo &&
        jogo->tabuleiro[2][2] == simbolo)
        return 1;

    if (jogo->tabuleiro[0][2] == simbolo &&
        jogo->tabuleiro[1][1] == simbolo &&
        jogo->tabuleiro[2][0] == simbolo)
        return 1;

    return 0;
}

void alternarJogador(Jogo *jogo) {
    if (jogo->jogadorAtual == 'X')
        jogo->jogadorAtual = 'O';
    else
        jogo->jogadorAtual = 'X';
}

void jogadaHumana(Jogo *jogo) {
    int linha, coluna;
    int valida = 0;

    while (valida == 0) {
        printf("Digite linha e coluna (ex: 1 2): ");

        if (scanf("%d %d", &linha, &coluna) != 2) {
            while (getchar() != '\n') {
            }
            printf("Entrada invalida! Digite dois numeros.\n");
        } else if (linha < 0 || linha > 2 || coluna < 0 || coluna > 2) {
            printf("Posicao invalida! Use numeros de 0 a 2.\n");
        } else if (jogo->tabuleiro[linha][coluna] != ' ') {
            printf("Essa casa ja esta ocupada!\n");
        } else {
            valida = 1;
        }
    }

    jogo->tabuleiro[linha][coluna] = jogo->jogadorAtual;
    jogo->quantidadeJogadas++;
}

int contarChancesDeVitoria(Jogo *jogo, char simbolo) {
    int i, j;
    int chances = 0;

    for (i = 0; i < TAMANHO; i++)
        for (j = 0; j < TAMANHO; j++)
            if (jogo->tabuleiro[i][j] == ' ') {
                jogo->tabuleiro[i][j] = simbolo;
                if (verificarVitoria(jogo, simbolo))
                    chances++;
                jogo->tabuleiro[i][j] = ' ';
            }

    return chances;
}

int procurarVitoria(Jogo *jogo, char simbolo, int *linha, int *coluna) {
    int i, j, venceu;

    for (i = 0; i < TAMANHO; i++)
        for (j = 0; j < TAMANHO; j++)
            if (jogo->tabuleiro[i][j] == ' ') {
                jogo->tabuleiro[i][j] = simbolo;
                venceu = verificarVitoria(jogo, simbolo);
                jogo->tabuleiro[i][j] = ' ';

                if (venceu) {
                    *linha = i;
                    *coluna = j;
                    return 1;
                }
            }

    return 0;
}

int procurarFork(Jogo *jogo, char simbolo, int *linha, int *coluna) {
    int i, j, chances;

    for (i = 0; i < TAMANHO; i++)
        for (j = 0; j < TAMANHO; j++)
            if (jogo->tabuleiro[i][j] == ' ') {
                jogo->tabuleiro[i][j] = simbolo;
                chances = contarChancesDeVitoria(jogo, simbolo);
                jogo->tabuleiro[i][j] = ' ';

                if (chances >= 2) {
                    *linha = i;
                    *coluna = j;
                    return 1;
                }
            }

    return 0;
}

int procurarCantoOposto(Jogo *jogo, int *linha, int *coluna) {
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    int i, l, c;

    for (i = 0; i < 4; i++) {
        l = cantos[i][0];
        c = cantos[i][1];
        if (jogo->tabuleiro[l][c] == 'X' &&
            jogo->tabuleiro[2 - l][2 - c] == ' ') {
            *linha = 2 - l;
            *coluna = 2 - c;
            return 1;
        }
    }

    return 0;
}

int procurarCantoVazio(Jogo *jogo, int *linha, int *coluna) {
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    int i;

    for (i = 0; i < 4; i++)
        if (jogo->tabuleiro[cantos[i][0]][cantos[i][1]] == ' ') {
            *linha = cantos[i][0];
            *coluna = cantos[i][1];
            return 1;
        }

    return 0;
}

int procurarCasaVazia(Jogo *jogo, int *linha, int *coluna) {
    int i, j;

    for (i = 0; i < TAMANHO; i++)
        for (j = 0; j < TAMANHO; j++)
            if (jogo->tabuleiro[i][j] == ' ') {
                *linha = i;
                *coluna = j;
                return 1;
            }

    return 0;
}

void jogadaComputador(Jogo *jogo) {
    int linha = 0, coluna = 0, achou;

    achou = procurarVitoria(jogo, 'O', &linha, &coluna);
    if (!achou)
        achou = procurarVitoria(jogo, 'X', &linha, &coluna);
    if (!achou)
        achou = procurarFork(jogo, 'O', &linha, &coluna);
    if (!achou && jogo->tabuleiro[1][1] == ' ') {
        linha = 1;
        coluna = 1;
        achou = 1;
    }
    if (!achou)
        achou = procurarCantoOposto(jogo, &linha, &coluna);
    if (!achou)
        achou = procurarCantoVazio(jogo, &linha, &coluna);
    if (!achou)
        procurarCasaVazia(jogo, &linha, &coluna);

    jogo->tabuleiro[linha][coluna] = 'O';
    jogo->quantidadeJogadas++;

    printf("Computador jogou na linha %d, coluna %d\n", linha, coluna);
}

void jogarPartida(int modo) {
    Jogo jogo;
    int fim = 0;

    inicializarJogo(&jogo);

    while (fim == 0) {
        mostrarTabuleiro(&jogo);
        printf("Vez do jogador %c\n", jogo.jogadorAtual);
        if (modo == 2 && jogo.jogadorAtual == 'O')
            jogadaComputador(&jogo);
        else
            jogadaHumana(&jogo);

        if (verificarVitoria(&jogo, jogo.jogadorAtual)) {
            mostrarTabuleiro(&jogo);
            printf("Jogador %c venceu!\n", jogo.jogadorAtual);
            fim = 1;
        } else if (jogo.quantidadeJogadas == 9) {
            mostrarTabuleiro(&jogo);
            printf("Deu velha! Empate.\n");
            fim = 1;
        } else {
            alternarJogador(&jogo);
        }
    }
}

void iniciarJogo(void) {
    int modo = 0;

    printf("===== JOGO DA VELHA =====\n");
    printf("1 - Dois jogadores\n");
    printf("2 - Contra o computador\n");

    while (modo != 1 && modo != 2) {
        printf("Escolha o modo: ");
        if (scanf("%d", &modo) != 1) {
            modo = 0;
            while (getchar() != '\n') {
            }
        }
        if (modo != 1 && modo != 2)
            printf("Opcao invalida. Digite 1 ou 2.\n");
    }
    jogarPartida(modo);
}