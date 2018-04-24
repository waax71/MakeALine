# makealine

## Synopsis: 
makealine [FIRSTNUMBER] FINALNUMBER STRING

## Description: 
Makealine is a little tool that takes a string and
creates a text file "out.txt" where it replaces the wildcards #*
or #? with numbers.

### Example: 
makealine 1 4 "Register#* = 0xFF;"

    Register1 = 0xFF;

    Register2 = 0xFF;

    Register3 = 0xFF;

    Register4 = 0xFF;


FIRSTNUMBER is the number makealine starts with. If FIRSTNUMBER is
omitted then it starts with one. Makealine replaces the wildcards
with increasing numbers until it reaches FINALNUMBER. This is the
last number.
STRING is a string that has to contain either the #* or the #?
wildcards.


The #* wildcard:

for every occurrence in the line the wildcard gets replaced with
the same number. In the next line the number gets increased.


### Example: 
makealine 1 4 "Register#* = #*;"

    Register1 = 1;
    
    Register2 = 2;
    
    Register3 = 3;
    
    Register4 = 4;
    
    
    
  The number of asterisks gives the number of leading zeros.
  
  
  ### Example: 
  makealine 1 4 "Register#*** = #**;"
    
    Register001 = 01;
    
    Register002 = 02;
    
    Register003 = 03;
    
    Register004 = 04;

 
 
The #? wildcard:

  for every occurrence in the line the wildcard gets replaced with
  an increasing number.
  
  Example: makealine 1 4 "Register#? = #?;"
  
    Register1 = 2;
    
    Register3 = 4;
    
    Register5 = 6;
    
    Register7 = 8;
    
  The number of questionmarks gives the number of leading zeros.
  
  
  ### Example: 
  makealine 1 4 "Register#??? = #??;"
  
    Register001 = 02;
    
    Register003 = 04;
    
    Register005 = 06;
    
    Register007 = 08;
    
