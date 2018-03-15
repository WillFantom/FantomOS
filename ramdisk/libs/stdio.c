#include <stdio.h>
#include <syscall.h>

char *gets(char *buffer)
{
	int index = 0;
	char input = 0;
	char strEscape = '\n';
	char backspace = '\b';

	while( (input = getInput()) != strEscape)
	{
		//Echo The Input To Display
		putChar(input);

		//BackSpace
		if(input == backspace)
			buffer[--index] = '\0';
		else
			buffer[index++] = input;

	}

	//Echo The Input To Display
	putChar(input);

	buffer[index] = '\0';
	return buffer;
}
