#include <syscall.h>
#include <file.h>
#include <stdio.h>
#include <string.h>

void runCommand(char *input);
void ls();
void viewFile(char *filename);

void main()
{
    makeForegroundProcess();
	char inputBuffer[128];
	cls();
	putStr("\n----- UserSpace App Shell -----\n");
	printTime();

	while(1)
	{
		putChar('\n');
        putStr("> ");
        char *input = gets(inputBuffer);
        runCommand(input);
	}
}

void runCommand(char *input)
{

    char *cmd = strtok(input, " ");
    char *p1 = strtok(NULL, " ");

	//Handle Empty Input
    if(cmd == NULL)
        return;

    if(strcmp(cmd, "ls") == 0)
    {
        ls();
    }
    else if(strcmp(cmd, "exec") == 0)
    {
        FILE *program = open(p1, 'r');
        if(program == NULL)
        {
            putStr("No Such Program Exists\n");
        }
        else
        {
            createChildProcess(program);
        }
    }
    else if(strcmp(cmd, "view") == 0)
    {
        viewFile(p1);
    }
    else if(strcmp(cmd, "help") == 0)
    {
        putStr("    ls -> list all files\n");
        putStr("    exec 'program file' -> run a program\n");
        putStr("    view 'program file' -> view a plaintext file\n");
        putStr("    killme -> kills current app (shell)\n");
    }
    else if(strcmp(cmd, "killme") == 0)
    {
        suicide();
    }

}

void ls()
{
    int index = 1;
    while(1)
    {
        char *filename = getFileAtIndex(index++);
        if(filename != NULL)
        {
            if(index > 2)
                putStr(", ");
            putStr(filename);
        }
        else
            break;
    }
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
