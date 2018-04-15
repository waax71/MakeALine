/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Axel
 *
 * Created on 23. Juli 2017, 20:14
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_STRING_SIZE         256
#define CHAR_HASH               35
#define CHAR_STAR               42
#define CHAR_QMARK              63
#define CHAR_END                '\0'


/*
char Input[] = "#****. Axel lives in #*. Street"; //#define INITVAL_REG_##***                 0xFF";
*/

char *gp_Input = NULL; 

//char Input[] = "#define INITVAL_REG##***                 0xFF;";
//char Input[] = "Initval #???, Initval #???, Initval #???, Initval #???, Initval #???;";
char Output[] = "";

typedef enum {START, NO_HASH, HASH, STAR, ONE_STAR, MORE_STARS, QMARK, ONE_QMARK, MORE_QMARKS, END} States_t;

int16_t g_StartValue = 1;
int16_t g_EndValue = 10;
int16_t g_ValueToInsert;
FILE *fp;



/* declare functions */
uint16_t convertLine(char*, uint16_t, uint16_t);
uint16_t raisingTenToPowerOf(uint16_t);
uint16_t checkCmdlineArguments(int, char **);
int16_t convertStrToVal(char *);
void showErrorMessage(void);
void showHelpMessage(void);



int main(int argc, char** argv) 
{
    g_ValueToInsert = g_StartValue;
        
    // check command line arguments 
    checkCmdlineArguments(argc, argv);
    
    // start
    fprintf(fp, "\nmakealine:\n");
    
    // open file 
    fp = fopen("out.txt", "w");

    // loop until all lines are written 
    do 
    {
        g_ValueToInsert = convertLine(gp_Input, g_ValueToInsert, g_EndValue);
        fprintf(fp, "\n");
    }while (g_ValueToInsert <= g_EndValue);
    
    // close file 
    fclose(fp);
    return (EXIT_SUCCESS);

}

uint16_t checkCmdlineArguments(int NumberOfArguments, char** p_Argument)
{
    char** p_ShowArguments;   
    int ShowNumbOfArg;
    
    p_ShowArguments = p_Argument;
    ShowNumbOfArg = NumberOfArguments;
    
    /* check command line arguments */
    switch (NumberOfArguments)
    {
        case 1:
        {
            showErrorMessage();
            exit(EXIT_FAILURE);
            break;
        }
        
        case 2:
        {
            if (strcmp((*(p_Argument+1)), "--help") == 0)
            {
                // display help info 
                showHelpMessage();
                exit (EXIT_SUCCESS);
            }
            else
            {
                showErrorMessage();
                exit(EXIT_FAILURE);
            }
            break;
        }
        
        case 3:
        {
            // convert 1. argument to EndValue 
            g_EndValue = convertStrToVal(*(p_Argument+1));
            if (g_EndValue < 0)
            {
                showErrorMessage();
                exit(EXIT_FAILURE);
            }
            else
                g_StartValue = 1;                       
                
            // convert 2. argument to LineToConvert 
            gp_Input = *(p_Argument+2);            
            break;
        }
        
        case 4:
        {
            /* convert 1. argument to StartValue */
            g_StartValue = convertStrToVal(*(p_Argument+1));
            if (g_StartValue < 0)
            {
                showErrorMessage();
                exit(EXIT_FAILURE);
            }
            
            /* convert 2. argument to EndValue */
            g_EndValue = convertStrToVal(*(p_Argument+2));
            if (g_EndValue < 0)
            {
                showErrorMessage();
                exit(EXIT_FAILURE);
            }
            
            /* make sure that second argument is bigger than first */
            if(g_EndValue <= g_StartValue)
            {
                showErrorMessage();
                exit(EXIT_FAILURE);                
            }
            
            /* convert 3. argument to LineToConvert */
            gp_Input = *(p_Argument+3);
            break;
        }
        
        default:
            showErrorMessage();
            break;            
    }
}



