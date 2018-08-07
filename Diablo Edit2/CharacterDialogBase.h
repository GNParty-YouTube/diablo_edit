#pragma once

#include "D2S_Struct.h"

// CCharacterDialogBase �Ի���

class CCharacterDialogBase : public CDialog
{
	DECLARE_DYNAMIC(CCharacterDialogBase)

public:
	CCharacterDialogBase(UINT nIDTemplate,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCharacterDialogBase();
// �Զ��庯��
	virtual void UpdateUI(const CD2S_Struct & character) = 0;
	virtual BOOL GatherData(CD2S_Struct & character) = 0;
	virtual void ResetAll() = 0;
	virtual void LoadText() = 0;	//���ؿؼ����ַ�������
	void OnOK(){}
	void OnCancel(){}
// �Ի�������

protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
	DECLARE_MESSAGE_MAP()
};
