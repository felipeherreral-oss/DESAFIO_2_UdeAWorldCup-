#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include "../estructuras/ArregloDinamico.h"
#include "../entidades/Equipo.h"
#include <fstream>

class GestorArchivos {
private:
    // Lee un campo hasta encontrar ';' o '\n' o '\r'
    static int leerCampo(const char* linea, int inicio, char* campo);

    // Convierte char[] a entero sin atoi
    static int cadenaAEntero(const char* str);

    // Limpia \r y espacios al final
    static void limpiarCadena(char* str);

    // Compara dos cadenas sin strcmp
    static bool iguales(const char* a, const char* b);

    // Longitud sin strlen
    static int longitud(const char* str);

public:
    // Carga los 48 equipos desde el CSV dado
    static void cargarEquiposDesdeCSV(ArregloDinamico<Equipo>& equipos,
                                      const char* ruta);

    // Guarda jugadores de todos los equipos en jugadores.csv
    static void guardarJugadores(const ArregloDinamico<Equipo>& equipos,
                                 const char* ruta);

    // Carga jugadores desde jugadores.csv y los asigna a sus equipos
    static void cargarJugadores(ArregloDinamico<Equipo>& equipos,
                                const char* ruta);
};

#endif
