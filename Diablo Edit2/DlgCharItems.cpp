// DlgCharItems.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "Diablo Edit2.h"
#include "DlgCharItems.h"

using namespace std;

//Popup menu item IDs
#define ID_ITEM_IMPORT                  100
#define ID_ITEM_EXPORT                  101
#define ID_ITEM_COPY                    102
#define ID_ITEM_PASTE                   103
#define ID_ITEM_MODIFY                  104
#define ID_ITEM_REMOVE                  105

const int GRID_WIDTH = 30;	//ÿ������ı߳�(����)

//��Ʒ��װ����λ��
enum EEquip {
	E_ANY = -1,				//�ɽ���������Ʒ

	E_STORAGE = 1,			//���ɴ�����ֻ�ܷ��ڴ洢����
	E_HEAD = 1 << 1,		//ͷ��
	E_NECK = 1 << 2,		//����
	E_BODY = 1 << 3,		//�·�
	E_HAND = 1 << 4,		//�����Ͷ���
	E_RING = 1 << 5,		//��ָ
	E_BELT = 1 << 6,		//����
	E_FOOT = 1 << 7,		//Ь��
	E_GLOVE = 1 << 8,		//����
	E_IN_BELT = 1 << 9,		//���������ҩƷ�ȣ�
	E_SOCKET = 1 << 10,		//����Ƕ���鱦�����ĵȣ�
};

static EEquip ItemToEquip(const CItemMetaData & meta) {
	if (0 == meta.Equip)
		return (meta.IsGem ? E_SOCKET : E_STORAGE);
	ASSERT(meta.Equip <= 10);
	return EEquip(1 << meta.Equip);
}

//��������λ��
enum EPosition {
	STASH,				//����
	INVENTORY,			//�ڴ�
	CUBE,				//����
	IN_BELT,			//������
	IN_SOCKET,			//��Ƕ�ڿ���
	GRID_COUNT,

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

	CORPSE_HEAD,		//ʬ���ͷ
	CORPSE_NECK,		//ʬ�������
	CORPSE_BODY,		//ʬ�������
	CORPSE_RIGHT_HAND,	//ʬ���������(I & II)
	CORPSE_LEFT_HAND,	//ʬ���������(I & II)
	CORPSE_RIGHT_RING,	//ʬ��Ľ�ָ��
	CORPSE_LEFT_RING,	//ʬ��Ľ�ָ��
	CORPSE_BELT,		//ʬ�������
	CORPSE_FOOT,		//ʬ���Ь��
	CORPSE_GLOVE,		//ʬ�������
	CORPSE_END,

	MERCENARY_HEAD = CORPSE_END,//��Ӷ����ͷ
	MERCENARY_BODY,				//��Ӷ��������
	MERCENARY_RIGHT_HAND,		//��Ӷ����������
	MERCENARY_LEFT_HAND,		//��Ӷ����������
	MERCENARY_END,

	GOLEM = MERCENARY_END,		//���ɽ���ʯħ����Ʒ

	POSITION_END,		//��������λ������

	IN_MOUSE = POSITION_END,	//���������
};

static BOOL IsCorpse(EPosition pos) { return CORPSE_HEAD <= pos && pos < CORPSE_END; }

static BOOL IsInMouse(EPosition pos) { return IN_MOUSE == pos; }

static BOOL IsInSocket(EPosition pos) { return IN_SOCKET == pos; }

//λ������
enum EPositionType {
	PT_STORAGE,		//�洢��
	PT_IN_SOCKET,	//��Ƕ�Ŀ�
	PT_WHOLE,		//����һ��
	PT_II,			//�����֣���I��II
	PT_CORPSE_II,	//ʬ��������֣���I��II
};

static EPositionType PositionType(EPosition pos) {
	if (IN_SOCKET == pos)
		return PT_IN_SOCKET;
	if (pos < GRID_COUNT)
		return PT_STORAGE;
	if (RIGHT_HAND == pos || LEFT_HAND == pos)
		return PT_II;
	if (CORPSE_RIGHT_HAND == pos || CORPSE_LEFT_HAND == pos)
		return PT_CORPSE_II;
	return PT_WHOLE;
}

static BOOL IsGrid(EPositionType type) { return (PT_STORAGE == type || PT_IN_SOCKET == type); }

static BOOL IsSockets(EPositionType type) { return (PT_IN_SOCKET == type); }

static BOOL HasII(EPositionType type) { return (PT_II == type || PT_CORPSE_II == type); }

//ÿ��λ��(EPosition)��UI����ʼ����(����),����,����
//left,top,col,row,equip
const int POSITION_INFO[POSITION_END][5] = {
	{10,5,6,8,E_ANY},		//����
	{10,255,10,4,E_ANY},	//�ڴ�
	{320,255,3,4,E_ANY},	//����
	{420,255,4,4,E_IN_BELT},//������
	{70,385,6,1,E_SOCKET},	//��

	{300,5,2,2,E_HEAD},		//ͷ
	{365,30,1,1,E_NECK},	//����
	{300,70,2,3,E_BODY},	//����
	{200,30,2,4,E_HAND},	//������
	{400,30,2,4,E_HAND},	//������
	{265,165,1,1,E_RING},	//��ָ��
	{365,165,1,1,E_RING},	//��ָ��
	{300,165,2,1,E_BELT},	//����		
	{400,155,2,2,E_FOOT},	//Ь��
	{200,155,2,2,E_GLOVE},	//����

	{660,35,2,2,E_HEAD},	//ʬ���ͷ
	{725,60,1,1,E_NECK},	//ʬ�������
	{660,100,2,3,E_BODY},	//ʬ�������
	{560,60,2,4,E_HAND},	//ʬ���������
	{760,60,2,4,E_HAND},	//ʬ���������
	{625,195,1,1,E_RING},	//ʬ��Ľ�ָ��
	{725,195,1,1,E_RING},	//ʬ��Ľ�ָ��
	{660,195,2,1,E_BELT},	//ʬ�������		
	{760,185,2,2,E_FOOT},	//ʬ���Ь��
	{560,185,2,2,E_GLOVE},	//ʬ�������

	{660,285,2,2,E_HEAD},	//��Ӷ����ͷ
	{660,350,2,3,E_BODY},	//��Ӷ��������
	{560,320,2,4,E_HAND},	//��Ӷ����������
	{760,320,2,4,E_HAND},	//��Ӷ����������

	{850,35,2,4,E_ANY},		//���ɽ���ʯħ����Ʒ
};

static CRect PositionToRect(EPosition pos) {
	ASSERT(pos < POSITION_END);
	auto & p = POSITION_INFO[pos];
	return CRect(p[0], p[1], p[0] + GRID_WIDTH * p[2], p[1] + GRID_WIDTH * p[3]);
}

static EEquip PositionToCol(EPosition pos) {
	ASSERT(pos < POSITION_END);
	return EEquip(POSITION_INFO[pos][2]);
}

static EEquip PositionToRow(EPosition pos) {
	ASSERT(pos < POSITION_END);
	return EEquip(POSITION_INFO[pos][3]);
}

static EEquip PositionToEquip(EPosition pos) {
	ASSERT(pos < POSITION_END);
	return EEquip(POSITION_INFO[pos][4]);
}

//body: 0-���ﱾ��1-ʬ�壬2-��Ӷ����3-Golem
tuple<EPosition, int, int> ItemToPosition(int iLocation, int iPosition, int iColumn, int iRow, int iStoredIn, int body) {
	if (3 == body)
		return make_tuple(GOLEM, 0, 0);
	int pos = -1, x = 0, y = 0;	//��Ʒ��λ��(EPosition)������
	switch (iLocation) {
		case 0:		//grid
			pos = (iStoredIn == 1 ? INVENTORY : (iStoredIn == 4 ? CUBE : (iStoredIn == 5 ? STASH : -1)));
			x = iColumn;
			y = iRow;
			break;
		case 1:		//equipped
			if (0 < iPosition && iPosition <= 12) {
				x = (iPosition <= 10 ? 0 : 1);
				pos = iPosition  + GRID_COUNT - 1 - 7 * x;
				if (1 == body)
					pos += CORPSE_HEAD - HEAD;
				else if (body == 2) {
					switch (pos) {
						case HEAD:pos = MERCENARY_HEAD; break;
						case BODY:pos = MERCENARY_BODY; break;
						case RIGHT_HAND:pos = MERCENARY_RIGHT_HAND; break;
						case LEFT_HAND:pos = MERCENARY_LEFT_HAND; break;
						default:ASSERT(FALSE && _T("Invalid body position of mercenary"));
					}
				}
			}
			break;
		case 2:		//in belt(��Ʒ���з�ʽ����������ͬ)
			pos = IN_BELT;
			x = iColumn % 4;
			y = POSITION_INFO[pos][3] - iColumn / 4 - 1;
			break;
		case 4:		//in hand(���)
			pos = IN_MOUSE;
			break;
		default:;
	}
	if (pos < 0)
		ASSERT(FALSE && _T("Invalid item position"));
	return make_tuple(EPosition(pos), x, y);
}

//struct CItemView

CItemView::CItemView(CD2Item & item, EEquip equip, EPosition pos, int x, int y)
	: Item(item)
	, nPicRes(IDB_BITMAP0 + item.MetaData().PicIndex)
	, iEquip(equip)
	, iPosition(pos)
	, iGridX(x)
	, iGridY(y)
	, iGridWidth((item.MetaData().Range >> 4) & 0xF)
	, iGridHeight(item.MetaData().Range & 0xF)
{
	ASSERT(0 < iGridWidth && 0 < iGridHeight);
	vGemItems.resize(item.Sockets(), -1);
	ASSERT(int(vGemItems.size()) <= PositionToCol(IN_SOCKET));
}

CSize CItemView::ViewSize() const { return CSize(iGridWidth * GRID_WIDTH, iGridHeight*GRID_WIDTH); }

//struct GridView

GridView::GridView(EPosition pos)
	: iPosition(pos)
	, iType(PositionType(pos))
	, iCol(PositionToCol(pos))
	, iRow(PositionToRow(pos))
	, Rect(PositionToRect(pos))
	, iEquip(PositionToEquip(pos))
{
	vItemIndex.resize((IsGrid() ? iCol * iRow : (::HasII(iType) ? 2 : 1)), -1);
}

BOOL GridView::IsGrid() const { return ::IsGrid(iType); }

BOOL GridView::IsSockets() const { return ::IsSockets(iType); }

int GridView::ItemIndex(int x, int y) const {
	ASSERT(0 <= x && 0 <= y);
	const UINT idx = x + y * iCol;
	ASSERT(idx < vItemIndex.size());
	return vItemIndex[idx];
}

void GridView::ItemIndex(int index, int x, int y) {
	ASSERT(0 <= x && x < iCol);
	ASSERT(0 <= y && y < iRow);
	const UINT idx = x + y * iCol;
	ASSERT(idx < vItemIndex.size());
	vItemIndex[idx] = index;
}

void GridView::ItemIndex(int index, int x, int y, int width, int height) {
	ASSERT(0 < width && 0 < height);
	if (IsGrid())
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
				ItemIndex(index, x + i, y + j);
	else
		ItemIndex(index, x, y);
}

CPoint GridView::IndexToXY(int x, int y, int width, int height) const {
	ASSERT(0 <= x && 0 <= y);
	ASSERT(0 < width && width <= iCol);
	ASSERT(0 < height && height <= iRow);
	if(IsGrid())
		return CPoint(Rect.left + x * GRID_WIDTH, Rect.top + y * GRID_WIDTH);
	x = Rect.left + (iCol - width) * GRID_WIDTH / 2;
	y = Rect.top + (iRow - height) * GRID_WIDTH / 2;
	return CPoint(x, y);
}

