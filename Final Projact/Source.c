//Daniel Michael 312278583
//Matan Shimon 203263710
#define	_CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "doubly_list.h"
#include<io.h>

typedef struct user {
	int SecurityLevel;
	char ID[15];
	char Password[15];
	char FullName[20];
}user;
//char ITEMS_PATH[] = "item.dat";
#define USERS_PATH "users.dat"
//#define ITEMS_PATH "item.dat"
#define LOG_PATH "log.txt"

//Current Time
void getDateTime(int* day, int* month, int* year, int* hours, int* mins)
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	*day = timeinfo.tm_mday;
	*month = timeinfo.tm_mon + 1;
	*year = timeinfo.tm_year + 1900;
	*hours = timeinfo.tm_hour;
	*mins = timeinfo.tm_min;
}

//Current Time for log
void CTfl(char *l)
{
	int day, month, year, hours, mins;
	getDateTime(&day, &month, &year, &hours, &mins);
	fprintf(l, "Current computer time: %02d/%02d/%d, %02d:%02d\n", day, month, year, hours, mins);
}

//Register Function
void register_new_users(char* new_user_to_system)
{
int num_of_ged_user = 0;
int x = 0;
printf("Enter The Number Of Users You Wish To Register: ");
char id[15], pass[15], f_n[20];
user add_new_user_to_system;

scanf("%d", &num_of_ged_user);

	while (x < num_of_ged_user)
	{
		FILE* f = fopen(new_user_to_system, "ab+");
		printf("Enter Your ID:");
		scanf("%s", &id);
		strcpy(&add_new_user_to_system.ID, id);

		printf("Enter Your Password: ");
		scanf("%s", &pass);
		strcpy(&add_new_user_to_system.Password, pass);

		printf("Enter Your Full Name: ");
		scanf("%s", &f_n);
		strcpy(&add_new_user_to_system.FullName, f_n);

		printf("Enter Your Security Level: ");
		scanf("%d", &add_new_user_to_system.SecurityLevel);

		fwrite(&add_new_user_to_system, sizeof(user), 1, f);
		fclose(f);

		x++;

		FILE* l = fopen("log.txt", "a+");
		fprintf(l, "\nA new user has been created, with an ID %s and a Full name %s\n", id, f_n);
		CTfl(l);
		fclose(l);
	}
}

//Delete User Function
void Delete_user(char* delete_user_from_system)
{
	user del_user;

	long loction_in_file;
	char id_input_to_delete[15];
	char id[15], pass[15], f_n[20];
	int s_l = 0, cnt = 0,num_of_users=0;

	printf("\n\nEnter ID Of The User You Wish To Delete: ");
	scanf("%s", id_input_to_delete);
	flushall();
	FILE* d_u = fopen(delete_user_from_system, "rb+");

	while (!feof(d_u))
	{
		fread(&del_user, sizeof(user), 1, d_u);
		if (strcmp(id_input_to_delete, del_user.ID) == 0)
		{
			//printf("\n\nID:%s \nfull name:%s \nSecurityLevel:%d", del_user.ID, del_user.FullName, del_user.SecurityLevel);
			break;
		}
		cnt++;
	}

	rewind(d_u);
	while (!feof(d_u))
	{
		fread(&del_user, sizeof(user), 1, d_u);
		num_of_users++;
	}
	fclose(d_u);
	
	d_u = fopen(delete_user_from_system, "rb+");
	fseek(d_u, sizeof(user)* (num_of_users - 1), SEEK_SET);
	fread(&del_user, sizeof(user), 1, d_u);
	strcpy(id, &del_user.ID);
	strcpy(pass, &del_user.Password);
	strcpy(f_n, &del_user.FullName);
	s_l = del_user.SecurityLevel;
	//printf("\n\nID:%s \nfull name:%s \nSecurityLevel:%d", del_user.ID, del_user.FullName, del_user.SecurityLevel);
	fclose(d_u);

	d_u = fopen(delete_user_from_system, "rb+");
	fseek(d_u, sizeof(user)*cnt, SEEK_SET);
	strcpy(&del_user.ID, id);
	strcpy(&del_user.Password, pass);
	strcpy(&del_user.FullName, f_n);
	del_user.SecurityLevel = s_l;
	fwrite(&del_user, sizeof(user), 1, d_u);

	rewind(d_u);
	_chsize_s(_fileno(d_u), sizeof(user) * (num_of_users - 2));//delete size of file

	fclose(d_u);

	FILE* l = fopen("log.txt", "a+");
	fprintf(l, "\nThe user with the ID %s has been deleted\n", id_input_to_delete);
	CTfl(l);
	fclose(l);

}

