/*�@======
�w�b�_�t�@�C��*/

#include <stdio.h>
// #include <math.h>
#include <stdlib.h>
#include <time.h>
//#include <opencv2/opencv.hpp>
#include "texmr.h"
#include <iostream>
using namespace std;

/* ======
�֐�*/

/*���d�𑜓x�������̃A���S���Y��*/
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
	����������*/

	//�e�𑜓x�ɂ�����𑜓x
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


	
	//���̓e�N�X�`���̒T���͈�
	vector<int>in_x_min(Mul_Level, 0), in_x_max(Mul_Level, 0),
				in_y_min(Mul_Level, 0), in_y_max(Mul_Level, 0);
	
	for (int i = 0; i < Mul_Level; i++) {
		if (in_cyc == 1) { //��łȂ�
			in_x_min[i] = nbr;
			in_x_max[i] = in_x[i] - nbr;
			in_y_min[i] = nbr;
			in_y_max[i] = in_y[i];
		}
		else {				//��ł���
			in_x_min[i] = 0;
			in_x_max[i] = in_x[i];
			in_y_min[i] = 0;
			in_y_max[i] = in_y[i];
		}
	}

	//�o�̓e�N�X�`���̏�����
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

	

	//L�^�ߗ׉�f�R���̉�f��
	int pxn = ((2 * nbr + 1) * (2 * nbr + 1)) / 2;
	
	//���o��
	cout << "nbr = " << nbr << ", pxn = " << pxn << ", Mul_Level = " << Mul_Level << endl;

	//�ő�SSD�̒l
	int ssd_max = 3 * 255 * 255 * pxn;


	//�Œ�𑜓x�̏o�̓e�N�X�`����S�T���A���S���Y���ō���
	int Level_min = Mul_Level - 1; //�z��ɂ�����Œ�𑜓x�𑜓x�̃��x��

	cout << "��������������" << endl;

	//�o�̓e�N�X�`���̉�f�ʒu�@�̃��[�v
	for (int oy = 0; oy < out_y[Level_min]; oy++) {
		for (int ox = 0; ox < out_x[Level_min]; ox++) {
			
			//�ŏ�SSD�̏�����
			int ssd_min = ssd_max;

			//���̓e�N�X�`���̑I����f�ʒu�̏�����
			int ix_s = -1;
			int iy_s = -1;

			//���̓e�N�X�`����̒T��
			for (int iy = in_y_min[Level_min]; iy < in_y_max[Level_min]; iy++) {
				for (int ix = in_x_min[Level_min]; ix < in_x_max[Level_min]; ix++) {

					//�ߗ׉�f�R��SSD�̒l�̌v�Z
					int s;
					int ssd = 0;

					for (int ny = (-nbr); ny < 0; ny++) {
						for (int nx = (-nbr); nx <= nbr; nx++) {
							
							//�ߗ׉�f�R�̒��S�ɗ�����v�Z�I��
							if ((ny == 0) && (nx == 0)) {
								break;
							}
							for (int c = 0; c < 3; c++) {

								//�v�Z
								s = (int)outtex[Level_min][(oy + ny + out_y[Level_min]) % out_y[Level_min]][(ox + nx + out_x[Level_min]) % out_x[Level_min]][c]
									- (int)intex[Level_min][(iy + ny + in_y[Level_min]) % in_y[Level_min]][(ix + nx + in_x[Level_min]) % in_x[Level_min]][c];
								ssd += (s * s);
								
							}
						}//nx
					}//ny

					//�ŏ�SSD�̒l�Ɠ��̓e�N�X�`���̑I����f�̍X�V
					if (ssd_min > ssd) {
						ssd_min = ssd;
						ix_s = ix;
						iy_s = iy;
					}
					//cout << "ix = " << ix << endl;
				}//ix
				//cout << "iy = " << iy << endl;
			}//iy

			//�o�̓e�N�X�`���̉�f�F�̌���
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
			
			//cout << "�S�T���A���S���Y���@ox�̃��[�v�@ox = " << ox << endl;
		}//ox
		//cout << "�S�T���A���S���Y���@oy�̃��[�v�@oy = " << oy <<endl;
	}//oy

	cout << "�Œ�𑜓x�̑S�T���A���S���Y������" << endl;

	//�Œ�𑜓x�ȊO�̏o�̓e�N�X�`�������Ⴂ�𑜓x���x���܂ł������č���
	for (int l = Level_min - 1; l >= 0; l--) {
		for (int oy = 0; oy < out_y[l]; oy++) {
			for (int ox = 0; ox < out_x[l]; ox++) {
				
				//�P���x���O�̋ߗׂɂ���
				int nbr_p = nbr - 1;
				int pxn_p = ((2 * nbr_p + 1) * (2 * nbr_p + 1)) / 2;

				//���݂̃��x���ŒT�����Ă����f�ʒu�ɑΉ�����1���x���O�̉�f�ʒu
				int ox_p = ox * 0.5;
				int oy_p = oy * 0.5;

				//�ŏ�SSD�̏�����
				int ssd_min = ssd_max;

				//���̓e�N�X�`���̑I����f�̏�����
				int ix_s = -1;
				int iy_s = -1;
				int Level;

				//1���x���O��Level
				int Level_p = l + 1;

				//cout << "oy = " << oy << " ox = " << ox << endl;

				//1���x���O�̓��̓e�N�X�`����̒T��
				for (int iy = in_y_min[Level_p]; iy < in_y_max[Level_p]; iy++) {
					for (int ix = in_x_min[Level_p]; ix < in_x_max[Level_p]; ix++) {

						//cout << "iy = " << iy << " ix = " << ix << endl;

						//�ߗ׉�f�QSSD�̒l�̌v�Z
						int s;
						int ssd = 0;

						//�����̈�ߗ�
						for (int ny = (-nbr_p); ny <= nbr_p; ny++) {
							for (int nx = (-nbr_p); nx <= nbr_p; nx++) {
								//�v�Z
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

							//cout << "�P�Ossd�̍X�V" << "Level = " << Level << endl;
						}
					}//ix
				}//iy
				/*cout << "iy_s = " << iy_s << " ix_s = " << ix_s << " Level = " << Level << endl;
				cout << "�P���x���O�̓��̓e�N�X�`���ł̒T������" << "ox = " << ox << " oy = "<< oy << endl;*/

				//���݂̃��x���̓��̓e�N�X�`����̒T��
				for (int iy = in_y_min[l]; iy < in_y_max[l]; iy++) {
					for (int ix = in_x_min[l]; ix < in_x_max[l]; ix++) {

						//cout << "iy = " << iy << " ix = " << ix << endl;

						//�ߗ׉�f�Q��SSD�̒l�̌v�Z
						int s;
						int ssd = 0;

						//L�^�̈�ߗ�
						for (int ny = (-nbr); ny <= 0; ny++) {
							for (int nx = (-nbr); nx <= nbr; nx++) {

								//���S�ɗ�����I��
								if ((ny == 0) && (nx == 0)) {
									break;
								}

								//�v�Z
								for (int c = 0; c < 3; c++) {
									s = (int)outtex[l][(oy + ny + out_y[l]) % out_y[l]][(ox + nx + out_x[l]) % out_x[l]][c]
										- (int)intex[l][(iy + ny + in_y[l]) % in_y[l]][(ix + nx + in_x[l]) % in_x[l]][c];

									ssd += (s * s);
								}//c
							}//nx
						}//ny

						//�ŏ�SSD�̒l�Ɠ��̓e�N�X�`���̑I����f�̍X�V
						if (ssd_min > ssd) {
							ssd_min = ssd;
							ix_s = ix;
							iy_s = iy;
							Level = l;

						}
					}//iy
				}//ix


				//cout <<"���݂̃��x���̓��̓e�N�X�`���ł̒T������" << "ox = " << ox << " oy = " << oy << endl;
				//cout << "iy_s = " << iy_s << " ix_s = " << ix_s << " Level = "  << Level <<endl;
				//�o�̓e�N�X�`���̉�f�F�̌���
				if (ssd_min == ssd_max) {
					for (int c = 0; c < 3; c++) {

						outtex[l][oy][ox][c] = 0;
						//cout << "���̓e�N�X�`���̉�f���I������Ă��Ȃ�" << endl;
					}
				}
				else {
					for (int c = 0; c < 3; c++) {
						outtex[l][oy][ox][c] = intex[Level][iy_s][ix_s][c];
						
						

 					}
				}//endif
				//cout << "���d�𑜓x�������@ox(�o�̓e�N�X�`���̉�f�ʒu x)�̃��[�v�@ox = " << ox << endl;
			} //ox
			//cout << "���d�𑜓x�������@oy(�o�̓e�N�X�`���̉�f�ʒu y)�̃��[�v�@oy = " << oy << endl;
		} //oy
		//cout << "���d�𑜓x�������@L�̃��[�v�@L =" << l << endl;
	} //l

	cout << "�I���������O" << endl;
	/*=====
	�I������*/

	//�o�̓e�N�X�`����Mat�^�z��̗̈�m��
	vector<cv::Mat>outtex_cv(Mul_Level);
	for (int i = 0; i < Mul_Level; i++) {
		outtex_cv[i].create(outtex[i].size(), outtex[i][0].size(), CV_8UC3);
	}

	//�z�񂩂�R�s�[
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