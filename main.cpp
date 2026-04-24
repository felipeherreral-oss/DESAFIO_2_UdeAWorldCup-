#include <iostream>
#include <cstdlib>
#include <ctime>
#include "estructuras/ArregloDinamico.h"
#include "entidades/Equipo.h"
#include "persistencia/GestorArchivos.h"
#include "simulacion/Partido.h"
using namespace std;

int main() {
    srand(time(nullptr)); // semilla aleatoria

    // Cargar equipos reales
    ArregloDinamico<Equipo> equipos(48);
    GestorArchivos::cargarEquiposDesdeCSV(equipos,
                                          "datos/selecciones_clasificadas_mundial.csv");

    cout << "\n=== PRUEBA Partido ===" << endl;

    // Partido Argentina vs France
    Partido p(
        Fecha(20, 6, 2026),
        "nombreSede",
        "codArbitro1", "codArbitro2", "codArbitro3",
        &equipos[0],  // France (ranking 1)
        &equipos[2]   // Argentina (ranking 3)
        );

    cout << "Antes de simular:" << endl;
    p.imprimir();

    p.simular();

    cout << "\nDespues de simular:" << endl;
    p.imprimir();
    p.imprimirGoleadores();

    // Verificar que los historicos se actualizaron
    p.actualizarHistoricos();
    cout << "\nHistoricos France tras partido:" << endl;
    equipos[0].getEstHistorica().imprimir(); cout << endl;
    cout << "Historicos Argentina tras partido:" << endl;
    equipos[2].getEstHistorica().imprimir(); cout << endl;

    // Usar operator
    cout << "\nOperador <<: " << p << endl;

    return 0;
}
