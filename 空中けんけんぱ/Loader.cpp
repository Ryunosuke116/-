#include"Dxlib.h"
#include"Windows.h"
#include "Loader.h"


bool Loader::mapLayerLoader(int** &mapData, int &mapXNum, int& mapYNum, const char* mapCSVFileName)
{
	// dst初期化
	mapData = NULL;
	mapXNum = 0;
	mapYNum = 0;

	// ファイルを開く
	FILE* fp;
	errno_t err = fopen_s(&fp, mapCSVFileName, "rt");;
	if (!fp)
	{
		printf("ファイルを開けません");
		return 0;
	}

	// ファイルを１行分空読みし、１行のデータ数を数える
	char c = 0;
	while (c != '\n')
	{
		// 読み取った文字がカンマなら1つカウント増やす
		c = fgetc(fp);
		if (',' == c)
		{
			mapXNum++;
		}
	}
	mapXNum++;

	//１データ 16bit最大値 "65535,"(6文字)として文字列入ってきたときの１行サイズ
	int bufferLen = mapXNum * 6 + 1;

	//１行分のバッファ確保
	char* buf = new char[bufferLen];
	char* ptmp = buf;

	//ファイルポインタを先頭に戻し行数を読む
	fseek(fp, 0, SEEK_SET);
	bool firstRow = true;
	while (fgets(buf, bufferLen - 1, fp) != NULL)
	{
		mapYNum++;
	}

	//2次元配列を動的確保する map[row][col] として動的確保する
	mapData = new int* [mapYNum];

	int ix, iy;
	for (iy = 0; iy < mapYNum; iy++)
	{
		mapData[iy] = new int[mapXNum];
	}

	//ファイルポインタを先頭に戻す
	fseek(fp, 0, SEEK_SET);
	// ファイルからデータを配列に移す
	char* readPoint;
	char* firstPoint;
	for (iy = 0; iy < mapYNum; iy++)
	{
		// 1行取り込み
		fgets(buf, bufferLen - 1, fp);
		readPoint = firstPoint = buf;

		for (ix = 0; ix < mapXNum; ix++)
		{
			//カンマまでreadPointの位置を進める
			while (',' != *readPoint && '\n' != *readPoint)
			{
				readPoint++;
			}
			// カンマをNULL文字に置き換え
			*readPoint = '\0';
			//この時点でfirstPoint - readPoint間で文字列が完成するので数値に変換
			mapData[iy][ix] = atoi(firstPoint);

			//次のデータ読み取り位置まで進める
			readPoint++;
			firstPoint = readPoint;
		}
	}

	// ファイルを閉じる
	fclose(fp);

	// バッファ開放
	delete[] buf;

	return true;
}