//����ʾ����
//#include <contact.h>
#include "contact.h"

int main()
{
	struct Contact* data = (struct Contact*)malloc(sizeof(struct Contact));
	assert(data);
	memset(data, 0, sizeof(struct Contact));
	//����ʾ���������data�������һ��ָ���ˣ�������ȡ��ַ������ͳ��˶���ָ��
	//data->next = &data;
	//data->prior = &data;
	data->next = data;
	data->prior = data;
	data->is_sentinel = 1;

	void (*func[])(struct Contact*) = { Exit, Create, Browse, Delete, Modify, Search, Sort };

	int sel = 0;
	while (sel = Select())
	{
		//����ʾ��
		//func[sel](&data);
		func[sel](data);
		printf("\n");
	}
	
	Exit(data);

	return 0;
}