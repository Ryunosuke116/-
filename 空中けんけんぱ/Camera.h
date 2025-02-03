#pragma once
#include"player.h"

class Camera
{
private:
    VECTOR Aimpos;
    VECTOR LookPos;
    float a;
    bool flg;
    bool isDistance;
public:

    Camera();
    ~Camera();

    void Initilize();
    void Update(Player& player);
    void TitleUpdate();
};

