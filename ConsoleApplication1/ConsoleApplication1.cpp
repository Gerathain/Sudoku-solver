﻿// ConsoleApplication1.cpp : Defines the entry point for the console application.
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

int main()
{
    std::vector<std::set<int> > row( size, std::set<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    Grid grid( size, row );

    initGrid( grid );

    loop( grid );
}

/**
* Takes the grid, adds the known values and then populates everything else with the
* set of every number that is possible
*/
void initGrid( Grid grid )
{
    grid[ 0 ][ 0 ] = std::set<int>{ 5 };
    return;
}

/**
* iterates all of the grid squares and removes any options that are not allowed in sudoku
* returns true if the grid as become impossible
*/
bool concreteGrid( Grid grid )
{
    return false;
}

void loop( Grid grid )
{
    bool impossible = concreteGrid( grid );
    if( impossible )
    {
        return;
    }


    // Find a square with the least options that isn't decided
    /* for i in square possiblities*/
    // Make a choice for the square
    loop( grid );
}
