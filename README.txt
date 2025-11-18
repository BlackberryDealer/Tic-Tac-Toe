================================================================================
                        TIC-TAC-TOE GAME
              Programming Methodologies Assignment
                     CSC1103 - Group [Your Number]
================================================================================

██████╗ ██╗   ██╗██╗ ██████╗██╗  ██╗    ███████╗████████╗ █████╗ ██████╗ ████████╗
██╔═══██╗██║   ██║██║██╔════╝██║ ██╔╝    ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝
██║   ██║██║   ██║██║██║     █████╔╝     ███████╗   ██║   ███████║██████╔╝   ██║   
██║▄▄ ██║██║   ██║██║██║     ██╔═██╗     ╚════██║   ██║   ██╔══██║██╔══██╗   ██║   
╚██████╔╝╚██████╔╝██║╚██████╗██║  ██╗    ███████║   ██║   ██║  ██║██║  ██║   ██║   
 ╚══▀▀═╝  ╚═════╝ ╚═╝ ╚═════╝╚═╝  ╚═╝    ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                    ✨ TO PLAY THE GAME ✨
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

    >>> DOUBLE-CLICK: run.bat

    That's all! No installation needed. Just run and play!

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


SYSTEM REQUIREMENTS:
--------------------
- Windows 7 or later (64-bit)
- Mouse and keyboard
- OpenGL 1.1+ compatible graphics card

✓ No MinGW installation needed
✓ No Raylib installation needed
✓ No compiler needed


GAME FEATURES:
--------------
✦ Single Player Mode (vs AI)
  - Perfect AI (unbeatable)
  - Imperfect AI (beatable)
  
✦ Two Player Mode (local multiplayer)

✦ Beautiful GUI with smooth animations

✦ Game history and save/load functionality


TROUBLESHOOTING:
----------------
Problem: "TicTacToe.exe not found"
Solution: Ensure you extracted the entire folder, not just run.bat

Problem: Nothing happens when clicking run.bat
Solution: Right-click run.bat → "Run as administrator"

Problem: Missing DLL errors
Solution: Install Visual C++ Redistributable:
         https://aka.ms/vs/17/release/vc_redist.x64.exe

Problem: Graphics not displaying
Solution: Update your graphics drivers


━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
                  FOR DEVELOPERS ONLY
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

If you want to rebuild the project from source code:

Requirements:
  ✓ MinGW GCC compiler
  ✓ Raylib library (included in lib/raylib/)

Steps:
  1. Install MinGW: https://www.mingw-w64.org/
  2. Add MinGW\bin to system PATH
  3. Double-click: build_and_run.bat

Note: Regular users should NOT use build_and_run.bat
      It requires a C compiler and is only for developers.


PROJECT STRUCTURE:
------------------
Tic-Tac-Toe/
├── run.bat              ← RUN THIS to play the game
├── bin/
│   ├── TicTacToe.exe    ← Pre-compiled game executable
│   └── *.json           ← Game configuration files
├── Team_A/              ← GUI implementation (screens, UI, state)
├── Team_B/              ← AI logic (minimax, gameboard, player modes)
├── lib/raylib/          ← Graphics library files
├── main.c               ← Program entry point
├── build.bat            ← (Developers) Compile source code
└── build_and_run.bat    ← (Developers) Compile and run


TEAM MEMBERS:
-------------
Team A (GUI): [Insert Names]
Team B (AI/Logic): [Insert Names]


VERSION HISTORY:
----------------
v1.0 (November 2025) - Initial release


DOCUMENTATION:
--------------
For detailed technical documentation, see:
- PMReportGroupXX.pdf (Full project report)
- Source code comments in Team_A/ and Team_B/ folders


CONTACT:
--------
[Your University]
Course: CSC1103 - Programming Methodologies
Instructor: [Instructor Name]


LICENSE:
--------
Educational project - For academic use only


================================================================================
              🎮 ENJOY THE GAME! 🎮
================================================================================
