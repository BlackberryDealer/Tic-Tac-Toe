================================================================================
TIC-TAC-TOE
Project summary and developer notes for the current repository layout
================================================================================

**Project**
- **Name:**: Tic-Tac-Toe (C, simple GUI + AI)
- **Location:**: repository root contains source, GUI handlers, AI, benchmarks, and helper scripts.

**Quick Start (Windows)**
- **Run game:**: Double-click or run the batch `\`TicTacToe.bat\``
- **Run benchmarks:**: Use `\`run_benchmarks.bat\``

PowerShell example:
```powershell
.\TicTacToe.bat
.\run_benchmarks.bat
```

**What’s in this repo**
- **`main.c`:**: Program entry point for the game.
- **`TicTacToe.bat`:**: Convenience script to launch the game (Windows).
- **`run_benchmarks.bat`:**: Runs benchmark suite found in `Benchmark Files/`.
- **`Game_algorithms/`:**: AI and game logic (minimax, helpers, models).
- **`GUI_handlers/`:**: UI, screens, and game state handling for the front-end.
- **`Benchmark Files/`:**: Benchmarking code and simulation harnesses.
- **`bin/`:**: Binary outputs and generated `game_history.txt` (runtime artifacts).
- **`resources/`:**: Assets used by the game (images, data, etc.).

**Benchmarks & Tests**
- **Benchmarks:**: See `Benchmark Files/` for `benchmark.c`, `benchmark_algorithms.c`, and harnesses.
- **How to run:**: Use `\`run_benchmarks.bat\`` (this will compile/run the benchmark code if needed).

**Build from source (developers)**
- **Note:**: The repository includes `lib/raylib/` headers — building raylib or linking the library may be required.
- **Typical GCC (MinGW) compile example:**:
```powershell
:: Example only — adjust include/lib paths for your environment
gcc -o TicTacToe.exe main.c Game_algorithms/*.c GUI_handlers/*.c -Ilib/raylib -Llib/raylib -lraylib -lopengl32 -lgdi32 -lwinmm
```
- **Recommendation:**: Use the provided batch files when available: `\`TicTacToe.bat\`` or any developer `build` scripts present in the repo.

**Notes & Troubleshooting**
- **Missing DLLs or link errors:**: Install Visual C++ Redistributable and ensure MinGW/MSYS2 or Visual Studio toolchain is configured if you plan to build.
- **Graphics issues:**: Update GPU drivers; raylib uses standard Windows graphics backends.

**Contributing / Code layout guidance**
- **AI & logic:**: Work inside `Game_algorithms/` (minimax, model files, training logs).
- **UI changes:**: Edit `GUI_handlers/` (`ui.c`, `screens.c`, `game_state.c`).
- **Benchmarks:**: Use `Benchmark Files/` and `run_benchmarks.bat` to validate algorithm performance changes.

**License & Contact**
- **License:**: Educational / project use; check course or instructor policies before external distribution.
- **Contact:**: See repository owner or course metadata for maintainers.

================================================================================
Updated: November 2025
================================================================================
