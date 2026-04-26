#ifndef PARTIDO_H
#define PARTIDO_H

#include <iostream>
#include "../estadisticas/EstPartidoEquipo.h"
#include "../entidades/Equipo.h"

class Partido {
private:
    // Fecha como tres enteros (antes era objeto Fecha)
    int dia, mes, anio;

    char*   sede;
    char*   codArbitro1;
    char*   codArbitro2;
    char*   codArbitro3;

    Equipo* equipo1;
    Equipo* equipo2;

    EstPartidoEquipo statsEq1;
    EstPartidoEquipo statsEq2;

    bool    fueJugado;
    bool    fueAProrrgoa;
    Equipo* ganador;

    // ─── Utilidades privadas ───────────────────────
    char* copiarCadena(const char* src);
    int   longitud(const char* src) const;

    double calcularLambda(double gfa, double gcb) const;
    void   simularEquipo(Equipo* eq, EstPartidoEquipo& stats,
                       double lambda, bool prorroga);
    void   calcularPosesion();
    void   seleccionarConvocados(Equipo* eq, EstPartidoEquipo& stats);

public:
    Partido();
    Partido(int d, int m, int a,
            const char* sede,
            const char* arb1, const char* arb2, const char* arb3,
            Equipo* eq1, Equipo* eq2);
    Partido(const Partido& otro);
    ~Partido();

    Partido& operator=(const Partido& otro);

    // Getters fecha
    int         getDia()          const;
    int         getMes()          const;
    int         getAnio()         const;

    // Getters
    const char*      getSede()        const;
    Equipo*          getEquipo1()     const;
    Equipo*          getEquipo2()     const;
    Equipo*          getGanador()     const;
    bool             getFueJugado()   const;
    bool             getFueAProrroga()const;
    EstPartidoEquipo& getStatsEq1();
    EstPartidoEquipo& getStatsEq2();

    // Setters
    void setGanador(Equipo* g);
    void setFueAProrroga(bool p);

    void simular();
    void actualizarHistoricos();
    void imprimir()          const;
    void imprimirGoleadores()const;

    friend std::ostream& operator<<(std::ostream& os, const Partido& p);
};

#endif
