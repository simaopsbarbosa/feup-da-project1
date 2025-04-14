# Route Planning Tool

A command-line application for urban route planning, developed in C++ as part of the *Algorithm Design* course (Spring 2025, FEUP).

## Features

- Fastest and alternative route computation using shortest-path algorithms  
- Restricted routing with support for node and edge exclusions  
- Environmentally-friendly routing combining driving and walking, with parking constraints  
- Batch mode processing via structured input/output files  
- Graph-based data representation with support for driving-only and mixed-mode travel

## Data Input

- `Locations.csv`: Node definitions and parking availability  
- `Distances.csv`: Edge definitions with driving and walking times

## Technical Highlights

- Greedy algorithm strategies and shortest-path computation  
- Modular, maintainable C++ code with full Doxygen documentation  
- Emphasis on time complexity and input validation

## Building

To build the project, ensure you have `CMake` and `Doxygen` installed.

### Build Instructions

From the root directory of the project:

```
mkdir build
cd build
cmake ..
cmake --build .
./DA2025_PRJ1_T13_G07
```
