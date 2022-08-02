/****************************************************************************************
* 
*   Example word = Zonal (solution) not Case Sensitive
*   User Try 1:     break
*   program output: ---*-
*   User Try 2:     rolax
*   program output: -*?*-
*   User Try 3:     sonar
*   program output: -***-
*   User Try 4:     zonal
*   program output: *****   Correct! You Win!!
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include<string.h>



//Memory cache:
//0-25 File position in the complete list with words that start with the given letter a-z
//26: Number of words in the solution list

#define NUM_SOLUTIONS 2315

char random_solution[6];
int printMenuReturnChoice(void);
bool setup(FILE ** fptDic, FILE ** fptSol);
void readWordFromFile(unsigned int index, char * resWord, FILE * fpt);
void printInstructions(void);
void gamePlay(FILE * fptDict, FILE * fptSol);
void toLowerCase(char * str);

int main()
{
    FILE * fptDictionary = NULL;
    FILE * fptSolutions = NULL;
    setup(&fptDictionary, &fptSolutions);
    int userChoice = 2;

    do
    {
        userChoice = printMenuReturnChoice();

        switch(userChoice)
        {
            case 1:
                gamePlay(fptDictionary, fptSolutions);
                break;
            case 2:
                printInstructions();
                break;
            case 3:
                printf("Exiting the game!\n");
                return(0);
            break;
            default:
                printf("\nEnter a valid choice!\n");
        }
    }
    while(userChoice != 3);
    printf("Exiting the game!\n");
    char testWord[6] = {0};
    for(int i=0; i<100; i++)
    {
        readWordFromFile(i, testWord,fptDictionary);
        printf("The word at index %d is: ", i);
        puts(testWord);
    }


    return 0;
}
/***** Take input from user and convert to lower case *******/
void Input(char *playerGuess, int tries)
{

    printf("\nTry %d: ", tries + 1);
    gets(playerGuess);
    toLowerCase(playerGuess); 


}
/**** CHECK IF PLAYER GUESS PRESENT IN DICTIONARY ********/
int Dict_Compare(char *PlayerGuess, FILE *fptDict)
{

    int dictIndex = 0;
    char dictWord[5];
    int Word_match;
    int flag = 0;

    while(dictIndex != 12946)
    {
        readWordFromFile(dictIndex, dictWord, fptDict);
        Word_match = strcmp(PlayerGuess, dictWord); // match would be 0 if the two strings are equal
        if(Word_match == 0)
        {
            flag = 1;
            return 1;
            break;
        }
        else
        {
            dictIndex++;
        }
    }

    if(flag == 0)
    {
        printf("\nWord Not Found in Dictionary. Try Again");
        return 0;
    }
}

/******* CHECK IF PLAYER GUESS IN SOLUTION ********/

bool Solution_Checker(char *sol, char *guess)
{

    char order[6] = {'-', '-', '-', '-', '-', '\0'};

    /***** For all correct matches *****/

    for(int i=0;i<5;i++)
    {
        if(guess[i] == sol[i])
        {
            order[i] = '*';
        }
    }

    /**** For matches at different locations */
    for(int i=0;i<5;i++)
    {
        if(order[i] == '-')
        {
            for(int j=0;j<5;j++)
            {
                if(guess[i] == sol[j])
                {
                    order[i] = '?';
                    break; // Leaving J loop because we dont want multiple instances of same word
                }
            }
        }
    }


    printf("%s", order);
    return strcmp(order, "*****") == 0;

}


void gamePlay(FILE * fptDict, FILE * fptSol)
{

    char solutionWord[6];
    char playerGuess[6];
    /** Get a random index into the solutions file */
    unsigned int solIdx = rand()% NUM_SOLUTIONS;
    readWordFromFile(solIdx, solutionWord,fptSol);
    //printf("The word at index %d is: ", solIdx);
    //puts(solutionWord);

    /** Get input from the player **/

    //puts(solutionWord);
    int no_of_guess = 0;
    bool guessed_correctly = false;

    while(no_of_guess < 6 && guessed_correctly == false)
    {
        // Input from user in function
        Input(&playerGuess, no_of_guess);
        // Check if length is valid
        int length = strlen(playerGuess);

        if(length != 5)
        {
            printf("\nEnter 5 letter Word\n");
            // Input(&playerGuess, no_of_guess);
            // length = strlen(playerGuess);
            continue;
        }

    /**************************************************************/
    /*************** CODE TO CHECK WORD IN DICTIONARY *************/

        int result = Dict_Compare(&playerGuess, fptDict);

        if(result == 1)
        {
            no_of_guess++;

            /************ CODE TO CHECK PLAYER GUESS IN SOLUTION ******/

            guessed_correctly = Solution_Checker(&solutionWord, &playerGuess);
            if(guessed_correctly)
            {
                printf("\n\nCONGRATULATIONS!!! You Guessed Correctly\n\n");
                break;
            }

        }


        if(result == 0)
        {
            continue;
        }


    }
    if(no_of_guess >= 6)
    {

        printf("\nYou are out of Guesses. \nTry Again\n\n\n");

    }

}

bool setup(FILE ** fptDic, FILE ** fptSol)
{
    srand(time(0));
    if((*fptDic = fopen("data\\dictionary.txt", "r")) == NULL)
    {
        printf("Failed to open the dictionary file!\n");
        return (false);
    }

    if((*fptSol = fopen("data\\solutions.txt", "r")) == NULL)
    {
        printf("Failed to open the solutions file!\n");
        return (false);
    }
    return(true);
}

int printMenuReturnChoice(void)
{
    printf("****************************************************************\n");
    printf("Welcome to text based Wordle game.\n\n");
    printf("Enter your choice:\n\n");
    printf("1. New game.\n");
    printf("2. Instructions.\n");
    printf("3. Exit.\n");
    int choice;
    scanf("%d", &choice);
    fflush(stdin);
    printf("\n\n");
    return(choice);
}
void printInstructions(void)
{
    printf("\n");
    printf("*************************************************************************\n");
    printf(" The Wordle game-play requires the player to guess a Five-letter word in\n");
    printf(" six tries. A try is valid if the user enters a 5 letter word that is\n");
    printf(" present in the dictionary. All the letters that are in their\n");
    printf(" correct place (compared to solution) are replaced by a '*' and all the\n");
    printf(" letters that are present in the solution but are out of place are replaced by\n");
    printf(" a '?'. A letter that is absent in the solution is replaced with a hyphen '-'.\n\n");
    printf(" Example word = Zonal (solution) not Case Sensitive.\n");
    printf(" User Try 1:     break\n");
    printf(" program output: ---*-\n");
    printf(" User Try 2:     rolax\n");
    printf(" program output: -*?*-\n");
    printf(" User Try 3:     sonar\n");
    printf(" program output: -***-\n");
    printf(" User Try 4:     zonal\n");
    printf(" program output: *****   Correct! You Win!!\n");

}

void readWordFromFile(unsigned int index, char * resWord, FILE * fpt)
{
    unsigned int offset = (index * 7);
    fseek(fpt, offset, SEEK_SET);
    fgets(resWord, 6, fpt);
}

void toLowerCase(char * str)
{
    int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
        {
			str[i] += 32; //Convert to lowercase
		}
		i += 1;
	}
}
