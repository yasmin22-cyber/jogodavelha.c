#ifndef JOGO_H
#define JOGO_H

#define TAMANHO 3

typedef struct {
    char tabuleiro[TAMANHO][TAMANHO];
    char jogadorAtual;
    int quantidadeJogadas;
} Jogo;

void inicializarJogo(Jogo *jogo);
void mostrarTabuleiro(Jogo *jogo);
int verificarVitoria(Jogo *jogo, char simbolo);
void alternarJogador(Jogo *jogo);
void jogadaHumana(Jogo *jogo);
void jogadaComputador(Jogo *jogo);
void iniciarJogo(void);

#endif