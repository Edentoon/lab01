#include "helper.h"
#include <string.h>
#include <stdio.h>

void strreverse(char* string, char* reverse_str_buffer)
{
    int length = strlen(string);
    int index;
    char reversed_str[30];
    // starts from end of string, copying each char to new string
    reverse_str_buffer[length] = '\0';
    for (int i = length - 1; i > -1; i --)
    {
        index = length-1 - i;
        reversed_str[index] = string[i];
    }
    strncpy(reverse_str_buffer, reversed_str, length);
}