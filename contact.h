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

struct Contact
{
	//¥ÌŒÛ æ∑∂
	//char[20] name;
	char name[20];
	int age;
	enum Gender gender;
	char tel[12];
	char address[30];

	struct Contact* prior;
	struct Contact* next;
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