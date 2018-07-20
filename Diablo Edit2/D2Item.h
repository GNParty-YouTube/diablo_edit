#pragma once

#include <vector>
#include <map>
#include "ItemDataStruct.h"
#include "MayExist.h"
#include "BinDataStream.h"

template<int N>
static bool IsSameType(const BYTE(&v1)[N], const char * v2) {
	return memcmp(v1, v2, N) == 0;
}

//Ear
struct CEar
{
	BYTE	iEarClass;		//bit 76-78
	BYTE	iEarLevel;		//bit 79-85
	BYTE	sEarName[16];	//bit 86-92,93-99,100-106,...,��0x00����
	void ReadData(CInBitsStream & bs);
	void WriteData(COutBitsStream & bs) const;
};

//Item Long Name
struct CLongName
{
	BYTE				iName1;		//8 bits,First Name
	BYTE				iName2;		//8 bits,Second Name
	BOOL				bPref1;		//1 bit,Prefix 1 flag
	CMayExist<WORD>		wPref1;		//11 bits,Prefix 1,if bPref1 == TRUE
	BOOL				bSuff1;		//1 bit,Suffix 1 flag
	CMayExist<WORD>		wSuff1;		//11 bits,Suffix 1,if bSuff1 == TRUE
	BOOL				bPref2;		//1 bit,Prefix 2 flag
	CMayExist<WORD>		wPref2;		//11 bits,Prefix 2,if bPref2 == TRUE
	BOOL				bSuff2;		//1 bit,Suffix 2 flag
	CMayExist<WORD>		wSuff2;		//11 bits,Suffix 2,if bSuff2 == TRUE
	BOOL				bPref3;		//1 bit,Prefix 3 flag
	CMayExist<WORD>		wPref3;		//11 bits,Prefix 3,if bPref3 == TRUE
	BOOL				bSuff3;		//1 bit,Suffix 3 flag
	CMayExist<WORD>		wSuff3;		//11 bits,Suffix 3,if bSuff3 == TRUE
	void ReadData(CInBitsStream & bs);
	void WriteData(COutBitsStream & bs) const;
};

//Extended Item Info
struct CExtItemInfo
{
	BYTE					nGems;			//bit 108-110, ����пף���Ƕ�ı�ʯ��
	DWORD					dwGUID;			//bit 111-142
	BYTE					iDropLevel;		//bit 143-149,drop level
	BYTE					iQuality;		/*4 bits
											1	low quality
											2	normal
											3	high quality(��ɫ)
											4	magically enhanced(ħ��,��ɫ)
											5	part of a set(��װ,��ɫ)
											6	rare(�ƽ�)
											7	unique(����)
											8	crafted(�ϳɣ���ɫ)
											//*/
	BOOL					bVarGfx;		//1 bit
	CMayExist<BYTE>			iVarGfx;		//3 bits,���bVarGfx == TRUE,�˽ṹ����,�ƺ���ʾ��Ʒ��ͼƬ����
	BOOL					bClass;			//1 bit,Class Info Flag
	CMayExist<WORD>			wClass;			//11 bits,Class Info,���bClass == TRUE,�˽ṹ����
//Low Quality
	CMayExist<BYTE>			loQual;			//3 bits,Low Quality sub-type,���iQuality == 1,�˽ṹ����
//Hi Quality
	CMayExist<BYTE>			hiQual;			//3 bits,Hi Quality sub-type,���iQuality == 3,�˽ṹ����
//Magic
	CMayExist<WORD>			wPrefix;		//11 bits,Magic Prefix,if iQuality == 4
	CMayExist<WORD>			wSuffix;		//11 bits,Magic Suffix,if iQuality == 4
//Set
	CMayExist<WORD>			wSetID;			//12 bits,Set ID,if iQuality == 5
//Rare
	CMayExist<CLongName>	pRareName;		//if iQuality == 6
//Unique
	CMayExist<WORD>			wUniID;			//12 bits,Unique ID,if iQuality == 7
//Craft
	CMayExist<CLongName>	pCraftName;		//if iQuality == 8
//Rune Word
	CMayExist<WORD>			wRune;			//16 bits,Rune word,if bRuneWord == TRUE;
//Personalized
	CMayExistArray<BYTE,16>	sPersonName;	//��sEarName��ͬ��ÿ�ַ�ռ7bits����0x00����,if bPersonalized == TRUE
//Tome
	CMayExist<BYTE>			iTome;			//5 bits,if sTypeName = "tbk " or "ibk "
//Monster ID
	CMayExist<WORD>			wMonsterID;		//10 bits,if sTypeName����������
//Charm
	CMayExist<WORD>			wCharm;			//12 bits,if sTypeName == "cm1" || "cm2" || "cm3"
//Spell ID
	CMayExist<BYTE>			bSpellID;		//5 bits,if sTypeName == "0sc"
	BOOL IsSet() const { return iQuality == 5; }
	int Gems() const { return nGems; }
	void ReadData(CInBitsStream & bs, BOOL bIsCharm, BOOL bRuneWord, BOOL bPersonalized, BOOL bIsTome, BOOL bHasMonsterID, BOOL bHasSpellID);
	void WriteData(COutBitsStream & bs, BOOL bIsCharm, BOOL bRuneWord, BOOL bPersonalized, BOOL bIsTome, BOOL bHasMonsterID, BOOL bHasSpellID) const;
};

//Gold Quantity
struct CGoldQuantity
{
	BOOL	bNotGold;		//1 bit
	WORD	wQuantity;		//12 bits,�ƽ�����
	void ReadData(CInBitsStream & bs);
	void WriteData(COutBitsStream & bs) const;
};

struct CPropertyList
{
	std::map<WORD, DWORD>	mProperty;		//�����б�ÿ�9 bits ID + VALUE)
	WORD					iEndFlag;		//9 bits, 0x1FF, ������־
	void ReadData(CInBitsStream & bs);
	void WriteData(COutBitsStream & bs) const;
};

//Type Specific info
struct CTypeSpecificInfo
{
	CMayExist<WORD>			iDefence;		//11 bits,ʵ�ʷ���ֵ+10,if bHasDef = TRUE
	CMayExist<WORD>			iMaxDurability;	//8 bits,if bNoDurability == FALSE
	CMayExist<WORD>			iCurDur;		//9 bits,if bNoDurability == FALSE && iMaxDur > 0
	CMayExist<BYTE>			iSocket;		//4 bits,Base Number of Sockets,������Ŀ,���滹�и�������Ŀ,if bSocketed = TRUE
	CMayExist<WORD>			iQuantity;		//9 bits,if bStacked == TRUE
	CMayExistArray<BOOL, 5>	aHasSetPropList;//5 bits,if iQuality == 5 
	//Rune Word Property
	CPropertyList			stPropertyList;		//�����б�
	CMayExist<CPropertyList> apSetProperty[5];	//��װ�����б�ÿ���б��Ƿ������(aHasSetPropList[i] == TRUE)����

	void ReadData(CInBitsStream & bs, BOOL bHasDef, BOOL bHasDur, BOOL bSocketed, BOOL bIsStacked, BOOL bIsSet, BOOL bRuneWord);
	void WriteData(COutBitsStream & bs, BOOL bHasDef, BOOL bHasDur, BOOL bSocketed, BOOL bIsStacked, BOOL bIsSet, BOOL bRuneWord) const;
};

//ItemInfo
struct CItemInfo
{
	union{
		BYTE						sTypeName[4];	//bit 76-83,84-91,92-99,100-107
		DWORD						dwTypeID;
	};
	CMayExist<CExtItemInfo>			pExtItemInfo;	//���bSimple == FALSE����˽ṹ����
	CMayExist<CGoldQuantity>		pGold;			//���sTypeName == "gld "����˽ṹ����
	BOOL							bHasRand;		//1 bit
	CMayExistArray<DWORD, 3>		pTmStFlag;		//���bHasRand == TRUE����˽ṹ����
	CMayExist<CTypeSpecificInfo>	pTpSpInfo;		//���bSimple == FALSE����˽ṹ����
	const CItemDataStruct * ReadData(CInBitsStream & bs, BOOL bSimple, BOOL bRuneWord, BOOL bPersonalized, BOOL bSocketed);
	void WriteData(COutBitsStream & bs, const CItemDataStruct & itemData, BOOL bSimple, BOOL bRuneWord, BOOL bPersonalized, BOOL bSocketed) const;
	BOOL IsNameValid() const;
	BOOL IsSet() const { return pExtItemInfo.IsValid() && pExtItemInfo.Value().IsSet(); }
	int Gems() const { return pExtItemInfo.IsValid() && pExtItemInfo.Value().Gems(); }
};

class CD2Item
{
//members
public:
	~CD2Item();
    //Quality()ֻ����ʾʱ,�������ֵ���ɫ��
	BYTE Quality() const{return !bEar && !bSimple ? pItemInfo->pExtItemInfo->iQuality : (pItemData->IsUnique ? 7 : 2);}
	void ReadData(CInBitsStream & bs);
	void WriteData(COutBitsStream & bs) const;
private:
	void findUnknownItem(CInBitsStream & bs);
public:
	std::vector<BYTE>		vItemData;	//�������ʶ����Ʒ,��ô��Ʒ�����ݽ���������
	const CItemDataStruct *	pItemData;	//��Ʒ�Ķ�������,��С,bHasDef,bNoDurability,bStacked,��.�������ʶ����Ʒ,pItemData = 0;
	//��Ʒ��Ϣ
	WORD	wMajic;				//0x4D4A,"JM"
	BOOL	bQuest;				//bit 16,�Ƿ�Ϊϵͳװ��
	BYTE	iUNKNOWN_01;		//bit 17-19
	BOOL	bIdentified;		//bit 20,�Ƿ��Ѿ���ʶ
	BYTE	iUNKNOWN_02;		//bit 21-23
	BOOL	bIllegalInventory;	//bit 24
	BYTE	iUNKNOWN_10;		//bit 25,26
	BOOL	bSocketed;			//bit 27,�Ƿ��п�
	BYTE	iUNKNOWN_03;		//bit 28,29
	BOOL	bBadEquipped;		//bit 30
	BOOL	iUNKNOWN_04;		//bit 31
	BOOL	bEar;				//bit 32
	BOOL	bNewbie;			//bit 33
	BYTE	iUNKNOWN_05;		//bit 34-36
	BOOL	bSimple;			//bit 37
	BOOL	bEthereal;			//bit 38
	BOOL	iUNKNOWN_06;		//bit 39,����1
	BOOL	bPersonalized;		//bit 40
	BOOL	iUNKNOWN_07;		//bit 41
	BOOL	bRuneWord;			//bit 42
	BYTE	iUNKNOWN_08;		//bit 43-47
	WORD	wVersion;			//bit 48-57
	BYTE	iLocation;			//bit 58-60,0 = grid 1 = equipped 2 = on belt 3 = ? 4 = in hand(has been picked up by the mouse) 5 = ? 6 = socket(glued into a socket) 7 = ?
	BYTE	iPosition;			//bit 61-64,Body position, 01 = head(ñ��), 02 = neck��������, 03 = tors���·���, 04 = rarm�����֣��������ܣ�, 05 = larm�����֣��������ܣ�, 06 = lrin������ָ����ָ��, 07 = rrin������ָ����ָ��, 08 = belt��������, 09 = feet��Ь�ӣ�, 0a = glov�����ף�, 0b = ralt����II�֣�Expansion Set only��, 0c = lalt����II�֣�Expansion Set only��
	BYTE	iColumn;			//bit 65-68
	BYTE	iRow;				//bit 69-72
	BYTE	iStoredIn;			//bit 73-75,0 = equip/belt 1 = inventory 2 = ? 3 = ? 4 = cube 5 = stash
	CMayExist<CEar>			pEar;			//���bEar == TRUE����˽ṹ����
	CMayExist<CItemInfo>	pItemInfo;		//���bEar == FALSE����˽ṹ����
	std::vector<CD2Item *>	aGemItems;		//����пף���Ƕ�ڿ����װ��
	//Functions
	BOOL IsSet() const { return pItemInfo.IsValid() && pItemInfo.Value().IsSet(); }
	BOOL Gems() const { return pItemInfo.IsValid() && pItemInfo.Value().Gems(); }
};
