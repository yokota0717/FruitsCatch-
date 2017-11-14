#include "MyPG.h"
#include "MyGameMain.h"

int finalScore;  //�ŏI�I���X�R�A
int highScore[6];

//�^�C�g�����
namespace Ending {
	//�Q�[�����
	int logoPosY;
	DI::Mouse mouse;
	int rankScoreCnt;
	bool kemushiMove;
	int resultCnt;

	struct Chara {
		int x;
		int y;
		int speed;
	};

	struct Text {
		bool move;
		int x;
		int y;
		ML::Box2D textBox;
		string text;
	};

	struct Image {
		bool active;
		int x;
		int y;
		ML::Box2D draw;
		ML::Box2D src;
	};

	Chara kemushi, kemushi6;
	Text rankText[5], myScore;
	Image result, rank;

	string imageName[2];

	//�ђ��̏�����
	void Kemushi_Initialize(Chara& c, int x, int y);
	void Kemushi_Update(Chara& c);
	void Kemushi6_Update(Chara& c);
	void Kemushi1_Draw(Chara& c);
	void Kemushi2_Draw(Chara& c);
	void Kemushi3_Draw(Chara& c);
	void Kemushi4_Draw(Chara& c);
	void Kemushi5_Draw(Chara& c);
	void Kemushi6_Draw(Chara& c);

	//�����L���O�X�R�A
	void RankScore_Initialize(Text& score, int x, int y, int num);
	void RankScore_Update(Text& score);
	void RankScore_Draw(Text& score);

	//�����̃X�R�A
	void MyScore_Initialize(Text& myscore);
	void MyScore_Update(Text& myscore);
	void MyScore_Draw(Text& myscore);

	//���ʔ��\�A�����L���O���
	void Image_Initialize(Image& img, int x, int y);
	void Image_Update(Image& img, int type);
	void Image_Draw(Image& img, int type);

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("BackImg", "./data/image/greenBack.bmp");
		DG::Image_Create("ResultImg", "./data/image/Result.png");
		DG::Image_Create("RankImg", "./data/image/endBack.png");
		//DG::Image_Create("BackImg", "./data/image/back.png");
		DG::Image_Create("MadaImg", "./data/image/mada.png");
		DG::Image_Create("YarunaImg", "./data/image/yaruna.png");
		DG::Image_Create("SugoiImg", "./data/image/sugoi.png");
		DG::Image_Create("KueruImg", "./data/image/kueru.png");
		DG::Image_Create("GyakuImg", "./data/image/gyaku.png");
		DG::Image_Create("AmaImg", "./data/image/ama.png");
		DG::Font_Create("FontA", "MS �S�V�b�N", 20, 60);
		DG::Font_Create("FontB", "MS �S�V�b�N", 15, 45);


		kemushiMove = false;
		logoPosY = 270;
		rankScoreCnt = 0;
		resultCnt = 0;
		imageName[0] = "ResultImg";
		imageName[1] = "RankImg";
		Kemushi_Initialize(kemushi, -180, 270 - 130);
		Kemushi_Initialize(kemushi6, -180, 270 - 130);

		MyScore_Initialize(myScore);
		for (int i = 0; i < 5; ++i) {
			RankScore_Initialize(rankText[i], 480, 70 + 38 * i, i);
		}

