================================================================================
   A Tic-Tac-Toe game with AI opponents, themes, and benchmarks
   Built with C and Raylib
================================================================================

Table of Contents
-----------------
  1.  Project Overview
  2.  Features
  3.  Screenshots & Game Flow
  4.  Quick Start
  5.  Building from Source
  6.  Project Structure
  7.  Architecture & Design
  8.  AI Algorithms
  9.  Benchmark Suite
  10. Test Suite
  11. Theme System
  12. Game History & Logging
  13. Requirements
  14. Contributing Guide
  15. Troubleshooting
  16. FAQ
  17. License

================================================================================
1. PROJECT OVERVIEW
================================================================================

This is a feature-rich Tic-Tac-Toe game written in C using the Raylib graphics
library. It includes multiple AI difficulty levels powered by different 
algorithms (classic Minimax, ML-based logistic regression), a theme system, 
a benchmarking suite, and game history tracking.

The game supports both single-player (vs AI) and two-player (local) modes 
with a polished, resizable GUI.

================================================================================
2. FEATURES
================================================================================

  * Game Modes
    - Single Player (vs AI) -- choose your symbol (X or O)
    - Two Player (local) -- play against a friend on the same screen

  * Three AI Difficulty Levels
    - Easy   -- Logistic regression model (ML-based evaluation)
    - Medium -- Minimax with 20% random move rate (imperfect play)
    - Hard   -- Perfect Minimax with alpha-beta pruning (unbeatable)

  * Five Color Themes
    - Default -- Clean blue & green on light gray
    - Dark    -- Blue accents on dark background
    - Forest  -- Earthy greens and browns
    - Space   -- Purple and yellow on near-black
    - Aquatic -- Turquoise and blue ocean tones

  * Audio Feedback
    - Click, win, lose, and draw sound effects (OGG format)

  * Resizable Window
    - All UI elements scale proportionally to the current window size
    - Design baseline: 1280x720

  * Game History
    - Results are saved to bin/game_history.txt after each game
    - View history from within the game via the History screen

  * AI Benchmark Suite
    - Compare performance of all AI algorithms
    - Measures execution time, stack usage, and game outcomes
    - Includes simulation harness for AI vs AI matchups

  * Automated Test Suite
    - 69 tests covering all core game logic
    - Tests AI correctness, board management, win detection, themes
    - Runs headlessly (no graphics window required)
    - One-click run script: run_tests.bat

================================================================================
3. SCREENSHOTS & GAME FLOW
================================================================================

The game follows a multi-screen state machine:

  +----------------+
  |  START SCREEN  |  <- Title, "Start Game" button
  +-------+--------+
          v
  +----------------+
  |  MODE SELECT   |  <- "1 Player" or "2 Player"
  +-------+--------+
          v
  +----------------+
  |  THEME SELECT  |  <- Pick from 5 color themes
  +-------+--------+
          v
  +----------------+
  | DIFFICULTY     |  <- Easy / Medium / Hard (1P only)
  |   SELECT       |
  +-------+--------+
          v
  +----------------+
  | SYMBOL SELECT  |  <- Choose X or O (1P: "You vs AI", 2P: "Player 1")
  +-------+--------+
          v
  +----------------+
  |  GAME SCREEN   |  <- The actual board, click to place pieces
  +-------+--------+
          v
  +----------------+
  |  GAME OVER     |  <- Win/Lose/Draw result overlay with replay option
  |   SCREEN       |
  +----------------+

  Additional screens accessible from the start menu:
  - INSTRUCTIONS -- How to play
  - HISTORY      -- View past game results

================================================================================
4. QUICK START
================================================================================

  Prerequisites:
    - Windows 10 or later
    - A pre-built binary is included in bin/TicTacToe.exe

  Option A -- Double-click:
    Simply double-click TicTacToe.bat in the repository root.

  Option B -- Command Line:
    > cd path\to\Tic-Tac-Toe
    > .\TicTacToe.bat

  The batch script presents an interactive menu:
    1. Build only         -- Compile the game without running
    2. Run only           -- Run the existing executable (no recompile)
    3. Build and Run      -- Compile, then immediately run
    4. Exit

  If the executable already exists in bin/, you can run it directly:
    > .\bin\TicTacToe.exe

