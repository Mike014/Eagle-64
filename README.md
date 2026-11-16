# Eagle-64

**AI-Assisted Lunar Guidance Simulator**: un simulatore di discesa e allunaggio che reinterpreta i principi dell'Apollo Guidance Computer di Apollo 11 su Windows x86-64.

Eagle era il modulo lunare di Apollo 11; 64 è l'architettura x86-64 su cui gira il progetto.

> Stato: Sprint 1 (fondamenta low-level). Vedi [`docs/PLAN.md`](docs/PLAN.md).

## Principio guida

Il Flight Core deterministico mantiene sempre l'autorità sulla missione. L'IA opera in modalità shadow/advisory: osserva, segnala e suggerisce, ma non comanda mai direttamente motore, fasi di volo, abort o touchdown.

## Architettura

| Modulo | Linguaggio | Cartella | Ruolo |
| --- | --- | --- | --- |
| Core | C17 + NASM x86-64 | `core/` | Fixed-point, Flight Core (scheduler, GNC, allarmi, restart), Safety Monitor |
| Simulatore | C++20 | `sim/` | Fisica 2D, sensori, attuatori, DSKY |
| IA | Python + ONNX Runtime | `ai/` | Rilevamento anomalie, inferenza in shadow mode |
| Cyber | C++20 | `cyber/` | Ground station, comandi autenticati, attacco replay |

Il nucleo è `core/` + `sim/` (Sprint 1–3); `ai/` e `cyber/` sono estensioni ridotte (Sprint 4–6).

## Struttura del repository

```text
Eagle-64/
├── core/
│   ├── fixed/     fixed-point Q16.16
│   ├── flight/    scheduler, navigazione, guida, controllo, allarmi, restart
│   └── safety/    Safety Monitor
├── sim/
├── ai/
├── cyber/
├── tests/
├── scenarios/
└── docs/          piano
```

Le cartelle nascono con il loro primo file: oggi esistono solo `core/fixed/` e `docs/`.

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
