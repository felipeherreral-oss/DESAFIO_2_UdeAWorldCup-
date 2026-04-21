#include <iostream>
#include "estructuras/ArregloDinamico.h"
#include "estructuras/Fecha.h"

using namespace std;

int main() {

    cout << "=== PRUEBA 1: ArregloDinamico ===" << endl;

    ArregloDinamico<int> arr(5);
    arr.agregar(10);
    arr.agregar(20);
    arr.agregar(30);

    cout << "Tamanio: " << arr.getTamanio() << endl;  // Esperado: 3
    cout << "Elemento [0]: " << arr[0] << endl;        // Esperado: 10
    cout << "Elemento [2]: " << arr[2] << endl;        // Esperado: 30

    // Prueba constructor de copia
    ArregloDinamico<int> copia(arr);
    copia[0] = 99;
    cout << "Original [0] tras modificar copia: " << arr[0] << endl; // Debe seguir siendo 10
    cout << "Copia [0]: " << copia[0] << endl;                        // Debe ser 99

    cout << "\n=== PRUEBA 2: Fecha ===" << endl;

    Fecha f1(20, 6, 2026);
    Fecha f2(25, 6, 2026);

    cout << "Fecha 1: "; f1.imprimir(); cout << endl;
    cout << "Fecha 2: "; f2.imprimir(); cout << endl;

    // Prueba operador
    if (f1 < f2)
        cout << "f1 es anterior a f2: CORRECTO" << endl;

    // Prueba operador + (sumar días)
    Fecha f3 = f1 + 5;
    cout << "f1 + 5 dias = "; f3.imprimir(); cout << endl; // Esperado: 25/06/2026

    // Prueba operador - (diferencia)
    int diff = f2 - f1;
    cout << "Diferencia f2 - f1 = " << diff << " dias" << endl; // Esperado: 5

    cout << "\n=== PRUEBA 3: ArregloDinamico de Fechas ===" << endl;

    ArregloDinamico<Fecha> fechas(3);
    fechas.agregar(Fecha(20, 6, 2026));
    fechas.agregar(Fecha(23, 6, 2026));
    fechas.agregar(Fecha(26, 6, 2026));

    for (int i = 0; i < fechas.getTamanio(); i++) {
        cout << "Fecha " << i << ": ";
        fechas[i].imprimir();
        cout << endl;
    }

    cout << "\nTodas las pruebas completadas." << endl;
    return 0;
}
