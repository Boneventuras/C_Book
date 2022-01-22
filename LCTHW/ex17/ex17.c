#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 1
#define MAX_ROWS 1

/*
struct Address {
	int id;
	int set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};
*/

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

/*
struct Database {
	struct Address rows[MAX_ROWS];
};
*/

/*
struct Database {
	int max_data;
	int max_rows;
	struct Address rows[MAX_ROWS];
};
*/

struct Database {
	int max_data;
	int max_rows;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_create(struct Connection *conn, int maxRows, int maxData);
	
void Database_close(struct Connection *conn){
	puts("DB_close");
	if(conn){
		if(conn->file) 
			fclose(conn->file);
		if(conn->db){
			for(int i = 0; i < conn->db->max_rows; i++){
				free((conn->db->rows + i)->email);
				free((conn->db->rows + i)->name);
			}
			free(conn->db->rows);
			free(conn->db);
		}
	free(conn);
	}
}

void die(const char *message, struct Connection *conn){
	puts("Die");
	Database_close(conn);
	if(errno) {
		perror(message);
	}
	else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Address_print(struct Address *addr){
	puts("Adr_Print");
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn){

	puts("DB_Load");
	int rc = fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	//puts("after 1 malloc");
	rc = fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	//puts("after 2 malloc");
	Database_create(conn, conn->db->max_rows, conn->db->max_data);
	//printf("max_data %d\n", conn->db->max_data);

	if(rc != 1) 
		puts("Failed to load database, or database is empty.");
//		die("Failed to load database.", conn);

	// Increment rows by I 
	for(int i = 0; i < conn->db->max_rows; i++){
		//puts("for");
		//printf("Name %lu\n", (conn->db->rows + i)->name);
		fread((conn->db->rows + i), sizeof(int), 2, conn->file);
		//puts("after rows");
		fread((conn->db->rows + i)->name, sizeof(char), conn->db->max_data, conn->file);
		//puts("after name");
		//printf("name is %s\n", (conn->db->rows + i)->name);
		fread((conn->db->rows + i)->email, sizeof(char), conn->db->max_data, conn->file);
		//puts("after email");
	}
}

struct Connection* Database_open(const char *filename, char mode){
	puts("DB_open");
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) 
		die("Memory error", NULL);
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db) 
		die("Memory error", NULL);

	if(mode == 'c'){
		conn->file = fopen(filename, "w");
	}
	else{
		conn->file = fopen(filename, "r+");
		if(conn->file) {
			Database_load(conn);
		}
	}

	if(!conn->file) die("Failed to open the file", NULL);

	return conn;
}

void Database_write(struct Connection *conn)
{
	int rc;
	int index = conn->db->max_rows;

	puts("DB_write");
	//rewind(conn->file);
	//int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	//sizetowrite = 

	fseek(conn->file, 0, 0);	
	fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 2, conn->file);

	for(int i = 0; i < index; i++){
		rc = fwrite(&(conn->db->rows + i)->id, sizeof(conn->db->rows->id), 2, conn->file);
		if(rc == 0 ){
			die("Failed to write database.", conn);
		}
		printf("(conn->db->rows + i)->name is\t%u\n", &(conn->db->rows + i)->name);
		rc = fwrite((conn->db->rows + i)->name, sizeof(*conn->db->rows->name), conn->db->max_data, conn->file);
		rc = fwrite((conn->db->rows + i)->email, sizeof(*conn->db->rows->email), conn->db->max_data, conn->file);
	}

	rc = fflush(conn->file);

	if(rc == -1) 
		die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int maxRows, int maxData){
	puts("DB_create");
	conn->db->max_rows = maxRows;
	conn->db->max_data = maxData;
	conn->db->rows = (struct Address *)calloc(maxRows, sizeof(*(conn->db->rows)));

	for(int i = 0; i < maxRows; i++) {
		// make a prototype to initialize it
		struct Address addr = {.id = i, .set = 0};
		// then just assign it
		*(conn->db->rows + i) = addr;
		(conn->db->rows + i)->name = (char *)calloc(maxData, sizeof(*conn->db->rows->name));
		(conn->db->rows + i)->email = (char *)calloc(maxData, sizeof(*conn->db->rows->email));
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email){
	puts("DB_set");
	struct Address *addr = (conn->db->rows + id);
	
	if(addr->set) 
		die("Already set, delete it first", conn);

	addr->set = 1;
	
	printf("*name\t%s\n*email\t%s\n", name, email);
	printf("&addr->name\t%08X\naddr->name\t%c\n", &addr->name, addr->name);
	printf("\n&addr->email\t%08X\naddr->email\t%c\n", &addr->email, addr->email);

	// WARNING: bug, read the "How To Break It" and fix this
	char *res = strncpy(addr->name, name, conn->db->max_data - 1);
	// demonstrate the strncpy bug
	if(!res) die("Name copy failed", conn);

	res = strncpy(addr->email, email, conn->db->max_data - 1);
	if(!res) 
		die("Email copy failed", conn);

	puts("Exit DB_set");
}

void Database_get(struct Connection *conn, int id){
	puts("DB_get");
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr);
	} 
	else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id){
	puts("DB_delete");
	
	(conn->db->rows + id)->set = 0;
	strncpy((conn->db->rows + id)->name, "\0", conn->db->max_data - 1);
	strncpy((conn->db->rows + id)->email, "\0", conn->db->max_data - 1);
}

