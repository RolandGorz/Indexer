#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
 
typedef struct files {
	char *filename; //make this into an array of "strings" right now it only holds one filename
	int count;
	struct files* next;
}files;

typedef struct Node {
	char* word;
	files* location;
	struct Node* next;
}Node;

files *list_switch( files *l1, files *l2 )
{
    l1->next = l2->next;
    l2->next = l1;
    return l2;
}

files *sort( files *start )
{
    files *p, *q, *top;
    int changed = 1;

    top = malloc(sizeof(files));

    top->next = start;
    if( start != NULL && start->next != NULL ) {
       
        while( changed ) {
            changed = 0;
            q = top;
            p = top->next;
            while( p->next != NULL ) {
                /* push smaller items down */
                if( p->count < p->next->count ) {
                    q->next = list_switch( p, p->next );
                    changed = 1;
                }
                q = p;
                if( p->next != NULL )
                    p = p->next;
            }
        }
    }
    p = top->next;
    free( top );
    return p;
}

void putStuff(Node** hash, char* origin, FILE *file) {
	char *token = malloc(30 * sizeof(char));
	int maxtemp = 30;
	char temp;
	int n = 0;
	while ((temp = fgetc(file)) != EOF) { //it works cool.
		if (n == maxtemp) {
			token = realloc(token, strlen(token) * 2 * sizeof(char));
			maxtemp = strlen(token) * 2 * sizeof(char);
		}
		if (!(isalpha(temp)) && !(isdigit(temp))) { //check for delimiter
			token[n] = '\0';
			if (n != 0) {// putting in structure
				if (isalpha(token[0])) {
					if (hash[token[0]-87]->word == NULL) { // this only works for the first time putting it into the array
						hash[token[0]-87]->word = malloc(n * sizeof(char) + sizeof(char));
						strcpy(hash[token[0]-87]->word, token);
						hash[token[0]-87]->location = (files*)malloc(sizeof(files));
						hash[token[0]-87]->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
						strcpy(hash[token[0]-87]->location->filename, origin);
						hash[token[0]-87]->location->count = 1;
						hash[token[0]-87]->location->next = NULL;
						hash[token[0]-87]->next = NULL;
						n=0;
						continue;
					}
					Node *behind = hash[token[0]-87];
					Node *current = hash[token[0]-87];
					files *checkfilename;
					//PUTTING IN THE STRUCTURE
					while (current != NULL) { //do stuff for greater than and have it iterate through the list
						if (strcmp(token, current->word) == 0) {
							checkfilename = current->location;
							while (checkfilename != NULL) {
								if (strcmp(checkfilename->filename, origin) == 0) {
									checkfilename->count++;
									n=0;
									break;
								}
								if (checkfilename->next == NULL) {
									files *newfile = malloc(sizeof(files));
									newfile->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
									strcpy(newfile->filename, origin);
									newfile->count = 1;
									newfile->next = NULL;
									checkfilename->next = newfile;
									n = 0;
									break;
								}
								checkfilename = checkfilename->next;
							}
							break;							
						}
						else if (strcmp(token, current->word) < 0) {//when the word comes before the current word
							if (current == behind) { //when it's the front of the list
								Node *new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								new->next = current;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
								strcpy(new->location->filename, origin);
								new->location->count = 1;
								new->location->next = NULL;
								hash[token[0]-87] = new; //move the head pointer back to the front
								n = 0;
								break;
							} else {
								Node *new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								new->next = current;
								behind->next = new;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
								strcpy(new->location->filename , origin);
								new->location->count = 1;
								new->location->next = NULL;
								n = 0;
							break;
							}
						}
						else if (strcmp(token, current->word) > 0) {
							if (current->next == NULL) {
								Node*new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								current->next = new;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));	
								strcpy(new->location->filename, origin);
								new->location->count = 1;
								new->location->next = NULL;
								new->next = NULL;
								n = 0;
								break;
							}
							behind = current;
							current = current->next;
						}
					}
				} else {
					int a = token[0] - 48;
					if (hash[a]->word == NULL) { // this only works for the first time putting it into the array
						hash[a]->word = malloc(n * sizeof(char) + sizeof(char));
						strcpy(hash[a]->word, token);
						hash[a]->location = (files*)malloc(sizeof(files));
						hash[a]->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
						strcpy(hash[a]->location->filename, origin);
						hash[a]->location->count = 1;
						hash[a]->location->next = NULL;
						hash[a]->next = NULL;
						n=0;
						continue;
					}
					Node *behind = hash[a];
					Node *current = hash[a];
					files *checkfilename;
					//PUTTING IN THE STRUCTURE
					while (current != NULL) { //do stuff for greater than and have it iterate through the list
						if (strcmp(token, current->word) == 0) {
							checkfilename = current->location;
							while (checkfilename != NULL) {
								if (strcmp(checkfilename->filename, origin) == 0) {
									checkfilename->count++;
									n=0;
									break;
								}
								if (checkfilename->next == NULL) {
									files *newfile = malloc(sizeof(files));
									newfile->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
									strcpy(newfile->filename,origin);
									newfile->count = 1;
									newfile->next = NULL;
									checkfilename->next = newfile;
									n = 0;
									break;
								}
								checkfilename = checkfilename->next;
							}
							break;							
						}
						else if (strcmp(token, current->word) < 0) {//when the word comes before the current word
							if (current == behind) { //when it's the front of the list
								Node *new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								new->next = current;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
								strcpy(new->location->filename, origin);
								new->location->count = 1;
								new->location->next = NULL;
								hash[a] = new; //move the head pointer back to the front
								n = 0;
								break;
							} else {
								Node *new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								new->next = current;
								behind->next = new;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));
								strcpy(new->location->filename, origin);
								new->location->count = 1;
								new->location->next = NULL;
								n = 0;
								break;
							}
						}
						else if (strcmp(token, current->word) > 0) {
							if (current->next == NULL) {
								Node*new = malloc(sizeof(Node));
								new->word = malloc(n * sizeof(char) + sizeof(char));
								strcpy(new->word, token);
								current->next = new;
								new->location = (files*)malloc(sizeof(files));
								new->location->filename = malloc(strlen(origin) * sizeof(char) + sizeof(char));	
								strcpy(new->location->filename, origin);
								new->location->count = 1;
								new->location->next = NULL;
								new->next = NULL;
								n = 0;
								break;
							}
							behind = current;
							current = current->next;
						}
					}
				}
			}
			n = 0;
			continue;
		}
		if (isalpha(temp)) {
			temp = tolower(temp);
		}
		token[n] = temp;
		n++;
	}
	free(token);//free the token holder
}

