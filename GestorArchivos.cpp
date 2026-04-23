#include "GestorArchivos.h"
#include <iostream>
#include <fstream>
using namespace std;

// ─── Utilidades privadas ────────────────────────────────────────────

int GestorArchivos::longitud(const char* str) {
    if (!str) return 0;
    int i = 0;
    while (str[i] != '\0') i++;
    return i;
}

bool GestorArchivos::iguales(const char* a, const char* b) {
    if (!a || !b) return false;
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

// Elimina \r y espacios sobrantes al final
void GestorArchivos::limpiarCadena(char* str) {
    int len = longitud(str);
    while (len > 0 && (str[len-1] == '\r' ||
                       str[len-1] == '\n' ||
                       str[len-1] == ' ')) {
        str[len-1] = '\0';
        len--;
    }
}

// Convierte cadena a entero sin atoi
int GestorArchivos::cadenaAEntero(const char* str) {
    int resultado = 0;
    int i = 0;
    // Saltar espacios
    while (str[i] == ' ') i++;
    while (str[i] >= '0' && str[i] <= '9') {
        resultado = resultado * 10 + (str[i] - '0');
        i++;
    }
    return resultado;
}

// Lee desde posicion 'inicio' hasta ';' o fin de línea
// Retorna la nueva posición
int GestorArchivos::leerCampo(const char* linea, int inicio, char* campo) {
    int i = inicio;
    int j = 0;
    while (linea[i] != '\0' &&
           linea[i] != ';'  &&
           linea[i] != '\n' &&
           linea[i] != '\r') {
        campo[j++] = linea[i++];
    }
    campo[j] = '\0';
    if (linea[i] == ';') i++; // saltar el ';'
    return i;
}

// ─── Carga equipos desde CSV ────────────────────────────────────────

void GestorArchivos::cargarEquiposDesdeCSV(ArregloDinamico<Equipo>& equipos,
                                           const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo: " << ruta << endl;
        return;
    }

    char linea[512];
    int  numLinea = 0;

    while (archivo.getline(linea, 512)) {
        numLinea++;

        // Saltar BOM de la primera línea (UTF-8: EF BB BF)
        int inicio = 0;
        if (numLinea == 1) {
            // Detectar BOM como bytes unsigned
            unsigned char* u = (unsigned char*)linea;
            if (u[0] == 0xEF && u[1] == 0xBB && u[2] == 0xBF)
                inicio = 3;
        }

        // Saltar filas 1 y 2 (título y encabezados)
        if (numLinea <= 2) continue;

        limpiarCadena(linea);
        if (longitud(linea + inicio) == 0) continue;

        // Campos temporales
        char ranking[10], pais[100], dt[150], fed[200], conf[50];
        char golesF[10], golesC[10], pg[10], pe[10], pp[10];

        int pos = inicio;
        pos = leerCampo(linea, pos, ranking);
        pos = leerCampo(linea, pos, pais);
        pos = leerCampo(linea, pos, dt);
        pos = leerCampo(linea, pos, fed);
        pos = leerCampo(linea, pos, conf);
        pos = leerCampo(linea, pos, golesF);
        pos = leerCampo(linea, pos, golesC);
        pos = leerCampo(linea, pos, pg);
        pos = leerCampo(linea, pos, pe);
        pos = leerCampo(linea, pos, pp);

        // Crear equipo con datos del CSV
        Equipo eq(pais, conf, fed, dt, cadenaAEntero(ranking));

        int gf  = cadenaAEntero(golesF);
        int gc  = cadenaAEntero(golesC);
        int PG  = cadenaAEntero(pg);
        int PE  = cadenaAEntero(pe);
        int PP  = cadenaAEntero(pp);
        int PJ  = PG + PE + PP;

        eq.getEstHistorica().setGolesAFavor(gf);
        eq.getEstHistorica().setGolesEnContra(gc);
        eq.getEstHistorica().setPartidosJugados(PJ);
        eq.getEstHistorica().setPartidosGanados(PG);
        eq.getEstHistorica().setPartidosEmpatados(PE);
        eq.getEstHistorica().setPartidosPerdidos(PP);
        // Amarillas, rojas y faltas = 0 según enunciado (ya lo hace el constructor)

        eq.generarPlantillaArtificial();
        equipos.agregar(eq);
    }

    archivo.close();
    cout << "Equipos cargados: " << equipos.getTamanio() << endl;
}

