# DX11Engine

## Contents

* [About](#about)
* [Features](#features)
* [Overview](#diagram)
* [Problems](#problems)
* [License](#license)

## <a name=about>About

This is C++ game engine using Direct3D 11 made for study purpose.

Demo : https://youtu.be/diQm5AByzn4

<img src="editor.png"/>
<img src="editor_script.png"/>

### <a name=features>Features

* Framework
  * Entity-Component-System architecture
  * Node script for entity specific behavior
  
  
* Rendering
  * Basic Phong lighting
  * Deferred Rendering
  * Cubemap shadow
  * Skybox
  * Outline using Stencil buffer
  
  
* Physics
  * SAT collision detection
  * Linear projection
  
  
* GUI
  * Editor GUI using ImGui
  * Node script using ImNodes
  
  
* Misc
  * Resource management using smart pointers
  * JSON based file management
  * Image loading using stb_image
  * Mesh loading using assimp
  
## <a name=diagram>Overview

<img src="ecs_diagram.png"/>
  
## <a name=problems>Problems

* Node script recursion
  * currently not checking if nodes are recurring
  
  
* Multiple input management
  * when multiple keys are pressed, there's no way of knowing the order
  
  
* No CPU caching
  * ECS is not using data-aligned structure
  
  
* String based entity id
  * Can't search when there are duplicates
  * Searching is slow
	
## <a name=license>License

This project is under the MIT license.