int16_t convertStrToVal(char * StrToConvert)
{
    int16_t Result;
    
    Result = strtol(StrToConvert, NULL, 10);
    
    if (Result == 0)
    {
        if (*StrToConvert == '0')
            Result = 0;
        else
            Result = -1;
    }
    return Result;
}


uint16_t convertLine(char *p_LineToConvert, uint16_t Number, uint16_t LastNumber)
{
    States_t State;
    char *p_Character;
    char *p_NextCharacter;
    uint16_t DecimalPlaceCntr;
    uint16_t IsZero;
    States_t LastState = START;
    
    /* init state machine */
    State = START;                              /* set state */
    p_Character = p_LineToConvert;              /* set pointer to begining of line*/

    
    /* loop in state machine until all characters are read */
    do
    {   
        /* switch state */
        switch (State)
        {
            /* start */
            case START:
                DecimalPlaceCntr = 1;               /* reset counter */
                if (*p_LineToConvert == CHAR_HASH)
                    State = HASH;
                else
                    State = NO_HASH;
                break;
            
                
            /* no hash */
            case NO_HASH:
                DecimalPlaceCntr = 1;               /* reset counter */
                fprintf(fp, "%c", *p_Character);         /* print character */
                p_Character++;                      /* move to next character */
                if (*p_Character == CHAR_END)       /* is end of line reached? */
                    State = END;                
                if (*p_Character == CHAR_HASH)      /* if next char is hash, change state */
                    State = HASH;
                break;
                
                
            /* hash */
            case HASH:
                LastState = HASH;
                p_NextCharacter = p_Character;      /* see what next character is */
                p_NextCharacter++;  

                switch (*p_NextCharacter) {
                                        
                    case CHAR_STAR:
                        State = STAR;
                    break;
                                        
                    case CHAR_QMARK:
                        State = QMARK;
                    break;

                    default:
                        State = NO_HASH;
                    break;                                                                                
                }               
                break;

                
            /* star */
            case STAR:
                p_NextCharacter++;  
                
                switch (*p_NextCharacter) {
                                        
                    case CHAR_STAR:       
                        DecimalPlaceCntr++;
                        State = STAR;
                    break;
                        
         
                    default:
                        /* next char is not a star, write number*/
                        if (DecimalPlaceCntr == 1){
                        /* write number without leading zeros */
                            fprintf(fp, "%u", Number);    
                            p_Character += 2;
                            State = NO_HASH;                            
                        }
                        else{
                        /* write leading zeros first, then number */
                            uint16_t MoveNumberOfPositions;
                            MoveNumberOfPositions = DecimalPlaceCntr;
                            do {
                                IsZero=(Number/raisingTenToPowerOf((DecimalPlaceCntr-1)));
                                if (IsZero == 0){
                                    fprintf(fp, "0");
                                    DecimalPlaceCntr--;
                                }
                                else{
                                    fprintf(fp, "%u", Number);
                                    DecimalPlaceCntr = 0;
                                }  
                            }while (DecimalPlaceCntr != 0);
                            p_Character += MoveNumberOfPositions+2;
                            State = NO_HASH;  
                        } 
                    break;                                                                                
                }               
                break;                
                
                
            /* question mark */
            case QMARK:
                LastState = QMARK;
                p_NextCharacter++;  
                
                switch (*p_NextCharacter) {
                                        
                    case CHAR_QMARK:       
                        DecimalPlaceCntr++;
                        State = QMARK;
                    break;                

                    
                   default:
                        /* next char is not a star, write number*/
                        if (DecimalPlaceCntr == 1){
                        /* write number without leading zeros */
                            fprintf(fp, "%u", Number);    
                            p_Character += 2;
                            State = NO_HASH; 
                            Number++;
                        }
                        else{
                        /* write leading zeros first, then number */
                            uint16_t MoveNumberOfPositions;
                            MoveNumberOfPositions = DecimalPlaceCntr;
                            do {
                                IsZero=(Number/raisingTenToPowerOf((DecimalPlaceCntr-1)));
                                if (IsZero == 0){
                                    fprintf(fp, "0");
                                    DecimalPlaceCntr--;
                                }
                                else{
                                    fprintf(fp, "%u", Number);
                                    DecimalPlaceCntr = 0;
                                    Number++;
                                }  
                            }while (DecimalPlaceCntr != 0);
                            p_Character += MoveNumberOfPositions+2;
                            State = NO_HASH;  
                        }
                    break;                                                                                
                }               
                break;                 
        }   
        /* check if all numbers are printed, only for QMARKS */
        if (Number == LastNumber+1)
            State = END;
        
    }while (State != END);
 
    if (LastState == QMARK)
        Number--;
    
    return ++Number;
}


