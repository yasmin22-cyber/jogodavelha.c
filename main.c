#include "jogo.h"

int main(void) {
    Partida partida;

    configuraJogadores(&partida);
    inicia(&partida);

    return 0;
}