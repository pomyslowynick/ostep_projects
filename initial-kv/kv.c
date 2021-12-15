#include <stdlib.h>
#include <intrin.h>
#include <malloc.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct DictPair
{
  char *value;
} DictPair;

// unsigned long hash(unsigned char *str) {
//   unsigned long hash = 5381;
//   int c;

//   while (c = *str++)
//       hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//   return hash;
// }

char *strsep(char *stringp, const char *delim, int delimNum)
{
  assert(delimNum > 0);
  int offset = 0;
  char *delimiterPointer = (stringp != NULL) ? strpbrk(stringp, delim) : NULL;

  if (delimiterPointer == NULL)
  {
    return NULL;
  }

  for (int i = 1; i != delimNum; i++)
  {
    offset = delimiterPointer - stringp + 1;
    delimiterPointer = strpbrk(delimiterPointer + 1, delim);
  }
  if (delimiterPointer == NULL)
  {
    delimiterPointer = stringp + strlen(stringp);
  }

  char *result = (char *)malloc(delimiterPointer - stringp - offset);
  strncpy(result, stringp + offset, delimiterPointer - stringp - offset);
  result[delimiterPointer - stringp - offset] = '\0';
  printf("The returned command: %s\n", result);
  return result;
}

int getValue(char* key, char* map)
{
  printf("%s\n", key);

  return 0;
}

int putValue(int key, char* value, char map[])
{
  map[key] = (char *)malloc(sizeof value);
  // strncpy(map[key], value, sizeof(value));
  // printf("Inside put value %d %s\n", key, map[key]);
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Possible options are:\
                      \n\t 'g arg' - get value for key arg\
                      \n\t 'p key:value' - put key value pair into database \n");
  }

  char* map[65536];

  for (int i = 1; i < argc; i++)
  {
    char *command = strsep(argv[i], ",", 1);
    switch (*command)
    {
    case 'g':
      getValue(strsep(argv[i], ",", 2), map);
      break;
    case 'p':
      putValue(atoi(strsep(argv[i], ",", 2)), strsep(argv[i], ",", 3), &map);
      // printf("The created struct: %d %s\n", (&map)[atoi(strsep(argv[i], ",", 2))]->key, (&map)[atoi(strsep(argv[i], ",", 2))]->value);
      break;

    default:
      printf("Unrecognized command has been issued.");
      break;
    }
  }
}
