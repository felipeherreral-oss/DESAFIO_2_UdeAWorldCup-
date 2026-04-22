#include "EstHistJugador.h"
#include <iostream>
using namespace std;

EstHistJugador::EstHistJugador()
    : partidosJugados(0), goles(0), minutosJugados(0),
    asistencias(0), tarjetasAmarillas(0), tarjetasRojas(0), faltas(0) {}

EstHistJugador::EstHistJugador(const EstHistJugador& otro)
    : partidosJugados(otro.partidosJugados),
    goles(otro.goles),
    minutosJugados(otro.minutosJugados),
    asistencias(otro.asistencias),
    tarjetasAmarillas(otro.tarjetasAmarillas),
    tarjetasRojas(otro.tarjetasRojas),
    faltas(otro.faltas) {}

int EstHistJugador::getPartidosJugados()   const { return partidosJugados; }
int EstHistJugador::getGoles()             const { return goles; }
int EstHistJugador::getMinutosJugados()    const { return minutosJugados; }
int EstHistJugador::getAsistencias()       const { return asistencias; }
int EstHistJugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstHistJugador::getTarjetasRojas()     const { return tarjetasRojas; }
int EstHistJugador::getFaltas()            const { return faltas; }

void EstHistJugador::setGoles(int g)          { goles = g; }
void EstHistJugador::setPartidosJugados(int p){ partidosJugados = p; }
void EstHistJugador::setMinutosJugados(int m) { minutosJugados = m; }

void EstHistJugador::actualizarConPartido(int golesP, int minutosP,
                                          int amarillasP, int rojasP,
                                          int faltasP, int asistenciasP) {
    partidosJugados   += 1;
    goles             += golesP;
    minutosJugados    += minutosP;
    asistencias       += asistenciasP;
    tarjetasAmarillas += amarillasP;
    tarjetasRojas     += rojasP;
    faltas            += faltasP;
}

void EstHistJugador::imprimir() const {
    cout << "PJ:"  << partidosJugados
         << " G:"  << goles
         << " MIN:"<< minutosJugados
         << " ASI:"<< asistencias
         << " TA:" << tarjetasAmarillas
         << " TR:" << tarjetasRojas
         << " F:"  << faltas;
}
