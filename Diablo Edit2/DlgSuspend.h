#pragma once

#include "D2Item.h"

// CDlgSuspend �Ի���

class CDlgSuspend : public CDialog
{
	DECLARE_DYNAMIC(CDlgSuspend)
public:
	CDlgSuspend(CWnd* pParent,UINT transparency);   // ��׼���캯��
	virtual ~CDlgSuspend();
// �Ի�������
	enum { IDD = IDD_DIALOG_Suspend };
//�Զ�������
private:
	typedef std::pair<BYTE,CString>	__MsgType;	//<������ɫ,����>
//�Զ�������
	static const COLORREF FONT_COLOR[];			//������ɫ
	enum {WHITE,BLUE,GREEN,RARE,UNIQUE,CRAFT};	//��ɫ����
	static const LONG HEIGHT_PER_LINE = 20;		//ÿ���ֵĸ߶�
	static const LONG WIDTH_PER_CHAR = 9;		//ÿ���ַ��Ŀ��
	static const LONG WINDOW_WIDTH_MIN = 273;	//������С���
	UINT m_nTransparency;       //����͸����
	const CD2Item * m_pItem;
	std::vector<__MsgType> m_sItemMsg;	//��Ʒ����Ϣ
//�Զ��庯��
	void AddMsg(BYTE color, const CString & msg);
public:
	LONG GetItemInfo(const CD2Item * pItem);		//���ش���ĸ߶�
	const CD2Item * GetItemPtr() const{return m_pItem;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
