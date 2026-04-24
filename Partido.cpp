#include "Partido.h"
#include <iostream>
#include <cmath>    // pow — permitido
#include <cstdlib>  // rand, srand — permitido por enunciado
using namespace std;

// ─── Constantes de la fórmula ──────────────────────────────────────
static const double MU    = 1.35;
static const double ALPHA = 0.60;
static const double BETA  = 0.40;

// ─── Utilidades privadas ───────────────────────────────────────────

int Partido::longitud(const char* src) const {
    if (!src) return 0;
    int i = 0;
    while (src[i] != '\0') i++;
    return i;
}

char* Partido::copiarCadena(const char* src) {
    if (!src) return nullptr;
    int len = longitud(src);
    char* dst = new char[len + 1];
    for (int i = 0; i <= len; i++)
        dst[i] = src[i];
    return dst;
}

// ─── Constructores ─────────────────────────────────────────────────

Partido::Partido()
    : fecha(1,1,2026), sede(nullptr),
    codArbitro1(nullptr), codArbitro2(nullptr), codArbitro3(nullptr),
    equipo1(nullptr), equipo2(nullptr),
    statsEq1(), statsEq2(),
    fueJugado(false), fueAProrrgoa(false), ganador(nullptr) {
    sede        = copiarCadena("sinSede");
    codArbitro1 = copiarCadena("arb1");
    codArbitro2 = copiarCadena("arb2");
    codArbitro3 = copiarCadena("arb3");
}

Partido::Partido(Fecha f, const char* s,
                 const char* arb1, const char* arb2, const char* arb3,
                 Equipo* eq1, Equipo* eq2)
    : fecha(f), sede(nullptr),
    codArbitro1(nullptr), codArbitro2(nullptr), codArbitro3(nullptr),
    equipo1(eq1), equipo2(eq2),
    statsEq1(), statsEq2(),
    fueJugado(false), fueAProrrgoa(false), ganador(nullptr) {
    sede        = copiarCadena(s);
    codArbitro1 = copiarCadena(arb1);
    codArbitro2 = copiarCadena(arb2);
    codArbitro3 = copiarCadena(arb3);
}

Partido::Partido(const Partido& otro)
    : fecha(otro.fecha), sede(nullptr),
    codArbitro1(nullptr), codArbitro2(nullptr), codArbitro3(nullptr),
    equipo1(otro.equipo1), equipo2(otro.equipo2),
    statsEq1(otro.statsEq1), statsEq2(otro.statsEq2),
    fueJugado(otro.fueJugado),
    fueAProrrgoa(otro.fueAProrrgoa),
    ganador(otro.ganador) {
    sede        = copiarCadena(otro.sede);
    codArbitro1 = copiarCadena(otro.codArbitro1);
    codArbitro2 = copiarCadena(otro.codArbitro2);
    codArbitro3 = copiarCadena(otro.codArbitro3);
}

Partido::~Partido() {
    delete[] sede;
    delete[] codArbitro1;
    delete[] codArbitro2;
    delete[] codArbitro3;
    // NO delete equipo1/equipo2 — son punteros prestados
}

Partido& Partido::operator=(const Partido& otro) {
    if (this != &otro) {
        delete[] sede;
        delete[] codArbitro1;
        delete[] codArbitro2;
        delete[] codArbitro3;

        fecha        = otro.fecha;
        sede         = copiarCadena(otro.sede);
        codArbitro1  = copiarCadena(otro.codArbitro1);
        codArbitro2  = copiarCadena(otro.codArbitro2);
        codArbitro3  = copiarCadena(otro.codArbitro3);
        equipo1      = otro.equipo1;
        equipo2      = otro.equipo2;
        statsEq1     = otro.statsEq1;
        statsEq2     = otro.statsEq2;
        fueJugado    = otro.fueJugado;
        fueAProrrgoa = otro.fueAProrrgoa;
        ganador      = otro.ganador;
    }
    return *this;
}

// ─── Getters ───────────────────────────────────────────────────────

Fecha       Partido::getFecha()       const { return fecha; }
const char* Partido::getSede()        const { return sede; }
Equipo*     Partido::getEquipo1()     const { return equipo1; }
Equipo*     Partido::getEquipo2()     const { return equipo2; }
Equipo*     Partido::getGanador()     const { return ganador; }
bool        Partido::getFueJugado()   const { return fueJugado; }
bool        Partido::getFueAProrroga()const { return fueAProrrgoa; }

EstPartidoEquipo& Partido::getStatsEq1() { return statsEq1; }
EstPartidoEquipo& Partido::getStatsEq2() { return statsEq2; }

// ─── Lógica de simulación ──────────────────────────────────────────

double Partido::calcularLambda(double gfa, double gcb) const {
    // Evitar divisiones por cero
    if (gfa <= 0) gfa = 0.5;
    if (gcb <= 0) gcb = 0.5;
    return MU * pow(gfa / MU, ALPHA) * pow(gcb / MU, BETA);
}

void Partido::seleccionarConvocados(Equipo* eq, EstPartidoEquipo& stats) {
    int plantillaSize = eq->getPlantilla().getTamanio();
    bool seleccionado[26] = {false};
    int  count = 0;

    while (count < 11) {
        int idx = rand() % plantillaSize;
        if (!seleccionado[idx]) {
            seleccionado[idx] = true;
            EstPartidoJugador epj(eq->getPlantilla()[idx].getNumeroCamiseta());
            stats.agregarJugador(epj);
            count++;
        }
    }
}

void Partido::simularEquipo(Equipo* eq, EstPartidoEquipo& stats,
                            double lambda, bool prorroga) {
    int golesAcumulados = 0;
    int golesEsperados  = (int)(lambda + 0.5); // redondeo simple

    // Simular cada jugador convocado
    for (int i = 0; i < stats.getJugadores().getTamanio(); i++) {
        EstPartidoJugador& epj = stats.getJugadores()[i];

        // Minutos jugados
        epj.setMinutosJugados(prorroga ? 120 : 90);

        // Tarjetas amarillas
        int amarillas = 0;
        if ((rand() % 10000) < 600)  { // 6%
            amarillas++;
            epj.setTarjetasAmarillas(1);
            if ((rand() % 10000) < 115) { // 1.15% segunda amarilla
                amarillas++;
                epj.setTarjetasAmarillas(2);
                epj.setTarjetasRojas(1); // doble amarilla = roja
            }
        }

        // Faltas
        int faltas = 0;
        if ((rand() % 10000) < 1300) { // 13%
            faltas++;
            if ((rand() % 10000) < 275) { // 2.75%
                faltas++;
                if ((rand() % 10000) < 70) { // 0.7%
                    faltas++;
                }
            }
        }
        epj.setFaltas(faltas);

        // Goles — 4% por jugador hasta alcanzar lambda
        if (golesAcumulados < golesEsperados) {
            if ((rand() % 100) < 4) {
                epj.setGoles(1);
                golesAcumulados++;
            }
        }
    }

    stats.setGolesAFavor(golesAcumulados);
}

void Partido::calcularPosesion() {
    int r1 = equipo1->getRankingFIFA();
    int r2 = equipo2->getRankingFIFA();
    double total = r1 + r2;
    // Menor ranking = mejor equipo = más posesión
    double posEq1 = (r2 / total) * 100.0;
    double posEq2 = 100.0 - posEq1;
    statsEq1.setPosesion(posEq1);
    statsEq2.setPosesion(posEq2);
}

void Partido::simular() {
    if (fueJugado) return;

    // 1. Seleccionar 11 convocados por equipo
    seleccionarConvocados(equipo1, statsEq1);
    seleccionarConvocados(equipo2, statsEq2);

    // 2. Calcular lambda de cada equipo
    double lambdaEq1 = calcularLambda(
        equipo1->getPromedioGolesAFavor(),
        equipo2->getPromedioGolesEnContra()
        );
    double lambdaEq2 = calcularLambda(
        equipo2->getPromedioGolesAFavor(),
        equipo1->getPromedioGolesEnContra()
        );

    // 3. Simular estadísticas de cada equipo
    simularEquipo(equipo1, statsEq1, lambdaEq1, false);
    simularEquipo(equipo2, statsEq2, lambdaEq2, false);

    // 4. Cruzar goles en contra
    statsEq1.setGolesEnContra(statsEq2.getGolesAFavor());
    statsEq2.setGolesEnContra(statsEq1.getGolesAFavor());

    // 5. Posesión
    calcularPosesion();

    // 6. Determinar ganador
    int g1 = statsEq1.getGolesAFavor();
    int g2 = statsEq2.getGolesAFavor();

    if (g1 > g2)      ganador = equipo1;
    else if (g2 > g1) ganador = equipo2;
    else {
        // Empate en fase de grupos — queda sin ganador
        ganador = nullptr;
    }

    fueJugado = true;
}

