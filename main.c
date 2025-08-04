//错误示范：
//#include <contact.h>
#include "contact.h"

int main()
{
	struct Contact* data = (struct Contact*)malloc(sizeof(struct Contact));
	assert(data);
	memset(data, 0, sizeof(struct Contact));
	//错误示范：这里的data本身就是一个指针了，不能再取地址，否则就成了二级指针
	//data->next = &data;
	//data->prior = &data;
	data->next = data;
	data->prior = data;
	data->is_sentinel = 1;

	void (*func[])(struct Contact*) = { Exit, Create, Browse, Delete, Modify, Search, Sort };

	int sel = 0;
	while (sel = Select())
	{
		//错误示范
		//func[sel](&data);
		func[sel](data);
		printf("\n");
	}
	
	Exit(data);

	return 0;
}