#ifndef JUGADOR_H
#define JUGADOR_H

#include "../estadisticas/EstHistJugador.h"
#include "../estadisticas/EstPartidoJugador.h"

class Jugador {
private:
    int    numeroCamiseta;
    char*  nombre;
    char*  apellido;
    EstHistJugador    estadisticasHistoricas;
    EstPartidoJugador estadisticasPartidoActual;

    char* copiarCadena(const char* src);
    int   longitud(const char* src) const;

public:
    Jugador();
    Jugador(int camiseta, const char* nom, const char* ape);
    Jugador(const Jugador& otro);
    ~Jugador();

    Jugador& operator=(const Jugador& otro);

    int               getNumeroCamiseta()    const;
    const char*       getNombre()            const;
    const char*       getApellido()          const;
    EstHistJugador&    getEstHistorica();
    const EstHistJugador& getEstHistorica() const;
    EstPartidoJugador& getEstPartidoActual();

    void setNombre(const char* nom);
    void setApellido(const char* ape);

    void reiniciarEstPartido(int camiseta);
    void confirmarPartido();

    void imprimir() const;
};

#endif
