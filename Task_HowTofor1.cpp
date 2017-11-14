#include "MyPG.h"
#include "MyGameMain.h"

//�^�C�g�����
namespace HowTofor1 {
	//�Q�[�����

	//DI::VGamePad in1 = DI::GPad_GetState("P1");

	bool howtoChange, start;
	bool wind;  //���������Ă��邩
	int windCnt;  //���t���[������������
	int cnt;

	struct Chara {
		int x;
		int y;
		float nowAngle;
		float moveAngle;
		float speed;  //�Ђ�Ђ炷�鑬��
	};

	Chara paper[4], pin[4];
	string objName[5];

	//�֐��v���g�^�C�v
	void Obj_Initialize(Chara& c_, int x_, int y_);
	void Obj_Update(Chara& c_);  //���̂Ђ�Ђ�
	void Obj_Draw(Chara& c_, int type);

	struct ResName
	{
		std::string name_, path_;
		ResName(const string& name, const string& path)
			:
			name_(name), path_(path) {}
	};
	std::vector<ResName> resname
	{
		{"Howto1Img", "./data/image/howtofor1-1.png"},
		{"Howto2Img", "./data/image/howto2.png"},
		{"paper0Img", "./data/image/paperApple.png"},
		{"paper1Img", "./data/image/paperAppleG.png"},
		{"paper2Img", "./data/image/paperAppleE.png"},
		{"paper3Img", "./data/image/paperKemushi.png"},
		{"PinImg", "./data/image/pin.png"},
		{"BackImg", "./data/image/Back.jpg"}
	};

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		srand((unsigned int)time(NULL));

		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Create(resname[i].name_, resname[i].path_);
		}

		//DG::Image_Create("Howto1Img", "./data/image/howto1.png");
		//DG::Image_Create("Howto2Img", "./data/image/howto2.png");
		//DG::Image_Create("paper0Img", "./data/image/paperApple.png");
		//DG::Image_Create("paper1Img", "./data/image/paperAppleG.png");
		//DG::Image_Create("paper2Img", "./data/image/paperAppleE.png");
		//DG::Image_Create("paper3Img", "./data/image/paperKemushi.png");
		//DG::Image_Create("PinImg", "./data/image/pin.png");
		//DG::Image_Create("BackImg", "./data/image/Back.jpg");

		howtoChange = false;
		start = false;
		wind = false;
		windCnt = 0;
		cnt = 0;

		for (int i = 0; i < 4; ++i) {
			objName[i] = "paper" + to_string(i) + "Img";
		}
		objName[4] = "PinImg";
		Obj_Initialize(paper[0], 30, 90);
		Obj_Initialize(pin[0], 30, 90);
		Obj_Initialize(paper[1], 225, 90);
		Obj_Initialize(pin[1], 225, 90);
		Obj_Initialize(paper[2], 45, 165);
		Obj_Initialize(pin[2], 45, 165);
		Obj_Initialize(paper[3], 235, 165);
		Obj_Initialize(pin[3], 235, 165);


	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Erase(resname[i].name_);
		}

		//DG::Image_Erase("BoardImg");
		//DG::Image_Erase("paperAImg");
		//DG::Image_Erase("paperGImg");
		//DG::Image_Erase("paperEImg");
		//DG::Image_Erase("paperKImg");
		//DG::Image_Erase("PinImg");
		//DG::Image_Erase("BackImg");
		//DG::Image_Erase("Howto1Img");
		//DG::Image_Erase("Howto2Img");
	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag Update()
	{
		auto in1 = DI::GPad_GetState("P1");

		if (in1.B1.down) {
			cnt++;
		}
		if (cnt == 1) {
			howtoChange = true;
		}
		else if(cnt >= 2){
			start = true;
		}

		if (!wind) {  //���������Ă��Ȃ�������
			switch (rand() % 80) {
			case 0:
				wind = true;
				//windCnt = rand() % 120 + 60;  //1�`3�b�ԕ�����
				break;
			default:
				break;
			}
		}
		//if (wind && windCnt != 0) {
		//	windCnt--;
		//	if (windCnt <= 0) {
		//		windCnt = 0;
		//		wind = false;
		//	}
		//}

		for (int i = 0; i < 4; ++i) {
			Obj_Update(paper[i]);
		}


		TaskFlag rtv = Task_HowTofor1;
		if (in1.B1.down && start) {  //�N���b�N���ꂽ��
			rtv = Task_Gamefor1;  //���̃^�X�N���Q�[���{�҂�
		}		
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������ �Q�c
	//-----------------------------------------------------------------------------
	void Render2D()
	{
		//�w�i
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		DG::Image_Draw("BackImg", draw, src);

		//HowTo
		if (howtoChange) {  //2����
			ML::Box2D draw1(0, 0, 480, 270);
			ML::Box2D src1(0, 0, 480, 270);
			DG::Image_Draw("Howto2Img", draw1, src1);

			for (int i = 0; i < 4; ++i) {
				Obj_Draw(paper[i], i);
				Obj_Draw(pin[i], 4);
			}
		}
		else {  //1����
			ML::Box2D draw1(0, 0, 480, 270);
			ML::Box2D src1(0, 0, 480, 270);
			DG::Image_Draw("Howto1Img", draw1, src1);
		}

	}


	//�֐���`
	void Obj_Initialize(Chara& c_, int x_, int y_) {
		c_.x = x_;
		c_.y = y_;
		c_.moveAngle = float(rand() % 4 + 1);
		c_.nowAngle = ML::ToRadian(c_.moveAngle);
		c_.speed = 0.0f;
	}

	void Obj_Update(Chara& c_) {
		////c_.moveAngle -= ML::ToRadian(genrand_int32() % 31 + 1);
		//c_.speed = -0.01f*ML::ToRadian(genrand_int32() % 10 + 1);
		//if (c_.nowAngle <= 0 && c_.nowAngle >= c_.maxAngle) {
		//	c_.nowAngle += c_.speed;
		//}
		//else {
		//	c_.nowAngle -= c_.speed;
		//}
		c_.moveAngle += float(rand() % 4 + 1);
		c_.nowAngle = cos(ML::ToRadian(c_.moveAngle))*0.2f -0.1f;

	}

	void Obj_Draw(Chara& c_, int type) {
		ML::Box2D draw(0, 0, 80, 80);
		draw.Offset(c_.x, c_.y);
		ML::Box2D src(0, 0, 560, 560);
		//�G��`���O�ɌX����ݒ肷��
		DG::Image_Rotation(objName[type], c_.nowAngle /*��]�p�x*/, ML::Vec2(40, 20) /*��]�����W*/);
		DG::Image_Draw(objName[type], draw, src);

	}
}