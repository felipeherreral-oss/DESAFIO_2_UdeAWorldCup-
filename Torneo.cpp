#include "Torneo.h"
#include "../persistencia/GestorArchivos.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// ─── Constructor ───────────────────────────────────────────────────

Torneo::Torneo()
    : equipos(48), grupos(12),
    partidosR16(32), partidosR8(16),
    partidosQF(8),   partidosSF(4),
    partidosFinal(2),
    bombo1(12), bombo2(12), bombo3(12), bombo4(12), equiposCargados(false),
    gruposSorteados(false), gruposSimulados(false), r16Construido(false),
    r16Simulado(false), r8Simulado(false), qfSimulado(false), sfSimulado(false),
    finalSimulado(false), contadorIteraciones(0){
    for (int i = 0; i < 19; i++)
        partidosPorDia[i] = 0;
}

// ─── Utilidades ────────────────────────────────────────────────────

bool Torneo::iguales(const char* a, const char* b) const {
    if (!a || !b) return false;
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == b[i];
}

int Torneo::contarConfederacion(Grupo& grupo, const char* conf) const {
    int count = 0;
    for (int i = 0; i < grupo.getEquipos().getTamanio(); i++)
        if (iguales(grupo.getEquipos()[i]->getConfederacion(), conf))
            count++;
    return count;
}

bool Torneo::puedeEntrarAlGrupo(Grupo& grupo, Equipo* eq) const {
    const char* conf = eq->getConfederacion();
    int count = contarConfederacion(grupo, conf);

    if (iguales(conf, "UEFA"))
        return count < 2;  // UEFA puede tener máximo 2
    return count < 1;      // resto máximo 1
}

Equipo* Torneo::sacarAleatorio(ArregloDinamico<Equipo*>& bombo) {
    if (bombo.estaVacio()) return nullptr;
    int idx = rand() % bombo.getTamanio();
    Equipo* eq = bombo[idx];
    bombo.eliminarEn(idx);
    return eq;
}

// ─── I — Carga de datos ────────────────────────────────────────────

void Torneo::cargarEquipos(const char* rutaCSV) {
    resetContador();
    GestorArchivos::cargarEquiposDesdeCSV(equipos, rutaCSV);
    cout << "Equipos cargados: " << equipos.getTamanio() << endl;
    incrementar(equipos.getTamanio());
    cout << "Equipos cargados: " << equipos.getTamanio() << endl;
    mostrarMetricas("Carga de equipos");
}

void Torneo::guardarJugadores(const char* ruta) {
    GestorArchivos::guardarJugadores(equipos, ruta);
}

void Torneo::cargarJugadores(const char* ruta) {
    GestorArchivos::cargarJugadores(equipos, ruta);
}

// ─── II — Conformación de grupos ───────────────────────────────────

void Torneo::formarBombos() {
    resetContador();
    // Separar EE.UU (país sede) — va directo al bombo 1
    int idxUSA = -1;
    for (int i = 0; i < equipos.getTamanio(); i++) {
        if (iguales(equipos[i].getPais(), "United States")) {
            idxUSA = i;
            break;
        }
    incrementar(equipos.getTamanio() * equipos.getTamanio());
    mostrarMetricas("Conformacion de bombos");
    }

    // Ordenar equipos por ranking (sin mover a EE.UU )

    ArregloDinamico<Equipo*> ordenados(48);
    for (int i = 0; i < equipos.getTamanio(); i++)
        if (i != idxUSA)
            ordenados.agregar(&equipos[i]);

    // Ordenar por ranking FIFA
    for (int i = 0; i < ordenados.getTamanio() - 1; i++)
        for (int j = 0; j < ordenados.getTamanio() - i - 1; j++)
            if (ordenados[j]->getRankingFIFA() >
                ordenados[j+1]->getRankingFIFA()) {
                Equipo* tmp    = ordenados[j];
                ordenados[j]   = ordenados[j+1];
                ordenados[j+1] = tmp;
            }

    // Bombo 1: EE.UU + los 11 mejores rankings
    if (idxUSA >= 0) bombo1.agregar(&equipos[idxUSA]);
    for (int i = 0; i < 11 && i < ordenados.getTamanio(); i++)
        bombo1.agregar(ordenados[i]);

    // Bombo 2: rankings 12-23
    for (int i = 11; i < 23 && i < ordenados.getTamanio(); i++)
        bombo2.agregar(ordenados[i]);

    // Bombo 3: rankings 24-35
    for (int i = 23; i < 35 && i < ordenados.getTamanio(); i++)
        bombo3.agregar(ordenados[i]);

    // Bombo 4: rankings 36-47
    for (int i = 35; i < ordenados.getTamanio(); i++)
        bombo4.agregar(ordenados[i]);

    cout << "Bombos formados:" << endl;
    cout << "Bombo 1: " << bombo1.getTamanio() << " equipos" << endl;
    cout << "Bombo 2: " << bombo2.getTamanio() << " equipos" << endl;
    cout << "Bombo 3: " << bombo3.getTamanio() << " equipos" << endl;
    cout << "Bombo 4: " << bombo4.getTamanio() << " equipos" << endl;
}

