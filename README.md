# Tetris
Tetris Game

If you make a new project with this code you will need to change some of the default settings.
Go to Project Properties and then:

1. Configuration Properties -> General -> Charater Set -> Set to: Use Unicode Character Set 
2. C/C++ -> General -> SDL Checks -> Set to: No (/sdl-)
3. Linker -> Input -> Additional Dependencies -> Add: winmm.lib
4. Linker -> System -> SubSystem -> Set to: Console (/SUBSYSTEM:CONSOLE)
