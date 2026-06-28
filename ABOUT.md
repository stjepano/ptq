# Path To Quake

This project is a historical journey through game development of the
1980s and 1990s, leading up to Quake and the arrival of mainstream PC 3D
graphics acceleration.

The purpose is to enjoy coding, study important graphics and game-design
techniques, and build knowledge that can be reused in future projects. I
will attempt to implement clones of games that left a significant mark
on game development before hardware-accelerated 3D became common on the
PC.

## List of games

Here is the list of games that I will attempt to implement as clones.
The categories are organized by graphics style rather than by genre.

| Game                | Target                                                                            | Done                                                                 |
|---------------------|-----------------------------------------------------------------------------------|----------------------------------------------------------------------|
| Pong                | Framebuffer, timing, input, simple collision                                      | Two paddles, ball physics, scoring                                   |
| Space Invaders      | Sprite drawing, animation, entities                                               | Player, bullets, invader formation                                   |
| Pac-Man             | Tilemap, grid movement, maze collision, ghost state AI                            | Maze, pellets, player movement, ghosts with chase                    |
| Tetris              | Grid simulation, deterministic rules                                              | Rotation, line clears, gravity, scoring                              |
| Asteroids           | 2D vector renderer, wrapping world                                                | Vector ship and rocks, thrust and rotation, splitting asteroids      |
| Super Mario Bros    | Scrolling tilemap, platformer physics, camera, sprite animation                   | One playable level with jumping, enemies and blocks                  |
| The Legend of Zelda | Room/screen system, top-down collision, simple combat                             | Several connected screens, enemies, sword, pickups, room transitions |
| Elite               | Wireframe 3D, 3D transforms, clipping, projection                                 | Flyable ship in 3D, wireframe objects, basic cockpit                 |
| Stunt Car Racer     | Filled polygons, depth sorting or z-buffer, 3D physics                            | Drive over a simple 3D track with filled polygons and camera follow  |
| Wolfenstein 3D      | Raycasting, texture mapping, doors/sprites                                        | Walkable textured maze, wall projection, sprites, collision, minimap |
| Doom                | 2.5D sector renderer, BSP visibility, floor/ceiling spans                         | A level with variable heights, textured walls/floors, collision      |
| Quake               | Fully polygonal software 3D, BSP/PVS-style visibility, lightmaps, texture mapping | Fully 3D level with several enemies                                  |

There are other games that I may choose to implement depending on my
mood and if I can prove the same/similar concept.

## Completion criteria

A game is done when the central rendering/input/simulation technique is
demonstrated, not when it is content complete.

## Technology

I will not limit myself to the technologies of the original era. The
implementations will run on modern PCs, use modern C (C23), and use
modern development tools. However, the project will avoid 3D graphics
acceleration. The primary target platform is Linux; ports to other
platforms are possible.

AI use is limited to research, discussion, and review. AI will not
write code for this project.

The goal is personal satisfaction, shared learning, a deeper
understanding of techniques that remain useful in future projects, and
the gradual construction of a library of reusable functionality.

### Platform

For the platform layer, I will use [GLFW](https://www.glfw.org/). GLFW
will handle window creation, input, and timing.

### Graphics

The renderer writes all game graphics into a CPU-owned framebuffer.
OpenGL may be used only to upload, scale, and present the final
framebuffer.

Rendering is still done on the CPU. OpenGL is used only as a modern
presentation path for the completed image. I do not consider this
cheating: modern desktop systems are usually composited, and Wayland,
X11 compositors, and display drivers may use GPU or display hardware
to present windows regardless of which API the application uses.

The project avoids hardware-accelerated game rendering, not
hardware-assisted presentation.

### Sound

Sound will use [miniaudio](https://github.com/mackron/miniaudio) for
audio device access and playback. Game audio logic, mixing, and
reusable audio functionality can be built on top of it as the project
grows.