void Torneo::sortearGrupos() {
    resetContador();
    char letras[12] = {'A','B','C','D','E','F',
                       'G','H','I','J','K','L'};
    for (int i = 0; i < 12; i++)
        grupos.agregar(Grupo(letras[i]));

    // EE.UU va FIJO al grupo A como cabeza de serie
    Equipo* usa = nullptr;
    int idxUSA = -1;
    for (int i = 0; i < bombo1.getTamanio(); i++) {
        if (iguales(bombo1[i]->getPais(), "United States")) {
            usa    = bombo1[i];
            idxUSA = i;
            break;
        }

    }
    if (usa) {
        grupos[0].agregarEquipo(usa);  // Grupo A, posición fija
        bombo1.eliminarEn(idxUSA);
    }

    // Resto del bombo 1 se sortea aleatoriamente en grupos B-L
    for (int g = 1; g < 12; g++) {
        Equipo* eq = sacarAleatorio(bombo1);
        if (eq) grupos[g].agregarEquipo(eq);
    }

    // Bombos 2, 3 y 4 con restricciones de confederación
    ArregloDinamico<Equipo*>* bombos[3] = {&bombo2, &bombo3, &bombo4};

    for (int b = 0; b < 3; b++) {
        ArregloDinamico<Equipo*> copia(*bombos[b]);

        for (int g = 0; g < 12; g++) {
            bool asignado = false;
            int  intentos = copia.getTamanio();

            for (int t = 0; t < intentos && !asignado; t++) {
                int    idx = rand() % copia.getTamanio();
                Equipo* eq = copia[idx];

                if (puedeEntrarAlGrupo(grupos[g], eq)) {
                    grupos[g].agregarEquipo(eq);
                    copia.eliminarEn(idx);
                    int idxReal = bombos[b]->buscar(
                        [eq](Equipo* e){ return e == eq; });
                    if (idxReal >= 0)
                        bombos[b]->eliminarEn(idxReal);
                    asignado = true;
                }
            }

            if (!asignado && !copia.estaVacio()) {
                Equipo* eq = sacarAleatorio(copia);
                grupos[g].agregarEquipo(eq);
            }
        }
    }
    incrementar(3 * 12 * 12);
    mostrarMetricas("Sorteo de grupos");
}
void Torneo::imprimirGrupos() const {
    cout << "\n=== GRUPOS DEL MUNDIAL ===" << endl;
    for (int g = 0; g < grupos.getTamanio(); g++) {
        cout << "\nGRUPO " << grupos[g].getIdentificador() << ":" << endl;
        for (int e = 0; e < grupos[g].getEquipos().getTamanio(); e++) {
            Equipo* eq = grupos[g].getEquipos()[e];
            cout << "  " << eq->getPais()
                 << " [" << eq->getConfederacion() << "]"
                 << " Ranking:" << eq->getRankingFIFA()
                 << endl;
        }
    }
}

// ─── III — Simulación de grupos ────────────────────────────────────

void Torneo::programarYSimularGrupos() {
    for (int g = 0; g < grupos.getTamanio(); g++)
        grupos[g].generarPartidos(20, 6, 2026, partidosPorDia, 19);

    cout << "Simulando fase de grupos..." << endl;
    for (int g = 0; g < grupos.getTamanio(); g++) {
        grupos[g].simularPartidos();
        grupos[g].construirTabla();
    }

    for (int g = 0; g < grupos.getTamanio(); g++) {
        grupos[g].imprimirTabla();
        grupos[g].imprimirPartidos();
    }
}

