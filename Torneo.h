#ifndef TORNEO_H
#define TORNEO_H

#include "../estructuras/ArregloDinamico.h"
#include "../entidades/Equipo.h"
#include "Grupo.h"
#include "Partido.h"

class Torneo {
private:
    ArregloDinamico<Equipo>  equipos;     // 48 equipos (dueño de la memoria)
    ArregloDinamico<Grupo>   grupos;      // 12 grupos
    ArregloDinamico<Partido> partidosR16; // 32 partidos
    ArregloDinamico<Partido> partidosR8;  // 16 partidos
    ArregloDinamico<Partido> partidosQF;  // 8 partidos
    ArregloDinamico<Partido> partidosSF;  // 4 partidos
    ArregloDinamico<Partido> partidosFinal; // 2 (3er puesto + final)

    // Bombos del sorteo
    ArregloDinamico<Equipo*> bombo1; // 12
    ArregloDinamico<Equipo*> bombo2; // 12
    ArregloDinamico<Equipo*> bombo3; // 12
    ArregloDinamico<Equipo*> bombo4; // 12

    // ─── Medidor de recursos ──────────────────
    long long contadorIteraciones;

    void  resetContador();
    void  incrementar(int n = 1);
    void  mostrarMetricas(const char* nombreFuncionalidad) const;

    // Cálculo de memoria por componente
    size_t memoriaArregloDinamicoEquipos()  const;
    size_t memoriaArregloDinamicoGrupos()   const;
    size_t memoriaPartidos()                const;
    size_t memoriaTotal()                   const;

    // Control de fechas fase de grupos
    int partidosPorDia[19];

    // ─── Utilidades privadas ───────────────────────────────────────
    bool equiposCargados;
    bool gruposSorteados;
    bool gruposSimulados;
    bool r16Construido;
    bool r16Simulado;
    bool r8Simulado;
    bool qfSimulado;
    bool sfSimulado;
    bool finalSimulado;
    // Compara dos cadenas sin strcmp
    bool iguales(const char* a, const char* b) const;

    // Verifica si un equipo puede entrar a un grupo
    // según restricciones de confederación
    bool puedeEntrarAlGrupo(Grupo& grupo, Equipo* eq) const;

    // Cuenta equipos de una confederación en un grupo
    int contarConfederacion(Grupo& grupo, const char* conf) const;

    // Saca un equipo aleatorio de un bombo (lo elimina del bombo)
    Equipo* sacarAleatorio(ArregloDinamico<Equipo*>& bombo);

    // Simula etapa eliminatoria genérica
    // Llena 'siguientes' con los ganadores
    void simularEtapa(ArregloDinamico<Partido>& partidos,
                      ArregloDinamico<Equipo*>& ganadores,
                      const char* nombreEtapa);

    // Rompe empate en eliminatorias usando ranking FIFA
    Equipo* romperEmpate(Equipo* eq1, Equipo* eq2,
                         Partido& partido);

    // Construye los 32 partidos de R16 según reglas del enunciado
    void construirR16(ArregloDinamico<Equipo*>& primeros,
                      ArregloDinamico<Equipo*>& segundos,
                      ArregloDinamico<Equipo*>& terceros);

public:
    Torneo();

    // ─── Funcionalidades principales ──────────────────────────────

    // I — Carga de datos
    void cargarEquipos(const char* rutaCSV);
    void guardarJugadores(const char* ruta);
    void cargarJugadores(const char* ruta);

    // II — Conformación de grupos
    void formarBombos();
    void sortearGrupos();
    void imprimirGrupos() const;

    // III — Simulación
    void programarYSimularGrupos();
    void construirTablas();
    void transicionR16();
    void simularR16();
    void simularR8();
    void simularQF();
    void simularSF();
    void simularFinal();

    // IV — Estadísticas
    void generarEstadisticas();

    // Menú principal
    void ejecutarMenu();
};

#endif
