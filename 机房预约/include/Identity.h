#pragma once
#include<iostream>
using namespace std;

//身份抽象基类
class Indentity
{
public:

	//操作菜单——纯虚函数
	virtual void operMenu() = 0;


	string m_UserName;
	string m_UserPassword;
};
