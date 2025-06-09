# MyOpenGl
## Description
This is a wrapper around opengl integrating a bit with wxwidges for texture <=> image convertions


## Downloading 
Don't forget the submodules
```bash
git clone --recurse-submodules --shallow-submodules git@github.com:Mozzarella32/MyOpenGl.git
```

# Building
Go to the root of this Project and run these commands the -j8 flag is for faster compilation and can be omitted
```bash
cd MyOpenGl
mkdir build
cd build
cmake ../ && cmake --build . -j8
```
