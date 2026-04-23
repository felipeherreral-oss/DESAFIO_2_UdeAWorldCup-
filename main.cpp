#include <iostream>
#include "estructuras/ArregloDinamico.h"
#include "entidades/Equipo.h"
#include "persistencia/GestorArchivos.h"
using namespace std;

int main() {

    cout << "=== PRUEBA GestorArchivos ===" << endl;

    ArregloDinamico<Equipo> equipos(48);

    // Ajusta la ruta según tu proyecto
    GestorArchivos::cargarEquiposDesdeCSV(equipos,
                                          "datos/selecciones_clasificadas_mundial.csv");

    cout << "Total equipos: " << equipos.getTamanio() << endl; // 48

    // Mostrar primeros 5 equipos
    cout << "\n--- Primeros 5 equipos ---" << endl;
    for (int i = 0; i < 5; i++) {
        equipos[i].imprimir();
        cout << " | PJ:" << equipos[i].getEstHistorica().getPartidosJugados()
             << " GF:"   << equipos[i].getEstHistorica().getGolesAFavor()
             << " GC:"   << equipos[i].getEstHistorica().getGolesEnContra()
             << endl;
    }

    // Verificar jugadores del primer equipo
    cout << "\n--- Jugadores de " << equipos[0].getPais() << " ---" << endl;
    int totalGoles = 0;
    for (int j = 0; j < equipos[0].getPlantilla().getTamanio(); j++) {
        totalGoles += equipos[0].getPlantilla()[j].getEstHistorica().getGoles();
    }
    cout << "Total jugadores: "
         << equipos[0].getPlantilla().getTamanio() << endl; // 26
    cout << "Total goles repartidos: " << totalGoles
         << " (debe ser "
         << equipos[0].getEstHistorica().getGolesAFavor()
         << ")" << endl;

    // Guardar jugadores
    cout << "\n--- Guardando jugadores ---" << endl;
    GestorArchivos::guardarJugadores(equipos, "datos/jugadores.csv");

    // Recargar jugadores
    cout << "\n--- Recargando jugadores ---" << endl;
    GestorArchivos::cargarJugadores(equipos, "datos/jugadores.csv");
    cout << "Primer jugador de "
         << equipos[0].getPais() << ": "
         << equipos[0].getPlantilla()[0].getNombre() << " "
         << equipos[0].getPlantilla()[0].getApellido() << endl;

    cout << "\nPrueba GestorArchivos completada." << endl;
    return 0;
}
