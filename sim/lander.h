/*
 * Modello fisico 1D del lander: discesa verticale sulla Luna.
 *
 * Decisioni (vedi docs/PLAN.md, "Decisioni di base"):
 *   - double, non fixed-point;
 *   - un tick = 10 ms, passo fisso;
 *   - asse y verso l'alto: quota > 0 in volo, velocità < 0 in discesa.
 *
 * Integrazione: Euler semi-implicito. Prima si aggiorna la velocità con
 * l'accelerazione, poi la quota con la velocità NUOVA. Con accelerazione
 * costante la velocità è esatta; la quota anticipa la soluzione esatta
 * di mezzo tick (h_n = h0 - g dt^2 n(n+1)/2 invece di h0 - g (n dt)^2 / 2).
 *
 * Propellente: si consuma m_dot = F / (Isp * g0). Quando finisce, la spinta
 * si azzera. L'accelerazione usa la massa a inizio tick.
 */
#ifndef EAGLE_SIM_LANDER_H
#define EAGLE_SIM_LANDER_H

namespace eagle {

constexpr double kMoonGravity = 1.62;     // m/s^2
constexpr double kEarthG0     = 9.80665;  // m/s^2, definizione di Isp
constexpr double kTickSeconds = 0.01;     // un tick = 10 ms

struct LanderParams {
    double dry_mass_kg;   // massa a vuoto
    double isp_s;         // impulso specifico del motore
    double max_thrust_n;  // spinta massima
};

struct LanderState {
    double altitude_m;    // quota sul suolo, > 0 in volo
    double velocity_mps;  // verticale, negativa = in discesa
    double fuel_kg;       // propellente rimasto
    double thrust_n;      // spinta applicata nell'ultimo tick
    unsigned long tick;   // tick trascorsi
    bool landed;          // true dal tick in cui tocca il suolo
};

double total_mass(const LanderParams& p, const LanderState& s);

// Avanza di un tick. La spinta richiesta viene limitata a [0, max_thrust]
// e al propellente disponibile. Al contatto col suolo la quota vale 0,
// landed = true e velocity_mps resta la velocità d'impatto.
void step(const LanderParams& p, LanderState& s, double thrust_cmd_n);

}  // namespace eagle

#endif
