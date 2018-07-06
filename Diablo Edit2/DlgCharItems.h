#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyDialog.h"
#include "DlgSuspend.h"
#include "DlgPrefixSuffix.h"

//��Ʒ��ͼ
struct CItemView
{
	const UINT		nPicRes;		//bmpͼƬ��Դ����
	const WORD		Range;			//����ռ�������С,x<<4 + y
	WORD Pos;						//��8λΪλ��,Stash,Inventory...,��8λΪ����(x<<4 + y)
	CD2Item * const pItem;	        //ָ����Ʒ��ָ��
	CItemView(UINT resID,WORD range,CD2Item * pIT):nPicRes(resID),Range(range),pItem(pIT){}
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
	const static BYTE	GRID_NUMBER = 3;						//�洢�������
	const static BYTE	GRID_BODY_NUMBER = GRID_NUMBER + 10 + 1;//GRID_NUMBER + ���崩����λ�� + ����̨(foundry)
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
	void InitUI();
	void DrawGrid(CPaintDC * pDC);	//������
	void DrawItemXY(CPaintDC * pDC,CPoint pos,CItemView * pVI);	//�ھ�������㻭��Ʒ
	void DrawItemsInGrid(CPaintDC * pDC);   //�������ڵ�������Ʒ
	WORD HitTestItem(const CPoint & pos,WORD range = MAKE_GRID(1,1));	//������������gridIndex
	BOOL PutItemInGrid(WORD itemIndex,WORD gridIndex);  //��m_vpItems������ΪitemIndex����Ʒ�ŵ���������gridIndex��λ��
	void PickItemFromGrid(WORD itemIndex);  //��m_vpItems������ΪitemIndex����Ʒ��������������
	void DestroyAllItems();
	void ShowItemInfoDlg(const CD2Item * pItem);    //��ʾ/����(pItem = 0)��Ʒ��Ϣ��������
	void ReadItemProperty(WORD itemIndex);  //��ȡm_vpItems������ΪitemIndex����Ʒ������(�����˶���̨��)
    HCURSOR CreateAlphaCursor(const CItemView & itemView);  //����Ʒbmpת���������
    void ResetFoundry();    //��ʼ������̨
    //����
    CPoint GRID2XY(WORD gridIndex) const{		//��grid����(��8λΪλ��,��8λΪ����(x<<4 + y))�õ�ʵ����������λ��,ֻ�����ڴ����䣬�ڴ��������
		int i = INDEX(gridIndex);
		ASSERT(i < GRID_NUMBER);
		return CPoint(m_rectGrid[i].left + GRID_WIDTH * COL(gridIndex),m_rectGrid[i].top + GRID_WIDTH * ROW(gridIndex));
	}
	CPoint GRID2XY(WORD gridID,WORD range) const{	//��֪���岿λ��ţ�����̨��ţ�,������Ʒ��С����ͼ��λ�ã�ֻ���������岿λ������̨
		ASSERT(gridID >= GRID_NUMBER && GRID_NUMBER < GRID_BODY_NUMBER);
		return CPoint(GRID_RECT[gridID][0] + (GRID_RECT[gridID][2] - COL(range)) * GRID_WIDTH / 2,
			GRID_RECT[gridID][1] + (GRID_RECT[gridID][3] - ROW(range)) * GRID_WIDTH / 2);
	}
	void SET_GRID_ITEM(int i,int x,int y,int itemIndex){		//��������λ�õ���Ʒm_vpItems����,INVALID_ITEM��ʾû��
		m_iGridItems[i][x + y * GRID_RECT[i][2]] = itemIndex;
	}
	void SET_GRID_ITEM(WORD gridIndex,int itemIndex){
		SET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex),itemIndex);
	}
	int GET_GRID_ITEM(WORD gridIndex) const{		//�õ�����λ�õ���Ʒm_vpItems����,INVALID_ITEM��ʾû��
		return GET_GRID_ITEM(INDEX(gridIndex),COL(gridIndex),ROW(gridIndex));
	}
	int GET_GRID_ITEM(int i,int x,int y) const{
		return m_iGridItems[i][x + y * GRID_RECT[i][2]];
	}
// �Զ����Ա
private: 
	HCURSOR m_hCursor;						//���
	WORD m_iPickedItemIndex;				//��ǰ����������Ʒ��m_vpItems�е�����
	CItemView * m_pPickedItem;				//��ǰ����������Ʒ
	CRect m_rectGrid[GRID_BODY_NUMBER];		//��������,��������0,�ڴ�����1,��������2
	std::vector<std::vector<WORD> >	m_iGridItems;	//�����ڵ���Ʒ��m_vpItems����
	std::vector<CItemView *> m_vpItems;		//���е���Ʒ,���������������
	CPoint m_pMouse;						//���λ��
	CDlgSuspend * m_pDlgItemInfo;			//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;				//ָʾ����ʾ��Ʒ��Ϣ������
    CSliderCtrl m_scTrasparent;             //������������͸����
    BOOL m_bSecondHand;                     //�Ƿ���ʾII������
	//����̨����Ʒ������
	//CString m_sItemName;		//��Ʒ����
	BOOL m_bItemSocket;			//�п�
	BYTE m_bBaseSocket;			//��������
	BOOL m_bEthereal;			//����,�����޸�
	//BOOL m_bItemInscribed;		//�Ѿ����˻�
	BYTE m_bItemLevel;			//��Ʒ�ȼ�
	CString m_ItemOwner;		//װ������,�������˻�����������,��������˵�
	short m_wItemQuantity;		//����,����еĻ�
	short m_wItemDefence;		//����ֵ,����еĻ�
	short m_wMaxDurability;		//����;ö�
	BOOL m_bIndestructible;		//�����ƻ�
	short m_wCurDurability;		//��ǰ�;ö�
	CComboBox m_cbQuality;		//��Ʒ����
	CListCtrl m_lcPropertyList;	//��Ʒ�����б�
	BYTE m_bExtSocket;			//�������
    //��������
    CString m_sText[12];
    CButton m_btButton[5];

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheck2();
    afx_msg void OnChangeHand();
    afx_msg void OnPrefixSuffix();
};
