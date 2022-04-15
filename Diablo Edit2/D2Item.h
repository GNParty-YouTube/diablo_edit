#pragma once

#include "MetaData.h"
#include "MayExist.h"
#include "BinDataStream.h"

#include <vector>

const int MAX_SOCKETS = 7;	//��Ʒ������������6�����ܻᵼ����Ϸ����

//��鲢������������
BOOL SetCharName(BYTE (&dest)[16], const CString & src);

//Ear
struct CEar
{
	BYTE	iEarClass = 0;	//3 bits
	BYTE	iEarLevel = 1;	//7 bits
	BYTE	sEarName[16];	//7 bit * Count, ��0x00����
	//Functions:
	explicit CEar(const char * name = 0);
	void ReadData(CInBitsStream & bs, BOOL isPtr24);
	void WriteData(COutBitsStream & bs, BOOL isPtr24) const;

};

//Item Long Name
struct CLongName
{
	BYTE				iName1;		//8 bits,First Name
	BYTE				iName2;		//8 bits,Second Name
	BOOL				bPref1;		//1 bit,Prefix 1 flag
	MayExist<WORD>		wPref1;		//11 bits,Prefix 1,if bPref1 == TRUE
	BOOL				bSuff1;		//1 bit,Suffix 1 flag
	MayExist<WORD>		wSuff1;		//11 bits,Suffix 1,if bSuff1 == TRUE
	BOOL				bPref2;		//1 bit,Prefix 2 flag
	MayExist<WORD>		wPref2;		//11 bits,Prefix 2,if bPref2 == TRUE
	BOOL				bSuff2;		//1 bit,Suffix 2 flag
	MayExist<WORD>		wSuff2;		//11 bits,Suffix 2,if bSuff2 == TRUE
	BOOL				bPref3;		//1 bit,Prefix 3 flag
	MayExist<WORD>		wPref3;		//11 bits,Prefix 3,if bPref3 == TRUE
	BOOL				bSuff3;		//1 bit,Suffix 3 flag
	MayExist<WORD>		wSuff3;		//11 bits,Suffix 3,if bSuff3 == TRUE
};

//Gold Quantity
struct CGoldQuantity
{
	BOOL	bNotGold = FALSE;	//1 bit
	WORD	wQuantity = 0;		//12 bits,�ƽ�����
};

struct CPropertyList
{
	std::vector<std::pair<WORD, DWORD>> mProperty;	//�����б�ÿ�9 bits ID + VALUE)
	WORD					iEndFlag;				//9 bits, 0x1FF, ������־
	//Functons:
	int ExtSockets() const;			//�����б���Ķ������
	BOOL IsIndestructible() const;	//�����б���Ĳ����ƻ�����
};

//Extended Item Info
struct CExtItemInfo
{
	BYTE					nGems = 0;		//bit 108-110, ����пף���Ƕ�ı�ʯ��
	DWORD					dwGUID;			//bit 111-142
	BYTE					iDropLevel = 99;//bit 143-149,drop level
	BYTE					iQuality = 2;	/*4 bits
												1	low quality
												2	normal
												3	high quality(��ɫ)
												4	magically enhanced(ħ��,��ɫ)
												5	part of a set(��װ,��ɫ)
												6	rare(�ƽ�)
												7	unique(����)
												8	crafted(�ϳɣ���ɫ) */
	BOOL					bVarGfx = FALSE;//1 bit
	MayExist<BYTE>			iVarGfx;		//3 bits,���bVarGfx == TRUE,�˽ṹ����,�ƺ���ʾ��Ʒ��ͼƬ����
	BOOL					bClass = FALSE;	//1 bit,Class Info Flag
	MayExist<WORD>			wClass;			//11 bits,Class Info,���bClass == TRUE,�˽ṹ����
	//Low Quality
	MayExist<BYTE>			loQual;			//3 bits,Low Quality sub-type,���iQuality == 1,�˽ṹ����
	//Hi Quality
	MayExist<BYTE>			hiQual;			//3 bits,Hi Quality sub-type,���iQuality == 3,�˽ṹ����
	//Magic
	MayExist<WORD>			wPrefix;		//11 bits,Magic Prefix,if iQuality == 4
	MayExist<WORD>			wSuffix;		//11 bits,Magic Suffix,if iQuality == 4
	//Set
	MayExist<WORD>			wSetID;			//12 bits,Set ID,if iQuality == 5
	//Rare
	MayExist<CLongName>		pRareName;		//if iQuality == 6
	//Unique
	MayExist<WORD>			wUniID;			//12 bits,Unique ID,if iQuality == 7
	//Craft
	MayExist<CLongName>		pCraftName;		//if iQuality == 8
	//Rune Word
	MayExist<WORD>			wRune;			//16 bits,Rune word,if bRuneWord == TRUE;
	//Personalized
	MayExist<BYTE, 16>		sPersonName;	//��sEarName��ͬ��ÿ�ַ�ռ7bits����0x00����,if bPersonalized == TRUE
	//Monster ID
	MayExist<WORD>			wMonsterID;		//10 bits,if sTypeName����������
	//Charm
	MayExist<WORD>			wCharm;			//12 bits,if iQuality == 2 && IsCharm == TRUE
	//Spell ID
	MayExist<BYTE>			iSpellID;		//5 bits,if SpellId > 0
	//Functions:
	explicit CExtItemInfo(const CItemMetaData * meta = 0);
	BOOL IsSet() const { return iQuality == 5; }
	int RuneWordId() const { ASSERT(wRune.exist()); return (wRune & 0xFFF); }
	int Gems() const { return nGems; }
};

//Type Specific info
struct CTypeSpecificInfo
{
	MayExist<WORD>			iDefence;		//11 bits,ʵ�ʷ���ֵ+10,if bHasDef = TRUE
	MayExist<WORD>			iMaxDurability;	//8 bits,if bNoDurability == FALSE
	MayExist<WORD>			iCurDur;		//9 bits,if iMaxDur > 0
	MayExist<BYTE>			iSocket;		//4 bits,��������(0-6),�����б�������и��ӿ���(����������6),if bSocketed = TRUE
	MayExist<WORD>			iQuantity;		//9 bits,if bStacked == TRUE
	MayExist<BOOL, 5>		aHasSetPropList;//5 bits,if iQuality == 5 
	//Rune Word Property
	CPropertyList			stPropertyList;			//�����б�
	MayExist<CPropertyList> apSetProperty[5];		//��װ�����б�ÿ���б��Ƿ������(aHasSetPropList[i] == TRUE)����
	MayExist<CPropertyList>	stRuneWordPropertyList;	//����֮�������б�if bRuneWord == TRUE
	//Functions:
	explicit CTypeSpecificInfo(const CItemMetaData * meta = 0);
	std::pair<int, int> Sockets() const;	//return: {base sockets, ext sockets}
	int TotalSockets() const { auto s = Sockets(); return s.first + s.second; }
	int GetDefence() const { ASSERT(iDefence.exist()); return iDefence - 10; }
	void SetDefence(int def) { iDefence.ensure() = def + 10; }
	BOOL IsIndestructible() const;
};

//ItemInfo
struct CItemInfo
{
	union{
		BYTE						sTypeName[4];	//bit 76-83,84-91,92-99,100-107
		DWORD						dwTypeID;
	};
	MayExist<CExtItemInfo>			pExtItemInfo;	//���bSimple == FALSE����˽ṹ����
	MayExist<CGoldQuantity>			pGold;			//���sTypeName == "gld "����˽ṹ����
	BOOL							bHasRand = FALSE;//1 bit
	MayExist<DWORD, 3>				pTmStFlag;		//���bHasRand == TRUE����˽ṹ����
	MayExist<CTypeSpecificInfo>		pTpSpInfo;		//���bSimple == FALSE����˽ṹ����
	//Functions:
	explicit CItemInfo(const CItemMetaData * meta = 0);
	const CItemMetaData * ReadData(CInBitsStream & bs, BOOL bSimple, BOOL bRuneWord, BOOL bPersonalized, BOOL bSocketed, BOOL isD2R, BOOL isPtr24);
	void WriteData(COutBitsStream & bs, const CItemMetaData & itemData, BOOL bSimple, BOOL bRuneWord, BOOL bPersonalized, BOOL bSocketed, BOOL isD2R, BOOL isPtr24) const;
	BOOL IsNameValid() const;
	BOOL IsSet() const { return pExtItemInfo.exist() && pExtItemInfo->IsSet(); }
	BOOL IsGold() const { return ::memcmp(sTypeName, "gld ", sizeof sTypeName) == 0; }
	BOOL IsBox() const { return ::memcmp(sTypeName, "box ", sizeof sTypeName) == 0; }			//�Ƿ�����Ͽ�����
	int RuneWordId() const { ASSERT(pExtItemInfo.exist()); return pExtItemInfo->RuneWordId(); }
	int Gems() const { return (pExtItemInfo.exist() ? pExtItemInfo->Gems() : 0); }
	int Sockets() const { return (pTpSpInfo.exist() ? pTpSpInfo->TotalSockets() : 0); }
};

struct CD2Item
{
	WORD	wMajic = 0x4D4A;		//0x4D4A,"JM"
	BOOL	bQuest = FALSE;			//bit 16,�Ƿ�Ϊϵͳװ��(���̵���ģ�)
	BYTE	iUNKNOWN_01 = 0;		//bit 17-19
	BOOL	bIdentified = TRUE;		//bit 20,�Ƿ��Ѿ���ʶ
	BYTE	iUNKNOWN_02 = 0;		//bit 21-23
	BOOL	bDisabled = FALSE;		//bit 24����Ʒ�����á������;öȱ��0�ˣ����������;öȲ�������Ҫ�������bit
	BYTE	iUNKNOWN_10 = 0;		//bit 25,26
	BOOL	bSocketed = FALSE;		//bit 27,�Ƿ��п�
	BYTE	iUNKNOWN_03 = 0;		//bit 28,29
	BOOL	bBadEquipped = FALSE;	//bit 30
	BOOL	bUNKNOWN_04 = FALSE;	//bit 31
	BOOL	bEar = FALSE;			//bit 32
	BOOL	bNewbie = FALSE;		//bit 33
	BYTE	iUNKNOWN_05 = 0;		//bit 34-36
	BOOL	bSimple = TRUE;			//bit 37
	BOOL	bEthereal = FALSE;		//bit 38
	BOOL	bUNKNOWN_06 = TRUE;		//bit 39,����1
	BOOL	bPersonalized = FALSE;	//bit 40
	BOOL	bUNKNOWN_07 = FALSE;	//bit 41
	BOOL	bRuneWord = FALSE;		//bit 42
	BYTE	iUNKNOWN_08 = 0;		//bit 43-47
	WORD	wVersion = 101;			//bit 48-57
	BYTE	iUNKNOWN_09 = 0xA0;		//bit 43-50 (D2R)
	BYTE	iLocation = 0;			//bit 58-60,0 = grid, 1 = equipped, 2 = on belt, 3 = ?, 4 = in hand(has been picked up by the mouse), 5 = ?, 6 = socket(glued into a socket), 7 = ?
	BYTE	iPosition = 0;			/*bit 61-64,Body position,
										01 = head(ñ��)
										02 = neck��������
										03 = tors���·���
										04 = rarm�����֣��������ܣ�
										05 = larm�����֣��������ܣ�
										06 = lrin������ָ����ָ��
										07 = rrin������ָ����ָ��
										08 = belt��������
										09 = feet��Ь�ӣ�
										0a = glov�����ף�
										0b = ralt����II�֣�Expansion Set only��
										0c = lalt����II�֣�Expansion Set only��*/
	BYTE	iColumn = 0;			//bit 65-68
	BYTE	iRow = 0;				//bit 69-72
	BYTE	iStoredIn = 1;			//bit 73-75,0 = equip/belt, 1 = inventory, 2 = ?, 3 = ?, 4 = cube, 5 = stash
	MayExist<CEar>			pEar;		//���bEar == TRUE����˽ṹ����
	MayExist<CItemInfo>		pItemInfo;	//���bEar == FALSE����˽ṹ����
	std::vector<CD2Item>	aGemItems;	//����пף���Ƕ�ڿ����װ��
	//Functions
	explicit CD2Item(DWORD type = 0);
	const CItemMetaData & MetaData() const { return *pItemData; }
	BYTE Quality() const{return !bEar && !bSimple ? pItemInfo->pExtItemInfo->iQuality : (pItemData->IsUnique ? 7 : 2);}
	BOOL IsSet() const { return pItemInfo.exist() && pItemInfo->IsSet(); }
	BOOL IsRuneWord() const { return bRuneWord; }
	BOOL IsEditable() const { return TRUE; }
	int RuneWordId() const { ASSERT(IsRuneWord() && pItemInfo.exist()); return pItemInfo->RuneWordId(); }
	int Gems() const { return (pItemInfo.exist() ? pItemInfo->Gems() : 0); }
	int Sockets() const { return (bSocketed && pItemInfo.exist() ? pItemInfo->Sockets() : 0); }	//��Ʒ�Ŀ����ܺͣ������������ӵĿף�
	CString ItemName() const;
	int GemIndexMax() const;	//��Ƕ��ʯλ���������ֵ��û�з���-1
	BOOL IsBox() const { return pItemInfo.exist() && pItemInfo->IsBox(); }	//�Ƿ�����Ͽ�����
	BOOL HasPropertyList() const { return pItemInfo.exist() && pItemInfo->pTpSpInfo.exist(); }
	void ReadData(CInBitsStream & bs, BOOL isD2R, BOOL isPtr24);
	void WriteData(COutBitsStream & bs, BOOL isD2R, BOOL isPtr24) const;
	BOOL ReadFile(CFile & file);
	void WriteFile(CFile & file) const;
private:
	std::vector<BYTE>		vUnknownItem;	//�������ʶ����Ʒ,��ô��Ʒ�����ݽ���������
	const CItemMetaData *	pItemData;		//��Ʒ�Ķ�������,��С,bHasDef,bNoDurability,bStacked,��.�������ʶ����Ʒ,pItemData = 0;
};

struct CItemList
{
	//������Ʒ��Ϣ
	WORD		wMajic;				//0x4D4A,"JM"
	//WORD		nItems;				//��Ʒ��Ŀ
	std::vector<CD2Item> vItems;	//������Ʒ����������Ƕ�ڿ����
	WORD		wEndMajic;			//0x4D4A,"JM"
	//Functions:
	void SwapItems(CItemList & list) { vItems.swap(list.vItems); }
	void Reset() { vItems.clear(); }
	void ReadData(CInBitsStream & bs, BOOL isD2R, BOOL isPtr24);
	void WriteData(COutBitsStream & bs, BOOL isD2R, BOOL isPtr24) const;
};
