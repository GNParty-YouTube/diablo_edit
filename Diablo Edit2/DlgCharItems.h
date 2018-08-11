#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "CharacterDialogBase.h"
#include "DlgSuspend.h"

#include <tuple>
#include <memory>

enum EEquip;
enum EPosition;

//��Ʒ����ͼ
struct CItemView
{
	CD2Item Item;						//��Ӧ����Ʒ
	const UINT nPicRes;					//bmpͼƬ��Դ����
	const EEquip iEquip;				//��Ʒ��װ����λ��
	EPosition iPosition;				//��Ʒλ��
	int iGridX, iGridY;					//��������
	const int iGridWidth, iGridHeight;	//����ռ�������С
	std::vector<int> vGemItems;			//��Ƕ����Ʒ��m_vItemViews��������-1��ʾû��
	//Functions:
	CItemView(const CD2Item & item, EEquip equip, EPosition pos, int x, int y);
	CSize ViewSize() const;
	CString ItemName() const { return Item.ItemName(); }
	int GemCount() const;				//��Ƕ�ı�ʯ����
	const CD2Item & UpdateItem(std::vector<CItemView> & vItemViews);	//����UI������Ʒ���ݣ�λ�ã���Ƕ��ʯ�ȣ�
};

//����λ�õ���ͼ
class GridView
{
	std::vector<int> vItemIndex;	//�����ڵ���Ʒ��m_vItemViews��������-1��ʾû��
	const EPosition iPosition;		//λ������
	const int iCol, iRow;			//������
public:
	const CRect Rect;				//��С��λ��
	const EEquip iEquip;			//��װ������Ʒ����
	BOOL bEnabled = TRUE;			//�Ƿ����ã��ɽ�����
	//Functions:
	explicit GridView(EPosition pos);
	BOOL IsGrid() const;			//�Ƿ�ֳɵ�������
	BOOL IsSockets() const;			//�Ƿ�Ϊ��Ƕ�Ŀ�
	BOOL CanEquip(EEquip equip) const { return (equip & iEquip) != 0; }	//�Ƿ�ɴ���
	int ItemIndex(int x, int y) const;			//����ָ���������Ʒ����
	void ItemIndex(int index, int x, int y);	//����ָ���������Ʒ����
	void ItemIndex(int index, int x, int y, int width, int height);	//����ָ������ͷ�Χ����Ʒ����
	CPoint IndexToXY(int x, int y, int width, int height) const;	//ָ������ʹ�С���õ���Ʒ������λ��
	std::tuple<int, int, int> XYToPositionIndex(CPoint pos, BOOL II, BOOL corpseII, int col, int row) const;	//������ƷUI���أ��õ�λ������������
	BOOL PutItem(int index, int x, int y, int width, int height, EEquip equip);	//����Ʒ�ŵ�ָ��λ�ã����ǿ��С���С���������ͣ������Ƿ�ɹ�
	void Reset();					//�������
};

// CDlgCharItems �Ի���

class CDlgCharItems : public CCharacterDialogBase
{
	DECLARE_DYNAMIC(CDlgCharItems)

	std::vector<GridView> m_vGridView;		//�����������Ϣ
	void DrawGrids(CPaintDC & dc);			//����������
	BOOL m_bHasCharacter = FALSE;			//�Ƿ����������
	
	//��Ʒ��λ��
	std::vector<CItemView> m_vItemViews;	//���е���Ʒ,������Ƕ�ڿ����
	BOOL m_bHasCorpse = FALSE;				//�Ƿ���ʬ��
	BOOL m_bSecondHand = FALSE;				//�Ƿ���ʾII������
	CButton m_chCorpseSecondHand;			//�Ƿ���ʾʬ���II������
	void AddItemInGrid(const CD2Item & item, int body);		//����Ʒ��ӵ�������, body: 0-���ﱾ��1-ʬ�壬2-��Ӷ����3-Golem
	void RecycleItemFromGrid(CItemView & view);				//����Ʒ�������Ƴ�
	CPoint GetItemPositionXY(const CItemView & view) const;	//�õ���Ʒ��ʵ����������

