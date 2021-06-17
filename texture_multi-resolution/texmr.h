#include <vector>
#include <opencv2/opencv.hpp>


#ifndef _TEXMR_H
#define _TEXMR_H

/* ========
   マクロ */
using namespace std;

   // 入力テクスチャの最大解像度
#define TS_IN_XMAX 100
#define TS_IN_YMAX 100

// 出力テクスチャの最大解像度
#define TS_OUT_XMAX 400
#define TS_OUT_YMAX 400

//多重解像度の最大レベル
#define MUL_LEVEL_MAX 10


/* ========
   関数宣言 */

/*
　多重解像度処理アルゴリズム*/
vector<cv::Mat> multi_resolution(
    int nbr,                                                            //近隣画素の大きさ（neighbor） nbr >= 1
    int in_cyc,                                                         //入力テクスチャが環状化どうか　0:環状　1:環状ではない
    unsigned int out_seed,                                              //出力テクスチャの初期化のための乱数の種
    int Mul_Level,													  //多重解像度レベル	
    std::vector<std::vector<std::vector<std::vector<uchar>>>> intex,    // 入力テクスチャ
    std::vector<std::vector<std::vector<std::vector<uchar>>>> outtex    // 出力テクスチャ
 );

#endif

// ==== EOF