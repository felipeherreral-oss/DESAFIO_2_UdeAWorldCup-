#include "Equipo.h"
#include <iostream>
using namespace std;

// ─── Utilidades privadas ────────────────────────────────────────────

int Equipo::longitud(const char* src) const {
    if (!src) return 0;
    int len = 0;
    while (src[len] != '\0') len++;
    return len;
}

char* Equipo::copiarCadena(const char* src) {
    if (!src) return nullptr;
    int len = longitud(src);
    char* dst = new char[len + 1];
    for (int i = 0; i <= len; i++)
        dst[i] = src[i];
    return dst;
}

// Convierte int a string en buffer (el buffer debe tener espacio suficiente)
void Equipo::intACadena(int num, char* buffer) const {
    if (num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    char temp[20];
    int digits = 0;
    while (num > 0) {
        temp[digits++] = '0' + (num % 10);
        num /= 10;
    }
    // Invertir
    for (int i = 0; i < digits; i++)
        buffer[i] = temp[digits - 1 - i];
    buffer[digits] = '\0';
}

// Concatena fuente al final de destino
void Equipo::concatenar(char* destino, const char* fuente) const {
    int lenDst = longitud(destino);
    int lenSrc = longitud(fuente);
    for (int i = 0; i <= lenSrc; i++)
        destino[lenDst + i] = fuente[i];
}

// ─── Constructores ──────────────────────────────────────────────────

Equipo::Equipo()
    : pais(nullptr), confederacion(nullptr),
    federacion(nullptr), directorTecnico(nullptr),
    rankingFIFA(0), estadisticasHistoricas(), plantilla(26) {
    pais            = copiarCadena("sinPais");
    confederacion   = copiarCadena("sinConf");
    federacion      = copiarCadena("sinFed");
    directorTecnico = copiarCadena("sinDT");
}

Equipo::Equipo(const char* p, const char* conf,
               const char* fed, const char* dt, int ranking)
    : pais(nullptr), confederacion(nullptr),
    federacion(nullptr), directorTecnico(nullptr),
    rankingFIFA(ranking), estadisticasHistoricas(), plantilla(26) {
    pais            = copiarCadena(p);
    confederacion   = copiarCadena(conf);
    federacion      = copiarCadena(fed);
    directorTecnico = copiarCadena(dt);
}

Equipo::Equipo(const Equipo& otro)
    : pais(nullptr), confederacion(nullptr),
    federacion(nullptr), directorTecnico(nullptr),
    rankingFIFA(otro.rankingFIFA),
    estadisticasHistoricas(otro.estadisticasHistoricas),
    plantilla(otro.plantilla) {
    pais            = copiarCadena(otro.pais);
    confederacion   = copiarCadena(otro.confederacion);
    federacion      = copiarCadena(otro.federacion);
    directorTecnico = copiarCadena(otro.directorTecnico);
}

Equipo::~Equipo() {
    delete[] pais;
    delete[] confederacion;
    delete[] federacion;
    delete[] directorTecnico;
}

Equipo& Equipo::operator=(const Equipo& otro) {
    if (this != &otro) {
        delete[] pais;
        delete[] confederacion;
        delete[] federacion;
        delete[] directorTecnico;

        pais                   = copiarCadena(otro.pais);
        confederacion          = copiarCadena(otro.confederacion);
        federacion             = copiarCadena(otro.federacion);
        directorTecnico        = copiarCadena(otro.directorTecnico);
        rankingFIFA            = otro.rankingFIFA;
        estadisticasHistoricas = otro.estadisticasHistoricas;
        plantilla              = otro.plantilla;
    }
    return *this;
}

// ─── Operadores ─────────────────────────────────────────────────────

bool Equipo::operator<(const Equipo& otro) const {
    return rankingFIFA < otro.rankingFIFA;
}

bool Equipo::operator==(const Equipo& otro) const {
    if (!pais || !otro.pais) return false;
    int i = 0;
    while (pais[i] != '\0' && otro.pais[i] != '\0') {
        if (pais[i] != otro.pais[i]) return false;
        i++;
    }
    return pais[i] == otro.pais[i]; // ambos deben terminar igual
}

// ─── Getters ────────────────────────────────────────────────────────

const char* Equipo::getPais()            const { return pais; }
const char* Equipo::getConfederacion()   const { return confederacion; }
const char* Equipo::getFederacion()      const { return federacion; }
const char* Equipo::getDirectorTecnico() const { return directorTecnico; }
int         Equipo::getRankingFIFA()     const { return rankingFIFA; }

EstHistEquipo& Equipo::getEstHistorica() {
    return estadisticasHistoricas;
}

ArregloDinamico<Jugador>& Equipo::getPlantilla() {
    return plantilla;
}

const ArregloDinamico<Jugador>& Equipo::getPlantilla() const {
    return plantilla;
}

// ─── Setters ────────────────────────────────────────────────────────

void Equipo::setPais(const char* p) {
    delete[] pais;
    pais = copiarCadena(p);
}

void Equipo::setConfederacion(const char* c) {
    delete[] confederacion;
    confederacion = copiarCadena(c);
}

void Equipo::setFederacion(const char* f) {
    delete[] federacion;
    federacion = copiarCadena(f);
}

void Equipo::setDirectorTecnico(const char* dt) {
    delete[] directorTecnico;
    directorTecnico = copiarCadena(dt);
}

void Equipo::setRankingFIFA(int r) { rankingFIFA = r; }

// ─── Lógica ─────────────────────────────────────────────────────────

void Equipo::generarPlantillaArtificial() {
    int totalGoles = estadisticasHistoricas.getGolesAFavor();
    int golesBase  = totalGoles / 26;
    int golesExtra = totalGoles % 26;

    for (int i = 1; i <= 26; i++) {
        char numStr[10];
        intACadena(i, numStr);

        // Construir "nombreN"
        char nom[30] = "nombre";
        concatenar(nom, numStr);

        // Construir "apellidoN"
        char ape[30] = "apellido";
        concatenar(ape, numStr);

        Jugador j(i, nom, ape);

        int goles = golesBase + (i <= golesExtra ? 1 : 0);
        j.getEstHistorica().setGoles(goles);

        plantilla.agregar(j);
    }
}

double Equipo::getPromedioGolesAFavor() const {
    return estadisticasHistoricas.getPromedioGolesAFavor();
}

double Equipo::getPromedioGolesEnContra() const {
    return estadisticasHistoricas.getPromedioGolesEnContra();
}

void Equipo::imprimir() const {
    cout << "[" << rankingFIFA << "] "
         << pais << " (" << confederacion << ")"
         << " DT: " << directorTecnico;
}
