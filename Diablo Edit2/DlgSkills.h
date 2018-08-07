#pragma once

#include "afxcmn.h"

// CDlgSkills �Ի���

class CDlgSkills : public CDialog
{
	DECLARE_DYNAMIC(CDlgSkills)
public:
	CDlgSkills(int charClass,BYTE * skills,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSkills();

// �Ի�������
	enum { IDD = IDD_DIALOG_Skills };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
// �Զ��庯��
public:
	void LoadText();
// �Զ����Ա
private:
	static const int SKILL_SIZE = CDiabloEdit2App::CLASS_SKILL_NAME_SIZE;
	static const int TAB_SIZE = CDiabloEdit2App::CLASS_SKILL_TAB_SIZE;
	static const int TEXT_SIZE = SKILL_SIZE + TAB_SIZE;
	CString m_sText[TEXT_SIZE];
	int m_nCharClass;			//�����ɫ
	BYTE m_pSkills[SKILL_SIZE];	//���ܵȼ�����
	BYTE * m_pData;
	BYTE m_bAll;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