void PrintStuff (Node** hash, char* wfile) {
	FILE *fp = fopen(wfile, "w+");
	int walkhash = 0;
	Node *go;
	files *filego;
	int linemax;
	while (walkhash < 37) {
		if (hash[walkhash]->word == NULL) {
			walkhash++;
			continue;
		}
		go = hash[walkhash];
		while (go != NULL) {
			if (go->word != NULL) {
				fputs("<list> ", fp);
				fputs(go->word, fp);
				fputs("\n\n", fp);
				filego = go->location;
				linemax = 0;
				while (filego != NULL) {
					fputs(filego->filename, fp);
					fputs(" ", fp);
					fprintf(fp, "%d", filego->count);
					fputs(" ", fp);
					filego = filego->next;
					linemax++;
					if (linemax == 5 && filego != NULL) {
						fputs("\n", fp);
						linemax = 0;
					}
				}
				fputs("\n\n", fp);
				fputs("</list>\n", fp);
				fputs("\n", fp);
			}
			go = go->next;
		}
		walkhash++;
	}
	fclose(fp);
}

void DirectoryTravel(DIR *direc, char *pathname, Node** hash) {
	char *path = "/\0";
	char* temp = malloc(strlen(pathname) * sizeof(char) + 2 * sizeof(char));
	strcpy(temp, pathname);
	strcat(temp, path);
	FILE *file;
	struct dirent *read;
	struct stat statbuf;
	while ((read = readdir(direc)) != NULL) {
		lstat(read->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			if (strcmp(".", read->d_name) == 0 || strcmp("..", read->d_name) == 0) {
				continue;
			}
			DIR *cont = opendir(read->d_name);
			char *copy = malloc(strlen(temp) * sizeof(char) + sizeof(char));
			strcpy(copy, temp);
			temp = realloc(temp, strlen(temp) * sizeof(char) + strlen(read->d_name) * sizeof(char) + sizeof(char));
			strcat(temp, read->d_name);
			chdir(read->d_name);
			DirectoryTravel(cont, temp, hash);
			chdir("..");
			strcpy(temp, copy);
			closedir(cont);
			free(copy);
		} else if(stat(read->d_name, &statbuf)) {
			continue;
		} else if(!statbuf.st_size) {
			continue;
		}else{
			file = fopen(read->d_name, "r");
			char* comeback = malloc(strlen(temp) * sizeof(char) + sizeof(char));
			strcpy(comeback, temp);
			temp = realloc(temp, strlen(temp) * sizeof(char) + strlen(read->d_name) * sizeof(char) + sizeof(char));
			strcat(temp, read->d_name);
			putStuff(hash, temp, file);
			strcpy(temp, comeback);
			free(comeback);
			fclose(file);
		}
	}
	free(temp);
}

