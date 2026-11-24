/*
 * Esegue una simulazione completa e scrive la telemetria in CSV.
 *
 * Il CSV deve essere identico byte per byte a parità di configurazione,
 * seed ed eseguibile: i numeri si formattano con fprintf e un numero
 * fisso di decimali, e chi apre il file lo apre in modalità binaria
 * ("wb"), così su Windows le righe finiscono con \n e non con \r\n.
 */
#ifndef EAGLE_SIM_SIMULATION_H
#define EAGLE_SIM_SIMULATION_H

#include <cstdio>

#include "config.h"

namespace eagle {

// Simula fino al contatto col suolo (o a max_ticks). Se csv non è nullo
// scrive l'intestazione, lo stato iniziale e una riga per tick.
// Restituisce lo stato finale.
LanderState run_simulation(const SimConfig& cfg, std::FILE* csv);

}  // namespace eagle

#endif
