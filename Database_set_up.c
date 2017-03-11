// File name of records: sbabackup.txt

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <mysql/mysql.h>

MYSQL *conn;

char address[200];	// the address of the record file
char host[30];
char user[30];
char pass[30];
char db[30];			// name of database
char sql[300];

int open_db() {
	int skip;
	char choice;

	printf("Please enter the MySQL host name: ");
	scanf("%s", &host);
	printf("Please enter the MySQL username: ");
	scanf("%s", &user);
	printf("Please enter the MySQL password: (Enter 'null' if no password) ");
	scanf("%s", &pass);

	conn = mysql_init(0);
	if( !conn ) {
		printf("Fail to connect.\n");
		printf("%s\n", mysql_error(conn));
		exit(2);
	}

	if ( strcmp(pass, "null") == 0 || strcmp(pass, "NULL") == 0) {
		sprintf(pass, "");
	}
	
	printf("Please enter the name of database: ");
	scanf("%s", &db);
	
	if ( mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0))
		printf("Connection established\n");
	else {
		printf("Fail to establish connection: %s\n", mysql_error(conn));
		if (strcmp(mysql_error(conn), "Unknown database") > 0) {
			do {
				printf("Create one now? [Y/N]");
				skip = 0;
				scanf("%s", &choice);
				if (choice == 'n' || choice == 'N') {
					printf("Exiting...");
					exit(3);
				}
				else if (choice != 'y' && choice != 'Y') {
					skip = 1;
					printf("Unknown response.\n");
				}
			} while (skip == 1);
			mysql_real_connect(conn, host, user, pass, NULL, 0, NULL, 0);
			sprintf(sql, "CREATE DATABASE %s;", db);
			if (!mysql_query(conn, sql)) {
	      	printf( "Database created successfully.\n");
	      	mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0);
	   	}
   		else {
				printf("==ERROR==: %s\n", mysql_error(conn));
			}
		}
	}
}

int close_db() {
	mysql_close(conn);
}

int create_table() {
	char choice;
	int skip;
	
   sprintf(sql, "CREATE TABLE record ( a1 int(1), a2 int(1), a3 int(1), \
													a4 int(1), a5 int(1), a6 int(1), \
													a7 int(1), a8 int(1), a9 int(1), \
													wingame int(1) );");
   if (!mysql_query(conn, sql))
   	printf("Table created.\n");
}

int import_file() {
	char command[300] = "LOAD DATA infile '";
  	char command2[150] = "INTO TABLE record\
  								fields terminated by ',' \
  								optionally enclosed by \'\"\' \
  								lines terminated by '\\r\\n';";
  	strcat(command, address);
  	strcat(command, "' ");
  	strcat(command, command2);
  	
	sprintf(sql, command);
	if (!mysql_query(conn, sql))
		printf("Record loaded successfully.\n");
	else {
		printf("==ERROR==: %s\n", mysql_error(conn));
		exit(3);
	}
}

int get_record_address() {
	int i = 0, j = 0, a = 0;
	char ch[200];
	char temp[200] = "";
	FILE *fd = popen("where sbabackup.txt", "r");
	fgets(ch, 200, fd);
	
	while (ch[a] != '\n') {
		a++;
	}
	strncat(temp, ch, a);
	
	while (temp[i] != '\0') {
		if (temp[i] == '\\') {
			for (a = 0; a < 3; a++) {
				address[a+j] = '\\';
			}
			j += 3;
		}
		address[j] = temp[i];
		i++;
		j++;
	}
	pclose(fd);
}

int edit_table() {
	sprintf(sql, "CREATE TABLE temp LIKE record; ");
	mysql_query(conn, sql); printf("1 ");
	sprintf(sql, "INSERT INTO temp SELECT DISTINCT * FROM record "); 
	strcat(sql, "ORDER BY wingame,a1,a2,a3,a4,a5,a6,a7,a8,a9; ");
	mysql_query(conn, sql); printf("2 ");
	sprintf(sql, "DELETE FROM record; ");
	mysql_query(conn, sql); printf("3 ");
	sprintf(sql, "INSERT INTO record SELECT * FROM temp; ");
	mysql_query(conn, sql); printf("4 ");
	sprintf(sql, "DROP TABLE temp;");
	mysql_query(conn, sql); printf("5 ");
	
	printf("Record rearranged.\n");
}

int backup_records() {
	int i = 0, j = 0, a = 0;
	char ch[200];
	char temp[300] = "";
	char command[600] = "";
	char homepath[500] = "";

	FILE *fd = popen("echo %cd%", "r");
	fgets(ch, 200, fd);
	
	while (ch[a] != '\n') {
		a++;
	}
	strncat(temp, ch, a);
	
	while (temp[i] != '\0') {
		if (temp[i] == '\\') {
			for (a = 0; a < 3; a++) {
				homepath[a+j] = '\\';
			}
			j += 3;
		}
		homepath[j] = temp[i];
		i++;
		j++;
	}
	strcat (command, "select * from record into outfile '");
	strcat (command, homepath);
	strcat (command, "\\\\sbabackup.txt' ");
	strcat (command, "fields terminated by ',' \
  								optionally enclosed by '\"' \
  								lines terminated by '\\r\\n';");
	pclose(fd);
	system("del sbabackup.txt");
	mysql_query(conn, command);
	printf("\nThe backup file has been created at\n%s\\sbabackup.txt\n", temp);
}

int menu() {
	int action;
	int i;
	printf("\n=====================Menu=====================\n\n");
	printf("	1, Create database and import records\n");
	printf("	2, Rearrange the table in ascending order\n");
	printf("	3, Backup the records\n");
	printf("	4, Exit\n");
	printf("\n\n");
	printf("Please select action (number): ");
	scanf("%d", &action);
	if (action > 0 && action < 4)
		return action;
	return 0;
}

main() {
	switch (menu()) {
		case 1:
			open_db();
			get_record_address();
			create_table();
			import_file();
			close_db();
			break;
		case 2:
			open_db();
			edit_table();
			close_db();
			break;
		case 3:
			open_db();
			edit_table();
			backup_records();
			close_db();
			break;
		case 4:
			exit(1);
		default:
			printf("\nInvalid action...\n");
	}
}
