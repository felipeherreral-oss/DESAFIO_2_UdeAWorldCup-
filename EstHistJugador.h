#ifndef ESTHISTJUGADOR_H
#define ESTHISTJUGADOR_H

class EstHistJugador {
private:
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    EstHistJugador();
    EstHistJugador(const EstHistJugador& otro);

    // Getters
    int getPartidosJugados()   const;
    int getGoles()             const;
    int getMinutosJugados()    const;
    int getAsistencias()       const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas()     const;
    int getFaltas()            const;

    // Setters
    void setGoles(int g);
    void setPartidosJugados(int p);
    void setMinutosJugados(int m);

    // Actualizar tras un partido
    void actualizarConPartido(int golesP, int minutosP,
                              int amarillasP, int rojasP,
                              int faltasP, int asistenciasP);

    void imprimir() const;
};

#endif
