#pragma once

#include "PropertyDialog.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgCharBasicInfo �Ի���

class CDlgCharBasicInfo : public CPropertyDialog
{
	DECLARE_DYNAMIC(CDlgCharBasicInfo)

public:
	CDlgCharBasicInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharBasicInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG_BasicCharInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
// �Զ����Ա
private:
	static const DWORD	LEVEL_AND_EXPERIENCE[100];
	static const INT	SKILL_SIZE = 30;	//ÿ������ļ�����Ŀ
	static const INT	TEXT_COUNT = 28;	//�����ı��ؼ�������
	CString m_sText[TEXT_COUNT];
	CButton m_btnSkills;
	CString m_sGoldinPer;
	CString m_sGoldinStash;
	CPropertyDialog	** m_dlgTabPage;
	CTabCtrl m_tcBasicInfo;
	int	m_nTabCurSel,m_nTabPageCount;
	//������Ϣ
	CString m_sVersion;
	CString m_sName;
	CComboBox m_cbCharClass;
	BOOL m_bLadder;
	BOOL m_bExpansion;
	BOOL m_bHardcore;
	BOOL m_bDiedBefore;
	CString m_sCharTitle;
	UINT m_uCharLevel;
	CComboBox m_cbWeaponSet;
	CComboBox m_cbLastDifficult;
	CComboBox m_cbLastACT;
	CTime m_tTime;
	DWORD m_dwExperience;
	CComboBox m_cbLevelAndExp;
	DWORD m_dwStrength;
	DWORD m_dwDexterity;
	DWORD m_dwEnergy;
	DWORD m_dwVitality;
	DWORD m_dwStatPointsRemaining;
	DWORD m_dwSkillPointsRemaining;
	DWORD m_dwMaximumStamina;
	DWORD m_dwCurrentStamina;
	DWORD m_dwMaximumLife;
	DWORD m_dwCurrentLife;
	DWORD m_dwMaximumMana;
	DWORD m_dwCurrentMana;
	DWORD m_dwGoldInBody;
	DWORD m_dwGoldInStash;
	BYTE m_bSkills[SKILL_SIZE];
	DWORD m_dwMaxGoldInBody;
	DWORD m_dwMaxGoldInStash;
// �Զ��庯��
public:
	void UpdateUI(const CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
private:
	void InitUI(void);
	BOOL CheckCharName(void);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked_Skills();
	afx_msg void OnEnChangeLevel();
	afx_msg void OnPaint();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
