#include "../GameScene/GameTextureManager.h"
#include <assert.h>
#include "../Common/GlobalFunction.h"

HGE* GameTextureManager::s_pHGE = NULL;
HDC GameTextureManager::s_HDC = 0;
//////////////////////////////////////////////////////////////////////////
GameTextureManager::GameTextureManager()
{

}

GameTextureManager::~GameTextureManager()
{

}
//	static function
void GameTextureManager::Init(HDC _dc, HGE* _phge)
{
	s_pHGE = _phge;
	s_HDC = _dc;
}