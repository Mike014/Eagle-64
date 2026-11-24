/*
 * Test della Definition of Done dello Sprint 2a:
 *   1. a spinta zero il lander tocca il suolo nel tempo della caduta
 *      libera t = sqrt(2 h0 / g), con velocità g t;
 *   2. due esecuzioni con la stessa configurazione producono un CSV
 *      identico byte per byte.
 * Più due controlli di contorno: limiti della spinta e chiavi di
 * configurazione sconosciute.
 */
#include <cmath>
#include <cstdio>
#include <string>

#include "config.h"
#include "simulation.h"
#include "test.h"

using namespace eagle;

static void test_freefall_time(void)
{
    SimConfig cfg;
    cfg.altitude_m = 100.0;
    cfg.thrust_n = 0.0;

    const LanderState end = run_simulation(cfg, nullptr);
    const double t_expected = std::sqrt(2.0 * cfg.altitude_m / kMoonGravity);
    const double t_sim = static_cast<double>(end.tick) * kTickSeconds;

    CHECK(end.landed);
    // Euler semi-implicito anticipa il contatto di mezzo tick e il passo
    // discreto aggiunge al massimo un tick: tolleranza 2 tick.
    CHECK(std::fabs(t_sim - t_expected) <= 2 * kTickSeconds);
    CHECK(std::fabs(-end.velocity_mps - kMoonGravity * t_expected) <= 0.05);
    CHECK(end.fuel_kg == cfg.fuel_kg);  // motore spento: niente consumo
}

static void test_thrust_limits(void)
{
    // Spinta oltre il massimo: viene limitata a max_thrust_n.
    LanderParams p{2150.0, 311.0, 45040.0};
    LanderState s{100.0, 0.0, 400.0, 0.0, 0, false};
    step(p, s, 1e9);
    CHECK(s.thrust_n == p.max_thrust_n);

    // Senza propellente la spinta è zero.
    LanderState empty{100.0, 0.0, 0.0, 0.0, 0, false};
    step(p, empty, 10000.0);
    CHECK(empty.thrust_n == 0.0);
    CHECK(empty.fuel_kg == 0.0);
}

static std::string read_file(const char* path)
{
    std::string data;
    std::FILE* f = nullptr;
    if (fopen_s(&f, path, "rb") != 0 || !f) return data;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) data.append(buf, n);
    std::fclose(f);
    return data;
}

static void write_run(const SimConfig& cfg, const char* path)
{
    std::FILE* f = nullptr;
    if (fopen_s(&f, path, "wb") != 0 || !f) return;
    run_simulation(cfg, f);
    std::fclose(f);
}

static void test_replay_identical(void)
{
    SimConfig cfg;
    cfg.altitude_m = 500.0;
    cfg.velocity_mps = -10.0;
    cfg.thrust_n = 3500.0;
    cfg.seed = 42;

    write_run(cfg, "replay_a.csv");
    write_run(cfg, "replay_b.csv");
    const std::string a = read_file("replay_a.csv");
    const std::string b = read_file("replay_b.csv");

    CHECK(!a.empty());
    CHECK(a == b);
    std::remove("replay_a.csv");
    std::remove("replay_b.csv");
}

static void test_config_rejects_unknown_key(void)
{
    std::FILE* f = nullptr;
    if (fopen_s(&f, "bad.cfg", "wb") == 0 && f) {
        std::fputs("altitudine_m = 100\n", f);  // refuso voluto
        std::fclose(f);
    }
    SimConfig cfg;
    std::string error;
    CHECK(!load_config("bad.cfg", cfg, &error));
    CHECK(error.find("altitudine_m") != std::string::npos);
    std::remove("bad.cfg");
}

int main(void)
{
    test_freefall_time();
    test_thrust_limits();
    test_replay_identical();
    test_config_rejects_unknown_key();
    TEST_MAIN_END();
}
