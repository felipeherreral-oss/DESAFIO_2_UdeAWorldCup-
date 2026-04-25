#include "Grupo.h"
#include <iostream>
#include <cstdlib>
using namespace std;

// ─── EntradaTabla ──────────────────────────────────────────────────

EntradaTabla::EntradaTabla()
    : equipo(nullptr), puntos(0), golesAFavor(0),
    golesEnContra(0), partidosJugados(0),
    partidosGanados(0), partidosEmpatados(0), partidosPerdidos(0) {}

EntradaTabla::EntradaTabla(Equipo* eq)
    : equipo(eq), puntos(0), golesAFavor(0),
    golesEnContra(0), partidosJugados(0),
    partidosGanados(0), partidosEmpatados(0), partidosPerdidos(0) {}

int EntradaTabla::getDiferenciaGoles() const {
    return golesAFavor - golesEnContra;
}

// ─── Grupo ─────────────────────────────────────────────────────────

Grupo::Grupo()
    : identificador('?'), equipos(4), partidos(6), tabla(4) {}

Grupo::Grupo(char id)
    : identificador(id), equipos(4), partidos(6), tabla(4) {}

void Grupo::agregarEquipo(Equipo* eq) {
    equipos.agregar(eq);
    tabla.agregar(EntradaTabla(eq));
}

char Grupo::getIdentificador() const { return identificador; }

ArregloDinamico<Equipo*>&      Grupo::getEquipos()  { return equipos; }
ArregloDinamico<Partido>&      Grupo::getPartidos() { return partidos; }
ArregloDinamico<EntradaTabla>& Grupo::getTabla()    { return tabla; }

const ArregloDinamico<Equipo*>& Grupo::getEquipos() const {
    return equipos;
}

const ArregloDinamico<Partido>& Grupo::getPartidos() const {
    return partidos;
}

const ArregloDinamico<EntradaTabla>& Grupo::getTabla() const {
    return tabla;
}

// ─── Generar partidos con restricciones de fecha ───────────────────

void Grupo::generarPartidos(const Fecha& fechaInicio,
                            int* partidosPorDia,
                            int maxDias) {
    // Parejas round-robin: (0,1),(0,2),(0,3),(1,2),(1,3),(2,3)
    int pares[6][2] = {
        {0,1},{0,2},{0,3},
        {1,2},{1,3},{2,3}
    };

    // Último día jugado por cada equipo (índice desde fechaInicio)
    int ultimoDiaEquipo[4] = {-99, -99, -99, -99};

    for (int p = 0; p < 6; p++) {
        int idxA = pares[p][0];
        int idxB = pares[p][1];

        // Buscar el primer día válido para este partido
        int diaValido = -1;
        for (int d = 0; d < maxDias; d++) {
            // Restricción 1: max 4 partidos por día (global)
            if (partidosPorDia[d] >= 4) continue;

            // Restricción 2: ningún equipo juega con menos de 3 días
            bool equipoAlibre = (d - ultimoDiaEquipo[idxA]) >= 3;
            bool equipoBlibre = (d - ultimoDiaEquipo[idxB]) >= 3;

            if (equipoAlibre && equipoBlibre) {
                diaValido = d;
                break;
            }
        }

        if (diaValido == -1) diaValido = maxDias - 1; // fallback

        Fecha fechaPartido = fechaInicio + diaValido;

        Partido partido(
            fechaPartido,
            "nombreSede",
            "codArbitro1", "codArbitro2", "codArbitro3",
            equipos[idxA],
            equipos[idxB]
            );

        partidos.agregar(partido);
        partidosPorDia[diaValido]++;
        ultimoDiaEquipo[idxA] = diaValido;
        ultimoDiaEquipo[idxB] = diaValido;
    }
}

// ─── Simular todos los partidos ────────────────────────────────────

void Grupo::simularPartidos() {
    for (int i = 0; i < partidos.getTamanio(); i++) {
        partidos[i].simular();
        partidos[i].actualizarHistoricos();
    }
}

// ─── Construir tabla de clasificación ─────────────────────────────

