Initial compilation for Linux.
Added a CMakeLists.txt
Fixed some includes.
To compile in Ubuntu:

```
$ sudo apt-get install glew-utils libglew-dev libglm-dev libglfw3-dev
$ mv depends/include/glm depends/include/_glm
$ cd voxelEngine ; mkdir build ; cd build
$ make -j 8
```