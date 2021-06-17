#include <vector>
#include <opencv2/opencv.hpp>


#ifndef _TEXMR_H
#define _TEXMR_H

/* ========
   �}�N�� */
using namespace std;

   // ���̓e�N�X�`���̍ő�𑜓x
#define TS_IN_XMAX 100
#define TS_IN_YMAX 100

// �o�̓e�N�X�`���̍ő�𑜓x
#define TS_OUT_XMAX 400
#define TS_OUT_YMAX 400

//���d�𑜓x�̍ő僌�x��
#define MUL_LEVEL_MAX 10


/* ========
   �֐��錾 */

/*
�@���d�𑜓x�����A���S���Y��*/
vector<cv::Mat> multi_resolution(
    int nbr,                                                            //�ߗ׉�f�̑傫���ineighbor�j nbr >= 1
    int in_cyc,                                                         //���̓e�N�X�`�����󉻂ǂ����@0:��@1:��ł͂Ȃ�
    unsigned int out_seed,                                              //�o�̓e�N�X�`���̏������̂��߂̗����̎�
    int Mul_Level,													  //���d�𑜓x���x��	
    std::vector<std::vector<std::vector<std::vector<uchar>>>> intex,    // ���̓e�N�X�`��
    std::vector<std::vector<std::vector<std::vector<uchar>>>> outtex    // �o�̓e�N�X�`��
 );

#endif

// ==== EOF