#include "simulation.h"

#include <cinttypes>

namespace eagle {

namespace {

void write_row(std::FILE* csv, const LanderParams& p, const LanderState& s)
{
    std::fprintf(csv, "%.2f,%.6f,%.6f,%.6f,%.3f\n",
                 static_cast<double>(s.tick) * kTickSeconds,
                 s.altitude_m, s.velocity_mps, total_mass(p, s), s.thrust_n);
}

}  // namespace

LanderState run_simulation(const SimConfig& cfg, std::FILE* csv)
{
    LanderState s{cfg.altitude_m, cfg.velocity_mps, cfg.fuel_kg, 0.0, 0, false};

    if (csv) {
        std::fprintf(csv, "# seed=%" PRIu64 "\n", cfg.seed);
        std::fprintf(csv, "time_s,altitude_m,velocity_mps,mass_kg,thrust_n\n");
        write_row(csv, cfg.lander, s);
    }

    while (!s.landed && s.tick < cfg.max_ticks) {
        step(cfg.lander, s, cfg.thrust_n);
        if (csv) write_row(csv, cfg.lander, s);
    }
    return s;
}

}  // namespace eagle
