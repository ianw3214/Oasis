# Building the project

## Requirements

You need the following to successfully build the project on your machine:

- A C++ compiler
- A package manager (vcpkg on windows, brew on mac)
- CMake

## Installing packages

### Windows

First, download vcpkg from [here] and find that directory using the command line.  
Hook up user wide integration via the following command:  
> .\vcpkg integrate install

Then, install the necessary libraries using the following commands:

> .\vcpkg install sdl2:x64-windows  
> .\vcpkg install openal-soft:x64-windows
> .\vcpkg install glew:x64-windows  
> .\vcpkg install freetype:x64-windows
> .\vcpkg install imgui

### Mac

On mac, use [homebrew](https://brew.sh/) to get the needed library files.  
Run the following commands to get SDL, GLEW, and FreeType setup on your local machine.

> brew install sdl2  
> brew install sdl2_mixer  
> brew install glew  
> brew install freetype  
> brew install imgui  

## Creating the project

Sample game using the Oasis engine can be found [here](https://github.com/ianw3214/game).

Import the Oasis game engine as a submodule to the root directory of the project to get started.

> git submodule add git@github.com:ianw3214/Oasis.git

Then, create a CMakelists.txt with the following format:

```cmake
cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(OASIS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Oasis)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/Oasis/cmake/")
include(OasisProject)

OasisProject(Sandbox)

# add cmake sources here...
```

## Running cmake

For this project, it is recommended to use an **out of source** build so that the build files don't interfere with the source files.  
To do so, first make a directory called **build**, and *cd* into it on the command line.  
Then, run the following command to generate the project:  
> cmake ..

Alternatively, you can use the included script files to generate the cmake build, just run the following command on windows:
> setup.bat

## Working with the project

The project should be all set up now. On windows, simply open the .vcxproj file to open the project in Visual Studio.