void Torneo::construirTablas() {
    for (int g = 0; g < grupos.getTamanio(); g++)
        grupos[g].construirTabla();
}

// ─── Transición a R16 ──────────────────────────────────────────────

void Torneo::transicionR16() {
    resetContador();
    construirTablas();

    ArregloDinamico<Equipo*> primeros(12);
    ArregloDinamico<Equipo*> segundos(12);
    ArregloDinamico<Equipo*> terceros(12);
    ArregloDinamico<int>     puntosTerceros(12);

    for (int g = 0; g < grupos.getTamanio(); g++) {
        primeros.agregar(grupos[g].getPrimero());
        segundos.agregar(grupos[g].getSegundo());
        terceros.agregar(grupos[g].getTercero());
        puntosTerceros.agregar(grupos[g].getPuntosPorPosicion(2));
    }

    // Ordenar terceros por puntos (burbuja sobre arreglos paralelos)
    for (int i = 0; i < puntosTerceros.getTamanio() - 1; i++) {
        for (int j = 0; j < puntosTerceros.getTamanio() - i - 1; j++) {
            if (puntosTerceros[j] < puntosTerceros[j+1]) {
                int tmpP            = puntosTerceros[j];
                puntosTerceros[j]   = puntosTerceros[j+1];
                puntosTerceros[j+1] = tmpP;

                Equipo* tmpE  = terceros[j];
                terceros[j]   = terceros[j+1];
                terceros[j+1] = tmpE;
            }
        }
    }

    // Imprimir tablas
    cout << "\n=== TABLAS DE CLASIFICACION ===" << endl;
    for (int g = 0; g < grupos.getTamanio(); g++)
        grupos[g].imprimirTabla();

    cout << "\n8 mejores terceros:" << endl;
    for (int i = 0; i < 8 && i < terceros.getTamanio(); i++)
        cout << "  " << terceros[i]->getPais()
             << " Pts:" << puntosTerceros[i] << endl;

    construirR16(primeros, segundos, terceros);
    incrementar(12 + (12 * 12) + 32);
    mostrarMetricas("Transicion a R16");
}
void Torneo::construirR16(ArregloDinamico<Equipo*>& primeros,
                          ArregloDinamico<Equipo*>& segundos,
                          ArregloDinamico<Equipo*>& terceros) {

    // Regla 1: primeros vs 8 mejores terceros
    for (int i = 0; i < 8 && i < primeros.getTamanio(); i++) {
        Equipo* eqP = primeros[i];
        Equipo* eqT = terceros[i];

        if (eqP == eqT) {
            for (int j = 8; j < terceros.getTamanio(); j++) {
                if (terceros[j] != eqP) {
                    Equipo* tmp = terceros[i];
                    terceros[i] = terceros[j];
                    terceros[j] = tmp;
                    eqT = terceros[i];
                    break;
                }
            }
        }

        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  eqP, eqT);
        partidosR16.agregar(p);
    }

    // Regla 2: primeros restantes vs peores segundos
    for (int i = 8; i < primeros.getTamanio(); i++) {
        Equipo* eqP = primeros[i];
        Equipo* eqS = segundos[i];

        if (eqP == eqS && i + 1 < segundos.getTamanio()) {
            Equipo* tmp  = segundos[i];
            segundos[i]  = segundos[i+1];
            segundos[i+1]= tmp;
            eqS = segundos[i];
        }

        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  eqP, eqS);
        partidosR16.agregar(p);
    }

    // Regla 3: primeros 8 segundos entre si
    for (int i = 0; i < 8; i += 2) {
        Equipo* eqA = segundos[i];
        Equipo* eqB = segundos[i+1];

        if (eqA == eqB && i + 2 < segundos.getTamanio()) {
            Equipo* tmp  = segundos[i+1];
            segundos[i+1]= segundos[i+2];
            segundos[i+2]= tmp;
            eqB = segundos[i+1];
        }

        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  eqA, eqB);
        partidosR16.agregar(p);
    }

    cout << "\nPartidos R16 configurados: "
         << partidosR16.getTamanio() << endl;
    for (int i = 0; i < partidosR16.getTamanio(); i++)
        cout << "  " << partidosR16[i] << endl;
}
// ─── Etapas eliminatorias ──────────────────────────────────────────

