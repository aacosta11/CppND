# CppND-Capstone Project

Capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

A simple, 2D video game built with SDL2. Controlling a very well-dressed man, the player is tasked to clear the area for construction of a mall.

---

## Criteria met

* The project demonstrates an understanding of C++ functions and control structures.
* The project reads data from a file and process the data, or the program writes data to a file. (1.)
* The project accepts user input and processes the input. (2.)

* The project uses Object Oriented Programming techniques.
* Classes use appropriate access specifiers for class members.
* Class constructors utilize member initialization lists.
* Classes abstract implementation details from their interfaces.
* Classes encapsulate behavior.

* The project makes use of references in function declarations. (3.)
* The project uses destructors appropriately.
* The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
* The project follows the Rule of 5.

(1.) Textures are loaded from the "assets" folder.

(2.) Inside of Game::run() \[Game.cpp, line 302], SDL_Event's are polled and passed to member objects to be handled accordingly.

(3.) In Game::run() \[Game.cpp, line 302], SDL_Event's are passed to member objects by reference.

---

## Class and Function Overview

Much of the code that initializes SDL and uses SDL functions is *heavily* inspired from this [SDL2 Tutorial](https://lazyfoo.net/tutorials/SDL/index.php). All thanks goes to the author of the tutorial.

### **Game**

The main class of the game. Handles SDL initialization and destruction, as well as the main game loop. 
Manages all of the other classes, passing the main SDL_Renderer where necessary. 
The Game class also manages the Tree class, which I felt didn't need it's own class. 

### **Game::run()**

The main game loop. 
Handles SDL_Event's and calls member functions to update and render the game. 
Initializes SDL and SDL_Image on entry, and proceeds to load textures and initialize member objects.

### **Entity**

An object that holds a Texture and a position.
Contains functions to render the Texture at the position.
Contains functions to handle collisions with other Entities.
Contains functions to apply physics to itself.

### **PlayableEntity**

An Entity that can be controlled by the player via its PlayableEntity::handleEvent() function.

### **Button**

A simple button class that can be used to create buttons that can be clicked on.
Contains functions to handle SDL_Event's and render a Texture.
The Button::handleEvent() function takes a boolean reference as an argument, which is set to true if the button is clicked on.

### **Window**

Encapsulates the SDL_Window, deallocating it on destruction.

### **Renderer**

Encapsulates the SDL_Renderer, deallocating it on destruction.

### **Texture**

Encapsulates an SDL_Texture, deallocating it on destruction (when no other instances are still active).
Contains rendering and loading functions.

### **Timer**

A simple timer class that can be used to time events. 
Contains functions to start, stop, and reset the timer, as well as a function to get the time elapsed since the timer was started.

---

## Code Structure

* root directory
  * /assets - Contains all the textures used in the game.
  * /include - Contains all .h files.
  * /src - Contains all .cpp files.

---

## Dependencies
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* SDL >= 2.6 (Linux, Mac), 2.0.8 (Windows)
  * Linux: `sudo apt-get install libsdl2-dev`
  * Mac: `brew install sdl2`
  * Windows: [Click here for installation instructions](https://wiki.libsdl.org/Installation)
* SDL_image >= 2.0.0 (Linux, Mac), 2.0.1 (Windows)
  * Linux: `sudo apt-get install libsdl2-image-dev`
  * Mac: `brew install sdl2_image`
  * Windows: [Click here for installation instructions](https://www.libsdl.org/projects/SDL_image/)

## Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./capstone`.