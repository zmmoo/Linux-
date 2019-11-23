#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

#define DATABASE "stu.db"

int do_insert(sqlite3 *db)
{
	int id;
	char name[32];
	int score;
	char sql[128] = {};
	char *errmsg;

	printf("Input id:");
	scanf("%d", &id);
	getchar();

	printf("Input name:");
	scanf("%s", name);
	getchar();

	printf("Input score:");
	scanf("%d", &score);
	getchar();
	sprintf(sql, "insert into stu valus(%d,'%s', %d);", id, name, score);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("Insert done\n");
	}
	return 0;
}
int do_delete(sqlite3 *db)
{
	int id;
	char sql[128] = {};
	char *errmsg;

	printf("Input id:");
	scanf("%d", &id);
	getchar();
	sprintf(sql, "delete from stu where id=%d;", id);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("Delete done\n");
	}
	return 0;
}
int do_update(sqlite3 *db)
{
	int id;
	char sql[128] = {};
	char *errmsg;
	int score;

	printf("Input id:");
	scanf("%d", &id);
	getchar();
	printf("Input score:");
	scanf("%d", &socre);
	getchar();
	
	
	sprintf(sql, "update stu set score = %d where id=%d;",score, id);
	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("Update done\n");
	}
	return 0;
}
int callback(void *para, int f_num, char **f_value, char **f_name)
{
	int i=0;
	for(i=0; i<f_num; i++)
	{
		printf("%s" , f_value[i]);
	}
	putchar(10);
	return 0;
}

int do_query(sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	sprintf(sql, "select * from stu;");
	if(sqlite3_exec(db,sql,callback,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("query success\n");
	}
	return 0;
}
int do_query_1(sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	char ** resultp;
	int nrow;
	int ncloumn;
	int i,j;

	sprintf(sql, "select * from stu;");
	if(sqlite3_get_table(db, sql, &resultp, &nrow, &ncloumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("query success\n");
	}
	itn index = ncloumn;
	for(i=0; i<nrow; i++)
	{
		for(j=0; j<ncloumn; j++)
		{
			printf("%s", resultp[index++]);
		}
		putchar(10);
	}
	
	return 0;
}
int do_quit(sqlite3 *db)
{
	char sql[128] = {};
	char *errmsg;
	sprintf(sql, ".exit");
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("quit success\n");
	}
	return 0;
}


int main(int argc, char **argv[])
{
	sqlite3 *db;
	char * errmsg;
	int cmd;
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
		exit(-1);	
	}
	else
	{
		printf("open DATABASE success.\n");
	}
	//创建表格
	if(sqlite3_exec(db, "ceate table stu(id Integer, name char, score Integer);", NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("ceate table or open success\n");	
	}
	while(1)
	{
		printf("*********************\n");
		printf("1 insert 2 delete 3 query 4 update 5 quit");
		printf("*********************\n");
		scanf("%d", &cmd);
		getchar();
		switch(cmd)
		{
			case 1:
				do_insert(db);
					break;
			case 2:
				do_delete(db);
					break;
			case 3:
				do_query(db);
					break;
			case 4:
				do_update(db);
					break;
			case 5:
				do_quit(db);
				exit(0);
			default:
				printf("input wrong cmd\n");
				break;
		}

	}
	return 0;
}
