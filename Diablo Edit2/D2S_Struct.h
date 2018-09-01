#pragma once

#include "D2Item.h"

//���������Ϣ
struct CQuestInfoData
{
	WORD	wIntroduced1;		//have been introduced (by Warriv) to Act I
	WORD	wActI[6];			//Act I, Bit 0 indicates the quest is complete,����Ҳһ��
	WORD	wTraval1;			//this gets set to a non-zero value after you travel from Act I to Act II.
	WORD	wIntroduced2;		//have been introduced (by Jerhyn) to Act II
	WORD	wActII[6];			//Act II
	WORD	wTraval2;			//this gets set to a non-zero value after you travel from Act II to Act III.
	WORD	wIntroduced3;		//have been introduced (by Hratli) to Act III
	WORD	wActIII[6];			//Act III
	WORD	wTraval3;			//this gets set to a non-zero value after you travel from Act III to Act IV.
	WORD	wIntroduced4;		//have been introduced to Act IV
	WORD	wActIV[3];			//Act IV
	WORD	wTraval4;
	WORD	unkown1[3];
	WORD	wIntroduced5;		//this was set to 1 after completing Terror's End and talking to Cain in act IV
	WORD	unkown2[2];
	WORD	wActV[6];			//Act V
	WORD	unkown3[7];
};

struct CQuestInfo
{
	DWORD	dwMajic;			//0x216F6F57
	DWORD	dwActs;				//�ƺ�����6
	WORD	wSize;				//Quest Info�ṹ���ܳ��ȣ�0x12A(=298=4+4+2+288)
	CQuestInfoData	QIData[3];
	//Functions:
	void Reset() {}
};

// Сվ��Ϣ
struct CWaypointData
{
	WORD	unkown;				//0x102;
	BYTE	Waypoints[5];
	BYTE	unkown2[17];		//ȫ0
};

struct CWaypoints
{
	WORD	wMajic;				//0x5357
	DWORD	unkown;
	WORD	wSize;				//0x50
	CWaypointData	wp[3];
	//Functions:
	void Reset() {}
};

//��������
struct CPlayerStats
{
	static const int ARRAY_SIZE = 0x10;
	WORD	wMajic;					//0x6667
	DWORD	m_adwValue[ARRAY_SIZE];	/*����Ϊ����Value�ĺ��壺
										0: 9 bits, + 10 bits Strength
										1: 9 bits, + 10 bits Energy
										2: 9 bits, + 10 bits Dexterity
										3: 9 bits, + 10 bits Vitality
										4: 9 bits, + 10 bits Stat Points Remaining
										5: 9 bits, + 8 bits Skill Choices Remaining
										6: 9 bits, + 21 bits Current Life (/256)
										7: 9 bits, + 21 bits Maximum Life (/256)
										8: 9 bits, + 21 bits Current Mana (/256)
										9: 9 bits, + 21 bits Maximum Mana (/256)
										A: 9 bits, + 21 bits Current Stamina (/256)
										B: 9 bits, + 21 bits Maximum Stamina (/256)
										C: 9 bits, + 7 bits Level
										D: 9 bits, + 32 bits Experience
										E: 9 bits, + 25 bits Gold on Person
										F: 9 bits, + 25 bits Gold in Stash */
	WORD iEnd;						//0x1FF: 9 bits, ����
	//Functions:
	void Reset() {}
};

//���＼��
struct CCharSkills
{
	WORD	wMagic;				//0x6669
	BYTE	bSkillLevel[30];	//���ܵȼ�
	//Functions:
	void Reset() {}
};

//ʬ������
struct CCorpseData
{
	BYTE		unknown[12];
	CItemList	stItems;	//ʬ�����ϵ�װ���б�
};

//ʬ��
struct CCorpse
{
	WORD		wMagic;					//0x4D4A
	WORD		wCount;					//ʬ�����, always 0 or 1
	MayExist<CCorpseData> pCorpseData;	//ʬ�������, if wCount == 1
	//Function:
	BOOL HasCorpse() const { return wCount > 0 && pCorpseData.exist(); }
	void Reset() { pCorpseData.reset(); }
};

