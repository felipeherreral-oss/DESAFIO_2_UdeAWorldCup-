#include <iostream>
#include <cstdlib>
#include <ctime>
#include "estructuras/ArregloDinamico.h"
#include "entidades/Equipo.h"
#include "persistencia/GestorArchivos.h"
#include "simulacion/Grupo.h"
using namespace std;

int main() {
    srand(time(nullptr));

    ArregloDinamico<Equipo> equipos(48);
    GestorArchivos::cargarEquiposDesdeCSV(equipos,
                                          "datos/selecciones_clasificadas_mundial.csv");

    cout << "\n=== PRUEBA Grupo ===" << endl;

    // Crear grupo A con 4 equipos reales
    Grupo grupoA('A');
    grupoA.agregarEquipo(&equipos[0]);  // France
    grupoA.agregarEquipo(&equipos[2]);  // Argentina
    grupoA.agregarEquipo(&equipos[5]);  // Brazil
    grupoA.agregarEquipo(&equipos[10]); // Morocco

    // Array global de partidos por día (19 días)
    int partidosPorDia[19] = {0};

    grupoA.generarPartidos(Fecha(20, 6, 2026), partidosPorDia, 19);

    cout << "Partidos generados: "
         << grupoA.getPartidos().getTamanio() << endl; // 6

    // Verificar fechas
    cout << "Fechas asignadas:" << endl;
    for (int i = 0; i < grupoA.getPartidos().getTamanio(); i++) {
        cout << "  Partido " << i+1 << ": ";
        grupoA.getPartidos()[i].getFecha().imprimir();
        cout << " — "
             << grupoA.getPartidos()[i].getEquipo1()->getPais()
             << " vs "
             << grupoA.getPartidos()[i].getEquipo2()->getPais()
             << endl;
    }

    // Simular
    grupoA.simularPartidos();
    grupoA.construirTabla();

    // Imprimir tabla
    grupoA.imprimirTabla();

    // Imprimir partidos con goleadores
    grupoA.imprimirPartidos();

    cout << "\nClasificados:" << endl;
    cout << "1ro: " << grupoA.getPrimero()->getPais() << endl;
    cout << "2do: " << grupoA.getSegundo()->getPais() << endl;
    cout << "3ro: " << grupoA.getTercero()->getPais() << endl;
    cout << "4to: " << grupoA.getCuarto()->getPais()  << endl;

    return 0;
}
