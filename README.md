# Routing Simulator: Distance Vector & Link State (CS425 – A4 – 2025)

This project simulates two foundational routing algorithms—**Distance Vector Routing (DVR)** and **Link State Routing (LSR)**—using an adjacency‑matrix representation of a network.  
It is written entirely in C++ in a single source file: `routing_sim.cpp` (shown in this repository).

---

## 1  Clone & Build

### 1.1 Clone the Repository
```bash
git clone https://github.com/kn-joshua/CS425-A4.git
cd CS425-A4
```

### 1.2 Compile
A simple g++ command is sufficient (there is no external dependency):

```bash
g++ routing_sim.cpp -o routing_sim
```

If you prefer a Makefile, add a target that invokes the same compile line.

## 2  Running the Simulator

### 2.1 Input‑File Format

```
n                           # number of nodes
row0_0 row0_1 ... row0_n-1  # adjacency‑matrix row 0
row1_0 row1_1 ...           # adjacency‑matrix row 1
...
rown-1_0 ... rown-1_n-1
```

* `0` means **no cost** (self‑loop or unused; ignored except on diagonal).
* `9999` (`INF`) means **no direct link / unreachable**.
* Any other positive integer is the **link cost** (bandwidth, latency, etc.).

### 2.2 Example Input (`sample.txt`)

```
4
0 10 100 30
10 0 20 40
100 20 0 10
30 40 10 0
```

### 2.3 Run

```bash
./routing_sim sample.txt
```

The program outputs:
1. **DVR** tables after convergence.
2. **LSR** tables (results from Dijkstra per source).

## 3  Algorithm Overview

| Algorithm | Core Idea | Loop‑Protection / Optimality | Our Implementation |
|-----------|-----------|------------------------------|-------------------|
| **DVR** | Each node iteratively exchanges its distance table with neighbors (Bellman‑Ford style). | Repeats until *no entry changes*, guaranteeing convergence to shortest paths given consistent metrics. | Triple‑nested loop; updates `dist` and `nextHop` until stable (see `simulateDVR`). |
| **LSR** | Every node floods link‑state info (full topology), then runs Dijkstra locally. | Always yields optimal paths (non‑negative weights). | Runs Dijkstra once **per source** (see `simulateLSR`). |

## 4  Code Walk‑Through

| Function | Role |
|----------|------|
| `readGraphFromFile` | Parses the adjacency matrix from the provided file. |
| `simulateDVR` | Performs iterative **Bellman‑Ford** updates until distance tables converge; prints final routing tables for each node. 3 loops to choose distance, neighbours and the source. Continues till no updates left |
| `simulateLSR` | Executes **Dijkstra's algorithm** from every node, producing shortest‑path trees and printing routing tables. |
| `printDVRTable` / `printLSRTable` | Nicely formats the distance/next‑hop information. |
| `main` | Validates CLI arguments, loads the graph, triggers both simulations in sequence. |

## 5  Testing

We validated the simulator on:
* **Small graphs** (3–6 nodes) with clearly known shortest paths.
* **Graphs containing unreachable links** (`9999`) to confirm correct "INF" behaviour.
* **Symmetric & asymmetric weights** to ensure DVR and LSR converge to the same metric results.
* **Edge cases**: fully‑connected graph, line topology, star topology.

All outputs matched hand‑computed routes and Dijkstra checks.

## 6  Team Contributions

| Member | Contribution | % |
|--------|--------------|---|
| **Kapu Nirmal Joshua** | Documentation, README, extensive testing & validation | 33.3 |
| **Aravind Seshadri** | Core implementation of DVR & helper functions | 33.3 |
| **Sumay Avi** | Implementation of LSR (Dijkstra) & I/O parsing | 33.3 |

## 7  Declaration

We affirm that this submission is entirely our own work. Any external resources have been properly cited and no portion is plagiarized.
