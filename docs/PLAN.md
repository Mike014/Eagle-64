# Eagle-64: piano agile

Il progetto è diviso in sprint. Ogni sprint ha un obiettivo, un backlog di attività spuntabili e una Definition of Done: lo sprint è chiuso solo quando tutti i criteri sono soddisfatti.

Durata indicativa: 1–2 settimane per sprint, da adattare al tuo tempo. Meglio uno sprint piccolo e chiuso che uno grande lasciato a metà.

## Regole di lavoro

- Un ramo Git per sprint o per funzionalità (`sprint-1/fixed-point`), poi merge su `main`.
- Commit piccoli e descrittivi.
- Ogni decisione tecnica importante va in un ADR in `docs/adr/` (una pagina: contesto, decisione, conseguenze).
- Ogni funzione ha un test prima di essere considerata finita.
- A fine sprint: breve retrospettiva in fondo a questo file (cosa è andato bene, cosa cambiare).

## Sprint 0: Setup e studio

**Obiettivo:** ambiente pronto e principi dell'AGC compresi, senza scrivere codice di prodotto.

### Ambiente

- [x] Creare la struttura delle cartelle
- [x] Inizializzare Git (`git init`) e fare il primo commit dello scheletro
- [ ] Creare il repository su GitHub e collegarlo come remote
- [x] Installare CMake e verificare con `cmake --version`
- [x] Installare Visual Studio 2022 Build Tools (carico "Sviluppo di applicazioni desktop con C++")
- [x] Installare NASM e verificare con `nasm -v`
- [x] Installare Python 3.12 e creare un ambiente virtuale `.venv`
- [x] Configurare l'editor: estensioni C/C++, CMake Tools, NASM, Python

### Studio AGC

Leggere i file nel repository Apollo-11 e scrivere una nota per ciascuno in `docs/historical-notes/`:

- [ ] `Luminary099/MAIN.agc`: come è composto il programma
- [ ] `EXECUTIVE.agc`: job, priorità, core set, allarme `1201`
- [ ] `WAITLIST.agc`: task temporizzati
- [ ] `FRESH_START_AND_RESTART.agc` e `RESTARTS_ROUTINE.agc`: come riparte il software
- [ ] `ALARM_AND_ABORT.agc`: allarmi e abort
- [ ] `THE_LUNAR_LANDING.agc`: struttura dei programmi di allunaggio P63–P66

### Documentazione

- [ ] `docs/architecture/overview.md`: diagramma degli strati e del flusso dei dati
- [ ] `docs/adr/0001-flight-core-in-c17.md`: perché C17 per il Flight Core
- [ ] `docs/adr/0002-ai-advisory-only.md`: perché l'IA non comanda gli attuatori
- [ ] `docs/historical-notes/agc-vs-eagle64.md`: tabella "principio AGC → come lo realizza Eagle-64"

### Definition of Done (Sprint 0)

- Tutti gli strumenti rispondono da terminale.
- Primo commit pubblicato su GitHub.
- Note storiche e due ADR scritti.

## Sprint 1: Fondamenta low-level

**Obiettivo:** la prima funzione NASM chiamata da C, compilata con CMake e testata.

- [x] `CMakeLists.txt` radice con C17, C++20 e supporto NASM
- [x] Preset di build Debug e Release
- [ ] Framework di test (per esempio CTest con una libreria leggera)
- [ ] Definire il formato fixed-point (per esempio Q16.16) in un ADR
- [ ] Implementazione di riferimento in C: somma, moltiplicazione, divisione con saturazione
- [ ] Stesse funzioni in NASM secondo la Windows x64 ABI
- [ ] Test differenziali C vs NASM con golden vector e casi limite (overflow, divisione per zero, valori minimi e massimi)
- [ ] Primo benchmark C vs NASM in `benchmarks/`
- [ ] CI con GitHub Actions su Windows: build e test a ogni push

### Definition of Done (Sprint 1)

- `cmake --build` e `ctest` passano in locale e in CI.
- Risultati del benchmark documentati, anche se il C risulta più veloce.

## Sprint 2: MVP deterministico

**Obiettivo:** un atterraggio nominale simulato in 2D, riproducibile.

- [ ] Modello fisico 2D: gravità lunare, massa, spinta, consumo di propellente
- [ ] Integratore numerico a passo fisso
- [ ] Sensori ideali (senza rumore) per iniziare
- [ ] Scheduler a priorità con task periodici
- [ ] Macchina a stati: `BOOT`, `DESCENT`, `APPROACH`, `LANDING`, `SAFE`
- [ ] Guida a profilo di discesa e controllore PID
- [ ] DSKY testuale nel terminale
- [ ] Log della telemetria in CSV
- [ ] Seed e configurazione per replay identici

