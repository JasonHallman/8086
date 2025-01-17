#include <stdio.h>
#include <string.h>

void print_bits(int num) {
  for ( int i = sizeof(num) - 1; i >= 0; i -= 1 ) {
    printf("%d", (num & (1 << i)) ? 1 : 0);
  }
}

void print_regs(int D, char* reg, char* r_m) {
  if (D == 0) {
    printf("%s, %s\n", reg, r_m);
  } else {
    printf("%s, %s\n", r_m, reg);
  }
}

/* Decode effective address calculation */
char* decode_ea(int r_m) {
  switch (r_m) {
    case 0b000: return "bx + si";
    case 0b001: return "bx + di";
    case 0b010: return "bp + si";
    case 0b011: return "bp + di";
    case 0b100: return "si";
    case 0b101: return "di";
    case 0b110: return "bp";
    case 0b111: return "bx";
  }
  return "ERROR";
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
  return "ERROR";
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

    /* Register to Register/Memory */
    if ((b & 0b11111100) == 0b10001000) {
      printf("mov ");

      int D = (b >> 1) & 1;
      int W = b & 1;

      b = fgetc(file);

      int mod = b >> 6;
      int reg = (b & 0b00111000) >> 3;
      int r_m = b & 0b00000111;

      if (mod == 0b11) {
        print_regs(D, decode_register(W, reg), decode_register(W, r_m));

      } else {
        char* ea_calc = decode_ea(r_m);

        if (mod == 0b01) {
          char disp_lo = fgetc(file);

          char ea_string[1 + strlen(ea_calc) + 3 + 1 + 2];

          snprintf(ea_string, sizeof(ea_string), "[%s + %d]", ea_calc);

          print_regs(D, decode_register(W, reg), ea_string);

        } else if (mod == 0b10) {
          char disp_lo = fgetc(file);
          char disp_hi = fgetc(file);



        } else if (mod == 0b00) {
          if (r_m == 0b110) {

          }

        } else {
          printf("Invaild mod: ");
          print_bits(mod);
          return 2;
        }
      }

    /* Immediate to Register/Memory */
    } else if ((b & 0b11111110) == 0b11000110) {
      printf("mov ");

      int W = b & 1;

      b = fgetc(file);

      int mod = b >> 6;
      int r_m = b & 0b00000111;


    /* Immediate to Register */
    } else if ((b & 0b11110000) == 0b10110000) {
      printf("mov ");

      int W = (b >> 3) & 1;
      int reg = (b & 0b00000111);

      printf("%s, ", decode_register(W, reg));
      
      if (W == 1) {
        char hi = fgetc(file);
        char lo = fgetc(file);

        int16_t immediate = (hi << 8) | lo;

        printf("%d\n", immediate);

      } else {
        printf("%d\n", fgetc(file));
      }
    /* Memory to Accumulator */
    } else if ((b & 0b11111110) == 0b10100000) {
      int W = b & 1;

    /* Accumulator to Memory */
    } else if ((b & 0b11111110) == 0b10100010) {
      int W = b & 1;

    }
  }

  fclose(file);

  return 0;
}
