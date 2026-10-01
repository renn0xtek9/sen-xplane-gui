# Project overview 
This is a software project which produces a graphical user interface that displays the altitude and speed of an aircraft.
The aircraft itself is being flow live in an X-plane 12 simulator.
See this page for the API: https://developer.x-plane.com/article/x-plane-web-api/
The data are transmitted via an Airbus "Sen" middleware available in this repository
On top of that there is a view of an openstreetmap view of the area where the aircraft is positioned on the map.

## Terminology 
- "HMI" stands for the graphical user interface application that will encompass all functionalities described later like displaying aircraft information etc.
- "X-plane bridge" stands for a standalone application that is run with the Sen middelware and receive data from the X-plane API and publish them as Sen object. See [this](https://airbus.github.io/sen/latest/users_guide/mental_model.html) for more information
- "SDK" the SDK is a package that contains the basic application code a library that enable compiling a "Plugin" 
- "Plugin" is a standalone package that contains a "UI" part and a "Middleware integration" part
    - it can be compiled using the "SDK" package. 
- "Panel" is a rectangular window that displays informations (for instance aircraft information or openstreetmap view)

## "HMI" description
When starting the application, the application shall open a window full screen 
with:
- a menubar containing a "File" , "View", "Plugin" and an "Exit" menu item 
    - the "View" menu shall have a "Panel" entry for every "Panel" provided by every "Panel" provided by our applic
    - the "Plugins" menu shall have one entry for every plugins available for our application, with a tickbox to disable/enable them
- a status bar displaying the status of the application "Connected" or "Not connected"
    - as soon as sen object sent by the "X-plane bridge" are received, the status shall be switch to connect

## Technical stack
We use 
- Modern C++ with gcc version as available from the Ubuntu 26.04 repository
- Packages (e.g. "SDK" and "Plugins") are made available via the [common package specifications](https://cmake.org/cmake/help/latest/release/4.3.html)
- Latest CMake 4.4.3 avaialable from https://cmake.org/download/
    - We will be using CTest to manage our test suites
    - We will be using CPack to create a debian package of the project (installable on ubuntu 26 platfoms).
        - We shall make use of [GNUInstallDirs](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html) for this.
    - We will be using CDash to create test dashboad of the project.
- For the graphical user interface Qt 6.10.2 and QML as available from the Ubuntu 26.04 repository
    - QML frontend module shall uses [TestCase QML Type](https://doc.qt.io/qt-6/qml-qttest-testcase.html) for UI/UIX test which shall integrate seemlessly with CTest. UI/UX tests are to be labelled as "UI" in CTest.
- release 0.6.0 of the Sen https://github.com/airbus/sen/releases#release-0.6.0
- the Conan package manager (conan 2) for external dependency (i.e. Sen)
- Gtest for unit testing and mocking
- [spec42](https://github.com/elan8/spec42) for SysMLv2 diagram (architecture diagram class diagrams etc)
- pre-commit hooks for formatting
- all those shall be installed in a docker container in form of a VSCode Dev containers that uses ubuntu 26.04 as a base image.

## Project structure
The project is structured as a monolitical CMake project with 
- a "cmake" subfolder for cmake macros
- a "conan" subfolder containing conan profiles required for this project
- an "app" subfolder for the main application itself 
- a "lib" subfolder for the libraries of the main application 
    - each library as an "include" subfolder wich itself has as sufolder named "hmi" where public headers of the libraries ares stored. This is done so that when including, files are prefixed with hmi. i.e. `#include "hmi/lib_header.h"`
- a "middleware-integration" subfolder for the libraries related to the integration and connection with the Sen middleware
- a "plugins" subfolder containting plugins. In our case there will be two plugins:
  - an "x-plane-aircraft" plugin to connect to X-Plane and receive data from it
  - an "osm-view" plugin that renders an open streetmap view 
  - every plugins folder structure shall consist of a 
    - "user-interface" subfolder for the UI (QML and C++ components)
    - "middleware-integration" subfolder for the integration and connection with the sen middelware.
- all "middelware-integration" subfolder consist of a 
    - "interfaces" subfolder containing Sen's ".stl" files that describe the interfaces on the SEN middleware
    - "lib" subfolder containing the actual implementation of the sen object.
    - "fake" subfolder that will implement fake nodes and services sending fake data for testing purposes.
    - "test" subfolder that will contains unit test and integration test of the middelware integration itself.

## HMI Software architecture
The HMI consist of a main apllication written in c++ and Qt/QML 6.
The HMI will lookup for available plugin at runtime and load them dynamically. 
Each plugin is expected to provide a user interface (QML) and a sen object (C++).
The implements a Sen Component that shall be started at runtime in a separate thread. 
Every sen object provided by every plugin shall be added to the main Sen component of the HMI via the `componentToLoad` method. 

## Code quality
- We shall use .pre-commit hooks for code formatting. We shall not use any pre-commit hook that require a docker image (e.g. hadolint)
    - clang-format
    - qmlformat
    - cmake-format


## Test strategy
- All C++ libraries must be unit tested (100% line coverage).
    - Use pure virtual interfaces for mocking and dependency injection
