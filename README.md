# Oasis

***

## Project setup

Follow the [setup instructions here](setup.md).

***

## TODO

- [x] Input Handling
- [x] Integrate ImGui
- [x] Basic Audio Engine Setup
- [x] Text Rendering
  - [x] Load different fonts
- [x] Resource System for textures (Try to make extensible for other things as well)
- [x] Renderer Draw Texture From Source
- [x] Animated Sprites
- [x] Setup application to be accessible from anywhere in code (singleton or manager class)
- [ ] Make CMake generate more visual studio friendly project
- [x] Add header for colour
- [x] Add header for traps
- [ ] Precompiled headers
- [x] Smart Pointer/Reference headers
  - [x] Get rid of smart pointers (I've decided I don't like them)
- [ ] Convert fonts to use resource system
- [ ] Convert shaders to use resource system
- [x] Gamestate with layer system
- [x] Implement custom dynamic cast for pointer classes
- [x] Make sprite animation be based on timers
- [x] Write better ImGui logic
- [x] Use git submodules
- [ ] Integrate FrameBuffer code
- [ ] Logger
- [ ] Profiler
- [x] Console
- [x] Sprite colour tinting
- [x] Make origin consistent (decide on where it is and make sure code matches it)
- [x] Handle resize events from SDL (still needs to be better)
- [ ] UI System
  - [x] Dynamic text
  - [x] UI serializer
    - [x] Automatically deserialize UI files in a certain folder
  - [ ] UI editor
  - [x] Buttons & UI Event system

### Organization

- [ ] Remove sandbox game from repository
  - [ ] Update CMakelists so projects can have minimal CMakelists
  - [ ] Setup sample game repository
- [ ] Plan editor architecture
  - [ ] Plan data driving game data

## Optimizations

- [ ] Draw entire string as one texture
- [ ] Cache strings that don't change for text rendering
- [ ] Cache strings for UI dynamic text