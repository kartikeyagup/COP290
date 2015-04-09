#include "AI.h" 




void UpdateAIBoard(Board &board)
{
	// UpdatePlayerAI(board);
	// UpdateAlien(board);
	// if (board.GetVectorBullets().size()>0)
	UpdateMissile(board);
}

void UpdatePlayerAI(Board &board)
{
	std::vector<Bullet> CurrentBullets = board.GetVectorBullets();
	std::vector<Ship> CurrentShips = board.GetVectorShips();
	std::vector<Alien> CurrentAliens = board.GetVectorAliens();
	WhatItShouldDo action;
	bool nearEnough;
	int nearestAlien;
	for (int i=0;i<CurrentShips.size();i++)
	{
		if (CurrentShips[i].IfAIControl())
		{
			
			std::pair<int,bool> NearestAlienPair = FindNearestAlien(CurrentShips[i],CurrentAliens,nearEnough);
			
			nearEnough = NearestAlienPair.first;
			nearestAlien= NearestAlienPair.second;
			action = DecideActionForShip(board,CurrentShips[i],nearEnough);
			switch (action)
			{
				case Move:
					MoveShipInDirectionOfAlien(CurrentShips[i],CurrentAliens[nearestAlien]);
					break;
				case Turn:
					TurnShipInDirectionOfAlien(CurrentShips[i],CurrentAliens[nearestAlien]);
					break;
				case FireBullet:
					FireBulletForShip(CurrentShips[i],board);
					break;
				case FireMissile:
					FireMissileForShip(CurrentShips[i],board);
					break;

			}
				
		}
	}
	board.SetVectorShips(CurrentShips); 	//actual updation
}

void UpdateAlien(Board &board)
{
	std::vector<Bullet> CurrentBullets = board.GetVectorBullets();
	std::vector<Ship> CurrentShips = board.GetVectorShips();
	std::vector<Alien> CurrentAliens = board.GetVectorAliens();
	WhatItShouldDo action;
	bool nearEnough;
	int nearestShip;
	for (int i=0; i<CurrentAliens.size(); i++)
	{

	std::pair<int, bool> NearestShipPair= FindNearestShip(CurrentAliens[i],CurrentShips,nearEnough);
	MoveAlienInDirectionOfShip(CurrentAliens[i],CurrentShips[nearestShip]);
}

	// for (int i=0; i<CurrentAliens.size(); i++)
	// {
	// 	std::pair<int, bool> NearestShipPair= FindNearestShip(CurrentAliens[i],CurrentShips,nearEnough);
	// 	nearEnough = NearestShipPair.first;
	// 	nearestShip = NearestShipPair.second;
	// 	action = DecideActionForAlien(board,CurrentAliens[i],nearEnough);
	// 	std::cout<<"Nearest ship for Alien "<<i<<" is Ship "<<nearestShip<<"\n";
		
	// 	switch (action)
	// 		{
	// 			case Move:
	// 				MoveAlienInDirectionOfShip(CurrentAliens[i],CurrentShips[nearestShip]);
	// 				break;
	// 			case Turn:
	// 				TurnAlienInDirectionOfShip(CurrentAliens[i],CurrentShips[nearestShip]);
	// 				break;
	// 			case FireBullet:
	// 				FireBulletForAlien(CurrentAliens[i],board);
	// 				break;
	// 			case FireMissile:
	// 				FireMissileForAlien(CurrentAliens[i],board);
	// 				break;
	// 		}
	// }
	board.SetVectorAliens(CurrentAliens); //actual updation
}

void UpdateMissile(Board &board)
{
	// std::cout<<"Update Missile called\n";
	UpdateAlienMissile(board);
	UpdateShipMissile(board);
	std::vector<Bullet> VectorBullets=board.GetVectorBullets();
			
	for (int i=0;i<VectorBullets.size();i++)
	{
		if(VectorBullets.at(i).GetTypeAI() == true)
		{
			VectorBullets.at(i).SetXPos(VectorBullets.at(i).GetXPos()+VectorBullets.at(i).GetVelX());
			VectorBullets.at(i).SetYPos(VectorBullets.at(i).GetYPos()+VectorBullets.at(i).GetVelY());
		}
	}
	board.SetVectorBullets(VectorBullets); //actual updation
}