// eagle_sim <config> <telemetria.csv>
// Simula la discesa con spinta costante e scrive la telemetria in CSV.
#include <cmath>
#include <cstdio>
#include <string>

#include "config.h"
#include "simulation.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::fprintf(stderr, "uso: %s <config> <telemetria.csv>\n", argv[0]);
        return 2;
    }

    eagle::SimConfig cfg;
    std::string error;
    if (!eagle::load_config(argv[1], cfg, &error)) {
        std::fprintf(stderr, "errore: %s\n", error.c_str());
        return 2;
    }

    std::FILE* csv = nullptr;
    if (fopen_s(&csv, argv[2], "wb") != 0 || !csv) {
        std::fprintf(stderr, "errore: impossibile scrivere %s\n", argv[2]);
        return 2;
    }
    const eagle::LanderState end = eagle::run_simulation(cfg, csv);
    std::fclose(csv);

    const double t = static_cast<double>(end.tick) * eagle::kTickSeconds;
    if (!end.landed) {
        std::printf("Ancora in volo dopo %.2f s (max_ticks)\n", t);
        return 0;
    }

    // Limiti di touchdown da docs/PLAN.md.
    const double v = std::fabs(end.velocity_mps);
    const char* verdict = v <= 1.0 ? "atterraggio morbido"
                        : v <= 3.0 ? "atterraggio duro, entro il limite strutturale"
                                   : "schianto";
    std::printf("Contatto a t = %.2f s, velocita' %.2f m/s: %s\n", t, v, verdict);
    return 0;
}