// ─── Guardar jugadores ──────────────────────────────────────────────

void GestorArchivos::guardarJugadores(const ArregloDinamico<Equipo>& equipos,
                                      const char* ruta) {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo crear: " << ruta << endl;
        return;
    }

    // Encabezado
    archivo << "PAIS;CAMISETA;NOMBRE;APELLIDO;PJ;GOLES;MINUTOS;ASIST;AMARILLAS;ROJAS;FALTAS\n";

    for (int i = 0; i < equipos.getTamanio(); i++) {
        const Equipo& eq = equipos[i];
        for (int j = 0; j < eq.getPlantilla().getTamanio(); j++) {
            const Jugador& jug = eq.getPlantilla()[j];
            const EstHistJugador& est = jug.getEstHistorica();
            archivo << eq.getPais()               << ";"
                    << jug.getNumeroCamiseta()     << ";"
                    << jug.getNombre()             << ";"
                    << jug.getApellido()           << ";"
                    << est.getPartidosJugados()    << ";"
                    << est.getGoles()              << ";"
                    << est.getMinutosJugados()     << ";"
                    << est.getAsistencias()        << ";"
                    << est.getTarjetasAmarillas()  << ";"
                    << est.getTarjetasRojas()      << ";"
                    << est.getFaltas()             << "\n";
        }
    }

    archivo.close();
    cout << "Jugadores guardados en: " << ruta << endl;
}

// ─── Cargar jugadores ───────────────────────────────────────────────

void GestorArchivos::cargarJugadores(ArregloDinamico<Equipo>& equipos,
                                     const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "AVISO: No existe archivo de jugadores. "
             << "Se usaran los datos generados." << endl;
        return;
    }

    char linea[512];
    int  numLinea = 0;

    while (archivo.getline(linea, 512)) {
        numLinea++;
        if (numLinea == 1) continue; // saltar encabezado

        limpiarCadena(linea);
        if (longitud(linea) == 0) continue;

        char paisBuf[100], camisetaBuf[5], nom[30], ape[30];
        char pj[5], goles[5], min[10], asist[5];
        char amarillas[5], rojas[5], faltas[5];

        int pos = 0;
        pos = leerCampo(linea, pos, paisBuf);
        pos = leerCampo(linea, pos, camisetaBuf);
        pos = leerCampo(linea, pos, nom);
        pos = leerCampo(linea, pos, ape);
        pos = leerCampo(linea, pos, pj);
        pos = leerCampo(linea, pos, goles);
        pos = leerCampo(linea, pos, min);
        pos = leerCampo(linea, pos, asist);
        pos = leerCampo(linea, pos, amarillas);
        pos = leerCampo(linea, pos, rojas);
        pos = leerCampo(linea, pos, faltas);

        // Buscar el equipo correspondiente
        for (int i = 0; i < equipos.getTamanio(); i++) {
            if (iguales(equipos[i].getPais(), paisBuf)) {
                int camiseta = cadenaAEntero(camisetaBuf);
                // Buscar el jugador por número de camiseta
                for (int j = 0; j < equipos[i].getPlantilla().getTamanio(); j++) {
                    Jugador& jug = equipos[i].getPlantilla()[j];
                    if (jug.getNumeroCamiseta() == camiseta) {
                        jug.setNombre(nom);
                        jug.setApellido(ape);
                        EstHistJugador& est = jug.getEstHistorica();
                        est.setPartidosJugados(cadenaAEntero(pj));
                        est.setGoles(cadenaAEntero(goles));
                        est.setMinutosJugados(cadenaAEntero(min));
                        break;
                    }
                }
                break;
            }
        }
    }

    archivo.close();
    cout << "Jugadores cargados desde: " << ruta << endl;
}
