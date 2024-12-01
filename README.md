## pontilus

Hey look, another game engine.
Big thanks to <a href="https://github.com/ambrosiogabe">GamesWithGabe</a> for his game engine tutorials and stuff.

Update: I added CMake integration! Kinda. I still need to work out instalation quirks and whatnot, but for the most part, the project runs smoothly, except for a major feature needed: embedded assets. I should move the default shader code into the project binary itself instead of linking them to outside files.

If you want to run the outputted `test`, first run these commands:

```
mkdir build
cd build

cmake ..
cmake --build .

./whatever_test_you_want
```

This was built on a Linux machine, and might not run on a Windows one. With CMake, it may be easier to create a Windows-working version. If you happen to do so, send in a pull request! I'd be happy to work with you to expand this project to others.

Furthermore, some extra details are needed to figure out dependency pulling and installations for building... I don't expect this to be a huge problem for distributing binaries, but oh well.