	//������
	static const int INFO_WINDOW_LEFT = 50;		//�����������λ��X
	static const int INFO_WINDOW_RIGHT = 550;	//�ұ���������λ��X
	std::unique_ptr<CDlgSuspend> m_pDlgItemInfo;//��ʾ��Ʒ��Ϣ����������
	BOOL m_bNotShowItemInfoDlg;					//�Ƿ�������Ʒ��Ϣ������
	CSliderCtrl m_scTrasparent;					//������������͸����
	void ShowItemInfoDlg(const CD2Item * pItem, int x, int gems);	//��ʾ/����(pItem = 0)��Ʒ��Ϣ�������ڡ�x����ѡ�񴰿�λ�ã�gemsΪ��Ƕ��ʯ����

	//���
	int m_iSelectedItemIndex = -1;		//ѡ�е���Ʒ��m_vItemViews�е�����
	int m_iSelectedSocketIndex = -1;	//ѡ�е���Ʒ��Ƕ�ڿ���ʱ����Ӧ��m_vItemViews�е�����
	int m_iPickedItemIndex = -1;		//��ǰ����������Ʒ��m_vItemViews�е�����
	HCURSOR m_hCursor;					//���
	CPoint m_pMouse;					//���λ��
	CItemView & SelectedParentItemView();//��ǰѡ�еĸ���Ʒ��ͼ
	CItemView & SelectedItemView();		//��ǰѡ�е���Ʒ��ͼ
	std::tuple<int, int, int> HitTestPosition(CPoint pos, int col = 1, int row = 1) const;	//������XY����Ʒ��С�õ�����λ��
	HCURSOR CreateAlphaCursor(const CItemView & itemView);	//����Ʒbmpת���������
	BOOL PutItemInGrid(EPosition pos, int x, int y);		//���Խ����������Ʒ�ŵ�ָ��λ�ã���������꣩

	//��������
	CString m_sText[10];

	//�����˵�
	BOOL m_bClickOnItem = FALSE;		//��ǰ����Ƿ��������Ʒ
	int m_iCopiedItemIndex = -1;		//���Ƶ���Ʒ��m_vItemViews�е�������-1Ϊû��

	//��Ӷ��
	BOOL m_bHasMercenary = FALSE;		//�Ƿ��й�Ӷ��
	CComboBox m_cbMercType;				//��������
	CComboBox m_cbMercName;				//�����������������������仯
	CEdit m_edMercExp;					//����
	CButton m_chMercDead;				//�Ƿ�����
	int m_iMercNameGroup = -1;			//��Ӷ�����������ڵ���
public:
	//�Ի�������
	enum { IDD = IDD_DIALOG_CharItems };
	CDlgCharItems(CWnd* pParent = NULL);
	//�麯��
	void UpdateUI(const CD2S_Struct & character);
	BOOL GatherData(CD2S_Struct & character);
	void ResetAll();
	void LoadText(void);	//���ؿؼ����ַ�������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
private:
	// UI
	void DrawItemXY(CPaintDC & dc, CPoint pos, const CItemView & itemView) const;	//�ھ�������㻭��Ʒ
	void DrawAllItemsInGrid(CPaintDC & dc) const;		//�������ڵ�������Ʒ�����ѡ�е���Ʒ��Ƕ����Ʒ��ҲҪ������

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCheck2();
    afx_msg void OnChangeHand();
	afx_msg void OnChangeCorpseHand();
	afx_msg void OnChangeCorpse();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnChangeMercenary();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnItemImport();
	afx_msg void OnItemExport();
	afx_msg void OnItemCopy();
	afx_msg void OnItemPaste();
	afx_msg void OnItemModify();
	afx_msg void OnItemRemove();
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnCbnSelchangeComboMercType();
};
