#include <string.h>
#include <syscall.h>


char* strtok(char * str, char *comp)
{
    static int pos;
    static char *s;
    int i =0, start = pos;

    if(str!=NULL)
        s = str;

    i = 0;
    int j = 0;
    while(s[pos] != '\0')
    {
        j = 0;
        while(comp[j] != '\0')
        {
            if(s[pos] == comp[j])
            {
                s[pos] = '\0';
                pos = pos+1;
                if(s[start] != '\0')
                    return (&s[start]);
                else
                {
                    start = pos;
                    pos--;
                    break;
                }
            }
            j++;
        }
        pos++;
    }
    s[pos] = '\0';

    pos = 0;

    if(s[start] == '\0')
        return NULL;
    else
        return &s[start];
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
