#include "config.h"

#include <fstream>
#include <stdexcept>

namespace eagle {

namespace {

std::string trim(const std::string& s)
{
    const char* ws = " \t\r";
    const auto b = s.find_first_not_of(ws);
    if (b == std::string::npos) return "";
    const auto e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

}  // namespace

bool load_config(const std::string& path, SimConfig& cfg, std::string* error)
{
    std::ifstream in(path);
    if (!in) {
        *error = "impossibile aprire " + path;
        return false;
    }

    std::string line;
    int line_no = 0;
    while (std::getline(in, line)) {
        line_no++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        const std::string where = path + ":" + std::to_string(line_no) + ": ";
        const auto eq = line.find('=');
        if (eq == std::string::npos) {
            *error = where + "manca '='";
            return false;
        }
        const std::string key = trim(line.substr(0, eq));
        const std::string val = trim(line.substr(eq + 1));

        try {
            if      (key == "dry_mass_kg")  cfg.lander.dry_mass_kg = std::stod(val);
            else if (key == "isp_s")        cfg.lander.isp_s = std::stod(val);
            else if (key == "max_thrust_n") cfg.lander.max_thrust_n = std::stod(val);
            else if (key == "altitude_m")   cfg.altitude_m = std::stod(val);
            else if (key == "velocity_mps") cfg.velocity_mps = std::stod(val);
            else if (key == "fuel_kg")      cfg.fuel_kg = std::stod(val);
            else if (key == "thrust_n")     cfg.thrust_n = std::stod(val);
            else if (key == "max_ticks")    cfg.max_ticks = std::stoul(val);
            else if (key == "seed")         cfg.seed = std::stoull(val);
            else {
                *error = where + "chiave sconosciuta '" + key + "'";
                return false;
            }
        } catch (const std::exception&) {
            *error = where + "valore non valido per '" + key + "'";
            return false;
        }
    }
    return true;
}

}  // namespace eagle
