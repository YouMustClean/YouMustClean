# YCM: YouMustClean

## Building WallpaperGenerator

### Requirements

| Library                                  | Tested Version |
| ---------------------------------------- | -------------- |
| OpenCV                                   | 4.6.0          |
| OpenCV's extra modules (opencv\_contrib) | 4.6.0          |
| FreeType                                 | 2.12.1         |

**Build System:** We use CMake as the only supported build system, so please make sure you have installed CMake for building YMC.

#### On Ubuntu

Clone our repository first.

```bash
## To whatever directory you want
cd <root_of_repos>
git clone https://github.com/YouMustClean/YouMustClean
## Or
git clone git@github.com:YouMustClean/YouMustClean
```

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

Many OpenCV modules will not be used by this project. If you also don't need these modules, you can use the flags provided by us in `YouMustClean/cmake_opencv4_flags.txt` to ignore these unused modules while building OpenCV.

```bash
mkdir opencv/build && cd opencv/build
cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules `cat <root_of_repos>/YouMustClean/cmake_opencv4_flags.txt` ..
sudo make install -j4

## Or, if you want to compile all default modules
mkdir opencv/build && cd opencv/build
cmake -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
sudo make install -j4
```

Note that if you are putting `opencv_contrib` somewhere else, you should replace that parameter with your own path.

P.S. The given process will build default modules of OpenCV and all extra modules in opencv\_contrib. If you don't want all of them, or if you want to know something more, you can learn it [here](https://github.com/opencv/opencv_contrib).
