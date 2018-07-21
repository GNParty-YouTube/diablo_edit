#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyDialog.h"
#include "DlgSuspend.h"
#include "DlgPrefixSuffix.h"

//��Ʒ��ͼ
struct CItemView
{
	const UINT		nPicRes;			//bmpͼƬ��Դ����
	const WORD		Range;				//����ռ�������С,x<<4 + y
	WORD Pos = -1;						//��8λΪλ��,Stash,Inventory...,��8λΪ����(x<<4 + y)
	CD2Item & Item;						//��Ӧ����Ʒ
	std::vector<CItemView> vGemItems;	//��Ƕ����ƷView
public:
	CItemView(UINT resID,WORD range,CD2Item & item):nPicRes(resID),Range(range),Item(item){}
};

//����
inline WORD MAKE_GRID(int x,int y){return (x << 4) + y;}
inline WORD MAKE_GRID(int i,int x,int y){return (WORD(i) << 8) + (x << 4) + y;}
inline int INDEX(WORD grid){return grid >> 8;}
inline int COL(WORD grid){return (grid >> 4) & 0xF;}
inline int ROW(WORD grid){return grid & 0xF;}

// CDlgCharItems �Ի���

class CDlgCharItems : public CPropertyDialog
{
	DECLARE_DYNAMIC(CDlgCharItems)
//�Զ��峣��
	const static WORD	INVALID_ITEM = -1;
	const static UINT	BMP_INDEX_BASE = IDB_BITMAP0;			//������ƷͼƬ��Դ����ʼ���
	const static BYTE	GRID_WIDTH = 30;						//ÿ��߳�
	const static BYTE	GRID_NUMBER = 4;						//�洢�������
	const static BYTE	GRID_BODY_NUMBER = GRID_NUMBER + 10;    //GRID_NUMBER + ���崩����λ��
	const static WORD	GRID_RECT[GRID_BODY_NUMBER][4];			//������ʼ����,����,����(x,y,col,row)
public:
	CDlgCharItems(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharItems();

// �Ի�������
	enum { IDD = IDD_DIALOG_CharItems };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
// �Զ��庯��
public:
	void UpdateUI(const CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
private:
	// UI
	void DrawGrids(CPaintDC & dc);	//����������
	void DrawItemXY(CPaintDC & dc,CPoint pos, const CItemView & itemView);	//�ھ�������㻭��Ʒ
	void DrawItemsInGrid(CPaintDC & dc);   //�������ڵ�������Ʒ
	WORD HitTestItem(const CPoint & pos,WORD range = MAKE_GRID(1,1));	//������������gridIndex
	BOOL PutItemInGrid(WORD itemIndex, WORD gridIndex);  //��m_vItemViews������ΪitemIndex����Ʒ�ŵ���������gridIndex��λ��
	void ShowItemInfoDlg(const CD2Item * pItem);    //��ʾ/����(pItem = 0)��Ʒ��Ϣ��������
	void ReadItemProperty(WORD itemIndex);  //��ȡm_vItemViews������ΪitemIndex����Ʒ�����ԣ�����ʾ�ڶ���̨
    void ResetFoundry();    //��ʼ������̨
    //����
    CPoint GRID2XY(WORD gridIndex) const{		//��grid����(��8λΪλ��,��8λΪ����(x<<4 + y))�õ�ʵ����������λ��,ֻ�����ڴ����䣬�ڴ������飬��
		int i = INDEX(gridIndex);
		ASSERT(i < GRID_NUMBER);
		return CPoint(m_rectGrid[i].left + GRID_WIDTH * COL(gridIndex),m_rectGrid[i].top + GRID_WIDTH * ROW(gridIndex));
	}
	CPoint GRID2XY(WORD gridID,WORD range) const{	//��֪���岿λ���,������Ʒ��С����ͼ��λ�ã�ֻ���������岿λ
		ASSERT(gridID >= GRID_NUMBER && GRID_NUMBER < GRID_BODY_NUMBER);
		return CPoint(GRID_RECT[gridID][0] + (GRID_RECT[gridID][2] - COL(range)) * GRID_WIDTH / 2,
			GRID_RECT[gridID][1] + (GRID_RECT[gridID][3] - ROW(range)) * GRID_WIDTH / 2);
	}
	void SET_GRID_ITEM(int i,int x,int y,int itemIndex){		//��������λ�õ���Ʒm_vItemViews����,INVALID_ITEM��ʾû��
		m_iGridItems[i][x + y * GRID_RECT[i][2]] = itemIndex;
	}
	void SET_GRID_ITEM(WORD gridIndex,int itemIndex){
		SET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex),itemIndex);
	}
	int GET_GRID_ITEM(WORD gridIndex) const{		//�õ�����λ�õ���Ʒm_vItemViews����,INVALID_ITEM��ʾû��
		return GET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex));
	}
	int GET_GRID_ITEM(int i,int x,int y) const{
		return m_iGridItems[i][x + y * GRID_RECT[i][2]];
	}
// �Զ����Ա
private: 
	//װ����Ʒ
	std::vector<CItemView> m_vItemViews;			//���е���Ʒ,���������������
	CRect m_rectGrid[GRID_BODY_NUMBER];				//��������,����0,�ڴ�1,����2����3
	std::vector<std::vector<WORD>> m_iGridItems;	//�����ڵ���Ʒ��m_vItemViews����
	BOOL m_bSecondHand = FALSE;						//�Ƿ���ʾII������

	//���
	CPoint m_pMouse;				//���λ��

	//����̨
	WORD m_iSelectedItemIndex = INVALID_ITEM;	//��ǰѡ�е���Ʒ��m_vItemViews�е�����
	//CString m_sItemName;			//��Ʒ����
	BYTE m_bItemLevel = 0;			//��Ʒ�ȼ�
	CComboBox m_cbQuality;			//��Ʒ����
	//BOOL m_bItemInscribed;		//�Ѿ����˻�
	CString m_ItemOwner = _T("");	//װ������,�������˻�����������,��������˵�
	BOOL m_bItemSocket = FALSE;		//�п�
	BYTE m_bBaseSocket = 0;			//��������
	BYTE m_bExtSocket = 0;			//�������
	BOOL m_bEthereal = FALSE;		//����,�����޸�
	BOOL m_bIndestructible = FALSE;	//�����ƻ�
	short m_wItemQuantity = 0;		//����,����еĻ�
	short m_wItemDefence = 0;		//����ֵ,����еĻ�
	short m_wCurDurability = 0;		//��ǰ�;ö�
	short m_wMaxDurability = 0;		//����;ö�
	CListCtrl m_lcPropertyList;		//��Ʒ�����б�

	//������
	CDlgSuspend * m_pDlgItemInfo = 0;	//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;			//�Ƿ���ʾ��Ʒ��Ϣ������
	CSliderCtrl m_scTrasparent;			//������������͸����

	//��������
    CString m_sText[13];
    CButton m_btButton[5];

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheck2();
    afx_msg void OnChangeHand();
    afx_msg void OnPrefixSuffix();
};