================================================================================
5. BUILDING FROM SOURCE
================================================================================

  Requirements:
    * MinGW-GCC (C99 or later) -- must be in your system PATH
    * Raylib 4.x+ -- headers included in lib/raylib/, but you must have
      raylib.dll available (either in the project directory or system PATH)
    * Windows 10+ (uses Win32 APIs via Raylib)

  Build Commands:

  Using the batch script (recommended):
    > .\TicTacToe.bat
    Select option 1 (Build only) or 3 (Build and Run).

  Manual compilation (GCC/MinGW):
    > gcc -o bin\TicTacToe.exe ^
        main.c ^
        GUI_handlers\game_state.c ^
        GUI_handlers\screens.c ^
        GUI_handlers\ui.c ^
        Game_algorithms\Minimax.c ^
        Game_algorithms\model_minimax.c ^
        Game_algorithms\minimax_utils.c ^
        -Ilib\raylib ^
        -Llib\raylib ^
        -lraylib -lopengl32 -lgdi32 -lwinmm ^
        -static -lm

  The output binary is placed in bin/TicTacToe.exe.

  Important: The executable must be able to find the Raylib DLL and the
  resources/ folder (containing audio files). The batch script handles this
  by running the game from the bin/ directory.

================================================================================
6. PROJECT STRUCTURE
================================================================================

  Tic-Tac-Toe/
  |
  |-- main.c                          # Program entry point & main game loop
  |-- README.txt                      # This file
  |-- TicTacToe.bat                   # Build & run script (interactive menu)
  |-- run_benchmarks.bat              # Compile & run the benchmark suite
  |-- run_tests.bat                   # Compile & run the test suite
  |
  |-- Game_algorithms/                # AI and game logic
  |   |-- Minimax.c                   # Core minimax (perfect & imperfect play)
  |   |-- minimax.h                   # Minimax API -- Move struct, findBestMove*
  |   |-- minimax_utils.c             # Bitboard conversion, win masks, helpers
  |   |-- minimax_utils.h             # Bitboard utility declarations
  |   |-- model_minimax.c             # Logistic regression board evaluator
  |   |-- model_minimax.h             # ML model API
  |   |-- logistic_regression_params.json  # Trained model weights (JSON)
  |   +-- AI_development.ipynb        # Jupyter notebook for model training
  |
  |-- GUI_handlers/                   # Frontend: UI, screens, game state
  |   |-- game_state.c                # Game state init, board logic, themes
  |   |-- game_state.h                # GameState struct, Theme system, colors
  |   |-- screens.c                   # All screen draw & handle functions
  |   |-- screens.h                   # Screen function prototypes
  |   |-- ui.c                        # Reusable button & UI helpers
  |   +-- ui.h                        # UI helper declarations
  |
  |-- Benchmark Files/                # Performance & AI quality testing
  |   |-- benchmark.c                 # Main benchmark runner (Part 1 & 2)
  |   |-- benchmark_algorithms.c      # Alternative algorithm implementations
  |   |-- benchmark_algorithms.h      # Benchmark algorithm declarations
  |   +-- simulation.c                # AI vs AI simulation harness
  |
  |-- Tests/                          # Automated test suite
  |   |-- test_suite.c                # 69 tests across 9 test groups
  |   +-- raylib_stub.c               # Minimal raylib stubs for headless testing
  |
  |-- lib/
  |   +-- raylib/
  |       +-- raylib.h                # Raylib header (for compilation)
  |
  +-- bin/                            # Build output directory (gitignored)
      |-- TicTacToe.exe               # Compiled game executable
      |-- benchmark.exe               # Compiled benchmark executable
      |-- simulation.exe              # Compiled simulation executable
      |-- test_suite.exe              # Compiled test suite executable
      |-- game_history.txt            # Game results log (generated at runtime)
      |-- logistic_regression_params.json  # Copy of model params
      +-- resources/                  # Runtime assets
          |-- click.ogg               # Button click sound
          |-- win.ogg                 # Victory sound
          |-- lose.ogg                # Defeat sound
          +-- draw.ogg                # Draw sound

