#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

void Database_create(struct Connection *conn, int maxRows, int maxData);
	
void Database_close(struct Connection *conn){
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
	Database_close(conn);
	if(errno) {
		perror(message);
	}
	else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Address_print(struct Address *addr, int len){
	printf("%3d %-*s %-*s %5d %5d %-*s\n", addr->id, len + 2, addr->name, len + 2, addr->email, addr->age, addr->rating, len + 2, addr->town);
}

void Database_load(struct Connection *conn){
	int rc = fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	rc = fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	Database_create(conn, conn->db->max_rows, conn->db->max_data);

	if(rc != 1) 
		puts("Failed to load database, or database is empty.");

	for(int i = 0; i < conn->db->max_rows; i++){
		fread((conn->db->rows + i), sizeof(int), 4, conn->file);
		fread((conn->db->rows + i)->name, sizeof(char), conn->db->max_data, conn->file);
		fread((conn->db->rows + i)->email, sizeof(char), conn->db->max_data, conn->file);
		fread((conn->db->rows + i)->town, sizeof(char), conn->db->max_data, conn->file);
	}
}

struct Connection* Database_open(const char *filename, char mode){
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

	if(!conn->file) 
		die("Failed to open the file", NULL);

	return conn;
}

void Database_write(struct Connection *conn)
{
	int rc;
	int index = conn->db->max_rows;

	fseek(conn->file, 0, 0);	
	fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 2, conn->file);

	for(int i = 0; i < index; i++){
		rc = fwrite(&(conn->db->rows + i)->id, sizeof(conn->db->rows->id), 4, conn->file);
		if(rc == 0 ){
			die("Failed to write database.", conn);
		}
		rc = fwrite((conn->db->rows + i)->name, sizeof(*conn->db->rows->name), conn->db->max_data, conn->file);
		rc = fwrite((conn->db->rows + i)->email, sizeof(*conn->db->rows->email), conn->db->max_data, conn->file);
		rc = fwrite((conn->db->rows + i)->town, sizeof(*conn->db->rows->town), conn->db->max_data, conn->file);
	}

	rc = fflush(conn->file);

	if(rc == -1) 
		die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int maxRows, int maxData){
	conn->db->max_rows = maxRows;
	conn->db->max_data = maxData;
	conn->db->rows = (struct Address *)calloc(maxRows, sizeof(*(conn->db->rows)));

	for(int i = 0; i < maxRows; i++) {
		struct Address addr = {.id = i, .set = 0};
		*(conn->db->rows + i) = addr;
		(conn->db->rows + i)->name = (char *)calloc(maxData, sizeof(*conn->db->rows->name));
		(conn->db->rows + i)->email = (char *)calloc(maxData, sizeof(*conn->db->rows->email));
		(conn->db->rows + i)->town = (char *)calloc(maxData, sizeof(*conn->db->rows->town));
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email, const char *age, const char *rating, const char *town){
	struct Address *addr = (conn->db->rows + id);
	
	if(addr->set) 
		die("Already set, delete it first", conn);

	addr->set = 1;
	addr->age = atoi(age);
	addr->rating = atoi(rating);
	
	char *res = strncpy(addr->name, name, conn->db->max_data - 1);
	
	if(!res) 
		die("Name copy failed", conn);

	res = strncpy(addr->email, email, conn->db->max_data - 1);
	
	if(!res) 
		die("Email copy failed", conn);

	res = strncpy(addr->town, town, conn->db->max_data - 1);
	
	if(!res) 
		die("Town copy failed", conn);
}

void Database_get(struct Connection *conn, int id){
	struct Address *addr = &conn->db->rows[id];

	if(addr->set) {
		Address_print(addr, conn->db->max_data);
	} 
	else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id){
	(conn->db->rows + id)->set = 0;
	(conn->db->rows + id)->age = 0;	
	(conn->db->rows + id)->rating = 0;
	strncpy((conn->db->rows + id)->name, "\0", conn->db->max_data - 1);
	strncpy((conn->db->rows + id)->email, "\0", conn->db->max_data - 1);
	strncpy((conn->db->rows + id)->town, "\0", conn->db->max_data - 1);
}

void Database_list(struct Connection *conn){
	int i = 0;

	for(i = 0; i < conn->db->max_rows; i++) {
		struct Address *cur;
		cur = (conn->db->rows + i);
		if(cur->set) {
			Address_print(cur, conn->db->max_data);
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
		die("Incorrect seach field.\
				\n\tn - use to serch by name.\
				\n\te - use to serch by name.", conn);
	}

	for(int i = 0; i < conn->db->max_rows; i++){
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
			Address_print(conn->db->rows + i, conn->db->max_data);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc < 3) 
		die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
	
	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;
	int maxRows = 0x0A, maxData = 0x0A;
	
	if(argc > 3 && action != 'c') 
		id = atoi(argv[3]);

	if(action != 'c' && id >= conn->db->max_rows) 
		die("There's not that many records.", conn);

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

			puts("Database dimensions:");
			printf("\tMax number of entries = %d\n", maxRows);
			printf("\tMax string length     = %d\n", maxData);
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
			char *str = " ";

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
			if(argc != 4) 
				die("Need id to delete", conn);

			Database_delete(conn, id);
			Database_write(conn);
		break;

		case 'l': // list database entries
			Database_list(conn);
		break;
		case 'f': // find database entry based on selected property
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

