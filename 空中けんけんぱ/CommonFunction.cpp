#include "DxLib.h"
#include<vector>
#include "CommonFunction.h"



void CommonFunction::SetSituation(int num)
{
    situation = num;
}

void CommonFunction::Setcol(int num)
{
    col = num;
}

void CommonFunction::TimerSet(int& startTime)
{
    startTime = GetNowCount();
}