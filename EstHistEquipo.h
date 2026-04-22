#ifndef ESTHISTEQUIPO_H
#define ESTHISTEQUIPO_H

class EstHistEquipo {
private:
    int golesAFavor;
    int golesEnContra;
    int partidosJugados;
    int partidosGanados;
    int partidosEmpatados;
    int partidosPerdidos;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    EstHistEquipo();
    EstHistEquipo(const EstHistEquipo& otro);

    // Getters
    int getGolesAFavor()       const;
    int getGolesEnContra()     const;
    int getPartidosJugados()   const;
    int getPartidosGanados()   const;
    int getPartidosEmpatados() const;
    int getPartidosPerdidos()  const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas()     const;
    int getFaltas()            const;

    // Setters (para carga desde CSV)
    void setGolesAFavor(int g);
    void setGolesEnContra(int g);
    void setPartidosJugados(int p);
    void setPartidosGanados(int p);
    void setPartidosEmpatados(int p);
    void setPartidosPerdidos(int p);

    // Promedios — CUIDADO con división por cero
    double getPromedioGolesAFavor()   const;
    double getPromedioGolesEnContra() const;

    // Actualizar tras finalizar un partido
    void actualizarConPartido(int gf, int gc,
                              bool gano, bool empato,
                              int amarillas, int rojas, int faltasP);

    void imprimir() const;
};

#endif
