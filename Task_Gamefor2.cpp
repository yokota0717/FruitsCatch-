#include "MyPG.h"
#include "MyGameMain.h"
#include "time.h"

extern int finalScore1P, finalScore2P;  //最終的総スコア

//ゲーム本編
namespace Gamefor2 {

	//ゲーム情報

	int remTime;  //残り時間
	int timeCnt;  //時間管理用
	int animCnt;  //残りタイム拡大縮小用
	int count;  //数字変更用

	bool bonus;  //ボーナスタイムかどうか
	int bonusTimeCnt;  //ボーナスタイム中時間管理用
	int bonusTimeMax;  //ボーナスタイムのMAXタイム
	int birdCome;  //鳥が飛んでくるタイミング
	float birdY;  //鳥が曲線を描いて飛ぶ用
	int clovFallX;  //四つ葉が落ち始める位置

	bool kagoHit;  //かご同士が接触しているか

	struct Chara {
		bool active;  //生死
		bool flyL;  //左にはじかれた
		bool flyR;  //右にはじかれた
		bool hit;  //かごに入った
		int x;
		int y;
		int speed;  //かごの移動速度
		int moveCnt;  //
		ML::Box2D hitBase;  //真ん中の当たり判定
		ML::Box2D outHitL;  //左側の当たり判定
		ML::Box2D outHitR;  //右側の当たり判定
		int typeNum;
		int score;  //加算されるスコア
		int totalScore;  //現在の総スコア
		float flyH;  //弾き飛ばされた時のジャンプ力
		int animCnt;  //アニメーション用
	};

	struct Num {
		int x;
		int y;
		ML::Box2D draw;
		ML::Box2D src;
		int drawY;
		int digit;  //何の位の数字か(1:1...6:10万)
	};

	
	Chara kago[2], fruits[100], kemushi[50], bird, clover, bonusF[50], bonusText, hurry, sky, tree, cloud1[2], cloud2[2], cursor, more, owari;
	string fileName[4];
	Num timer[2], score1P[5], score2P[5];

	ML::Box2D scoreBox;  //かごと接触した時のスコア表示用
	bool scoreBoxLive;
	int scoreBoxCnt;
	string scoreT;  //獲得したスコア


	//関数のプロトタイプ宣言
	//かごの初期化
	void Kago_Initialize(Chara& c_, int x_, int y_, int type);
	//かごの表示
	void Kago_Draw(Chara& c_);
	//かごの行動
	void Kago1_Update(Chara& c_);
	void Kago2_Update(Chara& c_);

	//かごと接触した時のスコア表示
	void Score_Initialize(int x_, int y_, int score_);
	void Score_Update();
	void Score_Draw();

	//かごとかごの接触判定
	void Kago_HitCheck(Chara& c);
	//かごと果物の接触判定
	void Fruits_HitCheck(Chara& c_);
	//かごと毛虫の接触判定
	void Kemushi_HitCheck(Chara& c_);
	//かごと四つ葉の接触判定
	void Clover_HitCheck(Chara& c_);

	//かごに弾き飛ばされる
	void FlyAwayL(Chara& c_);
	void FlyAwayR(Chara& c_);

	//果物の初期化
	void Fruits_Initialize(Chara& c_, int tn_, int x_, int y_);
	void Apple_Initialize(Chara& c_, int x_, int y_);
	void AppleG_Initialize(Chara& c_, int x_, int y_);
	void AppleE_Initialize(Chara& c_, int x_, int y_);

	//果物の出現
	void Fruits_Appear(int tn_, int x_, int y_);
	//果物の出現タイミング
	void Fruits_AppearTimer();

	//果物の表示
	void Fruits_Draw(Chara& c_);
	void Apple_Draw(Chara& c_);
	void AppleG_Draw(Chara& c_);
	void AppleE_Draw(Chara& c_);

	//果物の行動
	void Fruits_Update(Chara& c_);
	void Apple_Update(Chara& c_);
	void AppleG_Update(Chara& c_);
	void AppleE_Update(Chara& c_);

