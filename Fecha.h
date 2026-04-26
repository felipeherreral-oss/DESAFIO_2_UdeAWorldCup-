#ifndef FECHA_H
#define FECHA_H

class Fecha {
private:
    int dia, mes, anio;

public:
    Fecha(int d = 1, int m = 1, int a = 2026);
    Fecha(const Fecha& otra);

    // Getters
    int getDia() const;
    int getMes() const;
    int getAnio() const;

    // Operadores
    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
    Fecha operator+(int dias) const;      // Sumar días
    int operator-(const Fecha& otra) const; // Diferencia en días
    Fecha& operator=(const Fecha& otra);
    void imprimir() const;
};

#endif
