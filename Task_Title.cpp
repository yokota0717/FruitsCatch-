#include "MyPG.h"
#include "MyGameMain.h"

#include <vector>
#include <string>

//タイトル画面
namespace Title {
	//ゲーム情報

	int startCnt;

	struct Chara {
		bool stop;  //停止位置に着いたか
		bool appear;  //処理を開始するか(毛虫、始めるボタン用)
		float x;
		int y;
		int type;
		int speed;  //落下速度
		ML::Box2D hitBase;
	};

	Chara moji[5], kemushi[3], cursor, one, two, sky, tree, cloud1[2], cloud2[2];
	string fileName[4];

	void Moji_Initialize(Chara& c_, int type_);
	void Moji_Update(Chara& c_);
	void Moji_Update(Chara& c_);
	void Moji_Draw(Chara& c_);
	void Ku_Draw(Chara& c_);
	void Da_Draw(Chara& c_);
	void Mo_Draw(Chara& c_);
	void No_Draw(Chara& c_);
	void Catch_Draw(Chara& c_);
	void Kemushi_Initialize(Chara& c_, float x_, int y_);
	void Kemushi_Update(Chara& c_, int type);
	void Kemushi_Draw(Chara& c_, int type);

	void Cursor_Initialize(Chara& c, float x, int y);
	void Cursor_Update(Chara& c);
	void Cursor_Draw(Chara& c);

	void Mode_Initialize(Chara& c_, float x_, int y_, int type);
	void Mode_Draw(Chara& c_);

	//背景の初期化
	void Back_Initialize(Chara& c_, float x_, int y_);
	void Back_Update(Chara& c_, int i);
	//背景の表示
	void Back_Draw(Chara& c_, int type);

