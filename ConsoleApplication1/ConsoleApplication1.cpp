// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <set>
#include <vector>
#include <iostream>

//2d array of sets
typedef std::vector< std::vector< std::set<int> > > Grid;
const int size = 9;
const int subsize = 3;

void initGrid( Grid &grid );
bool loop( Grid &grid );

int main()
{
    Grid grid( size );
    std::vector<std::set<int> > row( size, std::set<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    for( int i = 0; i < size; i++ )
    {
        grid[ i ] = std::vector<std::set<int> >( size, std::set<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    }

    initGrid( grid );

    loop( grid );

    for( int x = 0; x < size; x++ )
    {
        for( int y = 0; y < size; y++ )
        {
            for( int choice : grid[ x ][ y ] )
            {
                std::cout << choice << ' ';
            }
            std::cout << std::endl;
        }
    }
}

/**
* Takes the grid, adds the known values and then populates everything else with the
* set of every number that is possible
*/
void initGrid( Grid &grid )
{
    // Assign sets of one value to the square s that the puzzle start with
    grid[ 0 ][ 0 ] = std::set<int>{ 5 };
    grid[ 1 ][ 0 ] = std::set<int>{ 3 };
    grid[ 4 ][ 0 ] = std::set<int>{ 7 };
    grid[ 0 ][ 1 ] = std::set<int>{ 6 };
    grid[ 3 ][ 1 ] = std::set<int>{ 1 };
    grid[ 4 ][ 1 ] = std::set<int>{ 9 };
    grid[ 5 ][ 1 ] = std::set<int>{ 5 };
    grid[ 1 ][ 2 ] = std::set<int>{ 9 };
    grid[ 2 ][ 2 ] = std::set<int>{ 8 };
    grid[ 7 ][ 2 ] = std::set<int>{ 6 };
    grid[ 0 ][ 3 ] = std::set<int>{ 8 };
    grid[ 4 ][ 3 ] = std::set<int>{ 6 };
    grid[ 8 ][ 3 ] = std::set<int>{ 3 };
    grid[ 0 ][ 4 ] = std::set<int>{ 4 };
    grid[ 3 ][ 4 ] = std::set<int>{ 8 };
    grid[ 5 ][ 4 ] = std::set<int>{ 3 };
    grid[ 8 ][ 4 ] = std::set<int>{ 1 };
    grid[ 0 ][ 5 ] = std::set<int>{ 7 };
    grid[ 4 ][ 5 ] = std::set<int>{ 2 };
    grid[ 8 ][ 5 ] = std::set<int>{ 6 };
    grid[ 1 ][ 6 ] = std::set<int>{ 6 };
    grid[ 6 ][ 6 ] = std::set<int>{ 2 };
    grid[ 7 ][ 6 ] = std::set<int>{ 8 };
    grid[ 3 ][ 7 ] = std::set<int>{ 4 };
    grid[ 4 ][ 7 ] = std::set<int>{ 1 };
    grid[ 5 ][ 7 ] = std::set<int>{ 9 };
    grid[ 8 ][ 7 ] = std::set<int>{ 5 };
    grid[ 4 ][ 8 ] = std::set<int>{ 8 };
    grid[ 7 ][ 8 ] = std::set<int>{ 7 };
    grid[ 8 ][ 8 ] = std::set<int>{ 9 };

    return;
}

/**
* iterates all of the grid squares and removes any options that are not allowed in sudoku
* returns true if the grid as become impossible
*/
bool concreteGrid( Grid &grid )
{
    for( int x = 0; x < size; x++ )
    {
        for( int y = 0; y < size; y++ )
        {
            //remove impossibilities from the other squares
            if( grid[ x ][ y ].size() == 1 )
            {
                //This one has been decided, update the ones that it affects
                //row
                for( int i = 0; i < size; i++ )
                {
                    if( i == x ) { continue; } // don't updated the square that we are looking at

                    grid[ i ][ y ].erase( grid[ x ][ y ].begin() );

                    if( grid[ i ][ y ].size() == 0 ) { return false; }
                }

                //column
                for( int i = 0; i < size; i++ )
                {
                    if( i == y ) { continue; } // don't updated the square that we are looking at

                    grid[ x ][ i ].erase( grid[ x ][ y ].begin() );

                    if( grid[ x ][ i ].size() == 0 ) { return false; }
                }

                int xOffset = x / 3;
                int yOffset = y / 3;

                // Local grid
                for( int localX = 0; localX < 3; localX++ )
                {
                    for( int localY = 0; localY < 3; localY++ )
                    {
                        if( x == (localX + 3 * xOffset) && y == (localY + 3 * yOffset) ) { continue; }

                        grid[ localX + 3 * xOffset ][ localY + 3 * yOffset ].erase( grid[ x ][ y ].begin() );
                        if( grid[ localX + 3 * xOffset ][ localY + 3 * yOffset ].size() == 0 ) { return false; }
                    }
                }
            }
        }
    }
    return false;
}
/**
* Make the decisions if the grid is undecidable
* return true if a solution has been found
*/
bool loop( Grid &grid )
{
    bool impossible = concreteGrid( grid );
    if( impossible )
    {
        return false;
    }

    int minSize = 10;
    int minX, minY = -1;
    for( int x = 0; x < size; x++ )
    {
        for( int y = 0; y < size; y++ )
        {
            // Find a square with the least options that isn't decided and make a choice for it
            if( grid[ x ][ y ].size() == 1 ) { continue; }

            if( grid[ x ][ y ].size() < minSize )
            {
                minSize = grid[ x ][ y ].size();
                minX = x;
                minY = y;
            }
        }
    }

    // all of the squares in the grid have a size of one are are allowed to by the rules of the game
    if( minX == -1 )
    {
        return true;
    }

    for( int possibility : grid[ minX ][ minY ] )
    {
        grid[ minX ][ minY ] = std::set<int>{ possibility };

        if( loop( grid ) )
        {
            // A solution was found
            return true;
        }
    }
}
