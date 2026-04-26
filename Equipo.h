#ifndef EQUIPO_H
#define EQUIPO_H

#include "../estructuras/ArregloDinamico.h"
#include "../estadisticas/EstHistEquipo.h"
#include "Jugador.h"

class Equipo {
private:
    char* pais;
    char* confederacion;
    char* federacion;
    char* directorTecnico;
    int   rankingFIFA;
    EstHistEquipo            estadisticasHistoricas;
    ArregloDinamico<Jugador> plantilla;

    char* copiarCadena(const char* src);
    int   longitud(const char* src) const;

    // Convierte entero a char[] sin sprintf
    void intACadena(int num, char* buffer) const;

    // Concatena dos cadenas en destino sin strcat
    void concatenar(char* destino, const char* fuente) const;

public:
    Equipo();
    Equipo(const char* pais, const char* conf,
           const char* fed,  const char* dt, int ranking);
    Equipo(const Equipo& otro);
    ~Equipo();

    Equipo& operator=(const Equipo& otro);

    bool operator<(const Equipo& otro)  const;
    bool operator==(const Equipo& otro) const;

    const char*              getPais()            const;
    const char*              getConfederacion()   const;
    const char*              getFederacion()      const;
    const char*              getDirectorTecnico() const;
    int                      getRankingFIFA()     const;
    EstHistEquipo&           getEstHistorica();
    ArregloDinamico<Jugador>& getPlantilla();
    const ArregloDinamico<Jugador>& getPlantilla() const;

    void setPais(const char* p);
    void setConfederacion(const char* c);
    void setFederacion(const char* f);
    void setDirectorTecnico(const char* dt);
    void setRankingFIFA(int r);

    void generarPlantillaArtificial();

    double getPromedioGolesAFavor()   const;
    double getPromedioGolesEnContra() const;

    void imprimir() const;
};

#endif
