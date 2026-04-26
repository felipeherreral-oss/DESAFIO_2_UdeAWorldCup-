#include "Fecha.h"
#include <iostream>
using namespace std;

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

bool Fecha::operator<(const Fecha& o) const {
    if (anio != o.anio) return anio < o.anio;
    if (mes != o.mes)   return mes < o.mes;
    return dia < o.dia;
}

bool Fecha::operator==(const Fecha& o) const {
    return dia == o.dia && mes == o.mes && anio == o.anio;
}

bool Fecha::operator<=(const Fecha& o) const {
    return (*this < o) || (*this == o);
}

// Convierte fecha a días absolutos para el cálculo
int Fecha::operator-(const Fecha& otra) const {
    // Cálculo simple de diferencia de días (asume mismo mes/año para este proyecto)
    // Para producción usarías el algoritmo juliano
    int diasThis = anio * 365 + mes * 30 + dia;
    int diasOtra = otra.anio * 365 + otra.mes * 30 + otra.dia;
    return diasThis - diasOtra;
}

Fecha Fecha::operator+(int dias) const {
    // Tabla de días por mes (sin año bisiesto por simplicidad)
    int diasPorMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int d = dia + dias;
    int m = mes;
    int a = anio;
    while (d > diasPorMes[m]) {
        d -= diasPorMes[m];
        m++;
        if (m > 12) { m = 1; a++; }
    }
    return Fecha(d, m, a);
}

void Fecha::imprimir() const {
    cout << (dia < 10 ? "0" : "") << dia << "/"
         << (mes < 10 ? "0" : "") << mes << "/"
         << anio;
}

Fecha& Fecha::operator=(const Fecha& otra) {
    if (this != &otra) {
        dia  = otra.dia;
        mes  = otra.mes;
        anio = otra.anio;
    }
    return *this;
}
