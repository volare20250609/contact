#include "contact.h"

//----------core----------

//block location

/*
static struct Contact* loc(
	const struct Contact* const pc,
	void* const pvalue,
	const int (*const filter)(const struct Contact* const pc, void* const pvalue))
{
	//优化：找到边缘节点还没找到，直接返回NULL
	//错！！！这么优化后，找到边缘节点返回的依然不是NULL
	//if (filter(pc, pvalue, sz) || pc->next == NULL)
	//应该这么做。而且为了防止越界访问，pc应该放前面
	if (pc && filter(pc, pvalue))
	{
		//强制类型转换用来解决C4090警报
		return (struct Contact*)pc;
	}
	else
	{
		//错误示范
		//return loc(pc->next, filter, pvalue, sz);
		return loc(pc->next, pvalue, filter);
	}
}
*/

//以上为旧方案，下面改为带哨兵位的双向循环链表后：
//0、递归改循环
//1、边缘节点（循环结束条件）由NULL改为哨兵节点
//2、拆分向前查找和向后查找，定义front和back算子，通过回调函数实现
static struct Contact* loc(
	const struct Contact* const pc,
	void* const pvalue,
	const int (* const filter)(const struct Contact* const pc, void* const pvalue),
	const struct Contact* (* const op)(const struct Contact* const pc))
{
	assert(pc);

	const struct Contact* i = NULL;

	//对于只有一个节点的情况判断边缘节点，只能加一种特殊情况了。虽然有点矬，但是没办法
	if (filter(pc, pvalue))
	{
		return (struct Contact*)pc;
	}

	for (i = op(pc); i->is_sentinel == 0; i = op(i))
	{
		if (filter(i, pvalue))
		{
			//错误示范：这里忘改成i了
			//return (struct Contact*)pc;
			return (struct Contact*)i;
		}
	}

	return NULL;
}

//insert a new block after a certain block, input the pointer after which the new block is inserted
//return the pointer of this new block
static struct Contact* insert(struct Contact* const pc)
{
	struct Contact* next = pc->next;
	pc->next = (struct Contact*)malloc(sizeof(struct Contact));
	if ((pc->next) == NULL)
	{
		pc->next = next;
		return NULL;
	}
	memset(pc->next, 0, sizeof(struct Contact));//增强稳健性
	pc->next->prior = pc;
	pc->next->next = next;
	//错误示范：忘记了这一步
	next->prior = pc->next;
	return pc->next;
}

/*
//count block number ending from a certain one
static int count_before(const struct Contact* const pc)
{
	return pc->prior == NULL ? 1 : (1 + count_before(pc->prior));
}

//count block number starting from a certain one
static int count_after(const struct Contact* const pc)
{
	return pc->next->next == NULL ? 1 : (1 + count_after(pc->next));
}
*/

//以上为旧方案，改为改为带哨兵位的双向循环链表后：
//0、递归改循环
//1、统一函数，定义front和back算子，通过回调函数实现
//2、边缘节点（循环结束条件）由NULL改为哨兵节点
static int count(const struct Contact* const pc,
				 const struct Contact* (* const op)(const struct Contact* const pc))
{
	assert(pc);

	const struct Contact* i = NULL;
	int sum = 0;

	for (i = op(pc); i->is_sentinel == 0; i = op(i))
	{
		sum++;
	}

	return sum;
}

//remove a block
//input the block pointer
//return the prior (next in case of first block) block pointer after removement
//错误示范：remove已经是一个库函数了
//static struct Contact* remove(struct Contact* const pc)
static struct Contact* erase(struct Contact* pc)
{
	//对于只有一个哨兵位的情况，这里加上pc != NULL的判断条件
	if (pc != NULL && pc->is_sentinel == 0)
	{
		struct Contact* prior = pc->prior;
		prior->next = pc->next;
		//错误示范：忘记修改下一节点的prior
		pc->next->prior = prior;
		free(pc);
		pc = NULL;
		return prior;
	}
	else
	{
		return NULL;
	}
	
}

