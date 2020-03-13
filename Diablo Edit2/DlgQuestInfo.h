#pragma once

#include "CharacterDialogBase.h"

// CDlgQuestInfo �Ի���

class CDlgQuestInfo : public CCharacterDialogBase
{
	DECLARE_DYNAMIC(CDlgQuestInfo)

public:
	CDlgQuestInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgQuestInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_QuestInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
// �Զ��庯��
public:
	void UpdateUI(const CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);
// �Զ����Ա
private:
	static const int QUEST_NAME_SIZE = 29;
	static const int LEVEL_SIZE = 3;
	static const int TEXT_SIZE = QUEST_NAME_SIZE + LEVEL_SIZE;
	static const WORD QUEST_COMPLETE[27];
	CString m_sText[TEXT_SIZE];
	INT m_nLevel;
	BOOL m_bQuestInfo[LEVEL_SIZE][QUEST_NAME_SIZE];
	BOOL m_bUIData[QUEST_NAME_SIZE];
public:
	afx_msg void OnBnClicked_Normal();
	afx_msg void OnBnClicked_Nightmare();
	afx_msg void OnBnClicked_Hell();
};

