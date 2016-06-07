#ifndef LOOK_H
#define LOOK_H
#define MAX_LETTERS 50
#define MAX_LINES 50


typedef struct ParseArgs {
   int dFlag;
   int fFlag;
   int tFlag;
   int fileFlag;
   int stringFlag;
   char *string;
   char terminate;
   char *file;
} ParseArgs;

void ArgumentList(int argc, char *argv[], ParseArgs *parsed);
void InitParse(ParseArgs *parsed);
void PrintArgs(ParseArgs *parsed);
char **SaveLines(ParseArgs *parsed, int *lineNdx, int removeFlag);
void PrintFileLines(char **completeFile);
void NoFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches);
void LowerBoundsNoFlag(char **completeFile, ParseArgs *parsed, int savedNdx);
void UpperBoundsNoFlag(char **completeFile, ParseArgs *parsed, int savedNdx,
 int lineNdx);
void FFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches);
void UpperBoundsFFlag(char **completeFile, ParseArgs *parsed, int savedNdx,
 int lineNdx);
void LowerBoundsFFlag(char **completeFile, ParseArgs *parsed, int savedNdx);
void TFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches);
void DFlagComparison(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int lineNdx, int *foundMatches);
void LowerBoundsDFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx);
void UpperBoundsDFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx, int lineNdx);
void DFFlagComparison(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int lineNdx, int *foundMatches);
void LowerBoundsDFFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx);
void UpperBoundsDFFlag(char **completeFile, char **numsAndLetters, 
 ParseArgs *parsed, int savedNdx, int lineNdx);
void FTFlagComparison(char **completeFile, ParseArgs *parsed, int lineNdx,
 int *foundMatches);
void RemoveExtraCharacters(char **numsAndLetters, int *lineNdx, char *line,
 int lineSize);
void RemoveStringCharacters(ParseArgs *parsed);
void FreeParse(ParseArgs *parsed);
#endif
