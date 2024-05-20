# Simulated Annealing for Cell Placement

This project implements a simulated annealing algorithm to optimize the placement of cells design. The goal is to minimize the total wire length (TWL) by finding an optimal arrangement of cells on a grid.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Graph Analysis](#graph-analysis)
- [Contributing](#contributing)
- [License](#license)

## Introduction

Simulated annealing is a probabilistic technique used for finding an approximate solution to an optimization problem. In this project, simulated annealing is used to optimize the placement of cells on a grid to minimize the total wire length (TWL) in the design.

## Features

- **Netlist Parsing:** Parse netlist files to extract information about cells and nets.
- **Random Placement:** Generate an initial random placement of cells on the grid.
- **Simulated Annealing:** Optimize the placement using simulated annealing to minimize TWL.
- **Binary Grid Visualization:** Visualize the grid with binary representation (1 for empty, 0 for occupied).
- **TWL Calculation:** Calculate total wire length before and after optimization.
- **Graph Analysis:** Analyze the effect of temperature and cooling rate on TWL.

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/your-username/Simulated-Anealing.git
   cd Simulated-Anealing

2. **Compile the code**
   Ensure you have a C++ compiler installed (e.g., g++). Compile the code using:
   ```sh
   g++ -o exec main.cpp

## Usage
1. Prepare the Netlist File:
   Ensure you have a netlist file (e.g., d0.txt) in the project directory.

2. Run the Program:
   Execute the compiled program and provide the netlist file path when prompted:
   ```sh
   ./exec
   Input the Netlist File Path: When prompted, input the path to the netlist file (e.g., d0.txt).

3. Observe the Output:
   The program will display the binary grid and cell placements before and after optimization. It will also print the total wire length (TWL) and time taken for optimization.

4. Graph Analysis
   TWL vs. Temperature
   To analyze the effect of initial temperature on TWL, we ran the simulated annealing algorithm with different initial temperatures across 5 different designs. The results are plotted in the graph TWL_vs_Temperature.png.

   TWL vs. Cooling Rate
   To analyze the effect of cooling rate on TWL, we ran the simulated annealing algorithm with different cooling rates across 5 different designs. The results are plotted in the graph TWL_vs_CoolingRate.png.
   
   These graphs are included in the graphs directory of the project.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.

License
This project is licensed under the MIT License. See the LICENSE file for details.

Feel free to adjust the file paths, project repository link, and other details to fit
