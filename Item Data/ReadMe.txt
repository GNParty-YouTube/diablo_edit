��Ʒ�Ķ�����Ϣ��������
ID	DWORD��BYTE[4],��ƷΨһID,��"elx "
Pic	WORD,��Ʒ��Bitmap��Դ����Ե�һ��ͼƬ������ƫ��,
Name	WORD,��Ʒ��String�б��е���������
EqRange	BYTE,��������
	Equip	4 bits,��Ʒ�ɴ�����λ��,
		0	���ɴ���
		1	ͷ
		2	����
		3	����
		4	�������
		5	��ָ
		6	��
		7	Ь��
		8	����
		9	����������
	Range,4bits��Ʒ����ռ�������С,((col-1) << 2) + (row-1)

Add	BYTE,����,��������:
	bDef		1 bit,�з���ֵ
	bDur		1 bit,���;ö�
	bStack		1 bit,������
	bTome		1 bit,�ǻس�����ʶ��,��iTome(5 bits)��
	bMonster	1 bit,��wMonsterID(10 bits)��
	bCharm		1 bit,�ǻ����(�������ϼ����Ե�),��wCharm��
	bSpell		1 bit,��bSpellID��
	bUnique		1 bit,�Ƿ���Unique��Ʒ(��û��iQuality���iQualityֵΪ����ֵ�����)
Dmg1Min	BYTE,�����ĵ�������˺�
Dmg1Max	BYTE,�����ĵ�������˺�
Dmg2Min	BYTE,�����ĵ�������˺�
Dmg2Max	BYTE,�����ĵ�������˺�
	
	

�ļ���ʽ:
����		�ֽ���		˵��
ͷ��Ϣ
WORD		2		Majic,0x11DE
BYTE[4]		4		"ITEM",��Ʒ�ļ���־
DWORD		4		�ļ��ֽڳ���
DWORD		4		CRCУ���
WORD		2		��Ʒ����
��Ʒ1
STRUCT				��Ʒ1,�ṹ��ǰ��ʾ
��Ʒ2
...

ע��:
�ļ����Ҫ��CRC���м���,���Բ�����2�����½��ж���д