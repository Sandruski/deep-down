# Deep Down
 
Welcome to Deep Down website!

## About the game and it is context
This is Deep Down, a side-scrolling platformer game about exploring an abandoned and mysterious cave. It has two different levels:
while the first one follows the player entering the underground world, the second one is about an epic chase towards the exit of this dangerous place.<br>

Deep Down is the materialization of our effort and acquired knowledge during the first quarter of the year in Development subject. This project would not be possible without the help of Ricard Pillosu and the entire CITM-UPC.

Special kudos to Momodora: Reviere under the Moonlight, Cave Story, Zelda: Breath of the Wild and Dark Souls series for the art of the game. 

## Team members

Alvarez Garcia, Sandra:

•	GitHub account: [Sandruski](https://github.com/Sandruski)

•	

Costa Miquel, Guillem:

•	GitHub account: [DatBeQuiet](https://github.com/DatBeQuiet)

•

## Core of the game

The entire game is structured in modules where the princiapl module (App) manages and calls all the other modules that inherits from a base class(j1Module).
All modules have the same methods: PreUpdate, Update, PostUpdate and all of them are called from the core module in an infinite loop.

Two of its modules(GUI and Entity Factory) manage a subsystem of modules that follows the same structure that the explained above with the objective to have a strong and independent enitity and UI system.

### Innovations

<br>First Assignament:</br>

• Tiled Objects layers that allows the designer to implement any locked door and it is respective activator wherever he wants from Tiled


<br>Second Assignament:</br>

• Collider Culing 

• Blitting only what camera sees

• Path and enemy behaviour full implemented in Tiled


<br>Third Assignament:</br>

• Pause Menu

• New Transitions


## GITHUB Repository and release page

• GitHub repository: [Here](https://github.com/Sandruski/Deep-Down-Game)

• Last Release: [Here](https://github.com/Sandruski/Deep-Down-Game/releases)

## Deep Down Trailer

## How to play?

### Gameplay

Right Arrow: move player right

Left Arrow: move player left

Down Arrow: crouch

Space: jump

Left Shift: dash

A: shoot

X: attack (you can make a kind of combo if x is pressed in a short period of time)

KP(+, -): increase, decrease music volume

### Debug

F1: start from the beggining of the first level

F2: start from the beggining of the current level

F4: switch between levels

F5: save game state

F6: load game state

F7: fullscreen (or return to default screen size)

F9: show colliders

F10: God Mode

F11: enable/disable cap frames

Number 1: increase offset of "onlyBlittingCamera"

Number 2: decrease offset of "onlyBlittingCamera"

Number 3: enable/disable "onlyBlittingCamera"

### License

MIT License

Copyright (c) 2017 Sandra Alvarez & Guillem Costa
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.