Equipo* Torneo::romperEmpate(Equipo* eq1, Equipo* eq2,
                             Partido& partido) {
    int r1    = eq1->getRankingFIFA();
    int r2    = eq2->getRankingFIFA();
    int total = r1 + r2;
    int dado  = rand() % total;
    Equipo* ganador = (dado < r2) ? eq1 : eq2;

    // Determinar cuál EstPartidoEquipo corresponde al ganador
    EstPartidoEquipo& statsGanador =
        (ganador == eq1) ? partido.getStatsEq1()
                         : partido.getStatsEq2();

    // Sumar el gol al marcador del equipo ganador
    statsGanador.setGolesAFavor(statsGanador.getGolesAFavor() + 1);

    // Asignar ese gol a un jugador aleatorio de los 11 convocados
    if (statsGanador.getJugadores().getTamanio() > 0) {
        int idxAleatorio = rand() %
                           statsGanador.getJugadores().getTamanio();
        EstPartidoJugador& jugador =
            statsGanador.getJugadores()[idxAleatorio];
        jugador.setGoles(jugador.getGoles() + 1);
    }

    // Actualizar goles en contra del perdedor
    EstPartidoEquipo& statsPerdedor =
        (ganador == eq1) ? partido.getStatsEq2()
                         : partido.getStatsEq1();
    statsPerdedor.setGolesEnContra(statsGanador.getGolesAFavor());

    // Marcar 120 minutos a todos los jugadores (prórroga)
    for (int i = 0;
         i < partido.getStatsEq1().getJugadores().getTamanio(); i++)
        partido.getStatsEq1().getJugadores()[i].setMinutosJugados(120);
    for (int i = 0;
         i < partido.getStatsEq2().getJugadores().getTamanio(); i++)
        partido.getStatsEq2().getJugadores()[i].setMinutosJugados(120);


    partido.setFueAProrroga(true);
    // Guardar el ganador en el partido
    partido.setGanador(ganador);

    return ganador;
}
void Torneo::simularEtapa(ArregloDinamico<Partido>& partidos, ArregloDinamico<Equipo*>& ganadores, const char* nombreEtapa) {
    cout << "\n=== " << nombreEtapa << " ===" << endl;

    for (int i = 0; i < partidos.getTamanio(); i++) {
        partidos[i].simular();

        // Si empate, romper con prorroga
        if (partidos[i].getGanador() == nullptr) {
            Equipo* g = romperEmpate(
                partidos[i].getEquipo1(),
                partidos[i].getEquipo2(),
                partidos[i]);
            ganadores.agregar(g);
        } else {
            ganadores.agregar(partidos[i].getGanador());
        }

        partidos[i].actualizarHistoricos();
        partidos[i].imprimir();
        partidos[i].imprimirGoleadores();
        cout << "---" << endl;
    }
}

void Torneo::simularR16() {
    ArregloDinamico<Equipo*> ganadores(32);
    simularEtapa(partidosR16, ganadores, "DIECISEISAVOS DE FINAL (R16)");

    for (int i = 0; i < ganadores.getTamanio(); i += 2) {
        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  ganadores[i], ganadores[i+1]);
        partidosR8.agregar(p);
    }
}

void Torneo::simularR8() {
    ArregloDinamico<Equipo*> ganadores(16);
    simularEtapa(partidosR8, ganadores, "OCTAVOS DE FINAL (R8)");

    for (int i = 0; i < ganadores.getTamanio(); i += 2) {
        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  ganadores[i], ganadores[i+1]);
        partidosQF.agregar(p);
    }
}

void Torneo::simularQF() {
    ArregloDinamico<Equipo*> ganadores(8);

    cout << "\n=== CUARTOS DE FINAL (QF) ===" << endl;
    for (int i = 0; i < partidosQF.getTamanio(); i++) {
        partidosQF[i].simular();
        Equipo* g = partidosQF[i].getGanador();
        if (!g) g = romperEmpate(
                partidosQF[i].getEquipo1(),
                partidosQF[i].getEquipo2(),
                partidosQF[i]);
        ganadores.agregar(g);
        partidosQF[i].actualizarHistoricos();
        partidosQF[i].imprimir();
        partidosQF[i].imprimirGoleadores();
        cout << "---" << endl;
    }

    for (int i = 0; i < ganadores.getTamanio(); i += 2) {
        Partido p(10, 7, 2026,
                  "nombreSede",
                  "codArbitro1", "codArbitro2", "codArbitro3",
                  ganadores[i], ganadores[i+1]);
        partidosSF.agregar(p);
    }
}

