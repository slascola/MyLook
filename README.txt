---Stephanie Lascola---
---Implementation of the Unix Command "look"---
---CPE 357 Final Project---
---Github URL: github.com/slascola/MyLook---

Overview of MyLook:
   MyLook is an implementation of the unix command "look". MyLook uses a binary
   search to find the inputted string within either the unix dictionary or
   an optional file the user inputs into the command line. Because MyLook 
   uses a binary search, it is required that the user sorts the file before 
   running MyLook. If the file is not specified, a dictionary is used 
   automatically to find the inputted string and therefore does not need to be
   sorted. 

To compile the program: 
   Run either "make" or "make Look" in the terminal using the Makefile. 
   This will create an executable called "MyLook". 

To run the program: 
   MyLook has to be called with a string as part of the arguments. A file can
   be specified after the string however it is not required. If a file is not
   specified then MyLook will run a binary search for the string with the unix
   dictionary. Please follow the specified usage of MyLook:
   
   ./MyLook [-df] [-t char] string [file]

   Optional flags:
      -d This flag will only compare alphanumeric characters from both the 
      inputted string and first word in the dictionary or file. For instance,
      if the string was "C   * a ***)))s" the string that would be compared
      to the dictionary or file would be "Cas" instead.

      -f This flag will not include case (upper or lower) when comparing the 
      inputted string to the first word in the dictionary or file.

      -t This flag shortens the inputted string to the first occurence of the
      character inputted. For instance if the string "Dean" was entered and 
      the flag -t with the character 'e' was entered, the new string that 
      would be compared to the dictionary or file would now be "De". If no 
      character follows the -t flag, an usage error is thrown.

   **IMPORTANT NOTE** 
      When no file is specified, the unix dictionary is used. Even when no flags
      may be present in the command line, the dictionary automatically sets the
      -d flag and the -f flag so it will only compare alphanumeric characters
      and case is ignored.  

Possible error codes:
   Upon return, MyLook returns 0 if one or more matches were found, 1 if no
   matches were found in the dictionary or file, and 2 if an error occured. 