//LogIn Function
int LogIn (char* access_to_user_file, char* log)
{
	user new_user;
	int day, month, year, hours, mins;
	int number_of_error = 0, successful_log_in_flag = 0, sec_lvl = 0;
	char id_input[15], password_input[15];
	while (number_of_error != 3)
	{
		printf("\n\nEnter ID: ");
		scanf("%s", id_input);
		flushall();
		printf("Enter Password : ");
		scanf("%s", password_input);
		flushall();

		FILE* l = fopen(log, "a+"); //text file
		fprintf(l, "\nthe user entered the ID : %s\n", id_input);
		fprintf(l, "the user entered the password : %s\n", password_input);
		getDateTime(&day, &month, &year, &hours, &mins);
		fprintf(l, "Current computer time: %02d/%02d/%d, %02d:%02d\n", day, month, year, hours, mins);


		FILE* users_out = fopen(access_to_user_file, "rb");// user file
		while (!feof(users_out))
		{
			fread(&new_user, sizeof(user), 1, users_out);
			//printf("\n\nID:%s \nfull name:%s \nSecurityLevel:%d",new_user.ID, new_user.FullName, new_user.SecurityLevel);
			if (strcmp(password_input, new_user.Password) == 0 && (strcmp(id_input, new_user.ID) == 0))
			{
				successful_log_in_flag = 1;
				sec_lvl = new_user.SecurityLevel;
				printf("\nLog in was successful.....\n\n");
				break;
			}

		}

		fclose(l);
		fclose(users_out);

		if (successful_log_in_flag == 1)
			break;
		else
			number_of_error++;
		printf("\nLog In was Unseccessful.....");
	}
	
	return sec_lvl;


}

//Add Items Function
void add_item(char* new_item_to_system,list * item_list)
{
	char m_n[20], p_n[20],is_adult[4];
	int s_n = 0;
	items add_new_item_to_system;
	int num_of_items = 0,x=0; 

	printf("Enter The Number Of Items You Wish To Add: ");
	scanf("%d", &num_of_items);

	while (x < num_of_items)
	{
		FILE* a_d = fopen(new_item_to_system, "ab+");
		printf("\nEnter Movie Name:");
		scanf("%s", &m_n);
		strcpy(&add_new_item_to_system.movie_name, m_n);

		printf("Enter Producer Name: ");
		scanf("%s", &p_n);
		strcpy(&add_new_item_to_system.producer_name, p_n);

		printf("Enter Release Date like this format: YYYYMMDD ");
		scanf("%d", &add_new_item_to_system.release_date);

		printf("The automatic serial number is: ");
		add_new_item_to_system.serial_number = SN_generator("item.dat");
		printf("%d", add_new_item_to_system.serial_number);

		printf("\nEnter Rating: ");
		scanf("%f", &add_new_item_to_system.rating);

		printf("Is The Movie For Adults? ");
		printf("\nAnswer With yes or no: ");
		scanf("%s", &is_adult);

		if (strcmp(is_adult, "yes") == 0)
			add_new_item_to_system.is_adult_video = 1;
		else if (strcmp(is_adult, "no") == 0)
			add_new_item_to_system.is_adult_video = 0;

		printf("\nYou Just Entered:");
		printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", add_new_item_to_system.movie_name, add_new_item_to_system.producer_name, add_new_item_to_system.release_date, add_new_item_to_system.serial_number, add_new_item_to_system.rating, add_new_item_to_system.is_adult_video ? "yes" : "no");
		printf("\n");
		fwrite(&add_new_item_to_system, sizeof(items), 1, a_d);
		fclose(a_d);

		FILE* l = fopen("log.txt", "a+");
		fprintf(l, "\nA new movie has been created: movie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s\n", add_new_item_to_system.movie_name, add_new_item_to_system.producer_name, add_new_item_to_system.release_date, add_new_item_to_system.serial_number, add_new_item_to_system.rating, add_new_item_to_system.is_adult_video ? "yes" : "no");
		CTfl(l);
		fclose(l);

		x++;
	}
}