void Torneo::simularSF() {
    ArregloDinamico<Equipo*> ganadores(4);
    ArregloDinamico<Equipo*> perdedores(4);

    cout << "\n=== SEMIFINALES ===" << endl;
    for (int i = 0; i < partidosSF.getTamanio(); i++) {
        partidosSF[i].simular();
        Equipo* g = partidosSF[i].getGanador();
        if (!g) g = romperEmpate(
                partidosSF[i].getEquipo1(),
                partidosSF[i].getEquipo2(),
                partidosSF[i]);

        Equipo* per = (g == partidosSF[i].getEquipo1())
                          ? partidosSF[i].getEquipo2()
                          : partidosSF[i].getEquipo1();

        ganadores.agregar(g);
        perdedores.agregar(per);
        partidosSF[i].actualizarHistoricos();
        partidosSF[i].imprimir();
        partidosSF[i].imprimirGoleadores();
        cout << "---" << endl;
    }

    // Partido 3er puesto
    Partido tercerPuesto(10, 7, 2026,
                         "nombreSede",
                         "codArbitro1", "codArbitro2", "codArbitro3",
                         perdedores[0], perdedores[1]);
    partidosFinal.agregar(tercerPuesto);

    // Final
    Partido partidoFinal(10, 7, 2026,
                         "nombreSede",
                         "codArbitro1", "codArbitro2", "codArbitro3",
                         ganadores[0], ganadores[1]);
    partidosFinal.agregar(partidoFinal);
}

void Torneo::simularFinal() {
    resetContador();
    cout << "\n=== PARTIDO 3ER PUESTO ===" << endl;
    partidosFinal[0].simular();
    Equipo* g3 = partidosFinal[0].getGanador();
    if (!g3) g3 = romperEmpate(
            partidosFinal[0].getEquipo1(),
            partidosFinal[0].getEquipo2(),
            partidosFinal[0]);
    partidosFinal[0].actualizarHistoricos();
    partidosFinal[0].imprimir();

    cout << "\n=== FINAL ===" << endl;
    partidosFinal[1].simular();
    Equipo* campeon = partidosFinal[1].getGanador();
    if (!campeon) campeon = romperEmpate(
            partidosFinal[1].getEquipo1(),
            partidosFinal[1].getEquipo2(),
            partidosFinal[1]);
    partidosFinal[1].actualizarHistoricos();
    partidosFinal[1].imprimir();
    partidosQF[1].imprimirGoleadores();

    cout << "\n CAMPEON DEL MUNDO: "
         << campeon->getPais() << endl;
    incrementar(48 * 26);
    mostrarMetricas("Generacion de estadisticas");
}


// ─── Medidor de recursos ───────────────────────────────────────────

void Torneo::resetContador() {
    contadorIteraciones = 0;
}

void Torneo::incrementar(int n) {
    contadorIteraciones += n;
}

// Calcula memoria de UN jugador
static size_t memoriaJugador(const Jugador& j) {
    size_t mem = sizeof(Jugador);
    // char* nombre y apellido son dinámicos
    int lenNom = 0; const char* n = j.getNombre();
    while (n && n[lenNom]) lenNom++;
    int lenApe = 0; const char* a = j.getApellido();
    while (a && a[lenApe]) lenApe++;
    mem += (lenNom + 1) + (lenApe + 1);
    return mem;
}

// Calcula memoria de UN equipo
static size_t memoriaEquipo(const Equipo& eq) {
    size_t mem = sizeof(Equipo);
    // char* dinámicos
    int len = 0;
    const char* p = eq.getPais();
    while (p && p[len]) len++; mem += len + 1;
    len = 0;
    const char* c = eq.getConfederacion();
    while (c && c[len]) len++; mem += len + 1;
    len = 0;
    const char* f = eq.getFederacion();
    while (f && f[len]) len++; mem += len + 1;
    len = 0;
    const char* d = eq.getDirectorTecnico();
    while (d && d[len]) len++; mem += len + 1;
    // Plantilla de jugadores (ArregloDinamico interno)
    mem += sizeof(Jugador) * eq.getPlantilla().getCapacidad();
    for (int j = 0; j < eq.getPlantilla().getTamanio(); j++)
        mem += memoriaJugador(eq.getPlantilla()[j])
               - sizeof(Jugador); // solo la parte dinámica
    return mem;
}

