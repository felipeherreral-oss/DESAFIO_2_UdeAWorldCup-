#ifndef ARRELODINAMICO_H
#define ARRELODINAMICO_H

template <typename T>
class ArregloDinamico {
private:
    T* datos;
    int capacidad;
    int tamanio;

    void redimensionar() {
        capacidad *= 2;
        T* nuevo = new T[capacidad];
        for (int i = 0; i < tamanio; i++)
            nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }

public:
    // Constructor
    ArregloDinamico(int cap = 10)
        : capacidad(cap), tamanio(0) {
        datos = new T[capacidad];
    }

    // Constructor de copia
    ArregloDinamico(const ArregloDinamico<T>& otro)
        : capacidad(otro.capacidad), tamanio(otro.tamanio) {
        datos = new T[capacidad];
        for (int i = 0; i < tamanio; i++)
            datos[i] = otro.datos[i];
    }

    // Destructor
    ~ArregloDinamico() {
        delete[] datos;
    }

    // Operador asignación
    ArregloDinamico<T>& operator=(const ArregloDinamico<T>& otro) {
        if (this != &otro) {
            delete[] datos;
            capacidad = otro.capacidad;
            tamanio = otro.tamanio;
            datos = new T[capacidad];
            for (int i = 0; i < tamanio; i++)
                datos[i] = otro.datos[i];
        }
        return *this;
    }

    // Operador [] con y sin const
    T& operator[](int i) { return datos[i]; }
    const T& operator[](int i) const { return datos[i]; }

    void agregar(const T& elem) {
        if (tamanio == capacidad) redimensionar();
        datos[tamanio++] = elem;
    }

    void eliminarEn(int indice) {
        for (int i = indice; i < tamanio - 1; i++)
            datos[i] = datos[i + 1];
        tamanio--;
    }

    int getTamanio() const { return tamanio; }
    bool estaVacio() const { return tamanio == 0; }

    // Para ordenar (burbuja simple, recibe función comparadora)
    template <typename Comparador>
    void ordenar(Comparador cmp) {
        for (int i = 0; i < tamanio - 1; i++)
            for (int j = 0; j < tamanio - i - 1; j++)
                if (!cmp(datos[j], datos[j+1]))
                    intercambiar(datos[j], datos[j+1]);
    }

private:
    void intercambiar(T& a, T& b) {
        T temp = a; a = b; b = temp;
    }
};

#endif
