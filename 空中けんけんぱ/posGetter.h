#pragma once
class PosGetter
{
public:
	int GetSituation() { return situation; }
	void SetSituation(int num);
	int GetCol() { return col; }
	void Setcol(int num);
private:
	int situation;
	int col;
};

