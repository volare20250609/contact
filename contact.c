#include "contact.h"

//----------core----------

//block location

/*
static struct Contact* loc(
	const struct Contact* const pc,
	void* const pvalue,
	const int (*const filter)(const struct Contact* const pc, void* const pvalue))
{
	//�Ż����ҵ���Ե�ڵ㻹û�ҵ���ֱ�ӷ���NULL
	//��������ô�Ż����ҵ���Ե�ڵ㷵�ص���Ȼ����NULL
	//if (filter(pc, pvalue, sz) || pc->next == NULL)
	//Ӧ����ô��������Ϊ�˷�ֹԽ����ʣ�pcӦ�÷�ǰ��
	if (pc && filter(pc, pvalue))
	{
		//ǿ������ת���������C4090����
		return (struct Contact*)pc;
	}
	else
	{
		//����ʾ��
		//return loc(pc->next, filter, pvalue, sz);
		return loc(pc->next, pvalue, filter);
	}
}
*/

//����Ϊ�ɷ����������Ϊ���ڱ�λ��˫��ѭ�������
//0���ݹ��ѭ��
//1����Ե�ڵ㣨ѭ��������������NULL��Ϊ�ڱ��ڵ�
//2�������ǰ���Һ������ң�����front��back���ӣ�ͨ���ص�����ʵ��
static struct Contact* loc(
	const struct Contact* const pc,
	void* const pvalue,
	const int (* const filter)(const struct Contact* const pc, void* const pvalue),
	const struct Contact* (* const op)(const struct Contact* const pc))
{
	assert(pc);

	const struct Contact* i = NULL;

	//����ֻ��һ���ڵ������жϱ�Ե�ڵ㣬ֻ�ܼ�һ����������ˡ���Ȼ�е��󣬵���û�취
	if (filter(pc, pvalue))
	{
		return (struct Contact*)pc;
	}

	for (i = op(pc); i->is_sentinel == 0; i = op(i))
	{
		if (filter(i, pvalue))
		{
			//����ʾ�����������ĳ�i��
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
	memset(pc->next, 0, sizeof(struct Contact));//��ǿ�Ƚ���
	pc->next->prior = pc;
	pc->next->next = next;
	//����ʾ������������һ��
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

//����Ϊ�ɷ�������Ϊ��Ϊ���ڱ�λ��˫��ѭ�������
//0���ݹ��ѭ��
//1��ͳһ����������front��back���ӣ�ͨ���ص�����ʵ��
//2����Ե�ڵ㣨ѭ��������������NULL��Ϊ�ڱ��ڵ�
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
//����ʾ����remove�Ѿ���һ���⺯����
//static struct Contact* remove(struct Contact* const pc)
static struct Contact* erase(struct Contact* pc)
{
	//����ֻ��һ���ڱ�λ��������������pc != NULL���ж�����
	if (pc != NULL && pc->is_sentinel == 0)
	{
		struct Contact* prior = pc->prior;
		prior->next = pc->next;
		//����ʾ���������޸���һ�ڵ��prior
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
//����ʾ��
//static struct Contact* assign(
//	struct Contact* const pc,
//	const char* const name,
//	const int* const age,
//	const enum Gender* const gender,
//	const char* const tel,
//	const char* const address)
static struct Contact* assign(
	struct Contact* const pc,
	const char* name,//�����*����ȥ������ΪstrcpyҪ���õ�ַ
	const int age,
	const enum Gender gender,
	const char* tel,
	const char* address)
{

	if (pc && pc->is_sentinel == 0)
	{
		//����ʾ��
		////���ϼӴ�ʾ��
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

//����Ϊ�ɰ汾�������Ǹ�Ϊ���ڱ�λ��˫��ѭ�������ķ���
static struct Contact* bubble_sort(struct Contact* const pc, int sz, int step, int (*cmp)(void* e1, void* e2))
{
	//����ʾ�����ж��Ƿ�ֻ��һ���ڵ㲻����pc������pc->next
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

	//sz���ö�����Ϊ��count�������Ѿ��޳��ڱ��ڵ���
	for (i = 0; i < sz; i++)
	{
		for (j = 0; j < sz - 1 - i; j++)
		{
			count = j;
			//��Ϊpc->next
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
		//����ʾ������������return 0;
		return 0;
	}
}

//name filter
static int is_name(const struct Contact* const pc, void* const pvalue)
{
	//����ʾ����������strcmp�Ĺ���
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
	//����ʾ����������strcmp�Ĺ���
	//return strcmp(pc->tel, pvalue);
	return strcmp(pc->tel, pvalue) == 0;
}

//address filter
static int is_addr(const struct Contact* const pc, void* const pvalue)
{
	//����ʾ����������strcmp�Ĺ���
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

	//����ʾ��
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
	//����ʾ��
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
	//�����ڱ��ڵ�������targetӦ��+1
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
	//�����ڱ��ڵ�������targetӦ��+1
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
	//����ʾ������������pvalue
	//struct Contact* ret = loc(pc, NULL, filters[sel]);
	printf("Input value:>");
	void* pvalue = NULL;
	//���ﲻǿ������ת���Ա���C4133����
	pvalue = sel == 2 ? malloc(sizeof(int)) : malloc(30);

	if (pvalue)
	{
		//ǿ������ת���Ա���C4477����
		sel == 2 ? scanf("%d", (int*)pvalue) : scanf("%s", (char*)pvalue);
	}
	else
	{
		printf("Search tool initiate failed.\n");
	}

	//ǿ������ת���Ա���C4090����
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

//����Ϊ�ɰ汾�������Ǹ�Ϊ���ڱ�λ��˫��ѭ�������ķ���
void Exit(struct Contact* pc)
{
	struct Contact* i = NULL;
	struct Contact* next = NULL;

	//����ʾ�����ᵼ��Խ�����
	//for (i = pc->next; i->is_sentinel == 0; i = i->next)
	//for (i = pc->next; i != NULL && i->is_sentinel == 0; i = i->next)
	//{
	//	free(i->prior);
	//	i->prior = NULL;
	//}
	//free(i);

	//����ʾ�����ͼ�����
	//for (i = pc->next; i->next->is_sentinel = 0; i = next)
	//����ʾ�����ᵼ�±�Ե�ڵ�©��
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