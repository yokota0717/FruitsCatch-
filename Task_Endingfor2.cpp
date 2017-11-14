#include "MyPG.h"
#include "MyGameMain.h"

int finalScore1P;
int finalScore2P;

//タイトル画面
namespace Endingfor2 {
	//ゲーム情報
	int ImgCnt;
	bool ImgMove;
	int resultCnt;
	bool end;  //表示し終わった
	bool win1P;  //1Pが勝ったか
	bool scoreMove;  //スコアの表示を開始する

	struct Chara {
		int x;
		int y;
		int speed;
	};

	struct Text {
		int x;
		int y;
		int cnt;
		ML::Box2D textBox;
		string text;
	};

	struct Back {
		bool active;
		int x;
		int y;
		ML::Box2D draw;
		ML::Box2D src;
	};

	Chara img1P, img2P;  //勝ち負けに応じた画像
	Text score1P, score2P;  //スコア
	Back ResultImg, RankingImg;  //背景

	string imageName[2];

	//winner,loserImg初期化
	void Image_Initialize(Chara& c, int x, int y);
	void Image_Update(Chara& c, int type);
	void Image_Draw(Chara& c, bool win1P);

	//スコア
	void Score_Initialize(Text& myscore, int x, int y);
	void Score_Update(Text& myscore, int score);
	void Score_Draw(Text& myscore);

	//結果発表、ランキング画面
	void Back_Initialize(Back& img, int x, int y);
	void Back_Update(Back& img, int type);
	void Back_Draw(Back& img, int type);

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
		{"RankImg", "./data/image/endBackfor2.png"},
		{"WinnerTxt","./data/image/winner.png"},
		{"LoserTxt","./data/image/loser.png"},
		{"WinnerImg","./data/image/winnerImg.png"},
		{"LoserImg","./data/image/loserImg.png"},
		{"DrawImg","./data/image/draw.png"}
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

		DG::Font_Create("FontA", "MS ゴシック", 15, 45);

		end = false;
		ImgMove = false;
		scoreMove = false;
		ImgCnt = 0;
		resultCnt = 0;
		win1P = true;

		imageName[0] = "ResultImg";
		imageName[1] = "RankImg";
		Image_Initialize(img1P, -200, 270 - 110);
		Image_Initialize(img2P, 480+200, 270 - 110);

		Score_Initialize(score1P,80,130);
		Score_Initialize(score2P,310,130);

		Back_Initialize(ResultImg, 0, 270);
		ResultImg.active = true;
		Back_Initialize(RankingImg, 0, -270);
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

	}
	//-----------------------------------------------------------------------------
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag Update()
	{
		auto in1 = DI::GPad_GetState("P1");
		auto in2 = DI::GPad_GetState("P2");

		Back_Update(ResultImg, 0);
		Back_Update(RankingImg, 1);

		if (ImgMove) { ImgCnt++; }

		Image_Update(img1P, 1);
		Image_Update(img2P, 2);

		Score_Update(score1P, finalScore1P);
		Score_Update(score2P, finalScore2P);

		if (finalScore1P > finalScore2P) { win1P = true; }
		else { win1P = false; }

		TaskFlag rtv = Task_Endingfor2;
		if (end) {
			if (in1.B1.down || in2.B1.down) {
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

		Back_Draw(ResultImg, 0);
		Back_Draw(RankingImg, 1);

		//スコア
		Score_Draw(score1P);
		Score_Draw(score2P);


		if (ImgMove) {
			//winner,loser文字
			ML::Box2D drawWin(0, 0, 230, 100);
			ML::Box2D drawLos(0, 0, 230, 100);
			ML::Box2D src(0, 0, 230, 100);
			if (finalScore1P > finalScore2P) {
				drawWin.Offset(10, 20);
				drawLos.Offset(480 - 240, 15);
				DG::Image_Draw("WinnerTxt", drawWin, src);
				DG::Image_Draw("LoserTxt", drawLos, src);
			}
			else if (finalScore1P < finalScore2P) {
				drawWin.Offset(480 - 240, 20);
				drawLos.Offset(10, 15);
				DG::Image_Draw("WinnerTxt", drawWin, src);
				DG::Image_Draw("LoserTxt", drawLos, src);
			}
			else {
				ML::Box2D drawDraw((480 - 230) / 2, 50, 230, 100);
				DG::Image_Draw("DrawImg", drawDraw, src);
			}
		}
		Image_Draw(img1P, win1P);
		Image_Draw(img2P, !win1P);
	}

	//----------------------------------------------------------------------------
	//関数化
	//----------------------------------------------------------------------------

	void Image_Initialize(Chara& c, int x, int y) {
		c.x = x;
		c.y = y;
		c.speed = 5;
	}

	void Image_Update(Chara& c, int type) {
		if (ImgMove) {
			switch (type) {
			case 1:  //1P
				if (ImgCnt >= 60) {
					c.x += c.speed;
					if (c.x >= 0) {
						c.x = 0;
						end = true;
					}
				}
				break;
			case 2:
				if (ImgCnt >= 60) {
					c.x -= c.speed;
					if (c.x <= 280) {
						c.x = 280;
						end = true;
					}
				}
				break;
			}
		}
	}

	void Image_Draw(Chara& c, bool win1P) {
		if (ImgMove) {
			ML::Box2D draw(0, 0, 200, 112);
			draw.Offset(c.x, c.y);
			ML::Box2D src(0, 0, 800, 450);
			if (win1P) {
				DG::Image_Draw("WinnerImg", draw, src);
			}
			else { DG::Image_Draw("LoserImg", draw, src);
			}
		}
	}

	void Score_Initialize(Text& myscore, int x, int y) {
		myscore.textBox = ML::Box2D(x, y, 200, 60);
		myscore.text = to_string(00000);
	}
	void Score_Update(Text& myscore, int score) {
		if (scoreMove) {
			myscore.textBox.y = 130;
			myscore.text = to_string(rand() % 90000 + 10000);
			myscore.cnt++;
			if (myscore.cnt >= 120) {
				myscore.text = to_string(score);
				ImgMove = true;
			}
			
		}
	}
	void Score_Draw(Text& myscore) {
		if (scoreMove) {
			DG::Font_Draw("FontA", myscore.textBox, myscore.text, ML::Color(1.0f, 1.0f, 0.75f, 0.0f));
		}
	}

	//結果発表、ランキング画面
	void Back_Initialize(Back& img, int x, int y) {
		img.x = x;
		img.y = y;
		img.draw = ML::Box2D(img.x, img.y, 480, 270);
		img.src = ML::Box2D(0, 0, 480, 270);
	}

	void Back_Update(Back& img, int type) {
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
					scoreMove = true;
				}
			}
			break;
		}
	}
	void Back_Draw(Back& img, int type) {
		if (img.active) {
			DG::Image_Draw(imageName[type], img.draw, img.src);
		}
	}

}