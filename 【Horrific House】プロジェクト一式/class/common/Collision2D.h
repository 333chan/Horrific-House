#pragma once
#include<list>
#include"Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollisionM = std::pair<Vector2Int, Vector2Int>;
using CollList = std::list<Collision>;
using CollListName = std::list<std::pair<std::string,Collision>>;
//first:ˆÊ’u second:y‚Ì•ûŒü
using CollisionList = std::pair<Vector2, Vector2>;