//Update Item Function 
void update_item (char* uptade_item_in_file)
{
	int serial_number=0,cnt=0,num_of_items=0;
	char m_n[20], p_n[20], is_adult[4];
	char answer[4];
	items update_item;

	printf("\n\nEnter The Serial-Number Of The Movie You Wish To Update: ");
	scanf("%d", &serial_number);
	
	FILE* u_i = fopen(uptade_item_in_file, "rb+");
	while (!feof(u_i))
	{
		fread(&update_item, sizeof(items), 1, u_i);
		if (serial_number == update_item.serial_number)
		{
			printf("\n\nthe data you want to update: ");
			printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", update_item.movie_name, update_item.producer_name, update_item.release_date, update_item.serial_number, update_item.rating, update_item.is_adult_video ? "yes" : "no");
			break;
		}
		cnt++;

	}

	FILE* l = fopen("log.txt", "a+");
	fprintf(l, "\nThe user made some updates with the serial number(movie): %d\n", serial_number);
	CTfl(l);
	fclose(l);

	fclose(u_i);

	u_i = fopen(uptade_item_in_file, "rb+");
	fseek(u_i, sizeof(items) * cnt, SEEK_SET);
	fread(&update_item, sizeof(items), 1, u_i);

			printf("\n\ndo you want to update Movie Name?  :");
			scanf("%s", &answer);
			//flushall();
			if (strcmp(answer, "yes") == 0)
			{
				printf("Enter The New Name : ");
				scanf("%s", &m_n);
				strcpy(update_item.movie_name, m_n);
			}

			printf("\ndo you want to update Producer Name?  :");
			scanf("%s", &answer);
			//flushall();
			if (strcmp(answer, "yes")==0)
			{
				printf("Enter The New Name : ");
				scanf("%s", &p_n);
				strcpy(update_item.producer_name, p_n);
			}

			printf("\ndo you want to update Release Dete?  :");
			scanf("%s", &answer);
			//flushall();
			if (strcmp(answer, "yes") == 0)
			{
				printf("Enter The New Date like this format: YYYYMMDD ");
				scanf("%d", &update_item.release_date);
			}

			printf("\ndo you want to update Rating?  :");
			scanf("%s", &answer);
			flushall();
			if (strcmp(answer, "yes") == 0)
			{
				printf("Enter The New Rating : ");
				scanf("%f", &update_item.rating);
			}

			printf("\ndo you want to update The  Classification age ?  :");
			scanf("%s", &answer);
			//flushall();
			if (strcmp(answer, "yes") == 0)
			{
				printf("\nIs The Movie For Adults? ");
				printf("\n Answer With yes or no: ");
				scanf("%s", &is_adult);

				if (strcmp(is_adult, "yes") == 0)
					update_item.is_adult_video = 1;
				else if (strcmp(is_adult, "no") == 0)
					update_item.is_adult_video = 0;
			}

	printf("\n\nthe new data after the update: ");
	printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", update_item.movie_name, update_item.producer_name, update_item.release_date, update_item.serial_number, update_item.rating, update_item.is_adult_video ? "yes" : "no");
	fseek(u_i, sizeof(items) * cnt, SEEK_SET);
	fwrite(&update_item, sizeof(items), 1, u_i);
	fclose(u_i);
}

//Delete Item Function
void delete_item(char* delete_item_in_file)
{
	int serial_number = 0,s_n=0, cnt = 0, num_of_items = 0,is_adlt, r_d=0;
	float rating = 0;
	char m_n[20], p_n[20], is_adult[4];
	items del_item;

	printf("\n\nEnter The Serial-Number Of The Movie You Wish To Delete: ");
	scanf("%d", &serial_number);
	FILE* d_i = fopen(delete_item_in_file, "rb+");
	while (!feof(d_i))
	{
		fread(&del_item, sizeof(items), 1, d_i);
		if (serial_number == del_item.serial_number)
		{
			printf("\n\nthe data you want to update: ");
			printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", del_item.movie_name, del_item.producer_name, del_item.release_date, del_item.serial_number, del_item.rating, del_item.is_adult_video ? "yes" : "no");
			break;
		}
		cnt++;
	}

	FILE* l = fopen("log.txt", "a+");
	fprintf(l, "\nThe serial number(movie) with the Serial Number %d has been deleted\n", serial_number);
	CTfl(l);
	fclose(l);

	rewind(d_i);
	while (!feof(d_i))
	{
		fread(&del_item, sizeof(items), 1, d_i);
		num_of_items++;
	}
	fclose(d_i);

	d_i = fopen(delete_item_in_file, "rb+");
	fseek(d_i, sizeof(items) * (num_of_items - 1), SEEK_SET);
	fread(&del_item, sizeof(items), 1, d_i);
	strcpy(m_n, del_item.movie_name);
	strcpy(p_n, del_item.producer_name);
	r_d = del_item.release_date;
	s_n = del_item.serial_number;
	rating = del_item.rating;
	is_adlt = del_item.is_adult_video;
	printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", del_item.movie_name, del_item.producer_name, del_item.release_date, del_item.serial_number, del_item.rating, del_item.is_adult_video ? "yes" : "no");

	fclose(d_i);

	d_i = fopen(delete_item_in_file, "rb+");
	fseek(d_i, sizeof(items) * cnt, SEEK_SET);
	strcpy(del_item.movie_name, m_n);
	strcpy(del_item.producer_name, p_n);
	del_item.release_date = r_d;
	del_item.serial_number=s_n;
	del_item.rating=rating;
	del_item.is_adult_video= is_adlt;
	fwrite(&del_item, sizeof(items), 1, d_i);

	rewind(d_i);
	_chsize_s(_fileno(d_i),sizeof(items)*(num_of_items-2));//delete size of file

	fclose(d_i);

	
}

