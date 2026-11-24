#include "lander.h"

#include <algorithm>

namespace eagle {

double total_mass(const LanderParams& p, const LanderState& s)
{
    return p.dry_mass_kg + s.fuel_kg;
}

void step(const LanderParams& p, LanderState& s, double thrust_cmd_n)
{
    if (s.landed) {
        return;
    }

    double thrust = std::clamp(thrust_cmd_n, 0.0, p.max_thrust_n);

    // Propellente necessario per questo tick; se non basta, la spinta
    // si riduce in proporzione a quello che resta.
    double fuel_needed = thrust / (p.isp_s * kEarthG0) * kTickSeconds;
    if (fuel_needed > s.fuel_kg) {
        thrust = thrust * (s.fuel_kg / fuel_needed);
        fuel_needed = s.fuel_kg;
    }

    const double accel = thrust / total_mass(p, s) - kMoonGravity;

    s.velocity_mps += accel * kTickSeconds;           // velocità nuova...
    s.altitude_m   += s.velocity_mps * kTickSeconds;  // ...usata qui
    s.fuel_kg      -= fuel_needed;
    s.thrust_n      = thrust;
    s.tick         += 1;

    if (s.altitude_m <= 0.0) {
        s.altitude_m = 0.0;
        s.landed = true;
    }
}

}  // namespace eagle
