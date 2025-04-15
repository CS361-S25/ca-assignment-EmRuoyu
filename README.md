# Continuous Cellular Automaton

This program implements a continuous cellular automaton with Empirical library.

## Rules

In order to create a continuous CA rule, we will need have rules account for cell brith and death. Because the living condition of a cell is related to the conditions of its environment, I take avrage value of its neighbours as a basis.

1. Find average value of 8 neighbours.
2. Count num_neighbors_alive (cells alive within the 8 neighbours. A cell is considered alive if it has a value greater than 0.5).
3. Update cell next based on current and its neighbours。Different rules are applied depends on number of neighours alive.

# Acknowledge

The program is inspired by Conway's ame of Life by John Horton Conway in 1970. In the process, I referenced class materials and Labs developed by Professor Anya Vostinar for CS361 Artificial Life and Digital Evolution(Spring 2025).
