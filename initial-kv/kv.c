#include <assert.h>
#include <intrin.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// all the strlen calls should be replaced with safer strlen_s, couldn't find
// the header file with in on Windows with Mingw-w64
typedef struct DictPair {
  char *value;
} DictPair;

// unsigned long hash(unsigned char *str) {
//   unsigned long hash = 5381;
//   int c;

//   while (c = *str++)
//       hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//   return hash;
// }

char *strsep(char *stringp, const char *delim, int delimNum) {
  assert(delimNum > 0);
  int offset = 0;
  char *delimiterPointer = (stringp != NULL) ? strpbrk(stringp, delim) : NULL;

  if (delimiterPointer == NULL) {
    return stringp;
  }

  for (int i = 1; i != delimNum; i++) {
    offset = delimiterPointer - stringp + 1;
    delimiterPointer = strpbrk(delimiterPointer + 1, delim);
  }
  if (delimiterPointer == NULL) {
    delimiterPointer = stringp + strlen(stringp);
  }

  char *result = (char *)malloc(delimiterPointer - stringp - offset);
  strncpy(result, stringp + offset, delimiterPointer - stringp - offset);
  result[delimiterPointer - stringp - offset] = '\0';
  return result;
}

int deleteValue(int key, char *map[]) {
  if (map[key] != NULL) {
    map[key] = NULL;
  } else {
    printf("Key not found");
  }
  return 0;
}

int getValue(int key, char *map[]) {
  if (map[key] != NULL) {
    printf("%d, %s\n", key, map[key]);
  } else {
    printf("Key not found");
  }
  return 0;
}

int putValue(int key, char *value, char *map[]) {
  map[key] = malloc(strlen(value) + 1);
  strcpy_s(map[key], strlen(value) + 1, value);
  map[strlen(value) - 1] = 0;
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(
        "Possible options are:\
                      \n\t 'g,key' - get value for key arg\
                      \n\t 'p,key,value' - put key value pair into the database \n");
  }

  char *map[64000] = {NULL};
  size_t arraySize = 0;
  for (int i = 1; i < argc; i++) {
    char *command = strsep(argv[i], ",", 1);
    char *end;
    int parsedKeyValue;

    switch (*command) {
      case 'g':
        parsedKeyValue = strtol(strsep(argv[i], ",", 2), &end, 10);
        getValue(parsedKeyValue, map);
        break;
      case 'p':
        // replaced atoi with strtol - safer function
        parsedKeyValue = strtol(strsep(argv[i], ",", 2), &end, 10);
        putValue(parsedKeyValue, strsep(argv[i], ",", 3), map);
        arraySize++;
        break;
      case 'a':
        for (size_t i = 0; i < arraySize; i++) {
          printf("Key: %lld, Value: %s\n", i, map[i]);
        }
        break;
      case 'd':
        parsedKeyValue = strtol(strsep(argv[i], ",", 2), &end, 10);
        deleteValue(parsedKeyValue, map);
        arraySize--;
        break;
      default:
        printf("Unrecognized command has been issued.");
        break;
    }
  }
  return 0;
}
