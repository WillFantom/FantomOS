#include <string.h>
#include <syscall.h>

char *strtok(char *str, const char c)
{

	static char* _buffer;
	if(str != NULL) _buffer = str;
    if(_buffer[0] == '\0') return NULL;
 
    char *ret = _buffer, *b;
    const char *d;
 
    for(b = _buffer; *b !='\0'; b++) {
        for(d = delim; *d != '\0'; d++) {
            if(*b == *d) {
                *b = '\0';
                _buffer = b+1;
 
                // skip the beginning delimiters
                if(b == ret) { 
                    ret++; 
                    continue; 
                }
                return ret;
            }
        }
    }
 
    return ret;

}
