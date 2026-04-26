#include "Jugador.h"
#include <iostream>
using namespace std;

// Calcula longitud sin strlen
int Jugador::longitud(const char* src) const {
    if (!src) return 0;
    int len = 0;
    while (src[len] != '\0') len++;
    return len;
}

// Copia profunda sin strcpy
char* Jugador::copiarCadena(const char* src) {
    if (!src) return nullptr;
    int len = longitud(src);
    char* dst = new char[len + 1];
    for (int i = 0; i <= len; i++)
        dst[i] = src[i];
    return dst;
}

Jugador::Jugador()
    : numeroCamiseta(0), nombre(nullptr), apellido(nullptr),
    estadisticasHistoricas(), estadisticasPartidoActual() {
    nombre   = copiarCadena("sinNombre");
    apellido = copiarCadena("sinApellido");
}

Jugador::Jugador(int camiseta, const char* nom, const char* ape)
    : numeroCamiseta(camiseta), nombre(nullptr), apellido(nullptr),
    estadisticasHistoricas(), estadisticasPartidoActual(camiseta) {
    nombre   = copiarCadena(nom);
    apellido = copiarCadena(ape);
}

Jugador::Jugador(const Jugador& otro)
    : numeroCamiseta(otro.numeroCamiseta),
    nombre(nullptr), apellido(nullptr),
    estadisticasHistoricas(otro.estadisticasHistoricas),
    estadisticasPartidoActual(otro.estadisticasPartidoActual) {
    nombre   = copiarCadena(otro.nombre);
    apellido = copiarCadena(otro.apellido);
}

Jugador::~Jugador() {
    delete[] nombre;
    delete[] apellido;
}

Jugador& Jugador::operator=(const Jugador& otro) {
    if (this != &otro) {
        delete[] nombre;
        delete[] apellido;
        numeroCamiseta            = otro.numeroCamiseta;
        nombre                    = copiarCadena(otro.nombre);
        apellido                  = copiarCadena(otro.apellido);
        estadisticasHistoricas    = otro.estadisticasHistoricas;
        estadisticasPartidoActual = otro.estadisticasPartidoActual;
    }
    return *this;
}

int         Jugador::getNumeroCamiseta() const { return numeroCamiseta; }
const char* Jugador::getNombre()         const { return nombre; }
const char* Jugador::getApellido()       const { return apellido; }

EstHistJugador& Jugador::getEstHistorica() {
    return estadisticasHistoricas;
}

const EstHistJugador& Jugador::getEstHistorica() const {
    return estadisticasHistoricas;
}

EstPartidoJugador& Jugador::getEstPartidoActual() {
    return estadisticasPartidoActual;
}

void Jugador::setNombre(const char* nom) {
    delete[] nombre;
    nombre = copiarCadena(nom);
}

void Jugador::setApellido(const char* ape) {
    delete[] apellido;
    apellido = copiarCadena(ape);
}

void Jugador::reiniciarEstPartido(int camiseta) {
    estadisticasPartidoActual = EstPartidoJugador(camiseta);
}

void Jugador::confirmarPartido() {
    estadisticasHistoricas.actualizarConPartido(
        estadisticasPartidoActual.getGoles(),
        estadisticasPartidoActual.getMinutosJugados(),
        estadisticasPartidoActual.getTarjetasAmarillas(),
        estadisticasPartidoActual.getTarjetasRojas(),
        estadisticasPartidoActual.getFaltas(),
        0
        );
}

void Jugador::imprimir() const {
    cout << "[" << numeroCamiseta << "] "
         << nombre << " " << apellido << " | ";
    estadisticasHistoricas.imprimir();
}
