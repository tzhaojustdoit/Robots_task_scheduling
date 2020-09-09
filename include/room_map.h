#pragma once

#include "ros/ros.h"
using namespace std;

typedef pair<int,int> RoomPair;
typedef vector<int> Value;

typedef pair<int,int> RoomPair;
typedef set<int> RltDoors;



class ROOMMap{
public: 
  static RltDoors getRelativeDoors(int room1, int room2){
    RltDoors s;
    return s;
  }
map<RoomPair,RltDoors>  ROOMMAP= {
    {RoomPair(0,1),{1}},
    {RoomPair(0,2),{1,2}},
    {RoomPair(0,3),{1,3}},
    {RoomPair(0,4),{1,4}},
    {RoomPair(0,5),{1,5}},
    {RoomPair(0,6),{1,6}},
    {RoomPair(0,7),{7}},
    {RoomPair(0,8),{7,8}},
    {RoomPair(0,9),{9}},
    {RoomPair(0,10),{10}},
    {RoomPair(0,11),{10,11}},
    {RoomPair(0,12),{10,12}},
    {RoomPair(0,13),{10,13}},
    {RoomPair(0,14),{14}},
    {RoomPair(0,15),{15}},
    {RoomPair(0,16),{16}},

    {RoomPair(1,2),{2}},
    {RoomPair(1,3),{3}},
    {RoomPair(1,4),{4}},
    {RoomPair(1,5),{5}},
    {RoomPair(1,6),{6}},
    {RoomPair(1,7),{1,7}},
    {RoomPair(1,8),{1,7,8}},
    {RoomPair(1,9),{1,9}},
    {RoomPair(1,10),{1,10}},
    {RoomPair(1,11),{1,10,11}},
    {RoomPair(1,12),{1,10,12}},
    {RoomPair(1,13),{1,10,13}},
    {RoomPair(1,14),{1,14}},
    {RoomPair(1,15),{1,15}},
    {RoomPair(1,16),{1,16}},
    
    {RoomPair(2,4),{2,4}},
    {RoomPair(2,5),{2,5}},
    {RoomPair(2,6),{2,6}},
    {RoomPair(2,7),{2,1,7}},
    {RoomPair(2,8),{2,1,8}},
    {RoomPair(2,9),{2,1,9}},
    {RoomPair(2,10),{2,1,10}},
    {RoomPair(2,11),{2,1,10,11}},
    {RoomPair(2,12),{2,1,10,12}},
    {RoomPair(2,13),{2,1,10,13}},
    {RoomPair(2,14),{2,1,14}},
    {RoomPair(2,15),{2,1,15}},
    {RoomPair(2,16),{2,1,16}},
    
    {RoomPair(3,4),{3,4}},
    {RoomPair(3,5),{3,5}},
    {RoomPair(3,6),{3,6}},
    {RoomPair(3,7),{3,1,7}},
    {RoomPair(3,8),{3,1,8}},
    {RoomPair(3,9),{3,1,9}},
    {RoomPair(3,10),{3,1,10}},
    {RoomPair(3,11),{3,1,10,11}},
    {RoomPair(3,12),{3,1,10,12}},
    {RoomPair(3,13),{3,1,10,13}},
    {RoomPair(3,14),{3,1,14}},
    {RoomPair(3,15),{3,1,15}},
    {RoomPair(3,16),{3,1,16}},

    {RoomPair(4,5),{4,5}},
    {RoomPair(4,6),{4,6}},
    {RoomPair(4,7),{4,1,7}},
    {RoomPair(4,8),{4,1,8}},
    {RoomPair(4,9),{4,1,9}},
    {RoomPair(4,10),{4,1,10}},
    {RoomPair(4,11),{4,1,10,11}},
    {RoomPair(4,12),{4,1,10,12}},
    {RoomPair(4,13),{4,1,10,13}},
    {RoomPair(4,14),{4,1,14}},
    {RoomPair(4,15),{4,1,15}},
    {RoomPair(4,16),{4,1,16}},

    {RoomPair(5,6),{5,6}},
    {RoomPair(5,7),{5,1,7}},
    {RoomPair(5,8),{5,1,8}},
    {RoomPair(5,9),{5,1,9}},
    {RoomPair(5,10),{5,1,10}},
    {RoomPair(5,11),{5,1,10,11}},
    {RoomPair(5,12),{5,1,10,12}},
    {RoomPair(5,13),{5,1,10,13}},
    {RoomPair(5,14),{5,1,14}},
    {RoomPair(5,15),{5,1,15}},
    {RoomPair(5,16),{5,1,16}},

    {RoomPair(6,7),{6,1,7}},
    {RoomPair(6,8),{6,1,8}},
    {RoomPair(6,9),{6,1,9}},
    {RoomPair(6,10),{6,1,10}},
    {RoomPair(6,11),{6,1,10,11}},
    {RoomPair(6,12),{6,1,10,12}},
    {RoomPair(6,13),{6,1,10,13}},
    {RoomPair(6,14),{6,1,14}},
    {RoomPair(6,15),{6,1,15}},
    {RoomPair(6,16),{6,1,16}},

    {RoomPair(7,8),{8}},
    {RoomPair(7,9),{7,9}},
    {RoomPair(7,10),{7,10}},
    {RoomPair(7,11),{7,10,11}},
    {RoomPair(7,12),{7,10,12}},
    {RoomPair(7,13),{7,10,13}},
    {RoomPair(7,14),{7,14}},
    {RoomPair(7,15),{7,15}},
    {RoomPair(7,16),{7,16}},
 
    {RoomPair(8,9),{8,7,9}},
    {RoomPair(8,10),{8,7,10}},
    {RoomPair(8,11),{8,7,10,11}},
    {RoomPair(8,12),{8,7,10,12}},
    {RoomPair(8,13),{8,7,10,13}},
    {RoomPair(8,14),{8,7,14}},
    {RoomPair(8,15),{8,7,15}},
    {RoomPair(8,16),{8,7,16}},

    {RoomPair(9,10),{9,10}},
    {RoomPair(9,11),{9,10,11}},
    {RoomPair(9,12),{9,10,12}},
    {RoomPair(9,13),{9,10,13}},
    {RoomPair(9,14),{9,14}},
    {RoomPair(9,15),{9,15}},
    {RoomPair(9,16),{9,16}},

   
    {RoomPair(10,11),{11}},
    {RoomPair(10,12),{12}},
    {RoomPair(10,13),{13}},
    {RoomPair(10,14),{10,14}},
    {RoomPair(10,15),{10,15}},
    {RoomPair(10,16),{10,16}},

    {RoomPair(11,12),{11,12}},
    {RoomPair(11,13),{11,13}},
    {RoomPair(11,14),{11,10,14}},
    {RoomPair(11,15),{11,10,15}},
    {RoomPair(11,16),{11,10,16}},

    {RoomPair(12,13),{12,13}},
    {RoomPair(12,14),{12,10,14}},
    {RoomPair(12,15),{12,10,15}},
    {RoomPair(12,16),{12,10,16}},

    {RoomPair(13,14),{12,10,14}},
    {RoomPair(13,15),{12,10,15}},
    {RoomPair(13,16),{12,10,16}},

    {RoomPair(14,15),{14,15}},
    {RoomPair(14,16),{14,16}},

    {RoomPair(15,16),{15,16}}
    
};
};