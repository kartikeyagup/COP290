#include "Board.h"


#define toDigit(c) (c-'0')

//to convert string to 4 letters
std::string ToFour(std::string x)
{
	if(x.length() == 0) 
		return ("0000"); 
	else if(x.length() == 1) 
		return (("000") + x);
	else if(x.length() == 2) 
		return (("00") + x);
	else if(x.length() == 3) 
		return (("0") + x);
	else  
		return x;
}

//compare pairs for sorting
bool PairCompare(std::pair<int,std::string> x, std::pair<int,std::string> y)
{
	return (x.first > y.first);
}

//compare int for sorting
bool MyFunction (int i,int j) 
{ 
	return (i<j); 
}

//to generate random floating point bw a and b 
float RandomFloat(float a, float b) 
{
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

//convert bool to int
int ConvertBool(bool a)
{
	if(a == true)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//convert int to bool
bool ExtractBool(int i)
{
	if(i == 0)
	{
		return false;
	}
	else 
	{
		return true;
	}
}

//constructor
Board::Board()
{

}

//taking in dimensions in constructor
Board::Board(double posx,double negx,double posy,double negy)
{
	DimensionPosX = posx;
	DimensionPosY = posy;
	DimensionNegX = negx;
	DimensionNegY = negy;
} 


//set functions
void Board::SetPosXDimension(double posx)
{
	DimensionPosX = posx;
}

void Board::SetPosYDimension(double posy)
{
	DimensionPosY = posy;
}

void Board::SetNegXDimension(double negx)
{
	DimensionNegX = negx;
}

void Board::SetNegYDimension(double negy)
{
	DimensionNegY = negy;
}

void Board::SetVectorBullets(std::vector<Bullet> bullet_vector)
{
	VectorBullets = bullet_vector;
}
void Board::SetVectorShips(std::vector<Ship> ship_vector)
{
	VectorShips = ship_vector;
}

void Board::SetVectorAliens(std::vector<Alien> alien_vector)
{
	VectorAliens = alien_vector;
}

void Board::SetNthBullet(int id,Bullet set_bullet)
{
	VectorBullets.at(id) = set_bullet;
}

void Board::SetNthAlien(int id, Alien set_alien)
{
	VectorAliens.at(id) = set_alien;
}

void Board::SetNthShip(int id, Ship set_ship)
{
	VectorShips.at(id) = set_ship;
}


//getting functions
std::vector<Bullet> Board::GetVectorBullets()
{
	return VectorBullets;
}

std::vector<Ship> Board::GetVectorShips()
{
	return VectorShips;
}

std::vector<Alien> Board::GetVectorAliens()
{
	return VectorAliens;
}

double Board::GetPosXDimension()
{
	return DimensionPosX;
}

double Board::GetPosYDimension()
{
	return DimensionPosY;
}

double Board::GetNegXDimension()
{
	return DimensionNegX;
}
double Board::GetNegYDimension()
{
	return DimensionNegY;
}

int Board::GetNumberShips()
{
	return VectorShips.size();
}

int Board::GetNumberAliens()
{
	return VectorAliens.size();
}

int Board::GetNumberBullets()
{
	return VectorBullets.size();
}

Bullet Board::GetNthBullet(int id)
{
	return VectorBullets.at(id);
}
		
Alien Board::GetNthAlien(int id)
{
	return VectorAliens.at(id);
}

Ship Board::GetNthShip(int id)
{
	return VectorShips.at(id);
}

void Board::RemoveNthAlien(int id)
{
	VectorAliens.erase(VectorAliens.begin() + id);
}

void Board::RemoveNthBullet(int id)
{
	VectorBullets.erase(VectorBullets.begin() + id);
}

void Board::RemoveNthShip(int id)
{
	VectorShips.erase(VectorShips.begin() + id);
}

//to get maximum distance
double Board::MaxDistance()
{
	return pow(DimensionPosX+DimensionNegX,2) + pow (DimensionPosY+DimensionNegY,2);
}


//returns -1 if bullets hits no alien and returns alien pos in vector aliens
int Board::CheckBulletHitAlien(int bullet_id)
{
	Bullet bullet_hitting = VectorBullets.at(bullet_id);
	if(bullet_hitting.GetTypePlayer() == true)
	{
		for (int i = 0;i < VectorAliens.size();i++)
		{
			Alien alien_hit = VectorAliens.at(i);
			float xdis = bullet_hitting.GetXPos() - alien_hit.GetXPos();
			float ydis = bullet_hitting.GetYPos() - alien_hit.GetYPos();
			if ((float) sqrt(xdis*xdis + ydis*ydis) < 50)
			{
				return i;
			}
		}
	}
	return -1;

}

//returns ship id of the bullet id hitting a ship
int Board::CheckBulletHitShip(int id)
{
	Bullet bullet_hitting = VectorBullets.at(id);
	if(bullet_hitting.GetTypePlayer() == false)
	{
		for (int i = 0;i < VectorShips.size();i++)
		{
			float xdis = bullet_hitting.GetXPos() - VectorShips.at(i).GetXPos();
			float ydis = bullet_hitting.GetYPos() - VectorShips.at(i).GetYPos();
			if ((float) sqrt(xdis*xdis + ydis*ydis) < 50)
			{
				return i;
			}
		}
	}
	return -1;
}


//takes in ship id to give vector of positions of aliens hitting ship
std::vector<int> Board::CheckAlienHitShip(int shipid)
{
	std::vector<int> answer;
	for(int i = 0;i < VectorAliens.size();i++)
	{
		float xdis = VectorShips.at(shipid).GetXPos() - VectorAliens.at(i).GetXPos();
		float ydis = VectorShips.at(shipid).GetYPos() - VectorAliens.at(i).GetYPos();
		if ((float) sqrt(xdis*xdis + ydis*ydis) < 50)
		{
			answer.push_back(i);
		}
	}
	return answer;
}

//upadate the position of all bullets
std::vector<Points> Board::UpdateAllBullets()
{
	std::vector<Points> ship_pos_vec; 		//returns the points of ship undergoing bullet or ship collision
	int bullet_size = VectorBullets.size();
	std::vector<int> bullets_delete;		//vector of bullet numbers in vector bullets to be deleted
	std::vector<int> ships_lives_reduce;	//vector of ship positions of lives to reduce
	std::vector<int> aliens_delete;			//returns the alien positions in alien vector of aliens to be deleted 

	for (int i = 0;i < bullet_size ;i++)
	{
		if(VectorBullets.at(i).GetTypePlayer() == false)//check bullet hit ship
		{
			int hit_ship = CheckBulletHitShip(i);		//ship id of ship being hit
			if(hit_ship > -1)
			{
				if(VectorShips.at(hit_ship).GetLives() > 0)
				{	
					ships_lives_reduce.push_back(hit_ship);
					Points ship_pos;
					ship_pos.x = VectorShips.at(hit_ship).GetXPos();
					ship_pos.y = VectorShips.at(hit_ship).GetYPos();
					ship_pos.z = 0.0;
					ship_pos_vec.push_back(ship_pos);
					bullets_delete.push_back(i);
				}
			}
		}
		else 
		{
			bool repeat = false;					//check bullet hit alien
			int hit_alien = CheckBulletHitAlien(i);
			if(hit_alien > -1)
			{
				for (int i = 0;i<aliens_delete.size();i++)
				{
					if(hit_alien == aliens_delete.at(i))
					{
						repeat = true;
						break;
					}	
				}
				if(repeat == false)					//to avoid repetition of alien to be deleted
				{	
					int ship_score_to_inc = VectorBullets.at(i).GetShipId();
					VectorShips.at(ship_score_to_inc).SetScore(VectorShips.at(ship_score_to_inc).GetScore() + 5);
					aliens_delete.push_back((hit_alien));
					//Points alien_del_pos;
					// alien_del_pos.x = VectorAliens.at(hit_alien).GetXPos();
					// alien_del_pos.y = VectorAliens.at(hit_alien).GetYPos();
					// alien_del_pos.z = 0.0;
					// aliens_pos.push_back(alien_del_pos);
				}
				bullets_delete.push_back(i);
			}	
		}
	}

	for(int i = 0;i < VectorShips.size();i++)		//checking alien hit ship or not 
	{
		if(VectorShips.at(i).GetLives() != 0)
		{
			std::vector<int> alientodel = CheckAlienHitShip(i);
			if(alientodel.size() != 0)
			{ 
				ships_lives_reduce.push_back(i);	//getting ship positions of the ones being hit
				Points ship_pos;
				ship_pos.x = VectorShips.at(i).GetXPos();
				ship_pos.y = VectorShips.at(i).GetYPos();
				ship_pos.z = 0.0;
				ship_pos_vec.push_back(ship_pos);
				for(int j = 0;j<alientodel.size();j++)
				{
					bool repeat  = false;
					for (int i = 0;i<aliens_delete.size();i++)
					{
						if(alientodel.at(j) == aliens_delete.at(i))
						{
							repeat = true;
							break;
						}
					}
					if(!repeat)
					{
						// Points newpoint;
						// newpoint.x = VectorAliens.at(alientodel.at(j)).GetXPos();
						// newpoint.y = VectorAliens.at(alientodel.at(j)).GetYPos();
						// newpoint.z = 0.0;
						// aliens_pos.push_back(newpoint);
						aliens_delete.push_back(alientodel.at(j));	//aliens to be deleted due to collision with the ships
					}
				}
			} 
		}
	}
	int bullets_delete_size  = bullets_delete.size();
	int ships_lives_reduce_size = ships_lives_reduce.size();
	int aliens_delete_size = aliens_delete.size();
	std::sort (aliens_delete.begin(), aliens_delete.end(), MyFunction);
	for (int i = bullets_delete_size - 1;i >= 0;i--)						//deleting bullets from the ships
	{
		VectorBullets.erase(VectorBullets.begin() + bullets_delete.at(i));
	} 
	for(int i = ships_lives_reduce_size - 1;i>=0;i--) 						//reducing lives of the ship
	{
		if(VectorShips.at(ships_lives_reduce.at(i)).GetLives() > 0)
		{
			VectorShips.at(ships_lives_reduce.at(i)).SetLives(VectorShips.at(ships_lives_reduce.at(i)).GetLives()-1);
		}
	}
	for (int i = aliens_delete_size-1;i>=0;i--)								//deleting aliens hit by bullets and ship
	{
		VectorAliens.erase(VectorAliens.begin()+aliens_delete.at(i));
	}
	for (int i=VectorBullets.size()-1;i>=0;i--)								//checking if bullets went out of board
	{
		if(VectorBullets.at(i).GetXPos()+2.0 > DimensionPosX 
		|| VectorBullets.at(i).GetXPos()-2.0 < -(DimensionNegX) 
		|| VectorBullets.at(i).GetYPos()+2.0 > DimensionPosY
		|| VectorBullets.at(i).GetYPos()-2.0 < -(DimensionNegY))
		{
			VectorBullets.erase(VectorBullets.begin() + i);
		}
	}
	for (int i=VectorBullets.size() - 1;i >= 0;i--)							//finally updating bullets positions
	{
		if(VectorBullets.at(i).GetTypeAI() == false)
		{
			VectorBullets.at(i).SetXPos(VectorBullets.at(i).GetXPos()+VectorBullets.at(i).GetVelX());
			VectorBullets.at(i).SetYPos(VectorBullets.at(i).GetYPos()+VectorBullets.at(i).GetVelY());
		}
		else 
		{
			if(time(0) - VectorBullets.at(i).GetTimeCreated() > 5) 
			{
				VectorBullets.erase(VectorBullets.begin() + i);
			}
		}
	}
	return ship_pos_vec;
}

void Board::UpdateAliens()
{
	//not required till now
}

void Board::UpdatePlayerAI()
{
	//not required till now
}

//adding bullets ships and aliens in their respective functions
void Board::InsertBullet(Bullet new_bullet)
{
	VectorBullets.push_back(new_bullet);
}

void Board::InsertAlien(Alien new_alien)
{
	VectorAliens.push_back(new_alien);
}

void Board::InsertShip(Ship new_ship)
{
	VectorShips.push_back(new_ship);
}


//get all the info of bullets ships and aliens in the form of string for debugging purposes
std::string Board::GetSummary()
{
	std::string get_summary_ship = "";
	std::string get_summary_bullet = "";
	std::string get_summary_alien = "";

	for (int i = 0;i < VectorShips.size();i++)
	{
		get_summary_ship  = get_summary_ship + VectorShips.at(i).GetSummary() + "\t";
	}
	for (int i = 0;i < VectorBullets.size();i++)
	{
		get_summary_ship  = get_summary_bullet + VectorBullets.at(i).GetSummary() + "\t";
	}
	for (int i = 0;i < VectorAliens.size();i++)
	{
		if (i == VectorAliens.size()-1)
		{
			get_summary_ship  = get_summary_bullet + VectorAliens.at(i).GetSummary();
		}
		else
		{
			get_summary_ship  = get_summary_bullet + VectorAliens.at(i).GetSummary() + "\t";	
		}
	}
	return (get_summary_ship + get_summary_bullet + get_summary_alien);
}


// to move ones own ship
void Board::MoveNthShip(int ship_id,int mov_type)
{
	//move types mapped according to different buttons
	Ship ship_to_move = VectorShips.at(ship_id);
	if(mov_type == 0)
	{
		ship_to_move.SetXPos(ship_to_move.GetXPos() - 10.0*cos((double) ship_to_move.GetAngle()*PI/180));
		ship_to_move.SetYPos(ship_to_move.GetYPos() - 10.0*sin((double) ship_to_move.GetAngle()*PI/180));			
	}
	else if(mov_type == 1)
	{
		ship_to_move.SetXPos(ship_to_move.GetXPos() + 10.0*cos((double) ship_to_move.GetAngle()*PI/180));
		ship_to_move.SetYPos(ship_to_move.GetYPos() + 10.0*sin((double) ship_to_move.GetAngle()*PI/180));
	}
	else if(mov_type == 2)
	{
		ship_to_move.SetYPos(ship_to_move.GetYPos() + 10.0*cos((double) ship_to_move.GetAngle()*PI/180));
		ship_to_move.SetXPos(ship_to_move.GetXPos() - 10.0*sin((double) ship_to_move.GetAngle()*PI/180));
	}
	else if(mov_type == 3)
	{
		ship_to_move.SetYPos(ship_to_move.GetYPos() - 10.0*cos((double) ship_to_move.GetAngle()*PI/180));
		ship_to_move.SetXPos(ship_to_move.GetXPos() + 10.0*sin((double) ship_to_move.GetAngle()*PI/180));
	}
	else if(mov_type == 4)
	{
		ship_to_move.SetAngle(ship_to_move.GetAngle() - 2.0);
		//std::cout<<ship_to_move.GetAngle() <<"\n";
	}
	else if(mov_type == 5)
	{
		ship_to_move.SetAngle(ship_to_move.GetAngle() + 2.0);
		//std::cout<<ship_to_move.GetAngle() <<"\n";
	}
	if(ship_to_move.GetXPos() < -(DimensionNegX))
	{
		ship_to_move.SetXPos(-(DimensionNegX));
	}
	if(ship_to_move.GetXPos() > DimensionPosX)
	{
		ship_to_move.SetXPos(DimensionPosX);		
	}
	if(ship_to_move.GetYPos() > DimensionPosY)
	{
		ship_to_move.SetYPos(DimensionPosY);
	}
	if(ship_to_move.GetYPos() < -(DimensionNegY))
	{
		ship_to_move.SetYPos(-(DimensionNegY));
	}
	VectorShips.at(ship_id) = ship_to_move; 
}

// void Board::MoveNthShip(int ship_id,int mov_type)
// {
// 	Ship ship_to_move = VectorShips.at(ship_id);
// 	if(mov_type == 0)
// 	{
// 		if(ship_to_move.GetXPos() - 5.0*cos((double) ship_to_move.GetAngle()*PI/180) > -(DimensionNegX) || 
// 		   ship_to_move.GetYPos() - 5.0*sin((double) ship_to_move.GetAngle()*PI/180) > -(DimensionNegY))
// 		{
// 			ship_to_move.SetXPos(ship_to_move.GetXPos() - 5.0*cos((double) ship_to_move.GetAngle()*PI/180));
// 			ship_to_move.SetYPos(ship_to_move.GetYPos() - 5.0*sin((double) ship_to_move.GetAngle()*PI/180));			
// 		}
// 	}
// 	else if(mov_type == 1)
// 	{
// 		if(ship_to_move.GetXPos() + 5.0*cos((double) ship_to_move.GetAngle()*PI/180) < DimensionPosX ||
// 		   ship_to_move.GetYPos() + 5.0*sin((double) ship_to_move.GetAngle()*PI/180) < DimensionPosY)
// 		{
// 			ship_to_move.SetXPos(ship_to_move.GetXPos() + 5.0*cos((double) ship_to_move.GetAngle()*PI/180));
// 			ship_to_move.SetYPos(ship_to_move.GetYPos() + 5.0*sin((double) ship_to_move.GetAngle()*PI/180));
// 		}
// 	}
// 	else if(mov_type == 2)
// 	{
// 		if(ship_to_move.GetYPos() + 5.0*cos((double) ship_to_move.GetAngle()*PI/180) < DimensionPosY ||
// 		   ship_to_move.GetXPos() - 5.0*sin((double) ship_to_move.GetAngle()*PI/180) > -(DimensionNegX))
// 		ship_to_move.SetYPos(ship_to_move.GetYPos() + 5.0*cos((double) ship_to_move.GetAngle()*PI/180));
// 		ship_to_move.SetXPos(ship_to_move.GetXPos() - 5.0*sin((double) ship_to_move.GetAngle()*PI/180));
// 	}
// 	else if(mov_type == 3)
// 	{
// 		if(ship_to_move.GetYPos() - 5.0*cos((double) ship_to_move.GetAngle()*PI/180) > -(DimensionNegY) ||
// 		   ship_to_move.GetXPos() + 5.0*sin((double) ship_to_move.GetAngle()*PI/180) < DimensionPosX)
// 		ship_to_move.SetYPos(ship_to_move.GetYPos() - 5.0*cos((double) ship_to_move.GetAngle()*PI/180));
// 		ship_to_move.SetXPos(ship_to_move.GetXPos() + 5.0*sin((double) ship_to_move.GetAngle()*PI/180));
// 	}
// 	else if(mov_type == 4)
// 	{
// 		ship_to_move.SetAngle(ship_to_move.GetAngle() - 5.0);
// 		std::cout<<ship_to_move.GetAngle() <<"\n";
// 	}
// 	else if(mov_type == 5)
// 	{
// 		ship_to_move.SetAngle(ship_to_move.GetAngle() + 5.0);
// 		std::cout<<ship_to_move.GetAngle() <<"\n";
// 	}
// 	VectorShips.at(ship_id) = ship_to_move; 
// }

//adding a random ship to the board
//the ship would be controlled by the AI initially and then added to the vector of ships
void Board::AddRandomShip()
{
	Ship random_ship;
	random_ship.SetXPos(RandomFloat(-(DimensionNegX),DimensionPosX));
	random_ship.SetYPos(RandomFloat(-(DimensionNegY) + 60,-(DimensionNegY) + 120));
	random_ship.SetColorFloat(rand() % 255,rand() % 255,rand() % 255);
	random_ship.SetAngle(RandomFloat(0.0,360.0));
	random_ship.SetId(VectorShips.size());
	random_ship.SetAIControlLevel(1);
	VectorShips.push_back(random_ship);
}

//adding random aliens being used in aliens coming from the top
void Board::AddRandomAlien()
{
	Alien random_alien;
	random_alien.SetXPos(RandomFloat(-(DimensionNegX),DimensionPosX));
	random_alien.SetYPos(RandomFloat((DimensionPosY)-10,DimensionPosY));
	random_alien.SetColorFloat(rand() % 255,rand() % 255,rand() % 255);
	random_alien.SetAngle(RandomFloat(0.0,360.0));
	random_alien.SetType(rand() % 2);	
	VectorAliens.push_back(random_alien);
}

//opcode 3 in ISA
//generating instructions of the ships to be sent as string over the network
std::string Board::GeneratePlayerPositionInstructions(int player_id)
{
	Ship player = VectorShips.at(player_id);
	return ("3_" + std::to_string(player_id) + "_" +
	player.GetName() + "_" +  
	std::to_string(player.GetXPos()) + "_" + 
	std::to_string(player.GetYPos()) + "_" + 
	std::to_string(player.GetAngle()) + "_" +
	std::to_string(player.GetColor().GetR()) + "_" + 
	std::to_string(player.GetColor().GetG()) + "_" + 
	std::to_string(player.GetColor().GetB()) + "_" + 
	std::to_string(player.GetLives()) + "_" + 
	std::to_string(player.GetScore()) + "_" + 
	std::to_string(player.GetMultiplier()) + "_" + 
	std::to_string(player.GetKills()) + "_" + 
	std::to_string(player.GetNumberBullets()) + "_" + 
	std::to_string(player.GetNumberMissiles()) + "_" + 
	std::to_string(player.GetAIControlLevel())
	);

}

//used in opcode 4 in ISA
//generating the informations of new bullets created by the ship to be sent over the network so that others can also see their positions
//these bullets to be added to the vector bullets in apply instructions 
std::string Board::GeneratePlayerBulletInstructions(std::vector<Bullet> vector_bullet)
{
	std::string answer = "";
	for(int i=0;i<vector_bullet.size();i++)
	{
		if(i<vector_bullet.size() - 1)
		{
			Bullet bullet_added = vector_bullet.at(i);
			answer  = answer + "4_" + std::to_string(bullet_added.GetXPos()) + "_" 
								   + std::to_string(bullet_added.GetYPos()) + "_" 
								   + std::to_string(bullet_added.GetVelX()) + "_" 
								   + std::to_string(bullet_added.GetVelY()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetR()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetG()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetB()) + "_" 
								   + std::to_string(bullet_added.GetShipId()) + "_" 
								   + std::to_string(ConvertBool(bullet_added.GetTypeAI())) + "_" 
								   + std::to_string(ConvertBool(bullet_added.GetTypePlayer())) + "\t";
		}
		else
		{
			Bullet bullet_added = vector_bullet.at(i);
			answer  = answer + "4_" + std::to_string(bullet_added.GetXPos()) + "_" 
								   + std::to_string(bullet_added.GetYPos()) + "_" 
								   + std::to_string(bullet_added.GetVelX()) + "_" 
								   + std::to_string(bullet_added.GetVelY()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetR()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetG()) + "_" 
								   + std::to_string(bullet_added.GetColorOfBullet().GetB()) + "_" 
								   + std::to_string(bullet_added.GetShipId()) + "_" 
								   + std::to_string(ConvertBool(bullet_added.GetTypeAI())) + "_" 
								   + std::to_string(ConvertBool(bullet_added.GetTypePlayer()));
		}
		
	}
	return answer;


}

//contains the information about the players and new bullets created by them
std::string Board::GeneratePlayerInstructions(int player_id,std::vector<Bullet> vector_bullet)
{
	return (GeneratePlayerPositionInstructions(player_id) + "\n" + 
	GeneratePlayerBulletInstructions(vector_bullet));

}

//split string s into vector of strings based on chartosplit
std::vector<std::string> Board::SplitString(std::string s, char chartosplit)
{
	// Break the string s into substrings wherever the chartosplit appears
	std::vector<std::string> answer;
	int initlength = s.length();
	std::string tostore = "";
	int i = 0;
	while(i < initlength)
	{
		if(s[i] == chartosplit)
		{
			// std::cout <<"storing :" << tostore<<"\n";
			answer.push_back(tostore);
			//std::cout <<tostore <<" was put in\n";
			tostore = "";
		}
		else
		{
			tostore = tostore+s[i];
		}
		i++;
	}	
	answer.push_back(tostore); 
	return answer;
}


//apply all the informations of a particular ship to update its status 
void Board::ApplyInsToShip(std::vector<std::string> s,Ship shiptochange)
{
	// std::cout << "0\t" <<s[0] <<"\n";
	// std::cout << "1\t" <<s[1] <<"\n";
	// std::cout << "2\t" <<s[2] <<"\n";
	// std::cout << "3\t" <<s[3] <<"\n";
	// std::cout << "4\t" <<s[4] <<"\n";
	// std::cout << "5\t" <<s[5] <<"\n";
	// std::cout << "6\t" <<s[6] <<"\n";
	// std::cout << "7\t" <<s[7] <<"\n";
	// std::cout << "8\t" <<s[8] <<"\n";
	shiptochange.SetName(s[2]);
	shiptochange.SetXPos(std::stof(s[3]));
	shiptochange.SetYPos(std::stof(s[4]));
	shiptochange.SetAngle(std::stof(s[5]));
	shiptochange.SetColorFloatInp(std::stof(s[6]),std::stof(s[7]),std::stof(s[8]));
	// shiptochange.SetLives(std::stoi(s[9]));
	// shiptochange.SetScore(std::stoi(s[10]));
	// shiptochange.SetMultiplier(std::stoi(s[11]));
	// shiptochange.SetKills(std::stoi(s[12]));
	// shiptochange.SetNumberBullets(std::stoi(s[13]));
	// shiptochange.SetNumberMissiles(std::stoi(s[14]));
	shiptochange.SetAIControlLevel(std::stoi(s[15]));
	VectorShips[std::stoi(s[1])] = shiptochange;	
	// std::cout << "applied all\n";
}

//apply informations of a particular bullet generated by ship so that it would be added to the vector ships
void Board::ApplyInsToBullets(std::string bulletinfostring)
{
	Bullet bullettoadd;
	std::vector<std::string> bulletinfo = SplitString(bulletinfostring,'_');
	bullettoadd.SetXPos(std::stof(bulletinfo[1]));
	bullettoadd.SetYPos(std::stof(bulletinfo[2]));
	bullettoadd.SetVelX(std::stof(bulletinfo[3]));
	bullettoadd.SetVelY(std::stof(bulletinfo[4]));
	bullettoadd.SetColorFloat(std::stof(bulletinfo[5]),std::stof(bulletinfo[6]),std::stof(bulletinfo[7]));
	bullettoadd.SetShipID(std::stoi(bulletinfo[8]));
	bullettoadd.SetTypeAI(ExtractBool(std::stoi(bulletinfo[9])));
	bullettoadd.SetTypePlayer(ExtractBool(std::stoi(bulletinfo[10])));
	VectorBullets.push_back(bullettoadd);
}

// void Board::ApplyShipInstructions(std::string information)
// {
// 	std::vector<std::string> ship_bullets = SplitString(information,'\n');	
// 	std::vector<std::string> shipinfo = SplitString(ship_bullets[0],'_');
// 	int shipid = std::stoi(shipinfo[1]);
// 	ApplyInsToShip(shipinfo,VectorShips[shipid]);
// 	for (int i = 1;i<ship_bullets.size();i++)
// 	{
// 		std::vector<std::string> bulletinfo = SplitString(ship_bullets[i],'\t');
// 		ApplyInsToBullets(bulletinfo);
// 	}
// }


//apply instructions coming in the form of string for all the ships 
void Board::ApplyShipInstructions(std::string information,int t)
{
	//std::vector<std::string> ship_bullets = SplitString(information,'\n');	
	std::vector<std::string> allshipinfo = SplitString(information,'\t');
	for(int i = 0;i<allshipinfo.size();i++)
	{
		std::vector<std::string> shipinfo = SplitString(allshipinfo[i],'_');
		// std::cout <<"splitted\t" <<shipinfo[1] <<"\n";
		int shipid = std::stoi(shipinfo[1]);
		// std::cout << "obtianed :" << shipid << "\n";
		while (shipid >= VectorShips.size())
		{
			VectorShips.push_back(Ship(VectorShips.size()));
		}
		if(shipid != t)
		{
			ApplyInsToShip(shipinfo,VectorShips[shipid]);
		}
		else
		{	
			VectorShips[shipid].SetLives(std::stoi(shipinfo[9]));
			VectorShips[shipid].SetScore(std::stoi(shipinfo[10]));
			VectorShips[shipid].SetMultiplier(std::stoi(shipinfo[11]));
			VectorShips[shipid].SetKills(std::stoi(shipinfo[12]));
			VectorShips[shipid].SetNumberBullets(std::stoi(shipinfo[13]));
			VectorShips[shipid].SetAIControlLevel(std::stoi(shipinfo[15]));
		}
	}
}

//generate all instructions string to all the bullets in vector bullets to be sent to ships other than the one which
//is controlling the AI
std::string Board::GenerateAllBulletInstructions()
{
	std::string answer = "";
	int bullet_vect_size = VectorBullets.size();
	for (int i = 0;i < bullet_vect_size;i++)
	{
		if(i<bullet_vect_size-1)
		{
			answer = answer + "5_" + std::to_string(i) + "_"  
					+ std::to_string(VectorBullets.at(i).GetXPos()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetYPos()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetVelX()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetVelY()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetR()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetG()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetB()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetShipId()) + "_" 
					+ std::to_string(ConvertBool(VectorBullets.at(i).GetTypeAI())) + "_" 
					+ std::to_string(ConvertBool(VectorBullets.at(i).GetTypePlayer())) + "\t";
		}
		else
		{
			answer = answer + "5_" + std::to_string(i) + "_"  
					+ std::to_string(VectorBullets.at(i).GetXPos()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetYPos()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetVelX()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetVelY()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetR()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetG()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetColorOfBullet().GetB()) + "_" 
					+ std::to_string(VectorBullets.at(i).GetShipId()) + "_" 
					+ std::to_string(ConvertBool(VectorBullets.at(i).GetTypeAI())) + "_" 
					+ std::to_string(ConvertBool(VectorBullets.at(i).GetTypePlayer()));

		}
	}
	return answer;
}

//apply the informations of bullets in vector of bullets of the pc being AI 
void Board::ApplySingleBulletInstructions(Bullet bullet_applied,std::string information)
{
	std::vector<std::string> bulletinfo = SplitString(information,'_');
	bullet_applied.SetXPos(std::stof(bulletinfo[2]));
	bullet_applied.SetYPos(std::stof(bulletinfo[3]));
	bullet_applied.SetVelX(std::stof(bulletinfo[4]));
	bullet_applied.SetVelY(std::stof(bulletinfo[5]));
	bullet_applied.SetColorFloat(std::stof(bulletinfo[6]),std::stof(bulletinfo[7]),std::stof(bulletinfo[8]));
	bullet_applied.SetShipID(std::stoi(bulletinfo[9]));
	bullet_applied.SetTypeAI(ExtractBool(std::stoi(bulletinfo[10])));
	bullet_applied.SetTypePlayer(ExtractBool(std::stoi(bulletinfo[11])));
	VectorBullets.at(std::stoi(bulletinfo[1])) =  bullet_applied;
}

//apply instructions to all the bullets using the above function 
void Board::ApplyAllBulletInstructions(std::string information)
{
	std::vector<std::string> allbulletsinfo = SplitString(information,'\t');
	if(allbulletsinfo.size() >= VectorBullets.size())
	{
		for(int i = 0;i<allbulletsinfo.size();i++)
		{
			
			if(i<VectorBullets.size())
			{
				ApplySingleBulletInstructions(VectorBullets.at(i),allbulletsinfo[i]);	
			}
			else
			{
				VectorBullets.push_back(Bullet());
				ApplySingleBulletInstructions(VectorBullets.at(i),allbulletsinfo[i]);
			}
		}
	}
	else
	{
		for(int i = 0;i<allbulletsinfo.size();i++)
		{
			ApplySingleBulletInstructions(VectorBullets.at(i),allbulletsinfo[i]);
		}
		VectorBullets.erase(VectorBullets.begin()+allbulletsinfo.size(),VectorBullets.end());
	}

}	
 
//generating string of informations for player and new bullets created by them 
std::string Board::GenerateOnlyPlayerInstructions(int player_id,std::vector<Bullet> vector_bullet)
{
	return (GeneratePlayerPositionInstructions(player_id) + "\n" + GeneratePlayerBulletInstructions(vector_bullet)); 
}

//generating string of informations to get number of aliens bullets ships all bullets info and all aliens info
//and info of the ship being controlled by pc acting as AI
std::string Board::GenerateAllInstructions(int player_id,std::vector<Points> points)
{

	return (GeneratingCount() + "\n" + GenerateAllBulletInstructions() + "\n" + GenerateAliensInformation() + "\n" + GetStringPoints(points) + "\n" + GenerateShipInsForAI()); 
}

//apply all the instructions based on opcodes 
std::vector<Points> Board::ApplyInstructions(std::string information,int shipid)
{
	//std::cout <<"applying for: " <<information<<"\n";
	std::vector<std::string> infosplitted = SplitString(information,'\n');
	std::vector<Points> vanswer;
	for (int i = 0;i<infosplitted.size();i++)
	{
		if (infosplitted[i]=="")
		{
			//std::cout <<"empty inst\n";
		}
		else if(infosplitted[i][0] == '9')
		{
			vanswer=GetVectorPoints(infosplitted[i]);
		}
		else if(infosplitted[i][0] == '6')
		{
			ApplyInstruction6(infosplitted[i]);
		}
		else if(infosplitted[i][0] == '3')
		{
			ApplyShipInstructions(infosplitted[i],shipid);
		}
		else if(infosplitted[i][0] == '4')
		{
			ApplyPlayerBulletInstructions(infosplitted[i]);
		}
		else if(infosplitted[i][0] == '5')
		{
			ApplyAllBulletInstructions(infosplitted[i]);
		}
		else if(infosplitted[i][0] == '7')
		{
			//std::cout <<"applying aliens\n";
			ApplyAllAlienInstructions(infosplitted[i]);
		}
		else if(infosplitted[i][0] == 't')
		{
			ApplyInstructionsToShipAI(infosplitted[i]);
		}
	}
	return vanswer;
}

//generating info containing the number of ships bullets and alien 
std::string Board::GeneratingCount()
{
	return ("6_" + std::to_string(VectorShips.size()) + "_" + 
			std::to_string(VectorBullets.size()) + "_" + 
			std::to_string(VectorAliens.size()));
}

//apply the information generated above
void Board::ApplyInstruction6(std::string information)
{
	std::vector<std::string> s = SplitString(information,'_');
	if(std::stoi(s[2]) == 0)
	{
		VectorBullets.clear();
	}
	if(std::stoi(s[3]) == 0)
	{
		VectorAliens.clear();
	} 
}

//generating aliens information to be sent to sent to other pcs other than server to update their position there
std::string Board::GenerateAliensInformation()
{
	std::string answer = "";
	int alien_vect_size = VectorAliens.size();
	 
	for(int i = 0;i < alien_vect_size;i++)
	{
		if(i < alien_vect_size - 1)
		{
			answer = answer + "7_" + std::to_string(i)  + "_" 
				 	+std::to_string(VectorAliens.at(i).GetXPos()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetYPos()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetAngle()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetColor().GetR()) + "_"
					+std::to_string(VectorAliens.at(i).GetColor().GetG()) + "_"
					+std::to_string(VectorAliens.at(i).GetColor().GetB()) + "_"
					+std::to_string(VectorAliens.at(i).GetLives()) + "_"
					+std::to_string(VectorAliens.at(i).GetLevel()) + "_"
					+std::to_string(VectorAliens.at(i).GetBullets()) + "_"
					+std::to_string(VectorAliens.at(i).GetMissiles()) + "_"
					+std::to_string(VectorAliens.at(i).GetType()) + "\t";
		}
		else
		{
			answer = answer + "7_"  + std::to_string(i)  + "_"
				 	+std::to_string(VectorAliens.at(i).GetXPos()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetYPos()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetAngle()) + "_"
				 	+std::to_string(VectorAliens.at(i).GetColor().GetR()) + "_"
					+std::to_string(VectorAliens.at(i).GetColor().GetG()) + "_"
					+std::to_string(VectorAliens.at(i).GetColor().GetB()) + "_"
					+std::to_string(VectorAliens.at(i).GetLives()) + "_"
					+std::to_string(VectorAliens.at(i).GetLevel()) + "_"
					+std::to_string(VectorAliens.at(i).GetBullets()) + "_"
					+std::to_string(VectorAliens.at(i).GetMissiles()) + "_"
					+std::to_string(VectorAliens.at(i).GetType());
		
		}
	}
	return answer;
}

//apply the info for single aliens generated above
void Board::ApplySingleAlienInstructions(Alien alien_applied,std::string information)
{
	std::vector<std::string> alieninfo = SplitString(information,'_');
	alien_applied.SetXPos(std::stof(alieninfo[2]));
	alien_applied.SetYPos(std::stof(alieninfo[3]));
	alien_applied.SetAngle(std::stof(alieninfo[4]));
	alien_applied.SetColorFloat2(std::stof(alieninfo[5]),std::stof(alieninfo[6]),std::stof(alieninfo[7]));
	alien_applied.SetLives(std::stoi(alieninfo[8]));
	alien_applied.SetLevel(std::stoi(alieninfo[9]));
	alien_applied.SetBullets(std::stoi(alieninfo[10]));
	alien_applied.SetMissiles(std::stoi(alieninfo[11]));
	alien_applied.SetType(std::stoi(alieninfo[12]));
	VectorAliens.at(std::stoi(alieninfo[1])) = alien_applied;
}


//apply the info generated above for all the aliens in the vector
void Board::ApplyAllAlienInstructions(std::string information)
{
	std::vector<std::string> allaliensinfo = SplitString(information,'\t');
	if(allaliensinfo.size() >= VectorAliens.size())
	{
		for(int i = 0;i<allaliensinfo.size();i++)
		{
			
			if(i<VectorAliens.size())
			{
				ApplySingleAlienInstructions(VectorAliens.at(i),allaliensinfo[i]);	
			}
			else
			{
				VectorAliens.push_back(Alien());
				ApplySingleAlienInstructions(VectorAliens.at(i),allaliensinfo[i]);
			}
		}
	}
	else
	{
		for(int i = 0;i<allaliensinfo.size();i++)
		{
			ApplySingleAlienInstructions(VectorAliens.at(i),allaliensinfo[i]);
		}
		VectorAliens.erase(VectorAliens.begin()+allaliensinfo.size(),VectorAliens.end());
	}
}

//apply the instructions for the new bullets generated by the player
void Board::ApplyPlayerBulletInstructions(std::string information)
{
	// std::cout << information <<"\n";
	std::vector<std::string> allbullets = SplitString(information,'\t');
	for (int i = 0;i<allbullets.size();i++)
	{
		ApplyInsToBullets(allbullets.at(i));
	}
}

//updating the bullets so that they are not killing anyone used when someone disconnects 
void Board::UpdateBulletsWithoutKilling()
{
	// for (int i=VectorBullets.size()-1;i>=0;i--)
	// {
	// 	if(VectorBullets.at(i).GetXPos()+2.0 > DimensionPosX 
	// 	|| VectorBullets.at(i).GetXPos()-2.0 < -(DimensionNegX) 
	// 	|| VectorBullets.at(i).GetYPos()+2.0 > DimensionPosY
	// 	|| VectorBullets.at(i).GetYPos()-2.0 < -(DimensionNegY))
	// 	{
	// 		VectorBullets.erase(VectorBullets.begin() + i);
	// 	}
	// }
	for (int i=0;i<VectorBullets.size();i++)
	{
		if(VectorBullets.at(i).GetTypeAI() == false)
		{
			VectorBullets.at(i).SetXPos(VectorBullets.at(i).GetXPos()+VectorBullets.at(i).GetVelX());
			VectorBullets.at(i).SetYPos(VectorBullets.at(i).GetYPos()+VectorBullets.at(i).GetVelY());
		}
	}
	
}	

//get the score of ship given its id
std::string Board::GetNthPlayerScore(int shipid)
{ 
	if(VectorShips.at(shipid).GetLives() == 0)
	{
		return "";
	}
	else
	{
		return (ToFour(std::to_string(VectorShips.at(shipid).GetScore())) +"\t\t\t" + VectorShips.at(shipid).GetName());
	}
}

//get the name of the player given shipid
std::string Board::GetNthPlayerName(int shipid)
{
	return VectorShips.at(shipid).GetName();
}

//opcode 9 in ISA
//generating string to be sent info of points where explosion would take place
std::string Board::GetStringPoints(std::vector<Points> points)
{
	std::string answer = "";
	// std::cout <<"starting for\n";
	if (points.size()>0)
	{
		for(int i = 0;i<points.size() -1;i++)
		{
			answer = answer + "9_" + std::to_string(points.at(i).x) + "_" 
									+ std::to_string(points.at(i).y) + "_" 
									+ std::to_string(points.at(i).z) + "\t";
		}
		// std::cout <<"primary for done\n";
		answer = answer + "9_" + std::to_string(points.at(points.size() - 1).x) + "_" 
					   + std::to_string(points.at(points.size() - 1).y) + "_" 
					   + std::to_string(points.at(points.size() - 1).z);
	}
	return answer;
}

//generating instructions to get vector of points coming as string 
std::vector<Points> Board::GetVectorPoints(std::string information)
{
	// std::cout << "info\t" << information <<"\n";
	std::vector<Points> answer;
	std::vector<std::string> splitted = SplitString(information,'\t'); 
	for(int i = 0;i<splitted.size();i++)
	{
		std::vector<std::string> pointinfo = SplitString(splitted.at(i),'_');
		Points newpoint;
		newpoint.x = std::stof(pointinfo.at(1));
		newpoint.y = std::stof(pointinfo.at(2));
		newpoint.z = std::stof(pointinfo.at(3));
		answer.push_back(newpoint);
	}
	// std::cout << answer.size() <<" these many explosions will come\n";
	// if (answer.size())
	// {
	// 	std::cout <<answer[0].x <<"\t" << answer[0].y <<"\t" <<answer[0].z <<"\n";
	// }
	return answer;
}

//generating instructions for the ship whose pc is controlling the AI
std::string Board::GenerateShipInsForAI()
{
	std::string answer = "";
	for(int i=0;i<VectorShips.size() - 1;i++)
	{
		answer = answer + "t_" + std::to_string(i) + "_" 
							   + std::to_string(VectorShips.at(i).GetLives()) + "_" 
							   + std::to_string(VectorShips.at(i).GetScore()) + "_" 
							   + std::to_string(VectorShips.at(i).GetNumberMissiles()) + "\t"; 
	}
	answer = answer + "t_" + std::to_string(VectorShips.size()-1) + "_" 
						   + std::to_string(VectorShips.at(VectorShips.size()-1).GetLives()) + "_" 
						   + std::to_string(VectorShips.at(VectorShips.size()-1).GetScore()) + "_" 
						   + std::to_string(VectorShips.at(VectorShips.size()-1).GetNumberMissiles());

	return answer;
}

//apply instructions for the ship whose pc is controlling the AI
void Board::ApplyInstructionsToShipAI(std::string information)
{
	std::vector<std::string> AllShipsInfo = SplitString(information,'\t');
	for(int i = 0;i<AllShipsInfo.size();i++)
	{
		std::vector<std::string> shipinfo = SplitString(AllShipsInfo[i],'_');
		int id  = std::stoi(shipinfo[1]);
		while (id >= VectorShips.size())
		{
			VectorShips.push_back(Ship(VectorShips.size()));
		}
		VectorShips.at(id).SetLives(std::stoi(shipinfo[2]));
		VectorShips.at(id).SetScore(std::stoi(shipinfo[3]));
		// VectorShips.at(id).SetNumberMissiles(std::stoi(shipinfo[4])); 
	}
}

//checking if the game is over
bool Board::CheckGameOver()
{
	for (int i=0; i<VectorShips.size(); i++)
	{
		if (VectorShips[i].GetLives()!=0)
		{
			return false;
		}
	}
	return true;
} 

//getting ranking of the player to be displayed after game is over
std::vector<std::string> Board::GetRanking()
{
	std::vector< std::pair<int,std::string> > player_score; 
	for (int i=0;i<VectorShips.size();i++)
	{
		std::pair<int,std::string> x; 
		x.first = VectorShips.at(i).GetScore();
		x.second = VectorShips.at(i).GetName();
		player_score.push_back(x);
	} 
	std::sort (player_score.begin(),player_score.end(), PairCompare);
	std::string rank = "Rank";
	std::string name =  "Name";
	std::string score  = "Score";
	std::vector<std::string> answer;
	answer.push_back(rank + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + score + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + name );
	for(int i=0;i<player_score.size() && i<5 ;i++)
	{
		answer.push_back(std::to_string(i+1) + "."  + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" +"\t" +"\t" + ToFour(std::to_string(player_score.at(i).first)) +  "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" +  player_score.at(i).second );
	}
	return answer;

}