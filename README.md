# waied
What Am I Even Doing

![image](https://user-images.githubusercontent.com/5008338/48269613-4adb7d00-e438-11e8-9591-218e1757d050.png)

## Build How To

### Ubuntu 18.04

Install tools and dependencies:

```shell
sudo apt install make cmake gcc-8 g++-8 \
                 qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev qtquickcontrols2-5-dev
```

Setup env:

```shell
export CXX=g++-8
export CC=gcc-8
```

Build

```shell
mkdir build
cd build
cmake ..
cmake --build .
```

### Ubuntu 16.04

Defauld ubuntu repos contain quite old packages, so it's necessary to add some ppa's:

```shell
sudo apt update
sudo apt install software-properties-common
sudo add-apt-repository ppa:beineri/opt-qt-5.11.1-xenial
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
```

Install packages:

```shell
sudo apt update
sudo apt install wget make gcc-8 g++-8 libssl-dev mesa-common-dev libglu1-mesa-dev \
                 qt511base qt511declarative qt511multimedia qt511quickcontrols2
```

Next, get some recent `cmake`:

```shell
wget https://cmake.org/files/v3.12/cmake-3.12.3-Linux-x86_64.sh
sudo mkdir /opt/cmake
sudo bash ./cmake-3.12.3-Linux-x86_64.sh --skip-license --exclude-subdir --prefix=/opt/cmake/
```

Setup env:

```shell
export PATH=/opt/cmake/bin:$PATH
export CXX=g++-8
export CC=gcc-8
/opt/qt511/bin/qt511-env.sh
```

Build

```shell
git submodule update --init --recursive
mkdir build
cd build
cmake ..
cmake --build .
```