// Calcula memoria de UN partido
static size_t memoriaPartido(const Partido& p) {
    size_t mem = sizeof(Partido);
    // char* sede y árbitros
    const char* s = p.getSede();
    int len = 0;
    while (s && s[len]) len++; mem += len + 1;
    mem += 3 * 15; // 3 árbitros fijos "codArbitroN"
    // EstPartidoEquipo tiene ArregloDinamico de 11 jugadores
    mem += 2 * sizeof(EstPartidoJugador) * 11;
    return mem;
}

size_t Torneo::memoriaTotal() const {
    size_t total = sizeof(Torneo); // base

    // 48 equipos con sus jugadores
    size_t memEquipos = sizeof(Equipo) * equipos.getCapacidad();
    for (int i = 0; i < equipos.getTamanio(); i++)
        memEquipos += memoriaEquipo(equipos[i]) - sizeof(Equipo);
    total += memEquipos;

    // 12 grupos con sus partidos
    size_t memGrupos = sizeof(Grupo) * grupos.getCapacidad();
    for (int g = 0; g < grupos.getTamanio(); g++) {
        size_t memPart = sizeof(Partido) *
                         grupos[g].getPartidos().getCapacidad();
        for (int p = 0; p < grupos[g].getPartidos().getTamanio(); p++)
            memPart += memoriaPartido(grupos[g].getPartidos()[p])
                       - sizeof(Partido);
        memGrupos += memPart;
    }
    total += memGrupos;

    // Partidos de eliminatorias
    total += sizeof(Partido) * partidosR16.getCapacidad();
    total += sizeof(Partido) * partidosR8.getCapacidad();
    total += sizeof(Partido) * partidosQF.getCapacidad();
    total += sizeof(Partido) * partidosSF.getCapacidad();
    total += sizeof(Partido) * partidosFinal.getCapacidad();

    return total;
}

void Torneo::mostrarMetricas(const char* nombreFuncionalidad) const {
    cout << "\n--- Metricas: " << nombreFuncionalidad << " ---" << endl;
    cout << "Iteraciones realizadas: " << contadorIteraciones << endl;
    cout << "Memoria consumida:      " << memoriaTotal()
         << " bytes ("
         << memoriaTotal() / 1024
         << " KB)" << endl;
    cout << "--------------------------------------" << endl;
}
// ─── IV — Estadísticas finales ─────────────────────────────────────

