#pragma once
#include "afxwin.h"


// CDlgPrefixSuffix �Ի���

class CDlgPrefixSuffix : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrefixSuffix)

public:
	CDlgPrefixSuffix(int quality,int * selIndex,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPrefixSuffix();

// �Ի�������
	enum { IDD = IDD_DIALOG_PrefixSuffix };

private:
//�Զ����Ա
    //ǰ��׺����
    CComboBox m_cbFirstName;
    CComboBox m_cbLastName;
    CComboBox m_cbPrefix1;
    CComboBox m_cbSuffix1;
    CComboBox m_cbPrefix2;
    CComboBox m_cbSuffix2;
    CComboBox m_cbPrefix3;
    CComboBox m_cbSuffix3;
    CComboBox m_cbUniqueName;
    CComboBox m_cbSubType;
    //ѡ������
    int m_nQuality;
    int * m_vSelect;
    //��������
    CString m_sText[10];
//�Զ��庯��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};