================================================================================
7. ARCHITECTURE & DESIGN
================================================================================

  The project follows a clean Model-View-Controller-like architecture:

  +---------------------------------------------------------------+
  |                         main.c                                |
  |   Initializes Raylib window (1280x720, resizable, 60 FPS)     |
  |   Runs the main game loop:                                    |
  |     1. Handle input  -> switch on game.screen -> HandleX...()  |
  |     2. Render frame  -> switch on game.screen -> DrawX...()    |
  +---------------------------------------------------------------+

  Key Design Patterns:

  * State Machine: The game.screen enum drives all screen transitions.
    Each screen has a separate Handle (input/logic) and Draw (render)
    function.

  * Global Game State: A single global `game` struct (GameState) holds all
    runtime state -- board, player symbols, difficulty, theme, screen, etc.

  * Theme System: Global Color variables are populated from a static array
    of Theme structs. Changing themes simply copies new colors into the
    globals.

  * Responsive UI: All sizes are computed relative to the current window
    dimensions using the ScaleSize() function (baseline: 1280x720).

  Module Responsibilities:
    - main.c             -> Entry point, window init, main loop dispatch
    - game_state.c/h     -> Game logic, board management, win detection, themes
    - screens.c/h        -> Screen rendering and input handling
    - ui.c/h             -> Reusable button widgets
    - Minimax.c/h        -> Minimax AI with adjustable difficulty
    - model_minimax.c/h  -> Logistic regression AI
    - minimax_utils.c/h  -> Bitboard helpers shared by AI modules
    - test_suite.c       -> Automated tests for all core logic
    - raylib_stub.c      -> Raylib stubs for headless test execution

================================================================================
8. AI ALGORITHMS
================================================================================

  The game features three distinct AI approaches:

  EASY -- Logistic Regression Model (model_minimax.c)
  ---------------------------------------------------
  Uses a pre-trained logistic regression model to evaluate board positions.
  A greedy one-step search evaluates all possible moves and picks the one
  with the highest score.

  * Model: 9 weights (one per cell) + bias
  * Encoding: X=+1, O=-1, Empty=0
  * Trained via AI_development.ipynb (Jupyter notebook)
  * Behavior: Makes "human-like" mistakes, good for casual play

  MEDIUM -- Imperfect Minimax (Minimax.c)
  ----------------------------------------
  Classic minimax with alpha-beta pruning and bitboard optimization.
  Uses a 20% error rate -- on each turn there is a 20% chance the AI will
  ignore strategy and play a random legal move instead.

  * Uses bitboard representation (two 9-bit masks for X and O)
  * Alpha-beta pruning for efficient search
  * Move ordering: Center -> Corners -> Edges (for better pruning)
  * Plays well but is beatable due to forced randomness

  HARD -- Perfect Minimax (Minimax.c)
  ------------------------------------
  The same minimax algorithm but with errorRate = 0 (no randomness).
  This produces mathematically optimal play -- the AI is unbeatable.

  * Full depth search with alpha-beta pruning
  * Bitboard representation for maximum speed
  * Tic-Tac-Toe is a solved game -- perfect play always draws

================================================================================
9. BENCHMARK SUITE
================================================================================

  The benchmark suite (in Benchmark Files/) compares AI algorithms on two
  axes: performance (speed, stack usage) and game-play quality.

  Running Benchmarks:
    > .\run_benchmarks.bat

  What it tests:

  Part 1 -- AI Difficulty Benchmarks:
    - Easy Mode (Logistic Regression):      10,000 iterations
    - Medium Mode (Imperfect Minimax 20%):  10,000 iterations
    - Hard Mode (Perfect Minimax):          10,000 iterations
    Measures execution time for each difficulty level.

  Part 2 -- Optimization Benchmarks:
    Compares four algorithm variants on an empty board (worst case):
    1. Bitboard + Alpha-Beta Pruning    (production version)
    2. Array-based + Alpha-Beta Pruning
    3. Array-based, NO pruning           (slowest baseline)
    4. Bitboard, NO pruning
    Measures execution time and max stack depth for 1,000 iterations each.

  Benchmark Files:
    - benchmark.c              -- Main benchmark runner with timing
    - benchmark_algorithms.c/h -- Alternative implementations for comparison
    - simulation.c             -- AI vs AI simulation (playouts)

