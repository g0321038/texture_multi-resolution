/* 
ヘッダファイル*/
#include <stdio.h>
// #include <math.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "texmr.h"
#include <iostream>
/*
マクロ*/

using namespace std;

/*　======
グローバル変数*/
//テクスチャのフォルダ
char TS_folder[] = "./textures/";

//近隣画素群の大きさ
int TS_nbr = 3;

//多重解像度レベル
int Mul_Level = 3;

//入力テクスチャ
//ファイル名
char TS_infile[] = "texture_o_icon.jpg";

//環状かどうか 　0: 環状	1: 環状でない　　
int TS_in_cyc = 0;

//解像度
int TS_in_x;
int TS_in_y;

//出力テクスチャ
//初期化用の乱数の種
unsigned int TS_out_seed = 1;

//解像度
int TS_out_x = 128;
int TS_out_y = 128;

/*
関数宣言*/

/*　====== 
関数*/


/*
メイン関数*/

int main(void)
{
	// ++++ 計算時間の計測
	clock_t time_start, time_end;
	double  time_sec;
	int     time_m;
	double  time_s;

	/* ++++ 計算時間の計測開始 */
	printf("main() ----> start\n");
	time_start = clock();

	/* =======
		入力テクスチャの読み込み*/

	//入力テクスチャ用の配列
	vector<vector<vector<vector<uchar>>>> intex(Mul_Level);

	//入力テクスチャ用のMat型配列
	vector<cv::Mat> intex_cv(Mul_Level);

	//出力テクスチャ用配列
	vector<vector<vector<vector<uchar>>>> outtex(Mul_Level);

	//出力テクスチャ用のMat型配列
	vector<cv::Mat> outtex_cv(Mul_Level);

	//ファイル＋フォルダ名
	char file[512];

	//ファイル名の入力
	strcpy_s(file, sizeof(file), TS_folder);
	strcat_s(file, sizeof(file), TS_infile);

	//ファイルの読み込み
	intex_cv[0] = cv::imread(file);
	//解像度
	TS_in_x = intex_cv[0].cols;
	TS_in_y = intex_cv[0].rows;

	//入力テクスチャの表示
	cv::namedWindow("input_0", cv::WINDOW_AUTOSIZE);
	cv::imshow("input_0", intex_cv[0]);

	
	//情報の出力
	//printf("main() : TS_infile >> %s\n", TS_infile);
	cout << "main() : TS_infile  = " << TS_infile << endl;
	cout << "input_0 = " << intex_cv[0].size() << endl;

	//多重解像度のテクスチャの配列の領域確保
	intex[0].resize(int(TS_in_y),
		vector<vector<uchar>>(int(TS_in_x), vector<uchar>(3)));

	outtex[0].resize(int(TS_out_y),
		vector<vector<uchar>>(int(TS_out_x), vector<uchar>(3)));
	for (int i = 1; i < Mul_Level; i++)
	{
		intex[i].resize(int(TS_in_y / (2 * i)),
						vector<vector<uchar>>(int(TS_in_x / (2 * i)), vector<uchar>(3)));
		outtex[i].resize(int(TS_out_y / (2 * i)),
						vector<vector<uchar>>(int(TS_out_x / (2 * i)), vector<uchar>(3)));
	}

	//テクスチャの低解像度化
	for (int i = 0; i < Mul_Level - 1; i++)
	{
		int j = i + 1;
		//i+1の解像度のMat型の領域確保
		intex_cv[j].create(intex[j].size(), intex[j][0].size(), CV_8UC3);
		//画像をリサイズして保存
		cv::resize(intex_cv[i], intex_cv[j], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
		
		//多重解像度画像の表示
		char texNUM[32];
		sprintf_s(texNUM, "input_%d", j);
		cv::namedWindow(texNUM, cv::WINDOW_AUTOSIZE);
		cv::imshow(texNUM, intex_cv[j]);
		//情報の出力
		cout << "input_" << j << " = " << intex_cv[j].size() << endl;
	}

	//入力テクスチャを各解像度ごとに配列へコピー
	for (int i = 0; i < Mul_Level; i++) {
		for (int y = 0; y < intex[i].size(); y++) {
			for(int x = 0; x < intex[i][y].size(); x++) {
				for (int c = 0; c < 3; c++) {
					intex[i][y][x][c] = intex_cv[i].at<cv::Vec3b>(y, x)[c];
				}
			}
		}
	}
	
	/*とりあえず繰り返しコピーで一枚のみ出力
	//出力テクスチャのMat型配列の領域確保
	outtex_cv[0].create(TS_out_y, TS_out_x, CV_8UC3);

	//出力テクスチャ配列に入力テクスチャ配列の繰り返しをコピー
	for (int y = 0; y < TS_out_y; y++) {
		for (int x = 0; x < TS_out_x; x++) {
			for (int c = 0; c < 3; c++) {
				outtex[0][y][x][c] = intex[0][y % TS_in_y][x % TS_in_x][c];
			}
		}
	}

	//出力テクスチャのMat型配列に出力テクスチャの配列をコピー
	for (int y = 0; y < TS_out_y; y++) {
		for (int x = 0; x < TS_out_x; x++) {
			for (int c = 0; c < 3; c++) {
				outtex_cv[0].at<cv::Vec3b>(y, x)[c] = outtex[0][y][x][c];
			}
		}
	}
	
	//出力テクスチャのコピー
	cv::imshow("outtex_0", outtex_cv[0]);*/


	/*======
	多重解像度処理アルゴリズム*/

	outtex_cv = multi_resolution(
		TS_nbr,
		TS_in_cyc,
		TS_out_seed,
		Mul_Level,
		intex,
		outtex
	);

	/*=====
	出力テクスチャの出力*/
	
	for (int i = 0; i < Mul_Level; i++) {
		//多重解像度の出力画像の表示
		char texNUM[32];
		sprintf_s(texNUM, "output_%d", i);
		cv::namedWindow(texNUM, cv::WINDOW_AUTOSIZE);
		cv::imshow(texNUM, outtex_cv[i]);

		//多重解像度の出力画像をファイルに保存
		char texFileName[32];
		sprintf_s(texFileName, "output_%d.jpg", i);
		strcpy_s(file, sizeof(file), TS_folder);
		strcat_s(file, sizeof(file), texFileName);
		cv::imwrite(file, outtex_cv[i]);

		//情報出力
		cout << "outtex_" << i << " = " << outtex_cv[i].size() << endl;
		cout << "main(): texFileName = " << texFileName << endl;
	}

	/*=====
	終了処理*/

	/* ++++ 計算時間の計測終了*/
	time_end = clock();
	time_sec = (double)(time_end - time_start) / (double)CLOCKS_PER_SEC;
	time_m = (int)(time_sec / 60.0);
	time_s = time_sec - (double)time_m * 60.0;
	printf("main() : time >> %lf sec. = %d min. %lf sec.\n", time_sec, time_m, time_s);
	printf("main() <---- end\n");

	//キー入力待ち
	cv::waitKey(0);

	//正常終了
	return 0;
}

// === EOF