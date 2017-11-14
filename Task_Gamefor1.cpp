#include "MyPG.h"
#include "MyGameMain.h"
#include "time.h"

//�o�u���\�[�g
void bubbleSort(int numbers[], int array_size)
{
	int i, j, temp;

	for (i = 0; i < (array_size - 1); i++) {
		for (j = (array_size - 1); j > i; j--) {
			if (numbers[j - 1] < numbers[j]) {
				temp = numbers[j - 1];
				numbers[j - 1] = numbers[j];
				numbers[j] = temp;
			}
		}
	}
}

extern int finalScore;  //�ŏI�I���X�R�A
extern int highScore[6];  //�����L���O�쐬�p�X�R�A

//�Q�[���{��
namespace Gamefor1 {

	//�Q�[�����
//	DI::VGamePad in1 = DI::GPad_GetState("P1");

	int totalScore;  //���݂̑��X�R�A
	int remTime;  //�c�莞��
	int timeCnt;  //���ԊǗ��p
	int animCnt;  //�c��^�C���g��k���p
	int count;  //�����ύX�p

	bool bonus;  //�{�[�i�X�^�C�����ǂ���
	int bonusTimeCnt;  //�{�[�i�X�^�C�������ԊǗ��p
	int bonusTimeMax;  //�{�[�i�X�^�C����MAX�^�C��
	int birdCome;  //�������ł���^�C�~���O
	float birdY;  //�����Ȑ���`���Ĕ�ԗp
	int clovFallX;  //�l�t�������n�߂�ʒu

	struct Chara {
		bool active;  //����
		bool flyL;  //���ɂ͂����ꂽ
		bool flyR;  //�E�ɂ͂����ꂽ
		bool hit;  //�����ɓ�����
		int x;
		float backX;  //�w�i�px���W
		int y;
		int speed;  //�����̈ړ����x
		int moveCnt;  //
		ML::Box2D hitBase;  //�^�񒆂̓����蔻��
		ML::Box2D outHitL;  //�����̓����蔻��
		ML::Box2D outHitR;  //�E���̓����蔻��
		int typeNum;
		int score;  //���Z�����X�R�A
		float flyH;  //�e����΂��ꂽ���̃W�����v��
		int animCnt;  //�A�j���[�V�����p
	};

	struct Num {
		int x;
		int y;
		ML::Box2D draw;
		ML::Box2D src;
		int drawY;
		int digit;  //���̈ʂ̐�����(1:1...6:10��)
	};

	
	Chara kago, fruits[100], kemushi[50], bird, clover, bonusF[50], bonusText, hurry, sky, tree, cloud1[2], cloud2[2], cursor, more, owari;
	string fileName[4];
	Num timer[2], score[5];

	ML::Box2D scoreBox;  //�����ƐڐG�������̃X�R�A�\���p
	bool scoreBoxLive;
	int scoreBoxCnt;
	string scoreT;  //�l�������X�R�A


	//�֐��̃v���g�^�C�v�錾
	//�����̏�����
	void Kago_Initialize(Chara& c_, int x_, int y_);
	//�����̕\��
	void Kago_Draw(Chara& c_);
	//�����̍s��
	void Kago_Update(Chara& c_);

	//�����ƐڐG�������̃X�R�A�\��
	void Score_Initialize(int x_, int y_, int score_);
	void Score_Update();
	void Score_Draw();

	//�����Ɖʕ��̐ڐG����
	void Fruits_HitCheck(Chara& c_);
	//�����Ɩђ��̐ڐG����
	void Kemushi_HitCheck(Chara& c_);
	//�����Ǝl�t�̐ڐG����
	void Clover_HitCheck(Chara& c_);

	//�����ɒe����΂����
	void FlyAwayL(Chara& c_);
	void FlyAwayR(Chara& c_);

	//�ʕ��̏�����
	void Fruits_Initialize(Chara& c_, int tn_, int x_, int y_);
	void Apple_Initialize(Chara& c_, int x_, int y_);
	void AppleG_Initialize(Chara& c_, int x_, int y_);
	void AppleE_Initialize(Chara& c_, int x_, int y_);

	//�ʕ��̏o��
	void Fruits_Appear(int tn_, int x_, int y_);
	//�ʕ��̏o���^�C�~���O
	void Fruits_AppearTimer();

	//�ʕ��̕\��
	void Fruits_Draw(Chara& c_);
	void Apple_Draw(Chara& c_);
	void AppleG_Draw(Chara& c_);
	void AppleE_Draw(Chara& c_);

	//�ʕ��̍s��
	void Fruits_Update(Chara& c_);
	void Apple_Update(Chara& c_);
	void AppleG_Update(Chara& c_);
	void AppleE_Update(Chara& c_);

	//�ђ��̏�����
	void Kemushi_Initialize(Chara& c_, int x_, int y_);
	//�ђ��̕\��
	void Kemushi_Draw(Chara& c_);
	//�ђ��̍s��
	void Kemushi_Update(Chara& c_);
	//�ђ��̏o��
	void Kemushi_Appear(int x_, int y_);
	//�ђ��̏o���^�C�~���O
	void Kemushi_AppearTimer();

