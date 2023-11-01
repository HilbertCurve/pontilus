## pontilus

Hey look, another game engine.
Big thanks to <a href="https://github.com/ambrosiogabe">GamesWithGabe</a> for his game engine tutorials and stuff.

Note to self: I use the json parsing library `nlohmann/json`, but chose not to put the 22,000+ lines of code that encompass that library into my project. The various `#include`s of the library come from the header file I put into `/usr/local/include/`; I promise an install script is coming soon. Whether or not this is a good practice I don't know.


Also, I added CMake integration! Kinda. I still need to work out instalation quirks and whatnot, but for the most part, the project runs smoothly, except for a major feature needed: embedded assets. I should move the default shader code into the project binary itself instead of linking them to outside files.