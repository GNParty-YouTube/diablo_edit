#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyDialog.h"
#include "DlgSuspend.h"
#include "DlgPrefixSuffix.h"

const int GRID_WIDTH = 30;		//ÿ������ı߳�(����)

//��Ʒ����ͼ
struct CItemView
{
	CD2Item & Item;						//��Ӧ����Ʒ
	const UINT nPicRes;					//bmpͼƬ��Դ����
	int iPosition, iGridX, iGridY;		//��Ʒλ��(CDlgCharItems::EPosition), �Լ���������
	const int iGridWidth, iGridHeight;	//����ռ�������С
	std::vector<CItemView> vGemItems;	//��Ƕ����ƷView
	CItemView(CD2Item & item, int pos, int x, int y);
	CSize ViewSize() const { return CSize(iGridWidth * GRID_WIDTH, iGridHeight*GRID_WIDTH); }
};

////��Ʒ��ͼ--------------------
//struct CItemView1
//{
//	const UINT		nPicRes;			//bmpͼƬ��Դ����
//	const WORD		Range;				//����ռ�������С,x<<4 + y
//	WORD Pos = -1;						//��8λΪλ��,Stash,Inventory...,��8λΪ����(x<<4 + y)
//	const CD2Item & Item;				//��Ӧ����Ʒ
//	std::vector<CItemView1> vGemItems;	//��Ƕ����ƷView
//public:
//	CItemView1(UINT resID,WORD range, const CD2Item & item):nPicRes(resID),Range(range),Item(item){}
//};

////����
//inline WORD MAKE_GRID(int x,int y){return (x << 4) + y;}
//inline WORD MAKE_GRID(int i,int x,int y){return (WORD(i) << 8) + (x << 4) + y;}
//inline int INDEX(WORD grid){return grid >> 8;}
//inline int COL(WORD grid){return (grid >> 4) & 0xF;}
//inline int ROW(WORD grid){return grid & 0xF;}

// CDlgCharItems �Ի���

class CDlgCharItems : public CPropertyDialog
{
	DECLARE_DYNAMIC(CDlgCharItems)

	//����λ��
	enum EPosition {
		STASH,				//����
		INVENTORY,			//�ڴ�
		CUBE,				//����
		IN_BELT,			//������
		IN_SOCKET,			//��Ƕ�ڿ���

		GRID_COUNT,			//��������λ������

		HEAD = GRID_COUNT,	//ͷ
		NECK,				//����
		BODY,				//����
		RIGHT_HAND,			//������(I & II)
		LEFT_HAND,			//������(I & II)
		RIGHT_RING,			//��ָ��
		LEFT_RING,			//��ָ��
		BELT,				//����
		FOOT,				//Ь��
		GLOVE,				//����

		IN_MOUSE,			//���������

		POSITION_END		//����λ������
	};
	const static WORD	POSITION_RECT[POSITION_END][4];	//ÿ��λ��(EPosition)��UI����ʼ����(����),����,����(x,y,col,row)
	CRect m_rectGrid[POSITION_END];						//��POSITION_RECTת����CRect, ����ʹ��
	void DrawGrids(CPaintDC & dc);						//����������
	
	//��Ʒ��λ��
	std::vector<CItemView> m_vItemViews;				//���е���Ʒ,������Ƕ�ڿ����
	std::vector<std::vector<int>> m_vGridItems;			//�����ڵ���Ʒ��m_vItemViews��������-1��ʾû��
	BOOL m_bSecondHand = FALSE;							//�Ƿ���ʾII������
	BOOL UpdateGridItem(int index, int pos, int x, int y);	//��index��Ʒ�Ž�����λ��(pos, x, y)�������Ƿ�ɹ�
	int GetGridItemIndex(int pos, int x, int y) const;		//�õ�����λ�õ���Ʒ��m_vItemViews����, -1��ʾû��
	void SetGridItemIndex(int pos, int x, int y, int index);//��������λ�õ���Ʒ��m_vItemViews1����, -1��ʾû��
	CPoint GetItemPositionXY(const CItemView & view) const;	//�õ���Ʒ��ʵ����������
	
	//����̨
	int m_iSelectedItemIndex = -1;	//ѡ�е���Ʒ��m_vItemViews�е�����
	int m_iSelectedSocketIndex = -1;//ѡ�е���Ʒ��Ƕ�ڿ���ʱ����Ӧ�׵�����(m_iSelectedItemIndex��ʾ���׵���Ʒ)
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
	void ResetFoundry();			//��ʼ������̨