	//���̏�����
	void Bird_Initialize(Chara& c_);
	//���̍s��
	void Bird_Update(Chara& c_);
	//���̕\��
	void Bird_Draw(Chara& c_);

	//�l�t�̏�����
	void Clover_Initialize(Chara& c_);
	//�l�t�̍s��
	void Clover_Update(Chara& c_);
	//�l�t�̕\��
	void Clover_Draw(Chara& c_);

	//�{�[�i�X�ʕ��̏�����
	void Bonus_Initialize(Chara& c_, int x_, int y_);
	//�{�[�i�X�ʕ��̕\��
	void Bonus_Draw(Chara& c_);
	//�{�[�i�X�ʕ��̍s��
	void Bonus_Update(Chara& c_);
	//�{�[�i�X�ʕ��̏o��
	void Bonus_Appear(int x_, int y_);
	//�{�[�i�X�ʕ��̏o���^�C�~���O
	void Bonus_AppearTimer();

	//�{�[�i�X�e�L�X�g�̏�����
	void BonusText_Initialize(Chara& c_, int x_, int y_);
	//�{�[�i�X�e�L�X�g�̍s��
	void BonusText_Update(Chara& c_);
	//�{�[�i�X�e�L�X�g�̕\��
	void BonusText_Draw(Chara& c_);

	//HurryUp�e�L�X�g�̏�����
	void Hurry_Initialize(Chara& c_,int x_, int y_);
	//HurryUp�e�L�X�g�̍s��
	void Hurry_Update(Chara& c_);
	//HurryUp�e�L�X�g�̕\��
	void Hurry_Draw(Chara& c_);

	//�w�i�̏�����
	void Back_Initialize(Chara& c_, int x_, int y_);
	void Back_Update(Chara& c_, int i);
	//�w�i�̕\��
	void Back_Draw(Chara& c_, int type);

	//�J�[�\��
	void Cursor_Initialize(Chara& c, int x, int y);
	void Cursor_Update(Chara& c);
	void Cursor_Draw(Chara& c);

	//�������I����
	void More_Initialize(Chara&c_);
	void More_Draw(Chara& c_);

	//�����I����
	void Owari_Initialize(Chara&c_);
	void Owari_Draw(Chara& c_);

	//�^�C�}�[�A�X�R�A�p����
	void Num_Initialize(Num& n, int x, int y, int digit);
	void timer_Update(Num& n, int i);
	void scoreNum_Update(Num& n);
	void Num_Draw(Num& n);

	void Chara_Finalize(Chara& c);

	struct ResName
	{
		std::string name_, path_;
		ResName(const string& name, const string& path)
			:
			name_(name), path_(path) {}
	};
	std::vector<ResName> resname
	{
		{"ReadyImg", "./data/image/ready.png"},
		{"GoImg", "./data/image/go.png"},
		{"KagoImg", "./data/image/kago.png"},  //����
		{"AppleImg", "./data/image/apple.png"},  //����
		{"AppleGImg", "./data/image/appleG.png"},  //����
		{"AppleEImg", "./data/image/appleE.png"},  //���H��
		{"KemushiImg", "./data/image/kemushi.png"},  //���ނ�
		{"TreeImg", "./data/image/tree.png"},  //��
		{"SkyImg", "./data/image/sky.png"},  //��
		{"Cloud1Img", "./data/image/cloud1.png"},  //�_1
		{"Cloud2Img", "./data/image/cloud2.png"},  //�_2
		{"FinishImg", "./data/image/FINISH.png"},  //�I��蕶��
		{"CursorImg", "./data/image/cursor.png"},  //�J�[�\��
		{"MoreImg", "./data/image/onemore2.png"},  //���g���C����
		{"OwariImg", "./data/image/owari2.png"},  //�G���h�I����
		{"BirdImg", "./data/image/blueBird.png"},  //����
		{"BonusImg", "./data/image/bonustime.png"},  //�{�[�i�X�^�C������
		{"CloverImg", "./data/image/clover.png"},  //���
		{"HurryImg", "./data/image/hurry.png"},  //HurryUp����
		{"TimeImg", "./data/image/time.png"},  //�c��^�C������
		{"NumImg", "./data/image/timer.png"},  //����
		{"ScoreImg", "./data/image/score.png"},  //�X�R�A����

	};
	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void  Initialize()
	{
		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Create(resname[i].name_, resname[i].path_);
		}


		DG::Font_Create("FontA", "MS �S�V�b�N", 7, 20);

		//�����\��I��
		srand((unsigned int)time(NULL));

		//�I�t�X�N���[�����o�^
		fileName[0] = "SkyImg";
		fileName[1] = "Cloud1Img";
		fileName[2] = "Cloud2Img";
		fileName[3] = "TreeImg";

		//���낢�돉����
		animCnt = 0;
		count = 0;

		totalScore = 0;
		remTime = 30;
		timeCnt = -120;
		finalScore = 0;
		bonus = false;
		bonusTimeCnt = 0;
		bonusTimeMax = 600;
		birdCome = rand() % 600 + 300;  //5~15�b�̂����ǂ����ŗ���
		birdY = 1.5f;
		clovFallX = rand() % 350 + 50;

