#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Look.h"

void InitParse(ParseArgs *parsed) {
   parsed->dFlag = 0;
   parsed->fFlag = 0;
   parsed->tFlag = 0;

   parsed->fileFlag = 0;
   parsed->stringFlag = 0;
   parsed->terminate = 0;

   parsed->string = calloc(MAX_LETTERS, sizeof(char));
   parsed->file = calloc(MAX_LETTERS, sizeof(char));
}

void ArgumentList(int argc, char *argv[], ParseArgs *parsed) {
   int count = 1;
   int flagCount = 1;
   FILE *file;

   if (argc < 2) {
      fprintf(stderr, "usage: ./MyLook [-df] [-t char] string [file]\n");
      exit(2);
   }
   while (argv[count] != NULL) {
      if (argv[count][0] == '-') {
         while (argv[count][flagCount] != '\0') {
            if (argv[count][flagCount] == 'd') {
               parsed->dFlag = 1;
            }
            else if (argv[count][flagCount] == 'f') {
               parsed->fFlag = 1;
            }
            else if (argv[count][flagCount] == 't') {
               parsed->tFlag = 1;
               parsed->terminate = argv[++count][0];
               break; 
            }
            else {
               fprintf(stderr, "look: invalid option -- '%c'\n", 
                argv[count][flagCount]);
               fprintf(stderr, "usage: ./MyLook [-df] [-t char] "
                "string [file]\n");
               exit(2);
            }
            flagCount++;
         }
         flagCount = 1;
      }
      else {
         if (parsed->stringFlag == 1) {
            if ((file = fopen(argv[count], "r")) != NULL) {
               parsed->fileFlag = 1;
               strcpy(parsed->file, argv[count]);
               fclose(file);
            }
            else {
               fprintf(stderr, "./MyLook: %s: No such file or directory\n",
                argv[count]);
               exit(2);
            }
         }
         else {
            parsed->stringFlag = 1;
            strcpy(parsed->string, argv[count]);
         }
      }
      count++;
   }
   if (parsed->stringFlag == 0) {
      fprintf(stderr, "usage: ./MyLook [-df] [-t char] string [file]\n");
      exit(2);
   }
}

void PrintArgs(ParseArgs *parsed) {
   if (parsed->dFlag) {
      printf("D flag set!\n");
   }
   if (parsed->fFlag) {
      printf("F flag set!\n");
   }
   if (parsed->tFlag) {
      printf("T flag set!\n");
      printf("Terminating character--%c\n", parsed->terminate);
   }
   printf("String: %s\n", parsed->string);
   if (parsed->fileFlag) {
      printf("File flag set!\n");
      printf("File: %s\n", parsed->file);
   }
   
}

char **SaveLines(ParseArgs *parsed, int *lineNdx, int removeFlag) {
   FILE *file;
   char *line, *saved, **completeFile, character;
   int lineSize = 0, maxLine = MAX_LINES, count = 0;
   int maxFileLine = MAX_LINES, maxFileString = MAX_LETTERS;

   line = calloc(MAX_LINES, sizeof(char));

   completeFile = calloc(MAX_LINES, sizeof(char *));
   for (count = 0; count < MAX_LINES; count++) {
      completeFile[count] = calloc(MAX_LETTERS, sizeof(char));
   }

   if (parsed->fileFlag) {
      file = fopen(parsed->file, "r");
   }
   else {
      file = fopen("/usr/share/dict/words", "r");
   }
   while ((character = fgetc(file)) != EOF) {
      while (character != '\n' && character != EOF) {
         if (lineSize == maxLine) {
            maxLine *= 2;
            line = realloc(line, sizeof(char) * maxLine);
         }
         if (lineSize == maxFileString) {
            maxFileString *= 2;
            completeFile[(*lineNdx)] = realloc(completeFile[(*lineNdx)], 
             sizeof(char) * maxFileString);

         }
         line[lineSize++] = character;
         character = getc(file);
      }
      if (lineSize != 0) {
         if ((*lineNdx) + 1 == maxFileLine) {
            maxFileLine *= 2;
            completeFile = realloc(completeFile, 
             sizeof(char *) * maxFileLine);
            for (count = (*lineNdx) + 1; count < maxFileLine; count++) {
               completeFile[count] = calloc(maxFileString, sizeof(char));
            }

         }
         if (removeFlag) {
            RemoveExtraCharacters(completeFile, lineNdx, line, lineSize);
         }
         else {
            strcpy(completeFile[(*lineNdx)], line);
         }
         (*lineNdx)++;
      }
      lineSize = 0;
      saved = line;
      free(saved);
      line = calloc(MAX_LINES, sizeof(char));
   }
   completeFile[(*lineNdx)] = NULL;
   fclose(file);
   return completeFile;
}