int main(int argc, char **argv) { //argv[1] has the write IF IT ALREADY EXISTS ASK IF ITS OK TO OVERWRITE to file argv[2] has the read file or directory
	if (argc != 3) {
		printf("ERROR -- Not enough arguments\n");
		return 0;
	}
	FILE *file;
	struct stat statbuf;
	DIR *open;//Everything works now just make the directory come in and then recursively go through it
	Node* hash[37];
	int n = 0;
	char a[2];
	while (n < 37) { //make the hash table
		hash[n] = malloc(sizeof(Node));
		hash[n]->word = NULL;
		hash[n]->location = NULL;
		hash[n]->next = NULL;
		n++;
	}
	if ((open = opendir(argv[2]))) {
		chdir(argv[2]);
		DirectoryTravel(open, argv[2], hash);
		chdir("..");
		closedir(open);
	} else if (!stat(argv[2], &statbuf)) {
		file = fopen(argv[2], "r");
		putStuff(hash, argv[2], file);
		fclose(file);
	} else {
		printf("ERROR -- File/Directory cannot be found\n");
		return 0;
	}
	n = 0;
	Node* yes;
	while (n<37) {
		yes = hash[n];
		while (yes != NULL) {
			yes->location = sort(yes->location);
			yes = yes->next;
		}
		n++;	
	}
	if(!stat(argv[1], &statbuf)) {
		printf("Would you like to overwrite this file? y/n\n");
check:
		scanf("%s", a);
		if (strcmp(a, "y") == 0) {
			printf("Output written into %s\n", argv[1]);
			PrintStuff(hash, argv[1]);
			goto free;
		}
		if (strcmp(a, "n") == 0) {
			goto free;
		} else {
			printf("Incorrect Input\n");
			goto check;
		}
	} else {
		printf("Output written into %s\n", argv[1]);
		PrintStuff(hash, argv[1]);
	}
free:
	//Free the structure
	n = 0;
	files *rid;
	files *ridtemp;
	Node *nodetemp;
	while (n<37) {		
		if (hash[n]->word == NULL) {
			free(hash[n]);
			n++;
			continue;
		}
		while (hash[n] != NULL) {
			free(hash[n]->word);
			rid = hash[n]->location;
			while (rid != NULL) {
				free(rid->filename);
				ridtemp = rid;
				rid = rid->next;
				free(ridtemp);
			}
			nodetemp = hash[n];
			hash[n] = hash[n]->next;
			free(nodetemp);
		}
		n++;
	}
	return 0;
}
