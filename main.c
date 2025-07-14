//¥ÌŒÛ æ∑∂£∫
//#include <contact.h>
#include "contact.h"

int main()
{
	struct Contact* data = (struct Contact*)malloc(sizeof(struct Contact));
	assert(data);
	memset(data, 0, sizeof(struct Contact));

	void (*func[])(struct Contact*) = { Exit, Create, Browse, Delete, Modify, Search, Sort };

	int sel = 0;
	while (sel = Select())
	{
		//¥ÌŒÛ æ∑∂
		//func[sel](&data);
		func[sel](data);
		printf("\n");
	}
	

	return 0;
}