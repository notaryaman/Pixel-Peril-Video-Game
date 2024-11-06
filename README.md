## Pixel Peril! - README
*Pixel Peril!* is an engaging and challenging 2D game developed in C++ using OpenGL and GLFW. In this dynamic game, you pilot a square avatar, dodging an array of moving NPCs (Non-Player Characters) that come in various colors. The game is initiated by pressing the spacebar, setting into motion the NPCs.

### Table of Contents
Requirements\
Compilation\
Running the Game\
Game Controls\
Gameplay

### Requirements
C++ compiler\
OpenGL\
GLFW\
GLUT\
VCcode(not actually required, but is recommended)
Ensure you have these dependencies installed before compiling and running the game.\

### Compilation
To compile the game, follow these steps:

Make sure you have all the required dependencies installed.

Download the source code and navigate to the directory containing the code.

Compile the code using your preferred C++ compiler. For example, you can do the following on VScode to compile the code:

**Cntrl + Shift + B**

Or you can use g++ with the following command:\

**g++ -o pixel_peril main.cpp -lGL -lGLU -lglfw -lglut -std=c++11**


### Running the Game
After successfully compiling the game, you can run it with the following command:

./main

This will launch the game window.

### Game Controls
W: Move up
A: Move left
S: Move down
D: Move right
Spacebar: Start/stop the game. NPCs start moving upon game start.
I: Hold down for instructions.
R: Press to restart the game after a game over.

### Gameplay
Your goal is to maneuver the square avatar, avoiding collisions with moving NPCs.
The game commences with a Spacebar press, triggering NPC movement.
Employ the WASD keys for navigation, aiming for the longest survival time possible.
Colliding with an NPC results in a game over, but you can instantly restart with another press of the Spacebar.
The game's difficulty increases over time, with speed increments and different NPC patterns.
Every 30 seconds, a green blob will show up, if you hit it, your score increases by 100 and you slow down for 20 seconds!
Enjoy the challenge of Pixel Peril! and strive for a high score!