tuple<int, int, int> GridView::XYToPositionIndex(CPoint pos, BOOL II, BOOL corpseII, int col, int row) const {
	ASSERT(0 <= pos.x && 0 <= pos.y);
	ASSERT(0 < col && 0 < row);
	if (IsGrid()) {
		int x = (pos.x - (col - 1) * GRID_WIDTH / 2 - Rect.left) / GRID_WIDTH;
		int y = (pos.y - (row - 1) * GRID_WIDTH / 2 - Rect.top) / GRID_WIDTH;
		ASSERT(0 <= x && x < iCol);
		ASSERT(0 <= y && y < iRow);
		return make_tuple(iPosition, x, y);
	} else if(PT_II == iType){
		return make_tuple(iPosition, (II ? 1 : 0), 0);
	} else if (PT_CORPSE_II == iType) 
		return make_tuple(iPosition, (corpseII ? 1 : 0), 0);
	return make_tuple(iPosition, 0, 0);
}

BOOL GridView::PutItem(int index, int x, int y, int width, int height, EEquip equip) {
	ASSERT(0 <= index);
	ASSERT(0 <= x && 0 <= y);
	ASSERT(0 < width && 0 < height);
	if (!CanEquip(equip))
		return FALSE;	//���ܴ����ڴ�λ��
	if (IsGrid()) {
		if (x + width > iCol || y + height > iRow)
			return FALSE;	//��Ʒ����������
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
				if (ItemIndex(x + i, y + j) >= 0)
					return FALSE;	//����������Ʒ
		ItemIndex(index, x, y, width, height);
	} else {
		if (ItemIndex(x, y) >= 0)
			return FALSE;	//����������Ʒ
		ItemIndex(index, x, y);
	}
	return TRUE;
}

void GridView::Reset() {
	bEnabled = !IsCorpse(iPosition);
	fill(vItemIndex.begin(), vItemIndex.end(), -1);
}

// CDlgCharItems �Ի���

IMPLEMENT_DYNAMIC(CDlgCharItems, CPropertyDialog)

CDlgCharItems::CDlgCharItems(CWnd* pParent /*=NULL*/)
    : CPropertyDialog(CDlgCharItems::IDD, pParent)
{
	//���
	m_hCursor = ::LoadCursor(0, IDC_ARROW);
	//��ʼ����������
	for(int i = STASH;i < POSITION_END;++i)
		m_vGridView.emplace_back(EPosition(i));
	//����ʬ�岿λ
	for (int i = CORPSE_HEAD; i < CORPSE_END; ++i)
		m_vGridView[i].bEnabled = m_bHasCorpse;
 }

void CDlgCharItems::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_x, m_pMouse.x);
	DDX_Text(pDX, IDC_STATIC_y, m_pMouse.y);
	DDX_Check(pDX, IDC_CHECK2, m_bNotShowItemInfoDlg);
	//DDX_Control(pDX, IDC_LIST1, m_lcPropertyList);
	//DDX_Control(pDX, IDC_COMBO1, m_cbQuality);
	////DDX_Text(pDX, IDC_EDIT1, m_sItemName);
	//DDX_Text(pDX, IDC_EDIT2, m_bItemLevel);
	////DDX_Check(pDX, IDC_CHECK3, m_bItemInscribed);
	//DDX_Text(pDX, IDC_EDIT4, m_ItemOwner);
	//DDV_MaxChars(pDX, m_ItemOwner, 15);
	//DDX_Check(pDX, IDC_CHECK5, m_bItemSocket);
	//DDX_Text(pDX, IDC_EDIT5, m_bBaseSocket);
	//DDX_Text(pDX, IDC_EDIT7, m_bExtSocket);
	//DDX_Text(pDX, IDC_EDIT8, m_wItemQuantity);
	//DDX_Text(pDX, IDC_EDIT9, m_wItemDefence);
	//DDX_Check(pDX, IDC_CHECK6, m_bEthereal);
	//DDX_Check(pDX, IDC_CHECK7, m_bIndestructible);
	//DDX_Text(pDX, IDC_EDIT12, m_wCurDurability);
	//DDX_Text(pDX, IDC_EDIT44, m_wMaxDurability);
	DDX_Control(pDX, IDC_SLIDER1, m_scTrasparent);
	DDX_Check(pDX, IDC_CHECK1, m_bSecondHand);
	DDX_Check(pDX, IDC_CHECK4, m_bCorpseSecondHand);
	DDX_Check(pDX, IDC_CHECK_Corpse, m_bHasCorpse);
	DDX_Check(pDX, IDC_CHECK_Mercenary, m_bHasMercenary);
	//   DDX_Text(pDX, IDC_STATIC1, m_sText[0]);
	//   DDX_Text(pDX, IDC_STATIC2, m_sText[1]);
	//   DDX_Text(pDX, IDC_STATIC3, m_sText[2]);
	//   DDX_Text(pDX, IDC_STATIC4, m_sText[3]);
	//   DDX_Text(pDX, IDC_STATIC5, m_sText[4]);
	//   DDX_Text(pDX, IDC_STATIC6, m_sText[5]);
	//   DDX_Text(pDX, IDC_STATIC7, m_sText[6]);
	//   DDX_Text(pDX, IDC_STATIC8, m_sText[7]);
	//   DDX_Text(pDX, IDC_CHECK2, m_sText[8]);
	//   DDX_Text(pDX, IDC_CHECK5, m_sText[9]);
	//   DDX_Text(pDX, IDC_CHECK6, m_sText[10]);
	//DDX_Text(pDX, IDC_CHECK7, m_sText[11]);
	DDX_Text(pDX, IDC_STATIC_Sockets, m_sText[0]);
	DDX_Text(pDX, IDC_STATIC_Golem, m_sText[1]);
	DDX_Text(pDX, IDC_STATIC_Cube, m_sText[2]);
	DDX_Text(pDX, IDC_STATIC_Belt, m_sText[3]);
	DDX_Text(pDX, IDC_CHECK_Corpse, m_sText[4]);
	DDX_Text(pDX, IDC_CHECK_Mercenary, m_sText[5]);
	//   DDX_Control(pDX, IDC_BUTTON1, m_btButton[0]);
	//   DDX_Control(pDX, IDC_BUTTON2, m_btButton[1]);
	//   DDX_Control(pDX, IDC_BUTTON3, m_btButton[2]);
	//   DDX_Control(pDX, IDC_BUTTON4, m_btButton[3]);
	//DDX_Control(pDX, IDC_BUTTON5, m_btButton[4]);
}

BEGIN_MESSAGE_MAP(CDlgCharItems, CDialog)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_CHECK2, &CDlgCharItems::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_CHECK1, &CDlgCharItems::OnChangeHand)
    //ON_BN_CLICKED(IDC_BUTTON2, &CDlgCharItems::OnPrefixSuffix)
    ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK4, &CDlgCharItems::OnChangeCorpseHand)
	ON_BN_CLICKED(IDC_CHECK_Corpse, &CDlgCharItems::OnChangeCorpse)
	ON_BN_CLICKED(IDC_CHECK_Mercenary, &CDlgCharItems::OnChangeMercenary)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ITEM_IMPORT, &CDlgCharItems::OnItemImport)
	ON_COMMAND(ID_ITEM_EXPORT, &CDlgCharItems::OnItemExport)
	ON_COMMAND(ID_ITEM_COPY, &CDlgCharItems::OnItemCopy)
	ON_COMMAND(ID_ITEM_PASTE, &CDlgCharItems::OnItemPaste)
	ON_COMMAND(ID_ITEM_MODIFY, &CDlgCharItems::OnItemModify)
	ON_COMMAND(ID_ITEM_REMOVE, &CDlgCharItems::OnItemRemove)
END_MESSAGE_MAP()

void CDlgCharItems::UpdateUI(CD2S_Struct & character) {
	ResetAll();
	//Character items
	for (auto & item : character.ItemList.vItems) 
		AddItemInGrid(item, 0);
	//Corpse items
	if (character.stCorpse.pCorpseData.exist()) {
		if (!m_bHasCorpse)
			OnChangeCorpse();
		for (auto & item : character.stCorpse.pCorpseData->stItems.vItems)
			AddItemInGrid(item, 1);
	}
	//Mercenary items
	if (character.stMercenary.stItems.exist()) {
		if (!m_bHasMercenary)
			OnChangeMercenary();
		for (auto & item : character.stMercenary.stItems->vItems)
			AddItemInGrid(item, 2);
	}
	//Golem
	if (character.stGolem.pItem.exist())
		AddItemInGrid(*character.stGolem.pItem, 3);

	m_bHasCharacter = TRUE;
	Invalidate();
}

void CDlgCharItems::AddItemInGrid(CD2Item & item, int body) {
	EEquip equip = ItemToEquip(item.MetaData());
	auto t = ItemToPosition(item.iLocation, item.iPosition, item.iColumn, item.iRow, item.iStoredIn, body);
	EPosition pos = get<0>(t);
	const int x = get<1>(t), y = get<2>(t);
	const int index = m_vItemViews.size();
	 m_vItemViews.emplace_back(item, equip, pos, x, y);
	if (::IsInMouse(pos)) {
		ASSERT(m_iPickedItemIndex < 0);
		m_iPickedItemIndex = index;
		m_hCursor = CreateAlphaCursor(m_vItemViews[index]);
	}else if (!m_vGridView[pos].PutItem(index, x, y, m_vItemViews[index].iGridWidth, m_vItemViews[index].iGridHeight, equip))
		ASSERT(FALSE && _T("Cannot put item in grid"));
	//Sockets & Gems
	if (!item.aGemItems.empty()) {
		ASSERT(item.aGemItems.size() <= m_vItemViews[index].vGemItems.size());
		int i = 0;
		for (auto & gem : item.aGemItems) {
			m_vItemViews[index].vGemItems[i] = m_vItemViews.size();
			m_vItemViews.emplace_back(gem, ItemToEquip(gem.MetaData()), IN_SOCKET, i++, 0);
		}
	}
}

CPoint CDlgCharItems::GetItemPositionXY(const CItemView & view) const {
	ASSERT(view.iPosition < POSITION_END);
	return m_vGridView[view.iPosition].IndexToXY(view.iGridX, view.iGridY, view.iGridWidth, view.iGridHeight);
}

CItemView & CDlgCharItems::SelectedParentItemView() {
	ASSERT(0 <= m_iSelectedItemIndex && m_iSelectedItemIndex < int(m_vItemViews.size()));
	return m_vItemViews[m_iSelectedItemIndex];
}

//const CItemView * CDlgCharItems::SelectedItemView() const {
//	const auto parent = SelectedParentItemView();
//	if(parent && 0 <= m_iSelectedSocketIndex && m_iSelectedSocketIndex < int(parent->vGemItems.size()))
//		return &parent->vGemItems[m_iSelectedSocketIndex];
//	return parent;
//}

//��һ����������
static void DrawGrid(CPaintDC & dc, const CRect & rect, int intervalX = 0, int intervalY = 0)
{
	if (intervalX == 0)
		intervalX = rect.Width();
	if (intervalY == 0)
		intervalY = rect.Height();
	for (int x = rect.left; x <= rect.right; x += intervalX) {
		dc.MoveTo(x, rect.top);
		dc.LineTo(x, rect.bottom);
	}
	for (int y = rect.top; y <= rect.bottom; y += intervalY) {
		dc.MoveTo(rect.left, y);
		dc.LineTo(rect.right, y);
	}
}

void CDlgCharItems::DrawGrids(CPaintDC & dc)
{
    CPen pen(PS_SOLID,1,RGB(0,200,100));
    CPen * pOld = dc.SelectObject(&pen);
	for (const auto & g : m_vGridView)
		if(g.IsGrid())
			::DrawGrid(dc, g.Rect, GRID_WIDTH, GRID_WIDTH);
		else
			::DrawGrid(dc, g.Rect);
	dc.SelectObject(pOld);
}

void CDlgCharItems::DrawItemXY(CPaintDC & dc, CPoint pos, const CItemView & view) const
{
	CBitmap bmp;
	bmp.LoadBitmap(view.nPicRes);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOld = memDC.SelectObject(&bmp);
	auto sz = view.ViewSize();
	dc.BitBlt(pos.x, pos.y, sz.cx,sz.cy, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
}

void CDlgCharItems::DrawAllItemsInGrid(CPaintDC & dc) const
{
	CRect selectedGrid(0, 0, 0, 0), selectedSocket(0, 0, 0, 0);
	for (size_t i = 0; i < m_vItemViews.size(); ++i) {
		auto & view = m_vItemViews[i];
		//���������ϣ���I, II��ʾ��ͬ��Ʒ������ʬ��
		if ((RIGHT_HAND == view.iPosition || LEFT_HAND == view.iPosition)
			&& view.iGridX != (m_bSecondHand ? 1 : 0))
			continue;
		if ((CORPSE_RIGHT_HAND == view.iPosition || CORPSE_LEFT_HAND == view.iPosition)
			&& view.iGridX != (m_bCorpseSecondHand ? 1 : 0))
			continue;
		if (::IsInMouse(view.iPosition))
			continue;
		if (::IsInSocket(view.iPosition))
			continue;
		auto pos = GetItemPositionXY(view);
		DrawItemXY(dc, pos, view);
		if (i == m_iSelectedItemIndex) {	//ѡ�е���Ʒ
			selectedGrid = CRect(pos, view.ViewSize());
			//Draw gems in sockets
			for (int g : view.vGemItems) {
				if (g < 0)
					continue;
				auto & gemView = m_vItemViews[g];
				pos = GetItemPositionXY(gemView);
				DrawItemXY(dc, pos, gemView);
				if (g == m_iSelectedSocketIndex)	//ѡ�еı�ʯ
					selectedSocket = CRect(pos, gemView.ViewSize());
			}
		}
	}
	// ����ѡ�еı�ʯ
	const int PEN_WIDTH = 2;
	if (selectedSocket.top) {
		CPen pen(PS_SOLID, PEN_WIDTH, RGB(255, 0, 0));
		CPen * pOld = dc.SelectObject(&pen);
		DrawGrid(dc, selectedSocket);
		dc.SelectObject(pOld);
	}
	// ����ѡ�е���Ʒ
	if (selectedGrid.top) {
		CPen pen(PS_SOLID, PEN_WIDTH, selectedSocket.top ? RGB(220, 50, 100) : RGB(255, 0, 0));
		CPen * pOld = dc.SelectObject(&pen);
		DrawGrid(dc, selectedGrid);
		dc.SelectObject(pOld);
	}
}

tuple<int, int, int> CDlgCharItems::HitTestPosition(CPoint pos, int col, int row) const {
	for (auto & g : m_vGridView)
		if (g.bEnabled && g.Rect.PtInRect(pos))
			return g.XYToPositionIndex(pos, m_bSecondHand, m_bCorpseSecondHand, col, row);
	return make_tuple(-1, -1, -1);
}

void CDlgCharItems::ShowItemInfoDlg(const CD2Item * pItem, int x){
    if(!m_bNotShowItemInfoDlg && pItem && (!m_pDlgItemInfo || pItem != m_pDlgItemInfo->GetItemPtr())){
        if(!m_pDlgItemInfo){
			m_pDlgItemInfo = make_unique<CDlgSuspend>(this, m_scTrasparent.GetPos());
            m_pDlgItemInfo->Create(CDlgSuspend::IDD,NULL);
        }
        m_pDlgItemInfo->GetItemInfo(pItem);
        CRect rect,rect1;
        m_pDlgItemInfo->GetWindowRect(&rect);
        GetWindowRect(&rect1);
		//Adjust Suspend Window position
		if (x < INFO_WINDOW_RIGHT - GRID_WIDTH * 2)
			rect1.left += INFO_WINDOW_RIGHT;
		else
			rect1.left += INFO_WINDOW_LEFT;
		m_pDlgItemInfo->MoveWindow(rect1.left, rect1.top, rect.Width(), rect.Height(), TRUE);
        m_pDlgItemInfo->ShowWindow(SW_SHOWNOACTIVATE); //��ʾ�Ի���
        m_pDlgItemInfo->Invalidate();
    }else if(!pItem && m_pDlgItemInfo)
        m_pDlgItemInfo.reset();
}

void CDlgCharItems::ReadItemProperty(const CD2Item & item) {

   // //m_sItemName = ::theApp.ItemName(item.MetaData().NameIndex);
   // if(m_bItemSocket = item.bSocketed)
   //     m_bBaseSocket = item.pItemInfo->pTpSpInfo->iSocket;
   // m_bEthereal = item.bEthereal;
   // //m_bItemInscribed = item.bPersonalized;
   // if(item.bEar){   //ear structure
   //     m_bItemLevel = item.pEar->iEarLevel;
   //     m_ItemOwner = item.pEar->sEarName;
   // }else{
   //     if(item.pItemInfo->IsTypeName("gld "))
   //         m_wItemQuantity = item.pItemInfo->pGold->wQuantity;
   //     if(!item.bSimple){
   //         m_bItemLevel = item.pItemInfo->pExtItemInfo->iDropLevel;
   //         if(item.bPersonalized)
   //             m_ItemOwner = &item.pItemInfo->pExtItemInfo->sPersonName[0];
   //         m_cbQuality.SetCurSel(item.pItemInfo->pExtItemInfo->iQuality - 1);
   //         if(item.MetaData().IsStacked)
   //             m_wItemQuantity = item.pItemInfo->pTpSpInfo->iQuantity;
   //         if(item.MetaData().HasDef)
   //             m_wItemDefence = item.pItemInfo->pTpSpInfo->iDefence - 10;
   //         if(item.MetaData().HasDur){
   //             m_wMaxDurability = item.pItemInfo->pTpSpInfo->iMaxDurability;
   //             if(!(m_bIndestructible = (m_wMaxDurability == 0)))
   //                 m_wCurDurability = item.pItemInfo->pTpSpInfo->iCurDur;
   //         }
			//for (const auto & p : item.pItemInfo->pTpSpInfo->stPropertyList.mProperty) {
			//	if (p.first == 194) {	//Adds X extra sockets to the item
			//		m_bExtSocket = BYTE(p.second);
			//	} else {
			//		int i = m_lcPropertyList.InsertItem(0, CSFormat(_T("%3d"), UINT(p.first)));	//���Դ���
			//		m_lcPropertyList.SetItemText(i, 1, ::theApp.PorpertyDescription(p.first, p.second)); //��������
			//	}
			//}
			////TODO: Set property lists
   //     }
   // }

   // UpdateData(FALSE);
}

void CDlgCharItems::ResetFoundry()
{
    ////m_sItemName = _T("");
    //m_bItemSocket = FALSE;
    //m_bBaseSocket = m_bExtSocket = 0;
    //m_bEthereal = FALSE;
    ////m_bItemInscribed = FALSE;
    //m_bItemLevel = 0;
    //m_ItemOwner = _T("");
    //m_wItemQuantity = 0;
    //m_cbQuality.SetCurSel(-1);
    //m_wItemDefence = 0;
    //m_wMaxDurability = m_wCurDurability = 0;
    //m_lcPropertyList.DeleteAllItems();

    //UpdateData(FALSE);
}

BOOL CDlgCharItems::GatherData(CD2S_Struct & character)
{
	// TODO:

    return TRUE;
}

void CDlgCharItems::ResetAll()
{
	m_vItemViews.clear();
	for (auto & grid : m_vGridView)
		grid.Reset();
	m_bSecondHand = m_bCorpseSecondHand = FALSE;
	m_iSelectedItemIndex = m_iSelectedSocketIndex = -1;
	m_pDlgItemInfo.reset();
	if (m_iPickedItemIndex >= 0) {
		::DestroyIcon(m_hCursor);
		m_hCursor = ::LoadCursor(0, IDC_ARROW);
		m_iPickedItemIndex = -1;
	}
	m_bHasCharacter = FALSE;
	Invalidate();
}

void CDlgCharItems::LoadText(void)
{
	int index = 0;
    for(auto & text : m_sText)
        text = ::theApp.CharItemsUI(index++);
    //m_cbQuality.ResetContent();
	//for(UINT i = 0;i < ::theApp.ItemQualityNameSize();++i)
	//	m_cbQuality.AddString(::theApp.ItemQualityName(i));
    //���������б�ı�������
    /*LVCOLUMN col;
    col.cchTextMax = 20;
    col.mask = LVCF_TEXT;
    col.pszText = (LPWSTR)::theApp.CharItemsUI(index++).GetString();
    m_lcPropertyList.SetColumn(0,&col);
    col.pszText = (LPWSTR)::theApp.CharItemsUI(index++).GetString();
    m_lcPropertyList.SetColumn(1,&col);*/

	UpdateData(FALSE);
}

// CDlgCharItems ��Ϣ�������

void CDlgCharItems::OnPaint()
{
    CPaintDC dc(this);
    DrawGrids(dc);
    DrawAllItemsInGrid(dc);	//�����������Ʒ
}

void CDlgCharItems::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_iPickedItemIndex < 0) {	//δ������Ʒ
		const CD2Item * item = 0;
		auto t = HitTestPosition(point);
		const int pos = get<0>(t), x = get<1>(t), y = get<2>(t);
		if (0 <= pos) {		//����Ч������
			auto & grid = m_vGridView[pos];
			int index = grid.ItemIndex(x, y);
			if (0 <= index) {	//����Ʒ
				ASSERT(index < int(m_vItemViews.size()));
				item = &m_vItemViews[index].Item;
			}
		}
		ShowItemInfoDlg(item, point.x);
	}
	m_pMouse = point;
	UpdateData(FALSE);
	CPropertyDialog::OnMouseMove(nFlags, point);
}

BOOL CDlgCharItems::PutItemInGrid(EPosition pos, int x, int y) {
	ASSERT(0 <= m_iPickedItemIndex && m_iPickedItemIndex < int(m_vItemViews.size()));
	ASSERT(pos < POSITION_END);
	auto & view = m_vItemViews[m_iPickedItemIndex];
	auto & grid = m_vGridView[pos];
	if (!grid.PutItem(m_iPickedItemIndex, x, y, view.iGridWidth, view.iGridHeight, view.iEquip))
		return FALSE;
	//���Է���
	view.iPosition = pos;
	view.iGridX = x;
	view.iGridY = y;
	::DestroyIcon(m_hCursor);
	m_hCursor = ::LoadCursor(0, IDC_ARROW);
	//û���޸�m_iPickedItemIndex
	return TRUE;
}

void CDlgCharItems::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_iPickedItemIndex < 0) {	//������Ʒ
		auto t = HitTestPosition(point);
		const int pos = get<0>(t), x = get<1>(t), y = get<2>(t);
		if (pos >= 0) {		//������Χ��
			auto & grid = m_vGridView[pos];
			int index = grid.ItemIndex(x, y);
			if (index >= 0) {	//��������Ʒ
				ASSERT(index < int(m_vItemViews.size()));
				if (grid.IsSockets()) {	//������Ƕ�ı�ʯ
					auto & gems = SelectedParentItemView().vGemItems;
					ASSERT(x < int(gems.size()));
					gems[x] = -1;
				}
				auto & view = m_vItemViews[index];
				m_iPickedItemIndex = index;
				m_hCursor = CreateAlphaCursor(view);  //�������Ϊ��ƷͼƬ
				grid.ItemIndex(-1, view.iGridX, view.iGridY, view.iGridWidth, view.iGridHeight);
				view.iPosition = IN_MOUSE;
				ShowItemInfoDlg(0, 0);
				Invalidate();
			}
		}
	} else {	//������Ʒ
		ASSERT(m_iPickedItemIndex < int(m_vItemViews.size()));
		auto & view = m_vItemViews[m_iPickedItemIndex];
		auto t = HitTestPosition(point, view.iGridWidth, view.iGridHeight);
		const int pos = get<0>(t), x = get<1>(t), y = get<2>(t);
		if (pos >= 0) {		//������Χ��
			auto & grid = m_vGridView[pos];
			if (grid.IsSockets()) {	//����Ʒ��Ƕ��ʯ
				if (0 <= m_iSelectedItemIndex	//��ѡ����Ʒ
						&& x < int(SelectedParentItemView().vGemItems.size())	//���㹻�Ŀ���
						&& PutItemInGrid(EPosition(pos), x, y)) {
					//��Ƕ�ɹ�
					SelectedParentItemView().vGemItems[x] = m_iPickedItemIndex;
					m_iPickedItemIndex = -1;
					Invalidate();
				}
			} else if (PutItemInGrid(EPosition(pos), x, y)) {	//���Է���
				if (m_iSelectedSocketIndex == m_iPickedItemIndex)	//����Ƕ�ı�ʯ�ٳ�����
					m_iSelectedSocketIndex = -1;
				m_iPickedItemIndex = -1;
				Invalidate();
			}
		}
	}
	CPropertyDialog::OnLButtonDown(nFlags, point);
}

void CDlgCharItems::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bClickOnItem = FALSE;
	if (m_iPickedItemIndex < 0) {	//δ������Ʒ
		auto t = HitTestPosition(point);
		const int pos = get<0>(t), x = get<1>(t), y = get<2>(t);
		if (pos >= 0) {		//������Χ��
			auto & grid = m_vGridView[pos];
			int index = grid.ItemIndex(x, y);
			if (index >= 0) {	//��������Ʒ
				m_bClickOnItem = TRUE;
				if (grid.IsSockets()) {	//����Ƕ�ı�ʯ
					if (index != m_iSelectedSocketIndex) {
						m_iSelectedSocketIndex = index;
						Invalidate();
					}
				} else if (index != m_iSelectedItemIndex || 0 <= m_iSelectedSocketIndex) {	//������Ʒ
					if (index != m_iSelectedItemIndex) {
						const auto & view = m_vItemViews[index];
						for (int i = 0; i < PositionToCol(IN_SOCKET); ++i)
							m_vGridView[IN_SOCKET].ItemIndex((i < int(view.vGemItems.size()) ? view.vGemItems[i] : -1), i, 0);
					}
					m_iSelectedItemIndex = index;
					m_iSelectedSocketIndex = -1;
					Invalidate();
				}
			}
		}
	}
    CPropertyDialog::OnRButtonUp(nFlags, point);
}

void CDlgCharItems::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
	if (m_bHasCharacter && m_iPickedItemIndex < 0) {	//δ������Ʒ
		/*	���������˵���
				Import
				Export
				---
				Copy
				Paste
				---
				Modify
				Remove
		*/
		CMenu menu;
		menu.CreatePopupMenu();
		ASSERT(::IsMenu(menu.m_hMenu));
		//Texts
		menu.AppendMenu(MF_STRING, ID_ITEM_IMPORT, ::theApp.CharItemPopupMenu(0));
		menu.AppendMenu(MF_STRING, ID_ITEM_EXPORT, ::theApp.CharItemPopupMenu(1));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_ITEM_COPY, ::theApp.CharItemPopupMenu(2));
		menu.AppendMenu(MF_STRING, ID_ITEM_PASTE, ::theApp.CharItemPopupMenu(3));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, ID_ITEM_MODIFY, ::theApp.CharItemPopupMenu(4));
		menu.AppendMenu(MF_STRING, ID_ITEM_REMOVE, ::theApp.CharItemPopupMenu(5));
		//Appearance
		menu.EnableMenuItem(ID_ITEM_IMPORT, (m_bClickOnItem ? MF_DISABLED : MF_ENABLED));
		menu.EnableMenuItem(ID_ITEM_EXPORT, (m_bClickOnItem ? MF_ENABLED : MF_DISABLED));
		menu.EnableMenuItem(ID_ITEM_COPY, (m_bClickOnItem ? MF_ENABLED : MF_DISABLED));
		menu.EnableMenuItem(ID_ITEM_PASTE, (0 <= m_iCopiedItemIndex ? MF_ENABLED : MF_DISABLED));
		menu.EnableMenuItem(ID_ITEM_MODIFY, (m_bClickOnItem ? MF_ENABLED : MF_DISABLED));
		menu.EnableMenuItem(ID_ITEM_REMOVE, (m_bClickOnItem ? MF_ENABLED : MF_DISABLED));

		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

BOOL CDlgCharItems::OnInitDialog()
{
    CPropertyDialog::OnInitDialog();
	/*m_lcPropertyList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lcPropertyList.InsertColumn(0, _T(""), LVCFMT_LEFT, 60);
	m_lcPropertyList.InsertColumn(1, _T(""), LVCFMT_LEFT, 225);*/
	m_scTrasparent.SetRange(0, 255);
	m_scTrasparent.SetPos(200);
    LoadText();
    return TRUE;
}

void CDlgCharItems::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CPropertyDialog::OnShowWindow(bShow, nStatus);
    //if(!bShow)			//����������£������������Ʒ���Դ���ʱ���������ڵ����
    //    ShowItemInfoDlg(0);
}

void CDlgCharItems::OnBnClickedCheck2()
{
    m_bNotShowItemInfoDlg = !m_bNotShowItemInfoDlg;
    UpdateData(TRUE);
}

void CDlgCharItems::OnChangeHand()
{
    m_bSecondHand = !m_bSecondHand;
    //����UI
	InvalidateRect(&m_vGridView[RIGHT_HAND].Rect);
	InvalidateRect(&m_vGridView[LEFT_HAND].Rect);
 }

void CDlgCharItems::OnChangeCorpseHand() {
	m_bCorpseSecondHand = !m_bCorpseSecondHand;
	//����UI
	InvalidateRect(&m_vGridView[CORPSE_RIGHT_HAND].Rect);
	InvalidateRect(&m_vGridView[CORPSE_LEFT_HAND].Rect);
}

//void CDlgCharItems::OnPrefixSuffix()
//{
//    std::vector<int> selIndex(10,-1);
//	auto view = SelectedItemView();
//    if(view){
//        auto & item = view->Item;
//        if(item.pItemInfo.exist() && item.pItemInfo->pExtItemInfo.exist()){
//            switch(m_cbQuality.GetCurSel() + 1){
//                case 1:     //low
//                    if(item.pItemInfo->pExtItemInfo->loQual.exist())
//                        selIndex[9] = item.pItemInfo->pExtItemInfo->loQual;
//                    break;
//                case 3:     //high
//                    if(item.pItemInfo->pExtItemInfo->hiQual.exist())
//                        selIndex[9] = item.pItemInfo->pExtItemInfo->hiQual;
//                    break;
//                case 4:     //magic
//                    if(item.pItemInfo->pExtItemInfo->wPrefix.exist())
//                        selIndex[2] = item.pItemInfo->pExtItemInfo->wPrefix;
//                    if(item.pItemInfo->pExtItemInfo->wSuffix.exist())
//                        selIndex[3] = item.pItemInfo->pExtItemInfo->wSuffix;
//                    break;
//                case 5:     //set
//                    break;
//                case 6:     //rare
//                    if(item.pItemInfo->pExtItemInfo->pRareName.exist()){
//                        selIndex[0] = item.pItemInfo->pExtItemInfo->pRareName->iName1;
//                        selIndex[1] = item.pItemInfo->pExtItemInfo->pRareName->iName2;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bPref1)
//                            selIndex[2] = item.pItemInfo->pExtItemInfo->pRareName->wPref1;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bSuff1)
//                            selIndex[3] = item.pItemInfo->pExtItemInfo->pRareName->wSuff1;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bPref2)
//                            selIndex[4] = item.pItemInfo->pExtItemInfo->pRareName->wPref2;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bSuff2)
//                            selIndex[5] = item.pItemInfo->pExtItemInfo->pRareName->wSuff2;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bPref3)
//                            selIndex[6] = item.pItemInfo->pExtItemInfo->pRareName->wPref3;
//                        if(item.pItemInfo->pExtItemInfo->pRareName->bSuff3)
//                            selIndex[7] = item.pItemInfo->pExtItemInfo->pRareName->wSuff3;
//                    }
//                    break;
//                case 7:     //unique
//                    if(item.pItemInfo->pExtItemInfo->wUniID.exist())
//                        selIndex[8] = item.pItemInfo->pExtItemInfo->wUniID;
//                    break;
//                case 8:     //crafted
//                    if(item.pItemInfo->pExtItemInfo->pCraftName.exist()){
//                        selIndex[0] = item.pItemInfo->pExtItemInfo->pCraftName->iName1;
//                        selIndex[1] = item.pItemInfo->pExtItemInfo->pCraftName->iName2;
//                        if(item.pItemInfo->pExtItemInfo->pCraftName->bPref1)
//                            selIndex[2] = item.pItemInfo->pExtItemInfo->pCraftName->wPref1;
//                        if(item.pItemInfo->pExtItemInfo->pCraftName->bSuff1)
//                            selIndex[3] = item.pItemInfo->pExtItemInfo->pCraftName->wSuff1;
//						if (item.pItemInfo->pExtItemInfo->pCraftName->bPref2)
//							selIndex[4] = item.pItemInfo->pExtItemInfo->pCraftName->wPref2;
//                        if(item.pItemInfo->pExtItemInfo->pCraftName->bSuff2)
//                            selIndex[5] = item.pItemInfo->pExtItemInfo->pCraftName->wSuff2;
//                        if(item.pItemInfo->pExtItemInfo->pCraftName->bPref3)
//                            selIndex[6] = item.pItemInfo->pExtItemInfo->pCraftName->wPref3;
//                        if(item.pItemInfo->pExtItemInfo->pCraftName->bSuff3)
//                            selIndex[7] = item.pItemInfo->pExtItemInfo->pCraftName->wSuff3;
//                    }
//                    break;
//                default:;
//            }
//        }
//    }
//    CDlgPrefixSuffix dlgPrefix(m_cbQuality.GetCurSel() + 1,&selIndex[0],this);
//    dlgPrefix.DoModal();
//}

void CDlgCharItems::OnChangeCorpse() {
	m_bHasCorpse = !m_bHasCorpse;
	for (int i = CORPSE_HEAD; i < CORPSE_END; ++i)
		m_vGridView[i].bEnabled = m_bHasCorpse;
}

void CDlgCharItems::OnChangeMercenary() {
	m_bHasMercenary = !m_bHasMercenary;
	for (int i = MERCENARY_HEAD; i < MERCENARY_END; ++i)
		m_vGridView[i].bEnabled = m_bHasMercenary;

}

static HCURSOR CreateCursorFromBitmap(int picIndex, CSize sz) {
	// Load bitmap
	CBitmap bmp;
	if (!bmp.LoadBitmap(picIndex))
		ASSERT(FALSE);
	// Create an empty mask bitmap.
	CBitmap monobmp;
	monobmp.CreateBitmap(sz.cx, sz.cy, 1, 1, NULL);
	// Icon header
	ICONINFO ii;
	ii.fIcon = FALSE;  // Change fIcon to TRUE to create an alpha icon
	ii.xHotspot = sz.cx >> 1;
	ii.yHotspot = sz.cy >> 1;
	ii.hbmMask = monobmp;
	ii.hbmColor = bmp;
	// Create the alpha cursor with the alpha DIB section, and return it.
	return ::CreateIconIndirect(&ii);
}

HCURSOR CDlgCharItems::CreateAlphaCursor(const CItemView & itemView) {
	return ::CreateCursorFromBitmap(itemView.nPicRes, itemView.ViewSize());
}

BOOL CDlgCharItems::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (nHitTest == HTCLIENT) {
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CPropertyDialog::OnSetCursor(pWnd, nHitTest, message);
}


void CDlgCharItems::OnItemImport() {
	// TODO: �ڴ���������������
	MessageBox(_T("Hi"));
}


void CDlgCharItems::OnItemExport() {
	// TODO: �ڴ���������������
}


void CDlgCharItems::OnItemCopy() {
	// TODO: �ڴ���������������
}


void CDlgCharItems::OnItemPaste() {
	// TODO: �ڴ���������������
}


void CDlgCharItems::OnItemModify() {
	// TODO: �ڴ���������������
}


void CDlgCharItems::OnItemRemove() {
	// TODO: �ڴ���������������
}