================================================================================
10. TEST SUITE
================================================================================

  The automated test suite verifies that all core game logic works correctly.
  It runs entirely in the console (no graphics window or Raylib library needed
  at runtime) by using minimal stubs for Raylib audio functions.

  Running the Tests:
    > .\run_tests.bat

  This compiles and runs the test suite. All 69 tests should pass.

  Manual Compilation:
    > gcc -o Tests\test_suite.exe ^
        Tests\test_suite.c ^
        Tests\raylib_stub.c ^
        GUI_handlers\game_state.c ^
        Game_algorithms\Minimax.c ^
        Game_algorithms\minimax_utils.c ^
        Game_algorithms\model_minimax.c ^
        -Ilib\raylib -I. -lm

  Test Groups (69 tests total):

    1. Bitboard Utilities (13 tests)
       - countBits: empty, single-bit, multi-bit masks
       - isWinnerMask: all 8 win lines, no-winner cases
       - boardToMasks: empty, full, winning boards
       - getPlayerMasks: AI as X, AI as O, mid-game detection

    2. CheckWinner (12 tests)
       - All 8 winning lines (rows, columns, diagonals)
       - No-win partial boards
       - Win statistics updates for 1-player and 2-player modes

    3. IsBoardFull (4 tests)
       - Empty boards, partial boards, full boards
       - Draw counter incrementing

    4. ResetBoard (6 tests)
       - Board clearing, player turn order
       - AI goes first when human chooses O
       - Statistics preservation across resets

    5. Minimax AI - Perfect Play (12 tests)
       - Finding winning moves (rows, columns, diagonals)
       - Blocking opponent threats
       - Valid move generation, full-board edge case
       - 100-game perfect-vs-perfect validation (all draws)
       - Fork (double-threat) creation

    6. Model AI (6 tests)
       - Valid move generation for X and O
       - Logistic regression evaluation scores
       - Full-board edge case (no crash)

    7. Theme Management (5 tests)
       - All 5 themes, invalid theme fallback
       - Color variable updates on theme change

    8. MakeAIMove (4 tests)
       - All three difficulty levels (Hard, Medium, Easy)
       - Winning move execution

    9. Edge Cases & Integration (7 tests)
       - Win detection consistency (bitboard vs array)
       - Full game simulation (perfect play -> draw)
       - Board-to-mask round-trip verification
       - Move ordering and bit count consistency

  Test Design:
    The test suite uses a lightweight custom framework (no external dependencies)
    with ASSERT_TRUE, ASSERT_FALSE, ASSERT_EQ_INT, and ASSERT_EQ_CHAR macros.
    Each test function is self-contained and resets the global game state before
    running to prevent state leakage between tests.

  Key Verification:
    The test_minimax_never_loses_from_empty test plays 100 complete games
    with perfect minimax on both sides. All 100 games result in draws,
    confirming that the AI is mathematically unbeatable.

================================================================================
11. THEME SYSTEM
================================================================================

  Five built-in color themes are available via the Theme Select screen.

  Each theme defines 7 colors:
    - background  -> Page background
    - primary     -> Main buttons, X pieces
    - secondary   -> Secondary buttons, O pieces
    - accent      -> Highlights, special elements
    - warning     -> Alerts, caution elements
    - dark        -> Grid lines, text on light surfaces
    - light       -> Text on dark surfaces

  Available Themes:
    THEME_DEFAULT  -- Light gray background, blue/green accents
    THEME_DARK     -- Dark background, blue accents
    THEME_FOREST   -- Pale honeydew background, green/brown accents
    THEME_SPACE    -- Near-black background, purple/yellow accents
    THEME_AQUATIC  -- Light cyan background, turquoise/blue accents

  Adding a New Theme:
    1. Add a new entry to the ThemeID enum in game_state.h
    2. Add the color palette to the allThemes[] array in game_state.c
    3. The theme will automatically appear in the Theme Select screen

