#include <stdint.h>
#include <stdio.h>

#define MAX_INSTRUCTION_SIZE 2

int main(int argc, char *argv[]) {
  FILE *fp;
  uint8_t inst[MAX_INSTRUCTION_SIZE];
  uint8_t op;
  uint8_t dir;
  uint8_t wrd;
  uint8_t mod;
  uint8_t reg;
  uint8_t rm;
  int n;

  // lookup tables
  const char *reg_names_byte[] = {"al", "cl", "dl", "bl", "ah", "ch",
                                  "dh", "bh", "ax", "cx", "dx", "bx",
                                  "sp", "bp", "si", "di"};
  const char *reg_names_word[] = {"ax", "cx", "dx", "bx", "sp", "bp",
                                  "si", "di", "al", "cl", "dl", "bl",
                                  "ah", "ch", "dh", "bh"};

  if (argc != 2) {
    printf("Usage: %s <input file>\n", argv[0]);
    return 1;
  }

  fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    printf("Error opening the file.");
    return 1;
  }

  while ((n = fread(inst, sizeof(uint8_t), MAX_INSTRUCTION_SIZE, fp)) > 0) {
    op = (inst[0] & 0xFC) >> 2;
    dir = inst[0] & 0x2;
    wrd = inst[0] & 0x1;
    mod = (inst[1] & 0xC0) >> 6;
    reg = (inst[1] & 0x38) >> 3;
    rm = inst[1] & 0x07;

    const char *op_name = (op == 0x22) ? "mov" : "invalid!";
    const char *reg_name =
        (wrd == 0) ? reg_names_byte[reg] : reg_names_word[reg];
    const char *rm_name = (wrd == 0) ? reg_names_byte[rm] : reg_names_word[rm];

    if (dir == 0)
      printf("%s %s, %s\n", op_name, rm_name, reg_name);
    else
      printf("%s %s, %s\n", op_name, reg_name, rm_name);
  }

  fclose(fp);
  return 0;
}
