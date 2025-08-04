#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>


enum Gender
{
	male = 1,
	female,
	secret
};

//改为带哨兵位的双向循环链表，加一个字段用来表示是不是哨兵节点
struct Contact
{
	//错误示范
	//char[20] name;
	char name[20];
	int age;
	enum Gender gender;
	char tel[12];
	char address[30];

	struct Contact* prior;
	struct Contact* next;
	int is_sentinel;
};



//select
int Select();

//create
void Create(struct Contact* pc);


//browse
void Browse(struct Contact* pc);


//delete
void Delete(struct Contact* pc);


//modify
void Modify(struct Contact* pc);


//search
void Search(struct Contact* pc);

//sort
void Sort(struct Contact* pc);

//exit
void Exit(struct Contact* pc);