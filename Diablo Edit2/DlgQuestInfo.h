#pragma once

#include "PropertyDialog.h"

// CDlgQuestInfo �Ի���

class CDlgQuestInfo : public CPropertyDialog
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
	void UpdateUI(CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);
// �Զ����Ա
private:
	static const int TEXT_SIZE = 31;
	static const WORD QUEST_COMPLETE[27];
	CString m_sText[TEXT_SIZE];
	INT m_nLevel;
	BOOL m_bQuestInfo[3][28];
	BOOL m_bUIData[28];
public:
	afx_msg void OnBnClicked_Normal();
	afx_msg void OnBnClicked_Nightmare();
	afx_msg void OnBnClicked_Hell();
	virtual BOOL OnInitDialog();
};

