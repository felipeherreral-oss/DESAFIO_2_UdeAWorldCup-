#include "Grupo.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// ─── Constructores ─────────────────────────────────────────────────

Grupo::Grupo()
    : identificador('?'), equipos(4), partidos(6) {
    for (int i = 0; i < 4; i++) {
        puntos[i] = golesAFavor[i] = golesEnContra[i] = 0;
        partidosJugados[i] = partidosGanados[i] = 0;
        partidosEmpatados[i] = partidosPerdidos[i] = 0;
        ordenTabla[i] = i;
    }
}

Grupo::Grupo(char id)
    : identificador(id), equipos(4), partidos(6) {
    for (int i = 0; i < 4; i++) {
        puntos[i] = golesAFavor[i] = golesEnContra[i] = 0;
        partidosJugados[i] = partidosGanados[i] = 0;
        partidosEmpatados[i] = partidosPerdidos[i] = 0;
        ordenTabla[i] = i;
    }
}

Grupo::Grupo(const Grupo& otro)
    : identificador(otro.identificador),
    equipos(otro.equipos),
    partidos(otro.partidos) {
    for (int i = 0; i < 4; i++) {
        puntos[i]           = otro.puntos[i];
        golesAFavor[i]      = otro.golesAFavor[i];
        golesEnContra[i]    = otro.golesEnContra[i];
        partidosJugados[i]  = otro.partidosJugados[i];
        partidosGanados[i]  = otro.partidosGanados[i];
        partidosEmpatados[i]= otro.partidosEmpatados[i];
        partidosPerdidos[i] = otro.partidosPerdidos[i];
        ordenTabla[i]       = otro.ordenTabla[i];
    }
}

Grupo& Grupo::operator=(const Grupo& otro) {
    if (this != &otro) {
        identificador = otro.identificador;
        equipos       = otro.equipos;
        partidos      = otro.partidos;
        for (int i = 0; i < 4; i++) {
            puntos[i]           = otro.puntos[i];
            golesAFavor[i]      = otro.golesAFavor[i];
            golesEnContra[i]    = otro.golesEnContra[i];
            partidosJugados[i]  = otro.partidosJugados[i];
            partidosGanados[i]  = otro.partidosGanados[i];
            partidosEmpatados[i]= otro.partidosEmpatados[i];
            partidosPerdidos[i] = otro.partidosPerdidos[i];
            ordenTabla[i]       = otro.ordenTabla[i];
        }
    }
    return *this;
}

// ─── Utilidades privadas ───────────────────────────────────────────

void Grupo::reiniciarTabla() {
    for (int i = 0; i < 4; i++) {
        puntos[i] = golesAFavor[i] = golesEnContra[i] = 0;
        partidosJugados[i] = partidosGanados[i] = 0;
        partidosEmpatados[i] = partidosPerdidos[i] = 0;
        ordenTabla[i] = i;
    }
}

int Grupo::getDiferenciaGoles(int idx) const {
    return golesAFavor[idx] - golesEnContra[idx];
}

bool Grupo::esMejorQue(int idxA, int idxB) const {
    if (puntos[idxA] != puntos[idxB])
        return puntos[idxA] > puntos[idxB];
    if (getDiferenciaGoles(idxA) != getDiferenciaGoles(idxB))
        return getDiferenciaGoles(idxA) > getDiferenciaGoles(idxB);
    if (golesAFavor[idxA] != golesAFavor[idxB])
        return golesAFavor[idxA] > golesAFavor[idxB];
    return (rand() % 2) == 0; // sorteo
}

void Grupo::ordenarTabla() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3 - i; j++)
            if (!esMejorQue(ordenTabla[j], ordenTabla[j+1])) {
                int tmp          = ordenTabla[j];
                ordenTabla[j]    = ordenTabla[j+1];
                ordenTabla[j+1]  = tmp;
            }
}

// ─── Gestión de equipos ────────────────────────────────────────────

void Grupo::agregarEquipo(Equipo* eq) {
    equipos.agregar(eq);
}

// ─── Generación de partidos ────────────────────────────────────────
void Grupo::generarPartidos(int diaInicio, int mesInicio, int anioInicio,
                            int* partidosPorDia, int maxDias) {
    int pares[6][2] = {
        {0,1},{0,2},{0,3},
        {1,2},{1,3},{2,3}
    };
    int ultimoDiaEquipo[4] = {-99,-99,-99,-99};

    for (int p = 0; p < 6; p++) {
        int idxA = pares[p][0];
        int idxB = pares[p][1];
        int diaValido = -1;

        for (int d = 0; d < maxDias; d++) {
            if (partidosPorDia[d] >= 4) continue;
            bool aLibre = (d - ultimoDiaEquipo[idxA]) >= 3;
            bool bLibre = (d - ultimoDiaEquipo[idxB]) >= 3;
            if (aLibre && bLibre) { diaValido = d; break; }
        }

        if (diaValido == -1) diaValido = maxDias - 1;

        // Calcular fecha sumando días al inicio
        int d = diaInicio, m = mesInicio, a = anioInicio;
        sumarDias(d, m, a, diaValido);

        Partido partido(d, m, a,
                        "nombreSede",
                        "codArbitro1","codArbitro2","codArbitro3",
                        equipos[idxA], equipos[idxB]);

        partidos.agregar(partido);
        partidosPorDia[diaValido]++;
        ultimoDiaEquipo[idxA] = diaValido;
        ultimoDiaEquipo[idxB] = diaValido;
    }
}

// ─── Simulación ────────────────────────────────────────────────────

void Grupo::simularPartidos() {
    for (int i = 0; i < partidos.getTamanio(); i++) {
        partidos[i].simular();
        partidos[i].actualizarHistoricos();
    }
}

// ─── Tabla de clasificación ────────────────────────────────────────

void Grupo::construirTabla() {
    reiniciarTabla();

    for (int p = 0; p < partidos.getTamanio(); p++) {
        Partido& partido = partidos[p];
        if (!partido.getFueJugado()) continue;

        Equipo* eq1 = partido.getEquipo1();
        Equipo* eq2 = partido.getEquipo2();
        int     g1  = partido.getStatsEq1().getGolesAFavor();
        int     g2  = partido.getStatsEq2().getGolesAFavor();

        // Encontrar índice de cada equipo en el arreglo
        int i1 = -1, i2 = -1;
        for (int i = 0; i < equipos.getTamanio(); i++) {
            if (equipos[i] == eq1) i1 = i;
            if (equipos[i] == eq2) i2 = i;
        }
        if (i1 < 0 || i2 < 0) continue;

        // Actualizar estadísticas equipo 1
        golesAFavor[i1]   += g1;
        golesEnContra[i1] += g2;
        partidosJugados[i1]++;
        if (g1 > g2) { puntos[i1] += 3; partidosGanados[i1]++; }
        else if (g1 == g2) { puntos[i1]++; partidosEmpatados[i1]++; }
        else partidosPerdidos[i1]++;

        // Actualizar estadísticas equipo 2
        golesAFavor[i2]   += g2;
        golesEnContra[i2] += g1;
        partidosJugados[i2]++;
        if (g2 > g1) { puntos[i2] += 3; partidosGanados[i2]++; }
        else if (g1 == g2) { puntos[i2]++; partidosEmpatados[i2]++; }
        else partidosPerdidos[i2]++;
    }

    ordenarTabla();
}

// ─── Getters posiciones ────────────────────────────────────────────

char Grupo::getIdentificador() const { return identificador; }

ArregloDinamico<Equipo*>& Grupo::getEquipos() { return equipos; }
const ArregloDinamico<Equipo*>& Grupo::getEquipos() const { return equipos; }
ArregloDinamico<Partido>& Grupo::getPartidos() { return partidos; }
const ArregloDinamico<Partido>& Grupo::getPartidos() const { return partidos; }

Equipo* Grupo::getPrimero() { construirTabla(); return equipos[ordenTabla[0]]; }
Equipo* Grupo::getSegundo() { construirTabla(); return equipos[ordenTabla[1]]; }
Equipo* Grupo::getTercero() { construirTabla(); return equipos[ordenTabla[2]]; }
Equipo* Grupo::getCuarto()  { construirTabla(); return equipos[ordenTabla[3]]; }

int Grupo::getPuntosPorPosicion(int pos) const {
    return puntos[ordenTabla[pos]];
}
int Grupo::getGolesAFavorPorPosicion(int pos) const {
    return golesAFavor[ordenTabla[pos]];
}
int Grupo::getGolesEnContraPorPosicion(int pos) const {
    return golesEnContra[ordenTabla[pos]];
}
int Grupo::getDiferenciaGolesPorPosicion(int pos) const {
    return getDiferenciaGoles(ordenTabla[pos]);
}
int Grupo::getPartidosJugadosPorPosicion(int pos) const {
    return partidosJugados[ordenTabla[pos]];
}
int Grupo::getPartidosGanadosPorPosicion(int pos) const {
    return partidosGanados[ordenTabla[pos]];
}
int Grupo::getPartidosEmpatadosPorPosicion(int pos) const {
    return partidosEmpatados[ordenTabla[pos]];
}
int Grupo::getPartidosPerdidosPorPosicion(int pos) const {
    return partidosPerdidos[ordenTabla[pos]];
}

// ─── Impresión ─────────────────────────────────────────────────────

void Grupo::imprimirTabla() const {
    cout << "\nGRUPO " << identificador << endl;
    cout << "Equipo              PJ PG PE PP GF GC DG PTS" << endl;
    cout << "------------------------------------------------" << endl;
    for (int i = 0; i < 4 && i < equipos.getTamanio(); i++) {
        int idx = ordenTabla[i];
        const char* nombre = equipos[idx]->getPais();
        cout << nombre;
        int len = 0;
        while (nombre[len]) len++;
        for (int s = len; s < 20; s++) cout << " ";
        cout << partidosJugados[idx]   << "  "
             << partidosGanados[idx]   << "  "
             << partidosEmpatados[idx] << "  "
             << partidosPerdidos[idx]  << "  "
             << golesAFavor[idx]       << "  "
             << golesEnContra[idx]     << "  "
             << getDiferenciaGoles(idx)<< "  "
             << puntos[idx]            << endl;
    }
}

void Grupo::imprimirPartidos() const {
    cout << "\nPartidos Grupo " << identificador << ":" << endl;
    for (int i = 0; i < partidos.getTamanio(); i++) {
        partidos[i].imprimir();
        partidos[i].imprimirGoleadores();
        cout << "---" << endl;
    }

}

void Grupo::sumarDias(int& d, int& m, int& a, int dias) const {
    int diasPorMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    d += dias;
    while (d > diasPorMes[m]) {
        d -= diasPorMes[m];
        m++;
        if (m > 12) { m = 1; a++; }
    }
}
