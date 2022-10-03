#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <bits/stdc++.h>

//srand(time(NULL));

#define PI 3.14159265

#include "slam.cpp"

const int num_obs = 15;

struct line{
    sf::Vertex points[2];
    
    void set(point A, point B){
        points[0].position = sf::Vector2f(A.x,A.y);
        points[1].position = sf::Vector2f(B.x,B.y);
    }
};

float det(Wall w, point c){
    point a=w.p1;
    point b=w.p2;
    
    float p = a.y*b.x+b.y*c.x+c.y*a.x-c.y*b.x-a.y*c.x-b.y*a.x;
    
    return p;
}

point VecToPoint(sf::Vector2f vec){
    point p;
    p.x=vec.x;
    p.y=vec.y;

    return p;
}

int main()
{
    Controler algorithm;
    sf::RectangleShape SfBody;
    sf::RectangleShape SfDet[3];
    sf::RectangleShape SfObs[num_obs];

    Rectangle Robot;
    Rectangle Det[3];
    Rectangle Obs[num_obs];

    std::vector<line> lines;

    Robot.setRectangle(23.f,34.f,11.5,17.f,500,950);

    Det[0].setRectangle(10.f,39.f,5.5,22,500,950);
    Det[1].setRectangle(34.f,10.f,25,5,500,950);
    Det[2].setRectangle(37.f,10.f,9,5,500,950);

    Obs[0].setRectangle(100.f,200.f,0,0,600,425);
    Obs[1].setRectangle(50.f,1300.f,0,0,0,0);
    Obs[2].setRectangle(1200.f,50.f,0,0,0,0);
    Obs[3].setRectangle(50.f,1300,0,0,1150,0);
    Obs[4].setRectangle(1200.f,50.f,0,0,0,1250);
    Obs[5].setRectangle(20.f,100.f,0,0,360,325);
    Obs[6].setRectangle(150.f,60.f,0,0,630,525);
    Obs[7].setRectangle(120.f,90.f,0,0,340,853);
    Obs[8].setRectangle(25.f,100.f,0,0,800,500);
    Obs[9].setRectangle(146.f,90.f,0,0,400,705);
    Obs[10].setRectangle(34.f,223.f,0,0,520,413);
    Obs[11].setRectangle(523.f,45.f,0,0,430,800);
    Obs[12].setRectangle(234.f,45.f,0,0,300,450);
    Obs[13].setRectangle(234.f,45.f,0,0,1200,1200);
    Obs[14].setRectangle(234.f,45.f,0,0,521,1000);

    sf::RenderWindow window(sf::VideoMode(1200, 1300), "SLAM simulation");
    sf::RenderWindow window2(sf::VideoMode(1200, 1300), "SLAM ");

    SfBody.setSize(sf::Vector2f(Robot.width,Robot.height));
    SfBody.setOrigin(Robot.origin.x,Robot.origin.y);
    SfBody.setFillColor(sf::Color::Red);
    SfBody.setPosition(Robot.position.x,Robot.position.y);

    SfDet[0].setSize(sf::Vector2f(Det[0].width,Det[0].height));
    SfDet[0].setOrigin(Det[0].origin.x,Det[0].origin.y);
    SfDet[1].setSize(sf::Vector2f(Det[1].width,Det[1].height));
    SfDet[1].setOrigin(Det[1].origin.x,Det[1].origin.y);
    SfDet[2].setSize(sf::Vector2f(Det[1].width,Det[1].height));
    SfDet[2].setOrigin(Det[2].origin.x,Det[2].origin.y);
    for(int i = 0; i<num_obs; i++){
        SfObs[i].setSize(sf::Vector2f(Obs[i].width,Obs[i].height));
        SfObs[i].setFillColor(sf::Color::Green);
        SfObs[i].setPosition(Obs[i].position.x,Obs[i].position.y);
    }

    for(int i = 0; i<3; i++){
        SfDet[i].setFillColor(sf::Color::Blue);
        SfDet[i].setPosition(Det[i].position.x,Det[i].position.y);
    }

    SfDet[2].setFillColor(sf::Color::White);
    /*
    sf::Vector2f SfBodyPos;
    sf::Vector2f SfBodyV(0.0f,0.0f);
    */
    //float Robot.rotation = 0;

    bool collission[3];

    for(int i = 0; i<3; i++){
        collission[i]=false;
    }
    Robot.rotation=100;
    while (window.isOpen()&&window2.isOpen())
    {
        for(int i = 0; i<3; i++){
            collission[i]=false;
        }
        Robot.getPosition(SfBody.getPosition());
        Robot.velocity.y=0.0f;
        Robot.velocity.x=0.0f;
        for(int i = 0; i<num_obs; i++){
            Obs[i].getPosition(SfObs[i].getPosition());
            point UpLeft = VecToPoint(SfObs[i].getTransform().transformPoint(sf::Vector2f(0.f,0.f)));
            point UpRight = VecToPoint(SfObs[i].getTransform().transformPoint(sf::Vector2f(SfObs[i].getSize().x,0.f)));
            point DownRight = VecToPoint(SfObs[i].getTransform().transformPoint(sf::Vector2f(SfObs[i].getSize().x,SfObs[i].getSize().y)));
            point DownLeft = VecToPoint(SfObs[i].getTransform().transformPoint(sf::Vector2f(0.f,SfObs[i].getSize().y)));
            Obs[i].getPoints(UpLeft,UpRight,DownRight,DownLeft);
        }
        
        for(int i = 0; i<3; i++){
            Det[i].getPosition(SfDet[i].getPosition());
            point UpLeft = VecToPoint(SfDet[i].getTransform().transformPoint(sf::Vector2f(0.f,0.f)));
            point UpRight = VecToPoint(SfDet[i].getTransform().transformPoint(sf::Vector2f(SfDet[i].getSize().x,0.f)));
            point DownRight = VecToPoint(SfDet[i].getTransform().transformPoint(sf::Vector2f(SfDet[i].getSize().x,SfDet[i].getSize().y)));
            point DownLeft = VecToPoint(SfDet[i].getTransform().transformPoint(sf::Vector2f(0.f,SfDet[i].getSize().y)));
            Det[i].getPoints(UpLeft,UpRight,DownRight,DownLeft);
            Det[i].setWalls();
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            Robot.rotation-=0.1f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
           Robot.rotation+=0.1f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            Robot.velocity.y=0.3*-cos(Robot.rotation*(PI/180.0));
            Robot.velocity.x=0.3*sin(Robot.rotation*(PI/180.0));
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            Robot.velocity.y=-0.3*-cos(Robot.rotation*(PI/180.0));
            Robot.velocity.x=-0.3*sin(Robot.rotation*(PI/180.0));
        }
        
        for(int l = 0; l<num_obs; l++){
            bool tcollission[3];
            for(int k = 0; k<3; k++){
                for(int i = 0; i<2; i++){
                    int a = i;
                if(k==1){
                    if(i==1){
                        a=3;
                    }
                }
                if(k==2){
                    a++;
                }
                tcollission[k] = true;
                for(int j = 0; j<4; j++){
                    if(det(Obs[l].walls[j], Det[k].points[a])>0){
                        tcollission[k]=false;
                    }
                }
                
                if(tcollission[k]){
                    collission[k]=tcollission[k];
                    break;
                }
            }
        }
        }
        /*
        if(collission[0]||collission[1]||collission[2]){
            SfObs.setFillColor(sf::Color::Yellow);
        }
        else{
            SfObs.setFillColor(sf::Color::Green);
        }
        */
        if(collission[0]){
            line li;
            li.set(Det[0].points[0],Det[0].points[1]);
            lines.push_back(li);
        }
        if(collission[1]){
            line li;
            li.set(Det[1].points[0],Det[1].points[3]);
            lines.push_back(li);
        }
        if(collission[2]){
            line li;
            li.set(Det[2].points[1],Det[2].points[2]);
            lines.push_back(li);
        }

        algorithm.getInput(collission[0],collission[1],collission[2]);
        algorithm.getPos(Robot.rotation,Robot.position.x,Robot.position.y);
        algorithm.setState();
        algorithm.giveCommand();

        Robot.rotation=algorithm.rotate();
        Robot.velocity=algorithm.go();
        //SfObs.setRotation(30);
        SfBody.setRotation(Robot.rotation);
        SfBody.move(Robot.velocity);
        for(int i = 0; i<3; i++){
            SfDet[i].setRotation(Robot.rotation);
            SfDet[i].move(Robot.velocity);
            Det[i].rotation=Robot.rotation;
        }  
        SfObs[6].setRotation(26);
        SfObs[7].setRotation(92);
        SfObs[8].setRotation(246);
        SfObs[9].setRotation(34);
        SfObs[10].setRotation(200);
        SfObs[11].setRotation(130);
        SfObs[12].setRotation(250);
        SfObs[13].setRotation(149);
        SfObs[14].setRotation(349);
        window.clear();
        for(int i = 0; i<3; i++){
            window.draw(SfDet[i]);
        }   
        window.draw(SfBody);
        for(int i = 0; i<num_obs; i++){
            window.draw(SfObs[i]);
        }
        //window.draw(line, 2, sf::Lines);
        window.display();

        window2.clear();
        for(int i = 0; i<3; i++){
            window2.draw(SfDet[i]);
        }   
        window2.draw(SfBody);
        for(int i = 0; i<lines.size(); i++){
            window2.draw(lines[i].points,2,sf::Lines);
        }
        window2.display();
    }

    return 0;
}