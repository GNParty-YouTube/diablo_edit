#pragma once
#include "afxwin.h"


// CDlgSelectChar �Ի���

class CDlgSelectChar : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectChar)

public:
	explicit CDlgSelectChar(int & sel,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSelectChar();

// �Ի�������
	enum { IDD = IDD_DIALOG_SelectChar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//�Զ����Ա
private:
	int & m_nSel;
	CComboBox m_cbSelectChar;
//�Զ��庯��
public:
	void OnOK(){
		m_nSel = m_cbSelectChar.GetCurSel();
		CDialog::OnOK();
	}

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
