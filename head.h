#ifndef FAMILY_H_INCLUDED
#define FAMILY_H_INCLUDED
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct node{
	int id;//编号 
	int parentid;//父母编号 
	char name[20];//姓名 
	char birthplace[20];//出生地 
	char birthday[10];//出生日期 
	char deathday[10];//死亡日期 
	char sex;//性别 
	float stature;//身高 
	char education[20];//学历 
	char profession[20];//职业
	int age;//年龄
	int generation;//代 
	int aescendantnum;//后代数
	int aescendant[10];//后代数组,假设最多只能有十个后代
}node;

void display();//初始界面 
void Initialize();//清空数据,初始化成员数组
void ReadFile();//读取文件内容,初始化成员数组 
void AddMemberInfo();//添加成员信息
int QueryMemberIndex(int id);//查询id编号在成员数组中的下标 
int QueryChildrenId(int id,int head);////查询id编号没有没有在孩子树head里面,1表示在,0表示不在 
void ChangeGeneration(int id);//修改子代中代的信息
void ModifierMemberInfo();//修改成员信息
void DeleteMemberInfo();//删除成员信息
void QueryMemberBasicInfo();//查询成员基础信息
void QueryMemberFamilyInfo();//查询成员家庭信息
void DisplayTree(int id);//显示家族树的递归 
void DisplayFamilyTree(int id);//显示家族树的main函数 
void countMemberInfo();//统计成员的基本信息
void DisplayMemberInfo();//输出所有成员的信息
int IsNameRepeate(char name[]);//判断成员姓名是否重复
void WriteToFile();//把成员数组的数据写入文件中 

#endif // FAMILY_H_INCLUDED

