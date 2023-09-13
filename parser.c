#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct Scanner {
  char *tokens[100];
  int current;
};

struct Compiler {
  uint16_t codes[2048];
  int current;
};
bool isHex8Bit(const char* input) {
    // Check if the input string has exactly 2 characters
    if (strlen(input) != 2) {
        return false;
    }

    // Check if each character is a valid hexadecimal digit
    for (int i = 0; i < 2; i++) {
        if (!isxdigit(input[i])) {
            return false;
        }
    }

    return true;
}
bool hexStringToUInt8(const char* hexString, uint8_t* result) {
    if (hexString == NULL || result == NULL) {
        return false; // Invalid input
    }

    // Ensure the input string has exactly 2 characters
    if (strlen(hexString) != 2) {
        return false; // Invalid input length
    }

    // Convert the hexadecimal string to a uint8_t
    if (sscanf(hexString, "%2hhx", result) != 1) {
        return false; // Conversion failed
    }

    return true;
}
int customHexToInt(const char hexString[2]) {
    if (hexString[0] == '\0' || hexString[1] == '\0') {
        return -1; // Empty input string
    }

    int result = 0;

    for (int i = 0; i < 2; i++) {
        char character = hexString[i];

        if ((character >= '0' && character <= '9') ||
            (character >= 'a' && character <= 'f') ||
            (character >= 'A' && character <= 'F')) {

            int intValue;
            if (character >= '0' && character <= '9') {
                intValue = character - '0';
            } else if (character >= 'a' && character <= 'f') {
                intValue = 10 + (character - 'a');
            } else {
                intValue = 10 + (character - 'A');
            }

            result = (result << 4) | intValue;
        } else {
            return -1; // Invalid character
        }
    }

    return result;
}
void importTokens(struct Scanner *s, char *input) {
  const char delimiter[] = " "; // Split on space

  // Initialize an array of pointers to strings
  char *tokens[100]; // Adjust the size according to your needs
  int token_count = 0;

  // Tokenize the input string
  char *token = strtok(input, delimiter);
  while (token != NULL) {
      s->tokens[token_count] = token;
      token_count++;
      token = strtok(NULL, delimiter);
  }
  
  for (int i = 0; i < token_count; i++) {
      printf("Token %d: %s\n", i, s->tokens[i]);
  }
}
void parseToken(struct Compiler *c, char *input) {
  printf("\nITERATION: %d", c->current);
  if (strcmp(input, "ldi") == 0) {
      printf("You selected case 1.\n");
      c->codes[c->current] = 0x0000;
      c->current+=1;
  } else if (strcmp(input, "add") == 0) {
      c->codes[c->current] = 0x1000;
      c->current += 1;
  } else if (strcmp(input, "sub") == 0) {
      printf("You selected case 3.\n");
  } else if (strcmp(input, "jmp") == 0) {
      printf("You selected case 3.\n");
  } else if (strcmp(input, "cjmp") == 0) {
      printf("You selected case 3.\n");
  } else if (strcmp(input, "halt") == 0) {
      c->codes[c->current] = 0xF000;
      c->current += 1;
  } else if (isHex8Bit(input) == true){
      uint16_t h = customHexToInt(input);
      printf("hex %04X\n", h);
      c->codes[c->current] = h;
      c->current+=1;
  }
  else {
    printf("yuh");
  }
}
void compile(struct Compiler *c) {
  uint16_t current = c->codes[c->current];

  uint16_t d = c->codes[c->current + 1] << 8;
  uint16_t kk = c->codes[c->current + 2];
  uint16_t x = c->codes[c->current + 2] << 4;
  uint16_t y = c->codes[c->current + 3] & 0x000F;


  printf("\nCurrent: %04X", current);
  printf("\nd: %04X", d);
  printf("\nkk: %04X", kk);
  printf("\n%04X", x);
  printf("\n%04X", y);
  switch(current) {
    case 0x0000:
      printf("\nFINAL: %04X", current | d | kk);
      c->current += 3;
      break;
    case 0x1000:
      printf("\nFINAL: %04X", current | d | x | y);
      c->current += 4;
      break; 
  }
}
int main() {
  struct Scanner s;
  struct Compiler c;
  uint16_t finalCodes[100];
  s.current = 0;
  int count = 0;
  char input[] = "ldi 01 FF ldi 02 69 add 03 01 02 halt";
  importTokens(&s, input);

  for(int i = 0; i < 11; i++) {
    parseToken(&c, s.tokens[i]);
    count+=1;
  }
  for(int i = 0; i < 10; i++) {
    printf("\n%04X\n", c.codes[i]);
  }
  c.current = 1;

  while(c.codes[c.current] != 0xF000) {
    compile(&c);
  }
    // Now, tokens[] contains the split strings
  printf("\n%d", c.current);
  return 0;
}
