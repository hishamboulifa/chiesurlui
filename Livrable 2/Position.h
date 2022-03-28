

#pragma once

#include <iostream>
#include <utility>
#include <cppitertools/range.hpp>

struct Position {
	int i; 
	int j; 

	bool operator==(const Position& autre) const
	{
		return (i == autre.i && j == autre.j);
	}
};

struct PositionHasher
{
	std::size_t operator()(const Position& pos) const
	{
		return std::hash<int>()(pos.i) ^ std::hash<int>()(pos.j);
	}
};
