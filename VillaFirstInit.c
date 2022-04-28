/********************************************************************************************** This is to certify that this 
project is my own work, based on my personal efforts in studying and applying the concepts learned.  I have constructed
the functions and their respective algorithms and corresponding code by myself.  The program was run, tested, and
debugged by my own efforts.  I further certify that I have not copied in part or whole or otherwise plagiarized the work of
other students and/or persons.
                                                                                   Jacob Miguel C. Villa, DLSU ID# 11946296
**********************************************************************************************/

/* Make sure that the main() is not in this file. */

#include "trivia.h"

/* You can add other data structures here.
   
   You are expected to implement other functions not listed
   in the trivia.h here. Do not forget to include internal 
   documentation. For functions with long (many lines) of code,
   make sure to also include inline comments to discuss your
   algorithm.
*/





/* Implement the functions listed in trivia.h here. 
   Read through the comments to know what the function is
   supposed to accomplish.  Note that the function names
   may not exactly be performing the whole task indicated
   in the mp specs.  For example, some may not have the 
   getting of input be part of the function, instead it 
   could be somewhere else and was just passed to the 
   function as parameter.
   
   Provided below are examples of the comments before 
   each function.  
*/

/* Given the filename stored in fname, this function
   loads the entries of the text file and ADDS them to
   the array aEntries. The number of elements should be
   updated in pElem. For each trivia word in the file 
   that already exists in aEntries, the user is asked if 
   he wants to keep the one in the array or replace it 
   with the one from the text file.
 
   @param fname - the string filename, including the extension
   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void 
import(strWord fname, wordList aEntries, int * pElem)
{
   FILE * fp;
   int i, nIndex;
   char ans, dump;
   struct triviaTag tempEntry;
   strWord tempWord;
   char tempLine[LEN2 * 2 + 3] = "";

   if ((fp = fopen(fname, "r")) != NULL)
   {
      while ((fgets(tempLine, LEN2 * 2 + 3, fp)) != NULL) // Getting current line as a string, repeats until EOF is reached
      {
         tempLine[strlen(tempLine) - 1] = '\0'; // Trimming off newline character
         for (i = 0; i < 8; i++)
            tempWord[i] = tempLine[i]; // Getting the first 7 characters to check if it read "Object: "
         tempWord[i] = '\0';
         
         if (strcmp(tempWord, "Object: ") == 0) // Checking if currently reading "Object: "
         {
            // Resetting fields of tempEntry
            tempEntry.numClues = 0;
            tempEntry.use = -1;
            for (i = 0; i < 10; i++)
            {
               tempEntry.clueList[i].relation[0] = '\0';
               tempEntry.clueList[i].relValue[0] = '\0';
            }

            for (i = 0; i < strlen(tempLine) - 8; i++)
               tempEntry.answer[i] = tempLine[i + 8]; 
            tempEntry.answer[i] = '\0'; // Obtaining the answer from the line that was read

            nIndex = searchWord(tempEntry.answer, aEntries, *pElem);
            if (nIndex != -1) // Word already exists
            {
               do
               {
                  printf("\n%s already exists in the list. Overwrite existing data for the word? [y/n]: ", tempEntry.answer);
                  scanf("%c", &ans);
                  scanf("%c", &dump);
               } while (ans != 'y' && ans != 'n');

               switch (ans)
               {
                  case 'y':
                     while (strcmp(fgets(tempLine, LEN2 * 2 + 3, fp), "\n") != 0 && tempEntry.numClues < 10) // Repeats until "\n" is read from the line
                     {
                        // Getting relation
                        i = 0;                        
                        do 
                        {
                           tempEntry.clueList[tempEntry.numClues].relation[i] = tempLine[i];
                           i++;
                        } while (tempLine[i] != ':');
                        tempEntry.clueList[tempEntry.numClues].relation[i] = '\0';

                        // Getting relation value
                        i = 0;                        
                        do
                        {
                           tempEntry.clueList[tempEntry.numClues].relValue[i] = tempLine[i + strlen(tempEntry.clueList[tempEntry.numClues].relation) + 2];
                           i++;
                        } while (tempLine[i + strlen(tempEntry.clueList[tempEntry.numClues].relation) + 2] != '\n');
                        tempEntry.clueList[tempEntry.numClues].relValue[i] = '\0';

                        tempEntry.numClues++;                       
                     }
                     // Assigning the obtained entry to replace the word
                     aEntries[nIndex] = tempEntry;
                     break;

                  case 'n':
                     // Will keep repeating the while() loop until the next line has "Object: "
                     break;

                  default:
                     break;
               }
            }
            else // Word does not exist yet, adding to aEntries
            {
               while (strcmp(fgets(tempLine, LEN2 * 2 + 3, fp), "\n") != 0 && tempEntry.numClues < 10) // Repeats until "\n" is read from the line
               {
                  // Getting relation
                  i = 0;                        
                  do 
                  {
                     tempEntry.clueList[tempEntry.numClues].relation[i] = tempLine[i];
                     i++;
                  } while (tempLine[i] != ':');
                  tempEntry.clueList[tempEntry.numClues].relation[i] = '\0';

                  // Getting relation value
                  i = 0;                        
                  do
                  {
                     tempEntry.clueList[tempEntry.numClues].relValue[i] = tempLine[i + strlen(tempEntry.clueList[tempEntry.numClues].relation) + 2];
                     i++;
                  } while (tempLine[i + strlen(tempEntry.clueList[tempEntry.numClues].relation) + 2] != '\n');
                  tempEntry.clueList[tempEntry.numClues].relValue[i] = '\0';

                  tempEntry.numClues++;                        
               }
               
               // Adding the entry as the last element of aEntries
               aEntries[*pElem] = tempEntry;
               (*pElem)++;
            }
         }
      }

      fclose(fp);
      printf("\nWords in %s successfully imported.\n", fname);
   }
   else
      printf("\n%s cannot be opened.\n", fname);
}

/* This function initializes the gameboard with nRows by
   nCols number of characters based on the contents of 
   aEntries.  The use member field is also updated to
   signify that this entry is already used in the board
   for the current game.  The function returns 1 if the
   board can be fully initialized with unique entries based
   on the needed dimension. Otherwise, the function returns 0.

   @param gameboard - 2D array of characters containing the
                      first letters of the answer
   @param nRows - the number rows to be initialized in gameboard
   @param nCols - the number columns to be initialized in gameboard
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
   @return 1 if successful initialization of the gameboard, 0 otherwise
*/
int 
initBoard(arrMatrix gameboard, int nRows, int nCols,
              wordList aEntries, int nElem) 
{
	int i, j, k, nRand, nCount;
   int bInit = 1, bPass;
   

   // Overwriting garbage values of use field of each element in aEntries
   for (i = 0; i < nElem; i++)
      aEntries[i].use = -1;
   

   if (nRows * nCols <= nElem)
   {
      for (i = 0; i < nRows && bInit; i++)
      {
         do
         {
            nRand = rand() % nElem;
         } while (aEntries[nRand].use != -1);
   
         
         // Initializing first element of row to the first character of random word
         gameboard[i][0] = aEntries[nRand].answer[0];

         // Setting use field to the position of the character on the gameboard
         aEntries[nRand].use = i * nCols;
         
         for (j = 1; j < nCols && bInit; j++)
         {
            // Reinitializing nCount to 0 and all the -2 .use to -1 
            nCount = 0; // Counts how many items from aEntries have been tested for the current element in the gameboard
            for (k = 0; k < nElem; k++)
            {
               if (aEntries[k].use == -2)
                  aEntries[k].use = -1;
            }

            do
            {
               bPass = 1;
               do
               {
                  nRand = rand() % nElem;
               } while (aEntries[nRand].use != -1 && nCount < nElem - (i * nCols + j));

               if (nCount < nElem - (i * nCols + j))
               {
                  // Cross-checking the randomly selected word with the previous elements in the row
                  for (k = 0; k < j && bPass; k++)
                  {
                     if (gameboard[i][k] == aEntries[nRand].answer[0])
                        bPass = 0;
                  }
                  if (bPass)
                  {
                     // Placing the character onto the gameboard if it does not conflict with other elements in the row
                     gameboard[i][j] = aEntries[nRand].answer[0];
                     aEntries[nRand].use = i * nCols + j;
                  }
                  else 
                  {
                     aEntries[nRand].use = -2; // -2 indicates that it was tested and was not able to be placed into the current element
                     nCount++;
                  }
               }
               else 
                  bInit = 0;
            } while (!bPass && bInit);
            
         }

         // Resetting all .use back to -1 after the last element in the row is initialized
         for (k = 0; k < nElem; k++)
         {
            if (aEntries[k].use == -2)
               aEntries[k].use = -1;
         }
      }
   }
   else
      bInit = 0;

   if (!bInit)
      printf("\nBoard could not be initialized.\n");

   return bInit;
}

/* This function facilitates the game play of the user,
   including determining when the game ends. 
   @param gameboard - the 2D array of characters
   @param nRows - the number of rows in gameboard
   @param nCols - the number of columns in gameboard
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries   
*/   
void 
play(arrMatrix gameboard, int nRows, int nCols,
          wordList aEntries, int nElem)
{
   char cChoice, dump;
   int i, j, k, row, col;
   int nColsGuessed, nChosenIndex, nWordIndex, nRand;
   int bPassRow, bLoseGame = 0, bValidInput, bWordFound;
   strWord tempWord;

   for (i = 0; i < nRows && !bLoseGame; i++)
   {  
      bPassRow = 0, nColsGuessed = 0, bValidInput = 0;

      while (nColsGuessed < nCols && !bPassRow && !bLoseGame)
      {
         // Displaying gameboard
         printf("\n");
         for (row = 0; row < nRows; row++)
         {
            for (col = 0; col < nCols; col++)
               printf("%c ", gameboard[row][col]);
            printf("\n");
         }

         /* Obtaining and filtering valid input */
         do
         {
            printf("\nROW #%d\n", i + 1);
            printf("\nSelect a letter from Row %d (Enter 0 to exit): ", i + 1);
            scanf("%c", &cChoice);
            scanf("%c", &dump);

            if (cChoice != '0')
            {
               bValidInput = 0;
               /* Finding character input in the current row */
               for (j = 0; j < nCols; j++)  
               {
                  if (gameboard[i][j] == cChoice && gameboard[i][j] != '*' && gameboard[i][j] != '-')
                  {
                     bValidInput = 1;
                     nChosenIndex = j;
                  }
               }
            }
         } while (!(bValidInput || cChoice == '0'));

         switch (cChoice)
         {
            case '0': // If the player decides to quit mid-game 
               bLoseGame = 1;
               break;

            default:
               /* Actual guessing gameplay */

               bWordFound = 0;

               // Searching for corresponding word in aEntries based on character input from user
               for (k = 0; k < nElem && !bWordFound; k++)
               {
                  if (aEntries[k].use == i * nCols + nChosenIndex) 
                  {
                     bWordFound = 1;
                     nWordIndex = k;
                  }   
               }
               
               // Randomizing clue to be shown 
               nRand = rand() % aEntries[nWordIndex].numClues;
               printf("\nClue:\n");
               printf("%s: %s\n", aEntries[nWordIndex].clueList[nRand].relation, aEntries[nWordIndex].clueList[nRand].relValue);
               printf("\nEnter guess: ");
               getLongString(tempWord);

               // Checking if guess is correct
               if (strcmp(tempWord, aEntries[nWordIndex].answer) == 0)
               {
                  gameboard[i][nChosenIndex] = '*';
                  bPassRow = 1;
               }
               else
               {   
                  gameboard[i][nChosenIndex] = '-';
                  nColsGuessed++;
               }
               break;
         }
         if (nColsGuessed == nCols)
            bLoseGame = 1;
      }
   }

   if (bLoseGame)
      printf("\nYou lost the Word Trivia game!\n");
   else if (i == nRows)
      printf("\nCongratulations! You won the Word Trivia game!\n");

}

