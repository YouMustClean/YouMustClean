# YCM: YouMustClean

## Building

### Requirements

| Library                                  | Tested Version |
| ---------------------------------------- | -------------- |
| OpenCV                                   | 4.6.0          |
| OpenCV's extra modules (opencv\_contrib) | 4.6.0          |
| FreeType                                 | 2.12.1         |

**Build System:** We use CMake as the only supported build system, so please make sure you have installed CMake for building YMC.

#### On Ubuntu

You must build required libraries under the following procedure, otherwise your compilation might fail at any possible point.

Compile [**FreeType**](https://freetype.org/) first. It's required to build module `opencv_freetype`.<br>
You can download FreeType [here](https://download.savannah.gnu.org/releases/freetype/).

```bash
## Run ./configure is recommended.
./configure
sudo make install -j4

## You can use CMake if you want to.
mkdir build && cd build
cmake ..
sudo make install -j4
```

**OpenCV** and **OpenCV's extra modules** will be built together.

You can place two repositories like this:

```bash
.
├── opencv/
│   └── build/
├── opencv_contrib/
│   └── modules/
└── freetype-<version>/
```

Checkout to the correct version before you compile them. Note that OpenCV and opencv\_contrib should have the SAME VERSION.

```bash
## Change the version of OpenCV
cd opencv
git checkout <version>
cd ..

## Change the version of opencv_contrib
cd opencv_contrib
git checkout <version>
cd ..
```

Then build OpenCV like this:

```bash
mkdir opencv/build && cd opencv/build
cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
sudo make install -j4
```

Note that if you are putting `opencv_contrib` somewhere else, you should replace that parameter with your own path.

P.S. The given process will build default modules of OpenCV and all extra modules in opencv\_contrib. If you don't want all of them, or if you want to know something more, you can learn it [here](https://github.com/opencv/opencv_contrib).
