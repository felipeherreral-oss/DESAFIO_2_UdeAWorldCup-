#include <iostream>
#include "estructuras/ArregloDinamico.h"
#include "estructuras/Fecha.h"
#include "estadisticas/EstHistEquipo.h"
#include "estadisticas/EstHistJugador.h"
#include "estadisticas/EstPartidoJugador.h"
#include "estadisticas/EstPartidoEquipo.h"

using namespace std;

int main() {

    cout << "=== PRUEBA EstHistEquipo ===" << endl;
    EstHistEquipo eqHist;
    eqHist.setGolesAFavor(30);
    eqHist.setGolesEnContra(15);
    eqHist.setPartidosJugados(10);
    eqHist.setPartidosGanados(7);
    eqHist.setPartidosEmpatados(2);
    eqHist.setPartidosPerdidos(1);
    eqHist.imprimir(); cout << endl;
    cout << "PromGF: " << eqHist.getPromedioGolesAFavor()   << endl; // 3.0
    cout << "PromGC: " << eqHist.getPromedioGolesEnContra() << endl; // 1.5

    // Simular un partido ganado 2-1
    eqHist.actualizarConPartido(2, 1, true, false, 1, 0, 2);
    cout << "Tras actualizar: "; eqHist.imprimir(); cout << endl;
    // PJ debe ser 11, PG debe ser 8, GF debe ser 32

    cout << "\n=== PRUEBA EstHistJugador ===" << endl;
    EstHistJugador jugHist;
    jugHist.setGoles(5);
    jugHist.imprimir(); cout << endl;
    jugHist.actualizarConPartido(2, 90, 1, 0, 1, 0);
    jugHist.imprimir(); cout << endl;
    // Goles debe ser 7, PJ debe ser 1, minutos 90

    cout << "\n=== PRUEBA EstPartidoJugador ===" << endl;
    EstPartidoJugador epj(10); // camiseta 10
    epj.setGoles(1);
    epj.setTarjetasAmarillas(1);
    epj.setFaltas(2);
    epj.imprimir(); cout << endl;

    cout << "\n=== PRUEBA EstPartidoEquipo ===" << endl;
    EstPartidoEquipo epe;
    epe.setGolesAFavor(3);
    epe.setGolesEnContra(1);
    epe.setPosesion(62.5);

    for (int i = 1; i <= 11; i++) {
        EstPartidoJugador j(i);
        if (i == 9) j.setGoles(2);
        if (i == 7) j.setGoles(1);
        epe.agregarJugador(j);
    }

    epe.imprimir();

    // Prueba constructor de copia de EstPartidoEquipo
    EstPartidoEquipo copia(epe);
    copia.setGolesAFavor(99);
    cout << "Original GF tras modificar copia: "
         << epe.getGolesAFavor() << endl; // Debe ser 3, no 99

    cout << "\nTodas las pruebas de estadisticas completadas." << endl;
    return 0;
}
