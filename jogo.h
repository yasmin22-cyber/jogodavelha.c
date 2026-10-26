#ifndef JOGO_H
#define JOGO_H

#define TAMANHO 3

typedef struct {
    char tabuleiro[TAMANHO][TAMANHO];
    char jogadorAtual;
    int quantidadeJogadas;
} Jogo;

#endif
typedef struct {
    char tabuleiro[TAMANHO][TAMANHO];
    char jogadorAtual;
    int quantidadeJogadas;
} Jogo;

void iniciarJogo(void);
