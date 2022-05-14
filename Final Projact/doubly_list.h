//Daniel Michael 312278583
//Matan Shimon 203263710
#include <stdbool.h>
typedef struct items {
	char movie_name[20];
	char producer_name[20];
	int release_date;
	float rating;
	int serial_number;
	bool is_adult_video;
}items;

typedef items list_type;

typedef struct node
{
	list_type value;
	struct node* next;
	struct node* prev;
}node;

typedef struct list
{
	node* head;
	node* tail;
}list;


void initList(list* lst);

int isEmpty(list* lst);

int length(list* lst);

node* allocItem(list_type value);

node* getLinkByIndex(list* lst, int idx);

int insertFirst(list* lst, node* item);

int insertLast(list* lst, node* item);

int deleteFirst(list* lst);

int deleteLast(list* lst);

int deleteLink(list* lst, node* link);

int insertAfter(list* lst, node* item, node* link);

int insertBefore(list* lst, node* item, node* link);