void Database_list(struct Connection *conn){
	int i = 0;
	//struct Database *db = conn->db;
	puts("DB_list");
	//printf("Stuff in conn\nmaxdata %d\n", conn->db->max_data);
	//printf("Row address is %lu\n",(conn->db->rows + i));

	for(i = 0; i < conn->db->max_rows; i++) {
		//printf("For, iteration %d\n", i);
		struct Address *cur;
		cur = (conn->db->rows + i);
		//printf("Address saved in cur is %lu\n", *cur);
		if(cur->set) {
			Address_print(cur);
		}
	}
}

void Datbase_find(struct Connection *conn, char find_by, char *find_str){
	switch (find_by){
		case 'n':
			
		break;
		case 'e':

		break;
		default:
			printf("find_by is %c, find_str is %s\n", find_by, find_str);
			die("Incorrect seach field.\
					\n\tn - use to serch by name.\
					\n\te - use to serch by name.", conn);




	}

	for(int i = 0; i < conn->db->max_rows; i++){
		

	}


}

int main(int argc, char *argv[]){
	if(argc < 3) 
		die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
	
	puts("Main Running");
	
	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	puts("Main After open");
	int id = 0;
	int maxRows = 0x0A, maxData = 0x0A;
	
	puts("Main before id num get");
	if(argc > 3 && action != 'c') 
		id = atoi(argv[3]);

	puts("Main before MAX_ROWS check");
	if(action != 'c' && id >= conn->db->max_rows) 
		die("There's not that many records.", conn);

	puts("Main before switch");
	switch(action) {
		case 'c':
			if(argc < 5){
				puts("Need MAX_ROW and MAX_DATA to create database.");
				puts("MAX_ROW and MAX_DATA is set to default: 10 and 10.");
			}
			else if(argc > 5){
				puts("To many arguments for mode c.");
				puts("MAX_ROW and MAX_DATA is set to default: 10 and 10.");
			}
			else{
				maxRows = atoi(argv[3]);
				maxData = atoi(argv[4]) + 1;
			}
			// add stuff so it works
			printf("maxRows = %d\n", maxRows);
			printf("maxData = %d\n", maxData);
			Database_create(conn, maxRows, maxData);
			Database_write(conn);
		break;

		case 'g':
			if(argc != 4) 
				die("Need an id to get", conn);

			Database_get(conn, id);
		break;

		case 's':
			puts("Main:\tIn case 's'");
			if(argc != 6) 
				die("Need id, name, email to set", conn);
			
			puts("Main:\tIn case 's':\tafter die");
			printf("argv[4] is %s, argv[5] is %s\n", argv[4], argv[5]);
			Database_set(conn, id, argv[4], argv[5]);
			puts("Main:\tIn case 's':\tAfter set");
			Database_write(conn);
			puts("Main:\tIn case 's':\tAfter write");
		break;
		
		case 'd':
			puts("Main:\tIn case 'd'");
			if(argc != 4) 
				die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
		break;

		case 'l':
			puts("Main:\tIn list, start create");
			
			Database_list(conn);
		break;
		case 'f':
			// add find here
			if(argc != 5) 
				die("Need more arguments to search.\
						\n\tf n *** - use to serch by name (* replace with string).\
						\n\tf e *** - use to serch by name (* replace with string).", conn);
			Datbase_find(conn, *argv[3], argv[4]);
		break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}
	
	/*
	printf("struct Address is %d\n",(int)sizeof(struct Address));
	printf("struct Database is %d\n",(int)sizeof(struct Database));
	printf("struct Connection is %d\n",(int)sizeof(struct Connection));
	printf("conn is %d\n",(int)sizeof(*conn));
	printf("FILE is %d\n",(int)sizeof(*conn->file));
	printf("db is %d\n",(int)sizeof(*conn->db));
	printf("rows is %d\n",(int)sizeof(*conn->db->rows));
	printf("name is %d\n",(int)sizeof(*conn->db->rows->name));
	printf("email is %d\n",(int)sizeof(*conn->db->rows->email));
	printf("conn->db->rows->name\naddress %lu\nvalue %s\n", conn->db->rows->name, conn->db->rows->name);
	*/

	Database_close(conn);

	return 0;
}

