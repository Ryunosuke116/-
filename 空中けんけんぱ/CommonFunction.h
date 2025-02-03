#pragma once
class CommonFunction
{
public:
	int GetSituation() { return situation; }
	int GetCol() { return col; }
	void SetSituation(int num);
	void Setcol(int num);
	void TimerSet(int& startTime);
private:
	int situation;
	int col;
};