void PrintFileLines(char **completeFile) {
   int count = 0;

   while (completeFile[count] != NULL) {
      printf("Line: %s\n", completeFile[count]);
      count++;
   }
}

void NoFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1;
   int middleLine;
   int savedNdx = 0;

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncmp(completeFile[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncmp(completeFile[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsNoFlag(completeFile, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsNoFlag(completeFile, parsed, savedNdx, lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void LowerBoundsNoFlag(char **completeFile, ParseArgs *parsed, int savedNdx) {
   int start;

   savedNdx--;
   start = savedNdx;

   while ((savedNdx >= 0) && strncmp(completeFile[savedNdx], parsed->string, 
    strlen(parsed->string)) == 0) {
      savedNdx--;
   }
   savedNdx++;
   while (savedNdx <= start) {
      printf("%s\n", completeFile[savedNdx++]);
   } 
}

void UpperBoundsNoFlag(char **completeFile, ParseArgs *parsed, int savedNdx,
 int lineNdx) {
   savedNdx++;
   while ((savedNdx <= lineNdx - 1) && strncmp(completeFile[savedNdx], 
    parsed->string, strlen(parsed->string)) == 0) {
      printf("%s\n", completeFile[savedNdx]);
      savedNdx++;
   }
}

void FFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1;
   int middleLine;
   int savedNdx = 0;

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncasecmp(completeFile[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncasecmp(completeFile[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsFFlag(completeFile, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsFFlag(completeFile, parsed, savedNdx, lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void LowerBoundsFFlag(char **completeFile, ParseArgs *parsed, int savedNdx) {
   int start;

   savedNdx--;
   start = savedNdx;

   while ((savedNdx >= 0) && strncasecmp(completeFile[savedNdx], parsed->string, 
    strlen(parsed->string)) == 0) {
      savedNdx--;
   }
   savedNdx++;
   while (savedNdx <= start) {
      printf("%s\n", completeFile[savedNdx++]);
   } 
}

void UpperBoundsFFlag(char **completeFile, ParseArgs *parsed, int savedNdx,
 int lineNdx) {
   savedNdx++;
   while ((savedNdx <= lineNdx - 1) && strncasecmp(completeFile[savedNdx], 
    parsed->string, strlen(parsed->string)) == 0) {
      printf("%s\n", completeFile[savedNdx]);
      savedNdx++;
   }
}

void TFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx, 
 int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1, middleLine, count = 0;
   int savedNdx = 0;
   char *saved = parsed->string;

   while (*(parsed->string) != parsed->terminate) {
      count++;
      if (count > strlen(saved)) {
         break;
      }
      (parsed->string)++;
   }
   (parsed->string)++;
   *(parsed->string) = 0;
   parsed->string = saved;

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncmp(completeFile[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncmp(completeFile[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsNoFlag(completeFile, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsNoFlag(completeFile, parsed, savedNdx, lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void FTFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx, 
 int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1, middleLine, count = 0;
   int savedNdx = 0;
   char *saved = parsed->string;

   while (*(parsed->string) != parsed->terminate) {
      count++;
      if (count > strlen(saved)) {
         break;
      }
      (parsed->string)++;
   }
   (parsed->string)++;
   *(parsed->string) = 0;
   parsed->string = saved;

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncasecmp(completeFile[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncasecmp(completeFile[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsFFlag(completeFile, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsFFlag(completeFile, parsed, savedNdx, lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void RemoveExtraCharacters(char **numsAndLetters, int *lineNdx, char *line,
 int lineSize) {
   char *saved = calloc(strlen(line) + 1, sizeof(char));
   int count = 0, origLineCount = 0;

   while (origLineCount < lineSize && line[origLineCount] != '\n') {
      if (isalnum(line[origLineCount])) {
         saved[count] = line[origLineCount];
         count++;
      }
      origLineCount++;
   }
   saved[count] = 0;
   strcpy(numsAndLetters[(*lineNdx)], saved);

}

void DFlagComparison(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int lineNdx, int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1, middleLine, count = 0;
   int savedNdx = 0;
   char *saved = parsed->string;

   if (parsed->tFlag) {
      while (*(parsed->string) != parsed->terminate) {
         count++;
         (parsed->string)++;
         if (count > strlen(saved)) {
            break;
         }
      }
      (parsed->string)++;
      *(parsed->string) = 0;
      parsed->string = saved;
   }
   RemoveStringCharacters(parsed);

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncmp(numsAndLetters[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncmp(numsAndLetters[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsDFlag(completeFile, numsAndLetters, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsDFlag(completeFile, numsAndLetters, parsed, savedNdx, 
          lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void LowerBoundsDFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx) {
   int start;

   savedNdx--;
   start = savedNdx;

   while ((savedNdx >= 0) && strncmp(numsAndLetters[savedNdx], parsed->string, 
    strlen(parsed->string)) == 0) {
      savedNdx--;
   }
   savedNdx++;
   while (savedNdx <= start) {
      printf("%s\n", completeFile[savedNdx++]);
   } 
}

void UpperBoundsDFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx, int lineNdx) {
   savedNdx++;
   while ((savedNdx <= lineNdx - 1) && strncmp(numsAndLetters[savedNdx], 
    parsed->string, strlen(parsed->string)) == 0) {
      printf("%s\n", completeFile[savedNdx]);
      savedNdx++;
   }
}

void DFFlagComparison(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int lineNdx, int *foundMatches) {
   int firstLine = 0, lastLine = lineNdx - 1, middleLine, count = 0;
   int savedNdx = 0;
   char *saved = parsed->string;

   if (parsed->tFlag) {
      while (*(parsed->string) != parsed->terminate) {
         count++;
         if (count > strlen(saved)) {
            break;
         }
         (parsed->string)++;
      }
      (parsed->string)++;
      *(parsed->string) = 0;
      parsed->string = saved;
   }
   RemoveStringCharacters(parsed);

   while (firstLine <= lastLine) {
      middleLine = (firstLine + lastLine) / 2;

      if (strncasecmp(numsAndLetters[middleLine], parsed->string,
       strlen(parsed->string)) < 0) {
         firstLine = middleLine + 1;
      }
      else if (strncasecmp(numsAndLetters[middleLine], parsed->string, 
       strlen(parsed->string)) == 0) {
         *foundMatches = 0;
         savedNdx = middleLine;
         LowerBoundsDFFlag(completeFile, numsAndLetters, parsed, savedNdx);
         printf("%s\n", completeFile[middleLine]);
         UpperBoundsDFFlag(completeFile, numsAndLetters, parsed, savedNdx, 
          lineNdx);
         break;
      }
      else {
         lastLine = middleLine - 1;
      }
   }
}

void LowerBoundsDFFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx) {
   int start;

   savedNdx--;
   start = savedNdx;

   while ((savedNdx >= 0) && strncasecmp(numsAndLetters[savedNdx], 
    parsed->string, strlen(parsed->string)) == 0) {
      savedNdx--;
   }
   savedNdx++;
   while (savedNdx <= start) {
      printf("%s\n", completeFile[savedNdx++]);
   } 
}

void UpperBoundsDFFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx, int lineNdx) {
   savedNdx++;
   while ((savedNdx <= lineNdx - 1) && strncasecmp(numsAndLetters[savedNdx], 
    parsed->string, strlen(parsed->string)) == 0) {
      printf("%s\n", completeFile[savedNdx]);
      savedNdx++;
   }
}

void RemoveStringCharacters(ParseArgs *parsed) {
   char *saved = calloc(strlen(parsed->string) + 1, sizeof(char));
   char *parsedString = parsed->string;
   int count = 0, origStringCount = 0, stringLen = strlen(parsed->string);

   while (origStringCount < stringLen && parsedString[origStringCount] 
    != '\n') {
      if (isalnum(parsedString[origStringCount])) {
         saved[count] = parsedString[origStringCount];
         count++;
      }
      origStringCount++;
   }
   saved[count] = 0;
   strcpy(parsed->string, saved);

}

void FreeParse(ParseArgs *parsed) {
   free(parsed->string);
   free(parsed->file);
}
