# 🚆 Transport Network & Graph Project


A **C program** to model a transport network (bus, metro, tram…) as a **graph**, explore it, and find optimal routes.  

---

## ✨ Features

- 🗂 **Load a network** from a text file (`metro.txt`)  
- ℹ️ **View station info** (ID, name, neighbors, degree)  
- 👥 **List neighbors** of a station  
- 🔗 **Check connectivity** between stations  
- 🛣 **Find shortest paths** using **Dijkstra’s algorithm**  
- 📊 **Analyze and sort stations** by degree  

---

## 🛠 Data Structures

- **Adjacency lists** for graph representation  
- **Dynamic arrays** for stations  
- **Hash table** for efficient station lookup  

---

## 🧮 Sorting

- Selection Sort, Insertion Sort, and Quicksort  
- Tracks **comparisons** and **swaps**  

---

## 💻 Usage

Compile and run the program:

```bash
make
./metro metro.txt
````
---

## 💻 Interactive Menu

After running the program, follow the interactive menu:
````
===== TRANSPORT NETWORK MENU =====
1 - View station info
2 - List neighbors
3 - Find shortest path
4 - Sort stations by degree
0 - Quit
````
## 📂 File Structure
```
.
├─ src/
│  ├─ main.c           # Program entry point
│  ├─ graph.c          # Graph representation and adjacency lists
│  ├─ dijkstra.c       # Shortest path algorithms
│  ├─ sort.c           # Sorting algorithm
├─ includes/
│  └─ *.h              # Header files
├─ data/
│  └─ metro.txt
└─ Makefile            # Build the project