uint16_t raisingTenToPowerOf(uint16_t Exponent)
{
    uint16_t i;
    uint16_t Result = 1;
    
    if (Exponent>4)
        return 0;
    
    for (i=1; i<=Exponent; i++)
        Result *= 10;
    
    return Result;
}


void showErrorMessage(void)
{
    printf("\ninvalid argument(s) - check with '--help' for correct line arguments.\n");
}


void showHelpMessage(void)
{

    printf("Name: ");   
    printf("makealine \n");
    printf("\n");
    
    printf("Synopsis: ");
    printf("makealine [FIRSTNUMBER] FINALNUMBER STRING");
    printf("\n\n");
    
    printf("Description: Makealine is a little tool that takes a string and\n");
    printf("creates a text file \"out.txt\" where it replaces the wildcards #* \n");
    printf("or #? with numbers.\n"); 
    printf("\n");
    
    printf("Example: makealine 1 4 \"Register#* = 0xFF;\"\n");
    printf("  Register1 = 0xFF;\n");
    printf("  Register2 = 0xFF;\n");
    printf("  Register3 = 0xFF;\n");
    printf("  Register4 = 0xFF;\n");
    printf("\n"); 
    
    printf("FIRSTNUMBER is the number makealine starts with. If FIRSTNUMBER is\n");
    printf("omitted then it starts with one. Makealine replaces the wildcards\n");
    printf("with increasing numbers until it reaches FINALNUMBER. This is the \n");
    printf("last number.\n");
    printf("STRING is a string that has to contain either the #* or the #? \n");
    printf("wildcards.\n");   
    printf("\n");
    
    printf("The #* wildcard:\n");
    printf("  for every occurrence in the line the wildcard gets replaced with \n");
    printf("  the same number. In the next line the number gets increased.\n");
    printf("  Example: makealine 1 4 \"Register#* = #*;\"\n");
    printf("    Register1 = 1;\n");
    printf("    Register2 = 2;\n");
    printf("    Register3 = 3;\n");
    printf("    Register4 = 4;\n");
    printf("  The number of asterisks gives the number of leading zeros. \n");
    printf("  Example: makealine 1 4 \"Register#*** = #**;\" \n");
    printf("    Register001 = 01;\n");
    printf("    Register002 = 02;\n");
    printf("    Register003 = 03;\n");
    printf("    Register004 = 04;\n");
    printf("\n");  

    printf("The #? wildcard:\n");
    printf("  for every occurrence in the line the wildcard gets replaced with\n");
    printf("  an increasing number.\n");
    printf("  Example: makealine 1 4 \"Register#? = #?;\" \n");
    printf("    Register1 = 2;\n");
    printf("    Register3 = 4;\n");
    printf("    Register5 = 6;\n");
    printf("    Register7 = 8;\n");
    printf("  The number of questionmarks gives the number of leading zeros.\n");
    printf("  Example: makealine 1 4 \"Register#??? = #??;\" \n");
    printf("    Register001 = 02;\n");
    printf("    Register003 = 04;\n");
    printf("    Register005 = 06;\n");
    printf("    Register007 = 08;\n");
}