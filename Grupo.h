#ifndef GRUPO_H
#define GRUPO_H

#include "../estructuras/ArregloDinamico.h"
#include "../estructuras/Fecha.h"
#include "../entidades/Equipo.h"
#include "Partido.h"

// Entrada en la tabla de clasificación de un grupo
struct EntradaTabla {
    Equipo* equipo;
    int     puntos;
    int     golesAFavor;      // solo en partidos del grupo
    int     golesEnContra;    // solo en partidos del grupo
    int     partidosJugados;
    int     partidosGanados;
    int     partidosEmpatados;
    int     partidosPerdidos;

    EntradaTabla();
    EntradaTabla(Equipo* eq);
    int getDiferenciaGoles() const;
};

class Grupo {
private:
    char                     identificador; // 'A', 'B', ..., 'L'
    ArregloDinamico<Equipo*> equipos;       // 4 punteros
    ArregloDinamico<Partido> partidos;      // 6 partidos
    ArregloDinamico<EntradaTabla> tabla;    // clasificación

    // Asigna fechas a los 6 partidos respetando restricciones
    void asignarFechas(const Fecha& fechaInicio,
                       int* partidosPorDia,
                       int maxDias);

    // Ordena la tabla por puntos y criterios de desempate
    void ordenarTabla();

    // Compara dos entradas para ordenar (retorna true si a > b)
    bool esMejorQue(const EntradaTabla& a, const EntradaTabla& b) const;

public:
    Grupo();
    Grupo(char id);

    // Agregar equipo al grupo
    void agregarEquipo(Equipo* eq);

    // Genera los 6 partidos del grupo con fechas válidas
    void generarPartidos(const Fecha& fechaInicio,
                         int* partidosPorDia,
                         int maxDias);

    // Simula todos los partidos del grupo
    void simularPartidos();

    // Construye la tabla de clasificación
    void construirTabla();

    // Getters
    char                          getIdentificador()  const;
    ArregloDinamico<Equipo*>&     getEquipos();
    const ArregloDinamico<Equipo*>&     getEquipos()  const;
    ArregloDinamico<Partido>&     getPartidos();
    const ArregloDinamico<Partido>&     getPartidos() const;
    ArregloDinamico<EntradaTabla>& getTabla();
    const ArregloDinamico<EntradaTabla>& getTabla() const;

    Equipo* getPrimero();   // 1er lugar
    Equipo* getSegundo();   // 2do lugar
    Equipo* getTercero();   // 3er lugar
    Equipo* getCuarto();    // 4to lugar

    EntradaTabla& getEntradaPrimero();
    EntradaTabla& getEntradaSegundo();
    EntradaTabla& getEntradaTercero();

    void imprimirTabla() const;
    void imprimirPartidos() const;
};

#endif
