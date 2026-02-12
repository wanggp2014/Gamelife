#!/usr/bin/env python3
"""
Game of Life Implementation
A simple implementation of Conway's Game of Life
"""

import time
import random
import os

class GameOfLife:
    def __init__(self, width=20, height=10):
        self.width = width
        self.height = height
        self.grid = [[False for _ in range(width)] for _ in range(height)]
    
    def set_cell(self, x, y, alive=True):
        """Set a cell to be alive or dead"""
        if 0 <= x < self.width and 0 <= y < self.height:
            self.grid[y][x] = alive
    
    def get_neighbors(self, x, y):
        """Count the number of alive neighbors for a cell"""
        count = 0
        for dy in [-1, 0, 1]:
            for dx in [-1, 0, 1]:
                if dx == 0 and dy == 0:
                    continue
                nx, ny = x + dx, y + dy
                if 0 <= nx < self.width and 0 <= ny < self.height:
                    if self.grid[ny][nx]:
                        count += 1
        return count
    
    def step(self):
        """Advance the game by one generation"""
        new_grid = [[False for _ in range(self.width)] for _ in range(self.height)]
        
        for y in range(self.height):
            for x in range(self.width):
                neighbors = self.get_neighbors(x, y)
                
                if self.grid[y][x]:  # Cell is alive
                    if neighbors in [2, 3]:
                        new_grid[y][x] = True
                else:  # Cell is dead
                    if neighbors == 3:
                        new_grid[y][x] = True
        
        self.grid = new_grid
    
    def randomize(self, probability=0.3):
        """Randomly populate the grid"""
        for y in range(self.height):
            for x in range(self.width):
                self.grid[y][x] = random.random() < probability
    
    def display(self):
        """Display the current state of the grid"""
        os.system('clear' if os.name == 'posix' else 'cls')
        print("Conway's Game of Life")
        print("-" * (self.width * 2 + 1))
        
        for row in self.grid:
            print("|", end="")
            for cell in row:
                print("██" if cell else "  ", end="")
            print("|")
        
        print("-" * (self.width * 2 + 1))

def main():
    """Main game loop"""
    game = GameOfLife(30, 15)
    game.randomize()
    
    try:
        generation = 0
        while True:
            game.display()
            print(f"Generation: {generation}")
            print("Press Ctrl+C to exit")
            
            game.step()
            generation += 1
            time.sleep(0.5)
    
    except KeyboardInterrupt:
        print("\nGame ended. Thanks for playing!")

if __name__ == "__main__":
    main()