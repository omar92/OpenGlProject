# OpenGL Project

A 3D puzzle game built with OpenGL, SFML, GLEW, and GLM. The player navigates a colored cube through tile-based levels by sliding it across the grid until it hits a wall, aiming to reach the goal tile.

## Features

- 3D rendered levels using OpenGL shaders
- Smooth easing animation for player movement
- Texture support for the player and blocks via GLSL shaders
- Multiple levels with automatic progression
- Perspective camera that follows the player

## Dependencies

| Library | Purpose |
|---------|---------|
| [SFML](https://www.sfml-dev.org/) | Window creation, event handling, image loading |
| [GLEW](http://glew.sourceforge.net/) | OpenGL extension loading |
| [GLM](https://glm.g-truc.net/) | Math library (vectors, matrices, transforms) |
| OpenGL 3.x+ | Rendering |

## Building

The project is configured as a **Visual Studio** solution (`.sln`).

### Prerequisites

1. Visual Studio 2015 or later (with C++ workload)
2. Install the following libraries and ensure their headers/libs are on the include/library paths:
   - SFML (headers under `SFML/`, libs `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`)
   - GLEW (header at `gl/glew/glew.h`, lib `glew32.lib`)
   - GLM (headers at `gl/glm/`)
   - OpenGL (`opengl32.lib` — ships with the Windows SDK)

### Steps

1. Open `openGLproject.sln` in Visual Studio.
2. Set the build configuration to **Debug** or **Release** (x86 or x64 to match your installed DLLs).
3. Build the solution (**Ctrl+Shift+B**).
4. Copy the required runtime DLLs (`sfml-*.dll`, `glew32.dll`) next to the generated executable.
5. Make sure the following files are in the same directory as the executable:
   - `texture_vs.glsl` and `texture_fs.glsl`
   - `mylevel.txt`, `mylevel1.txt`, `mylevel2.txt`
   - `res/block.png` and `res/player.png`

## Running the Game

Launch the compiled executable. The game opens a **1920×1080** fullscreen/window and loads the first level automatically.

## How to Play

| Key | Action |
|-----|--------|
| **W** / **↑** | Move player up (−Z) |
| **S** | Move player down (+Z) |
| **A** | Move player left (−X) |
| **D** | Move player right (+X) |
| **Esc** | Quit the game |

### Movement Rules

- The player cube **slides** in the chosen direction until it hits a wall (`#` tile).
- Reaching a **goal tile** (`$`) automatically advances to the next level.
- Levels cycle: Level 1 → Level 2 → Level 0 → …

## Level Format

Levels are plain-text files (`mylevel*.txt`). The first line contains the grid dimensions (`cols rows`), followed by the tile data:

| Character | Meaning |
|-----------|---------|
| `@` | Player starting position (floor tile) |
| `#` | Wall block (elevated, blocks movement) |
| `$` | Goal tile (advances to next level) |
| `-` | Empty floor tile |

**Example:**
```
8 8
@$--#---
--------
--#-----
```

## Project Structure

```
openGLproject/
├── Source.cpp          # Entry point, game loop, level logic
├── Window.h / .cpp     # Window and event handling wrapper
├── Shader.h / .cpp     # GLSL shader & texture loading
├── Mesh.h / .cpp       # 3D mesh (cube) creation and rendering
├── Camera.h / .cpp     # Perspective/orthographic camera
├── OUtilities.h        # Shared vertex struct
├── texture_vs.glsl     # Vertex shader
├── texture_fs.glsl     # Fragment shader
├── myLevel.txt         # Level 0
├── myLevel1.txt        # Level 1
├── myLevel2.txt        # Level 2
└── res/
    ├── block.png       # Block texture
    ├── player.png      # Player texture
    └── target.png      # Target/goal texture
```
