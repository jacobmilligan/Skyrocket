# Skyrocket Framework

Skyrocket is a cross-platform graphics and media library for game development.

### Supported platforms:
* macOS (10.11+)
* Windows (in progress)

### Supported graphics API's:
* Metal
* OpenGL (in progress)

### Planned platform support:
* Linux
* Android
* iOS

### Planned graphics API support:
* Direct3D 9/10/11
* Vulkan

## Getting Started and Building

* Clone the source ``git clone https://github.com/jacobmilligan/Skyrocket.git``
* The project is developed with the intention to support multiple IDE's and text-editors as easily as possible so right now, the only supported build system is **CMake**.

### Building on Unix systems
* Open a terminal and ``cd`` into the root directory of the cloned project, once there:
	* ``mkdir build && cd build``
	* ``cmake ..``
	* ``make``
* This will build the library as a static library in the ``SkyrocketRoot/build/<Debug/Release>`` directory and all test executables under ``SkyrocketRoot/build/Tests``.

### Building on Windows
* If building from **cmd** you need to install cmake add it to your ``PATH``
* For those running Visual Studio version <= VS2015:
	* ``cd`` into the root directory where you cloned Skyrocket
	* ``mkdir VS && cd VS``
	* ``cmake ..``
	* This will generate a ``Skyrocket Framework.sln`` Visual Studio solution file
	* To build the project either build from within Visual Studio or run ``cmake --build .`` from the command line
	* If modifying the Skyrocket source, you'll need to run ``cmake ..`` from the command line each time a change is made
* For those running Visual Studio 2017 and up CMake is supported directly and the project includes a `CMakeSettings.json` config file:
	* Open the root folder in Visual Studio
	* Choose ``build CMakeLists.txt`` from the CMake dropdown
	* This will build all files in a ``build`` directory if it exists, otherwise it will create a new one	
