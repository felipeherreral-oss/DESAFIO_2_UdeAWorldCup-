#ifndef PARTIDO_H
#define PARTIDO_H
#include <iostream>
#include "../estructuras/Fecha.h"
#include "../estadisticas/EstPartidoEquipo.h"
#include "../entidades/Equipo.h"

class Partido {
private:
    Fecha   fecha;
    char*   sede;
    char*   codArbitro1;
    char*   codArbitro2;
    char*   codArbitro3;

    Equipo* equipo1;       // Puntero — no copia
    Equipo* equipo2;

    EstPartidoEquipo statsEq1;
    EstPartidoEquipo statsEq2;

    bool    fueJugado;
    bool    fueAProrrgoa;
    Equipo* ganador;       // nullptr si no se ha jugado

    // ─── Utilidades privadas ───────────────────────
    char* copiarCadena(const char* src);
    int   longitud(const char* src) const;

    // Calcula goles esperados con la fórmula λ
    double calcularLambda(double gfa, double gcb) const;

    // Simula los goles y estadísticas de UN equipo
    void simularEquipo(Equipo* eq, EstPartidoEquipo& stats,
                       double lambda, bool prorroga);

    // Calcula posesión basada en ranking FIFA
    void calcularPosesion();

    // Selecciona 11 jugadores aleatorios de la plantilla
    void seleccionarConvocados(Equipo* eq, EstPartidoEquipo& stats);



public:
    Partido();
    Partido(Fecha f, const char* sede,
            const char* arb1, const char* arb2, const char* arb3,
            Equipo* eq1, Equipo* eq2);
    Partido(const Partido& otro);
    ~Partido();

    Partido& operator=(const Partido& otro);

    // Getters
    Fecha        getFecha()      const;
    const char*  getSede()       const;
    Equipo*      getEquipo1()    const;
    Equipo*      getEquipo2()    const;
    Equipo*      getGanador()    const;
    bool         getFueJugado()  const;
    bool         getFueAProrroga() const;
    EstPartidoEquipo& getStatsEq1();
    EstPartidoEquipo& getStatsEq2();

    // Simula el partido completo
    void simular();
    // Actualiza estadísticas históricas de equipos y jugadores
    void actualizarHistoricos();
    // Imprime toda la info del partido
    void imprimir() const;
    // Imprime solo los goleadores (número de camiseta)
    void imprimirGoleadores() const;
    void setGanador(Equipo* g); 
    void setFueAProrroga(bool p);

    // Sobrecarga << para imprimir con cout
    friend std::ostream& operator<<(std::ostream& os, const Partido& p);
};

#endif