	struct ResName
	{
		std::string name_, path_;
		ResName(const string& name, const string& path)
			:
			name_(name), path_(path) {}
	};
	std::vector<ResName> resname
	{
		{"TreeImg", "./data/image/tree.png"},
		{"SkyImg", "./data/image/sky.png"},
		{"Cloud1Img", "./data/image/cloud1.png"},
		{"Cloud2Img", "./data/image/cloud2.png"},
		{"KuImg", "./data/image/ku.png"},
		{"DaImg", "./data/image/da.png"},
		{"MoImg", "./data/image/mo.png"},
		{"NoImg", "./data/image/no.png"},
		{"CatchImg", "./data/image/catch.png"},
		{"KemushiLImg", "./data/image/kemushiL.png"},
		{"KemushiRImg", "./data/image/kemushiR.png"},
		{"OneImg", "./data/image/forOne.png"},
		{"TwoImg", "./data/image/forTwo.png"},
		{"CursorImg", "./data/image/cursor.png"}
	};
	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Create(resname[i].name_, resname[i].path_);
		}

		fileName[0] = "SkyImg";
		fileName[1] = "Cloud1Img";
		fileName[2] = "Cloud2Img";
		fileName[3] = "TreeImg";

		startCnt = 0;

		Back_Initialize(sky, 0, 0);
		Back_Initialize(tree, 0, 0);
		Back_Initialize(cloud1[0], 0, 0);
		Back_Initialize(cloud2[0], 0, 0);
		Back_Initialize(cloud1[1], 480, 0);
		Back_Initialize(cloud2[1], 480, 0);


		for (int i = 0; i < 5; ++i) {
			Moji_Initialize(moji[i], i);
		}
		Cursor_Initialize(cursor, 60, 200);
		Mode_Initialize(one, 60.0f, 200, 1);
		Mode_Initialize(two, 280.0f, 200, 2);
		Kemushi_Initialize(kemushi[0], 60.0f, 207);
		Kemushi_Initialize(kemushi[1], 280.0f, 207);
		Kemushi_Initialize(kemushi[2], 375.0f, 207);
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
	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag Update()
	{
		auto in1 = DI::GPad_GetState("P1");
		Back_Update(cloud1[0], 1);
		Back_Update(cloud1[1], 1);
		Back_Update(cloud2[0], 2);
		Back_Update(cloud2[1], 2);


		Moji_Update(moji[0]);
		for (int i = 0; i < 4; ++i) {
			if (moji[i].stop) {
				Moji_Update(moji[i + 1]);
			}
		}
		if (moji[4].stop) {
			one.appear = true;
			two.appear = true;
			if (in1.B1.down || in1.LStick.U.down || in1.LStick.L.down || in1.LStick.R.down || in1.LStick.D.down) { 
				cursor.appear = true;
				startCnt++;
			}
			for (int i = 0; i < 3; ++i) {
				kemushi[i].appear = true;
			}
		}
		Cursor_Update(cursor);
		for (int i = 0; i < 3; ++i) {
			Kemushi_Update(kemushi[i], i);
		}


		TaskFlag rtv = Task_Title;
		if (one.appear && two.appear && cursor.appear) {  //モード選択ボタンとカーソルが表示されていて
			if (in1.B1.down && startCnt>=2) {  //ボタンが押されて
				if (cursor.x == 60.0f) {  //カーソルが左にあれば
					rtv = Task_HowTofor1;  //次のタスクを遊び方一人用へ
				}
				else if(cursor.x == 280.0f){
					rtv = Task_HowTofor2;  //遊び方二人用へ
				}
			}
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
	//-----------------------------------------------------------------------------
	void Render2D()
	{
		Back_Draw(sky, 0);
		//Back_Draw(cloud1[0], 1);
		//Back_Draw(cloud1[1], 1);
		//Back_Draw(cloud2[0], 2);
		//Back_Draw(cloud2[1], 2);

		for (int i = 0; i < 2; ++i) {
			Back_Draw(cloud1[i], 1);
			Back_Draw(cloud2[i], 2);
		}
		Back_Draw(tree, 3);


		for (int i = 0; i < 5; ++i) {
			Moji_Draw(moji[i]);
		}

		if (one.appear && two.appear) {
			for (int i = 0; i < 3; ++i) {
				Kemushi_Draw(kemushi[i], i);
			}
			Mode_Draw(one);
			Mode_Draw(two);
		}
		if (cursor.appear) {
			Cursor_Draw(cursor);
		}
	}


	//-----------------------------------------------------------------------------
    //関数化
	//-----------------------------------------------------------------------------

	
	void Moji_Initialize(Chara & c_, int type_)
	{
		switch (type_) {
		case 0:
			c_.stop = false;
			c_.x = 100.0f;
			c_.y = -55;
			c_.type = type_;
			c_.speed = 10;
			break;
		case 1:
			c_.stop = false;
			c_.x = 175.0f;
			c_.y = -55;
			c_.type = type_;
			c_.speed = 10;
			break;
		case 2:
			c_.stop = false;
			c_.x = 250.0f;
			c_.y = -55;
			c_.type = type_;
			c_.speed = 10;
			break;
		case 3:
			c_.stop = false;
			c_.x = 325.0f;
			c_.y = -55;
			c_.type = type_;
			c_.speed = 10;
			break;
		case 4:
			c_.stop = false;
			c_.x = float(480 - 200) / 2.0f;
			c_.y = 320;
			c_.type = type_;
			c_.speed = -10;
			break;
		}
	}

	void Moji_Update(Chara& c_) {
		c_.y += c_.speed;
		if (c_.type == 4) {
			if (c_.y <= 100) {
				c_.stop = true;
			}
			if (c_.stop) {
				c_.y = 100;
			}
		}
		else {
			if (c_.y >= 50) {
				c_.stop = true;
			}
			if (c_.stop) {
				c_.y = 50;
			}
		}
	}




	void Moji_Draw(Chara & c_)
	{
		switch (c_.type) {
		case 0:
			Ku_Draw(c_);
			break;
		case 1:
			Da_Draw(c_);
			break;
		case 2:
			Mo_Draw(c_);
			break;
		case 3:
			No_Draw(c_);
			break;
		case 4:
			Catch_Draw(c_);
			break;
		}
	}

	void Ku_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 55, 55);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 100, 100);
		DG::Image_Draw("KuImg", draw, src);
	}
	void Da_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 55, 55);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 100, 100);
		DG::Image_Draw("DaImg", draw, src);
	}
	void Mo_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 55, 55);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 100, 100);
		DG::Image_Draw("MoImg", draw, src);
	}
	void No_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 55, 55);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 100, 100);
		DG::Image_Draw("NoImg", draw, src);
	}
	void Catch_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 200, 80);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 200, 80);
		DG::Image_Draw("CatchImg", draw, src);
	}


	void Mode_Initialize(Chara & c_, float x_, int y_, int type)
	{
		c_.appear = false;
		c_.x = x_;
		c_.y = y_;
		c_.type = type;
		c_.hitBase = ML::Box2D((int)c_.x, c_.y, 140, 44);
	}

	void Mode_Draw(Chara & c_)
	{
		ML::Box2D draw(0, 0, 140, 44);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 350, 110);
		switch (c_.type) {
		case 1:
			DG::Image_Draw("OneImg", draw, src);
			break;
		case 2:
			DG::Image_Draw("TwoImg", draw, src);
			break;
		}
	}

	void Kemushi_Initialize(Chara& c_, float x_, int y_) {
		c_.appear = false;
		c_.x = x_;
		c_.y = y_;
	}

	void Kemushi_Update(Chara& c_, int type) {
		if (cursor.appear) {
			if (c_.appear) {
				switch (type) {
				case 0:
					if (cursor.x == 60.0f) {
						c_.x -= 2.0f;
						if (c_.x <= float(60 - 30)) {
							c_.x = float(60 - 30);
						}
					}
					else {
						if (c_.x <= 60.0f) {
							c_.x += 2.0f;
							if (c_.x >= 60.0f) {
								c_.x = 60.0f;
							}
						}
					}
					break;
				case 1:
					if (cursor.x == 280.0f) {
						c_.x -= 2.0f;
						if (c_.x <= float(280 - 30)) {
							c_.x = float(280 - 30);
						}
					}
					else {
						if (c_.x <= 280.0f) {
							c_.x += 2.0f;
							if (c_.x >= 280.0f) {
								c_.x = 280.0f;
							}
						}
					}

					break;
				case 2:
					if (cursor.x == 280.0f) {
						c_.x += 2.0f;
						if (c_.x >= float(375 + 30)) {
							c_.x = float(375 + 30);
						}
					}
					else {
						if (c_.x >= 375.0f) {
							c_.x -= 2.0f;
							if (c_.x <= 375.0f) {
								c_.x = 375.0f;
							}
						}
					}

					break;
				}
			}
		}
	}

	void Kemushi_Draw(Chara& c_, int type) {
		ML::Box2D draw(0, 0, 45, 30);
		draw.Offset((int)c_.x, c_.y);
		ML::Box2D src(0, 0, 634, 417);
		switch (type) {
		case 0:
		case 1:
			DG::Image_Draw("KemushiLImg", draw, src);
			break;
		case 2:
			DG::Image_Draw("KemushiRImg", draw, src);
			break;
		}
	}

	//背景の初期化
	void Back_Initialize(Chara& c_, float x_, int y_) {
		c_.x = x_;
		c_.y = y_;
	}

	void Back_Update(Chara& c_, int i) {
		c_.x -= 0.5f * i;
		if (c_.x <= -480) {
			c_.x = 480;
		}
	}

	//背景の表示
	void Back_Draw(Chara& c_, int type) {
		ML::Box2D draw(0, 0, 480, 270);
		draw.Offset((int)c_.x, 0);
		ML::Box2D src(0, 0, 755, 425);
		DG::Image_Draw(fileName[type], draw, src);
	}

	//カーソル
	void Cursor_Initialize(Chara& c, float x, int y) {
		c.appear = false;
		c.x = x;
		c.y = y;

	}
	void Cursor_Update(Chara& c) {
		if (c.appear) {
			auto in1 = DI::GPad_GetState("P1");
			if (in1.LStick.L.down) {
				if (cursor.x == 60.0f) { c.x = 280.0f; }
				else { cursor.x = 60.0f; }
			}
			if (in1.LStick.R.down) {
				if (cursor.x == 60.0f) { c.x = 280.0f; }
				else { cursor.x = 60.0f; }
			}
		}
	}
	void Cursor_Draw(Chara& c) {
		ML::Box2D draw((int)c.x, c.y, 140, 44);
		ML::Box2D src(0, 0, 350, 110);
		DG::Image_Draw("CursorImg", draw, src);
	}

}