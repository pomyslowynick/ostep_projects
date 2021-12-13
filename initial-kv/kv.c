#include <stdlib.h>
#include <intrin.h>
#include <malloc.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct DictPair {
    int value;
    char* key;
} DictPair;

char* strsep(char* stringp, const char* delim)
{
  char* p;

  p = (stringp != NULL) ? strpbrk(stringp, delim) : NULL;

  if (p == NULL)
  {
    stringp = NULL;
  }
  else
  {
    *p = '\0';
    // stringp = p + 1;
  }
  printf("%s\n", stringp);
  return stringp;
}

int getValue(char* key, DictPair* map) {
    printf("%s\n", key);

    return 0;
}

int putValue(char* key, int value, DictPair** position) {
    printf("%s\n", key);
    *position = (DictPair* )malloc(sizeof(DictPair));
    (*position)->value = value;
    (*position)->key = key;
    printf("Inside put value %s %d\n", (*position)->key, (*position)->value);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Possible options are:\
                        \n\t 'g arg' - get value for key arg\
                        \n\t 'p key:value' - put key value pair into database \n");
    }

    DictPair map[1000]; 
    DictPair* nextPosition = map;

    for (int i = 1; i < argc; i++) {
        char *token = strsep(argv[i], ",");
        printf("%c\n", *token);
        switch (*token)
        {
        case 'g':
            getValue(argv[i], map);
            break;
        case 'p':
            putValue(argv[i], argv[i][2], &nextPosition);
            printf("%s %d\n", nextPosition->key, nextPosition->value);
            nextPosition += sizeof(DictPair);
            break;
        
        default:
            break;
        }
    }
}

