#ifndef JOGO_H
#define JOGO_H

#define TAMANHO 3

typedef struct {
    int tabuleiro[TAMANHO][TAMANHO];
    char jogadorAtual;
    int quantidadeJogadas;
} Jogo;

void iniciarJogo(void);
void inicializarTabuleiro(Jogo *jogo);
void exibirTabuleiro(const Jogo *jogo);
char sortearJogadorInicial(void);
int realizarJogada(Jogo *jogo);
int verificarVitoria(const Jogo *jogo, char jogador);
int verificarEmpate(const Jogo *jogo);
void alternarJogador(Jogo *jogo);

#endif
