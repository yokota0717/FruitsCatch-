#include "MyGameMain.h"
#include "MyPG.h"

//ゲーム情報
TaskFlag  actTask, nextTask;



//-----------------------------------------------------------------------------
//外部ファイルより
namespace Title {  //タイトルタスク
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace HowTofor1 {
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace HowTofor2 {
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace Gamefor1 {  //ゲーム本編タスク
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace Gamefor2 {  //ゲーム本編タスク
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace Endingfor1 {  //エンディングタスク
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}
namespace Endingfor2 {  //エンディングタスク
	extern void Initialize();
	extern TaskFlag Update();
	extern void Render2D();
	extern void Finalize();
}

//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initialize( )
{
	//キーボードの入力を受け取るオブジェクトを生成する
	DI::AnalogAxisKB ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
	DI::AnalogAxisKB rs = { DIK_NUMPAD4, DIK_NUMPAD6 ,DIK_NUMPAD8, DIK_NUMPAD2 };
	DI::KeyDatas_KB key={
		{ DIK_Z,DI::But::B1 },{ DIK_X,DI::But::B2 },{ DIK_C,DI::But::B3 },{ DIK_V,DI::But::B4 },
		{ DIK_A,DI::But::SE },{ DIK_S,DI::But::ST },{ DIK_Q,DI::But::L1 },{ DIK_W,DI::But::R1 },
		{ DIK_E,DI::But::L2 },{ DIK_R,DI::But::R2 },{ DIK_D,DI::But::L3 },{ DIK_NUMPAD5,DI::But::R3 },
	};

	//ゲームパッドパラメータ
	DI::KeyDatas_GP but = {
		{ DI::GPB::B01,DI::But::B1 },{ DI::GPB::B02,DI::But::B2 },
		{ DI::GPB::B03,DI::But::B3 },{ DI::GPB::B04,DI::But::B4 },
		{ DI::GPB::B05,DI::But::L1 },{ DI::GPB::B06,DI::But::R1 },
		{ DI::GPB::B07,DI::But::L2 },{ DI::GPB::B08,DI::But::R2 },
		{ DI::GPB::B09,DI::But::SE },{ DI::GPB::B10,DI::But::ST },
		{ DI::GPB::B11,DI::But::L3 },{ DI::GPB::B12,DI::But::R3 },
	};
	DI::GPad_CreateKBGP("P1", ls, rs, key, 0, but);
	DI::GPad_CreateKBGP("P2", ls, rs, key, 1, but);

	actTask = Task_Non;     //初期状態は実行中タスクを無効にしておく
	nextTask = Task_Title;  //次のタスクをタイトルにすることで最初にタイトルが実行


}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize( )
{
	DI::GPad_Erase("P1");
	DI::GPad_Erase("P2");
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate( )
{
	//今のタスクと次のタスクが一致しないとき
	if (actTask != nextTask) {
		//今のタスクの終了処理を呼び出す
		switch (actTask) {
		case Task_Title:
			Title::Finalize();
			break;
		case Task_HowTofor1:
			HowTofor1::Finalize();
			break;
		case Task_HowTofor2:
			HowTofor2::Finalize();
			break;
		case Task_Gamefor1:
			Gamefor1::Finalize();
			break;
		case Task_Gamefor2:
			Gamefor2::Finalize();
			break;
		case Task_Endingfor1:
			Endingfor1::Finalize();
			break;
		case Task_Endingfor2:
			Endingfor2::Finalize();
			break;
		}
		//次のタスクに移る
		actTask = nextTask;
		//次のタスクの初期化処理を呼び出す
		switch(actTask) {
		case Task_Title:
			Title::Initialize();
			break;
		case Task_HowTofor1:
			HowTofor1::Initialize();
			break;
		case Task_HowTofor2:
			HowTofor2::Initialize();
			break;
		case Task_Gamefor1:
			Gamefor1::Initialize();
			break;
		case Task_Gamefor2:
			Gamefor2::Initialize();
			break;
		case Task_Endingfor1:
			Endingfor1::Initialize();
			break;
		case Task_Endingfor2:
			Endingfor2::Initialize();
			break;

		}
	}

	//今のタスクと次のタスクが一致するとき
	//現在のタスクの実行・更新処理を呼び出す
	switch(actTask) {
	case Task_Title:
		nextTask = Title::Update();
		break;
	case Task_HowTofor1:
		nextTask = HowTofor1::Update();
		break;
	case Task_HowTofor2:
		nextTask = HowTofor2::Update();
		break;
	case Task_Gamefor1:
		nextTask = Gamefor1::Update();
		break;
	case Task_Gamefor2:
		nextTask = Gamefor2::Update();
		break;
	case Task_Endingfor1:
		nextTask = Endingfor1::Update();
		break;
	case Task_Endingfor2:
		nextTask = Endingfor2::Update();
		break;
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D( )
{
	switch (actTask) {
	case Task_Title:
		Title::Render2D();
		break;
	case Task_HowTofor1:
		HowTofor1::Render2D();
		break;
	case Task_HowTofor2:
		HowTofor2::Render2D();
		break;
	case Task_Gamefor1:
		Gamefor1::Render2D();
		break;
	case Task_Gamefor2:
		Gamefor2::Render2D();
		break;
	case Task_Endingfor1:
		Endingfor1::Render2D();
		break;
	case Task_Endingfor2:
		Endingfor2::Render2D();
		break;
	}
}
