// EstPartidoJugador.h
#ifndef ESTPARTIDOJUGADOR_H
#define ESTPARTIDOJUGADOR_H

class EstPartidoJugador {
private:
    int numeroCamiseta;
    int goles;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;
    int minutosJugados;

public:
    EstPartidoJugador();
    EstPartidoJugador(int camiseta);
    EstPartidoJugador(const EstPartidoJugador& otro);

    int getNumeroCamiseta()    const;
    int getGoles()             const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas()     const;
    int getFaltas()            const;
    int getMinutosJugados()    const;

    void setGoles(int g);
    void setTarjetasAmarillas(int t);
    void setTarjetasRojas(int t);
    void setFaltas(int f);
    void setMinutosJugados(int m);

    void imprimir() const;
};

#endif