void Grupo::construirTabla() {
    // Reiniciar tabla
    for (int i = 0; i < tabla.getTamanio(); i++) {
        tabla[i].puntos          = 0;
        tabla[i].golesAFavor     = 0;
        tabla[i].golesEnContra   = 0;
        tabla[i].partidosJugados = 0;
        tabla[i].partidosGanados = 0;
        tabla[i].partidosEmpatados = 0;
        tabla[i].partidosPerdidos  = 0;
    }

    // Acumular resultados de cada partido
    for (int p = 0; p < partidos.getTamanio(); p++) {
        Partido& partido = partidos[p];
        if (!partido.getFueJugado()) continue;

        Equipo* eq1 = partido.getEquipo1();
        Equipo* eq2 = partido.getEquipo2();
        int     g1  = partido.getStatsEq1().getGolesAFavor();
        int     g2  = partido.getStatsEq2().getGolesAFavor();

        // Buscar en la tabla
        for (int i = 0; i < tabla.getTamanio(); i++) {
            if (tabla[i].equipo == eq1) {
                tabla[i].golesAFavor   += g1;
                tabla[i].golesEnContra += g2;
                tabla[i].partidosJugados++;
                if (g1 > g2) { tabla[i].puntos += 3; tabla[i].partidosGanados++; }
                else if (g1 == g2) { tabla[i].puntos += 1; tabla[i].partidosEmpatados++; }
                else tabla[i].partidosPerdidos++;
            }
            if (tabla[i].equipo == eq2) {
                tabla[i].golesAFavor   += g2;
                tabla[i].golesEnContra += g1;
                tabla[i].partidosJugados++;
                if (g2 > g1) { tabla[i].puntos += 3; tabla[i].partidosGanados++; }
                else if (g1 == g2) { tabla[i].puntos += 1; tabla[i].partidosEmpatados++; }
                else tabla[i].partidosPerdidos++;
            }
        }
    }

    ordenarTabla();
}

// ─── Ordenar tabla ─────────────────────────────────────────────────

bool Grupo::esMejorQue(const EntradaTabla& a, const EntradaTabla& b) const {
    if (a.puntos != b.puntos)
        return a.puntos > b.puntos;
    if (a.getDiferenciaGoles() != b.getDiferenciaGoles())
        return a.getDiferenciaGoles() > b.getDiferenciaGoles();
    if (a.golesAFavor != b.golesAFavor)
        return a.golesAFavor > b.golesAFavor;
    // Sorteo aleatorio
    return (rand() % 2) == 0;
}

void Grupo::ordenarTabla() {
    // Burbuja simple
    for (int i = 0; i < tabla.getTamanio() - 1; i++)
        for (int j = 0; j < tabla.getTamanio() - i - 1; j++)
            if (!esMejorQue(tabla[j], tabla[j+1])) {
                EntradaTabla temp = tabla[j];
                tabla[j]         = tabla[j+1];
                tabla[j+1]       = temp;
            }
}

// ─── Getters de posiciones ─────────────────────────────────────────

Equipo* Grupo::getPrimero()  { construirTabla(); return tabla[0].equipo; }
Equipo* Grupo::getSegundo()  { construirTabla(); return tabla[1].equipo; }
Equipo* Grupo::getTercero()  { construirTabla(); return tabla[2].equipo; }
Equipo* Grupo::getCuarto()   { construirTabla(); return tabla[3].equipo; }

EntradaTabla& Grupo::getEntradaPrimero()  { return tabla[0]; }
EntradaTabla& Grupo::getEntradaSegundo()  { return tabla[1]; }
EntradaTabla& Grupo::getEntradaTercero()  { return tabla[2]; }

// ─── Impresión ─────────────────────────────────────────────────────

void Grupo::imprimirTabla() const {
    cout << "\nGRUPO " << identificador << endl;
    cout << "Equipo              PJ PG PE PP GF GC DG PTS" << endl;
    cout << "------------------------------------------------" << endl;
    for (int i = 0; i < tabla.getTamanio(); i++) {
        const EntradaTabla& e = tabla[i];
        cout << e.equipo->getPais();
        // Padding simple
        int len = 0;
        const char* n = e.equipo->getPais();
        while (n[len]) len++;
        for (int s = len; s < 20; s++) cout << " ";
        cout << e.partidosJugados    << "  "
             << e.partidosGanados    << "  "
             << e.partidosEmpatados  << "  "
             << e.partidosPerdidos   << "  "
             << e.golesAFavor        << "  "
             << e.golesEnContra      << "  "
             << e.getDiferenciaGoles()<< "  "
             << e.puntos             << endl;
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
