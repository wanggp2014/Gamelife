# Game of Life Configuration

## Game Settings
- Grid Width: 30 cells
- Grid Height: 15 cells
- Initial Population Probability: 30%
- Animation Speed: 0.5 seconds per generation

## Controls
- The game runs automatically
- Press Ctrl+C to exit

## Rules
Conway's Game of Life follows these simple rules:
1. Any live cell with fewer than two live neighbors dies (underpopulation)
2. Any live cell with two or three live neighbors lives on to the next generation
3. Any live cell with more than three live neighbors dies (overpopulation)
4. Any dead cell with exactly three live neighbors becomes a live cell (reproduction)

## How to Run
```bash
python3 game.py
```

## Requirements
- Python 3.x
- No additional dependencies required