/* This function shows the menu options in the maintenance
   phase as well as call the functions (like listWords(), 
   addWord(), etc.) that are relevant to the chosen option.   
*/
void maintenance() 
{
   int nChoice, nIndex;
   char dump; 

   /* Declaring local instances variables to be passed as parameters */
   wordList aEntries;
   strWord tempWord;
   strWord fname;
   int nElem = 0;

   do
   {   
      printf("\nProgram Maintenance\n\n"
         "\t[1] - Add Word\n"
         "\t[2] - Add Trivia\n"
         "\t[3] - Modify Entry\n"
         "\t[4] - Modify Word\n"
         "\t[5] - Delete Word\n"
         "\t[6] - Delete Clue\n"
         "\t[7] - View Words\n"
         "\t[8] - View Clues\n"
         "\t[9] - Export\n"
         "\t[10] - Import\n"
         "\t[11] - Exit\n");
      printf("\nSelect action: ");
      scanf("%d", &nChoice);
      scanf("%c", &dump);
      switch (nChoice)
      {
         case 1: // Add Word
            // Checking if aEntries can still accept words
            if (nElem < CAP)
               addWord(aEntries, &nElem);
            else 
               printf("\nNo more words can be added.\n");
            break;

         case 2: // Add Trivia
            printf("\nEnter word to add trivia to: ");
            getLongString(tempWord);
            nIndex = searchWord(tempWord, aEntries, nElem);

            if (nIndex != -1)
               addTrivia(&aEntries[nIndex]);
            else  
               printf("\n""%s"" does not exist in the list.\n", tempWord);
            break;

         case 3: // Modify Entry
            listWords(aEntries, nElem);

            printf("\nEnter word whose clues will be modified: ");
            getLongString(tempWord);
            nIndex = searchWord(tempWord, aEntries, nElem);

            if (nIndex != -1)
               modifyEntry(&aEntries[nIndex]);
            else  
               printf("\n""%s"" does not exist in the list.\n", tempWord);
            break;

         case 4: // Modify Word
            listWords(aEntries, nElem);

            printf("\nEnter word to be modified: ");
            getLongString(tempWord);
            nIndex = searchWord(tempWord, aEntries, nElem);

            if (nIndex != -1)
               modifyWord(&aEntries[nIndex], aEntries, nElem);
            else  
               printf("\n""%s"" does not exist in the list.\n", tempWord);
            break;

         case 5: // Delete Word
            deleteWord(aEntries, &nElem);
            break;

         case 6: // Delete Clue
            deleteClue(aEntries, nElem);
            break;

         case 7: // View Words
            viewWords(aEntries, nElem);
            break;

         case 8: // View Clues
            listWords(aEntries, nElem);

            printf("\nEnter word whose clues will be viewed: ");
            getLongString(tempWord);
            nIndex = searchWord(tempWord, aEntries, nElem);

            if (nIndex != -1)
               viewEntry(aEntries[nIndex]);
            else  
               printf("\n""%s"" does not exist in the list.\n", tempWord);
            break;

         case 9: // Export
            printf("\nEnter filename to export words to: ");
            getLongString(fname);
            export(fname, aEntries, nElem);
            break;

         case 10: // Import
            printf("\nEnter filename to import words from: ");
            getLongString(fname);
            import(fname, aEntries, &nElem);
            break;

         case 11: // Exit
            break;

         default:
            printf("\nPlease provide valid input.\n");
            break;
      }
   } while (nChoice != 11);

}