### Definition of Done (Sprint 2)

- Il lander atterra entro i limiti di velocità definiti.
- Due esecuzioni con lo stesso seed producono telemetria identica.

## Sprint 3: Core fault-tolerant

**Obiettivo:** il sistema sopravvive a guasti e sovraccarichi.

- [ ] Rumore, bias e deriva dei sensori
- [ ] Filtro di Kalman esteso (EKF) per la stima dello stato
- [ ] Watchdog e deadline dei task
- [ ] Overload shedding: scarto dei task non critici
- [ ] Allarmi in stile `1201/1202`
- [ ] Checkpoint e restart dello stato critico
- [ ] Modalità `ABORT` e `SAFE`
- [ ] Fault injection configurabile da file di scenario
- [ ] Safety Monitor con limiti di spinta, assetto, corridoio e propellente

### Definition of Done (Sprint 3)

- Demo: sovraccarico iniettato, allarme, task scartati, atterraggio completato.
- Ogni guasto di scenario ha un test automatico.

## Sprint 4: AI MVP

**Obiettivo:** primo rilevamento anomalie misurato su dati sintetici.

- [ ] Generatore di campagne Monte Carlo con guasti etichettati
- [ ] Formato del dataset versionato con manifest (seed, commit, scenario)
- [ ] Split per scenario: train, validation, calibration, test
- [ ] Baseline deterministiche: residui del Kalman, EWMA, CUSUM
- [ ] PCA (SPE e T²) e Isolation Forest
- [ ] Random Forest per classificare i guasti noti
- [ ] Metriche per evento: precision, recall, F0.5, falsi allarmi per ora, latenza
- [ ] Notebook o report di valutazione in `ai/`

### Definition of Done (Sprint 4)

- Tabella di confronto tra baseline deterministiche e modelli ML.
- Nessun dato di test usato per scegliere soglie o iperparametri.

## Sprint 5: IA temporale e ONNX

**Obiettivo:** un modello temporale eseguito in C++ in shadow mode.

- [ ] TCN causale compatta e baseline LSTM
- [ ] Ensemble di 3 modelli
- [ ] Calibrazione e split conformal con astensione (`ADVICE`, `UNCERTAIN`, `NO_ADVICE`)
- [ ] Rilevamento input fuori distribuzione
- [ ] Export ONNX e test di parità Python vs ONNX Runtime
- [ ] Integrazione in `inference/` con controllo di hash, schema e deadline
- [ ] Il Safety Monitor riceve l'advisory e decide se ignorarlo o segnalarlo
- [ ] Model card in `docs/model-cards/`

### Definition of Done (Sprint 5)

- Spegnere o corrompere il modello non cambia le decisioni del Flight Core.
- Il TCN batte le baseline su metriche fissate prima dei test, oppure lo si documenta onestamente.

## Sprint 6: Cyber Range

**Obiettivo:** ground station e attacchi simulati, con difese e rilevamento.

- [ ] Threat model in `docs/threat-model/`
- [ ] Ground station e protocollo comandi/telemetria
- [ ] Autenticazione dei messaggi con libreria crittografica standard, sequence number e finestra anti-replay
- [ ] Attacchi: replay, spoofing, command injection, DoS, false-data injection, manipolazione del tempo
- [ ] Rilevamento: regole deterministiche più modello IA
- [ ] Fuzzing del parser del protocollo
- [ ] Report di sicurezza

### Definition of Done (Sprint 6)

- Ogni attacco della threat matrix ha uno scenario riproducibile e un risultato documentato.

## Sprint 7: Ricerca avanzata e demo

**Obiettivo:** estensioni sperimentali e demo finale per il portfolio.

- [ ] Dinamica 6-DOF
- [ ] Almeno un modulo sperimentale (GNN, residual learning, MPC assistito o Control Barrier Functions)
- [ ] Demo finale registrata: deriva radar, attacco replay, sovraccarico, atterraggio o abort sicuro, report finale
- [ ] Assurance case in `docs/assurance-case/`
- [ ] README finale con GIF o video, risultati e limiti

### Definition of Done (Sprint 7)

- Demo riproducibile da seed e manifest.
- Limiti del progetto dichiarati esplicitamente.

## Retrospettive

### Sprint 0

- Cosa è andato bene:
- Cosa cambiare:
