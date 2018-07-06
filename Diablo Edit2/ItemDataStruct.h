#pragma once

/*
�ṹ˵��:
ID			DWORD��BYTE[4],��ƷΨһID,��"elx "
Pic			WORD,��Ʒ��Bitmap��Դ����Ե�һ��ͼƬ������ƫ��,
Name		WORD,��Ʒ��String�б��е���������
EqRange		BYTE,��������
				Equip	4 bits,��Ʒ�ɴ�����λ��,
					0	���ɴ���
					1	ͷ
					2	����
					3	����
					4	����
					5	��ָ
					6	��
					7	Ь��
					8	����
					9	����������
				Range,4bits��Ʒ����ռ�������С,((col-1) << 2) + (row-1)
Add			BYTE,����,��������:
				bDef		1 bit,�з���ֵ
				bDur		1 bit,���;ö�
				bStack		1 bit,������
				bTome		1 bit,�ǻس�����ʶ��,��iTome(5 bits)��
				bMonster	1 bit,��wMonsterID(10 bits)��
				bCharm		1 bit,�ǻ����(�������ϼ����Ե�),��wCharm��
				bSpell		1 bit,��bSpellID��
				bUnique		1 bit,�Ƿ���Unique��Ʒ(��û��iQuality�������)
Dmg1Min	    BYTE,�����ĵ�������˺�
Dmg1Max	    BYTE,�����ĵ�������˺�
Dmg2Min	    BYTE,������˫������˺�
Dmg2Max	    BYTE,������˫������˺�
//*/
class CItemDataStruct
{
//fields
	union{
		DWORD	dwTypeID;
		BYTE	sTypeName[4];
	};
	WORD	wPic;
	WORD	wName;
	BYTE	bEqRange;
	BYTE	bAdd;
    BYTE    bDmg1Min,bDmg1Max;
    BYTE    bDmg2Min,bDmg2Max;
public:
	static const int SIZE = 14;
//members
	DWORD	TypeID() const{return dwTypeID;}
	WORD	PicIndex() const{return wPic;}
	WORD	NameIndex() const{return wName;}
	BYTE	Equip() const{return bEqRange >> 4;}
	BYTE	Range() const{return ((bEqRange & 0xC) << 2) + (bEqRange & 3) + 0x11;}
	BOOL	HasDef() const{return (bAdd & 0x80) != 0;}
	BOOL	HasDur() const{return (bAdd & 0x40) != 0;}
	BOOL	IsStacked() const{return (bAdd & 0x20) != 0;}
	BOOL	IsTome() const{return (bAdd & 0x10) != 0;}
	BOOL	HasMonsterID() const{return (bAdd & 8) != 0;}
	BOOL	IsCharm() const{return (bAdd & 4) != 0;}
	BOOL	HasSpellID() const{return (bAdd & 2) != 0;}
	BOOL	IsUnique() const{return (bAdd & 1) != 0;}
    UINT    Damage1Min() const{return bDmg1Min;}
    UINT    Damage1Max() const{return bDmg1Max;}
    UINT    Damage2Min() const{return bDmg2Min;}
    UINT    Damage2Max() const{return bDmg2Max;}
};
