#pragma once

#include "DataStructs.h"
#include "ItemList.h"

struct CD2S_Struct
{
//members
	BOOL ReadFile(CFile & cf);
	void WriteFile(CFile & cf) const;
private:
	BOOL ReadData(CInBitsStream & bs);
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
	WORD			wCorpses;		//ʬ����Ŀ

	std::vector<BYTE>	vLeftData;		//ʣ�µ�����
};