void Torneo::generarEstadisticas() {
    cout << "\n=== ESTADISTICAS FINALES ===" << endl;

    if (partidosFinal.getTamanio() < 2) {
        cout << "ERROR: La final no ha sido jugada aun." << endl;
        return;
    }

    // ─── Obtener los 4 puestos de forma segura ────────────────────
    Equipo* campeon    = nullptr;
    Equipo* subcampeon = nullptr;
    Equipo* tercero    = nullptr;
    Equipo* cuarto     = nullptr;

    // Partido final (índice 1)
    Partido& partidoFinal = partidosFinal[1];
    if (partidoFinal.getEquipo1() && partidoFinal.getEquipo2()) {
        campeon = partidoFinal.getGanador();
        if (!campeon) {
            campeon = (partidoFinal.getEquipo1()->getRankingFIFA() < partidoFinal.getEquipo2()->getRankingFIFA()) ?
                          partidoFinal.getEquipo1() : partidoFinal.getEquipo2();
        }
        subcampeon = (campeon == partidoFinal.getEquipo1())
                         ? partidoFinal.getEquipo2()
                         : partidoFinal.getEquipo1();
    }

    // Partido 3er puesto (índice 0)
    Partido& partidoTercerPuesto = partidosFinal[0];
    if (partidoTercerPuesto.getEquipo1() &&
        partidoTercerPuesto.getEquipo2()) {
        tercero = partidoTercerPuesto.getGanador();
        if (!tercero) {
            tercero = (partidoTercerPuesto.getEquipo1()->getRankingFIFA() < partidoTercerPuesto.getEquipo2()->getRankingFIFA()) ?
                          partidoTercerPuesto.getEquipo1() : partidoTercerPuesto.getEquipo2();
        }
        cuarto = (tercero == partidoTercerPuesto.getEquipo1())
                     ? partidoTercerPuesto.getEquipo2()
                     : partidoTercerPuesto.getEquipo1();
    }

    // ─── Top 4 ────────────────────────────────────────────────────
    cout << "\n--- Top 4 del Mundial ---" << endl;
    if (campeon)    cout << "1ro: " << campeon->getPais()    << endl;
    if (subcampeon) cout << "2do: " << subcampeon->getPais() << endl;
    if (tercero)    cout << "3ro: " << tercero->getPais()    << endl;
    if (cuarto)     cout << "4to: " << cuarto->getPais()     << endl;

    // ─── Máximo goleador del campeón ──────────────────────────────
    if (campeon) {
        cout << "\n--- Maximo goleador de "
             << campeon->getPais() << " ---" << endl;
        int maxGoles = -1;
        int maxIdx   = -1;
        for (int j = 0; j < campeon->getPlantilla().getTamanio(); j++) {
            int g = campeon->getPlantilla()[j]
                        .getEstHistorica().getGoles();
            if (g > maxGoles) {
                maxGoles = g;
                maxIdx   = j;
            }
        }
        if (maxIdx >= 0) {
            campeon->getPlantilla()[maxIdx].imprimir();
            cout << endl;
        } else {
            cout << "Sin goleadores registrados." << endl;
        }
    }

    // ─── Top 3 goleadores del torneo ──────────────────────────────
    cout << "\n--- Top 3 Goleadores del Torneo ---" << endl;

    Equipo* topEq[3]  = {nullptr, nullptr, nullptr};
    int     topCam[3] = {0, 0, 0};
    int     topGol[3] = {0, 0, 0};

    for (int i = 0; i < equipos.getTamanio(); i++) {
        for (int j = 0; j < equipos[i].getPlantilla().getTamanio(); j++) {
            int g = equipos[i].getPlantilla()[j]
                        .getEstHistorica().getGoles();
            if (g > topGol[2]) {
                topGol[2] = g;
                topEq[2]  = &equipos[i];
                topCam[2] = equipos[i].getPlantilla()[j]
                                .getNumeroCamiseta();
                // Burbuja para mantener ordenado el top 3
                for (int k = 2; k > 0 && topGol[k] > topGol[k-1]; k--) {
                    int     tg  = topGol[k];
                    topGol[k]   = topGol[k-1];
                    topGol[k-1] = tg;

                    int     tc  = topCam[k];
                    topCam[k]   = topCam[k-1];
                    topCam[k-1] = tc;

                    Equipo* te = topEq[k];
                    topEq[k]   = topEq[k-1];
                    topEq[k-1] = te;
                }
            }
        }
    }

    for (int k = 0; k < 3; k++) {
        if (topEq[k])
            cout << k+1 << ". " << topEq[k]->getPais()
                 << " | Camiseta #" << topCam[k]
                 << " | Goles: "    << topGol[k] << endl;
        else
            cout << k+1 << ". Sin datos" << endl;
    }

    // ─── Equipo con más goles históricos ──────────────────────────
    cout << "\n--- Equipo con mas goles historicos ---" << endl;
    int     maxGF = -1;
    Equipo* maxEq = nullptr;
    for (int i = 0; i < equipos.getTamanio(); i++) {
        int gf = equipos[i].getEstHistorica().getGolesAFavor();
        if (gf > maxGF) {
            maxGF = gf;
            maxEq = &equipos[i];
        }
    }
    if (maxEq)
        cout << maxEq->getPais()
             << " | GF historicos: " << maxGF << endl;

    // ─── Confederación con mayor presencia por etapa ──────────────
    cout << "\n--- Confederacion con mayor presencia ---" << endl;

    const char* etapas[3] = {"R16", "R8", "QF"};
    ArregloDinamico<Partido>* etapaPartidos[3] = {
        &partidosR16, &partidosR8, &partidosQF
    };

    for (int e = 0; e < 3; e++) {
        const char* confs[6] = {
            "UEFA", "CONMEBOL", "Concacaf", "CAF", "AFC", "OFC"
        };
        int counts[6] = {0, 0, 0, 0, 0, 0};

        for (int p = 0; p < etapaPartidos[e]->getTamanio(); p++) {
            Equipo* eq1 = (*etapaPartidos[e])[p].getEquipo1();
            Equipo* eq2 = (*etapaPartidos[e])[p].getEquipo2();

            for (int c = 0; c < 6; c++) {
                if (eq1 && iguales(eq1->getConfederacion(), confs[c]))
                    counts[c]++;
                if (eq2 && iguales(eq2->getConfederacion(), confs[c]))
                    counts[c]++;
            }
        }

        int maxCount = -1;
        int maxConf  = 0;
        for (int c = 0; c < 6; c++) {
            if (counts[c] > maxCount) {
                maxCount = counts[c];
                maxConf  = c;
            }
        }
        cout << etapas[e] << ": " << confs[maxConf]
             << " (" << maxCount << " equipos)" << endl;
    }
}

