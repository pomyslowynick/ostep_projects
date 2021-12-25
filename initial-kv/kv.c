#include <assert.h>
#include <intrin.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define BUFFER_SIZE 64000
// all the strlen calls should be replaced with safer strlen_s, couldn't find
// the header file with in on Windows with Mingw-w64

// unsigned long hash(unsigned char *str) {
//   unsigned long hash = 5381;
//   int c;

//   while (c = *str++)
//       hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//   return hash;
// }

// helper functions
int saveMapToFile(char *map[], size_t allocated_elements);

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

int getIntegerLength(unsigned x) {
  if (x >= 10000) return 5;
  if (x >= 1000) return 4;
  if (x >= 100) return 3;
  if (x >= 10) return 2;
  return 1;
}

// User level functions
int deleteValue(int key, char *map[]) {
  if (map[key] != NULL) {
    free(map[key]);
    map[key] = NULL;
  } else {
    printf("Key not found\n");
  }
  return 0;
}

int getValue(int key, char *map[]) {
  if (map[key] != NULL) {
    printf("%d, %s\n", key, map[key]);
  } else {
    printf("Key not found\n");
  }
  return 0;
}

int putValue(int key, char *value, char *map[]) {
  map[key] = malloc(strlen(value) + 1);
  strcpy_s(map[key], strlen(value) + 1, value);
  map[key][strlen(value)] = 0;
  return 0;
}

int printAll(char *map[], size_t allocated_elements) {
  for (size_t i = 0; (i < BUFFER_SIZE - 1) && allocated_elements != 0; i++) {
    if (map[i] != NULL) {
      printf("%lld, %s\n", i, map[i]);
      allocated_elements--;
    }
  }
  return 0;
}

int clearDatabaseFile() {
  int returned = remove("database.txt");

  if (returned != 0) {
    printf("Error");
    return EXIT_FAILURE;
  }
  return 0;
}

int clearValues(char *map[], size_t allocated_elements) {
  for (size_t i = 0; (i < BUFFER_SIZE - 1) && allocated_elements != 0; i++) {
    if (map[i] != NULL) {
      free(map[i]);
      map[i] = NULL;
      allocated_elements--;
    }
  }
  if (clearDatabaseFile() != 0) {
    printf("Error removing the file");
  }
  return 0;
}


int saveMapToFile(char *map[], size_t allocated_elements) {
  FILE *fd = fopen("database.txt", "a");
  if (!fd) {
    return EXIT_FAILURE;
  }

  for (size_t i = 0; (i < BUFFER_SIZE - 1) && allocated_elements != 0; i++) {
    if (map[i] != NULL) {
      char formattedSaveString[100];
      size_t stringLen = strlen(map[i]);
      size_t bufferSize =
          100 < stringLen ? 100 : stringLen + 5 + getIntegerLength(i);
      snprintf(formattedSaveString, bufferSize, "%lld, %s\n", i, map[i]);
      fprintf_s(fd, formattedSaveString);
    }
  }
  fclose(fd);
  fd = NULL;
  return 0;
}

size_t readDatabaseFile(char *map[]) {
  FILE *fd = fopen("database.txt", "r");
  char line[1000];
  char *end;
  size_t addedLines = 0;

  if (fd != NULL) {
    while (fgets(line, sizeof line, fd) != NULL) {
      char *formattedLine = (strsep(line, ",", 2) + 1);
      formattedLine[strcspn(formattedLine, "\n")] = 0;
      map[strtol(strsep(line, ",", 1), &end, 10)] = formattedLine;
      addedLines += 1;
    }
  } else {
    return EXIT_FAILURE;
  }

  fclose(fd);
  fd = NULL;

  return addedLines;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(
        "Possible options are:\
                      \n\t 'g,key' - get value for key arg\
                      \n\t 'p,key,value' - put key value pair into the database \n");
  }

  char *map[BUFFER_SIZE] = {NULL};
  size_t numAddedFromFile = readDatabaseFile(map);
  size_t arraySize = numAddedFromFile;
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
        arraySize += 1;
        break;
      case 'd':
        parsedKeyValue = strtol(strsep(argv[i], ",", 2), &end, 10);
        deleteValue(parsedKeyValue, map);
        arraySize--;
        break;
      case 'a':
        printAll(map, arraySize);
        break;
      case 'c':
        clearValues(map, arraySize);
        break;
      default:
        printf("bad command\n");
        break;
    }
  }

  saveMapToFile(map, arraySize);

  return 0;
}
