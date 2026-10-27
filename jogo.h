#ifndef JOGO_H
#define JOGO_H
 
#define VAZIO 0
#define MARCA_O 1
#define MARCA_X 4
 
typedef struct {
    int M[3][3];
} Tabuleiro;
 
typedef struct {
    int tipo;
} JogadorTeclado;
 
typedef struct {
    int tipo;
} JogadorIA;
 
typedef struct {
    Tabuleiro tabuleiro;
    JogadorTeclado jogadores[2];
    JogadorIA ia;
    int modo;
} Partida;
 
void inicializaTabuleiro(Tabuleiro *tab);
void desenha(Tabuleiro *tab);
int temVencedor(Tabuleiro *tab);
void marcaJogada(Tabuleiro *tab, int x, int y, int tipo);
 
void jogaTeclado(JogadorTeclado *jogador, Tabuleiro *tab);
void jogaIA(JogadorIA *jogador, Tabuleiro *tab);
 
void configuraJogadores(Partida *partida);
void inicia(Partida *partida);
 
#endif
 