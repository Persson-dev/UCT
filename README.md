# UCT
UCT is a french software to improve productivity. 
It uses : 
- [GLFW](https://www.glfw.org) (to help managing the window)
- [ImGui](https://github.com/ocornut/imgui) (for the gui)
- [Json](https://github.com/nlohmann/json) (for json saving)

English translations will be added if really needed. For now, this is just a small project (the code is still a mess).

# Screenshots

![image](https://user-images.githubusercontent.com/66266021/182838420-fda60085-e208-47ae-b8d2-adec41871d71.png)
![image](https://user-images.githubusercontent.com/66266021/182838188-717971be-2270-48a9-a3eb-48f2b8584f63.png)

# Build
You will need a recent compiler supporting c++17 and [xmake](https://xmake.io/#/getting_started).
Just cd into the project folder and type :
```
xmake
```
xmake should download all the depencencies for you.

# Run
```
xmake run -w test
```

# Install
Currently, there is no install script so you should just copy the binary.

The binary should be located in the build folder.
