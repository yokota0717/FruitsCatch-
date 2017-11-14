#include "MyPG.h"
#include "MyGameMain.h"

//タイトル画面
namespace HowTofor1 {
	//ゲーム情報

	//DI::VGamePad in1 = DI::GPad_GetState("P1");

	bool howtoChange, start;
	bool wind;  //風が吹いているか
	int windCnt;  //何フレーム風が吹くか
	int cnt;

	struct Chara {
		int x;
		int y;
		float nowAngle;
		float moveAngle;
		float speed;  //ひらひらする速さ
	};

	Chara paper[4], pin[4];
	string objName[5];

	//関数プロトタイプ
	void Obj_Initialize(Chara& c_, int x_, int y_);
	void Obj_Update(Chara& c_);  //紙のひらひら
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
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
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
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
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
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
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

		if (!wind) {  //風が吹いていなかったら
			switch (rand() % 80) {
			case 0:
				wind = true;
				//windCnt = rand() % 120 + 60;  //1～3秒間風吹く
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
		if (in1.B1.down && start) {  //クリックされたら
			rtv = Task_Gamefor1;  //次のタスクをゲーム本編へ
		}		
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
	//-----------------------------------------------------------------------------
	void Render2D()
	{
		//背景
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		DG::Image_Draw("BackImg", draw, src);

		//HowTo
		if (howtoChange) {  //2枚目
			ML::Box2D draw1(0, 0, 480, 270);
			ML::Box2D src1(0, 0, 480, 270);
			DG::Image_Draw("Howto2Img", draw1, src1);

			for (int i = 0; i < 4; ++i) {
				Obj_Draw(paper[i], i);
				Obj_Draw(pin[i], 4);
			}
		}
		else {  //1枚目
			ML::Box2D draw1(0, 0, 480, 270);
			ML::Box2D src1(0, 0, 480, 270);
			DG::Image_Draw("Howto1Img", draw1, src1);
		}

	}


	//関数定義
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
		//絵を描く前に傾きを設定する
		DG::Image_Rotation(objName[type], c_.nowAngle /*回転角度*/, ML::Vec2(40, 20) /*回転軸座標*/);
		DG::Image_Draw(objName[type], draw, src);

	}
}