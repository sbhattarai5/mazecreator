//Name: Saurav Bhattarai
//File: ciss350t01.cpp

//Description
//Builds a random 2D maze 
//Takes input n where n creates a n-by-n maze and r, c where it is a location in the maze
//No error handled for input outside of the box

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

//Class Cell, each cell is an object locating a cell in the maze
//======================================================================================================
class Cell
{
public:
    Cell(int r_, int c_)
    : r(r_), c(c_) 
    {}
    int r, c;
    bool operator==(Cell c0)
    {
        return (c0.r == r && c0.c == c);
    }
};
//======================================================================================================

//PunchedWall class which records the walls that are punched. Consists of two cells
//======================================================================================================
class PunchedWall
{
public:
    PunchedWall(Cell c0_, Cell c1_)
    : c0(c0_), c1(c1_)
    {}
    Cell c0, c1;
};
//======================================================================================================


// isunvisited, returns true if the cell is unvisited, else returns false
//======================================================================================================
bool isunvisited(Cell c0, const std::vector< Cell > & unvisited)
{
    for (int i = 0; i < unvisited.size(); ++i)
    {
        Cell c1 = unvisited[i];
        if (c0 == c1) return true;    
    }
    return false;
}
//======================================================================================================

//======================================================================================================
// delete_from_unvisited
// return-type = none
// parameters = Cell, which needs to be deleted || Unvisited, a vector of Cells which has not been visited yet
// deletes the mentioned cell from the vector unvisited
//======================================================================================================
void delete_from_unvisited(Cell c0, std::vector< Cell > & unvisited)
{
    for (int i = 0; i < unvisited.size(); ++i)
    {
        if (unvisited[i] == c0) 
        {
            unvisited.erase(unvisited.begin() + i);  //delete the i-th element and return
            return;
        }
    }
}
//======================================================================================================


//======================================================================================================
// inpuchedwall
// returns true iff Cell(row0, col0) and Cell(row1, col1) is in the punchedwalls vector
//======================================================================================================
bool inpunchedwall(int row0, int col0, int row1, int col1, const std::vector< PunchedWall > & punchedwalls)
{
    for (int i = 0; i < punchedwalls.size(); ++i)
    {   
        PunchedWall pwall = punchedwalls[i];
        Cell c0 = pwall.c0;
        Cell c1 = pwall.c1;
        if (c0.r == row0 && c0.c == col0 && c1.r == row1 && c1.c == col1) return true;
    }
    return false;
}
//======================================================================================================

//======================================================================================================
// build_maze
// creates a maze of size n-by-n where the maze building starts from the cell Cell(r, c)
//======================================================================================================
std::vector< PunchedWall > build_maze(int n, int r, int c)
{

    //initialization
    //=================================================================================================
    std::vector< PunchedWall > punchedwalls;
    std::vector< Cell > unvisited;
    for (int row = 0; row < n; ++row)
    {
        for (int col = 0; col < n; ++col)
        {
            if (row != r || col != c) unvisited.push_back(Cell(row, col));  // add everything to unvisited except for the starting position
        }
    }
    std::vector< Cell > path;
    Cell c0(r, c);
    path.push_back(c0);
    int path_len = 1;
    //==================================================================================================


    //main loop for maze buling
    //==================================================================================================
    while (path_len != 0) 
    {
        Cell current_cell = path[path_len - 1];
        std::vector< Cell > available_cells;

        Cell east_cell(current_cell.r + 1, current_cell.c);
        Cell north_cell(current_cell.r, current_cell.c - 1);
        Cell west_cell(current_cell.r - 1, current_cell.c);
        Cell south_cell(current_cell.r, current_cell.c + 1);

        if (isunvisited(east_cell, unvisited)) available_cells.push_back(east_cell);
        if (isunvisited(north_cell, unvisited)) available_cells.push_back(north_cell);
        if (isunvisited(west_cell, unvisited)) available_cells.push_back(west_cell);
        if (isunvisited(south_cell, unvisited)) available_cells.push_back(south_cell);
        

        if (available_cells.size() != 0)
        {
            //select a random cell
            int rand_index = rand() % available_cells.size();
            Cell selected_cell = available_cells[rand_index];
            //add it to punched_wall
            punchedwalls.push_back(PunchedWall(current_cell, selected_cell));
            punchedwalls.push_back(PunchedWall(selected_cell, current_cell));
            //delete it from unvisited    
            delete_from_unvisited(selected_cell, unvisited);
            //add the selected cell to path
            path.push_back(selected_cell);
            ++path_len;
        }
        else //back track
        {   
            path.pop_back();
            --path_len;
        }
    }
    //==================================================================================================
    return punchedwalls;

}
//======================================================================================================

//======================================================================================================
//print_maze function
//prints the maze on the basis of the punchedwalls
//======================================================================================================
void print_maze(int n, const std::vector< PunchedWall > & punchedwalls)
{
    //top
    for (int i = 0; i < n; ++i)
    {
        std::cout << "+-";
    }
    std::cout << "+\n";

    //middle
    for (int row = 0; row < n; ++row)
    {
        std::cout << "|";
        for (int col = 0 ; col < n; ++col)
        {
            std::cout << ' ';
            if (inpunchedwall(row, col, row, col + 1, punchedwalls)) std::cout << ' ';
            else std::cout << '|';
        }

        std::cout << std::endl;
        
        //print +- between the column
        std::cout << '+';
        for (int col = 0; col < n; ++col)
        {
            if (inpunchedwall(row, col, row + 1, col, punchedwalls)) std::cout << ' ';
            else std::cout << '-';
            std::cout << '+';
        }
        std::cout << std::endl;
    }

    return;
}
//======================================================================================================

//======================================================================================================
// main
// takes n, r, c as input
// builds and prints the randomly built maze
//======================================================================================================
int main()
{
    srand(time(NULL));
    int n, r, c;
    std::cin >> n >> r >> c;
    std::vector< PunchedWall > punchedwalls = build_maze(n, r, c);
    print_maze(n, punchedwalls);
    return 0;
}
//=======================================================================================================