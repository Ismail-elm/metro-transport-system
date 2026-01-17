🚆 Transport Network & Graph Traversal Project
📖 Overview

This project models a transport network (bus, metro, tram, etc.) as a graph and provides tools to analyze it and find routes.
Written in C, runs on the command line.

⚡ Features

🗂 Load network from a text file (metro.txt) with stations and edges.

ℹ️ Display station info (ID, name, degree, neighbors).

👥 List neighbors of a station.

🔗 Check connectivity between stations.

🛣 Shortest path calculation using Dijkstra’s algorithm.

📊 Analyze and sort stations by degree.

🛠 Data Structures

📌 Graph represented with adjacency lists.

🧩 Stations stored in a dynamic array.

⚡ Efficient lookup using a hash table.

🧮 Sorting Algorithms

Selection Sort, Insertion Sort, and Quicksort

Counts comparisons and swaps for analysis

💻 Usage

Compile and run:

make
./metro metro.txt


Follow the interactive menu to explore the network.

✅ Requirements

C99/C11 standard

gcc with -Wall -Wextra

Only standard C library, no external libraries

🎯 Goals

Practice dynamic data structures (lists, arrays) in C

Implement graph traversal and shortest path algorithms

Write clean, modular, and documented code
