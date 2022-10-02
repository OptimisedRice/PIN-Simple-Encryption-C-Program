/*  
    Description: A C Program implementing a security authentication-like functionality.
    The user can enter a code, encrypt it and see if it matches the authorised code, decrypt it, check successful and unsuccessful matches and then exit if needed.
    There is also validation in place to recognise incorrect data type/range input with appropriate ERROR messages. The no. of digits in the PIN can be changed easily. 
    Compiled with g++ via MinGW on Visual Studio Code on Windows 10
*/
// Author: Kieran Silada
// Date: 3/03/21

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//symbolic name for array DIGIT (no. of digits) & SIZE to accomdate the NULL character for strings
#define DIGIT 4
#define SIZE DIGIT + 1

//function signatures for the different options
void input(char *, int *);
int encrypt(char *, const char *, int *, int *);
int decrypt(char *);
void display(int *, int *);
void exitprog(char *);

int main(void)
{
    int correctcounter, incorrectcounter, option, e, d, p;
    char exit, floats, PIN[SIZE] ={"0000"}; 
    const char access_code[SIZE] = {"4523"};

    //initialising encryption states
    e = 0;
    d = 1;

    //input state 
    p = 0;
    correctcounter = 0;
    incorrectcounter = 0;
    exit = 'n';

    //do while loop to have the program continuously running
    do
    {
        //initialise option to be zero so no option is selected
        option = 0;
        floats = 0;
        //Prints the program options display and asks the user to select an option
        printf("\n\n\tSecurity Authentication\nEnter your selection:");
        printf("\n1.  Enter any code");
        printf("\n2.  Encrypt code entered and verify if correct");
        printf("\n3.  Decrypt code");
        printf("\n4.  Display the number of times the encrypted code entered matches the authorised code (i) Successfully (ii) Unsuccessfully");
        printf("\n5.  Exit Program\n\n");
        
        //clears the buffer of any previous input. this is to insure our scanf's don't read input from our earlier or outer loops
        fflush(stdin);

        scanf("%d", &option);
        //character validation which reads the newline character and triggers if extra characters are inputted
        floats = getchar();
        if (floats != '\n')
        {
            option = 6; //skips the cases and goes to default
        }
        fflush(stdin);
        switch(option)
        {
            case 1:
            {
                //set encryption states incase user changes code again
                e = 0;
                d = 1;
                p = 1;
                input(PIN, &p);
                break;
            }

            case 2:
            {
                //checks for input state
                if(p == 0)
                {
                    printf("ERROR no code inputted");
                    break;
                }
                //checks for encryption state
                if(e == 1)
                {
                    printf("\nERROR input code is already encrypted, please decrypt first\n");
                }
                else
                {
                    e = encrypt(PIN, access_code, &correctcounter, &incorrectcounter);
                    //set encryption to true then set decryption to false
                    d = 0;
                }
                break;
            }

            case 3:
            {
                //checks for input state
                if(p == 0)
                {
                    printf("ERROR no code inputted");
                    break;
                }
                //checks for encryption state
                if(d == 1)
                {
                    printf("\nERROR input code is already decrypted, please encrypt first\n");
                }
                else
                {
                    d = decrypt(PIN);
                    //set decryption to true then set encryption to false
                    e = 0;
                }
                break;
            }

            case 4:
            {
                display(&correctcounter, &incorrectcounter);
                break;
            }

            case 5:
            {
                exitprog(&exit);
                break;
            }

            default:
            {
                printf("ERROR - please input whole numbers only between 1 and 5 inclusive\n");
            }
        }
    } 
    while (exit != 'y');

    printf("\nThe program has exited successfully");
    return 0;
}

void input(char *P, int *code) //reading input from the user
{
    //asks for user input
    printf("\nInput your %d-digit code:\n", DIGIT);
    for(int i = 0; i < DIGIT; i++)
    {
        scanf("%1s", &*(P + i));

        //validation for numbers only
        //ASCII decimal value range for numeric characters
        if(*(P + i) < 48 || *(P + i) > 57)
        {
            printf("ERROR input only numbers 0 through 9 for each digit. Returning to menu....");
            *code = 0;
            return;
        }
        
    }
    return;
}

int encrypt(char *P, const char *ac, int *c, int *ic) //encrypts the inputted code and compares it to the access_code array
{
    int result;
    //value n for finding the first element index of the second half of the array
    //ceil function deals with odd digits
    int n = ceil((double)DIGIT/2);
    //temp variable for swap
    char t;
    for(int i = 0;i < floor(DIGIT/2); i++)
    {
        //swaps the i element in the 1st half with the corresponding i element in the 2nd half and increments each by 1
        t = *(P + i);
        *(P + i) = *(P + i + n) + 1; 
        *(P + i + n) = t + 1;

        //if it goes above the range, it sets to zero
        if(*(P + i) == 10)
        {
            *(P + i) = 0;
        }
        if(*(P + i + n) == 10)
        {
            *(P + i + n) = 0;
        }
    }

    //comparing the PIN string and access_code string & storing the returned int
    result = strcmp(P, ac);

    if(result == 0)
    {
        printf("\nAccess granted. The encrypted input code matches the authorised access code.\n");
        *c += 1;
        return 1;
    }
    else
    {
        printf("Access denied. The encrypted input code does not match the authorised access code.\n");
        *ic += 1;
        return 1;
    }
}

int decrypt(char *P) // decrypts the encrypted code back to the original inputted code
{
    //value n for finding the first element index of the second half of the array
    //ceil function deals with odd digits
    int n = ceil((double)DIGIT/2);
    //temp variable for swap
    char t;
    for(int i = 0;i < floor(DIGIT/2); i++)
    {
        //swaps the i element in the 1st half with the corresponding i element in the 2nd half and decrements each by 1
        t = *(P + i);
        *(P + i) = *(P + i + n) - 1; 
        *(P + i + n) = t - 1;

        //if it goes below the range, it sets to 9
        if(*(P + i) == -1)
        {
            *(P + i) = 9;
        }
        if(*(P + i + n) == -1)
        {
            *(P + i + n) = 9;
        }
    }
    printf("\nCode decrypted: %s\n", P);
    return 1;
}

void display(int *right, int *wrong) //displays the number of times the 2 codes matched and unmatched
{
    printf("The encrypted code matched the authorised access code %d times and unmatched %d times",*right, *wrong);
    return;
}

void exitprog(char *e) //asks the user for exit confirmation and ends the program or returns to the menu
{
    char answer, verif;

    do
    {
        //flushes the input buffer so scanf doesn't read anything from a previous loop
        fflush(stdin);
        //asks the user for exit confirmation
        printf("Are you sure you want to exit the program? (y/n)");
        scanf("%1s", &answer);
        verif = getchar();
        
        //forces answer to be lowercase;
        answer |= 32;

    } while(verif != '\n'); //checks for extra inputted characters
    //makes the do while loop condition false to break out of it
    *e = answer;
    return;   
}