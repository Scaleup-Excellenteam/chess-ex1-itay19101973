<table>
<tr style="border: none">
<td style="border: none">

# THE Chess Template Repository

</td>
<td align="right" style="border: none">
<img src="./img/scaleup.png" alt="Scaleup" height="100">
</td>
</tr>

</table>
This is a template repository for the Tel Hai Excellenteam (THE) CPP course.

All exercises and submissions should follow the format of this repository. For your convenience, you can start each assignment by cloning this template.

## Environment Setup
During the course we will utilize Linux based operating system (OS), to run and execute programs.

### Windows Installation
1. Install [WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install
) , with Ubuntu distribution.


### Mac Installation
1. Follow the instruction presented in the following [video]( https://www.youtube.com/watch?v=LjL_N0OZxvY
), install Ubuntu (no GUI) version
2. To install GUI, use following [guide](https://askubuntu.com/questions/53822/how-do-you-run-ubuntu-server-with-a-gui
)
3. In case you forgot the default credentials, read following [article](https://www.debugpoint.com/virtualbox-id-password/
).


### Linux Installation
Ensure you have `g++`, `CMake`, and `make` installed:

```sh
sudo apt update
sudo apt install -y g++ cmake make
```

## Grading and Conventions
Your assignment will be graded according to the following criteria. Please make sure your assignment follows the standards and conventions outlined below:


For best practices please use examples presented in [PRACTICES.md](PRACTICES.md), if still there is an uncertainty or missing use cases, you are highly encouraged to contact the staff for further clarifications.

### Branching
Use the [CONTRIBUTING.md](CONTRIBUTING.md) file as your guideline for proper use of Git. For more information, you are encouraged to search online for "[GitHub Workflow Methodology](https://www.youtube.com/watch?v=U_IFGpJDbeU&ab_channel=DevOpsToolkit)."


### Repository Structure
Please follow the guidelines in this section strictly.

- The repository should include a `CMakeLists.txt` file at the root directory.
- The repository **MUST NOT** include any compiled binaries (e.g., build/, .o files, a.out, or any other generated executables).(put those file names in `.gitignore` file)

<p align="center">
  <img src="./img/cpp-logo.png" alt="C++ Logo" width="100" height="100">
  <img src="./img/linux-logo.jpg" alt="Linux Logo" width="100" height="100">
</p>    

