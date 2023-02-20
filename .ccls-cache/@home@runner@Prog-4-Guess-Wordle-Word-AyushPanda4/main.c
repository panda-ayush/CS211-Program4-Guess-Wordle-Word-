//  wordleGuessSecretWord.c
//  Have the program do the guessing to discover the secret wordle word.
//
//  Author: Ayush Panda, 11/20/22
//  System: Replit on MacOS
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
/*  Running the program looks like:
        Using file wordsLarge.txt with 12947 words. 
        -----------------------------------------------------------
        
        Enter a secret word or just r to choose one at random: dream
        Trying to find secret word: 
               d r e a m 
        
            1. s o r e s 
                   * *   
            2. s e r e r 
                 * *     
            3. r a r e r 
               * *   *   
            4. r a r e e 
               * *   *   
            5. s a r e e 
                 * * *   
            6. p a r e r 
                 * * *   
            7. c a r e r 
                 * * *   
            8. b a r e r 
                 * * *   
            9. f a r e r 
                 * * *   
           10. D a r e r 
                 * * *   
           11. D e a r e 
                 * * *   
           12. D e b A r 
                 *     * 
           13. D R E A r 
                       * 
           14. D R E A d 
                         
           15. D R E A M 
                         
        Got it!

     ... and then it runs two more times ...
 */
#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
//#define WORDS_FILE_NAME "wordsLarge.txt"
#define WORDS_FILE_NAME  "wordsTiny.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
};

//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once to
// find out how many words are in the file.
void readWordsFromFile(
        char fileName[],        // Filename we'll read from
        wordCountStruct *words, // Array of words where we'll store words we read from file
        int *wordCount)          // How many words.  Gets updated here and returned
{
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
        words[ *wordCount].score = 0;
        (*wordCount)++;
    }

    // Close the file
    fclose( inFilePtr);
} // end readWordsFromFile(..)


//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
} //end compareFunction(..)

void secondCompare(char secretWord[], char rando[]) {
    char secretWordCopy[6];
    char randoCopy[6];

    // make copy
    char a;
    for (a = 0; secretWord[a]; a++) {
        randoCopy[a] = secretWord[a];
    }

    // make copy
    char b;
    for (b = 0; rando[b] != 0; b++) {
        randoCopy[b] = rando[b];
    }

    // update word
    for (int c = 0; c < 5; c++) {
        if (secretWordCopy[c] == randoCopy[c]) {
            secretWordCopy[c] = '*';
            randoCopy[c] = '*';
        }
    }

    // update word
    for (int d = 0; d < 5; d++) {
        char e = secretWordCopy[d];
        if (e != ' ') {
            for (int f = 0; f < 5; f++) {
                if (e == randoCopy[f]) {
                    secretWordCopy[d] = '-';
                    randoCopy[f] = '-';
                    break;
                }
            }
        }
    }

}

// -----------------------------------------------------------------------------------------
// 1. we need to first use rand to get the first random word to check if there are any matching letters
// 2. if there are matching letters we find other words in the list with those same letters & as more new words are found, we need to find more matching letters until all the letters are the same
// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
    char computerGuess[ 6];  // Allocate space for the computer guess
    wordCountStruct allWordsAnswers[ MAX_NUMBER_OF_WORDS];
    wordCountStruct allWordsWrong[ MAX_NUMBER_OF_WORDS];
    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i=0; i<WORD_LENGTH; i++) {
        printf("%c ", secretWord[ i]);
    }
    printf("\n");
    printf("\n");

    // Loop until the word is found
    int guessNumber = 1;
    int repeat[wordCount];
    for (int i = 0; i < wordCount; i++) {
      repeat[i] = -1;
    }
  
    while( true) {
      
      int randomIndex = rand() % wordCount;
        char randomWord[6];
       strcpy(randomWord, allWords[randomIndex].word);
        char randomWordCopy[6];
        strcpy( randomWordCopy, randomWord);
        char secretWordCopy[6];
        strcpy (secretWordCopy, secretWord);
        // if letter matches in correct position, make it uppercase
        for (int a = 0; a < 6; a++) {
            if (secretWord[a] == randomWord[a]) {
                randomWordCopy[a] = toupper(randomWordCopy[a]);
                secretWordCopy[a] = " ";
             }
        }
        printf("%5d. %c %c %c %c %c\n", guessNumber, randomWordCopy[0], randomWordCopy[1], randomWordCopy[2], randomWordCopy[3], randomWordCopy[4]);

        // if letter matches but isn't in correct position, place a star below
        char emptyString[6] = {' ', ' ', ' ', ' ', ' ', ' '};
        for (int b = 0; b < 6; b++) {
            char c = secretWordCopy[b];
            if (c != ' ') {
                for (int d = 0; d < 5; d++) {
                    if (c == randomWord[d]) {
                        emptyString[d] = '*';
                        break;
                    }
                }
            }
        }
        printf("       %c %c %c %c %c\n", emptyString[0], emptyString[1], emptyString[2], emptyString[3], emptyString[4]);

      //   for (int i = 0; i < wordCount; i++) {
      //     for (int j = 0; j < 6; j++) {
      //       if (isupper(randomWordCopy[j]) && (randomWord[j] == allWords[i].word[j])) {
      //         continue;
      //       }
      //       else {
      //         allWords[i].score = -1;
      //       }
      //     }
      //   }
      //   int tempWordCount = 0;
      //   for (int i = 0; i < wordCount; i++) {
      //     if (allWords[i].score != -1) {
      //         tempWordCount += 1;
      //       }
      //   }
      // for (int i  = 0; i < wordCount; i++) {
      //   if (allWords[i].score == -1) {
      //     strcpy(allWordsWrong[i].word, allWords[i].word);
      //     allWordsWrong[i].score = allWords[i].score;
      //   }
      // }
      // for (int i  = 0; i < wordCount; i++) {
      //   if (allWords[i].score != -1) {
      //     strcpy(allWordsAnswers[i].word, allWords[i].word);
      //    allWordsAnswers[i].score = allWords[i].score;
      //   }
      // }

      // int index = 0;
      // for (int j = 0; j < tempWordCount; j++) {
      //   strcpy(allWords[j].word, allWordsAnswers[j].word);
      //   allWords[j].score = allWordsAnswers[j].score;
      //   index += 1;
      // }
      // for (int j = 0; j < (wordCount-tempWordCount); j++) {
      //   strcpy(allWords[index].word, allWordsWrong[j].word);
      //   allWords[index].score = allWordsWrong[j].score;
      //   index += 1;
      // }
      //   wordCount = tempWordCount;
        // Update guess number
        guessNumber++;
        int compare = strcmp (randomWord, secretWord);
      if(compare == 0) {
        printf("Got it!\n");
        break;
      }
    } //end for( int i...)
} //end findSecretWord


// -----------------------------------------------------------------------------------------
int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct allWords[ MAX_NUMBER_OF_WORDS];
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];
    char userInput[ 81];                // Used for menu input of secret word

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);

    // Run the word-guessing game three times
    for( int i=0; i<3; i++) {
        // Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("\n-----------------------------------------------------------\n");
        printf("\n");
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }
        strcpy( secretWord, userInput);   // Store the secret word from user input

        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }

        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord);
    }

    printf("Done\n");
    printf("\n");
    return 0;
} // end main()