		Kago_Initialize(kago, (480-60)/2, 270 - 45 - 5);
		Bird_Initialize(bird);
		Clover_Initialize(clover);

		BonusText_Initialize(bonusText, 480, (270 - 67) / 2);
		Hurry_Initialize(hurry, 480, (270 - 67) / 2);

		for (int i = 0; i < 2; ++i) {
			Back_Initialize(cloud1[i], 480*i, 0);
			Back_Initialize(cloud2[i], 480*i, 0);
		}

		More_Initialize(more);
		Owari_Initialize(owari);
		
		for (int i = 0; i < 2; ++i) {
			Num_Initialize(timer[i], (480-40)/2 + i*19, 45, 0);
		}
		for (int i = 0; i < 5; ++i) {
			Num_Initialize(score[i], 15 + i * 18, 45, i+1);
		}

		Cursor_Initialize(cursor, 90, 270 - 65);

	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void  Finalize()
	{
		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Erase(resname[i].name_);
		}


		DG::Font_Erase("FontA");

		for (int i = 0; i < 100; ++i) {
			Chara_Finalize(fruits[i]);
		}
		for (int i = 0; i < 50; ++i) {
			Chara_Finalize(kemushi[i]);
			Chara_Finalize(bonusF[i]);
		}
	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag  Update()
	{
		auto in1 = DI::GPad_GetState("P1");

		if (!cursor.active) { Kago_Update(kago); }

		for (int i = 0; i < 2; ++i) {
			Back_Update(cloud1[i], 1);
			Back_Update(cloud2[i], 2);
		}

		if (!bonus) {
			Fruits_AppearTimer();
			Kemushi_AppearTimer();
			timeCnt++;
		}

		if (bonus) {
			bonusTimeCnt++;
			if (bonusTimeCnt <= bonusTimeMax) {
				Bonus_AppearTimer();
			}
		}
		if (bonusTimeCnt > bonusTimeMax) {
			bonus = false;
		}

		for (int f = 0; f < 100; ++f) {
			Fruits_Update(fruits[f]);
			FlyAwayL(fruits[f]);
			FlyAwayR(fruits[f]);
		}

		for (int k = 0; k < 50; ++k) {
			Kemushi_Update(kemushi[k]);
			FlyAwayL(kemushi[k]);
			FlyAwayR(kemushi[k]);
		}

		for (int b = 0; b < 50; ++b) {
			Bonus_Update(bonusF[b]);
			FlyAwayL(bonusF[b]);
			FlyAwayR(bonusF[b]);
		}

		Score_Update();

		if (timeCnt >= birdCome) {
			bird.active = true;
			if (!clover.hit) { clover.active = true; }
		}

		Bird_Update(bird);
		Clover_Update(clover);
		FlyAwayL(clover);
		FlyAwayR(clover);
		BonusText_Update(bonusText);

		if (timeCnt >= 1200) {
			hurry.active = true;
		}
		Hurry_Update(hurry);
		Cursor_Update(cursor);

		finalScore = totalScore;

		if (timeCnt >= 1860) {
			more.active = true;
			owari.active = true;
			cursor.active = true;
		}


		//�^�C�}�[�X�V
		for (int i = 0; i < 2; ++i) { timer_Update(timer[i], i); }
		//�X�R�A�X�V
		for (int i = 0; i < 5; ++i) { scoreNum_Update(score[i]); }

		TaskFlag rtv = Task_Gamefor1;
		if (timeCnt >= 1860) {
			if (cursor.x == 90) {
				if (in1.B1.down) {  //�������I�����������ꂽ��
					rtv = Task_HowTofor1;  //���̃^�X�N��HowTo��
				}
			}
			if (cursor.x == 290) {
				if (in1.B1.down) {  //�����I�����������ꂽ��
					//�����L���O���X�V
					highScore[5] = totalScore;
					bubbleSort(highScore, 6);
					rtv = Task_Endingfor1;  //���̃^�X�N���G���f�B���O��
				}
			}
		}
    	return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������ �Q�c
	//-----------------------------------------------------------------------------
	void  Render2D()
	{
		//�w�i
		Back_Draw(sky, 0);
		for (int i = 0; i < 2; ++i) {
			Back_Draw(cloud1[i], 1);
			Back_Draw(cloud2[i], 2);
		}
		Back_Draw(tree, 3);
		//ReadyGo
		if (timeCnt >= -120 && timeCnt < -50) {
			ML::Box2D drawR(0, 0, 480, 270);
			ML::Box2D srcR(0, 0, 480, 270);
			DG::Image_Draw("ReadyImg", drawR, srcR);
		}
		else if (timeCnt >= -50 && timeCnt < 0) {
			ML::Box2D drawG(0, 0, 480, 270);
			ML::Box2D srcG(0, 0, 480, 270);
			DG::Image_Draw("GoImg", drawG, srcG);
		}

		//����
		Kago_Draw(kago);
		//�������̂Ɩђ�
		for (int f = 0; f < 100; ++f) { Fruits_Draw(fruits[f]); }
		for (int k = 0; k < 50; ++k) { Kemushi_Draw(kemushi[k]); }
		//�{�[�i�X�������̕���
		for (int b = 0; b < 50; ++b) { Bonus_Draw(bonusF[b]); }
		//���Ƃ��e�L�X�g��
		Clover_Draw(clover);
		Bird_Draw(bird);
		BonusText_Draw(bonusText);
		Hurry_Draw(hurry);
		Score_Draw();

		//�X�R�A�̕\��
		ML::Box2D drawS(10, 0, 108, 40);
		ML::Box2D srcS(0, 0, 216, 80);
		DG::Image_Draw("ScoreImg", drawS, srcS);
		for (int i = 0; i < 5; ++i) { Num_Draw(score[i]); }
		if (totalScore < 0) {  //�X�R�A���}�C�i�X�ɂȂ������̑Ώ�
			ML::Box2D drawM(5, 18 + 30, 15, 15);
			ML::Box2D srcM(0, 217 * 10, 200, 217);
			DG::Image_Draw("NumImg", drawM, srcM);
			
		}

		//�c�莞�Ԃ̕\��
		ML::Box2D drawT((480 - 100) / 2, 0, 100, 50);
		ML::Box2D srcT(0, 0, 200, 100);
		DG::Image_Draw("TimeImg", drawT, srcT);

		for (int i = 0; i < 2; ++i) { Num_Draw(timer[i]); }


		//FINISH
		if (timeCnt >= 1800) {  //�I����
			ML::Box2D draw(65, 65, 350, 140);
			ML::Box2D src(0, 0, 350, 140);
			DG::Image_Draw("FinishImg", draw, src);
		}

		More_Draw(more);
		Owari_Draw(owari);
		Cursor_Draw(cursor);

	}
	//-----------------------------------------------------------------------------
	//�֐���
	//-----------------------------------------------------------------------------

	//�����̏�����
	void Kago_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.x = x_;
		c_.y = y_;
		c_.speed = 3;
		c_.hitBase = ML::Box2D(1, 23, 58, 1);
		c_.outHitL = ML::Box2D(0, 0, 1, 45);
		c_.outHitR = ML::Box2D(59, 0, 1, 45);
	}
	//�����̕\��
	void Kago_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 60, 45);
		draw.Offset(c_.x, c_.y);
		ML::Box2D src(0, 0, 400, 300);
		DG::Image_Draw("KagoImg", draw, src);
	}
	//�����̍s��
	void Kago_Update(Chara& c_) {
		auto in1 = DI::GPad_GetState("P1");

		if (in1.B1.on) { c_.speed = 6; }
		else { c_.speed = 3; }

		if (in1.LStick.L.on) {
			c_.x -= c_.speed;
			if (c_.x <= 0) {
				c_.x = 0;
			}
		}
		if (in1.LStick.R.on) { 
			c_.x += c_.speed;
			if (c_.x >= 480 - 60) {
				c_.x = 480 - 60;
			}
		}
	}

	//�ڐG���̃X�R�A�\��
	void Score_Initialize(int x_, int y_, int score_) {
		scoreBoxLive = true;
		scoreBoxCnt = 0;
		scoreBox = ML::Box2D(x_, y_, 30, 25);
		scoreT = to_string(score_);
	}
	void Score_Update() {
		if (scoreBoxLive) {
			scoreBox.y -= 1;
			scoreBoxCnt++;
			if (scoreBoxCnt == 30) {
				scoreBoxLive = false;
			}
		}
	}
	void Score_Draw() {
		if (scoreBoxLive) {
			DG::Font_Draw("FontA", scoreBox, scoreT, ML::Color(0.8f, 0.7f, 0.0f, 0.8f));
		}
	}

	//�����Ɖʕ��̐ڐG����
	void Fruits_HitCheck(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			//�ʕ��̋�`
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			if (kago.active) {
				//�����̒��ɓ������Ƃ�
				ML::Box2D you = kago.hitBase.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (you.Hit(me)) {
					c_.active = false;
					totalScore += c_.score;
					Score_Initialize(c_.x, c_.y, c_.score);
					if (c_.typeNum == 1) {
					}
				}
				//�����̍����ɓ��������Ƃ�
				ML::Box2D youL = kago.outHitL.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youL.Hit(me)) {
					c_.flyL = true;
				}
				//�����̉E���ɓ��������Ƃ�
				ML::Box2D youR = kago.outHitR.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youR.Hit(me)) {
					c_.flyR = true;
				}
			}
		}
	}

	//�����Ɩђ��̐ڐG����
	void Kemushi_HitCheck(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			//�ʕ��̋�`
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			if (kago.active) {
				//�����̒��ɓ������Ƃ�
				ML::Box2D you = kago.hitBase.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (you.Hit(me)) {
					c_.active = false;
					totalScore += c_.score;
					Score_Initialize(c_.x, c_.y, c_.score);
				}
				//�����̍����ɓ��������Ƃ�
				ML::Box2D youL = kago.outHitL.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youL.Hit(me)) {
					c_.flyL = true;
				}
				//�����̉E���ɓ��������Ƃ�
				ML::Box2D youR = kago.outHitR.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youR.Hit(me)) {
					c_.flyR = true;
				}
			}
		}
	}

	//�����Ǝl�t�̐ڐG����
	void Clover_HitCheck(Chara& c_) {
		if (c_.active) {
			//�l�t�̋�`
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			if (kago.active) {
				//�����̒��ɓ������Ƃ�
				ML::Box2D you = kago.hitBase.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (you.Hit(me) && !c_.flyL && !c_.flyR) {
					c_.active = false;
					c_.hit = true;
					bonus = true;
					bonusText.active = true;
				}
				//�����̍����ɓ��������Ƃ�
				ML::Box2D youL = kago.outHitL.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youL.Hit(me)) {
					c_.flyL = true;
				}
				//�����̉E���ɓ��������Ƃ�
				ML::Box2D youR = kago.outHitR.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youR.Hit(me)) {
					c_.flyR = true;
				}
			}
		}
	}

	//�����ƃ{�[�i�X�ʕ��̐ڐG����
	void Bonus_HitCheck(Chara& c_) {
		if (c_.active) {
			//�l�t�̋�`
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			if (kago.active) {
				//�����̒��ɓ������Ƃ�
				ML::Box2D you = kago.hitBase.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (you.Hit(me)) {
					c_.active = false;
				}
				//�����̍����ɓ��������Ƃ�
				ML::Box2D youL = kago.outHitL.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youL.Hit(me)) {
					c_.flyL = true;
				}
				//�����̉E���ɓ��������Ƃ�
				ML::Box2D youR = kago.outHitR.OffsetCopy(kago.x, kago.y);
				//�ڐG����
				if (youR.Hit(me)) {
					c_.flyR = true;
				}
			}
		}
	}

	//�����ɂ͂�����΂����
	//������
	void FlyAwayL(Chara& c_) {
		if (c_.flyL) {
			c_.x -= 6;
			c_.y += (int)c_.flyH;
			c_.flyH += 3;
		}
	}
	//�E����
	void FlyAwayR(Chara& c_) {
		if (c_.flyR) {
			c_.x += 6;
			c_.y += (int)c_.flyH;
			c_.flyH += 3;
		}
	}

	//�ʕ��̏�����
	void Fruits_Initialize(Chara& c_, int tn_, int x_, int y_) {
		switch (tn_) {
		case 0:
			Apple_Initialize(c_, x_, y_);
			break;
		case 1:
			AppleG_Initialize(c_, x_, y_);
			break;
		case 2:
			AppleE_Initialize(c_, x_, y_);
			break;
		}
	}

	void Apple_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.flyL = false;
		c_.flyR = false;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = ML::Box2D(0, 0, 30, 33);  //388*426
		c_.moveCnt = 0;
		c_.score = 50;
		c_.typeNum = 0;
		c_.flyH = -15.f;
	}

	void AppleG_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.flyL = false;
		c_.flyR = false;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = ML::Box2D(0, 0, 30, 33);
		c_.moveCnt = 0;
		c_.score = 100;
		c_.typeNum = 1;
		c_.flyH = -10.f;
	}

	void AppleE_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.flyL = false;
		c_.flyR = false;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = ML::Box2D(0, 0, 30, 33);
		c_.moveCnt = 0;
		c_.score = 10;
		c_.typeNum = 2;
		c_.flyH = -17.f;
	}

	//�ʕ��̏o��
	void Fruits_Appear(int tn_, int x_, int y_) {
		for (int f = 0; f < 100; f++) {
			if (!fruits[f].active) {
				Fruits_Initialize(fruits[f], tn_, x_, y_);
				break;
			}
		}
	}

	//�ʕ��̏o���^�C�~���O
	void Fruits_AppearTimer() {
		struct sAppearData {
			int tim;
			int typeNumber;
			int x;
			int y;
		};

		sAppearData ad[100];

		//�`�I��
		if (timeCnt < 1800) {
			//����
			for (int a = 0; a < 30; ++a) {
				ad[a].tim = rand() % 1650;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//����
			for (int g = 30; g < 40; ++g) {
				ad[g].tim = rand() % 1650;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//���H��
			for (int e = 40; e < 60; ++e) {
				ad[e].tim = rand() % 1650;
				ad[e].typeNumber = 2;
				ad[e].x = rand() % (480 - 30);
				ad[e].y = -33;
			}
		}

		//10�b�`������
		if (timeCnt >= 600) {
			//����
			for (int a = 0; a < 40; ++a) {
				ad[a].tim = rand() % 1050 + 600;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//����
			for (int g = 40; g < 60; ++g) {
				ad[g].tim = rand() % 1050 + 600;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//���H��
			for (int e = 60; e < 80; ++e) {
				ad[e].tim = rand() % 1050 + 600;
				ad[e].typeNumber = 2;
				ad[e].x = rand() % (480 - 30);
				ad[e].y = -33;
			}
		}

		//25�`�I��������
		if (timeCnt >= 1500) {
			//����
			for (int a = 0; a < 50; ++a) {
				ad[a].tim = rand() % 150 + 1500;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//����
			for (int g = 50; g < 70; ++g) {
				ad[g].tim = rand() % 150 + 1500;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//���H��
			for (int e = 70; e < 100; ++e) {
				ad[e].tim = rand() % 150 + 1500;
				ad[e].typeNumber = 2;
				ad[e].x = rand() % (480 - 30);
				ad[e].y = -33;
			}
		}


		for (int i = 0; i < _countof(ad); ++i) {
			if (timeCnt == ad[i].tim) {
				Fruits_Appear(ad[i].typeNumber, ad[i].x, ad[i].y);
			}
		}
	}


	//�ʕ��̕\��
	void Fruits_Draw(Chara& c_) {
		switch (c_.typeNum) {
		case 0:
			Apple_Draw(c_);
			break;
		case 1:
			AppleG_Draw(c_);
			break;
		case 2:
			AppleE_Draw(c_);
			break;
		}
	}

	void Apple_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 30, 33); 
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 388, 426);
			DG::Image_Draw("AppleImg", draw, src);
		}
	}
	void AppleG_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 30, 33);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 388, 426);
			DG::Image_Draw("AppleGImg", draw, src);
		}
	}
	void AppleE_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 30, 33);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 388, 426);
			DG::Image_Draw("AppleEImg", draw, src);
		}

	}

	//�ʕ��̍s��
	void Fruits_Update(Chara& c_) {
		switch (c_.typeNum) {
		case 0:
			Apple_Update(c_);
			break;
		case 1:
			AppleG_Update(c_);
			break;
		case 2:
			AppleE_Update(c_);
			break;
		}
	}

	void Apple_Update(Chara& c_) {
		if (c_.active&& !c_.flyL && !c_.flyR) {
			c_.moveCnt++;
			c_.y += (9 * c_.moveCnt) / 200;
			if (c_.y >= 270 || c_.x < 0 || c_.x > 480) {
				c_.active = false;
				c_.moveCnt = 0;
			}
			Fruits_HitCheck(c_);
		}
	}
	void AppleG_Update(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			c_.moveCnt++;
			c_.y += (12 * c_.moveCnt) / 200;
			if (c_.y >= 270 || c_.x < 0 || c_.x > 480) {
				c_.active = false;
				c_.moveCnt = 0;
			}
			Fruits_HitCheck(c_);
		}
	}
	void AppleE_Update(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			c_.moveCnt++;
			c_.y += (6 * c_.moveCnt) / 200;
			if (c_.y >= 270 || c_.x < 0 || c_.x > 480) {
				c_.active = false;
				c_.moveCnt = 0;
			}
			Fruits_HitCheck(c_);
		}
	}


	//�ђ��̏�����
	void Kemushi_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.flyL = false;
		c_.flyR = false;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = ML::Box2D(0, 0, 20, 10);
		c_.moveCnt = 0;
		c_.score = -50;
		c_.flyH = -15.f;
	}

	//�ђ��̕\��
	void Kemushi_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 40, 26);  //�摜�T�C�Y634*417
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 634, 417);
			DG::Image_Draw("KemushiImg", draw, src);
		}
	}

	//�ђ��̍s��
	void Kemushi_Update(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			c_.moveCnt++;
			c_.y += (12 * c_.moveCnt) / 200;
			if (c_.y >= 270 || c_.x < 0 || c_.x > 480) {
				c_.active = false;
				c_.moveCnt = 0;
			}
			Kemushi_HitCheck(c_);
		}
	}

	//�ђ��̏o��
	void Kemushi_Appear(int x_, int y_) {
		for (int k = 0; k < 50; k++) {
			if (!kemushi[k].active) {
				Kemushi_Initialize(kemushi[k], x_, y_);
				break;
			}
		}
	}

	//�ђ��̏o���^�C�~���O
	void Kemushi_AppearTimer() {
		struct sAppearData {
			int tim;
			int x;
			int y;
		};

		sAppearData ad[50];

		//�`20�b
		if (timeCnt < 1200) {
			for (int k = 0; k < 20; ++k) {
				ad[k].tim = rand() % 1200;
				ad[k].x = rand() % (480 - 20);
				ad[k].y = -13;
			}
		}

		if (timeCnt >= 1200) {
			for (int k = 0; k < 50; ++k) {
				ad[k].tim = rand() % 450 + 1200;
				ad[k].x = rand() % (480 - 20);
				ad[k].y = -13;
			}
		}



		for (int i = 0; i < _countof(ad); ++i) {
			if (timeCnt == ad[i].tim) {
				Kemushi_Appear(ad[i].x, ad[i].y);
			}
		}
	}

	//���̏�����
	void Bird_Initialize(Chara& c_) {
		c_.active = false;
		c_.x = 480 + 40 + 30;
		c_.y = 0;
		c_.moveCnt = 0;
		c_.animCnt = 0;
	}
	//���̍s��
	void Bird_Update(Chara& c_) {
		if (c_.active) {
			c_.moveCnt++;
			c_.x -= 4;
			if (c_.x >= 240) {
				c_.y += (int)birdY;
				birdY -= 0.01f;
			}
			else if (c_.x < 240) {
				c_.y -= (int)birdY;
				birdY += 0.01f;
			}
			c_.animCnt++;
			if (c_.animCnt >= 30) {
				c_.animCnt = 0;
			}
		}
		if (c_.x <= -32) {
			c_.active = false;
		}
	}
	//���̕\��
	void Bird_Draw(Chara& c_) {
		if (c_.active){
			ML::Box2D draw(0, 0, 40, 25);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 57*(c_.animCnt/10), 113, 57);
			DG::Image_Draw("BirdImg", draw, src);
		}
	}

	//�l�t�̏�����
	void Clover_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 480 + 40;
		c_.y = 50;
		c_.moveCnt = 0;
		c_.hitBase = ML::Box2D(0, 0, 35, 30);
		c_.flyH = -15.f;
	}
	//�l�t�̍s��
	void Clover_Update(Chara& c_) {
		if (c_.active&& !c_.flyL&& !c_.flyR) {
			if (c_.x > clovFallX) {
				c_.x = bird.x - 30;
				c_.y = bird.y - 15;
			}
			else {
				c_.moveCnt++;
				c_.x = clovFallX;
				c_.y += 1 + (6 * c_.moveCnt) / 200;
			}
			if (c_.y > 270) {
				c_.active = false;
			}
		}
		Clover_HitCheck(c_);
	}
	//�l�t�̕\��
	void Clover_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 35, 30);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 492, 432);
			DG::Image_Draw("CloverImg", draw, src);
		}
	}


	//�{�[�i�X�ʕ��̏�����
	void Bonus_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.flyL = false;
		c_.flyR = false;
		c_.x = x_;
		c_.y = y_;
		c_.hitBase = ML::Box2D(0, 0, 28, 32);
		c_.moveCnt = 0;
		c_.score = 100;
		c_.typeNum = 1;
		c_.flyH = -10.f;
	}

	//�{�[�i�X�ʕ��̏o��
	void Bonus_Appear(int x_, int y_) {
		for (int b = 0; b < 50; ++b) {
			if (!bonusF[b].active) {
				Bonus_Initialize(bonusF[b], x_, y_);
				break;
			}
		}
	}

	//�{�[�i�X�ʕ��̕\��
	void Bonus_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 30, 33);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 388, 426);
			DG::Image_Draw("AppleGImg", draw, src);
		}
	}

	//�{�[�i�X�ʕ��̍s��
	void Bonus_Update(Chara& c_){
		if (c_.active && !c_.flyL && !c_.flyR) {
			c_.moveCnt++;
			c_.y += (12 * c_.moveCnt) / 200;
			if (c_.y >= 270 || c_.x < 0 || c_.x > 480) {
				c_.active = false;
				c_.moveCnt = 0;
			}
			Fruits_HitCheck(c_);
		}
	}

	//�{�[�i�X�ʕ��̏o���^�C�~���O
	void Bonus_AppearTimer() {
		struct sAppearData {
			int tim;
			int typeNumber;
			int x;
			int y;
		};

		sAppearData ad[50];
		for (int g = 0; g < 50; ++g) {
			ad[g].tim = rand() % 600;
			ad[g].x = rand() % (480 - 28);
			ad[g].y = -33;
		}

		for (int i = 0; i < _countof(ad); ++i) {
			if (bonusTimeCnt == ad[i].tim) {
				Bonus_Appear(ad[i].x, ad[i].y);
			}
		}
	}

	//�{�[�i�X�e�L�X�g�̏�����
	void BonusText_Initialize(Chara& c_, int x_, int y_) {
		c_.active = false;
		c_.x = x_;
		c_.y = y_;
	}

	//�{�[�i�X�e�L�X�g�̍s��
	void BonusText_Update(Chara& c_) {
		if (c_.active) {
			c_.x -= 5;
			if (c_.x < -350) {
				c_.active = false;
			}
		}
	}

	//�{�[�i�X�e�L�X�g�̕\��
	void BonusText_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 350, 67);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 67);
			DG::Image_Draw("BonusImg", draw, src);
		}
	}

	//HurryUp�e�L�X�g�̏�����
	void Hurry_Initialize(Chara& c_, int x_, int y_) {
		c_.active = false;
		c_.x = x_;
		c_.y = y_;
	}
	//HurryUp�e�L�X�g�̍s��
	void Hurry_Update(Chara& c_) {
		if (c_.active) {
			c_.x -= 7;
			if (c_.x < 350) {
				c_.active = false;
			}
		}
	}
	//HurryUp�e�L�X�g�̕\��
	void Hurry_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 350, 67);
		draw.Offset(c_.x, c_.y);
		ML::Box2D src(0, 0, 350, 67);
		DG::Image_Draw("HurryImg", draw, src);
	}


	

	//�w�i�̏�����
	void Back_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.backX = (float)x_;
		c_.y = y_;
	}

	void Back_Update(Chara& c_, int i) {
		c_.backX -= 0.5f * i;
		if (c_.backX <= -480.0f) {
			c_.backX = 480.0f;
		}
	}

	//�w�i�̕\��
	void Back_Draw(Chara& c_, int type) {
		ML::Box2D draw(0, 0, 480, 270);
		draw.Offset((int)c_.backX, 0);
		ML::Box2D src(0, 0, 755, 425);
		DG::Image_Draw(fileName[type], draw, src);
	}

	//�J�[�\��
	void Cursor_Initialize(Chara& c, int x, int y) {
		c.active = false;
		c.x = x;
		c.y = y;
	}
	void Cursor_Update(Chara& c) {
		if (cursor.active) {
			auto in1 = DI::GPad_GetState("P1");
			auto in2 = DI::GPad_GetState("P2");

			if (in1.LStick.L.down) {
				if (cursor.x == 90) { c.x = 290; }
				else { cursor.x = 90; }
			}
			if (in1.LStick.R.down) {
				if (cursor.x == 90) { c.x = 290; }
				else { cursor.x = 90; }
			}
		}
	}
	void Cursor_Draw(Chara& c) {
		if (c.active) {
			ML::Box2D draw(c.x, c.y, 120, 42);
			ML::Box2D src(0, 0, 350, 110);
			DG::Image_Draw("CursorImg", draw, src);
		}
	}


	//�������I�����̏�����
	void More_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 100;
		c_.y = 270 - 60;
		c_.hitBase = ML::Box2D(c_.x, c_.y, 100, 32);
	}
	//�������I�����̕\��
	void More_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 100, 32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 110);
			DG::Image_Draw("MoreImg", draw, src);
		}
	}

	//�����I�����̏�����
	void Owari_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 300;
		c_.y = 270 - 60;
		c_.hitBase = ML::Box2D(c_.x, c_.y, 100, 32);

	}
	//�����I�����̕\��
	void Owari_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 100, 32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 110);
			DG::Image_Draw("OwariImg", draw, src);
		}
	}
	

	//�����̊֐�
	void Num_Initialize(Num& n, int x, int y, int t) {
		n.x = x;
		n.y = y;
		n.digit = t;
		n.draw = ML::Box2D(n.x, n.y, 20, 20);
		n.src = ML::Box2D(0, 0, 0, 0);  //�_�~�[�f�[�^
		n.drawY = 0;  //�_�~�[����
	}

	void timer_Update(Num& n, int i) {
		if (timeCnt >= 0) {
			if (!bonus) {
				count++;
				if (count > 1200) {
					count = 0;
				}
				n.drawY = count / 120;

				if (timeCnt < 1800) {
					switch (i) {
					case 0:  //�\�̈�
						if (timeCnt >= 0 && timeCnt < 600) {
							n.src = ML::Box2D(0, 7 * 217, 200, 217);  //200*217 ����
						}
						else if (timeCnt >= 600 && timeCnt < 1200) {
							n.src = ML::Box2D(0, 8 * 217, 200, 217);  //200*217 ����
						}
						else {  //�c��10�b�ȍ~
							n.src = ML::Box2D(200, 9 * 217, 200, 217);  //200*217�@�ԕ���
						}
						break;
					case 1:  //��̈�
						if (timeCnt < 1200) {
							n.src = ML::Box2D(0, n.drawY * 217, 200, 217);  //200*217 ����
						}
						else {  //�c��10�b�ȍ~
							n.src = ML::Box2D(200, n.drawY * 217, 200, 217);  //200*217�@�ԕ���
						}
						break;
					}
				}

			}
			else {  //�{�[�i�X�^�C����
				if (timeCnt < 1800) {
					switch (i) {
					case 0:  //�\�̈�
						if (timeCnt >= 0 && timeCnt < 600) {
							n.src = ML::Box2D(200, 7 * 217, 200, 217);  //200*217 �ԕ���
						}
						else {
							n.src = ML::Box2D(200, 8 * 217, 200, 217);  //200*217 �ԕ���

						}
						break;
					case 1:  //��̈�
						n.src = ML::Box2D(200, n.drawY * 217, 200, 217);  //200*217 �ԕ���
						break;
					}
				}

			}
		}
	}

	void scoreNum_Update(Num& n) {
		int remScore;
		if(totalScore >= 0){ remScore = totalScore; }
		else { remScore = -1 * totalScore; }
		score[0].src = ML::Box2D(0, (9 - remScore / 10000) * 217, 200, 217);
		remScore %= 10000;
		score[1].src = ML::Box2D(0, (9 - remScore / 1000) * 217, 200, 217);
		remScore %= 1000;
		score[2].src = ML::Box2D(0, (9 - remScore / 100) * 217, 200, 217);
		remScore %= 100;
		score[3].src = ML::Box2D(0, (9 - remScore / 10) * 217, 200, 217);
		remScore %= 10;
		score[4].src = ML::Box2D(0, (9 - remScore) * 217, 200, 217);

	}

	void Num_Draw(Num& n) {
		DG::Image_Draw("NumImg", n.draw, n.src);
	}

	void Chara_Finalize(Chara& c) {
		c.active = false;
		c.flyL = false;
		c.flyR = false;
		c.hit = false;
		c.animCnt = 0;
		c.flyH = 0;
		c.hitBase = ML::Box2D(0, 0, 0, 0);
		c.moveCnt = 0;
		c.outHitR = ML::Box2D(0, 0, 0, 0);
		c.outHitR = ML::Box2D(0, 0, 0, 0);
		c.score = 0;
		c.typeNum = 0;
		c.x = 0;
		c.y = 0;
	}
}