/* This function returns the index where the key was found
   in aEntries. If not found, the function returns -1.
   @param key - the word or phrase to look for
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
   @return the index where key was found, -1 otherwise
*/
int 
searchWord(strWord key, wordList aEntries, int nElem)
{
	int i, nWordIndex;
   int bFound = 0;

   for (i = 0; i < nElem && !bFound; i++)
   {
      if (!strcmp(key, aEntries[i].answer))
      {
         nWordIndex = i;
         bFound = 1;
      }
   }

   if (bFound)
      return nWordIndex;
   return -1;
}


/* This function displays the list of words (without the clues)
   in aEntries. 
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/ 
void 
listWords(wordList aEntries, int nElem)
{
   int i;

   sortEntries(aEntries, nElem); 

   printf("\n");
   for (i = 0; i < nElem; i++)
      printf("%s\n", aEntries[i].answer);
}

/* This function will ask the user to input the word, calls
   searchWord() to check if the word is unique.  If unique, this
   word is added, the addTrivia() is called, and the
   number of elements is updated. Other error checking (as 
   indicated in the MP specs requirements) are also done.

   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void 
addWord(wordList aEntries, int * pElem) 
{
   strWord tempWord;

   printf("\n\nEnter word to be added: ");
   getLongString(tempWord);

   // Checking if word contains at least 1 character
   if (tempWord[0] != '\0')
   {
      if (searchWord(tempWord, aEntries, *pElem) == -1)
      {
         strcpy(aEntries[*pElem].answer, tempWord);
         printf("\n""%s"" has been added to the list of words.\n", tempWord);
         printf("\nAdding trivia for ""%s"".\n", tempWord);
         addTrivia(&aEntries[*pElem]);
         (*pElem)++;
      }
      else 
         printf("\n\n""%s"" already exists in the list of words.", tempWord);
   }
   else 
      printf("\nWord must contain at least 1 character.\n");
}

/* This function will ask the user for the relation and
   relation value and store it in the clueList member field
   in *oneEntry, as long as the array can still accommodate
   more data and as long as the user still wants to add
   more entries. If there are existing clues already, new
   entries are added (not overwritten). The member field
   numClues is also updated.

   @param oneEntry - address to the structure containing the
                     array of clues and the number of clues,
					      among other data
*/
void 
addTrivia(struct triviaTag * oneEntry)
{
   char choice = 'y', dump;

   while (choice != 'n' && choice != 'N' && oneEntry->numClues < 10)
   {
      printf("\nAdd relation: ");
      getLongString(oneEntry->clueList[oneEntry->numClues].relation);
      if (oneEntry->clueList[oneEntry->numClues].relation[0] != '\0')
      {
         printf("\nAdd relation value: ");
         getLongString(oneEntry->clueList[oneEntry->numClues].relValue);
         if (oneEntry->clueList[oneEntry->numClues].relValue[0] != '\0')
            oneEntry->numClues++;
         else
            printf("\nRelation value must contain at least 1 character.\n");
      }
      else
         printf("\nRelation must contain at least 1 character.\n");

      printf("\nCurrent number of trivia for this word: %d", oneEntry->numClues);

      if (oneEntry->numClues < 10)
      {
         printf("\n\nAdd more trivia? [y/n]: ");
         scanf("%c", &choice);
         scanf("%c", &dump);
      }
   } 
   if (oneEntry->numClues >= 10)
      printf("\nNo more clues can be added.\n");
   
}

