#include <stdio.h>

void print_bits(int num) {
  for ( int i = sizeof(num) - 1; i >= 0; i -= 1 ) {
    printf("%d", (num & (1 << i)) ? 1 : 0);
  }
}

char* decode_register(int W, int REG) {
  if (W == 0) {
    switch (REG) {
      case 0b000: return "al";
      case 0b001: return "cl";
      case 0b010: return "dl";
      case 0b011: return "bl";
      case 0b100: return "ah";
      case 0b101: return "ch";
      case 0b110: return "dh";
      case 0b111: return "bh";
    }
  } else {
    switch (REG) {
      case 0b000: return "ax";
      case 0b001: return "cx";
      case 0b010: return "dx";
      case 0b011: return "bx";
      case 0b100: return "sp";
      case 0b101: return "bp";
      case 0b110: return "si";
      case 0b111: return "di";
    }
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    perror("\n");
    printf("Improper number of arguments: %d", argc);
    return 1;
  }

  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Could not open file:");
    printf("%s\n", argv[1]);
    return 1;
  }

  printf("bits 16\n\n");
  char b;

  while ((b = fgetc(file)) != EOF) {

    /* Check for mov */
    if ((b & 0b11111100) == 0b10001000) {
      printf("mov ");

      /* int D = (b >> 1) & 1; */
      int W = b & 1;

      b = fgetc(file);

      /* int mod = b >> 6; */
      int reg = (b & 0b00111000) >> 3;
      /* int reg = (b << 2) >> 5; */
      int r_m  = b & 0b00000111;

      printf("%s, ", decode_register(W, r_m));
      printf("%s\n", decode_register(W, reg));
    }
  }

  fclose(file);

  return 0;
}
