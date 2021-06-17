/*　======
ヘッダファイル*/

#include <stdio.h>
// #include <math.h>
#include <stdlib.h>
#include <time.h>
//#include <opencv2/opencv.hpp>
#include "texmr.h"
#include <iostream>
using namespace std;

/* ======
関数*/

/*多重解像度化処理のアルゴリズム*/
vector<cv::Mat> multi_resolution(
	int nbr,
	int in_cyc,
	unsigned int out_seed,
	int Mul_Level,
	vector<vector<vector<vector<uchar>>>> intex,
	vector<vector<vector<vector<uchar>>>> outtex
)
{
	int count = 0;

	/* =====
	初期化処理*/

	//各解像度における解像度
	vector<int> in_x(Mul_Level, 0);
	vector<int> in_y(Mul_Level, 0);
	vector<int> out_x(Mul_Level, 0);
	vector<int> out_y(Mul_Level, 0);

	for (int i = 0; i < Mul_Level; i++) {
		in_x[i] = intex[i][0].size();
		in_y[i] = intex[i].size();
		out_x[i] = outtex[i][0].size();
		out_y[i] = outtex[i].size();

		cout << "in_x[" << i << "] = " << in_x[i] << endl;
		cout << "in_y[" << i << "] = " << in_y[i] << endl;
		cout << "out_x[" << i << "] = " << out_x[i] << endl;
		cout << "out_y[" << i << "] = " << out_y[i] << endl;
	}


	
	//入力テクスチャの探索範囲
	vector<int>in_x_min(Mul_Level, 0), in_x_max(Mul_Level, 0),
				in_y_min(Mul_Level, 0), in_y_max(Mul_Level, 0);
	
	for (int i = 0; i < Mul_Level; i++) {
		if (in_cyc == 1) { //環状でない
			in_x_min[i] = nbr;
			in_x_max[i] = in_x[i] - nbr;
			in_y_min[i] = nbr;
			in_y_max[i] = in_y[i];
		}
		else {				//環状である
			in_x_min[i] = 0;
			in_x_max[i] = in_x[i];
			in_y_min[i] = 0;
			in_y_max[i] = in_y[i];
		}
	}

	//出力テクスチャの初期化
	srand(out_seed);
	for (int i = 0; i < Mul_Level; i++) {
		for (int oy = 0; oy < out_y[i]; oy++) {
			for (int ox = 0; ox < out_x[i]; ox++) {
				for (int c = 0; c < 3; c++) {
					outtex[i][oy][ox][c] = intex[i][rand() % in_y[i]][rand() % in_x[i]][c];
				}
			}
		}
	}

	

	//L型近隣画素軍中の画素数
	int pxn = ((2 * nbr + 1) * (2 * nbr + 1)) / 2;
	
	//情報出力
	cout << "nbr = " << nbr << ", pxn = " << pxn << ", Mul_Level = " << Mul_Level << endl;

	//最大SSDの値
	int ssd_max = 3 * 255 * 255 * pxn;


	//最低解像度の出力テクスチャを全探索アルゴリズムで合成
	int Level_min = Mul_Level - 1; //配列における最低解像度解像度のレベル

	cout << "初期化処理完了" << endl;

	//出力テクスチャの画素位置　のループ
	for (int oy = 0; oy < out_y[Level_min]; oy++) {
		for (int ox = 0; ox < out_x[Level_min]; ox++) {
			
			//最小SSDの初期化
			int ssd_min = ssd_max;

			//入力テクスチャの選択画素位置の初期化
			int ix_s = -1;
			int iy_s = -1;

			//入力テクスチャ状の探索
			for (int iy = in_y_min[Level_min]; iy < in_y_max[Level_min]; iy++) {
				for (int ix = in_x_min[Level_min]; ix < in_x_max[Level_min]; ix++) {

					//近隣画素軍のSSDの値の計算
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny < 0; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {
							
							//近隣画素軍の中心に来たら計算終了
							if ((ny == 0) && (nx == 0)) {
								break;
							}
							for (int c = 0; c < 3; c++) {

								//計算
								s = (int)outtex[Level_min][(oy + ny + out_y[Level_min]) % out_y[Level_min]][(ox + nx + out_x[Level_min]) % out_x[Level_min]][c]
									- (int)intex[Level_min][(iy + ny + in_y[Level_min]) % in_y[Level_min]][(ix + nx + in_x[Level_min]) % in_x[Level_min]][c];
								ssd += (s * s);
								
							}
						}//nx
					}//ny

					//最小SSDの値と入力テクスチャの選択画素の更新
					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ix;
						iy_s = iy;
					}
					//cout << "ix = " << ix << endl;
				}//ix
				//cout << "iy = " << iy << endl;
			}//iy

			//出力テクスチャの画素色の決定
			if (ssd_min == ssd_max) {
				for (int c = 0; c < 3; c++) {
					outtex[Level_min][oy][ox][c] = 0;
				}
			}
			else {
				for (int c = 0; c < 3; c++) {
					outtex[Level_min][oy][ox][c] = intex[Level_min][iy_s][ix_s][c];
					
				}
			}
			
			//cout << "全探索アルゴリズム　oxのループ　ox = " << ox << endl;
		}//ox
		//cout << "全探索アルゴリズム　oyのループ　oy = " << oy <<endl;
	}//oy

	cout << "最低解像度の全探索アルゴリズム完了" << endl;

	//最低解像度以外の出力テクスチャをより低い解像度レベルまでもちいて合成
	for (int l = Level_min - 1; l >= 0; l--) {
		for (int oy = 0; oy < out_y[l]; oy++) {
			for (int ox = 0; ox < out_x[l]; ox++) {
				
				//１レベル前の近隣について
				int nbr_p = nbr - 1;
				int pxn_p = ((2 * nbr_p + 1) * (2 * nbr_p + 1)) / 2;

				//現在のレベルで探索している画素位置に対応する1レベル前の画素位置
				int ox_p = ox * 0.5;
				int oy_p = oy * 0.5;

				//最小SSDの初期化
				int ssd_min = ssd_max;

				//入力テクスチャの選択画素の初期化
				int ix_s = -1;
				int iy_s = -1;
				int Level;

				//1レベル前のLevel
				int Level_p = l + 1;

				//cout << "oy = " << oy << " ox = " << ox << endl;

				//1レベル前の入力テクスチャ上の探索
				for (int iy = in_y_min[Level_p]; iy < in_y_max[Level_p]; iy++) {
					for (int ix = in_x_min[Level_p]; ix < in_x_max[Level_p]; ix++) {

						//cout << "iy = " << iy << " ix = " << ix << endl;

						//近隣画素群SSDの値の計算
						int s;
						int ssd = 0;

						//正方領域近隣
						for (int ny = (-nbr_p); ny <= nbr_p; ny++) {
							for (int nx = (-nbr_p); nx <= nbr_p; nx++) {
								//計算
								for (int c = 0; c < 3; c++) {
									s = (int)outtex[Level_p][(oy_p + ny + out_y[Level_p]) % out_y[Level_p]][(ox_p + nx + out_x[Level_p]) % out_x[Level_p]][c]
										- (int)intex[Level_p][(iy + ny + in_y[Level_p]) % in_y[Level_p]][(ix + nx + in_x[Level_p]) % in_x[Level_p]][c];

									ssd += (s * s);
								}
								
							}//nx
						}//ny

						if (ssd_min > ssd) {
							ssd_min = ssd;
							ix_s = ix;
							iy_s = iy;
							Level = Level_p;

							//cout << "１つ前ssdの更新" << "Level = " << Level << endl;
						}
					}//ix
				}//iy
				/*cout << "iy_s = " << iy_s << " ix_s = " << ix_s << " Level = " << Level << endl;
				cout << "１レベル前の入力テクスチャでの探索完了" << "ox = " << ox << " oy = "<< oy << endl;*/

				//現在のレベルの入力テクスチャ上の探索
				for (int iy = in_y_min[l]; iy < in_y_max[l]; iy++) {
					for (int ix = in_x_min[l]; ix < in_x_max[l]; ix++) {

						//cout << "iy = " << iy << " ix = " << ix << endl;

						//近隣画素群のSSDの値の計算
						int s;
						int ssd = 0;

						//L型領域近隣
						for (int ny = (-nbr); ny <= 0; ny++) {
							for (int nx = (-nbr); nx <= nbr; nx++) {

								//中心に来たら終了
								if ((ny == 0) && (nx == 0)) {
									break;
								}

								//計算
								for (int c = 0; c < 3; c++) {
									s = (int)outtex[l][(oy + ny + out_y[l]) % out_y[l]][(ox + nx + out_x[l]) % out_x[l]][c]
										- (int)intex[l][(iy + ny + in_y[l]) % in_y[l]][(ix + nx + in_x[l]) % in_x[l]][c];

									ssd += (s * s);
								}//c
							}//nx
						}//ny

						//最小SSDの値と入力テクスチャの選択画素の更新
						if (ssd_min > ssd) {
							ssd_min = ssd;
							ix_s = ix;
							iy_s = iy;
							Level = l;

						}
					}//iy
				}//ix


				//cout <<"現在のレベルの入力テクスチャでの探索完了" << "ox = " << ox << " oy = " << oy << endl;
				//cout << "iy_s = " << iy_s << " ix_s = " << ix_s << " Level = "  << Level <<endl;
				//出力テクスチャの画素色の決定
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {

						outtex[l][oy][ox][c] = 0;
						//cout << "入力テクスチャの画素が選択されていない" << endl;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						outtex[l][oy][ox][c] = intex[Level][iy_s][ix_s][c];
						
						

 					}
				}//endif
				//cout << "多重解像度処理内　ox(出力テクスチャの画素位置 x)のループ　ox = " << ox << endl;
			} //ox
			//cout << "多重解像度処理内　oy(出力テクスチャの画素位置 y)のループ　oy = " << oy << endl;
		} //oy
		//cout << "多重解像度処理内　Lのループ　L =" << l << endl;
	} //l

	cout << "終了処理直前" << endl;
	/*=====
	終了処理*/

	//出力テクスチャのMat型配列の領域確保
	vector<cv::Mat>outtex_cv(Mul_Level);
	for (int i = 0; i < Mul_Level; i++) {
		outtex_cv[i].create(outtex[i].size(), outtex[i][0].size(), CV_8UC3);
	}

	//配列からコピー
	for (int i = 0; i < Mul_Level; i++) {
		for (int y = 0; y < outtex[i].size(); y++) {
			for (int x = 0; x < outtex[i][y].size(); x++) {
				for (int c = 0; c < 3; c++) {
					outtex_cv[i].at<cv::Vec3b>(y, x)[c] = outtex[i][y][x][c];
				}
			}
		}
	}

	return outtex_cv;
}