// EstPartidoEquipo.cpp
#include "EstPartidoEquipo.h"
#include <iostream>
using namespace std;

EstPartidoEquipo::EstPartidoEquipo()
    : golesAFavor(0), golesEnContra(0),
    posesionBalon(50.0), jugadoresConvocados(11) {}

EstPartidoEquipo::EstPartidoEquipo(const EstPartidoEquipo& otro)
    : golesAFavor(otro.golesAFavor),
    golesEnContra(otro.golesEnContra),
    posesionBalon(otro.posesionBalon),
    jugadoresConvocados(otro.jugadoresConvocados) {}

int    EstPartidoEquipo::getGolesAFavor()   const { return golesAFavor; }
int    EstPartidoEquipo::getGolesEnContra() const { return golesEnContra; }
double EstPartidoEquipo::getPosesion()      const { return posesionBalon; }

void EstPartidoEquipo::setGolesAFavor(int g)   { golesAFavor = g; }
void EstPartidoEquipo::setGolesEnContra(int g) { golesEnContra = g; }
void EstPartidoEquipo::setPosesion(double p)   { posesionBalon = p; }

void EstPartidoEquipo::agregarJugador(const EstPartidoJugador& j) {
    jugadoresConvocados.agregar(j);
}

ArregloDinamico<EstPartidoJugador>& EstPartidoEquipo::getJugadores() {
    return jugadoresConvocados;
}

void EstPartidoEquipo::imprimir() const {
    cout << "GF:" << golesAFavor
         << " GC:" << golesEnContra
         << " Posesion:" << posesionBalon << "%" << endl;
    for (int i = 0; i < jugadoresConvocados.getTamanio(); i++) {
        cout << "  ";
        jugadoresConvocados[i].imprimir();
        cout << endl;
    }
}