	//毛虫の初期化
	void Kemushi_Initialize(Chara& c_, int x_, int y_);
	//毛虫の表示
	void Kemushi_Draw(Chara& c_);
	//毛虫の行動
	void Kemushi_Update(Chara& c_);
	//毛虫の出現
	void Kemushi_Appear(int x_, int y_);
	//毛虫の出現タイミング
	void Kemushi_AppearTimer();

	//鳥の初期化
	void Bird_Initialize(Chara& c_);
	//鳥の行動
	void Bird_Update(Chara& c_);
	//鳥の表示
	void Bird_Draw(Chara& c_);

	//四つ葉の初期化
	void Clover_Initialize(Chara& c_);
	//四つ葉の行動
	void Clover_Update(Chara& c_);
	//四つ葉の表示
	void Clover_Draw(Chara& c_);

	//ボーナス果物の初期化
	void Bonus_Initialize(Chara& c_, int x_, int y_);
	//ボーナス果物の表示
	void Bonus_Draw(Chara& c_);
	//ボーナス果物の行動
	void Bonus_Update(Chara& c_);
	//ボーナス果物の出現
	void Bonus_Appear(int x_, int y_);
	//ボーナス果物の出現タイミング
	void Bonus_AppearTimer();

	//ボーナステキストの初期化
	void BonusText_Initialize(Chara& c_, int x_, int y_);
	//ボーナステキストの行動
	void BonusText_Update(Chara& c_);
	//ボーナステキストの表示
	void BonusText_Draw(Chara& c_);

	//HurryUpテキストの初期化
	void Hurry_Initialize(Chara& c_,int x_, int y_);
	//HurryUpテキストの行動
	void Hurry_Update(Chara& c_);
	//HurryUpテキストの表示
	void Hurry_Draw(Chara& c_);

	//背景の初期化
	void Back_Initialize(Chara& c_,int x_, int y_);
	void Back_Update(Chara& c_, int i);
	//背景の表示
	void Back_Draw(Chara& c_, int type);

	//カーソル
	void Cursor_Initialize(Chara& c, int x, int y);
	void Cursor_Update(Chara& c);
	void Cursor_Draw(Chara& c);


	//もう一回選択肢
	void More_Initialize(Chara&c_);
	void More_Draw(Chara& c_);

	//おわり選択肢
	void Owari_Initialize(Chara&c_);
	void Owari_Draw(Chara& c_);

