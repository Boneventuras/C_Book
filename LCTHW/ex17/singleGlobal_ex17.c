#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 1
#define MAX_ROWS 1

struct Connection {
	FILE *file;
	int max_data;
	int max_rows;
	//int *id;
	int *set;
	int *age;
	int *rating;
	char **name;
	char **email;
	char **town;
};

/*
struct Address {
	int id;
	int set;
	int age;
	int rating;
	char *name;
	char *email;
	char *town;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};
*/

void Database_create(struct Connection *conn, int maxRows, int maxData);
	
void Database_close(struct Connection *conn){
	puts("DB_close");
	if(conn){
		if(conn->file) 
			fclose(conn->file);
			
		free(conn->set);
		free(conn->age);
		free(conn->rating);
// patikrinti ar ka=kaip kitaip nereikia free ** char
		free(conn->name);
		free(conn->email);
		free(conn->town);
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

void Address_print(struct Connection *conn, int id){
	puts("Adr_Print");
	printf("%3d %s %s %5d %5d %s\n", id, conn->name + (id * conn->max_data), \
			conn->email + (id * conn->max_data), conn->age + id, \
			conn->rating + id, conn->town + (id * conn->max_data));
}

void Database_load(struct Connection *conn){

	puts("DB_Load");
	int rc = fread(&conn->max_data, sizeof(conn->max_data), 1, conn->file);
	rc = fread(&conn->max_rows, sizeof(conn->max_rows), 1, conn->file);
	Database_create(conn, conn->max_rows, conn->max_data);

	if(rc != 1) 
		puts("Failed to load database, or database is empty.");

	for(int i = 0; i < conn->max_rows; i++){
		fread((conn->db->rows + i), sizeof(int), 4, conn->file);
		fread((conn->db->rows + i)->name, sizeof(char), conn->max_data, conn->file);
		fread((conn->db->rows + i)->email, sizeof(char), conn->max_data, conn->file);
		fread((conn->db->rows + i)->town, sizeof(char), conn->max_data, conn->file);
	}
}

struct Connection* Database_open(const char *filename, char mode){
	puts("DB_open");
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) 
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

	if(!conn->file) 
		die("Failed to open the file", NULL);

	return conn;
}

void Database_write(struct Connection *conn)
{
	int rc;
	int index = conn->max_rows;

	puts("DB_write");

	fseek(conn->file, 0, 0);	
	fwrite(&conn->max_data, sizeof(conn->max_data), 2, conn->file);

	// add write from 2D arr
	//
	for(int i = 0; i < index; i++){
		rc = fwrite(&(conn->db->rows + i)->id, sizeof(conn->db->rows->id), 4, conn->file);
		if(rc == 0 ){
			die("Failed to write database.", conn);
		}
		printf("(conn->db->rows + i)->name is\t%X\n", &(conn->db->rows + i)->name);
		rc = fwrite((conn->db->rows + i)->name, sizeof(*conn->db->rows->name), conn->max_data, conn->file);
		rc = fwrite((conn->db->rows + i)->email, sizeof(*conn->db->rows->email), conn->max_data, conn->file);
		printf("town address is\t\t\t%X\n", (conn->db->rows + i)->town);
		rc = fwrite((conn->db->rows + i)->town, sizeof(*conn->db->rows->town), conn->max_data, conn->file);
	}

	rc = fflush(conn->file);

	if(rc == -1) 
		die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int maxRows, int maxData){
	puts("DB_create");
	conn->max_rows = maxRows;
	conn->max_data = maxData;
	conn->set = (int*)calloc(maxRows, sizeof(*(conn->set)));
	conn->age = (int*)calloc(maxRows, sizeof(*(conn->age)));
	conn->rating = (int*)calloc(maxRows, sizeof(*(conn->rating)));
// kaip alloc 2D array
	conn->name = (char**)calloc(maxRows, sizeof(*(conn->name)));
	conn->email = (char**)calloc(maxRows, sizeof(*(conn->email)));
	conn->town = (char**)calloc(maxRows, sizeof(*(conn->town)));

	for(int i = 0; i < conn->max_rows; i++){
		*(conn->name + i) = (char*)calloc(conn->max_data + 1, sizeof(char));

		*(conn-> email + i) = (char*)calloc(conn->max_data + 1, sizeof(char));

		*(conn->town + i) = (char*)calloc(conn->max_data + 1, sizeof(char));
	}
	for(int i = 0; i < maxRows; i++) {
		// make a prototype to initialize it
		struct Address addr = {.id = i, .set = 0};
		// then just assign it
		*(conn->db->rows + i) = addr;
		(conn->db->rows + i)->name = (char *)calloc(maxData, sizeof(*conn->db->rows->name));
		(conn->db->rows + i)->email = (char *)calloc(maxData, sizeof(*conn->db->rows->email));
		(conn->db->rows + i)->town = (char *)calloc(maxData, sizeof(*conn->db->rows->town));
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email, const char *age, const char *rating, const char *town){
	puts("DB_set");
	struct Address *addr = (conn->db->rows + id);
	
	if(addr->set) 
		die("Already set, delete it first", conn);

	addr->set = 1;
	addr->age = atoi(age);
	addr->rating = atoi(rating);
	
	printf("*name\t%s\n*email\t%s\n", name, email);
	printf("&addr->name\t%08X\naddr->name\t%c\n", &addr->name, addr->name);
	printf("\n&addr->email\t%08X\naddr->email\t%c\n", &addr->email, addr->email);

	char *res = strncpy(addr->name, name, conn->max_data - 1);
	
	if(!res) 
		die("Name copy failed", conn);

	res = strncpy(addr->email, email, conn->max_data - 1);
	
	if(!res) 
		die("Email copy failed", conn);

	res = strncpy(addr->town, town, conn->max_data - 1);
	
	if(!res) 
		die("Town copy failed", conn);

	puts("Exit DB_set");
}

void Database_get(struct Connection *conn, int id){
	puts("DB_get");
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(conn, id);
	} 
	else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id){
	puts("DB_delete");
	
	(conn->db->rows + id)->set = 0;
	(conn->db->rows + id)->age = 0;	
	(conn->db->rows + id)->rating = 0;
	strncpy((conn->db->rows + id)->name, "\0", conn->max_data - 1);
	strncpy((conn->db->rows + id)->email, "\0", conn->max_data - 1);
	strncpy((conn->db->rows + id)->town, "\0", conn->max_data - 1);
}

void Database_list(struct Connection *conn){
	int i = 0;
	//struct Database *db = conn->db;
	puts("DB_list");

	for(i = 0; i < conn->max_rows; i++) {
		struct Address *cur;
		cur = (conn->db->rows + i);
		if(cur->set) {
			Address_print(conn, i);
		}
	}
}

int search_string (char *str, char *find_str){
	int index;	

	for(int j = 0; *(str + j) != '\0'; j++){
		for(int i = 0; *(find_str + i) != '\0'; i++){
	       		if(*(str + j + i) == *(find_str + i))
				index = j;
			else{
				index = -1;
				continue;
			}
		}
		if(index >= 0)
			return index;
	}

	return -1;
}

int find_num (int num, int limit, char equation){

	switch(equation){
		case 'e':
			if(num == limit)
				return 0;
			break;
		case 'l':
			if(limit > num)
				return 0;
			break;
		case 'g':
			if(limit < num)
				return 0;
			break;
		default:
			printf("Incorrect or no condition is provided, use:\n\
					\t%ce for =\n\t%cl for >\n\t%cg for <\n",\
					equation, equation, equation);
			return -2;
	}
	return -1;
}

void Datbase_find(struct Connection *conn, char *arg_str, char *find_str){
	int result;
	char find_by = arg_str[0];
	char range = arg_str[1];

	if(find_by != 'n' && find_by != 'e' && find_by != 'a' && find_by != 'r' && find_by != 't'){
		printf("find_by is %c, find_str is %s\n", find_by, find_str);
		die("Incorrect seach field.\
				\n\tn - use to serch by name.\
				\n\te - use to serch by name.", conn);
	}

	for(int i = 0; i < conn->max_rows; i++){
		switch (find_by){
			case 'n':
				result = search_string((conn->db->rows + i)->name, find_str);
			break;
			case 'e':
				result = search_string((conn->db->rows + i)->email, find_str);
			break;
			case 'a':
				result = find_num((conn->db->rows + i)->age, atoi(find_str), range);
			break;
			case 'r':
				result = find_num((conn->db->rows + i)->rating, atoi(find_str), range);
			break;
			case 't':
				result = search_string((conn->db->rows + i)->town, find_str);
			break;
		}
		
		if(result >= 0){
			puts("Entry found:");
			Address_print(conn, i);
		}
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
	if(action != 'c' && id >= conn->max_rows) 
		die("There's not that many records.", conn);

	puts("Main before switch");
	switch(action) {
		case 'c': // create database
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

		case 'g': // get database entry by ID
			if(argc != 4) 
				die("Need an id to get", conn);

			Database_get(conn, id);
		break;
	 
		case 's': // save entry to database
		// id, name, email required, age, rating town option
		// usage: progname DB_file s id name email *age *rating *town
			puts("Main:\tIn case 's'");
			char *str = "0";

			if(argc < 6) 
				die("At least need id, name, email to set, optional age, rating, town", conn);
			switch(argc){
				case 6:
					Database_set(conn, id, argv[4], argv[5], str, str, str);
				break;
				case 7:
					Database_set(conn, id, argv[4], argv[5], argv[6], str, str);
				break;
				case 8:
					Database_set(conn, id, argv[4], argv[5], argv[6], argv[7], str);
				break;
				case 9:
					Database_set(conn, id, argv[4], argv[5], argv[6], argv[7], argv[8]);
				break;
				default:
					die("To much args", conn);
			}
			Database_write(conn);
		break;
		
		case 'd': // delete entry from database 4 4
			puts("Main:\tIn case 'd'");
			if(argc != 4) 
				die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
		break;

		case 'l': // list database entries
			puts("Main:\tIn list, start create");
			
			Database_list(conn);
		break;
		case 'f': // find database entry based on selected property
		// 
			puts("Main:\tfind");
			if(argc != 5) 
				die("Need more arguments to search.\
						\n\tf n *** - use to serch by name (*** replace with string).\
						\n\tf e *** - use to serch by name (*** replace with string).\
						\n\tf a* *** - use to search by age, (a* replace with ae for =, al for <, ag for >, *** with string\
						\n\tf r* *** - use to search by age, (r* replace with re for =, rl for <, rg for >, *** with string\
						\n\tf t *** - use to serch by name (*** replace with string).", conn);
			Datbase_find(conn, argv[3], argv[4]);
		break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
	}
	
	Database_close(conn);

	return 0;
}

