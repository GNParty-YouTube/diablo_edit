
// Diablo Edit2.h: Diablo Edit2 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include <vector>
#include <unordered_map>
#include "ItemDataStruct.h"

struct CProperty
{
	int bits, bias;
	CProperty(int b1 = 0, int b2 = 0) :bits(b1), bias(b2) {}
};
// CDiabloEdit2App:
// 有关此类的实现，请参阅 Diablo Edit2.cpp
//

class CDiabloEdit2App : public CWinApp
{
public:
	CDiabloEdit2App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
// 自定义成员
public:
	INT m_nLangIndex,m_nLangCount;							//语言种类索引和总数
	std::vector<std::vector<CString> > m_saLanguage;		//语言数据
	INT m_nItemNameBase,m_nPropertyName;					//物品名字和属性名字在m_saLanguage中的起始索引
    INT m_nMonsterNameBase;                                 //怪物名字在m_saLanguage中的起始索引
	INT m_nMagicPrefixBase,m_nMagicSuffixBase;				//魔法前后缀名字在m_saLanguage中的起始索引
    INT m_nRareCraftedNameBase;                             //Rare,Crafted名字在m_saLanguage中的起始索引
    INT m_nUniqueNameBase;                                  //Unique装备名字在m_saLanguage中的起始索引
	std::unordered_map<DWORD, CItemDataStruct>	m_mapItemData;	//物品额外信息,以TypeID作为关键字
	std::vector<CProperty> m_vProperty;                     //属性数据
	CString m_sAppPath;										//程序运行的目录
private:
	// 自定义函数
	BOOL ReadLangRes(void);		//从语言资源文件里读取语言数据
	BOOL ReadItemRes(void);		//从物品资源文件里读取额外物品数据
	BOOL ReadPropRes(void);		//从属性资源文件里读取属性数据
public:
	CString PorpertyDescription(WORD id,DWORD value) const;             //根据属性id和参数value,得到属性的描述字符串
    CString SkillName(UINT skill_id,UINT * class_id = 0) const;         //根据skill_id得到技能名字,并得到技能的角色id
    //内联
    INT UniqueNameCount() const{return INT(m_saLanguage[m_nLangIndex].size() - m_nUniqueNameBase - 1);}
    INT RareCraftedNameCount() const{return m_nUniqueNameBase - m_nRareCraftedNameBase - 1;}
    INT MagicSuffixCount() const{return m_nRareCraftedNameBase - m_nMagicSuffixBase - 1;}
    INT MagicPrefixCount() const{return m_nMagicSuffixBase - m_nMagicPrefixBase - 1;}
    INT MonsterNameCount() const{return m_nMagicPrefixBase - m_nMonsterNameBase - 1;}
    INT PropertyNameCount() const{return m_nMonsterNameBase - m_nPropertyName - 1;}
    INT ItemNameCount() const{return m_nPropertyName - m_nItemNameBase - 1;}
	const CString & LangTitle(UINT index) const{
        ASSERT(index < m_saLanguage.size());
		return m_saLanguage[index][0];
	}
	const CString & String(UINT index) const{
        ASSERT(index < m_saLanguage[m_nLangIndex].size());
		return  m_saLanguage[m_nLangIndex][index];
	}
	const CString & ItemName(UINT index) const{
        ASSERT(m_nItemNameBase + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nItemNameBase + index);
	}
	const CString & PropertyName(UINT index) const{
        ASSERT(m_nPropertyName + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nPropertyName + index);
	}
    const CString & MonsterName(UINT index) const{
        ASSERT(m_nMonsterNameBase + index < m_saLanguage[m_nLangIndex].size());
        return index <= 703 ? String(m_nMonsterNameBase + index) : String(15);
    }
    const CString & MagicPrefix(UINT index) const{
        ASSERT(m_nMagicPrefixBase + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nMagicPrefixBase + index);
    }
    const CString & MagicSuffix(UINT index) const{
        ASSERT(m_nMagicSuffixBase + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nMagicSuffixBase + index);
    }
    const CString & RareCraftedName(UINT index) const{
        ASSERT(m_nRareCraftedNameBase + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nRareCraftedNameBase + index);
    }
    const CString & UniqueName(UINT index) const{
        ASSERT(m_nUniqueNameBase + index < m_saLanguage[m_nLangIndex].size());
		return String(m_nUniqueNameBase + index);
    }
    CString ClassName(UINT id) const{                   //角色名字,主要是越界处理
        return id < 7 ? String(24 + id) : String(15);
    }
    CString TimeName(UINT index) const{                 //时刻名字,主要是越界处理,by time属性需要
        return index < 4 ? String(410 + index) : String(15);
    }
    CString SkillTabName(UINT tab,UINT class_id) const{ //根据角色id和技能树索引,得到技能树名字
        const int TAB_INDEX[7] = {132,166,200,234,268,302,336};
        if(tab > 2 || class_id > 6)
            return String(15);
        return String(TAB_INDEX[class_id] + tab);
    }
	const CItemDataStruct * ItemData(DWORD typeID) const {	//如果没有,返回0
		std::unordered_map<DWORD, CItemDataStruct>::const_iterator wh = m_mapItemData.find(typeID);
		return wh == m_mapItemData.end() ? 0 : &wh->second;
	}
	const CProperty & PropertyData(UINT index) const {
		ASSERT(index < m_vProperty.size());
		return m_vProperty[index];
	}

};

extern CDiabloEdit2App theApp;
