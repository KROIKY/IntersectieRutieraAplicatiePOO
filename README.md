# Cod Rutier — Simulare de trafic rutier în consolă

Proiect pentru disciplina **Programare Orientată pe Obiecte (POO)** — Tema 3121A.

Un joc în consolă (Linux/Ubuntu), vedere de sus, în care conduci o mașină printr-o
serie de intersecții. La fiecare intersecție trebuie să respecți o **regulă de
circulație** (STOP, semafor, prioritate de dreapta, cedează trecerea) și să eviți
coliziunile cu celelalte vehicule.

> **Stare:** în dezvoltare, pe etape. Vezi secțiunea *Etape* mai jos.

---

## Concept

- **Vedere de sus**, intersecția ocupă tot ecranul consolei și este **fixă**.
- Singurele entități care se redesenează („refresh") sunt **vehiculele**, pentru
  că ele sunt singurele care se mișcă — fundalul (drumul) se desenează o singură
  dată. Acest lucru elimină pâlpâirea (flicker).
- Controlezi mașina cu **WASD** sau cu **săgețile**.
- Progresezi prin **niveluri secvențiale**: fiecare nivel este o intersecție (sau
  câteva) pe tot ecranul; după ce o treci corect, avansezi la următorul nivel.

## Controale

| Tastă | Acțiune |
|-------|---------|
| `W` / `↑` | Accelerează / mergi în sus |
| `S` / `↓` | Frânează / mergi în jos |
| `A` / `←` | Virează / mergi la stânga |
| `D` / `→` | Virează / mergi la dreapta |
| `P` | Pauză |
| `Q` / `Esc` | Ieșire |

## Vehicule (sprite-uri ASCII)

| Vehicul | Sprite | Observații |
|---------|--------|-----------|
| Mașina jucătorului | `e@@e / @@ / e@@e` | controlată de jucător |
| Mașină (NPC) | `0XX0 / XX / 0XX0` | trafic automat |
| Camion (NPC) | vehicul lung (5 rânduri) | mai lent, mai mare |
| Motocicletă (NPC) | `o / @ / o` | mică, rapidă |

---

## Cerințe de sistem

- Sistem **Linux** (testat pe Ubuntu).
- Compilator **g++** cu suport C++17 (`sudo apt install build-essential`).
- Opțional: `cmake` (`sudo apt install cmake`).
- Un terminal de cel puțin ~100×30 caractere pentru afișare corectă.

## Compilare și rulare

Cu **Make** (recomandat):

```bash
make          # compilează jocul -> binarul ./cod_rutier
make run      # compilează și rulează
make test     # compilează și rulează testele unitare
make clean    # curăță fișierele generate
```

Cu **CMake** (alternativă):

```bash
mkdir build && cd build
cmake ..
make
./cod_rutier
```

---

## Structura proiectului

```
.
├── src/            # codul sursă (.cpp)
├── include/        # antetele (.h)
├── tests/          # teste unitare (fără biblioteci externe)
├── docs/           # documentația proiectului
├── assets/         # resurse (sprite-uri de referință, layout-uri de nivel)
├── Makefile        # build cu make
├── CMakeLists.txt  # build cu cmake
└── README.md
```

## Etape de dezvoltare (commit-uri)

- [x] **Etapa 0** — Schelet proiect (structură, build, README)
- [x] **Etapa 1** — Motor terminal (ecran ANSI + input WASD/săgeți)
- [x] **Etapa 2** — Ierarhia `Vehicul` + intersecție 2 benzi + mișcare pe bandă (contrasens, vieți)
- [x] **Etapa 3** — Intersecții și reguli de circulație polimorfe (STOP, semafor, prioritate) + scor
- [ ] **Etapa 4** — Rețea rutieră, vehicule NPC și coliziuni
- [ ] **Etapa 5** — Logging și excepții
- [ ] **Etapa 6** — Niveluri și extinderea hărții
- [ ] **Etapa 7** — Teste unitare
- [ ] **Etapa 8** — Facultativ (Observer, pauză/accelerare, export CSV)
- [ ] **Etapa 9** — Documentație finală

## Concepte POO folosite

- **Clase abstracte și moștenire:** `Vehicul` → `Masina`, `Motocicleta`, `Camion`.
- **Polimorfism:** viteze maxime diferite, comportament diferit la intersecție.
- **Compoziție:** `ReteaRutiera` conține `Intersectie` și `Strada`.
- **Încapsulare:** starea vehiculelor și a ecranului este protejată.
- **STL:** `std::vector`, `std::string`, `std::unique_ptr` etc.
- **Excepții:** tratarea coliziunilor și a traseelor invalide.

## Autor

Proiect realizat în cadrul disciplinei POO.