//��Ӷ��
struct CMercenary
{
	WORD				wMagic;		//0x666A, "jf"
	MayExist<CItemList>	stItems;	//��Ӷ����װ���б�, if wMercName != 0
	//Function:
	void Reset() { stItems.reset(); }
};

//����ʯħ
struct CGolem
{
	WORD	wMagic;				//0x666B, "kf"
	BYTE	bHasGolem;
	MayExist<CD2Item>	pItem;	//�ٻ�����ʯħ����Ʒ, if bHasGolem != 0
	//Function:
	void Reset() { bHasGolem = FALSE; pItem.reset(); }
};

struct CD2S_Struct
{
//members
	void ReadData(CInBitsStream & bs);
	void ReadFile(const CString & path);
	void WriteFile(const CString & path) const;
	BOOL HasCorpse() const { return stCorpse.HasCorpse(); }
	BOOL HasMercenary() const { return wMercName > 0; }
	void Reset();
private:
	BOOL WriteData(COutBitsStream & bs) const;
public:
	//������Ϣ
	DWORD	dwMajic;			//0xAA55AA55
	DWORD	dwVersion;			//�ļ��汾
								//����1.10��0x60
								//����1.09��0x5C
	DWORD	dwSize;				//�ļ���Сbytes
	DWORD	dwCRC;				//����У��
	DWORD	dwWeaponSet;		//0�Ƿ���?
	BYTE	Name[16];			//��������,ռ��16�ַ�
	BYTE	charType;			//��������,Ladder ,Expansion ,Hardcore��
	BYTE	charTitle;			//�����ν
								//0:	��
								//5:	Slayer
								//0xA:	Champion
								//0xF:	Matriarch��Patriarch
	WORD	unkown1;			//0
	BYTE	charClass;			//�����ɫ
								//����ѷAma:			0x00
								//��ʦSorceress:		0x01
								//���鷨ʦNecromancer:	0x02
								//ʥ��ʿPaladin:		0x03
								//Ұ����Barbarian:		0x04
								//��Ŭ��Druid:			0x05
								//�̿�Assassin:			0x06
	WORD	unkown2;			//0x101E
	BYTE	charLevel;			//����ȼ�
	DWORD	unkown3;			//0
	DWORD	dwTime;				//ʱ���
	DWORD	unkown4;			//0xFFFFFFFF
	DWORD	dwSkillKey[16];		//���ܿ�ݼ�,����16
	DWORD	dwLeftSkill1;		//����ǰ��߼���
	DWORD	dwRightSkill1;		//����ǰ�ұ߼���
	DWORD	dwLeftSkill2;		//���ֺ���߼���
	DWORD	dwRightSkill2;		//���ֺ��ұ߼���
	BYTE	outfit[16];			//����16
	BYTE	colors[16];			//����16
	BYTE	Town[3];			//���ͣ���ĳ���
	DWORD	dwMapSeed;			//��ͼ���������
	WORD	unkown5;			//0
	BYTE	bMercDead;			//��Ӷ��������־
	BYTE	unkown6;			//0
	DWORD	dwMercControl;		//��Ӷ��Control seed
	WORD	wMercName;			//��Ӷ����������
	WORD	wMercType;			//��Ӷ������
	DWORD	dwMercExp;			//��Ӷ������
	BYTE	unkown7[0x90];		//ȫ0,����0x90

	CQuestInfo		QuestInfo;		//���������Ϣ,�ܳ�0x12A
	CWaypoints		Waypoints;		//Сվ��Ϣ
	BYTE			NPC[0x34];;		//��NPC�Ľ�̸��Ϣ
	CPlayerStats	PlayerStats;	//����״̬��Ϣ
	CCharSkills		Skills;			//���＼����Ϣ
	CItemList		ItemList;		//��Ʒ�б�
	CCorpse			stCorpse;		//ʬ��
	CMercenary		stMercenary;	//��Ӷ��
	CGolem			stGolem;		//Golem
};
