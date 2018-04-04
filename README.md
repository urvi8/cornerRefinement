# Corner Refinement

Understanding and improving biased behavior of corner detectors

## Background and Objective

Standard corner detectors e.g. Harris sometimes produce biased estimation of corners. These detectors detect actual corners under very specific conditions and are often not scale invariant. The goal is to understand why the detector mentioned above is biased and find an alternative which exhibits better behavior.

## Getting Started

### Platform used: Ubuntu 14.04 LTS

## Dependencies
[OpenCV 2.4](https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html)

## Installation

### Follow these steps to install and build the current project.

#### Clone the respository in desired directory

```
git clone(http://github.com/urvi8/Corner-Refinement)

```

#### Build package

```
mkdir build
cd build
cmake ..
```

#### Compile the project

```
make
```

#### Run executable

Follow the syntax _executable_  _path/to/test/image_  _sizeOfHarrisWIndowRadius_

An example would be
```
CornerRefine /home/user/Corner-Refine/test-image 7
``` 
The result will appear in a window and will be saved to the current directory.

## Built With

[CMake] (https://cmake.org/install/)
