I need a space to collect my thoughts for the Renderer.


Basically, the Renderer should take a vertex object and a shader and, well, render them.
Every rData has a paired shader that's specified by the user. Defaults are specified in Application.cpp

IconMap should "register" itself with the Renderer. So should Font.

There should be a few "popular" render targets, such as quads in world space and a full window quad for screen effects.

For example:

```c++
static rData d = rData(2<<16, &Primitives::QUAD);
static Shader dShader = Shader("path/to/vert", "path/to/frag");

...

```