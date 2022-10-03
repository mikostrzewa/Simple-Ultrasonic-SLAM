#include "slam.h"

void Rectangle::setRectangle(float w, float h, float Ox, float Oy, float Px, float Py){
	height = h;
    width = w;
	origin.x=Ox;
	origin.y=Oy;
	position.x=Px;
	position.y=Py;
	rotation = 0;
	sf::Vector2f Velocity;
}

void Rectangle::getPosition(sf::Vector2f pos){
        position=pos;
}

void Rectangle::getPoints(point p1,point p2,point p3,point p4){
	points[0]=p1;
	points[1]=p2;
	points[2]=p3;
	points[3]=p4;
	setWalls();
}
void Rectangle::setWalls(){
        for(int i = 0; i<4; i++){
	    walls[i].p1=points[i];
	    walls[i].p2=points[(i+1)%4];
	}
}

void Controler::getInput(bool det1, bool det2, bool det3){
	col_1 = det1;
	col_2 = det2;
	col_3 = det3;
}

void Controler::getPos(float rot, float posx, float posy){
	rotation = rot;
	x = posx;
	y = posy;

	if(rotation<0){
		rotation=360+rotation;
	}
}


void Controler::setState(){
	if(state==0&&(col_1||col_2||col_3)){
		state = 8;
		spiral = 1.0;
	}
	else if((state==9||state==10)&&(col_2||col_3)){
		/*
		if(state==9){
			rotation-=5;
		}
		else{
			rotation+=5;
		}
		*/

		if(col_3){
			fx[0].push_back(x-10);
			fx[1].push_back(x+10);
			fy[0].push_back(y-20);
			fy[1].push_back(y+20);
		}
		else if(col_2){
			fx[0].push_back(x-10);
			fx[1].push_back(x+10);
			fy[0].push_back(y-20);
			fy[1].push_back(y+20);
		}

		state = 11;
	}
	else if(!(col_1||col_2||col_3)&&!state==8){
		state = 0;
	}
}

void Controler::giveCommand(){
	if(state==0){
		search();
	}
	else if(state==8){
		findNext();
	}
	else if(state==9){
		rotation-=accuracy;
	}
	else if(state==10){
		rotation+=accuracy;
	}
	else if(state==11){
		goAlong();
	}
}

void Controler::search(){
	move.y=0.5*-cos(rotation*(PI/180.0));
	move.x=0.5*sin(rotation*(PI/180.0));
	if(delaySpiral>0){
		delaySpiral--;
	}
	else{
		spiral/=1.0005;
		rotation+=(spiral);
	}
}

void Controler::findNext(){
	move.y=0;
	move.x=0;
	float angle = std::fmod(rotation,360.0);
	if((angle>=0&&angle<=90)||(angle>90&&angle<=180)){
		state=10;
	}
	else if((angle>180&&angle<=270)||angle>270){
		state=9;
	}
	//std::cout << angle << "\n";

}

void Controler::goAlong(){
	if(delay>0){
		delay--;
	}
	else{
		for(int i = 0; i<fx[0].size(); i++){
			if(fx[0][i]<=x&&fx[1][i]>=x&&fy[0][i]<=y&&fy[1][i]>=y){
			//std::cout << "KONIEC!" << std::endl;
			state=0;
			srand(time(NULL));
			if(prev==2){
				rotation+=rand()%180+10;
			}
			else if(prev==3){
				rotation-=rand()%180+10;
			}
			delay=100;
			delaySpiral = 1000;
			return;
		}
		}
		if(col_3){
			fx[0].push_back(x-20);
			fx[1].push_back(x+20);
			fy[0].push_back(y-20);
			fy[1].push_back(y+20);
		}
		else if(col_2){
			fx[0].push_back(x-20);
			fx[1].push_back(x+20);
			fy[0].push_back(y-20);
			fy[1].push_back(y+20);
		}
		delay=100;
	}
	move.y=0;
	move.x=0;
	float angle = std::fmod(rotation,360.0);
	
	if(col_2^col_3){
		if(col_2){
			prev=2;
		}
		else{
			prev=3;
		}
	}

	if(!(col_2||col_3)){

		/*
		if(prev==3){
			rotation+=1;
		}
		else{
			rotation-=1;
		}
		*/
		if(!(col_1)){
			rotation-=adjust;
			move.y=1.2*-cos(rotation*(PI/180.0));
			move.x=1.2*sin(rotation*(PI/180.0));

		}
		else{
			rotation+=adjust;
		}

	}
	else if(col_2&&col_3&&(!col_1)){
		move.y=1.2*-cos(rotation*(PI/180.0));
		move.x=1.2*sin(rotation*(PI/180.0));
	}
	else if(col_2){

		rotation+=accuracy;
		adjust=accuracy;
	}
	else if(col_3){
		rotation-=accuracy;
		adjust=accuracy*-1;
	}
	/*
	if((col_2||col_3)&&(!col_1)){
		move.y=0.2*-cos(rotation*(PI/180.0));
		move.x=0.2*sin(rotation*(PI/180.0));
	}
	*/

}

	sf::Vector2f Controler::go(){
	//std::cout << state << "\n";
	return move;
	}

	float Controler::rotate(){
	return rotation;
	}
