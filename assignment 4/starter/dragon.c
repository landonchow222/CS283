#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "dshlib.h"



// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon()
{
  uint8_t encodedAscii[DRAGON_ASCII_ART_LEN]; // Dragon Logo Encoded Array

  // Set all the values inside the encoded dragon representation to zero
  if (memset(encodedAscii, 0, sizeof(encodedAscii)) == NULL){ printf("Something went Wrong!"); }



  // TODO implement 
  // Uncompressed Print
  // printf("%s", DRAGON_ASCII_ART);

  // Compressed Print
  encode_dragon_ascii(DRAGON_ASCII_ART, encodedAscii);
  decode_dragon_ascii(encodedAscii, DRAGON_ASCII_ART_LEN);
}


int encode_dragon_ascii (char *ascii_art, uint8_t *x) 
{
  int xCounter = 0;                       // Counter for each byte
  int i = 0;                              // Counter for loop through all characters in ascii representation
  char tmpChar;

  while (*(ascii_art + i) != '\0') 
  {
      uint8_t tmpInt = 0;

      // Loop through a byte
      for (int j = 0; j < 4; j++) 
      {

          // For last remaining byte
          if (*(ascii_art + i) == '\0') 
          { 
              // printf("Hello\n");
              tmpInt <<= 2;
              continue; 
          }

          tmpChar = *(ascii_art + i);

          // Left shift by 2 to make space
          tmpInt <<= 2; 

          if (tmpChar == ' ') { tmpInt |= SPACE; } 
          else if (tmpChar == '%') { tmpInt |= PERCENT; } 
          else if (tmpChar == '@') { tmpInt |= AT; } 
          else if (tmpChar == '\n') { tmpInt |= NEWLINE; }
  
          ++i;
      }

      x[xCounter] = tmpInt;
      ++xCounter;
  }

  return xCounter;
}

void decode_dragon_ascii (uint8_t *x, size_t length)
{
  for (size_t i = 0; i < length; i++) 
  {

      uint8_t tmpInt = x[i];
      
      for (int j = 6; j >= 0 ; j -= 2) 
      {
          uint8_t value = (tmpInt >> j) & 0b11;

          if (value == SPACE) { printf(" "); } 
          else if (value == PERCENT) { printf("%%"); } 
          else if (value == AT) { printf("@"); } 
          else if (value == NEWLINE) { printf("\n"); } 
          // else { printf("WHAT"); }
      }
  }

} 