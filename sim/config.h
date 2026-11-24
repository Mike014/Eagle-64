/*
 * Configurazione di una simulazione, letta da un file di testo
 * "chiave = valore" (righe vuote e righe che iniziano con # ignorate).
 * Una chiave sconosciuta è un errore: un refuso non deve passare in
 * silenzio lasciando il valore di default.
 *
 * Il seed non è ancora usato (nello Sprint 2a non c'è niente di casuale):
 * è già nel formato perché il rumore dei sensori (Sprint 3) lo userà, e
 * viene scritto nel CSV per sapere da quale esecuzione viene.
 */
#ifndef EAGLE_SIM_CONFIG_H
#define EAGLE_SIM_CONFIG_H

#include <cstdint>
#include <string>

#include "lander.h"

namespace eagle {

struct SimConfig {
    LanderParams lander{2150.0, 311.0, 45040.0};  // ordini di grandezza del LM
    double altitude_m = 100.0;
    double velocity_mps = 0.0;
    double fuel_kg = 400.0;
    double thrust_n = 0.0;             // spinta costante (Sprint 2a)
    unsigned long max_ticks = 100000;  // limite di sicurezza: 1000 s
    std::uint64_t seed = 0;
};

// Legge il file sopra i valori già presenti in cfg. In caso di errore
// restituisce false e descrive il problema in *error.
bool load_config(const std::string& path, SimConfig& cfg, std::string* error);

}  // namespace eagle

#endif
