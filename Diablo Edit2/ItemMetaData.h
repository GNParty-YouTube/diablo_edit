#pragma once

#include <string>

//��Ʒ����Ԫ����
struct CItemMetaData
{
	std::string name;		//��Ʒ���֣�ΨһID��, ��"elx "��DEBUG��
	WORD PicIndex = 0;		//��Ʒ��Bitmap��Դ����Ե�һ��ͼƬ������ƫ��
	WORD NameIndex = 0;		//��Ʒ��ItemName�б��е���������
	BYTE Equip = 0;			//4 bits,��Ʒ�ɴ�����λ��,
							//	0	���ɴ���
							//	1	ͷ
							//	2	����
							//	3	����
							//	4	����
							//	5	��ָ
							//	6	��
							//	7	Ь��
							//	8	����
							//	9	����������
	BYTE Range = 0;			//4 bits��Ʒ����ռ�������С
	BOOL HasDef = FALSE;	//�з���ֵ
	BOOL HasDur = FALSE;	//���;ö�
	BOOL IsStacked = FALSE;	//������
	BOOL IsTome = FALSE;	//�ǻس�����ʶ��,��iTome(5 bits)��
	BOOL HasMonsterID = FALSE;	//��wMonsterID(10 bits)��
	BOOL IsCharm = FALSE;	//�ǻ����(�������ϼ����Ե�),��wCharm��
	BOOL HasSpellID = FALSE;//��bSpellID��
	BOOL IsUnique = FALSE;	//�Ƿ���Unique��Ʒ(��û��iQuality�������)
	UINT Damage1Min = 0;	//�����ĵ�������˺�
	UINT Damage1Max = 0;	//�����ĵ�������˺�
	UINT Damage2Min = 0;	//������˫������˺�
	UINT Damage2Max = 0;	//������˫������˺�
};
