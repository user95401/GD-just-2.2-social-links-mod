# GD just 2.2 social links mod
this thing will add 2 more RobTop's social links to the Geometry Dash menu that you may see in update 2.2.<br>
size and position are near to the original)<br>
the first С++ mod in my life...<br>
![bandicam 2022-12-07 22-32-23-123 (2) (2)](https://user-images.githubusercontent.com/90561697/206267208-033cc289-9648-44b0-b593-5f1147b5e4b6.gif)
<br>CMake + MinHook + gd.h + cocos2d.
BASED ON [matcool's gd-mod-example](https://github.com/matcool/gd-mod-example)
## Install
##### Images drop to game folder
![Screenshot_11](https://user-images.githubusercontent.com/90561697/206272981-092cc330-f441-47c2-a023-193c2f3acafe.png)
##### or Resources folder
![Screenshot_12](https://user-images.githubusercontent.com/90561697/206273262-0003d9f9-83d1-460c-8494-0fd9a9e4ac08.png)
#### gd22links.dll inject to gd or drop to modloader(dll loader) folder
### Compile
1. Configure
``` bash

cmake -B build -A win32
```
2. Build
```bash
cmake --build build --config Release
```
