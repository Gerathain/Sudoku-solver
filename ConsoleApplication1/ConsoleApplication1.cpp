// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

//2d array of sets
typedef std::vector< std::vector< std::set<int> > > Grid;
const int gridSize = 9;
const int subsize = 3;

void initGrid( Grid& grid );
void readGrid( Grid& grid );
bool loop( Grid grid );
void printGrid( const Grid &grid );

bool operator< ( std::set<int> a, std::set<int> b )
{
    return a.size() < b.size();
}

int main()
{
    Grid grid( gridSize );

    for( int i = 0; i < gridSize; i++ )
    {
        std::set<int> s;
        for( int j = 1; j <= gridSize; j++ )
        {
            s.insert( j );
        }
        grid[ i ] = std::vector<std::set<int> >( gridSize, s );
    }

    initGrid( grid );
    // readGrid( grid );

    loop( grid );
}

/**
* Takes the grid, adds the known values and then populates everything else with the
* set of every number that is possible
*/
void initGrid( Grid& grid )
{
    // This should be changed to be much easier to edit. I think that it should
    // be able to read the grig in from a csv in a file or something.

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

void readGrid( Grid& grid )
{
    std::fstream fin;

    fin.open( "sudoku.csv", std::ios::in );

    std::string line, word, temp;

    int x = 0, y = 0, i = 0;

    while( fin >> temp )
    {
        getline( fin, line );
        std::stringstream s( line );

        while( getline( s, word, ',' ) )
        {
            int square = stoi( word );
            std::set<int> possibilities;

            if( square == 0 )
            {
                i = 0;
                std::generate_n( std::inserter( possibilities, possibilities.begin() ), gridSize, [&i]() { return i++; } );
            }
            else
            {
                possibilities = { square };
            }

            grid[ x ][ y ] = possibilities;
            x++;
        }
        y++;
    }

    return;
}

/**
* iterates all of the grid squares and removes any options that are not allowed in sudoku
* returns false if the grid as become impossible
*/
bool concreteGrid( Grid &grid )
{
    for( int x = 0; x < gridSize; x++ )
    {
        for( int y = 0; y < gridSize; y++ )
        {
            //remove impossibilities from the other squares
            if( grid[ x ][ y ].size() == 1 )
            {
                //This one has been decided, update the ones that it affects
                //row
                for( int i = 0; i < gridSize; i++ )
                {
                    if( i == x ) { continue; } // don't updated the square that we are looking at

                    grid[ i ][ y ].erase( *(grid[ x ][ y ].begin()) );

                    if( grid[ i ][ y ].size() == 0 ) { return false; }
                }

                //column
                for( int i = 0; i < gridSize; i++ )
                {
                    if( i == y ) { continue; } // don't updated the square that we are looking at

                    grid[ x ][ i ].erase( *(grid[ x ][ y ].begin()) );

                    if( grid[ x ][ i ].size() == 0 ) { return false; }
                }

                int xOffset = x / subsize;
                int yOffset = y / subsize;

                // Local grid
                for( int localX = 0; localX < subsize; localX++ )
                {
                    for( int localY = 0; localY < subsize; localY++ )
                    {
                        if( x == (localX + subsize * xOffset) && y == (localY + subsize * yOffset) ) { continue; }

                        grid[ localX + subsize * xOffset ][ localY + subsize * yOffset ].erase( *(grid[ x ][ y ].begin()) );
                        if( grid[ localX + subsize * xOffset ][ localY + subsize * yOffset ].size() == 0 ) { return false; }
                    }
                }
            }
        }
    }
    return true;
}
/**
* Make the decisions if the grid is undecidable
* return true if a solution has been found
*/
bool loop( Grid grid )
{
    bool legalGrid = concreteGrid( grid );
    if( !legalGrid )
    {
        return false;
    }

    unsigned int minSize = 10;
    unsigned int minX, minY = -1;
    for( int x = 0; x < gridSize; x++ )
    {
        for( int y = 0; y < gridSize; y++ )
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
    if( minY == -1 )
    {
        printGrid( grid );
        return true;
    }

    std::set<int> x = grid[ minX ][ minY ];
    for( int possibility : x )
    {
        grid[ minX ][ minY ] = std::set<int>{ possibility };

        if( loop( grid ) )
        {
            // A solution was found
            return true;
        }
    }
    return false;
}

void printGrid( const Grid& grid )
{
    for( int y = 0; y < gridSize; y++ )
    {
        if( y % subsize == 0 )
        {
            for( int x = 0; x < gridSize; x++ )
            {
                if( x % subsize == 0 )
                {
                    std::cout << "+";
                }
                std::cout << "--";
            }
            std::cout << std::endl;
        }
        for( int x = 0; x < gridSize; x++ )
        {

            if( x % subsize == 0 )
            {
                std::cout << "|";
            }

            for( int choice : grid[ x ][ y ] )
            {
                std::cout << choice << ' ';
            }
        }
        std::cout << std::endl;
    }
}