//set block values
//错误示范
//static struct Contact* assign(
//	struct Contact* const pc,
//	const char* const name,
//	const int* const age,
//	const enum Gender* const gender,
//	const char* const tel,
//	const char* const address)
static struct Contact* assign(
	struct Contact* const pc,
	const char* name,//这里的*不能去掉，因为strcpy要引用地址
	const int age,
	const enum Gender gender,
	const char* tel,
	const char* address)
{

	if (pc && pc->is_sentinel == 0)
	{
		//错误示范
		////错上加错示范
		////pc->name = name;
		//*pc->name = name;
		//pc->age = age;
		//pc->gender = gender;
		//*pc->tel = tel;
		//*pc->address = address;

		strcpy(pc->name, name);
		pc->age = age;
		pc->gender = gender;
		strcpy(pc->tel, tel);
		strcpy(pc->address, address);

		return pc;
	}
	else
	{
		return NULL;
	}
	
}


//sort
static void chg_by_bit(char* const e1, char* const e2, const int step)
{
	int i = 0;
	char tmp = 0;
	for (i = 0; i < step; i++)
	{
		tmp = e1[i];
		e1[i] = e2[i];
		e2[i] = tmp;
	}
}

/*
static struct Contact* bubble_sort(void* const pc, int sz, int step, int (*cmp)(void* e1, void* e2))
{
	if (pc == NULL)
	{
		return NULL;
	}

	int i = 0;
	int j = 0;
	int flag = 1;
	struct Contact* former = NULL;
	struct Contact* latter = NULL;
	int count = 0;

	for (i = 0; i < sz; i++)
	{
		for (j = 0; j < sz - 1 - i; j++)
		{
			count = j;
			former = (j == 0) ? pc : latter;
			latter = former->next;
			if (cmp((void*)former, (void*)latter) > 0)
			{
				chg_by_bit((char* const)former, (char* const)latter, step);
				flag = 0;
			}
		}

		if (flag)
		{
			break;
		}
	}

	return pc;
}
*/

//以上为旧版本，下面是改为带哨兵位的双向循环链表后的方案
static struct Contact* bubble_sort(struct Contact* const pc, int sz, int step, int (*cmp)(void* e1, void* e2))
{
	//错误示范：判断是否只有一个节点不能用pc，而是pc->next
	//if (pc == NULL || pc->is_sentinel == 1)
	if (pc == NULL || pc->next->is_sentinel == 1)
	{
		return NULL;
	}

	int i = 0;
	int j = 0;
	int flag = 1;
	struct Contact* former = NULL;
	struct Contact* latter = NULL;
	int count = 0;

	//sz不用动，因为在count函数就已经剔除哨兵节点了
	for (i = 0; i < sz; i++)
	{
		for (j = 0; j < sz - 1 - i; j++)
		{
			count = j;
			//改为pc->next
			former = (j == 0) ? pc->next : latter;
			latter = former->next;
			if (cmp((void*)former, (void*)latter) > 0)
			{
				chg_by_bit((char* const)former, (char* const)latter, step);
				flag = 0;
			}
		}

		if (flag)
		{
			break;
		}
	}

	return pc;
}

//----------filters----------

//edge filter
static int is_edge(const struct Contact* const pc, void* const pvalue)
{
	//return (pc->next == NULL);
	return pc->next->is_sentinel;
}

//count filter
static int is_number(const struct Contact* const pc, void* const pvalue)
{
	if (*(int*)pvalue == 1)
	{
		return 1;
	}
	else
	{
		(*(int*)pvalue)--;
		//错误示范：这里忘了return 0;
		return 0;
	}
}

//name filter
static int is_name(const struct Contact* const pc, void* const pvalue)
{
	//错误示范：忘记了strcmp的规则
	//return strcmp(pc->name, pvalue);
	return strcmp(pc->name, pvalue) == 0;
}

//age filter
static int is_age(const struct Contact* const pc, void* const pvalue)
{
	return pc->age == *(int*)pvalue;
}

