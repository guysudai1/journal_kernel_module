#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0

int read_journal(int, char*);
void write_journal(int, char*);
char* draw_menu();
int open_journal();
void release_journal(int, char*);

int main(int argc, char* argv) {
	int journal_fd, choice;
	char* journal;
       	char* input;
	input   = (char*) calloc(1500, 1);
	journal = (char*) calloc(1500, 1);
	journal_fd = open_journal();

	// Main loop
	do {
		printf("%s", draw_menu());
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				if (read_journal(journal_fd, journal) == SUCCESS)
					printf("Journal: %s\n", journal);
				break;
			case 2:
				printf("Input: ");
				read(0, input, 1500);
				input[strlen(input) - 1] = '\0';
				printf("%s\n", input);
				write_journal(journal_fd, input);
				break;
			case 3:
				break;
			default:
				printf("Invalid number");
				break;
		}
	} while (choice != 3);

	// Release journal
	printf("Closing journal file...\n");
	release_journal(journal_fd, journal);
	exit(1);
}

void release_journal(int journal_fd, char* journal) {
	if (close(journal_fd) == -1) {
		printf("Error closing /dev/journal: %s", strerror(errno));
	}
	free(journal);
}

int read_journal(int journal_fd, char* buf) {
	if (read(journal_fd, buf, 1500) == -1) {
		printf("Error reading journal: %s", strerror(errno));
		return FAILURE;
	}
	return SUCCESS;
}

void write_journal(int journal_fd, char* input) {
	if (write(journal_fd, input, strlen(input)) == -1) 
		printf("Error writing into journal %s", strerror(errno));
	else
		printf("Wrote into journal successfully!\n");
}

char* draw_menu() {
	return "======MENU======\n"
	       "1. Read Journal \n"
	       "2. Write Journal\n"
	       "3. Exit\n";
}

int open_journal() {
	int fd;
	fd = open("/dev/journal", O_RDWR);
	if (fd == -1) {
		printf("Error opening /dev/journal: %s", strerror(errno));
		exit(-1);
	}
	return fd;
}