	//������
	CDlgSuspend * m_pDlgItemInfo = 0;	//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;			//�Ƿ�������Ʒ��Ϣ������
	CSliderCtrl m_scTrasparent;			//������������͸����

	//��������
	CString m_sText[14];
	CButton m_btButton[5];

	//���
	CPoint m_pMouse;	//���λ��
public:
	CDlgCharItems(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharItems();
// �Ի�������
	enum { IDD = IDD_DIALOG_CharItems };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	// �Զ��庯��
	// �麯��
	void UpdateUI(CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
private:
	// UI
	void DrawItemXY(CPaintDC & dc, CPoint pos, const CItemView & itemView) const;	//�ھ�������㻭��Ʒ
	void DrawAllItemsInGrid(CPaintDC & dc) const;		//�������ڵ�������Ʒ�����ѡ�е���Ʒ��Ƕ����Ʒ��ҲҪ������
	//WORD HitTestItem(const CPoint & pos,WORD range = MAKE_GRID(1,1));	//������������gridIndex
	////BOOL PutItemInGrid(WORD itemIndex, WORD gridIndex);  //��m_vItemViews1������ΪitemIndex����Ʒ�ŵ���������gridIndex��λ��
	//void ShowItemInfoDlg(const CD2Item * pItem);    //��ʾ/����(pItem = 0)��Ʒ��Ϣ��������
	//void ReadItemProperty(WORD itemIndex);  //��ȡm_vItemViews1������ΪitemIndex����Ʒ�����ԣ�����ʾ�ڶ���̨
    //����
 //   CPoint GRID2XY(WORD gridIndex) const{		//��grid����(��8λΪλ��,��8λΪ����(x<<4 + y))�õ�ʵ����������λ��,ֻ�����ڴ����䣬�ڴ������飬��
	//	int i = INDEX(gridIndex);
	//	ASSERT(i < GRID_NUMBER);
	//	return CPoint(m_rectGrid1[i].left + GRID_WIDTH * COL(gridIndex),m_rectGrid1[i].top + GRID_WIDTH * ROW(gridIndex));
	//}
	//CPoint GRID2XY(WORD gridID,WORD range) const{	//��֪���岿λ���,������Ʒ��С����ͼ��λ�ã�ֻ���������岿λ
	//	ASSERT(gridID >= GRID_NUMBER && GRID_NUMBER < GRID_BODY_NUMBER);
	//	return CPoint(POSITION_RECT[gridID][0] + (POSITION_RECT[gridID][2] - COL(range)) * GRID_WIDTH / 2,
	//		POSITION_RECT[gridID][1] + (POSITION_RECT[gridID][3] - ROW(range)) * GRID_WIDTH / 2);
	//}
	//void SET_GRID_ITEM(int i,int x,int y,int itemIndex){		//��������λ�õ���Ʒm_vItemViews1����,INVALID_ITEM��ʾû��
	//	m_iGridItems1[i][x + y * POSITION_RECT[i][2]] = itemIndex;
	//}
	//void SET_GRID_ITEM(WORD gridIndex,int itemIndex){
	//	SET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex),itemIndex);
	//}
	//int GET_GRID_ITEM(WORD gridIndex) const{		//�õ�����λ�õ���Ʒm_vItemViews1����,INVALID_ITEM��ʾû��
	//	return GET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex));
	//}
	//int GET_GRID_ITEM(int i,int x,int y) const{
	//	return m_iGridItems1[i][x + y * POSITION_RECT[i][2]];
	//}
// �Զ����Ա
private: 
	//װ����Ʒ
	//std::vector<CItemView1> m_vItemViews1;			//���е���Ʒ,���������������
	//CRect m_rectGrid1[GRID_BODY_NUMBER];				//��������,����0,�ڴ�1,����2����3
	//std::vector<std::vector<WORD>> m_iGridItems1;	//�����ڵ���Ʒ��m_vItemViews1����
	//const static WORD	INVALID_ITEM = -1;
	//const static BYTE	GRID_NUMBER = GRID_COUNT;				//�洢�������
	//const static BYTE	GRID_BODY_NUMBER = GRID_NUMBER + 10;    //GRID_NUMBER + ���崩����λ��

	//CString m_sItemName;			//��Ʒ����

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
