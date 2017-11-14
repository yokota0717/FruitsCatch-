#pragma once
#include "MyPG.h"

enum TaskFlag {
	Task_Non,      //タスク未選択
	Task_Title,    //タイトル
	Task_HowTofor1,    //遊び方一人用
	Task_HowTofor2,    //遊び方二人用
	Task_Gamefor1, //一人プレイゲーム本編
	Task_Gamefor2, //二人プレイ用 
	Task_Endingfor1,   //エンディング
	Task_Endingfor2,   //エンディング
};

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initialize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );

