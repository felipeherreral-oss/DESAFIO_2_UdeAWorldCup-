// EstPartidoJugador.cpp
#include "EstPartidoJugador.h"
#include <iostream>
using namespace std;

EstPartidoJugador::EstPartidoJugador()
    : numeroCamiseta(0), goles(0), tarjetasAmarillas(0),
    tarjetasRojas(0), faltas(0), minutosJugados(90) {}

EstPartidoJugador::EstPartidoJugador(int camiseta)
    : numeroCamiseta(camiseta), goles(0), tarjetasAmarillas(0),
    tarjetasRojas(0), faltas(0), minutosJugados(90) {}

EstPartidoJugador::EstPartidoJugador(const EstPartidoJugador& otro)
    : numeroCamiseta(otro.numeroCamiseta),
    goles(otro.goles),
    tarjetasAmarillas(otro.tarjetasAmarillas),
    tarjetasRojas(otro.tarjetasRojas),
    faltas(otro.faltas),
    minutosJugados(otro.minutosJugados) {}

int EstPartidoJugador::getNumeroCamiseta()    const { return numeroCamiseta; }
int EstPartidoJugador::getGoles()             const { return goles; }
int EstPartidoJugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstPartidoJugador::getTarjetasRojas()     const { return tarjetasRojas; }
int EstPartidoJugador::getFaltas()            const { return faltas; }
int EstPartidoJugador::getMinutosJugados()    const { return minutosJugados; }

void EstPartidoJugador::setGoles(int g)              { goles = g; }
void EstPartidoJugador::setTarjetasAmarillas(int t)  { tarjetasAmarillas = t; }
void EstPartidoJugador::setTarjetasRojas(int t)      { tarjetasRojas = t; }
void EstPartidoJugador::setFaltas(int f)             { faltas = f; }
void EstPartidoJugador::setMinutosJugados(int m)     { minutosJugados = m; }

void EstPartidoJugador::imprimir() const {
    cout << "Camiseta:" << numeroCamiseta
         << " G:"   << goles
         << " TA:"  << tarjetasAmarillas
         << " TR:"  << tarjetasRojas
         << " F:"   << faltas
         << " MIN:" << minutosJugados;
}
