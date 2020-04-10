# C-Dungeon-Crawler

A c++ game which load a new random dungeon every time and spawns a character '0' to play as. The goal is to reach the '$' sign. controls are the usual WASD and one can use 'p'-key to attack although there are no enemies or npc's yet.

# Random Dungeon

Rooms are added using a brute force method:-
>make room 
>if new room collides with any previous room, scratch it and go to step one

Recursive Backtracking Algorithm is used to make the pathways:-
>Thanks to https://www.youtube.com/watch?v=elMXlO28Q1U and https://en.wikipedia.org/wiki/Maze_generation_algorithm#Recursive_backtracker
>Implementation for C++ but the random aspect is skipped as the standard output appears random due to the presence of rooms

# Display

>the usual system("cls") approach causes flicker
>a map is loaded, then:-

void setCursorPosition(int x, int y)
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

>using a setCursorPosition() only the the characters where '0' moves are updated, this way no (significant) flicker is apparent

# Bugs

>some rooms are disjointed i.e. not connected to to path
>the path sometimes removes the right bounding wall
>fixes:-
  ->at least the goal room has non-procedural pathways that connected it the path
  ->the bounding walls are add again after the room and path formation
