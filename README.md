# Eagle-64

**AI-Assisted Lunar Guidance Simulator**: un simulatore di discesa e allunaggio che reinterpreta i principi dell'Apollo Guidance Computer di Apollo 11 su Windows x86-64.

Eagle era il modulo lunare di Apollo 11; 64 è l'architettura x86-64 su cui gira il progetto.

> Stato: Sprint 0 (setup). Nessun codice ancora scritto. Vedi [`docs/PLAN.md`](docs/PLAN.md).

## Principio guida

Il Flight Core deterministico mantiene sempre l'autorità sulla missione. L'IA opera in modalità shadow/advisory: osserva, segnala e suggerisce, ma non comanda mai direttamente motore, fasi di volo, abort o touchdown.

## Architettura

| Strato | Linguaggio | Cartella | Ruolo |
| --- | --- | --- | --- |
| Low-level | NASM x86-64 | `asm/` | Fixed-point, vettori, checksum, benchmark |
| Flight Core | C17 | `flight-core/` | Scheduler, navigazione, guida, controllo, allarmi, restart |
| Safety Monitor | C17 | `safety-monitor/` | Valida i vincoli e gli output IA, gestisce il fallback |
| Digital Twin | C++20 | `simulator/` | Fisica, sensori, attuatori, terreno, DSKY |
| AI Lab | Python | `ai/` | Dataset, feature, modelli, incertezza, export ONNX |
| Inferenza | C++20 + ONNX Runtime | `inference/` | Esecuzione locale dei modelli in shadow mode |
| Cyber Range | C++20 / Python | `cyber-range/` | Ground station, protocollo autenticato, attacchi, detection |

## Struttura del repository

```text
Eagle-64/
├── docs/            piano, architettura, ADR, note storiche, threat model, assurance case, model card
├── asm/             fixed_point, vectors, checksum
├── flight-core/     scheduler, navigation, guidance, control, alarms, restart
├── safety-monitor/
├── simulator/       dynamics, sensors, actuators, terrain, dsky
├── ai/              datasets, features, baselines, temporal, uncertainty, export
├── inference/
├── cyber-range/     ground-station, protocol, attacks, detection
├── scenarios/       scenari di missione versionati
├── tests/           unit, integration, differential, fault-injection, adversarial
├── benchmarks/
└── tools/
```

## Toolchain prevista

- Git
- CMake 3.28 o superiore
- Visual Studio 2022 Build Tools (MSVC) oppure LLVM/Clang
- NASM
- Python 3.12 o superiore
- ONNX Runtime (dallo Sprint 5)

## Riferimenti

- Codice originale Apollo 11: <https://github.com/chrislgarry/Apollo-11>
- Virtual AGC: <https://github.com/rburkey2005/virtualagc>
- Documento di progetto completo: `REPORT.md` nel repository Apollo-11 di studio