// ─── Menú principal ────────────────────────────────────────────────

void Torneo::ejecutarMenu() {
    srand(time(nullptr));
    int opcion = 0;

    do {
        cout << "\n==============================" << endl;
        cout << "   UdeAWorldCup - Menu"          << endl;
        cout << "==============================" << endl;
        cout << "1.  Cargar equipos desde CSV"    << endl;
        cout << "2.  Formar bombos y sortear grupos" << endl;
        cout << "3.  Simular fase de grupos"      << endl;
        cout << "4.  Transicion a R16"            << endl;
        cout << "5.  Simular R16"                 << endl;
        cout << "6.  Simular Octavos (R8)"        << endl;
        cout << "7.  Simular Cuartos (QF)"        << endl;
        cout << "8.  Simular Semifinales"         << endl;
        cout << "9.  Simular Final"               << endl;
        cout << "10. Estadisticas finales"        << endl;
        cout << "0.  Salir"                       << endl;
        cout << "Opcion: ";
        cin  >> opcion;

        switch(opcion) {
        case 1:
            cargarEquipos("datos/selecciones_clasificadas_mundial.csv");
            cargarJugadores("datos/jugadores.csv");
            equiposCargados = true;
            break;

        case 2:
            if (!equiposCargados) {
                cout << "ERROR: Primero cargue los equipos (opcion 1)" << endl;
                break;
            }
            formarBombos();
            sortearGrupos();
            imprimirGrupos();
            gruposSorteados = true;
            break;

        case 3:
            if (!gruposSorteados) {
                cout << "ERROR: Primero sortee los grupos (opcion 2)" << endl;
                break;
            }
            programarYSimularGrupos();
            gruposSimulados = true;
            break;

        case 4:
            if (!gruposSimulados) {
                cout << "ERROR: Primero simule los grupos (opcion 3)" << endl;
                break;
            }
            transicionR16();
            r16Construido = true;
            break;

        case 5:
            if (!r16Construido) {
                cout << "ERROR: Primero construya el R16 (opcion 4)" << endl;
                break;
            }
            simularR16();
            r16Simulado = true;
            break;

        case 6:
            if (!r16Simulado) {
                cout << "ERROR: Primero simule el R16 (opcion 5)" << endl;
                break;
            }
            simularR8();
            r8Simulado = true;
            break;

        case 7:
            if (!r8Simulado) {
                cout << "ERROR: Primero simule los octavos (opcion 6)" << endl;
                break;
            }
            simularQF();
            qfSimulado = true;
            break;

        case 8:
            if (!qfSimulado) {
                cout << "ERROR: Primero simule los cuartos (opcion 7)" << endl;
                break;
            }
            simularSF();
            sfSimulado = true;
            break;

        case 9:
            if (!sfSimulado) {
                cout << "ERROR: Primero simule las semifinales (opcion 8)" << endl;
                break;
            }
            simularFinal();
            finalSimulado = true;
            break;

        case 10:
            if (!finalSimulado) {
                cout << "ERROR: Primero simule la final (opcion 9)" << endl;
                break;
            }
            generarEstadisticas();
            guardarJugadores("datos/jugadores.csv");
            break;

        case 0:
            cout << "Saliendo..." << endl;
            break;

        default:
            cout << "Opcion invalida" << endl;
        }
    } while (opcion != 0);
}