//Function - Display all items
void display_all_items(char* display_all_items)
{
	int cnt = 0, num_of_items=0;
	items all_items;
	FILE* d_a_i = fopen(display_all_items, "rb+");

	while (!feof(d_a_i))
	{
		fread(&all_items, sizeof(items), 1, d_a_i);
		num_of_items++;
	}

	rewind(d_a_i);
	
	while (cnt<num_of_items-1)
	{
		fread(&all_items, sizeof(items), 1, d_a_i);
		printf("\n\nItem Number %d :\n", cnt);
		printf("movie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", all_items.movie_name, all_items.producer_name, all_items.release_date, all_items.serial_number, all_items.rating, all_items.is_adult_video ? "yes" : "no");
		cnt++;
	}
	fclose(d_a_i);

	FILE* l = fopen("log.txt", "a+");
	fprintf(l, "\nThe user selected the to view all the items\n");
	CTfl(l);
	fclose(l);
}

//Function - Display all users
void display_all_users(char* display_all_users)
{
	int cnt = 0, num_of_users = 0;
	user all_users;
	FILE* d_a_u = fopen(display_all_users, "rb+");

	while (!feof(d_a_u))
	{
		fread(&all_users, sizeof(user), 1, d_a_u);
		num_of_users++;
	}

	rewind(d_a_u);

	while (cnt < num_of_users - 1)
	{
		fread(&all_users, sizeof(user), 1, d_a_u);
		printf("\n\nUser Number %d :\n", cnt);
		printf("ID:%s \nFull_name:%s", all_users.ID, all_users.FullName);
		cnt++;
	}
	fclose(d_a_u);

	FILE* l = fopen("log.txt", "a+");
	fprintf(l, "\nThe user selected the to view all the users\n");
	CTfl(l);
	fclose(l);

}

//Function - Serial Number Generator
int SN_generator(char* SN_generator)
{
	int cnt = 0, serial_num = 0;
	items all_items;
	FILE* c_a_u = fopen(SN_generator, "rb+");

	while (!feof(c_a_u))
	{
		fread(&all_items, sizeof(items), 1, c_a_u);
		serial_num = all_items.serial_number;
	}
	fclose(c_a_u);
	if (serial_num < 0 || serial_num > 99999)
		serial_num = 0;
	return(serial_num+1);
}

