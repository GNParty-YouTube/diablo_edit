// PropertyDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Diablo Edit2.h"
#include "CharacterDialogBase.h"


// CCharacterDialogBase �Ի���

IMPLEMENT_DYNAMIC(CCharacterDialogBase, CDialog)

CCharacterDialogBase::CCharacterDialogBase(UINT nIDTemplate, CWnd* pParent)
	: CDialog(nIDTemplate, pParent)
{
}

CCharacterDialogBase::~CCharacterDialogBase()
{
}

BEGIN_MESSAGE_MAP(CCharacterDialogBase, CDialog)
END_MESSAGE_MAP()


// CCharacterDialogBase ��Ϣ�������
