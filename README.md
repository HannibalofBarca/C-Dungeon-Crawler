\n # C-Dungeon-Crawler
\n 
\n A c++ game which load a new random dungeon every time and spawns a character '0' to play as. The goal is to reach the '$' sign. controls are the usual WASD and one can use 'p'-key to attack although there are no enemies or npc's yet.
\n 
\n # Random Dungeon
\n 
\n Rooms are added using a brute force method:-
\n >make room 
\n >if new room collides with any previous room, scratch it and go to step one
\n 
\n Recursive Backtracking Algorithm is used to make the pathways:-
\n >Thanks to https://www.youtube.com/watch?v=elMXlO28Q1U and https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_backtracker
\n >Implementation for C++ but the random aspect is skipped as the standard output appears random due to the presence of rooms
\n 
\n # Display
\n 
\n >the usual system("cls") approach causes flicker
\n >a map is loaded, then:-
\n 
\n void setCursorPosition(int x, int y)
\n {
\n     COORD coord = { (SHORT)x, (SHORT)y };
\n     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
\n }
\n 
\n >using a setCursorPosition() only the the characters where '0' moves are updated, this way no (significant) flicker is apparent
\n 
\n # Bugs
\n 
\n >some rooms are disjointed i.e. not connected to to path
\n >the path sometimes removes the right bounding wall
\n >fixes:-
\n   ->at least the goal room has non-procedural pathways that connected it the path
\n   ->the bounding walls are add again after the room and path formation
