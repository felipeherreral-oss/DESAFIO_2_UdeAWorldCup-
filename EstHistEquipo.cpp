#include "EstHistEquipo.h"
#include <iostream>
using namespace std;

EstHistEquipo::EstHistEquipo()
    : golesAFavor(0), golesEnContra(0), partidosJugados(0),
    partidosGanados(0), partidosEmpatados(0), partidosPerdidos(0),
    tarjetasAmarillas(0), tarjetasRojas(0), faltas(0) {}

EstHistEquipo::EstHistEquipo(const EstHistEquipo& otro)
    : golesAFavor(otro.golesAFavor),
    golesEnContra(otro.golesEnContra),
    partidosJugados(otro.partidosJugados),
    partidosGanados(otro.partidosGanados),
    partidosEmpatados(otro.partidosEmpatados),
    partidosPerdidos(otro.partidosPerdidos),
    tarjetasAmarillas(otro.tarjetasAmarillas),
    tarjetasRojas(otro.tarjetasRojas),
    faltas(otro.faltas) {}

// Getters
int EstHistEquipo::getGolesAFavor()       const { return golesAFavor; }
int EstHistEquipo::getGolesEnContra()     const { return golesEnContra; }
int EstHistEquipo::getPartidosJugados()   const { return partidosJugados; }
int EstHistEquipo::getPartidosGanados()   const { return partidosGanados; }
int EstHistEquipo::getPartidosEmpatados() const { return partidosEmpatados; }
int EstHistEquipo::getPartidosPerdidos()  const { return partidosPerdidos; }
int EstHistEquipo::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int EstHistEquipo::getTarjetasRojas()     const { return tarjetasRojas; }
int EstHistEquipo::getFaltas()            const { return faltas; }

// Setters
void EstHistEquipo::setGolesAFavor(int g)      { golesAFavor = g; }
void EstHistEquipo::setGolesEnContra(int g)    { golesEnContra = g; }
void EstHistEquipo::setPartidosJugados(int p)  { partidosJugados = p; }
void EstHistEquipo::setPartidosGanados(int p)  { partidosGanados = p; }
void EstHistEquipo::setPartidosEmpatados(int p){ partidosEmpatados = p; }
void EstHistEquipo::setPartidosPerdidos(int p) { partidosPerdidos = p; }

// Promedios — si no hay partidos jugados retorna 0 para evitar division por cero
double EstHistEquipo::getPromedioGolesAFavor() const {
    if (partidosJugados == 0) return 0.0;
    return (double)golesAFavor / partidosJugados;
}

double EstHistEquipo::getPromedioGolesEnContra() const {
    if (partidosJugados == 0) return 0.0;
    return (double)golesEnContra / partidosJugados;
}

void EstHistEquipo::actualizarConPartido(int gf, int gc,
                                         bool gano, bool empato,
                                         int amarillas, int rojas,
                                         int faltasP) {
    golesAFavor      += gf;
    golesEnContra    += gc;
    partidosJugados  += 1;
    tarjetasAmarillas+= amarillas;
    tarjetasRojas    += rojas;
    faltas           += faltasP;

    if (gano)        partidosGanados++;
    else if (empato) partidosEmpatados++;
    else             partidosPerdidos++;
}

void EstHistEquipo::imprimir() const {
    cout << "PJ:" << partidosJugados
         << " PG:" << partidosGanados
         << " PE:" << partidosEmpatados
         << " PP:" << partidosPerdidos
         << " GF:" << golesAFavor
         << " GC:" << golesEnContra
         << " DG:" << (golesAFavor - golesEnContra)
         << " TA:" << tarjetasAmarillas
         << " TR:" << tarjetasRojas
         << " F:"  << faltas;
}
