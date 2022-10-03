struct point{
    float x;
    float y;
};

struct Wall{
    point p1;
    point p2;
};

class Rectangle{
public:
    float height;
    float width;
    float rotation;
    point origin;
    point points[4];
    sf::Vector2f position;
    sf::Vector2f velocity;
    Wall walls[4];
    void setRectangle(float w, float h, float Ox, float Oy, float Px, float Py);
    void getPosition(sf::Vector2f pos);
    void getPoints(point p1,point p2,point p3,point p4);
    void setWalls();

};

class Controler
{

    bool col_1;
    bool col_2;
    bool col_3;

    int adjust = 0;

    float rotation;
    float x;
    float y;
    std::vector<float> fx[2];
    std::vector<float> fy[2];

    int delay=100;

    int prev;

    float spiral = 1.0;

    sf::Vector2f move;

    int state = 0;

    float accuracy = 5;

    float delaySpiral = 0;

public:
    void getInput(bool det1, bool det2, bool det3);
    void getPos(float rot, float posx, float posy);
    void setState();
    void giveCommand();
    
    void search();
    void findNext();
    void goAlong();

    sf::Vector2f go();
    float rotate();

    //void alongWall();
};
