#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "PropertyDialog.h"
#include "DlgSuspend.h"
#include "DlgPrefixSuffix.h"

#include <tuple>
#include <memory>

enum EEquip;
enum EPosition;
enum EPositionType;

//��Ʒ����ͼ
struct CItemView
{
	CD2Item & Item;						//��Ӧ����Ʒ
	const UINT nPicRes;					//bmpͼƬ��Դ����
	const EEquip iEquip;				//��Ʒ��װ����λ��
	EPosition iPosition;				//��Ʒλ��
	int iGridX, iGridY;					//��������
	const int iGridWidth, iGridHeight;	//����ռ�������С
	std::vector<CItemView> vGemItems;	//��Ƕ����ƷView
	CItemView(CD2Item & item, EEquip equip, EPosition pos, int x, int y);
	CSize ViewSize() const;
};

//����λ�õ���ͼ
class GridView
{
	std::vector<int> vItemIndex;	//�����ڵ���Ʒ��m_vItemViews���������������Ƕ����Ʒ������CItemView::vGemItems��������-1��ʾû��
	const EPosition iPosition;		//λ������
	const EPositionType iType;		//λ�õ����ͣ�PositionType()
	const int iCol, iRow;			//������
public:
	const CRect Rect;				//��С��λ��
	const EEquip iEquip;			//��װ������Ʒ����
	BOOL bEnabled;					//�Ƿ����ã��ɽ�����
	GridView(EPosition pos, int left, int top, int col, int row);
	BOOL IsGrid() const { return iType == 0; }	//�Ƿ�ֳɵ�������
	BOOL CanEquip(EEquip equip) const { return (equip & iEquip) != 0; }	//�Ƿ�ɴ���
	int ItemIndex(int x, int y) const;			//����ָ���������Ʒ����
	void ItemIndex(int x, int y, int index);	//����ָ���������Ʒ����
	CPoint IndexToXY(int x, int y, int width, int height) const;	//ָ������ʹ�С���õ���Ʒ������λ��
	std::tuple<int, int, int> XYToPositionIndex(CPoint pos, BOOL II, BOOL corpseII) const;	//������ƷUI���أ��õ�λ������������
	BOOL PutItem(int index, int x, int y, int width, int height, EEquip equip);	//����Ʒ�ŵ�ָ��λ�ã����ǿ��С���С���������ͣ������Ƿ�ɹ�
	void Reset();					//�������
};

// CDlgCharItems �Ի���

class CDlgCharItems : public CPropertyDialog
{
	DECLARE_DYNAMIC(CDlgCharItems)

	std::vector<GridView> m_vGridView;		//�����������Ϣ
	void DrawGrids(CPaintDC & dc);			//����������
	
	//��Ʒ��λ��
	std::vector<CItemView> m_vItemViews;					//���е���Ʒ,������Ƕ�ڿ����
	BOOL m_bHasCorpse = FALSE;								//��ʬ��
	BOOL m_bSecondHand = FALSE;								//�Ƿ���ʾII������
	BOOL m_bCorpseSecondHand = FALSE;						//�Ƿ���ʾʬ���II������
	BOOL UpdateGridItem(EPosition pos, int x, int y, int index);	//��index��Ʒ�Ž�����λ��(pos, x, y)�������Ƿ�ɹ�
	int GetGridItemIndex(EPosition pos, int x, int y) const;		//�õ�����λ�õ���Ʒ��m_vItemViews����, -1��ʾû��
	void SetGridItemIndex(EPosition pos, int x, int y, int index);	//��������λ�õ���Ʒ��m_vItemViews1����, -1��ʾû��
	CPoint GetItemPositionXY(const CItemView & view) const;			//�õ���Ʒ��ʵ����������
	
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
	void ReadItemProperty(const CD2Item & item);  //��ȡ��Ʒ�����ԣ�����ʾ�ڶ���̨
	const CItemView * SelectedParentItemView() const;	//��ǰѡ�еĸ���Ʒ��ͼ��û�з���0
	const CItemView * SelectedItemView() const;			//��ǰѡ�е���Ʒ��ͼ��û�з���0

	//������
	static const int INFO_WINDOW_LEFT = 550;	//��������λ��X
	std::unique_ptr<CDlgSuspend> m_pDlgItemInfo;//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;					//�Ƿ�������Ʒ��Ϣ������
	CSliderCtrl m_scTrasparent;					//������������͸����
	void ShowItemInfoDlg(const CD2Item * pItem);//��ʾ/����(pItem = 0)��Ʒ��Ϣ��������

	//��������
	CString m_sText[4];
	//CButton m_btButton[5];

	//���
	CPoint m_pMouse;	//���λ��
	std::tuple<int, int, int> HitTestPosition(CPoint pos) const;	//������XY�õ�����λ��
public:
	CDlgCharItems(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharItems() {};
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
    //afx_msg void OnPrefixSuffix();
	afx_msg void OnChangeCorpseHand();
	afx_msg void OnChangeCorpse();
};
