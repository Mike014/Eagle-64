# Eagle-64: piano agile

Il progetto è diviso in sprint. Ogni sprint ha un obiettivo, un backlog di attività spuntabili e una Definition of Done: lo sprint è chiuso solo quando tutti i criteri sono soddisfatti.

Durata indicativa: 1–2 settimane per sprint, da adattare al tuo tempo. Meglio uno sprint piccolo e chiuso che uno grande lasciato a metà.

## Ambito

Il **nucleo** (Sprint 1–3) è il progetto: un lander 2D che atterra, sopravvive a sovraccarichi e guasti come l'AGC di Apollo 11, sotto la sorveglianza di un Safety Monitor. Regge da solo come portfolio.

Le **estensioni** (Sprint 4–6) aggiungono IA e sicurezza, in forma ridotta e solo dopo che il nucleo è chiuso.

Fuori ambito, salvo ripensamenti a nucleo finito: dinamica 6-DOF, GNN, MPC, Control Barrier Functions, RL, ensemble e conformal prediction, dataset esterni, attacchi oltre il replay.

## Decisioni di base

- **Numeri:** Flight Core e simulatore usano `double`. Il replay identico a parità di seed vale a parità di eseguibile. Il fixed-point Q16.16 (`core/fixed/`) è un esercizio C/NASM chiuso nello Sprint 1, non la base della guida: il suo range (±32768) è troppo stretto per velocità al quadrato e filtri.
- **Tempo:** il tempo è simulato, non reale. Un tick vale 10 ms. Ogni task dichiara un costo in unità di budget e ogni tick ha un budget fisso. Il sovraccarico (`1201/1202`) si ottiene superando il budget, quindi è deterministico e riproducibile anche su un PC veloce.
- **Dinamica:** si parte dalla sola discesa verticale (1D). Moto orizzontale e assetto arrivano nello Sprint 3.
- **Limiti di touchdown:** velocità verticale ≤ 1,0 m/s (obiettivo), ≤ 3,0 m/s (limite strutturale, come il modulo lunare Apollo). Dallo Sprint 3: velocità orizzontale ≤ 1,0 m/s, assetto entro ±5° dalla verticale.

## Regole di lavoro

- Un ramo Git per sprint o per funzionalità (`sprint-1/tests`), poi PR e merge su `main`. Dopo il merge il ramo si elimina e si riparte da `main` aggiornato.
- Commit piccoli e descrittivi.
- Una cartella nasce insieme al suo primo file: niente cartelle vuote in anticipo.
- Le decisioni tecniche si documentano con un commento in testa al codice che le implementa.
- Ogni funzione ha un test prima di essere considerata finita.
- A fine sprint: breve retrospettiva in fondo a questo file (cosa è andato bene, cosa cambiare).

## Struttura

```text
Eagle-64/
├── core/        C17 + NASM: chi ha l'autorità sulla missione
│   ├── fixed/     fixed-point Q16.16 (C di riferimento + NASM)    Sprint 1
│   ├── flight/    scheduler, navigazione, guida, controllo, ...   Sprint 2b-3
│   └── safety/    Safety Monitor                                  Sprint 3
├── sim/         C++20: fisica, sensori, attuatori, DSKY           Sprint 2a
├── ai/          Python + inferenza ONNX in shadow mode            Sprint 4-5
├── cyber/       ground station, protocollo, attacco replay        Sprint 6
├── tests/       test unitari, differenziali, di scenario          Sprint 1
├── scenarios/   scenari di missione versionati                    Sprint 3
└── docs/        piano
```

Le cartelle indicate con uno sprint futuro non esistono ancora: nascono in quello sprint.

## Sprint 0: Setup

**Obiettivo:** ambiente pronto, senza scrivere codice di prodotto.

- [x] Creare la struttura delle cartelle
- [x] Inizializzare Git (`git init`) e fare il primo commit dello scheletro
- [x] Creare il repository su GitHub e collegarlo come remote
- [x] Installare CMake e verificare con `cmake --version`
- [x] Installare Visual Studio 2022 Build Tools (carico "Sviluppo di applicazioni desktop con C++")
- [x] Installare NASM e verificare con `nasm -v`
- [x] Installare Python 3.12 e creare un ambiente virtuale `.venv`
- [x] Configurare l'editor: estensioni C/C++, CMake Tools, NASM, Python
- [x] Ristrutturare il repository in moduli e ridurre l'ambito

### Definition of Done (Sprint 0)

- Tutti gli strumenti rispondono da terminale.
- Primo commit pubblicato su GitHub.

## Sprint 1: Fondamenta low-level

**Obiettivo:** la prima funzione NASM chiamata da C, compilata con CMake e testata.

- [x] `CMakeLists.txt` radice con C17, C++20 e supporto NASM
- [x] Preset di build Debug e Release
- [x] Formato fixed-point Q16.16 documentato in `core/fixed/fixed_point.h`
- [x] Framework di test: CTest con un mini-framework in C, senza dipendenze
- [x] Implementazione di riferimento in C: somma, sottrazione, moltiplicazione, divisione con saturazione, con test
- [ ] Stesse funzioni in NASM secondo la Windows x64 ABI
- [ ] Test differenziali C vs NASM con golden vector e casi limite (overflow, divisione per zero, valori minimi e massimi)
- [ ] Benchmark C vs NASM (Release)
- [ ] CI con GitHub Actions su Windows: build e test a ogni push

