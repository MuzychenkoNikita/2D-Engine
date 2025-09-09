# 2D Engine - by Nikita Muzychenko

## Mission
This project is a small self training to get important skills for working in game industry. The main thing there is the self written library for making 2d game development easier with a small demonstartion of this library usage within an example JRPG game. If you want to use this library for your own project, you are free to go. Any contribution or discussion are also always welcome.

## How to use
Project has support for Windows, MacOS and Linux. All necessary libraries and includes are already within a directory. Scripts for relevant platforms are provided within a `Scripts/` directory to setup project using premake5.
- Windows: open `Scripts/` and run `Setup-Windows.bat`, then open generated Visual Studio solution in root directory `2D-Engine/`
- MacOs: open `Scripts/` with terminal and run `sh Setup-Mac.sh`. If you getting permission errors, you have to give permission for scrpits by typing next two commands `chmod +x ../Vendor/Binaries/Premake/macOS/premake5` and `chmod +x Setup-Mac.sh`. After that Xcode solution will be generated in root directory `2D-Engine/`
- Linux: within 'Scripts/' directory run `chmod +x script.sh && ./script.sh`. Then open generated Gmake solution.

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)