	//タイマー、スコア用数字
	void Num_Initialize(Num& n, int x, int y, int digit);
	void timer_Update(Num& n, int i);
	void scoreNum_Update(Num& n, int type);
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
		{"1PImg", "./data/image/1P.png"},  //かご
		{"2PImg","./data/image/2P.png"},
		{"AppleImg", "./data/image/apple.png"},  //普通
		{"AppleGImg", "./data/image/appleG.png"},  //黄金
		{"AppleEImg", "./data/image/appleE.png"},  //虫食い
		{"KemushiImg", "./data/image/kemushi.png"},  //けむし
		{"TreeImg", "./data/image/tree.png"},  //木
		{"SkyImg", "./data/image/sky.png"},  //空
		{"Cloud1Img", "./data/image/cloud1.png"},  //雲1
		{"Cloud2Img", "./data/image/cloud2.png"},  //雲2
		{"FinishImg", "./data/image/FINISH.png"},  //終わり文字
		{"MoreImg", "./data/image/more.png"},  //リトライ文字
		{"OwariImg", "./data/image/owari.png"},  //エンド選択肢
		{"BirdImg", "./data/image/blueBird.png"},  //青い鳥
		{"BonusImg", "./data/image/bonustime.png"},  //ボーナスタイム文字
		{"CloverImg", "./data/image/clover.png"},  //よつば
		{"HurryImg", "./data/image/hurry.png"},  //HurryUp文字
		{"TimeImg", "./data/image/time.png"},  //残りタイム文字
		{"NumImg", "./data/image/timer.png"},  //数字
		{"ScoreImg", "./data/image/score.png"},  //スコア文字
		{"CursorImg","./data/image/cursor.png"}
	};

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void  Initialize()
	{
		for (size_t i = 0; i < resname.size(); ++i)
		{
			DG::Image_Create(resname[i].name_, resname[i].path_);
		}

		DG::Font_Create("FontA", "MS ゴシック", 7, 20);

		//乱数表を選択
		srand((unsigned int)time(NULL));

		//オフスクリーン名登録
		fileName[0] = "SkyImg";
		fileName[1] = "Cloud1Img";
		fileName[2] = "Cloud2Img";
		fileName[3] = "TreeImg";

		//いろいろ初期化
		animCnt = 0;
		count = 0;

		kagoHit = false;

		remTime = 30;
		timeCnt = -120;
		finalScore1P = 0;
		finalScore2P = 0;
		bonus = false;
		bonusTimeCnt = 0;
		bonusTimeMax = 600;
		birdCome = rand() % 600 + 300;  //5~15秒のうちどこかで来る
		birdY = 1.5f;
		clovFallX = rand() % 350 + 50;

		for (int i = 0; i < 2; ++i) {
			Kago_Initialize(kago[i], 100+(i*220), (270 - 5) - 45, i);
		}

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
			Num_Initialize(score1P[i], 15 + i * 18, 45, i+1);
		}
		for (int i = 0; i < 5; ++i) {
			Num_Initialize(score2P[i], 480 - 110 + i * 18, 45, i + 1);
		}

		Cursor_Initialize(cursor, 90, 270 - 65);

	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
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
	//更新処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag  Update()
	{
		auto in1 = DI::GPad_GetState("P1");
		auto in2 = DI::GPad_GetState("P2");

		if (!cursor.active) {
			Kago1_Update(kago[0]);
			Kago2_Update(kago[1]);
		}

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
		if (bonusTimeCnt > bonusTimeMax) { bonus = false; }

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


		if (timeCnt >= birdCome) {
			bird.active = true;
			if (!clover.hit) { clover.active = true; }
		}

		Bird_Update(bird);
		Clover_Update(clover);
		FlyAwayL(clover);
		FlyAwayR(clover);
		BonusText_Update(bonusText);
		Score_Update();

		if (timeCnt >= 1200) { hurry.active = true; }
		Hurry_Update(hurry);


		if (timeCnt >= 1860) {
			more.active = true;
			owari.active = true;
			cursor.active = true;
		}
		Cursor_Update(cursor);


		//タイマー更新
		for (int i = 0; i < 2; ++i) { timer_Update(timer[i], i); }
		//スコア更新
		for (int i = 0; i < 5; ++i) { 
			scoreNum_Update(score1P[i], 1);
			scoreNum_Update(score2P[i], 2);
		}

		TaskFlag rtv = Task_Gamefor2;
		if (timeCnt >= 1860) {
			if (cursor.x == 90) {
				if (in1.B1.down || in2.B1.down) {  //もう一回選択肢が押されたら
					rtv = Task_HowTofor2;  //次のタスクをHowToへ
				}
			}
			if (cursor.x == 290) {
				if (in1.B1.down || in2.B1.down) {  //おわり選択肢が押されたら
					finalScore1P = kago[0].totalScore;
					finalScore2P = kago[1].totalScore;
					rtv = Task_Endingfor2;  //次のタスクをエンディングへ
				}
			}
		}
    	return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
	//-----------------------------------------------------------------------------
	void  Render2D()
	{
		//背景
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

		auto in2 = DI::GPad_GetState("P2");
		//かご
		for (int i = 0; i < 2; ++i) { Kago_Draw(kago[i]); }
		//くだものと毛虫
		for (int f = 0; f < 100; ++f) { Fruits_Draw(fruits[f]); }
		for (int k = 0; k < 50; ++k) { Kemushi_Draw(kemushi[k]); }
		//ボーナスくだもの文字
		for (int b = 0; b < 50; ++b) { Bonus_Draw(bonusF[b]); }
		//鳥とかテキスト類
		Clover_Draw(clover);
		Bird_Draw(bird);
		BonusText_Draw(bonusText);
		Hurry_Draw(hurry);
		Score_Draw();

		//スコアの表示
		ML::Box2D drawS1(10, 0, 108, 40);
		ML::Box2D drawS2(480-118, 0, 108, 40);
		ML::Box2D srcS(0, 0, 216, 80);
		DG::Image_Draw("ScoreImg", drawS1, srcS);
		DG::Image_Draw("ScoreImg", drawS2, srcS);
		for (int i = 0; i < 5; ++i) {
			Num_Draw(score1P[i]);
			Num_Draw(score2P[i]);
		}
		if (kago[0].totalScore < 0) {  //スコアがマイナスになった時の対処
			ML::Box2D drawM(5, 18 + 30, 15, 15);
			ML::Box2D srcM(0, 217 * 10, 200, 217);
			DG::Image_Draw("NumImg", drawM, srcM);		
		}
		if (kago[1].totalScore < 0) {  //スコアがマイナスになった時の対処
			ML::Box2D drawM2(480-120, 18 + 30, 15, 15);
			ML::Box2D srcM(0, 217 * 10, 200, 217);
			DG::Image_Draw("NumImg", drawM2, srcM);
		}


		//残り時間の表示
		ML::Box2D drawT((480 - 100) / 2, 0, 100, 50);
		ML::Box2D srcT(0, 0, 200, 100);
		DG::Image_Draw("TimeImg", drawT, srcT);

		for (int i = 0; i < 2; ++i) { Num_Draw(timer[i]); }


		//FINISH
		if (timeCnt >= 1800) {  //終了時
			ML::Box2D draw(65, 65, 350, 140);
			ML::Box2D src(0, 0, 350, 140);
			DG::Image_Draw("FinishImg", draw, src);
		}

		More_Draw(more);
		Owari_Draw(owari);
		Cursor_Draw(cursor);

	}
	//-----------------------------------------------------------------------------
	//関数化
	//-----------------------------------------------------------------------------

	//かごの初期化
	void Kago_Initialize(Chara& c_, int x_, int y_, int type) {
		c_.active = true;
		c_.x = x_;
		c_.y = y_;
		c_.speed = 3;
		c_.typeNum = type;
		c_.hitBase = ML::Box2D(1, 23, 58, 1);
		c_.outHitL = ML::Box2D(0, 0, 1, 45);
		c_.outHitR = ML::Box2D(59, 0, 1, 45);
		c_.totalScore = 0;
	}
	//かごの表示
	void Kago_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 60, 45);
		draw.Offset(c_.x, c_.y);
		ML::Box2D src(0, 0, 400, 300);
		if (c_.typeNum == 0) { DG::Image_Draw("1PImg", draw, src); }
		else{ DG::Image_Draw("2PImg", draw, src); }
	}
	//かごの行動
	void Kago1_Update(Chara& c_) {
		auto in1 = DI::GPad_GetState("P1");

		if (c_.active) {
			//移動速度調整
			if (in1.B1.off) { c_.speed = 3; }
			else if (in1.B1.on) { c_.speed = 6; }
			//移動
			int preX = c_.x;
			if (in1.LStick.L.on) { 
				c_.x -= c_.speed;
				if (c_.x <= 0) { c_.x = 0; }
			}
			if (in1.LStick.R.on) { 
				c_.x += c_.speed;
				if (c_.x >= 480 - 60) {
					c_.x = 480 - 60;
				}
				if (kagoHit) {
					c_.x = preX;
				}
			}	

			Kago_HitCheck(c_);
		}
	}

	void Kago2_Update(Chara& c_) {
		auto in2 = DI::GPad_GetState("P2");

		if (c_.active) {
			//移動速度調整
			if (in2.B1.off) { c_.speed = 3; }
			else if (in2.B1.on) { c_.speed = 6; }
			//移動
			int preX = c_.x;
			if (in2.LStick.L.on) { 
				c_.x -= c_.speed;
				if (c_.x <= 0) { c_.x = 0; }
				if (kagoHit) {
					c_.x = preX;
				}
			}
			if (in2.LStick.R.on) { 
				c_.x += c_.speed;
				if (c_.x >= 480 - 60) {
					c_.x = 480 - 60;
				}
			}
		}
	}

	//接触時のスコア表示
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

	//かごとかごの接触判定(1Pで行う)
	void Kago_HitCheck(Chara& c) {
		if (c.active) {
			//1Pの右側の矩形
			ML::Box2D me = ML::Box2D(0,0,60,45).OffsetCopy(c.x, c.y);
			if (kago[1].active) {
				//2Pの左側の矩形
				ML::Box2D you = ML::Box2D(0,0,60,45).OffsetCopy(kago[1].x, kago[1].y);
				if (you.Hit(me)) {
					kagoHit = true;
				}
				else { kagoHit = false; }
			}
		}
	}


	//かごと果物の接触判定
	void Fruits_HitCheck(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			//果物の矩形
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			for (int i = 0; i < 2; ++i) {
				if (kago[i].active) {
					//かごの中に入ったとき
					ML::Box2D you = kago[i].hitBase.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (you.Hit(me)) {
						c_.active = false;
						kago[i].totalScore += c_.score;
						Score_Initialize(kago[i].x, kago[i].y, c_.score);
						//if (c_.typeNum == 1) {
						//}
					}
					//かごの左縁に当たったとき
					ML::Box2D youL = kago[i].outHitL.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youL.Hit(me)) {
						c_.flyL = true;
					}
					//かごの右縁に当たったとき
					ML::Box2D youR = kago[i].outHitR.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youR.Hit(me)) {
						c_.flyR = true;
					}
				}
			}
		}
	}

	//かごと毛虫の接触判定
	void Kemushi_HitCheck(Chara& c_) {
		if (c_.active && !c_.flyL && !c_.flyR) {
			//果物の矩形
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			for (int i = 0; i < 2; ++i) {
				if (kago[i].active) {
					//かごの中に入ったとき
					ML::Box2D you = kago[i].hitBase.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (you.Hit(me)) {
						c_.active = false;
						kago[i].totalScore += c_.score;
						Score_Initialize(kago[i].x, kago[i].y, c_.score);
					}
					//かごの左縁に当たったとき
					ML::Box2D youL = kago[i].outHitL.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youL.Hit(me)) {
						c_.flyL = true;
					}
					//かごの右縁に当たったとき
					ML::Box2D youR = kago[i].outHitR.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youR.Hit(me)) {
						c_.flyR = true;
					}
				}
			}
		}
	}

	//かごと四つ葉の接触判定
	void Clover_HitCheck(Chara& c_) {
		if (c_.active) {
			//四つ葉の矩形
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			for (int i = 0; i < 2; ++i) {
				if (kago[i].active) {
					//かごの中に入ったとき
					ML::Box2D you = kago[i].hitBase.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (you.Hit(me) && c_.flyL == false && c_.flyR == false) {
						c_.active = false;
						c_.hit = true;
						bonus = true;
						bonusText.active = true;
					}
					//かごの左縁に当たったとき
					ML::Box2D youL = kago[i].outHitL.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youL.Hit(me)) {
						c_.flyL = true;
					}
					//かごの右縁に当たったとき
					ML::Box2D youR = kago[i].outHitR.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youR.Hit(me)) {
						c_.flyR = true;
					}
				}
			}
		}
	}

	//かごとボーナス果物の接触判定
	void Bonus_HitCheck(Chara& c_) {
		if (c_.active) {
			//四つ葉の矩形
			ML::Box2D me = c_.hitBase.OffsetCopy(c_.x, c_.y);
			for (int i = 0; i < 2; ++i) {
				if (kago[i].active) {
					//かごの中に入ったとき
					ML::Box2D you = kago[i].hitBase.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (you.Hit(me)) {
						c_.active = false;
					}
					//かごの左縁に当たったとき
					ML::Box2D youL = kago[i].outHitL.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youL.Hit(me)) {
						c_.flyL = true;
					}
					//かごの右縁に当たったとき
					ML::Box2D youR = kago[i].outHitR.OffsetCopy(kago[i].x, kago[i].y);
					//接触判定
					if (youR.Hit(me)) {
						c_.flyR = true;
					}
				}
			}
		}
	}

	//かごにはじき飛ばされる
	//左方向
	void FlyAwayL(Chara& c_) {
		if (c_.flyL) {
			c_.x -= 6;
			c_.y += (int)c_.flyH;
			c_.flyH += 3;
		}
	}
	//右方向
	void FlyAwayR(Chara& c_) {
		if (c_.flyR) {
			c_.x += 6;
			c_.y += (int)c_.flyH;
			c_.flyH += 3;
		}
	}

	//果物の初期化
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

	//果物の出現
	void Fruits_Appear(int tn_, int x_, int y_) {
		for (int f = 0; f < 100; f++) {
			if (!fruits[f].active) {
				Fruits_Initialize(fruits[f], tn_, x_, y_);
				break;
			}
		}
	}

	//果物の出現タイミング
	void Fruits_AppearTimer() {
		struct sAppearData {
			int tim;
			int typeNumber;
			int x;
			int y;
		};

		sAppearData ad[100];

		//～終了
		if (timeCnt < 1800) {
			//普通
			for (int a = 0; a < 30; ++a) {
				ad[a].tim = rand() % 1650;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//黄金
			for (int g = 30; g < 40; ++g) {
				ad[g].tim = rand() % 1650;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//虫食い
			for (int e = 40; e < 60; ++e) {
				ad[e].tim = rand() % 1650;
				ad[e].typeNumber = 2;
				ad[e].x = rand() % (480 - 30);
				ad[e].y = -33;
			}
		}

		//10秒～増加分
		if (timeCnt >= 600) {
			//普通
			for (int a = 0; a < 40; ++a) {
				ad[a].tim = rand() % 1050 + 600;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//黄金
			for (int g = 40; g < 60; ++g) {
				ad[g].tim = rand() % 1050 + 600;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//虫食い
			for (int e = 60; e < 80; ++e) {
				ad[e].tim = rand() % 1050 + 600;
				ad[e].typeNumber = 2;
				ad[e].x = rand() % (480 - 30);
				ad[e].y = -33;
			}
		}

		//25～終了増加分
		if (timeCnt >= 1500) {
			//普通
			for (int a = 0; a < 50; ++a) {
				ad[a].tim = rand() % 150 + 1500;
				ad[a].typeNumber = 0;
				ad[a].x = rand() % (480 - 30);
				ad[a].y = -33;
			}

			//黄金
			for (int g = 50; g < 70; ++g) {
				ad[g].tim = rand() % 150 + 1500;
				ad[g].typeNumber = 1;
				ad[g].x = rand() % (480 - 30);
				ad[g].y = -33;
			}

			//虫食い
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


	//果物の表示
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

	//果物の行動
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


	//毛虫の初期化
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

	//毛虫の表示
	void Kemushi_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 40, 26);  //画像サイズ634*417
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 634, 417);
			DG::Image_Draw("KemushiImg", draw, src);
		}
	}

	//毛虫の行動
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

	//毛虫の出現
	void Kemushi_Appear(int x_, int y_) {
		for (int k = 0; k < 50; k++) {
			if (!kemushi[k].active) {
				Kemushi_Initialize(kemushi[k], x_, y_);
				break;
			}
		}
	}

	//毛虫の出現タイミング
	void Kemushi_AppearTimer() {
		struct sAppearData {
			int tim;
			int x;
			int y;
		};

		sAppearData ad[50];

		//～20秒
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

	//鳥の初期化
	void Bird_Initialize(Chara& c_) {
		c_.active = false;
		c_.x = 480 + 40 + 30;
		c_.y = 0;
		c_.moveCnt = 0;
		c_.animCnt = 0;
	}
	//鳥の行動
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
	//鳥の表示
	void Bird_Draw(Chara& c_) {
		if (c_.active){
			ML::Box2D draw(0, 0, 40, 25);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 57*(c_.animCnt/10), 113, 57);
			DG::Image_Draw("BirdImg", draw, src);
		}
	}

	//四つ葉の初期化
	void Clover_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 480 + 40;
		c_.y = 50;
		c_.moveCnt = 0;
		c_.hitBase = ML::Box2D(0, 0, 35, 30);
		c_.flyH = -15.f;
	}
	//四つ葉の行動
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
	//四つ葉の表示
	void Clover_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 35, 30);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 492, 432);
			DG::Image_Draw("CloverImg", draw, src);
		}
	}


	//ボーナス果物の初期化
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

	//ボーナス果物の出現
	void Bonus_Appear(int x_, int y_) {
		for (int b = 0; b < 50; ++b) {
			if (!bonusF[b].active) {
				Bonus_Initialize(bonusF[b], x_, y_);
				break;
			}
		}
	}

	//ボーナス果物の表示
	void Bonus_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 30, 33);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 388, 426);
			DG::Image_Draw("AppleGImg", draw, src);
		}
	}

	//ボーナス果物の行動
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

	//ボーナス果物の出現タイミング
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

	//ボーナステキストの初期化
	void BonusText_Initialize(Chara& c_, int x_, int y_) {
		c_.active = false;
		c_.x = x_;
		c_.y = y_;
	}

	//ボーナステキストの行動
	void BonusText_Update(Chara& c_) {
		if (c_.active) {
			c_.x -= 5;
			if (c_.x < -350) {
				c_.active = false;
			}
		}
	}

	//ボーナステキストの表示
	void BonusText_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 350, 67);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 67);
			DG::Image_Draw("BonusImg", draw, src);
		}
	}

	//HurryUpテキストの初期化
	void Hurry_Initialize(Chara& c_, int x_, int y_) {
		c_.active = false;
		c_.x = x_;
		c_.y = y_;
	}
	//HurryUpテキストの行動
	void Hurry_Update(Chara& c_) {
		if (c_.active) {
			c_.x -= 7;
			if (c_.x < 350) {
				c_.active = false;
			}
		}
	}
	//HurryUpテキストの表示
	void Hurry_Draw(Chara& c_) {
		ML::Box2D draw(0, 0, 350, 67);
		draw.Offset(c_.x, c_.y);
		ML::Box2D src(0, 0, 350, 67);
		DG::Image_Draw("HurryImg", draw, src);
	}


	

	//背景の初期化
	void Back_Initialize(Chara& c_, int x_, int y_) {
		c_.active = true;
		c_.x = x_;
		c_.y = y_;
	}

	void Back_Update(Chara& c_, int i) {
		c_.x -= 1*i;
		if (c_.x <= -480) {
			c_.x = 480;
		}
	}

	//背景の表示
	void Back_Draw(Chara& c_, int type) {
		ML::Box2D draw(0, 0, 480, 270);
		draw.Offset(c_.x, 0);
		ML::Box2D src(0, 0, 755, 425);
		DG::Image_Draw(fileName[type], draw, src);
	}

	//カーソル
	void Cursor_Initialize(Chara& c, int x, int y) {
		c.active = false;
		c.x = x;
		c.y = y;
	}
	void Cursor_Update(Chara& c) {
		if (cursor.active) {
			auto in1 = DI::GPad_GetState("P1");
			auto in2 = DI::GPad_GetState("P2");

			if (in1.LStick.L.down || in2.LStick.L.down) {
				if (cursor.x == 90) { c.x = 290; }
				else { cursor.x = 90; }
			}
			if (in1.LStick.R.down || in2.LStick.R.down) {
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


	//もう一回選択肢の初期化
	void More_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 100;
		c_.y = 270 - 60;
		c_.hitBase = ML::Box2D(c_.x, c_.y, 100, 32);
	}
	//もう一回選択肢の表示
	void More_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 100, 32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 110);
			DG::Image_Draw("MoreImg", draw, src);
		}
	}

	//おわり選択肢の初期化
	void Owari_Initialize(Chara& c_) {
		c_.active = false;
		c_.hit = false;
		c_.x = 300;
		c_.y = 270 - 60;
		c_.hitBase = ML::Box2D(c_.x, c_.y, 100, 32);

	}
	//おわり選択肢の表示
	void Owari_Draw(Chara& c_) {
		if (c_.active) {
			ML::Box2D draw(0, 0, 100, 32);
			draw.Offset(c_.x, c_.y);
			ML::Box2D src(0, 0, 350, 110);
			DG::Image_Draw("OwariImg", draw, src);
		}
	}
	

	//数字の関数
	void Num_Initialize(Num& n, int x, int y, int t) {
		n.x = x;
		n.y = y;
		n.digit = t;
		n.draw = ML::Box2D(n.x, n.y, 20, 20);
		n.src = ML::Box2D(0, 0, 0, 0);  //ダミーデータ
		n.drawY = 0;  //ダミーさん
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
					case 0:  //十の位
						if (timeCnt >= 0 && timeCnt < 600) {
							n.src = ML::Box2D(0, 7 * 217, 200, 217);  //200*217 青文字
						}
						else if (timeCnt >= 600 && timeCnt < 1200) {
							n.src = ML::Box2D(0, 8 * 217, 200, 217);  //200*217 青文字
						}
						else {  //残り10秒以降
							n.src = ML::Box2D(200, 9 * 217, 200, 217);  //200*217　赤文字
						}
						break;
					case 1:  //一の位
						if (timeCnt < 1200) {
							n.src = ML::Box2D(0, n.drawY * 217, 200, 217);  //200*217 青文字
						}
						else {  //残り10秒以降
							n.src = ML::Box2D(200, n.drawY * 217, 200, 217);  //200*217　赤文字
						}
						break;
					}
				}

			}
			else {  //ボーナスタイム時
				if (timeCnt < 1800) {
					switch (i) {
					case 0:  //十の位
						if (timeCnt >= 0 && timeCnt < 600) {
							n.src = ML::Box2D(200, 7 * 217, 200, 217);  //200*217 赤文字
						}
						else {
							n.src = ML::Box2D(200, 8 * 217, 200, 217);  //200*217 赤文字

						}
						break;
					case 1:  //一の位
						n.src = ML::Box2D(200, n.drawY * 217, 200, 217);  //200*217 赤文字
						break;
					}
				}

			}
		}
	}

	void scoreNum_Update(Num& n, int type) {
		int remScore;

		switch (type) {
		case 1:
			if (kago[0].totalScore >= 0) { remScore = kago[0].totalScore; }
			else { remScore = -1 * kago[0].totalScore; }
			score1P[0].src = ML::Box2D(0, (9 - remScore / 10000) * 217, 200, 217);
			remScore %= 10000;
			score1P[1].src = ML::Box2D(0, (9 - remScore / 1000) * 217, 200, 217);
			remScore %= 1000;
			score1P[2].src = ML::Box2D(0, (9 - remScore / 100) * 217, 200, 217);
			remScore %= 100;
			score1P[3].src = ML::Box2D(0, (9 - remScore / 10) * 217, 200, 217);
			remScore %= 10;
			score1P[4].src = ML::Box2D(0, (9 - remScore) * 217, 200, 217);
			break;
		case 2:
			if (kago[1].totalScore >= 0) { remScore = kago[1].totalScore; }
			else { remScore = -1 * kago[1].totalScore; }
			score2P[0].src = ML::Box2D(0, (9 - remScore / 10000) * 217, 200, 217);
			remScore %= 10000;
			score2P[1].src = ML::Box2D(0, (9 - remScore / 1000) * 217, 200, 217);
			remScore %= 1000;
			score2P[2].src = ML::Box2D(0, (9 - remScore / 100) * 217, 200, 217);
			remScore %= 100;
			score2P[3].src = ML::Box2D(0, (9 - remScore / 10) * 217, 200, 217);
			remScore %= 10;
			score2P[4].src = ML::Box2D(0, (9 - remScore) * 217, 200, 217);
			break;
		}
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