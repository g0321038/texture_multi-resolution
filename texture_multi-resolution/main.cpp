/* 
�w�b�_�t�@�C��*/
#include <stdio.h>
// #include <math.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/opencv.hpp>
#include "texmr.h"
#include <iostream>
/*
�}�N��*/

using namespace std;

/*�@======
�O���[�o���ϐ�*/
//�e�N�X�`���̃t�H���_
char TS_folder[] = "./textures/";

//�ߗ׉�f�Q�̑傫��
int TS_nbr = 3;

//���d�𑜓x���x��
int Mul_Level = 3;

//���̓e�N�X�`��
//�t�@�C����
char TS_infile[] = "texture_o_icon.jpg";

//�󂩂ǂ��� �@0: ��	1: ��łȂ��@�@
int TS_in_cyc = 0;

//�𑜓x
int TS_in_x;
int TS_in_y;

//�o�̓e�N�X�`��
//�������p�̗����̎�
unsigned int TS_out_seed = 1;

//�𑜓x
int TS_out_x = 128;
int TS_out_y = 128;

/*
�֐��錾*/

/*�@====== 
�֐�*/


/*
���C���֐�*/

int main(void)
{
	// ++++ �v�Z���Ԃ̌v��
	clock_t time_start, time_end;
	double  time_sec;
	int     time_m;
	double  time_s;

	/* ++++ �v�Z���Ԃ̌v���J�n */
	printf("main() ----> start\n");
	time_start = clock();

	/* =======
		���̓e�N�X�`���̓ǂݍ���*/

	//���̓e�N�X�`���p�̔z��
	vector<vector<vector<vector<uchar>>>> intex(Mul_Level);

	//���̓e�N�X�`���p��Mat�^�z��
	vector<cv::Mat> intex_cv(Mul_Level);

	//�o�̓e�N�X�`���p�z��
	vector<vector<vector<vector<uchar>>>> outtex(Mul_Level);

	//�o�̓e�N�X�`���p��Mat�^�z��
	vector<cv::Mat> outtex_cv(Mul_Level);

	//�t�@�C���{�t�H���_��
	char file[512];

	//�t�@�C�����̓���
	strcpy_s(file, sizeof(file), TS_folder);
	strcat_s(file, sizeof(file), TS_infile);

	//�t�@�C���̓ǂݍ���
	intex_cv[0] = cv::imread(file);
	//�𑜓x
	TS_in_x = intex_cv[0].cols;
	TS_in_y = intex_cv[0].rows;

	//���̓e�N�X�`���̕\��
	cv::namedWindow("input_0", cv::WINDOW_AUTOSIZE);
	cv::imshow("input_0", intex_cv[0]);

	
	//���̏o��
	//printf("main() : TS_infile >> %s\n", TS_infile);
	cout << "main() : TS_infile  = " << TS_infile << endl;
	cout << "input_0 = " << intex_cv[0].size() << endl;

	//���d�𑜓x�̃e�N�X�`���̔z��̗̈�m��
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

	//�e�N�X�`���̒�𑜓x��
	for (int i = 0; i < Mul_Level - 1; i++)
	{
		int j = i + 1;
		//i+1�̉𑜓x��Mat�^�̗̈�m��
		intex_cv[j].create(intex[j].size(), intex[j][0].size(), CV_8UC3);
		//�摜�����T�C�Y���ĕۑ�
		cv::resize(intex_cv[i], intex_cv[j], cv::Size(), 0.5, 0.5, cv::INTER_NEAREST);
		
		//���d�𑜓x�摜�̕\��
		char texNUM[32];
		sprintf_s(texNUM, "input_%d", j);
		cv::namedWindow(texNUM, cv::WINDOW_AUTOSIZE);
		cv::imshow(texNUM, intex_cv[j]);
		//���̏o��
		cout << "input_" << j << " = " << intex_cv[j].size() << endl;
	}

	//���̓e�N�X�`�����e�𑜓x���Ƃɔz��փR�s�[
	for (int i = 0; i < Mul_Level; i++) {
		for (int y = 0; y < intex[i].size(); y++) {
			for(int x = 0; x < intex[i][y].size(); x++) {
				for (int c = 0; c < 3; c++) {
					intex[i][y][x][c] = intex_cv[i].at<cv::Vec3b>(y, x)[c];
				}
			}
		}
	}
	
	/*�Ƃ肠�����J��Ԃ��R�s�[�ňꖇ�̂ݏo��
	//�o�̓e�N�X�`����Mat�^�z��̗̈�m��
	outtex_cv[0].create(TS_out_y, TS_out_x, CV_8UC3);

	//�o�̓e�N�X�`���z��ɓ��̓e�N�X�`���z��̌J��Ԃ����R�s�[
	for (int y = 0; y < TS_out_y; y++) {
		for (int x = 0; x < TS_out_x; x++) {
			for (int c = 0; c < 3; c++) {
				outtex[0][y][x][c] = intex[0][y % TS_in_y][x % TS_in_x][c];
			}
		}
	}

	//�o�̓e�N�X�`����Mat�^�z��ɏo�̓e�N�X�`���̔z����R�s�[
	for (int y = 0; y < TS_out_y; y++) {
		for (int x = 0; x < TS_out_x; x++) {
			for (int c = 0; c < 3; c++) {
				outtex_cv[0].at<cv::Vec3b>(y, x)[c] = outtex[0][y][x][c];
			}
		}
	}
	
	//�o�̓e�N�X�`���̃R�s�[
	cv::imshow("outtex_0", outtex_cv[0]);*/


	/*======
	���d�𑜓x�����A���S���Y��*/

	outtex_cv = multi_resolution(
		TS_nbr,
		TS_in_cyc,
		TS_out_seed,
		Mul_Level,
		intex,
		outtex
	);

	/*=====
	�o�̓e�N�X�`���̏o��*/
	
	for (int i = 0; i < Mul_Level; i++) {
		//���d�𑜓x�̏o�͉摜�̕\��
		char texNUM[32];
		sprintf_s(texNUM, "output_%d", i);
		cv::namedWindow(texNUM, cv::WINDOW_AUTOSIZE);
		cv::imshow(texNUM, outtex_cv[i]);

		//���d�𑜓x�̏o�͉摜���t�@�C���ɕۑ�
		char texFileName[32];
		sprintf_s(texFileName, "output_%d.jpg", i);
		strcpy_s(file, sizeof(file), TS_folder);
		strcat_s(file, sizeof(file), texFileName);
		cv::imwrite(file, outtex_cv[i]);

		//���o��
		cout << "outtex_" << i << " = " << outtex_cv[i].size() << endl;
		cout << "main(): texFileName = " << texFileName << endl;
	}

	/*=====
	�I������*/

	/* ++++ �v�Z���Ԃ̌v���I��*/
	time_end = clock();
	time_sec = (double)(time_end - time_start) / (double)CLOCKS_PER_SEC;
	time_m = (int)(time_sec / 60.0);
	time_s = time_sec - (double)time_m * 60.0;
	printf("main() : time >> %lf sec. = %d min. %lf sec.\n", time_sec, time_m, time_s);
	printf("main() <---- end\n");

	//�L�[���͑҂�
	cv::waitKey(0);

	//����I��
	return 0;
}

// === EOF