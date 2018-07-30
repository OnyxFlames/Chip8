-- this script expects to be run in the excutables directory
require "os"
print("Running ROM 15puzzle")
os.execute("chip8 ../roms/15puzzle")
print("Running ROM blinky")
os.execute("chip8 ../roms/blinky")
print("Running ROM blitz")
os.execute("chip8 ../roms/blitz")
print("Running ROM brix")
os.execute("chip8 ../roms/brix")
print("Running ROM connect4")
os.execute("chip8 ../roms/connect4")
print("Running ROM guess")
os.execute("chip8 ../roms/guess")
print("Running ROM hidden")
os.execute("chip8 ../roms/hidden")
print("Running ROM invaders")
os.execute("chip8 ../roms/invaders")
    --[[
    15PUZZLE
    BLINKY
    BLITZ
    BRIX
    CONNECT4
    GUESS
    HIDDEN
    INVADERS --
    KALEID
    MAZE
    MERLIN
    MISSILE
    PONG
    PONG2
    PUZZLE
    SYZYGY
    TANK
    TETRIS
    TICTAC
    UFO
    VBRIX
    VERS
    WIPEOFF
    ]]--

print("Done tests.\n")