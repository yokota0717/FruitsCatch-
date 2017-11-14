#include "MyPG.h"
#include "MyGameMain.h"

int finalScore;  //最終的総スコア
int highScore[6];

//タイトル画面
namespace Endingfor1 {
	//ゲーム情報
//	DI::VGamePad in1 = DI::GPad_GetState("P1");

	int rankScoreCnt;
	bool kemushiMove;
	int resultCnt;
	bool end;  //表示し終わった

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
	Image ResultImg, RankingImg;

	string imageName[2];

	//毛虫の初期化
	void Kemushi_Initialize(Chara& c, int x, int y);
	void Kemushi_Update(Chara& c);
	void Kemushi6_Update(Chara& c);
	void Kemushi1_Draw(Chara& c);
	void Kemushi2_Draw(Chara& c);
	void Kemushi3_Draw(Chara& c);
	void Kemushi4_Draw(Chara& c);
	void Kemushi5_Draw(Chara& c);
	void Kemushi6_Draw(Chara& c);

	//ランキングスコア
	void RankScore_Initialize(Text& score, int x, int y, int num);
	void RankScore_Update(Text& score);
	void RankScore_Draw(Text& score);

	//自分のスコア
	void MyScore_Initialize(Text& myscore);
	void MyScore_Update(Text& myscore);
	void MyScore_Draw(Text& myscore);

	//結果発表、ランキング画面
	void Image_Initialize(Image& img, int x, int y);
	void Image_Update(Image& img, int type);
	void Image_Draw(Image& img, int type);

	struct ResName
	{
		std::string name_, path_;
		ResName(const string& name, const string& path)
			:
			name_(name), path_(path) {}
	};
	std::vector<ResName> resname
	{
		{"BackImg", "./data/image/greenBack.bmp"},
		{"ResultImg", "./data/image/Result.png"},
		{"RankImg", "./data/image/endBack.png"},
		{"MadaImg", "./data/image/mada.png"},
		{"YarunaImg", "./data/image/yaruna.png"},
		{"SugoiImg", "./data/image/sugoi.png"},
		{"KueruImg", "./data/image/kueru.png"},
		{"GyakuImg", "./data/image/gyaku.png"},
		{"AmaImg", "./data/image/ama.png"}

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

		DG::Font_Create("FontA", "MS ゴシック", 20, 60);
		DG::Font_Create("FontB", "MS ゴシック", 15, 45);

		end = false;
		kemushiMove = false;
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

		Image_Initialize(ResultImg, 0, 270);
		ResultImg.active = true;
		Image_Initialize(RankingImg, 0, -270);
		RankingImg.active = false;
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

		DG::Font_Erase("FontA");
		DG::Font_Erase("FontB");

	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag Update()
	{
		auto in1 = DI::GPad_GetState("P1");

		Image_Update(ResultImg, 0);
		Image_Update(RankingImg, 1);


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

		TaskFlag rtv = Task_Endingfor1;
		if (end) {  //すべての表示が終わっていて
			if (in1.B1.down) {
				rtv = Task_Title;
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
		ML::Box2D drawB(0, 0, 480, 270);
		ML::Box2D srcB(0, 0, 480, 270);
		DG::Image_Draw("BackImg", drawB, srcB);  //緑背景

		Image_Draw(ResultImg, 0);
		Image_Draw(RankingImg, 1);

		if (finalScore < 0) {
			Kemushi5_Draw(kemushi);
		}
		else if (finalScore == 0) {
			Kemushi4_Draw(kemushi);
		}
		else if (finalScore <= 1000) {
			Kemushi1_Draw(kemushi);
		}
		else if (finalScore <= 2000) {
			Kemushi2_Draw(kemushi);
		}
		else if(finalScore <= 2500){
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
	//関数化
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
				end = true;
			}
		}
	}

	void Kemushi6_Update(Chara& c) {
		if (kemushiMove) {
			c.x += c.speed;
			if (c.x >= -30) {
				c.x = -30;
				end = true;
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

	//結果発表、ランキング画面
	void Image_Initialize(Image& img, int x, int y) {
		img.x = x;
		img.y = y;
		img.draw = ML::Box2D(img.x, img.y, 480, 270);
		img.src = ML::Box2D(0, 0, 480, 270);
	}

	void Image_Update(Image& img, int type) {
		switch (type) {
		case 0:  //結果発表
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
						RankingImg.active = true;
					}
				}
			}
			break;
		case 1:  //ランキング
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