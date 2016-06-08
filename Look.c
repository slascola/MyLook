#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Look.h"


int main(int argc, char *argv[]) {
   char **completeFile, **numsAndLetters;
   int lineNdx = 0, foundMatches = 1;

   ParseArgs *parsed = calloc(1, sizeof(ParseArgs));

   InitParse(parsed);
   ArgumentList(argc, argv, parsed);
   completeFile = SaveLines(parsed, &lineNdx, 0);

   lineNdx = 0;
   numsAndLetters = SaveLines(parsed, &lineNdx, 1);


   if (parsed->fileFlag == 0) {
      DFFlagComparison(completeFile, numsAndLetters, parsed, lineNdx, 
       &foundMatches);
   }
   else if (!(parsed->dFlag) && parsed->fFlag && !(parsed->tFlag)) {
      FFlagComparison(completeFile, parsed, lineNdx, &foundMatches);
   }
   else if (!(parsed->dFlag) && (!parsed->fFlag) && parsed->tFlag) {
      TFlagComparison(completeFile, parsed, lineNdx, &foundMatches);
   }
   else if (parsed->dFlag && !(parsed->fFlag) && !(parsed->tFlag)) {
      DFlagComparison(completeFile, numsAndLetters, parsed, lineNdx,
       &foundMatches);      
   }
   else if (parsed->dFlag && parsed->fFlag && !(parsed->tFlag)) {
      DFFlagComparison(completeFile, numsAndLetters, parsed, lineNdx,
       &foundMatches);
   }
   else if (parsed->dFlag && !(parsed->fFlag) && parsed->tFlag) {
      DFlagComparison(completeFile, numsAndLetters, parsed, lineNdx,
       &foundMatches);
   }
   else if (!(parsed->dFlag) && parsed->fFlag && parsed->tFlag) {
      FTFlagComparison(completeFile, parsed, lineNdx, &foundMatches);
   }
   else if (parsed->dFlag && parsed->fFlag && parsed->tFlag) {
      DFFlagComparison(completeFile, numsAndLetters, parsed, lineNdx,
       &foundMatches);
   }
   else {
      NoFlagComparison(completeFile, parsed, lineNdx, &foundMatches);
   }
   FreeParse(parsed);
   free(parsed);
   
   if (foundMatches) {
      return 1;
   }
   
   return 0;
}