/* This function will allow multiple edits to the clues 
   in *oneEntry. The user is allowed to modify multiple 
   clues in *oneEntry, until the user chooses to finish 
   the editing. Only modification can be done. That is, 
   deletion or addition is not allowed here.

   @param oneEntry - address to the structure containing the
                     words, array of clues and the number of clues,
					      among other data
*/
void 
modifyEntry(struct triviaTag * oneEntry)
{
   int nChoice, nClueChoice;
   char dump;
   phrase tempPhrase;
   
   do
   {
      printf("\n\nDisplaying all clues for ""%s"".\n\n", oneEntry->answer);
      displayAllClues(oneEntry->clueList, oneEntry->numClues);
      
      do
      {
         printf("Enter which clue to modify [1 - %d, 0 to exit]: ", oneEntry->numClues);
         scanf("%d", &nChoice);
         scanf("%c", &dump);
      } while (nChoice > oneEntry->numClues || nChoice < 0);
      
      // Modifying clue
      if (nChoice != 0)
      {
         printf("\n%s: %s\n", oneEntry->clueList[nChoice - 1].relation, oneEntry->clueList[nChoice - 1].relValue);
         printf("\n\t[1] - Relation"
                "\n\t[2] - Relation Value");
         do
         {
            printf("\n\nModify [1 - 2]: ");
            scanf("%d", &nClueChoice);
            scanf("%c", &dump);
         } while (nClueChoice != 1 && nClueChoice != 2);
         
         if (nClueChoice == 1)
         {
            printf("\nInput new relation: ");
            getLongString(tempPhrase);
            strcpy(oneEntry->clueList[nChoice - 1].relation, tempPhrase);
         }
         else
         {
            printf("\nInput new relation value: ");
            getLongString(tempPhrase);
            strcpy(oneEntry->clueList[nChoice - 1].relValue, tempPhrase);
         }
      }
   } while (nChoice != 0);
}

/* This function will ask the user to input the word (or phrase)
   that will be deleted.  Calls to other functions, like 
   searchWord(), as needed by the function, will be done.
   If deletion is successful (i.e., the word was found), the
   number of elements in pElem should also be updated.

   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void 
deleteWord(wordList aEntries, int *pElem)
{
   strWord tempWord;
   int i, j, nIndex;

   listWords(aEntries, *pElem);

   printf("\nEnter word to be deleted: ");
   getLongString(tempWord);
   nIndex = searchWord(tempWord, aEntries, *pElem);
   if (nIndex != -1)
   {
      for (i = nIndex; i < *pElem - 1; i++)
         aEntries[i] = aEntries[i + 1];

      // Deleting the last element aEntries[i]
      aEntries[i].answer[0] = '\0';
      for (j = 0; j < aEntries[i].numClues; j++)
      {
         aEntries[i].clueList[j].relation[0] = '\0';
         aEntries[i].clueList[j].relValue[0] = '\0';
      }
      aEntries[i].numClues = 0;
      aEntries[i].use = -1; // use will never be negative, setting it to -1 to indicate unused in gameboard

      // Updating the value of *pElem after deletion
      (*pElem)--;
   }
   else  
      printf("\n""%s"" does not exist in the list.\n", tempWord);
}

/* This function will list the words in aEntries, then
   prompt the user on which word will clues be deleted
   from. If the word exists in aEntries, the function
   will delete the clue specified by the user.

   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void
deleteClue(wordList aEntries, int nElem)
{
   strWord tempWord;
   int i, nIndex, nChoice;
   char dump;

   listWords(aEntries, nElem);

   printf("\nEnter word whose clue(s) will be deleted: ");
   getLongString(tempWord);
   nIndex = searchWord(tempWord, aEntries, nElem);
   if (nIndex != -1)
   {
      printf("\n");
      displayAllClues(aEntries[nIndex].clueList, aEntries[nIndex].numClues);
      printf("\nWhich clue will be deleted? [1 - %d]: ", aEntries[nIndex].numClues);
      scanf("%d", &nChoice);
      scanf("%c", &dump);

      if (nChoice > 0 && nChoice <= aEntries[nIndex].numClues && aEntries[nIndex].numClues != 1)
      {
         for (i = nChoice - 1; i < aEntries[nIndex].numClues - 1; i++)
            aEntries[nIndex].clueList[i] = aEntries[nIndex].clueList[i + 1];
         
         // Deleting the last clue set in aEntries[nIndex].clueList[i]
         aEntries[nIndex].clueList[i].relation[0] = '\0';
         aEntries[nIndex].clueList[i].relValue[0] = '\0';

         aEntries[nIndex].numClues--;
         printf("\nClue has been deleted.\n");
      }
      else
         printf("\nCannot delete clue.\n");
   }
   else  
      printf("\n""%s"" does not exist in the list.\n", tempWord);
}


/* This function displays all information per element in aEntries.
   Part of the solution is to call the other function/s, like 
   viewEntry(), as needed by the function.
 
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void 
viewWords(wordList aEntries, int nElem) 
{
   int nWordIndex;
   char ch, dump;
   strWord key;

   /* Displaying all word answers */
   listWords(aEntries, nElem);

   
   printf("\nView which word: ");

   getLongString(key);
   
   nWordIndex = searchWord(key, aEntries, nElem);
   
   if (nWordIndex != -1)
   {
      viewEntry(aEntries[nWordIndex]);

      /* Scrolling through entries */
      do 
      {
         printf("\nScroll [P - prev, N - next, X - exit]: ");
         scanf("%c", &ch);
         scanf("%c", &dump);
         switch(ch)
         {
            case 'P':
               if (nWordIndex != 0)
               {
                  nWordIndex--;
                  viewEntry(aEntries[nWordIndex]);
               }
               break;

            case 'N':
               if (nWordIndex != nElem - 1)
               {
                  nWordIndex++;
                  viewEntry(aEntries[nWordIndex]);
               }
               break;

            default:
               break;
         }
      } while (ch != 'X');
   }
   else
      printf("\nWord does not exist.\n");
}

/* This function displays all information in entry.
   Part of the solution is to call the other function/s, like 
   displayAllClues(), as needed by the function.
 
   @param entry - structure containing the word and the clues, 
                  among other data
*/
void
viewEntry(struct triviaTag entry)
{
   printf("\nObject: %s", entry.answer);
   printf("\n");
   displayAllClues(entry.clueList, entry.numClues);
}

/* This function displays numClues number of elements from aList.

   @param aList - array of relation and relation values
   @param numClues - number of elements in aList
*/
void
displayAllClues(arrClues aList, int numClues)
{
   int i;

   for (i = 0; i < numClues; i++)
   {
      printf("%s", aList[i].relation);
      printf(": ");
      printf("%s", aList[i].relValue);
      printf("\n");
   }
   printf("\n");
}

/* Given the filename stored in fname, this function
   overwrites the contents of the text file with the contents 
   of the array aEntries. Contents of aEntries and nElem will 
   not change in this function.
 
   @param fname - the string filename, including the extension
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void
export(strWord fname, wordList aEntries, int nElem) 
{
   int i, j;
   FILE * fp;

   fp = fopen(fname, "w");
   
   if (fp != NULL)
   {
      for (i = 0; i < nElem; i++)
      {
         fprintf(fp, "Object: %s\n", aEntries[i].answer);
         for (j = 0; j < aEntries[i].numClues; j++)
         {
            fprintf(fp, "%s: %s\n", aEntries[i].clueList[j].relation, aEntries[i].clueList[j].relValue);
         }
         fprintf(fp, "\n");
      }

      fclose(fp);
   }
   else
      printf("\n%s cannot be opened.\n", fname);  
}

/* This function sorts the contents of aEntries in increasing
   order (based on ASCII value). The solution to this function
   should be an IMPLEMENTATION of a sorting algorithm, NOT by
   calling a sort function from some C library.
   
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void 
sortEntries(wordList aEntries, int nElem)
{
   int i, j, max;

   /* Sorting entries using selection sort */
   for (i = 0; i < nElem - 1; i++)
   {
      max = i;

      for (j = i + 1; j < nElem; j++)
      {
         if (strcmp(aEntries[max].answer, aEntries[j].answer) < 0)
            max = j;
      }
      swapEntries(&aEntries[i], &aEntries[max]); 
   }
}

/* This function swaps all content in triviaTag entry1 with
   triviaTag entry2.

   @param entry1 - the initial entry
   @param entry2 - the entry to be swapped with the initial entry
*/
void
swapEntries(struct triviaTag *entry1, struct triviaTag *entry2)
{
   struct triviaTag tempEntry;

   tempEntry = *entry1;
   *entry1 = *entry2;
   *entry2 = tempEntry;
}


/* This function obtains a string input by storing individual characters
   and returns the string of characters obtained as input. 

   @param str[] - the string variable that will store the string input
*/
void
getLongString(char * str) 
{ 
   char ch;
   int i = 0;

   do
   {
      scanf("%c", &ch);
      if (ch != '\n')
      {  
         str[i] = ch;
         i++;
      } 
   } while (ch != '\n');
   str[i] = '\0';
}

/* This function facilitates the modification of the word 
   (answer member) of the entry *oneEntry, after confirming 
   the new word does not already exist in aEntries.

   @param *oneEntry - the entry whose word will be modified
   @param aEntries - the array containing all entries
   @param nElem - the number of elements in aEntries
*/
void
modifyWord(struct triviaTag *oneEntry, wordList aEntries, int nElem)
{
   strWord tempWord;

   printf("\nEnter new word: ");
   getLongString(tempWord);
   if (tempWord[0] != '\0')
   {
      if (searchWord(tempWord, aEntries, nElem) == -1)
         strcpy(oneEntry->answer, tempWord);
      else  
         printf("\n""%s"" already exists in the list of words.\n", tempWord);
   }
   else
      printf("\nWord must contain at least 1 character.\n");
}