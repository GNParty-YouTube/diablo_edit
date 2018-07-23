#pragma once

#include "PropertyDialog.h"
#include "afxwin.h"

// CDlgWayPoints �Ի���

class CDlgWayPoints : public CPropertyDialog
{
	DECLARE_DYNAMIC(CDlgWayPoints)

public:
	CDlgWayPoints(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWayPoints();

// �Ի�������
	enum { IDD = IDD_DIALOG_WayPoints };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
// �Զ��庯��
public:
	void UpdateUI(CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
private:
	void GetOriginData(int level,BOOL * way);
// �Զ����Ա
private:
	static const int TEXT_SIZE = 43;
	CString m_sText[TEXT_SIZE];
	INT m_nLevel;	//��ǰѡ����Ѷȼ���,0,1,2
	BYTE m_byteOriginData[3][5];
	BOOL m_bWayData[3][40];
	BOOL m_bUIData[40];
	CButton m_nSelAll;
public:
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedNightmare();
	afx_msg void OnBnClickedHell();
	afx_msg void OnBnClicked_SelAll();
	virtual BOOL OnInitDialog();
};
