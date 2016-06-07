#include "DeleteConfirmDlg.h"
#include "../LoginScene/SelChrDlg.h"
#include <ZipArchive.h>
#include "../../CommonModule/SaveFile.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define DCD_WIDTH	400
#define DCD_HEIGHT	150
//////////////////////////////////////////////////////////////////////////
DeleteConfirmDlg::DeleteConfirmDlg()
{
	SetFixed();
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_rcClient.left = VIEW_WIDTH / 2 - DCD_WIDTH / 2;
	m_rcClient.right = m_rcClient.left + DCD_WIDTH;
	m_rcClient.top = (VIEW_HEIGHT + VIEW_OPERATE) / 2 - DCD_HEIGHT / 2 - 50;
	m_rcClient.bottom = m_rcClient.top + DCD_HEIGHT;
	SetVisible(false);

	AddCommonButton(CBTN_NORMAL, 0, DCD_WIDTH / 2 - 200 / 2 + 75 * 0 + 50 * 0, 90, "确认");
	AddCommonButton(CBTN_NORMAL, 1, DCD_WIDTH / 2 - 200 / 2 + 75 * 1 + 50 * 1, 90, "取消");

	SetWindowTitle("删除确认");
	SetWindowInfo("删除人物后将无法恢复");
	CalcCloseButtonPosition();
	m_nCurSel = 0;
	m_pParent = NULL;
}

DeleteConfirmDlg::~DeleteConfirmDlg()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
bool DeleteConfirmDlg::OnCommonButtonClick(int _id)
{
	if(_id == 1)
	{
		SetVisible(false);
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else if(_id == 0)
	{
		if(pTheGame->GetGameMode() == GM_NORMAL)
		{
			ExecuteDelete();
		}
		else if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			LoginExecuteDelete();
		}
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	return true;
}

void DeleteConfirmDlg::LoginExecuteDelete()
{
	SetVisible(false);

	if(m_nCurSel >= 0 &&
		m_nCurSel <= 2)
	{
		const char* pszHeroName = m_pParent->GetHumName(m_nCurSel);
		if(pszHeroName != NULL)
		{
			if(pszHeroName[0] != 0)
			{
				g_xBuffer.Reset();
				g_xBuffer << (int)0;
				g_xBuffer << (int)PKG_LOGIN_DELGAMEROLE_REQ;
				g_xBuffer << (char)strlen(pszHeroName);
				g_xBuffer.Write(pszHeroName, strlen(pszHeroName));
				SendBufferToLS(g_xBuffer);
			}
		}
	}
}

void DeleteConfirmDlg::ExecuteDelete()
{
	SetVisible(false);

	if(m_nCurSel >= 0 &&
		m_nCurSel <= 2)
	{
		CZipArchive xSave;
		bool bDelete = false;
		char szPath[MAX_PATH];
		sprintf(szPath, "%s\\Save\\hum.sav",
			GetRootPath());

		if(PathFileExists(szPath))
		{
			if(xSave.Open(szPath))
			{
				xSave.SetPassword(SaveFile::CalcInternalPassword());

				char szFile[10];
				strcpy(szFile, "head");
				if(m_nCurSel != 0)
				{
					sprintf(szFile, "head%d",
						m_nCurSel);
				}

				ZIP_INDEX_TYPE zIndex = xSave.FindFile(szFile);
				if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
				{
					xSave.RemoveFile(zIndex);
					bDelete = true;

					strcpy(szFile, "data");
					if(m_nCurSel != 0)
					{
						sprintf(szFile, "data%d",
							m_nCurSel);
					}
					zIndex = xSave.FindFile(szFile);
					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						xSave.RemoveFile(zIndex);
					}
				}

				if(bDelete)
				{
					m_pParent->DeleteHum(m_nCurSel);
				}
				xSave.Close();
			}
		}
	}
}