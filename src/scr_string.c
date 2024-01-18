#include "script.h"

char *xstrdup(char *str)
{
    size_t str_len = strlen(str);
    char *s = (char*)malloc(str_len + 1);
    strncpy(s,str,strlen(str));
    s[str_len] = '\0';
    return s;
}

void Scr_StrTok(int a1)
{
    char* str = Scr_GetString(0);
    char* delim = Scr_GetString(1);
    char* tok = strtok(str, delim);
    Scr_MakeArray();
    while(NULL!=tok)
    {
        Scr_AddString(tok);
        Scr_AddArray();
        tok = strtok(NULL, delim);
    }
}

void Scr_IsSubStr(int a1)
{
    char* string = Scr_GetString(0);
    char* substring = Scr_GetString(1);

    Scr_AddBool( ( strstr(string, substring) != NULL ) );
}

void Scr_ToLower(int a1)
{
    char* str = Scr_GetString(0);
    char *s = xstrdup(str);
    for(char *it = s; *it; ++it)
        *it = tolower(*it);
    Scr_AddString(s);
    free(s);
}

void Scr_ToUpper(int a1)
{
    char* str = Scr_GetString(0);
    char *s = xstrdup(str);
    for(char *it = s; *it; ++it)
        *it = toupper(*it);
    Scr_AddString(s);
    free(s);
}

void Scr_trim(int a1)
{
    char *ws = "\n\r\t\f ";
    char *str = Scr_GetString(0);
    char *tmp = (char*)malloc(strlen(str) + 1);
    size_t tmp_idx = 0;
    
    size_t str_size = strlen(str);
    
    for(int i = 0; i < str_size; i++)
    {
        int ff = 0;	
        for(char *c = ws; *c; c++)
        {
            if(*c == str[i])
            {
                ff=1;
                break;
            }
        }
        if(ff)
            continue;
        
        tmp[tmp_idx++] = str[i];
    }
    tmp[tmp_idx] = '\0';
    Scr_AddString(tmp);
    free(tmp);
}

void Scr_convertToIString(int a1)
{
    char* str = Scr_GetString(0);
    Scr_AddIString(str);
}

void Scr_ucfirst(int a1)
{
    char* str = Scr_GetString(0);
    char *s = xstrdup(str);
    *s = toupper(*s);
    Scr_AddString(s);
    free(s);
}

void Scr_replace(int a1)
{
    char* orig = Scr_GetString(0);
    char* rep = Scr_GetString(1);
    char* with = Scr_GetString(2);
    char *result = str_replace(orig, rep, with);
    Scr_AddString(result);
    free(result);
}