//Search Items
void search_items(list* item_list, char* items_to_search)
{
	int x = 0;
	node* temp = item_list->head;

	FILE* s_i = fopen(items_to_search, "rb+");
	//list items_list;
	items temp_item;

	while (fread(&temp_item, sizeof(items), 1, s_i))
	{
		node* temp = allocItem(temp_item);
		insertLast(item_list, temp);
		x++;
	}

	temp = item_list->head;
	int p = 0;
	int option = 0;
	char M_N_by_user[20];
	int S_N_by_user;
	int YYYYMMDD_by_user;

	FILE* l = fopen("log.txt", "a+");
	printf("\nPlease select the search method you wish to use:\n");
	printf("1.Search by the name\n");
	printf("2.Search by bigger serial numbers than a specific serial number\n");
	printf("3.Search by smaller serial numbers than a specific serial number\n");
	printf("4.Search NSFW movies\n");
	printf("5.Search SFW movies\n");
	printf("6.Search from a specific date and further\n");
	printf("7.Search from a specific date and before\n");
	printf("8.Go Back\n");
	printf("Enter Action : ");
	scanf("%d", &option);
	switch (option)
	{
	case 1:
		printf("Please write the name of the movie you wish to search:\n");
		flushall();
		scanf("%s", M_N_by_user);
		while (temp != NULL)
		{
			if (strcmp_(M_N_by_user, temp->value.movie_name) == 1)
			{
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
				break;
			}
			else
			temp = temp->next;
	 	}
		fprintf(l, "\nThe user made a search for the name %s\n", M_N_by_user);
		CTfl(l);
		fclose(l);
		break;
	case 2:
		printf("Please enter from which serial number you wish to search\n");
		scanf("%d", &S_N_by_user);
		while (temp != NULL)
		{
			if (S_N_by_user <= temp->value.serial_number)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search(serial number goes up) for %d\n", S_N_by_user);
		CTfl(l);
		fclose(l);
		break;
	case 3:
		printf("Please enter until which serial number you wish to search\n");
		scanf("%d", &S_N_by_user);
		while (temp != NULL)
		{
			if (S_N_by_user >= temp->value.serial_number)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search(serial number goes down) for %d\n", S_N_by_user);
		CTfl(l);
		fclose(l);
		break;
	case 4:
		while (temp != NULL)
		{
			if (temp->value.is_adult_video == 1)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search for NSFW movies\n");
		CTfl(l);
		fclose(l);
		break;
	case 5:
		while (temp != NULL)
		{
			if (temp->value.is_adult_video == 0)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search for SFW movies\n");
		CTfl(l);
		fclose(l);
		break;
	case 6:
		printf("Please enter from which date you wish to search(YYYYMMDD)\n");
		scanf("%d", &YYYYMMDD_by_user);
		while (temp != NULL)
		{
			if (YYYYMMDD_by_user <= temp->value.release_date)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search for release date and further\n");
		CTfl(l);
		fclose(l);
		break;
	case 7:
		printf("Please enter until which date you wish to search(YYYYMMDD)\n");
		scanf("%d", &YYYYMMDD_by_user);
		while (temp != NULL)
		{
			if (YYYYMMDD_by_user >= temp->value.release_date)
				printf("\nmovie_name:%s \nproducer_name:%s \nrelease_date:%d  \nserial_number:%d  \nrating:%.1f  \nis adult video? :  %s", temp->value.movie_name, temp->value.producer_name, temp->value.release_date, temp->value.serial_number, temp->value.rating, temp->value.is_adult_video ? "yes" : "no");
			temp = temp->next;
		}
		fprintf(l, "\nThe user made a search for release date and backward\n");
		CTfl(l);
		fclose(l);
		break;
	case 8:
		printf("Going Back\n");
		fprintf(l, "\nThe user made canceled the search\n");
		CTfl(l);
		fclose(l);
		break;
	}
	while (x != 0)
	{
		deleteFirst(item_list);
		x--;
	}
}

//Function - Compare STR with STR
int strcmp_(const char* s1, const char* s2)
{
	//define pointer
	const unsigned char* p1 = (const unsigned char*)s1;
	const unsigned char* p2 = (const unsigned char*)s2;

	while (*p1)
	{
		if (*p2 > * p1) return 0;//ASCII compare
		if (*p1 > * p2) return 0;//ASCII compare
		p1++;//Move forward
		p2++;//Move forward
	}
	if (*p2) return 0;
	return 1;
}

//Main - security Level 1
int SL1()
{
	int action_select = 0;
	printf("\n------------------------------------------------------------------------------------\n");
	printf("                                   Security Level 1                               ");
	printf("\n------------------------------------------------------------------------------------\n");
	printf("\nChoose Action Below:\n");
	printf("\n1.Search Item Database");
	printf("\n2.Add New Item");
	printf("\n0.Exit\n");
	printf("Enter Action : ");
	scanf("%d", &action_select);
	return(action_select);
}

//Main - secutiry Level 2
int SL2()
{
	int action_select = 0;
	printf("\n------------------------------------------------------------------------------------\n");
	printf("                                   Security Level 2                               ");
	printf("\n------------------------------------------------------------------------------------\n");
	printf("\nChoose Action Below:\n");
	printf("\n1.Search Item Database");
	printf("\n2.Add New Item");
	printf("\n3.Update Item");
	printf("\n4.Delete Item");
	printf("\n0.Exit\n");
	printf("Enter Action : ");
	scanf("%d", &action_select);
	return(action_select);
}

//Main - security Level 3
int SL3()
{
	int action_select = 0;
	printf("\n------------------------------------------------------------------------------------\n");
	printf("                                Administrator permission                             ");
	printf("\n------------------------------------------------------------------------------------\n");
	printf("\nChoose Action Below:\n");
	printf("\n1.Search Item Database");
	printf("\n2.Add New Item");
	printf("\n3.Update Items");
	printf("\n4.Delete Item");
	printf("\n5.Display All Items");
	printf("\n6.Register New Users");
	printf("\n7.Delete User");
	printf("\n8.Display All Users");
	printf("\n0.Exit\n");
	printf("Enter Action : ");
	scanf("%d", &action_select);
	return(action_select);
}

//Main 
void main()
{
	list item_list; //items only
	node new_node; //" "
	initList(&item_list);//" "


	int number_of_error = 0, action_select = 0, flag = 1;
	int day, month, year, hours, mins, SecurityLevel;

	// creating files 
	FILE* log;
	log = fopen("log.txt", "a");
	fclose(log);

	FILE* users;
	users = fopen("users.dat", "rb+");

	user new_user;
	if (users == NULL)
	{
		FILE* users;
		users = fopen("users.dat", "ab+");
		new_user.SecurityLevel = 3;
		strcpy(&new_user.Password, "admin");
		strcpy(&new_user.ID, "admin");
		strcpy(&new_user.FullName, "System_Manager");
		fwrite(&new_user, sizeof(user), 1, users);
		fclose(users);
	}


	FILE* item;
	item = fopen("item.dat", "ab+");
	fclose(item);



	//main program begins
	printf("\n------------------------------------------------------------------------------------\n");
	printf("                               Welocome To Movie DataBase                               ");
	printf("\n------------------------------------------------------------------------------------\n");
	printf("Please LogIn To System:");

	SecurityLevel = LogIn("users.dat", "log.txt");//login to system
	system("cls");

	if (SecurityLevel == 1)
	{
		FILE* l = fopen("log.txt", "a+");
		fprintf(l, "\nA user with security level 1 logged in\n");
		CTfl(l);
		fclose(l);
		while (flag)
		{
			switch (SL1())
			{
			case 0:
				flag = 0;
				break;
			case 1:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Search Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				system("cls");
				search_items(&item_list,"item.dat");
				break;
			case 2:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Add New Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				add_item("item.dat", &item_list);
				break;
			}
		}
	}
	else if (SecurityLevel == 2)
	{
		FILE* l = fopen("log.txt", "a+");
		fprintf(l, "\nA user with security level 2 logged in\n");
		CTfl(l);
		fclose(l);
		while (flag)
		{
			switch (SL2())
			{
			case 0:
				flag = 0;
				break;
			case 1:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Search Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				system("cls");
				search_items(&item_list, "item.dat");
				break;
			case 2:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Add New Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				add_item("item.dat", &item_list); 
				break;
			case 3:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Update Item                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				update_item("item.dat");
				break;
			case 4:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Delete Item                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				delete_item("item.dat");
				break;
			}
		}
	}
	else if (SecurityLevel == 3)
	{
		FILE* l = fopen("log.txt", "a+");
		fprintf(l, "\nA user with security level 3 logged in\n");
		CTfl(l);
		fclose(l);
		while (flag)
		{
			switch (SL3())
			{
			case 0:
				flag = 0;
				break;
			case 1:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Search Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				system("cls");
				search_items(&item_list, "item.dat");
				break;
			case 2:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Add New Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				add_item("item.dat", &item_list); 
				break;
			case 3:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Update Item                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				update_item("item.dat");
				break;
			case 4:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Delete Item                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				delete_item("item.dat");
				break;
			case 5:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                   Display All Items                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				display_all_items("item.dat");
				break;
			case 6:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                 Register New Users                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				register_new_users("users.dat");
				break;
			case 7:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                    Delete User                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				Delete_user("users.dat");
				break;
			case 8:
				printf("\n------------------------------------------------------------------------------------\n");
				printf("                                   Display All Users                             ");
				printf("\n------------------------------------------------------------------------------------\n");
				display_all_users("users.dat");
				break;

			}
		}
	}

	printf("\n\n");
	getDateTime(&day, &month, &year, &hours, &mins);
	printf("\nCurrent computer time: %02d/%02d/%d, %02d:%02d\n", day, month, year, hours, mins);

	system("pause");
}

//system("cls") 

//55 -> Cars