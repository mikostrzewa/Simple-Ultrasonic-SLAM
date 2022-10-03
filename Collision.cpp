//
//  main.cpp
//  Collison detection
//
//  Created by PsychoAce on 02/07/2019.
//  Copyright © 2019 PsychoAce. All rights reserved.
//

#include <iostream>

using namespace std;

struct point{
    int x;
    int y;
};

struct Wall{
    point p1;
    point p2;
};

struct rectangle{
    point points[4];
    Wall walls[4];
    void setRectangle(){
        for(int i = 0; i<4; i++){
            cin >> points[i].x;
            cin >> points[i].y;
        }
        for(int i = 0; i<4; i++){
            walls[i].p1=points[i];
            walls[i].p2=points[(i+1)%4];
        }
    }
};

int det(Wall w, point c){
    point a=w.p1;
    point b=w.p2;
    
    int p = a.y*b.x+b.y*c.x+c.y*a.x-c.y*b.x-a.y*c.x-b.y*a.x;
    
    return p;
}

int main() {
    rectangle Robot;
    rectangle Obstacle;
    
    Robot.setRectangle();
    Obstacle.setRectangle();
    
    bool collission = false;
    for(int i = 0; i<4; i++){
        collission = true;
        for(int j = 0; j<4; j++){
            if(det(Obstacle.walls[j], Robot.points[i])<0){
                collission=false;
            }
        }
        
        if(collission){
            break;
        }
    }
    
   
    if(collission){
        cout << "COLLISION!\n";
    }
    else{
        for(int i = 0; i<4; i++){
            collission = true;
            for(int j = 0; j<4; j++){
                if(det(Robot.walls[j], Obstacle.points[i])<0){
                    collission=false;
                }
            }
            if(collission){
                break;
            }
        }
        
        if(collission){
            cout << "COLLISION!\n";
        }
        else{
            cout << "Nothing\n";
        }
        
    }
    /*
    int result =  det(Obstacle.walls[0],Robot.points[0]);
    
    if(result<0){
        cout << "Left\n";
    }
    else if(result==0){
        cout << "On line";
    }
    else{
        cout << "Right\n";
    }
    */
    return 0;
}
