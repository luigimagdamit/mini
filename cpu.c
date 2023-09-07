#include <stdio.h>
#include <stdint.h> 

#define REGISTER_LIM 16
#define IM_LIM 64
struct CPU {
  uint8_t REGISTER[REGISTER_LIM];
  uint16_t pc;

  uint16_t im[IM_LIM];
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
  c->REGISTER[dest] = REG[a] + REG[b];
}
// 0x1000
void sub(struct CPU *c, uint8_t dest, uint8_t a, uint8_t b) {
  uint8_t *REG = c->REGISTER;
  c->REGISTER[dest] = REG[a] - REG[b];
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
  op.d = (hex & 0x0F00) >> 8;
  op.kk = hex & 0x00FF;
  op.x = hex & 0x00F0;
  op.y = hex & 0x000F;
  return op;
}
void printRegisters(struct CPU *c) {
  for(int i = 0; i < REGISTER_LIM; i++) {
    printf("%X\n", c->REGISTER[i]);
  }
}
int main() {
  struct CPU c;
  uint16_t I1 = 0x0003;
  uint16_t I2 = 0X0102;
  //LOAD INSTRUCTION -0xNdKK

  uint16_t I3 = 0X1201;
  c.im[0] = I1;
  c.im[1] = I2;
  c.im[2] = I3;
  
  // get instruction 1, add. then at register 2, add the values of register 1 and 2
  opcode op1 = createOpcode(c.im[0]);
  opcode op2 = createOpcode(c.im[1]);
  opcode op3 = createOpcode(c.im[2]);

  load(&c, op1.d, op1.kk);
  load(&c, op2.d, op2.kk);
  add(&c, op3.d, op3.x, op3.y);
  printRegisters(&c);
  return 0;
}