#### GitHub Configuration
* The repository should include a `README.md` file at the root directory.
* The repository should include a `CONTRIBUTING.md` file at the root directory.
* You must have a `.gitignore` file, and there shouldn’t be any unnecessary files in the repository.
* 🚨 **CRITICAL:** Repositories without a workflow file at `.github/workflows/c-cpp.yml` will not be graded. :(
* You must have an `img` directory at the root directory.

#### Project's Files

- The repository should include a `main.cpp` file in src directory.
- The repository should include a `src` directory at the root.
  - All source files should be placed in the `src` directory.
- The repository should include a `include` directory at the root.
  - All `.h` files should be placed in the `include` directory.
- The repository should include a `test` directory at the root.
  - All tests should be placed in the `test` directory.


#### Project Tree

Project tree should match the following structure:

```bash
.
├── CONTRIBUTING.md
├── .git
│   ├── ...
├── .github
│   └── workflows
│       └── c-cpp.yml
├── .gitignore
├── img
│   ├── excellenteam.png
│   ├── scaleup.png
│   └── cpp_logo.png
├── README.md
├── CMakeLists.txt
├── src
│   ├── main.cpp
│   ├── example.cpp
│   ├── ...
├── include
│   ├── example.h
│   ├── ...
├── tests
│   ├── ...
```
## Compilation Instructions
The evaluators will compile your code using **CMake**. Ensure your project follows the CMake structure correctly.

To compile your project manually, use the following commands:

```sh
mkdir -p build
cd build
cmake ..
make
./Chess 
```

Any project that does not compile properly using these steps will not be graded.


## How to Submit an Exercise
You are required to submit each exercise using "GitHub Classroom". To do this, you must upload a link to your "GitHub Classroom" repository via Moodle..

### Branching and Pull Request Guidelines

- 🚨 Your `main` branch **must remain clean and stable** at all times. Never push directly to `main`.  
- Create a **new branch** from `main` for each exercise (e.g., `exercise1`, `feature/queen-movement`, etc.).  
- Work on those branches and push your changes there.  
- Once you're done, open a **Pull Request (PR)** from your branch where all the changes where made into `main`.  
- In the PR, review your changes carefully using GitHub’s diff viewer.  
- Make sure all checks pass (e.g., linter, build).  
- After merging the PR into `main`, you can submit the repository link via Moodle.


### Pre-submission Checkup
1. Make sure you've answered all the questions.  
2. Review and refactor your code for better readability (ideally, review your code one or two days later — sometimes it's better to review with fresh eyes).  
3. Ensure that all intended files are uploaded to Git and follow the structure convention outlined in the [Repository Structure](#repository-structure) section.  
4. Ensure that your code is running.  
5. Once you open a PR, review the changes **carefully**. You can leverage GitHub's built-in diff viewer.  
6. Wait and confirm that the linter test completed successfully. If the linter test fails, assess the errors and refactor accordingly — otherwise, each error will negatively impact your grade.  
7. Make sure you followed the [Branching and Pull Request Guidelines](#branching-and-pull-request-guidelines).  
8. Upload the repository link to Moodle.  
9. Good luck :)

<!-- Center Excellenteam image -->
<p align="center">
  <img src="./img/excellenteam.png" alt="Excellenteam">
</p>


# Chess Move Recommender Algorithm

## Overview

The `MoveRecommender` class implements an intelligent chess move recommendation system using a **minimax algorithm with position evaluation**. 
The algorithm analyzes potential moves for the current player and ranks them based on their strategic value,
considering both immediate tactical advantages and future opponent responses.

## Algorithm Architecture

### Core Components

1. **Move Generation**: Systematically generates all legal moves for the current player
2. **Position Evaluation**: Scores each position using multiple chess heuristics
3. **Minimax Search**: Looks ahead to evaluate opponent responses
4. **Priority Queue**: Maintains ranked list of best moves

### Key Features

- **Minimax with Limited Depth**: Recursive lookahead with configurable maximum depth
- **Multi-Factor Position Evaluation**: Considers captures, checks, center control, threats, and piece safety
- **Temporary Move Simulation**: Makes and undoes moves to evaluate positions safely
- **Early Termination**: Alpha-beta style cutoffs for performance optimization

## How the Algorithm Works

### 1. Move Generation Phase (`refreshMoveQueue`)

```
For each square on the board:
    If square contains our piece:
        For each possible destination:
            If move is legal:
                Evaluate move using minimax
                Add to priority queue if score > 0
```

**Time Complexity**: O(64 × 64 × M) where M is the minimax evaluation cost
- 64 source squares × 64 destination squares = 4,096 potential moves per turn
- Most moves are filtered out by chess rules, reducing actual evaluations significantly

### 2. Minimax Evaluation (`minimax`)

The minimax algorithm implements the following recursive structure:

```
minimax(move, depth, isMaximizing):
    currentScore = evaluatePosition(move)
    
    if depth == 0:
        return currentScore
    
    makeTemporaryMove(move)
    bestScore = isMaximizing ? -∞ : +∞
    
    For each opponent response:
        score = minimax(opponentMove, depth-1, !isMaximizing)
        if isMaximizing:
            bestScore = max(bestScore, score)
        else:
            bestScore = min(bestScore, score)
    
    restoreBoard()
    return currentScore + bestScore/depth
```

**Key Optimizations**:
- **Early Cutoff**: Stops searching when finding moves above/below threshold values
- **Depth Scaling**: Divides deeper scores by depth to prefer immediate tactical gains
- **No-Move Detection**: Recognizes checkmate/stalemate conditions

### 3. Position Evaluation (`evaluatePosition`)

The position evaluation function considers multiple strategic factors:

#### Scoring Components

| Factor | Weight | Description |
|--------|--------|-------------|
| **Material Capture** | Piece Value × Multiplier | Points for capturing opponent pieces |
| **Check Bonus** | Fixed Value | Bonus for putting opponent in check |
| **Center Control** | Positional Bonus | Rewards controlling center squares (d4, d5, e4, e5) |
| **King Safety** | Penalty | Discourages unnecessary king moves |
| **Threat Assessment** | Variable Penalty | Penalizes moves that expose pieces to capture |
| **Randomness** | Small Random Value | Adds variation to prevent predictable play |

#### Piece Values (Standard Chess Values)
- **Pawn**: 1 point
- **Knight**: 3 points  
- **Bishop**: 3 points
- **Rook**: 5 points
- **Queen**: 9 points
- **King**: 1000 points (effectively infinite)

#### Center Control Evaluation
- **Inner Center** (d4, d5, e4, e5): Higher bonus
- **Outer Center** (12 surrounding squares): Lower bonus
- Encourages piece development and central control

#### Threat Assessment
The algorithm checks if the moved piece becomes vulnerable:
- **Heavy Penalty**: When attacked by a piece of lower value
- **Standard Penalty**: When attacked by piece of equal/higher value
- **Calculation**: Scans all opponent pieces to see if they can capture the moved piece

## Runtime Complexity Analysis

### Theoretical Complexity

**Overall Time Complexity**: O(B^D × E)

Where:
- **B** = Branching factor (average legal moves per position) ≈ 35 in chess
- **D** = Maximum search depth
- **E** = Position evaluation cost ≈ O(64) for threat analysis

### Detailed Breakdown

#### 1. Move Generation: O(64² × V)
- **64² iterations**: Check all source-destination pairs
- **V**: Move validation cost (piece-specific rules)
- **Practical reduction**: Most combinations are illegal, reducing actual work

#### 2. Minimax Search: O(B^D)
- **Recursive calls**: Each level explores ~35 legal moves
- **Exponential growth**: Depth 1 = 35 calls, Depth 3 = 42,875 calls
- **With optimizations**: Early cutoffs reduce effective branching factor

#### 3. Position Evaluation per Move: O(64)
- **Threat analysis**: Check all 64 squares for attacking pieces
- **Other factors**: O(1) calculations for captures, checks, center control

### Practical Performance

| Max Depth | Estimated Evaluations | Typical Runtime |
|-----------|----------------------|-----------------|
| 1 | ~35 | < 1ms |
| 2 | ~1,000 | 10-50ms |
| 3 | ~35,000 | 0.5-2s |
| 4 | ~1,000,000 | 30-120s |

**Performance Factors**:
- **Position complexity**: Endgames have fewer pieces, reducing branching
- **Early cutoffs**: Alpha-beta style pruning reduces search space
- **Move ordering**: Priority queue ensures best moves are found first

### Memory Complexity: O(D)
- **Stack space**: Recursive calls use O(D) stack frames
- **Board states**: Each level saves/restores one board state
- **Move storage**: Priority queue holds all evaluated moves

## Algorithm Strengths

1. **Tactical Awareness**: Identifies captures, checks, and immediate threats
2. **Strategic Understanding**: Values center control and piece development  
3. **Lookahead Capability**: Considers opponent responses to avoid blunders
4. **Configurable Depth**: Adjustable between fast play and stronger analysis
5. **Robust Evaluation**: Multiple factors prevent one-dimensional play

## Algorithm Limitations

1. **Exponential Complexity**: Deep searches become computationally expensive
2. **Limited Horizon**: Cannot see beyond maximum depth setting
3. **Static Evaluation**: Position scoring based on heuristics, not perfect chess knowledge
4. **No Opening/Endgame Specialization**: Uses same evaluation throughout game phases
5. **Threat Assessment Scope**: Only evaluates immediate piece safety

## Usage Example

```cpp
// Initialize with board and search depth
MoveRecommender recommender(chessBoard, 3);

// Generate and evaluate all moves
recommender.recommendMoves();

// Display best moves in order
recommender.printRecommendations();
```

## Configuration Parameters

- **maxDepth**: Controls search depth (1-4 recommended)
- **ALPHA_BETA_CUTOFF**: Threshold for early termination
- **CAPTURE_MULTIPLIER**: Weight for material gains
- **CHECK_BONUS**: Bonus for checking opponent
- **CENTER_BONUS_INNER/OUTER**: Positional control values
- **THREAT_MULTIPLIER/PENALTY**: Threat assessment weights


---