//gender filter
static int is_gender(const struct Contact* const pc, void* const pvalue)
{
	return pc->gender == *(int*)pvalue;
}
//tel filter
static int is_tel(const struct Contact* const pc, void* const pvalue)
{
	//错误示范：忘记了strcmp的规则
	//return strcmp(pc->tel, pvalue);
	return strcmp(pc->tel, pvalue) == 0;
}

//address filter
static int is_addr(const struct Contact* const pc, void* const pvalue)
{
	//错误示范：忘记了strcmp的规则
	//return strcmp(pc->address, pvalue);
	return strcmp(pc->address, pvalue) == 0;
}


//----------operators----------

//front
static struct Contact* front(const struct Contact* const pc)
{
	return pc->next;
}

//back
static struct Contact* back(const struct Contact* const pc)
{
	return pc->prior;
}

//----------cmps----------
static int cmp_name(void* e1, void* e2)
{
	return strcmp(((struct Contact*)e1)->name, ((struct Contact*)e2)->name);
}

static int cmp_age(void* e1, void* e2)
{
	return ((struct Contact*)e1)->age - ((struct Contact*)e2)->age;
}

static int cmp_tel(void* e1, void* e2)
{
	return strcmp(((struct Contact*)e1)->tel, ((struct Contact*)e2)->tel);
}

static int cmp_addr(void* e1, void* e2)
{
	return strcmp(((struct Contact*)e1)->address, ((struct Contact*)e2)->address);
}

//----------GUI----------

//menu
int Select()
{
	srand((unsigned int)time(NULL));
	char* text[] = { "Hehe.", "Haha.", "Xixi." };

input_loop:
	printf("CONTACT\n1.Create;\n2.Browse;\n3.Delete;\n4.Modify;\n5.Search;\n6.Sort;\n0.Exit.\n");
	printf("Input here:> ");
	int input = 0;
	scanf("%d", &input);
	if (input >= 0 && input <= 6)
	{
		return input;
	}
	else
	{
		printf("%s\n\n",text[rand()%(sizeof text)/(sizeof text[0])]);
		goto input_loop;
	}
}


//create
void Create(struct Contact* pc)
{
	char name[20] = { 0 };
	int age = 0;
	enum Gender gender = 0;
	char tel[12] = { 0 };
	char address[30] = { 0 };

	printf("Name:>");
	scanf("%s", &name);
	printf("Age:>");
	scanf("%d", &age);
	printf("Gender (1:Male, 2:Female, 3:Secret):>");
	scanf("%d", &gender);
	printf("Phone Number:>");
	scanf("%s", &tel);
	printf("Address:>");
	scanf("%s", &address);

	struct Contact* edge = loc(pc, NULL, is_edge, back);
	struct Contact* new = NULL;

	//错误示范
	//insert(pc);
	if (new = insert(edge))
	{
		assign(
			new,
			name, age, gender, tel, address);
		printf("Saved.\n");
	}
	else
	{
		printf("Failed.\n");
	}

	
}


//browse
void Browse(struct Contact* pc)
{
	printf("%-10s%-20s%-20s%-20s%-20s%-20s\n", "No.", "Name", "Age", "Gender", "Tel.", "Address");
	struct Contact* i = NULL;
	int num = 1;
	//错误示范
	//for (i = pc; i != NULL; i = i->next)
	for (i = pc->next; i->is_sentinel == 0; i = i->next)
	{
		printf("%-10d%-20s%-20d%-20d%-20s%-20s\n",
			num, i->name, i->age, i->gender, i->tel, i->address);
		num++;
	}
}


//delete
void Delete(struct Contact* pc)
{
	int target = 0;
	printf("Input the number of which to delete:>");
	scanf("%d", &target);
	//加了哨兵节点后，这里的target应该+1
	target++;
	if (erase(loc(pc, &target, is_number, front)))
	{
		printf("Deleted.\n");
	}
	else
	{
		printf("No contact to delete.\n");
	}
	
}


