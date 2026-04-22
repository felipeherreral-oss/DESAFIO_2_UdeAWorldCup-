// EstPartidoEquipo.h
#ifndef ESTPARTIDOEQUIPO_H
#define ESTPARTIDOEQUIPO_H

#include "../estructuras/ArregloDinamico.h"
#include "EstPartidoJugador.h"

class EstPartidoEquipo {
private:
    int golesAFavor;
    int golesEnContra;
    double posesionBalon;
    ArregloDinamico<EstPartidoJugador> jugadoresConvocados; // 11 exactos

public:
    EstPartidoEquipo();
    EstPartidoEquipo(const EstPartidoEquipo& otro);

    int    getGolesAFavor()    const;
    int    getGolesEnContra()  const;
    double getPosesion()       const;

    void setGolesAFavor(int g);
    void setGolesEnContra(int g);
    void setPosesion(double p);

    void agregarJugador(const EstPartidoJugador& j);
    ArregloDinamico<EstPartidoJugador>& getJugadores();

    void imprimir() const;
};

#endif
