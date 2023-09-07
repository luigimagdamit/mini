#include <stdio.h>
#include <stdint.h> 
#include <time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

#define REGISTER_LIM 16
#define IM_LIM 64
struct CPU {
  uint8_t REGISTER[REGISTER_LIM];
  uint16_t pc;

  uint16_t im[IM_LIM];
  uint8_t LAST_ALU;
  // INSTRUCTION MEMORY
};

//0XNdkk
typedef struct opcode {
  uint16_t inst;
  uint16_t d;
  uint16_t kk;
  uint16_t nnn;

  uint16_t x;
  uint16_t y;
} opcode;
// ALU ==================================================
void load(struct CPU *c, uint8_t dest, uint8_t val) {
  c->REGISTER[dest] = val;
}
// 0x0000
void add(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  
  uint8_t *REG = c->REGISTER;
  printf("ADD R[%04X] + R[%04X] - %04X + %04X = %04X", a, b, REG[a], REG[b], REG[a] + REG[b]);
  c->REGISTER[dest] = REG[a] + REG[b];
  c->LAST_ALU = c->REGISTER[dest];
}
// 0x1000
void sub(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;
  printf("SUB R[%04X] - R[%04X] - %04X - %04X = %04X", a, b, REG[a], REG[b], REG[a] - REG[b]);
  c->REGISTER[dest] = REG[a] - REG[b];
  c->LAST_ALU = c->REGISTER[dest];
}
// 0x2000
void orr(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = REG[a] | REG[b];
}
// 0x3000
void nor(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = ~(REG[a] | REG[b]);
}
// 0x4000
void and(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = (REG[a] & REG[b]);
}
// 0x5000
void xor(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = (REG[a] ^ REG[b]);
}
// 0x6000
void inc(struct CPU *c, uint8_t dest, uint8_t a) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = (REG[a] + 0x01);
}
// 0x7000
void dec(struct CPU *c, uint8_t dest, uint8_t a) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = (REG[a] - 0x01);
}
// 0x8000
void rsh(struct CPU *c, uint8_t dest, uint8_t a) {
  uint8_t *REG = c->REGISTER;

  c->REGISTER[dest] = (REG[a] >> 0x01);
}
// 0x9000
// END OF ALU =========================================
opcode createOpcode(uint16_t hex) {
  opcode op;
  op.inst = hex & 0xF000;
  op.nnn = hex & 0x0FFF;
  op.d = (hex & 0x0F00) >> 8;
  op.kk = hex & 0x00FF;
  op.x = (hex & 0x00F0) >> 4;
  op.y = hex & 0x000F;
  return op;
}
void execute(struct CPU *c, uint16_t hex) {

  c->pc += 1;
  opcode op = createOpcode(hex);
  printf("\n%04X", op.inst);
  printf("\nNNN: %04X", op.nnn);
  switch(op.inst) {
    case(0x000):
      load(c, op.d, op.kk);
      break;
    case(0x1000):
      add(c, op.d, op.x, op.y);
      break;
    case(0x2000):
      sub(c, op.d, op.x, op.y);
      break;
    case(0xA000):
      printf("\nBEFORE %04X", c->pc);
      c->pc = op.nnn;
      printf("\nAFTER %04X", c->pc);
      break;
    // JUMP nnn
    case(0xB000):
      if(c->LAST_ALU == 0) {
        printf("\nB000 NNN: %04X", op.nnn);
        c->pc = op.nnn;
      }
      break;
    
  }
}
void printRegisters(struct CPU *c) {
  for(int i = 0; i < REGISTER_LIM; i++) {
    printf("REGISTER %X: %X\n", i, c->REGISTER[i]);
  }
}
int main() {
  struct CPU c;
  c.LAST_ALU = 0xFF;
  c.pc = 0x0000;
  uint16_t I1 = 0x0003;
  uint16_t I2 = 0X0102;
  
  c.im[0] = 0xE000;
  c.im[1] = 0x0001;
  c.im[2] = 0x0104;
  c.im[3] = 0xB006;
  c.im[4] = 0x2110;
  c.im[5] = 0xA003;
  c.im[6] = 0xF000;
  
  while(c.im[c.pc] != 0xF000) {
    
    execute(&c, c.im[c.pc]);
    
    printf("\nLAST_ALU: %04X\n", c.LAST_ALU);
    printRegisters(&c);
    delay(1000);
  }
 
  return 0;
}