//modify
void Modify(struct Contact* pc)
{
	int target = 0;
	printf("Input the number of which to modify:>");
	scanf("%d", &target);
	//加了哨兵节点后，这里的target应该+1
	target++;

	char name[20] = { 0 };
	int age = 0;
	enum Gender gender = 0;
	char tel[12] = { 0 };
	char address[30] = { 0 };

	printf("Name:>");
	scanf("%s", &name);
	printf("Age:>");
	scanf("%d", &age);
	printf("Gender (1:Male, 2:Female, 3:Secret):>");
	scanf("%d", &gender);
	printf("Phone Number:>");
	scanf("%s", &tel);
	printf("Address:>");
	scanf("%s", &address);

	if (assign(
		loc(pc, &target, is_number, front),
		name, age, gender, tel, address))
	{
		printf("Modified.\n");
	}
	else
	{
		printf("No such contact.\n");
	}

}


//search
void Search(struct Contact* pc)
{
	int (*filters[])(void*, void*) = { 0,is_name,is_age,is_tel,is_addr };
	int sel = 0;
	printf("Search by what?\n1.Name\n2.Age\n3.Tel\n4.Address\nInput here:>");
	scanf("%d", &sel);
	//错误示范：忘记输入pvalue
	//struct Contact* ret = loc(pc, NULL, filters[sel]);
	printf("Input value:>");
	void* pvalue = NULL;
	//这里不强制类型转换以避免C4133警告
	pvalue = sel == 2 ? malloc(sizeof(int)) : malloc(30);

	if (pvalue)
	{
		//强制类型转换以避免C4477警告
		sel == 2 ? scanf("%d", (int*)pvalue) : scanf("%s", (char*)pvalue);
	}
	else
	{
		printf("Search tool initiate failed.\n");
	}

	//强制类型转换以避免C4090警告
	struct Contact* ret = loc(pc, pvalue,
							  (const int (*const)(const struct Contact* const, void* const))filters[sel],
							  front);

	free(pvalue);
	pvalue = NULL;
	if (ret)
	{
		printf("%-10s%-20s%-20s%-20s%-20s%-20s\n", "No.", "Name", "Age", "Gender", "Tel.", "Address");
		printf("%-10d%-20s%-20d%-20d%-20s%-20s\n",
			count(ret, front), ret->name, ret->age, ret->gender, ret->tel, ret->address);
	}
	else
	{
		printf("No such contact.\n");
	}

}


//sort
void Sort(struct Contact* pc)
{
	int (*cmps[])(void*, void*) = { 0,cmp_name,cmp_age,cmp_tel,cmp_addr };
	int sel = 0;
	printf("Sort by what?\n1.Name\n2.Age\n3.Tel\n4.Address\nInput here:>");
	scanf("%d", &sel);
	struct Contact* ret = bubble_sort(
						  pc,
						  count(pc, front),
						  sizeof(*pc) - sizeof(pc->prior) - sizeof(pc->next),
						  cmps[sel]);
	if (ret)
	{
		printf("Sort successful.\n");
	}
	else
	{
		printf("Filed.\n");
	}
}

//exit
/*
void Exit(struct Contact* pc)
{
	if (pc->next)
	{
		Exit(pc->next);
	}
	if (pc->prior == NULL)
	{
		printf("Bye!\n");
	}
	free(pc);
}
*/

//以上为旧版本，下面是改为带哨兵位的双向循环链表后的方案
void Exit(struct Contact* pc)
{
	struct Contact* i = NULL;
	struct Contact* next = NULL;

	//错误示范：会导致越界访问
	//for (i = pc->next; i->is_sentinel == 0; i = i->next)
	//for (i = pc->next; i != NULL && i->is_sentinel == 0; i = i->next)
	//{
	//	free(i->prior);
	//	i->prior = NULL;
	//}
	//free(i);

	//错误示范：低级错误
	//for (i = pc->next; i->next->is_sentinel = 0; i = next)
	//错误示范：会导致边缘节点漏掉
	//for (i = pc->next; i->next->is_sentinel == 0; i = next)
	for (i = pc->next; i->is_sentinel == 0; i = next)
	{
		next = i->next;
		free(i);
	}

	i = NULL;
	free(pc);
	pc = NULL;

	printf("Bye!\n");
}