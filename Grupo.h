#ifndef GRUPO_H
#define GRUPO_H

#include "../estructuras/ArregloDinamico.h"
#include "../estructuras/Fecha.h"
#include "../entidades/Equipo.h"
#include "Partido.h"

class Grupo {
private:
    char                     identificador;
    ArregloDinamico<Equipo*> equipos;
    ArregloDinamico<Partido> partidos;

    // Tabla de clasificación como arreglos paralelos
    // índice i corresponde al equipo en equipos[ordenTabla[i]]
    int puntos[4];
    int golesAFavor[4];
    int golesEnContra[4];
    int partidosJugados[4];
    int partidosGanados[4];
    int partidosEmpatados[4];
    int partidosPerdidos[4];
    int ordenTabla[4]; // índices ordenados: ordenTabla[0]=1ro, [1]=2do...

    // Utilidades privadas
    void reiniciarTabla();
    int  getDiferenciaGoles(int idx) const;
    bool esMejorQue(int idxA, int idxB) const;
    void ordenarTabla();

public:
    Grupo();
    Grupo(char id);
    Grupo(const Grupo& otro);
    Grupo& operator=(const Grupo& otro);

    void agregarEquipo(Equipo* eq);

    void generarPartidos(const Fecha& fechaInicio,
                         int* partidosPorDia,
                         int maxDias);

    void simularPartidos();
    void construirTabla();

    char                      getIdentificador()  const;
    ArregloDinamico<Equipo*>& getEquipos();
    const ArregloDinamico<Equipo*>& getEquipos()  const;
    ArregloDinamico<Partido>& getPartidos();
    const ArregloDinamico<Partido>& getPartidos() const;

    Equipo* getPrimero();
    Equipo* getSegundo();
    Equipo* getTercero();
    Equipo* getCuarto();

    // Getters de la tabla por posición (0=1ro, 1=2do, 2=3ro, 3=4to)
    int getPuntosPorPosicion(int pos)          const;
    int getGolesAFavorPorPosicion(int pos)     const;
    int getGolesEnContraPorPosicion(int pos)   const;
    int getDiferenciaGolesPorPosicion(int pos) const;
    int getPartidosJugadosPorPosicion(int pos) const;
    int getPartidosGanadosPorPosicion(int pos) const;
    int getPartidosEmpatadosPorPosicion(int pos)const;
    int getPartidosPerdidosPorPosicion(int pos) const;

    void imprimirTabla() const;
    void imprimirPartidos() const;
};

#endif