================================================================================
12. GAME HISTORY & LOGGING
================================================================================

  After each completed game, the result is appended to:
    bin/game_history.txt

  This file records the outcome (Win/Lose/Draw), the game mode, difficulty,
  and which symbol you played. You can view the history in-game via the
  History screen, or open the file directly in any text editor.

================================================================================
13. REQUIREMENTS
================================================================================

  Runtime (to play the game):
    * Windows 10 or later
    * raylib.dll (must be alongside TicTacToe.exe or in system PATH)
    * Visual C++ Redistributable (for MinGW-compiled binaries)
    * GPU with OpenGL 3.3+ support (for Raylib rendering)

  Development (to build/modify):
    * MinGW-GCC (C99+) -- tested with gcc 12.x+
    * Raylib 4.x+ development library
    * (Optional) Python 3 + Jupyter -- for retraining the ML model
      (see Game_algorithms/AI_development.ipynb)

  Testing (to run the test suite):
    * MinGW-GCC (C99+) -- same as development
    * No Raylib DLL or window required (uses stub functions)

================================================================================
14. CONTRIBUTING GUIDE
================================================================================

  Code Organization Conventions:
    * AI & game logic   -> Game_algorithms/
    * UI & screens      -> GUI_handlers/
    * Benchmarks        -> Benchmark Files/
    * Tests             -> Tests/
    * Runtime assets    -> bin/resources/

  Coding Style:
    * C99 standard with extensive Doxygen-style comments
    * Each module has a paired .c/.h file
    * Headers use include guards (#ifndef FOO_H / #define FOO_H)
    * Global state is minimized (only `game` and color globals)

  Making Changes:
    * AI changes:  Edit files in Game_algorithms/, then rebuild
    * UI changes:  Edit files in GUI_handlers/, then rebuild
    * New themes:  See "Adding a New Theme" in Section 11
    * Benchmarks:  Add new tests in Benchmark Files/, update run_benchmarks.bat
    * Tests:       Add new tests in Tests/test_suite.c, update run_tests.bat

  Build & Test Workflow:
    1. Make your changes
    2. Build with TicTacToe.bat (option 1 or 3)
    3. Run the automated test suite: run_tests.bat
    4. Test the game manually
    5. Run benchmarks with run_benchmarks.bat to check for regressions

  Adding New Tests:
    1. Open Tests/test_suite.c
    2. Add a new TEST() function following the existing pattern
    3. Register it in main() using RUN_TEST()
    4. Run run_tests.bat to verify your test passes

================================================================================
15. TROUBLESHOOTING
================================================================================

  Problem: "GCC not found in PATH!"
    Solution: Install MinGW or MSYS2. Add the bin/ directory of your MinGW
    installation to the Windows PATH environment variable. Restart your
    terminal after updating PATH.

  Problem: "raylib.dll not found" or link errors at compile time
    Solution: Ensure Raylib is properly installed. The header is at
    lib/raylib/raylib.h for compilation, but you also need:
      - At compile time: the .a/.lib file in a -L path
      - At runtime: raylib.dll in the same directory as TicTacToe.exe
    Download Raylib from: https://github.com/raysan5/raylib

  Problem: Graphics issues / black screen / window doesn't render
    Solution: Update your GPU drivers. Raylib uses OpenGL -- ensure your
    GPU supports at least OpenGL 3.3. Try running with the window at its
    default size (1280x720) before resizing.

  Problem: "COMPILATION FAILED!" when building
    Solution: Check the GCC error messages. Common causes:
      - Missing Raylib library files (.a files)
      - Incorrect -I or -L paths in the compile command
      - C99-specific syntax used with a pre-C99 compiler
      - Verify all .c and .h files are present in the expected directories

  Problem: No sound / audio not working
    Solution: Ensure the bin/resources/ directory contains all .ogg files
    (click.ogg, win.ogg, lose.ogg, draw.ogg). The game must be run from
    the bin/ directory, or the resource paths must be adjusted.

  Problem: Benchmark executable crashes
    Solution: The benchmark suite is console-only (no Raylib dependency).
    Make sure all Game_algorithms/ source files are present and that gcc
    can find them via the -I include paths.

  Problem: Test suite fails to compile
    Solution: The test suite uses stubs for Raylib functions. Ensure:
      - Tests/raylib_stub.c is present
      - The -Ilib\raylib flag is included (for the raylib.h header)
      - All Game_algorithms/ and GUI_handlers/ source files are present

================================================================================
16. FAQ (FREQUENTLY ASKED QUESTIONS)
================================================================================

  Q: Can I play this on macOS or Linux?
  --------------------------------------
  A: The game uses Raylib which is cross-platform, and the C code is
  platform-agnostic. However, the provided .bat scripts and compile
  commands are Windows-specific (they use MinGW-GCC with Win32 linker
  flags like -lgdi32 and -lwinmm).

  To build on macOS/Linux:
    1. Install Raylib for your platform (brew install raylib on macOS,
       or build from source on Linux).
    2. Compile with the appropriate flags:
       gcc -o TicTacToe main.c GUI_handlers/*.c Game_algorithms/*.c \
           -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    3. Adjust include paths as needed for your Raylib installation.

  -------------------------------------------------------------------------

  Q: Is the Hard AI really unbeatable?
  -------------------------------------
  A: Yes. The Hard difficulty uses a perfect Minimax algorithm with full-
  depth search and alpha-beta pruning. Since Tic-Tac-Toe is a solved game,
  the AI will never lose. The best possible outcome against it is a draw.
  You can verify this by running AI vs AI simulations in the benchmark
  suite, or by running the test suite (test_minimax_never_loses_from_empty
  plays 100 perfect-vs-perfect games and verifies all end in draws).

  -------------------------------------------------------------------------

  Q: How was the ML model for Easy mode trained?
  -----------------------------------------------
  A: The logistic regression model was trained in the Jupyter notebook at
  Game_algorithms/AI_development.ipynb. It has 9 weights (one per board
  position) plus a bias term. The model was trained on completed game
  boards to predict the probability of X winning. The trained parameters
  are stored in Game_algorithms/logistic_regression_params.json.

  To retrain the model:
    1. Open AI_development.ipynb in Jupyter
    2. Modify the training data or hyperparameters as desired
    3. Export the new weights to logistic_regression_params.json
    4. Update the LR_WEIGHTS and LR_BIAS constants in model_minimax.c
    5. Rebuild the game

  -------------------------------------------------------------------------

  Q: What is a "bitboard" and why does the AI use one?
  -----------------------------------------------------
  A: A bitboard represents the game board as two 9-bit integers -- one for
  X positions and one for O positions. Each bit maps to a cell:

    Bit:  0  1  2       Cell: [0][0] [0][1] [0][2]
          3  4  5             [1][0] [1][1] [1][2]
          6  7  8             [2][0] [2][1] [2][2]

  Benefits:
    * Win checking is a single bitwise AND (vs. multiple array lookups)
    * Move generation is a bitwise NOT + AND (finds empty cells)
    * Very cache-friendly -- entire board fits in two CPU registers
    * The benchmarks show bitboards are significantly faster than arrays

  -------------------------------------------------------------------------

  Q: How do I add a new AI difficulty level?
  -------------------------------------------
  A: The AI difficulty is controlled by the errorRate parameter in
  findBestMoveMinimax(). Here is how to add a new level:

    1. In game_state.h, add a new entry to the Difficulty enum
    2. In screens.c, add a button for the new difficulty in the Draw
       and Handle functions
    3. In game_state.c, map the new difficulty to an errorRate value
       (e.g., 10 for a "Medium-Hard" between Medium's 20 and Hard's 0)
    4. Rebuild the game

  Alternatively, create a completely new algorithm (like model_minimax.c)
  and wire it into the MakeAIMove() function in game_state.c.

  -------------------------------------------------------------------------

  Q: Can I change the window resolution?
  ---------------------------------------
  A: Yes! The window is resizable -- just drag the edges. The default
  resolution is 1280x720 (set in main.c via InitWindow). All UI elements
  automatically scale using the ScaleSize() function, which converts from
  design coordinates (1280x720) to the actual window size.

  To change the default resolution, edit the InitWindow() call in main.c:
    InitWindow(YOUR_WIDTH, YOUR_HEIGHT, "Tic-Tac-Toe");

  -------------------------------------------------------------------------

  Q: Where is game data stored?
  ------------------------------
  A: All runtime data is stored in the bin/ directory:
    - bin/TicTacToe.exe              -- The game executable
    - bin/game_history.txt           -- Game results log
    - bin/logistic_regression_params.json -- Model parameters (copy)
    - bin/resources/*.ogg            -- Audio assets

  The game_history.txt file is created automatically and appended to after
  each game. It is safe to delete -- the game will recreate it.

  -------------------------------------------------------------------------

  Q: Why does the game use static linking (-static flag)?
  -------------------------------------------------------
  A: Static linking bundles the C runtime library into the executable,
  so users do not need to install a separate MSVCRT DLL. However, Raylib
  itself is still dynamically linked (via raylib.dll) because the static
  Raylib library (.a file) may not be included in all distributions.

  If you have the static Raylib library, you can link it statically too:
    - Replace -lraylib with the full path to libraylib.a
    - Add -DSTATIC or similar flags if required by your Raylib build

  -------------------------------------------------------------------------

  Q: How do I run the AI vs AI simulation?
  -----------------------------------------
  A: The simulation harness is in Benchmark Files/simulation.c. To run it:

    1. Compile: The run_benchmarks.bat script compiles simulation.exe
       into bin/. Alternatively, compile manually:

       gcc -o bin\simulation.exe ^
           "Benchmark Files\simulation.c" ^
           "Game_algorithms\Minimax.c" ^
           "Game_algorithms\model_minimax.c" ^
           "Game_algorithms\minimax_utils.c" ^
           -I"Game_algorithms" ^
           -static -lm

    2. Run: bin\simulation.exe

  The simulation pits different AI algorithms against each other over
  many games and reports win/draw/loss statistics.

  -------------------------------------------------------------------------

  Q: How do I run the automated tests?
  -------------------------------------
  A: The test suite is in Tests/test_suite.c. To run it:

    1. Quick method: Double-click run_tests.bat or run it from a terminal:
       > .\run_tests.bat

    2. Manual method:
       > gcc -o Tests\test_suite.exe Tests\test_suite.c Tests\raylib_stub.c ^
           GUI_handlers\game_state.c Game_algorithms\Minimax.c ^
           Game_algorithms\minimax_utils.c Game_algorithms\model_minimax.c ^
           -Ilib\raylib -I. -lm
       > .\Tests\test_suite.exe

  The test suite requires NO graphical window or Raylib DLL -- it runs
  entirely in the console using stub functions for Raylib audio calls.

  -------------------------------------------------------------------------

  Q: Can I use this code in my own project?
  ------------------------------------------
  A: This project was created for educational purposes. The AI algorithms
  (minimax, logistic regression evaluation), UI patterns (state machine,
  theme system), and benchmark infrastructure can serve as learning
  references. Check with the repository owner regarding licensing before
  redistributing.

  -------------------------------------------------------------------------

  Q: I found a bug. How do I report it?
  --------------------------------------
  A: Open an issue on the repository (if hosted on GitHub/GitLab) with:
    - Steps to reproduce
    - Expected vs. actual behavior
    - Your OS and GCC version
    - Any relevant error messages or screenshots

  If the repository is not publicly hosted, contact the maintainer directly.

================================================================================
17. LICENSE
================================================================================

  Educational / project use. Check course or instructor policies before
  external distribution. See repository owner for licensing details.

================================================================================
Last Updated: April 2026
================================================================================