### Definition of Done (Sprint 1)

- `cmake --build` e `ctest` passano in locale e in CI.
- Risultati del benchmark annotati, anche se il C risulta più veloce.

## Sprint 2a: Il lander cade

**Obiettivo:** una discesa verticale simulata e registrata. Il lander non ha ancora una guida, quindi si schianta.

- [ ] Modello fisico 1D: gravità lunare, massa, spinta, consumo di propellente (`sim/`)
- [ ] Integratore numerico a passo fisso (tick da 10 ms)
- [ ] Spinta costante da file di configurazione
- [ ] Log della telemetria in CSV (tempo, quota, velocità, massa, spinta)
- [ ] Seed e configurazione per replay identici

### Definition of Done (Sprint 2a)

- Con spinta zero il lander arriva al suolo nel tempo previsto dalla caduta libera (verifica con la formula, in un test).
- Due esecuzioni con lo stesso seed producono un CSV identico byte per byte.

## Sprint 2b: Il lander atterra

**Obiettivo:** il Flight Core in C guida il lander del simulatore fino a un atterraggio morbido.

- [ ] Interfaccia C tra simulatore (C++) e Flight Core (C): struttura dei sensori in ingresso, comando di spinta in uscita
- [ ] Sensori ideali (senza rumore)
- [ ] Scheduler a priorità con task periodici e budget per tick (vedi "Decisioni di base")
- [ ] Macchina a stati: `BOOT`, `DESCENT`, `APPROACH`, `LANDING`, `SAFE`
- [ ] Guida a profilo di discesa e controllore PID sulla velocità verticale
- [ ] DSKY testuale nel terminale

### Definition of Done (Sprint 2b)

- Il lander atterra con velocità verticale ≤ 1,0 m/s.
- Telemetria identica tra due esecuzioni con lo stesso seed.

## Sprint 3: Core fault-tolerant

**Obiettivo:** il sistema sopravvive a guasti e sovraccarichi.

- [ ] Dinamica 2D: moto orizzontale e angolo di assetto
- [ ] Rumore, bias e deriva dei sensori
- [ ] Filtro di Kalman lineare per la stima dello stato
- [ ] Watchdog e deadline dei task
- [ ] Overload shedding: scarto dei task non critici
- [ ] Allarmi in stile `1201/1202`
- [ ] Checkpoint e restart dello stato critico
- [ ] Modalità `ABORT` e `SAFE`
- [ ] Fault injection configurabile da file di scenario
- [ ] Safety Monitor con limiti di spinta, assetto, corridoio e propellente
- [ ] Checksum in NASM per lo stato salvato nei checkpoint

### Definition of Done (Sprint 3)

- Demo: task che supera il budget iniettato, allarme `1201`, task non critici scartati, atterraggio completato entro i limiti di touchdown.
- Ogni guasto di scenario ha un test automatico.

**Qui il nucleo è completo.** Prima di proseguire: README con risultati e limiti.

## Sprint 4: Estensione IA, rilevamento anomalie

**Obiettivo:** rilevare una deriva del radar su dati sintetici, misurando il risultato.

- [ ] Generatore di missioni Monte Carlo con guasti etichettati (seed, commit, scenario)
- [ ] Split per scenario: train, validation, test
- [ ] Baseline deterministiche: residui del Kalman e CUSUM
- [ ] Un modello ML piccolo (Isolation Forest o una TCN compatta)
- [ ] Metriche per evento: precision, recall, falsi allarmi, latenza di rilevamento

### Definition of Done (Sprint 4)

- Tabella di confronto baseline vs modello, con soglie scelte senza usare il test set.

## Sprint 5: Estensione IA, shadow mode

**Obiettivo:** il modello gira in C++ accanto al Flight Core, senza autorità.

- [ ] Export ONNX e test di parità Python vs ONNX Runtime
- [ ] Inferenza in C++ con controllo di schema, NaN e deadline
- [ ] Il Safety Monitor riceve l'advisory e decide se ignorarlo o segnalarlo

### Definition of Done (Sprint 5)

- Spegnere o corrompere il modello non cambia le decisioni del Flight Core.

## Sprint 6: Estensione cyber, comandi autenticati

**Obiettivo:** una ground station che invia comandi autenticati e un attacco replay rigettato.

- [ ] Ground station e protocollo comandi/telemetria
- [ ] Autenticazione con libreria crittografica standard, sequence number e finestra anti-replay
- [ ] Attacco replay simulato e rigettato in modo deterministico

### Definition of Done (Sprint 6)

- Scenario riproducibile: replay inviato, rigettato, evento registrato.

## Demo finale

- [ ] Demo riproducibile da seed: discesa, sovraccarico con allarme `1201`, deriva radar rilevata, replay rigettato, atterraggio o abort sicuro
- [ ] README finale con GIF o video, risultati e limiti dichiarati

## Retrospettive

### Sprint 0

- Cosa è andato bene:
- Cosa cambiare: l'ambito iniziale era troppo ampio; ridotto a un nucleo più due estensioni. Fissate subito le decisioni di base (numeri, tempo, dinamica, limiti) e diviso lo Sprint 2 in due.