void Partido::actualizarHistoricos() {
    if (!fueJugado) return;

    int g1 = statsEq1.getGolesAFavor();
    int g2 = statsEq2.getGolesAFavor();

    bool eq1Gano   = (g1 > g2);
    bool eq2Gano   = (g2 > g1);
    bool empate    = (g1 == g2);

    // Contar tarjetas y faltas del equipo
    int ama1=0, roj1=0, fal1=0;
    int ama2=0, roj2=0, fal2=0;

    for (int i = 0; i < statsEq1.getJugadores().getTamanio(); i++) {
        ama1 += statsEq1.getJugadores()[i].getTarjetasAmarillas();
        roj1 += statsEq1.getJugadores()[i].getTarjetasRojas();
        fal1 += statsEq1.getJugadores()[i].getFaltas();
    }
    for (int i = 0; i < statsEq2.getJugadores().getTamanio(); i++) {
        ama2 += statsEq2.getJugadores()[i].getTarjetasAmarillas();
        roj2 += statsEq2.getJugadores()[i].getTarjetasRojas();
        fal2 += statsEq2.getJugadores()[i].getFaltas();
    }

    // Actualizar históricas de equipos
    equipo1->getEstHistorica().actualizarConPartido(
        g1, g2, eq1Gano, empate, ama1, roj1, fal1);
    equipo2->getEstHistorica().actualizarConPartido(
        g2, g1, eq2Gano, empate, ama2, roj2, fal2);

    // Actualizar históricas de jugadores convocados
    for (int i = 0; i < statsEq1.getJugadores().getTamanio(); i++) {
        EstPartidoJugador& epj = statsEq1.getJugadores()[i];
        int camiseta = epj.getNumeroCamiseta();
        for (int j = 0; j < equipo1->getPlantilla().getTamanio(); j++) {
            if (equipo1->getPlantilla()[j].getNumeroCamiseta() == camiseta) {
                equipo1->getPlantilla()[j].getEstHistorica().actualizarConPartido(
                    epj.getGoles(), epj.getMinutosJugados(),
                    epj.getTarjetasAmarillas(), epj.getTarjetasRojas(),
                    epj.getFaltas(), 0);
                break;
            }
        }
    }
    for (int i = 0; i < statsEq2.getJugadores().getTamanio(); i++) {
        EstPartidoJugador& epj = statsEq2.getJugadores()[i];
        int camiseta = epj.getNumeroCamiseta();
        for (int j = 0; j < equipo2->getPlantilla().getTamanio(); j++) {
            if (equipo2->getPlantilla()[j].getNumeroCamiseta() == camiseta) {
                equipo2->getPlantilla()[j].getEstHistorica().actualizarConPartido(
                    epj.getGoles(), epj.getMinutosJugados(),
                    epj.getTarjetasAmarillas(), epj.getTarjetasRojas(),
                    epj.getFaltas(), 0);
                break;
            }
        }
    }
}

void Partido::setGanador(Equipo* g) {
    ganador = g;
}

// ─── Impresión ─────────────────────────────────────────────────────

void Partido::imprimir() const {
    cout << "Partido: " << equipo1->getPais()
    << " vs " << equipo2->getPais() << endl;
    cout << "Fecha: "; fecha.imprimir();
    cout << " | Sede: " << sede << endl;
    cout << "Arbitros: " << codArbitro1
         << ", " << codArbitro2
         << ", " << codArbitro3 << endl;

    if (fueJugado) {
        cout << "Resultado: "
             << statsEq1.getGolesAFavor() << " - "
             << statsEq2.getGolesAFavor() << endl;
        cout << "Posesion: "
             << equipo1->getPais() << " " << statsEq1.getPosesion() << "% | "
             << equipo2->getPais() << " " << statsEq2.getPosesion() << "%" << endl;
        if (ganador)
            cout << "Ganador: " << ganador->getPais() << endl;
        else
            cout << "Resultado: EMPATE" << endl;
        if (fueAProrrgoa)
            cout << "(Se fue a prorroga)" << endl;
    } else {
        cout << "(Partido no jugado aun)" << endl;
    }
}

void Partido::imprimirGoleadores() const {
    cout << "Goleadores " << equipo1->getPais() << ": ";
    for (int i = 0; i < statsEq1.getJugadores().getTamanio(); i++)
        if (statsEq1.getJugadores()[i].getGoles() > 0)
            cout << "#" << statsEq1.getJugadores()[i].getNumeroCamiseta()
                 << "(" << statsEq1.getJugadores()[i].getGoles() << ") ";
    cout << endl;

    cout << "Goleadores " << equipo2->getPais() << ": ";
    for (int i = 0; i < statsEq2.getJugadores().getTamanio(); i++)
        if (statsEq2.getJugadores()[i].getGoles() > 0)
            cout << "#" << statsEq2.getJugadores()[i].getNumeroCamiseta()
                 << "(" << statsEq2.getJugadores()[i].getGoles() << ") ";
    cout << endl;
}

ostream& operator<<(ostream& os, const Partido& p) {
    os << p.equipo1->getPais() << " vs " << p.equipo2->getPais()
    << " | ";
    if (p.fueJugado)
        os << p.statsEq1.getGolesAFavor() << "-"
           << p.statsEq2.getGolesAFavor();
    else
        os << "pendiente";
    return os;
}
