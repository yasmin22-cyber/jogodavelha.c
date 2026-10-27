#include <stdio.h>
#include "jogo.h"

void inicializaTabuleiro(Tabuleiro *tab) {
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            tab->M[i][j] = VAZIO;
}

void desenha(Tabuleiro *tab) {
    int i, j;
    char simbolo;

    printf("\033[2J\033[H");
    printf("JOGO DA VELHA\n\n");
    printf("    0   1   2\n");

    for (i = 0; i < 3; i++) {
        printf("%d ", i);
        for (j = 0; j < 3; j++) {
            if (tab->M[i][j] == MARCA_O)
                simbolo = 'O';
            else if (tab->M[i][j] == MARCA_X)
                simbolo = 'X';
            else
                simbolo = ' ';

            printf(" %c ", simbolo);
            if (j < 2)
                printf("|");
        }
        printf("\n");
        if (i < 2)
            printf("  ---+---+---\n");
    }
    printf("\n");
}

int temVencedor(Tabuleiro *tab) {
    int i, soma;

    for (i = 0; i < 3; i++) {
        soma = tab->M[i][0] + tab->M[i][1] + tab->M[i][2];
        if (soma == 3)
            return MARCA_O;
        if (soma == 12)
            return MARCA_X;

        soma = tab->M[0][i] + tab->M[1][i] + tab->M[2][i];
        if (soma == 3)
            return MARCA_O;
        if (soma == 12)
            return MARCA_X;
    }

    soma = tab->M[0][0] + tab->M[1][1] + tab->M[2][2];
    if (soma == 3)
        return MARCA_O;
    if (soma == 12)
        return MARCA_X;

    soma = tab->M[0][2] + tab->M[1][1] + tab->M[2][0];
    if (soma == 3)
        return MARCA_O;
    if (soma == 12)
        return MARCA_X;

    return 0;
}

void marcaJogada(Tabuleiro *tab, int x, int y, int tipo) {
    tab->M[x][y] = tipo;
}

void jogaTeclado(JogadorTeclado *jogador, Tabuleiro *tab) {
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
        } else if (tab->M[linha][coluna] != VAZIO) {
            printf("Essa casa ja esta ocupada!\n");
        } else {
            valida = 1;
        }
    }

    marcaJogada(tab, linha, coluna, jogador->tipo);
}

int contaChances(Tabuleiro *tab, int tipo) {
    int i, j, chances = 0;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tab->M[i][j] == VAZIO) {
                tab->M[i][j] = tipo;
                if (temVencedor(tab) == tipo)
                    chances++;
                tab->M[i][j] = VAZIO;
            }

    return chances;
}

int completaSequencia(Tabuleiro *tab, int tipo, int *x, int *y) {
    int i, j, venceu;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tab->M[i][j] == VAZIO) {
                tab->M[i][j] = tipo;
                venceu = (temVencedor(tab) == tipo);
                tab->M[i][j] = VAZIO;

                if (venceu) {
                    *x = i;
                    *y = j;
                    return 1;
                }
            }

    return 0;
}

int regra1(Tabuleiro *tab, int tipo, int oponente, int *x, int *y) {
    if (completaSequencia(tab, tipo, x, y))
        return 1;
    return completaSequencia(tab, oponente, x, y);
}

int regra2(Tabuleiro *tab, int tipo, int *x, int *y) {
    int i, j, chances;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tab->M[i][j] == VAZIO) {
                tab->M[i][j] = tipo;
                chances = contaChances(tab, tipo);
                tab->M[i][j] = VAZIO;

                if (chances >= 2) {
                    *x = i;
                    *y = j;
                    return 1;
                }
            }

    return 0;
}

int regra3(Tabuleiro *tab, int *x, int *y) {
    if (tab->M[1][1] == VAZIO) {
        *x = 1;
        *y = 1;
        return 1;
    }
    return 0;
}

int regra4(Tabuleiro *tab, int oponente, int *x, int *y) {
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    int i, l, c;

    for (i = 0; i < 4; i++) {
        l = cantos[i][0];
        c = cantos[i][1];
        if (tab->M[l][c] == oponente && tab->M[2 - l][2 - c] == VAZIO) {
            *x = 2 - l;
            *y = 2 - c;
            return 1;
        }
    }
    return 0;
}

int regra5(Tabuleiro *tab, int *x, int *y) {
    int cantos[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    int i;

    for (i = 0; i < 4; i++)
        if (tab->M[cantos[i][0]][cantos[i][1]] == VAZIO) {
            *x = cantos[i][0];
            *y = cantos[i][1];
            return 1;
        }
    return 0;
}

int regra6(Tabuleiro *tab, int *x, int *y) {
    int i, j;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (tab->M[i][j] == VAZIO) {
                *x = i;
                *y = j;
                return 1;
            }
    return 0;
}

void jogaIA(JogadorIA *jogador, Tabuleiro *tab) {
    int x = 0, y = 0, achou;
    int oponente;

    if (jogador->tipo == MARCA_O)
        oponente = MARCA_X;
    else
        oponente = MARCA_O;

    achou = regra1(tab, jogador->tipo, oponente, &x, &y);
    if (!achou)
        achou = regra2(tab, jogador->tipo, &x, &y);
    if (!achou)
        achou = regra3(tab, &x, &y);
    if (!achou)
        achou = regra4(tab, oponente, &x, &y);
    if (!achou)
        achou = regra5(tab, &x, &y);
    if (!achou)
        regra6(tab, &x, &y);

    marcaJogada(tab, x, y, jogador->tipo);
}

void configuraJogadores(Partida *partida) {
    partida->modo = 0;

    printf("JOGO DA VELHA\n");
    printf("1 - Dois jogadores\n");
    printf("2 - Contra o computador\n");

    while (partida->modo != 1 && partida->modo != 2) {
        printf("Escolha o modo: ");
        if (scanf("%d", &partida->modo) != 1) {
            partida->modo = 0;
            while (getchar() != '\n') {
            }
        }
        if (partida->modo != 1 && partida->modo != 2)
            printf("Opcao invalida. Digite 1 ou 2.\n");
    }

    partida->jogadores[0].tipo = MARCA_X;
    partida->jogadores[1].tipo = MARCA_O;
    partida->ia.tipo = MARCA_O;
}

void inicia(Partida *partida) {
    int jogadas = 0;
    int vez = 0;
    int vencedor = 0;

    inicializaTabuleiro(&partida->tabuleiro);
    desenha(&partida->tabuleiro);

    while (vencedor == 0 && jogadas < 9) {
        if (vez == 0) {
            printf("Vez do jogador X\n");
            jogaTeclado(&partida->jogadores[0], &partida->tabuleiro);
        } else if (partida->modo == 2) {
            jogaIA(&partida->ia, &partida->tabuleiro);
        } else {
            printf("Vez do jogador O\n");
            jogaTeclado(&partida->jogadores[1], &partida->tabuleiro);
        }

        jogadas++;
        desenha(&partida->tabuleiro);
        vencedor = temVencedor(&partida->tabuleiro);

        if (vez == 0)
            vez = 1;
        else
            vez = 0;
    }

    if (vencedor == MARCA_X)
        printf("Jogador X venceu!\n");
    else if (vencedor == MARCA_O)
        printf("Jogador O venceu!\n");
    else
        printf("Deu velha! Empate.\n");
}