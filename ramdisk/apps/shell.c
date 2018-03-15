#include <syscall.h>
#include <file.h>
#include <stdio.h>

void runCommand(char *input);

void main()
{
	char inputBuffer[128];
	cls();
	putStr("\n----- UserSpace App Shell -----\n");
	printTime();

	while(1)
	{
		putChar('\n');

        //Run UserSpace App
        FILE *program = NULL;
        while(program == NULL) {
            program = open(gets(inputBuffer), 'r');
            if(program == NULL)
                putStr("No Such Program");
        }
        createChildProcess(program);

        viewFile(gets(inputBuffer));
	}

    while(1);
}

void runCommand(char *input)
{
	//Handle Empty Input
	if(input == '\0')
		return;

	//Get Command
	//char *command = ;
}

void viewFile(char *filename)
{
	FILE *fileInfo = dlcalloc(1, sizeof(FILE));
	FILE *node = open(filename, 'r');
	if(node == NULL)
	{
		putStr("No Such File\n");
		return;
	}
    getFileInfo(node,fileInfo );
	char *fileContents = dlcalloc(1, fileInfo->file_size + 1);
	read(node, fileContents, fileInfo->file_size + 1, 0);

	putStr(fileContents);
	putChar('\n');

	dlfree(fileInfo);
	dlfree(fileContents);

	return;
}
