#include "Board.h"
#include <cmath>
#include <iostream>

Board::Board()
{
	dimension_x=1000.0;
	dimension_y=1000.0;
	dimension_z=1000.0;
	number_balls=0;
}

bool CheckCorrect(std::vector<Ball> v,Ball n1)
{
	bool sofar=true;
	for (int i=0; (i< v.size()) && sofar; i++)
	{
		double dx=v[i].GetX()-n1.GetX();
		double dy=v[i].GetY()-n1.GetY();
		double dz=v[i].GetZ()-n1.GetZ();
		if (n1.GetRadius()+v[i].GetRadius()>=sqrt(dx*dx + dy*dy +dz*dz))
		{
			sofar=false;
		}
	}
	return sofar;
}

Board::Board(double x,double y, double z, int n)
{
	// Constructs a new Board
	// Default number of balls is 0
	// Default vector of balls is empty
	dimension_x=x;
	dimension_y=y;
	dimension_z=z;
	number_balls=n;
	for (int i=0; i<n;i++)
	{
		Ball newball=Ball(x,y,z,1);
		while (!CheckCorrect(vector_of_balls,newball))
		{
			std::cout <<"In here for: " <<i<<"\n";
			newball=Ball(x,y,z,1);	
		}
		vector_of_balls.push_back(newball);
	}
}

double Board::GetDimensionX()
{
	// Returns the x dimension of the board
	return dimension_x;
}

double Board::GetDimensionY()
{
	// Returns the y dimension of the board
	return dimension_y;
}

double Board::GetDimensionZ()
{
	// Returns the z dimension of the board
	return dimension_z;
}

int Board::GetNumberBalls()
{
	// Returns the number of balls on the board
	return number_balls;
}

vector<Ball> Board::GetVectorBalls()
{
	// Returns the vector of all the balls on the board
	return vector_of_balls;
}

Ball Board::GetBallFromId(int position)
{
	// Returns the position id ball from the vector of balls if the position id is less than the number of balls on the board
	// TODO: Add Exception
	// if (position < number_balls)
		return vector_of_balls[position];
	// else
		// return Ball(1.0,1.0);
}

string Board::GetBoardInformation()
{
	string BoardInfo="Board\n";
	BoardInfo += "Dimension x:"+to_string(dimension_x)+"  Dimension y:" +to_string(dimension_y)+"  Dimension z:" +to_string(dimension_z)+ "  Number of Balls:" + to_string(number_balls)+"\n";
	for (int i=0;i<number_balls;i++)
	{
		BoardInfo+="Ball Number: "+to_string(i)+vector_of_balls[i].GetBallInformation() +"\n";
	}
	BoardInfo+="\n";
	return BoardInfo;
}


void Board::SetDimensionX(double x)
{
	// Updates the x dimension of the board
	dimension_x=x;
}

void Board::SetDimensionY(double y)
{
	// Updates teh y dimension of the board
	dimension_y=y;
}

void Board::SetDimensionZ(double z)
{
	// Updates the z dimension of the board
	dimension_z=z;
}

void Board::SetNumberOfBalls(int number)
{
	// Updates the number of balls on the board
	// CAUTION: This might violate that length of vector == number_balls
	number_balls=number;
}

void Board::SetVectorBalls(vector<Ball> newvector)
{
	// Updates the vector of balls to the new vector and changes the number of balls variable as well
	number_balls=newvector.size();
	vector_of_balls=newvector;
}

void Board::SetBallFromId(int id, Ball newBall)
{
	// Updates the ball at position id if id < number of balls with the new ball
	// TODO: Add exception
	if (id<number_balls)
	{
		vector_of_balls[id]=newBall;
	}
}

void Board::AddBallToBoard(Ball newBall)
{
	// Adds a new ball to the board and increments the nunmber of balls counter
	number_balls += 1;
	vector_of_balls.push_back(newBall);
}

void Board::RemoveBallFromBoard()
{
	// Removes the last ball from the vector of balls and decrements the counter
	// TODO: Add Exception
	if (number_balls>0)
	{
		number_balls -=1;
		vector_of_balls.pop_back();
	}
}

void Board::UpdateBoard(double time_elapsed)
{
	// Updates the situation of the board after a time time_elapsed has passed using the updae ball function
	for (int i=0; i<number_balls; i++)
	{
		vector_of_balls[i].UpdateBall(time_elapsed);
		if (vector_of_balls[i].GetX() > dimension_x)
		{
			vector_of_balls[i].SetX(vector_of_balls[i].GetX()-dimension_x);
		}
		if (vector_of_balls[i].GetY() > dimension_y)
		{
			vector_of_balls[i].SetY(vector_of_balls[i].GetY()-dimension_y);
		}
		if (vector_of_balls[i].GetZ() > dimension_z)
		{
			vector_of_balls[i].SetZ(vector_of_balls[i].GetZ()-dimension_z);
		}
	}
}