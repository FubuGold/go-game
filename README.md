# go-game

Version 0.1

## File tree

```bash
go-game
 ┣ 📂.vscode
 ┃ ┣ 📜settings.json
 ┃ ┗ 📜tasks.json
 ┣ 📂data
 ┃ ┗ 📜config.json
 ┣ 📂include
 ┃ ┣ 📜board.h
 ┃ ┣ 📜game_logic.h
 ┃ ┣ 📜game_scoring.h
 ┃ ┣ 📜test.h
 ┃ ┗ 📜zobrist_hash.h
 ┣ 📂lib
 ┃ ┗ 📜json.hpp
 ┣ 📂src
 ┃ ┣ 📜board.cpp
 ┃ ┣ 📜game_logic.cpp
 ┃ ┣ 📜game_scoring.cpp
 ┃ ┣ 📜main.cpp
 ┃ ┣ 📜test.cpp
 ┃ ┗ 📜zobrist_hash.cpp
 ┣ 📜.gitignore
 ┣ 📜Makefile
 ┗ 📜README.md
```

## Tech stack
Language: C++
Graphic: SFML
AI:
- Random - No judgement 
- Minimax - Shallow 
- Minimax + Alpha-Beta Pruning

## Extensions
file-tree-generator