# Simple Voxel Engine

The code here is the result of a three or four days of free time over the 2016 Christmas break. Both the look and constructible / destructible feel of [voxels](https://en.wikipedia.org/wiki/Voxel) have always appealed to me perhaps as a result of a childhood playing with Lego and I felt like doing some fun, completely none work related, coding and so I rolled up my sleeves and started coding. Did I have fun? Yup - this was a super fun little project to play with. Since then it's evolved a little and is very much a work in progress.

As this was all about having fun and getting back to basics I stayed away from existing game engines and so other than the STL and some bootstrap OpenGL libraries ([glfw](http://www.glfw.org), [glad](http://glad.dav1d.de) and [glm](http://glm.g-truc.net/0.9.8/index.html)) this is all written from the ground up.

At the end of day 3 I had a couple of nice demos of basic rendering that you can see in the "self-portrait" video and screenshots below.

[![Video of the photo height map demo](https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/screenshots/photoHeightMapVideo.png)](https://www.youtube.com/watch?v=aETUvjQp-yc)

(links to YouTube)

Before anyone leans on this code too heavily its worth noting that I haven't written C++ in anger since about 1998 (though I have continued to do a reasonable amount of C) and although I've done some basic 3d work over the years have only used OpenGL in an extremely basic sense. That said if you do want to use it the below may help.

## Examples

There are three included examples:

* bwImageHeightMap - this takes a black and white image, converts it to a height map, and renders it as voxels. You can move around the scene with the keyboard and mouse. [Video here](https://www.youtube.com/watch?v=aETUvjQp-yc).
* perlinLandscape - generates a height map using [perlin noise](https://en.wikipedia.org/wiki/Perlin_noise) and creates a simple landscape from it.
![Perlin noise demo](https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/screenshots/perlinNoiseLandscape.png)
* sprites - this demonstrates using voxel sprites and behaviours to build a cityscape.
![Cityscape demo](https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/screenshots/cityConstruction.png)
* voxelInvaders - work in progress, a simple space invaders type game that demonstrates sprites, behaviours, and collision detection
![Invaders demo](https://github.com/JamesRandall/SimpleVoxelEngine/blob/master/screenshots/invaders.png)

## Using the engine

Currently this only targets Windows via Visual Studio though the code should be easily portable to other platforms and as my laptop is a Macbook and Parallels doesn't support OpenGL 3.3 I'm likely to port it over to *nix soon.

If your system meets those minimum requirements then open the solution in Visual Studio (I'm currently using the 2017 Community RC) and hit build and you can then run the two examples bwImageHeightMap and perlinLandscape directly from the IDE.

To generate scenes of your own firstly take a look at the examples - they're pretty simple and show the setup process. Following that the key thing to understand is the concept of a chunk factory. Scenes are subdivided into chunks which are grids of voxels (currently 16x16x16 in this engine) and for each chunk in the scene the construct method of the factory will be called and should return a chunk. To do this in code implement a class that derives from the interface IChunkFactory and at least one class that dervies from IVoxel. The reason for the latter is that the engine doesn't make any assumptions about the properties of your voxels and simply asks them to let the engine know what color to use.

Note that chunk creation is executed in parallel and so your code needs to be thread safe.

## Future Developments

I'll keep working on this as long as it is fun to do so and I've got a couple of small projects in mind. Some of the engine improvements I have in mind include:

* Mac / *nix support
* General optimisation. I've only done the really low hanging fruit so far and there is scope for improvement everywhere. Both OpenGL use and memory management are currently very naive. That said I'm likely to focus on clarity of code and ease of change over raw performance.
* Voxel sprite support
* Shadows
* Skybox / fog support
* Paging / virtualisation - with this in mind this is one of the main reasons that scenes are constructed using the chunk factory
* Level of detail support
* Simple physics

## Useful Resources

Coming at the OpenGL world basically from a position of complete ignorance I found the below immensely useful and am massively grateful to the authors.

* [Learn OpenGL](https://learnopengl.com)
* [OpenGL Tutorial](http://www.opengl-tutorial.org)

For a refresher on C++ I basically skim read these:

* [Accelerated C++](https://www.amazon.co.uk/Accelerated-Practical-Programming-Example-Depth/dp/020170353X) 
* [Effective Modern C++](https://www.amazon.co.uk/gp/product/1491903996)

No doubt I've made lots of mistakes with C++ but after 18 years it mostly felt like putting on a comfy, if slightly itchy, pair of socks!

## Linux
To build in Linux:

```
$ sudo apt-get install glew-utils libglew-dev libglm-dev libglfw3-dev
$ mv depends/src/glad/glad.c depends/src/glad/glad.cpp
$ mv depends/include/glm depends/include/_glm
$ mkdir build ; cd build
$ make -j 8
```