		Image_Initialize(result, 0, 270);
		result.active = true;
		Image_Initialize(rank, 0, -270);
		rank.active = false;
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("BackImg");
		DG::Image_Erase("ResultImg");
		DG::Image_Erase("RankImg");
		//DG::Image_Erase("BackImg");
		DG::Image_Erase("MadaImg");
		DG::Image_Erase("YarunaImg");
		DG::Image_Erase("SugoiImg");
		DG::Image_Erase("KueruImg");
		DG::Image_Erase("GyakuImg");
		DG::Image_Erase("AmaImg");
		DG::Font_Erase("FontA");
		DG::Font_Erase("FontB");

	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag Update()
	{
		mouse = DI::Mouse_GetState();

		Image_Update(result, 0);
		Image_Update(rank, 1);


		if (rankText[4].move) {
			rankScoreCnt++;
		}

		for (int i = 1; i <= 4; ++i) {
			if (rankScoreCnt >= 30 * i) {
				rankText[4 - i].move = true;
			}
		}
		if (rankText[0].move && rankScoreCnt >= 180) {
			myScore.move = true;
		}

		Kemushi_Update(kemushi);
		Kemushi6_Update(kemushi6);

		MyScore_Update(myScore);
		for (int i = 0; i < 5; ++i) {
			RankScore_Update(rankText[i]);
		}

		TaskFlag rtv = Task_Ending;
		if (logoPosY <= 0) {
			if (mouse.LB.down) {
				rtv = Task_Title;
			}
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������ �Q�c
	//-----------------------------------------------------------------------------
	void Render2D()
	{
		ML::Box2D drawB(0, 0, 480, 270);
		ML::Box2D srcB(0, 0, 480, 270);
		DG::Image_Draw("BackImg", drawB, srcB);  //�Δw�i

		Image_Draw(result, 0);
		Image_Draw(rank, 1);

		if (finalScore < 0) {
			Kemushi5_Draw(kemushi);
		}
		else if (finalScore == 0) {
			Kemushi4_Draw(kemushi);
		}
		else if (finalScore <= 2000) {
			Kemushi1_Draw(kemushi);
		}
		else if (finalScore <= 3000) {
			Kemushi2_Draw(kemushi);
		}
		else if(finalScore <= 3500){
			Kemushi6_Draw(kemushi6);
		}
		else {
			Kemushi3_Draw(kemushi);
		}

		MyScore_Draw(myScore);
		for (int i = 0; i < 5; ++i) {
			RankScore_Draw(rankText[i]);
		}
	}

	//----------------------------------------------------------------------------
	//�֐���
	//----------------------------------------------------------------------------

	void Kemushi_Initialize(Chara& c, int x, int y) {
		c.x = x;
		c.y = y;
		c.speed = 3;
	}

	void Kemushi_Update(Chara& c) {
		if (kemushiMove) {
			c.x += c.speed;
			if (c.x >= -80) {
				c.x = -80;
			}
		}
	}

	void Kemushi6_Update(Chara& c) {
		if (kemushiMove) {
			c.x += c.speed;
			if (c.x >= -30) {
				c.x = -30;
			}
		}
	}

	void Kemushi1_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("MadaImg", draw, src);
	}
	void Kemushi2_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("YarunaImg", draw, src);
	}
	void Kemushi3_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("SugoiImg", draw, src);
	}
	void Kemushi4_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("GyakuImg", draw, src);
	}
	void Kemushi5_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("AmaImg", draw, src);
	}
	void Kemushi6_Draw(Chara& c) {
		ML::Box2D draw(0, 0, 180, 120);
		draw.Offset(c.x, c.y);
		ML::Box2D src(0, 0, 634, 417);
		DG::Image_Draw("KueruImg", draw, src);
	}


	void RankScore_Initialize(Text& score, int x, int y, int num) {
		score.move = false;
		score.x = x;
		score.y = y;
		score.textBox = ML::Box2D(x, y, 100, 60);
		score.text = to_string(highScore[num]);
	}
	void RankScore_Update(Text& score) {
		if (score.move) {
			score.textBox.x -= 8;
			if (score.textBox.x <= 300) {
				score.textBox.x = 300;
			}
		}
	}
	void RankScore_Draw(Text& score) {
		DG::Font_Draw("FontB", score.textBox, score.text, ML::Color(1.0f, 0.0f, 0.0f, 1.0f));
	}


	void MyScore_Initialize(Text& myscore) {
		myscore.move = false;
		myscore.textBox = ML::Box2D(120, -60, 200, 60);
		myscore.text = to_string(finalScore);
	}
	void MyScore_Update(Text& myscore) {
		if (myscore.move) {
			myscore.textBox.y += 10;
			if (myscore.textBox.y >= 110) {
				myscore.textBox.y = 110;
				kemushiMove = true;
			}
		}
	}
	void MyScore_Draw(Text& myscore) {
		DG::Font_Draw("FontA", myscore.textBox, myscore.text, ML::Color(1.0f, 1.0f, 0.75f, 0.0f));
	}

	//���ʔ��\�A�����L���O���
	void Image_Initialize(Image& img, int x, int y) {
		img.x = x;
		img.y = y;
		img.draw = ML::Box2D(img.x, img.y, 480, 270);
		img.src = ML::Box2D(0, 0, 480, 270);
	}

	void Image_Update(Image& img, int type) {
		switch (type) {
		case 0:  //���ʔ��\
			if (img.active) {
				img.draw.y = img.y;
				img.y -= 5;
				if (img.y <= 0 && resultCnt<30) {
					img.y = 0;
					resultCnt++;
				}
				if (resultCnt >= 30) {
					img.y -= 10;
					if (img.y <= -270) {
						img.active = false;
						rank.active = true;
					}
				}
			}
			break;
		case 1:  //�����L���O
			if (img.active) {
				img.draw.y = img.y;
				img.y += 5;
				if (img.y >= 0) {
					img.y = 0;
					rankText[4].move = true;
				}
			}
			break;
		}
	}
	void Image_Draw(Image& img, int type) {
		if (img.active) {
			//img.draw.OffsetCopy(img.x, img.y);
			DG::Image_Draw(imageName[type], img.draw, img.src);
		}
	}

}