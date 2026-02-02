#include <string.h>

#include "rv32izicsr.h"

void RV32IZicsr_InitState(struct RV32IZicsr_State *state) {
   memset(state, 0, sizeof(*state));
}

#define REG(id) (state->regs[id])

void RV32IZicsr_Step(struct RV32IZicsr_State *state, uint8_t *image) {
   uint32_t pc = state->pc;
   uint32_t ir;

   ir = RV32IZicsr_LoadU32(image, pc); 

   uint8_t opcode = ir & 0x7f;
   uint8_t funct3 = (ir >> 12) & 0x7;
   uint8_t funct7 = (ir >> 25) & 0x7f;
   uint8_t rd = (ir >> 7) & 0x1f;
   uint8_t rs1 = (ir >> 15) & 0x1f;
   uint8_t rs2 = (ir >> 20) & 0x1f;

   uint32_t rs1val = REG(rs1);
   uint32_t rs2val = REG(rs2);

   uint32_t u_imm = (ir & 0xfffff000);
   uint32_t j_imm = ( ((ir & 0x80000000) ? 0xfff00000 : 0x0) |
                      ((ir & 0x00100000) ? 0x00000800 : 0x0) |
                      ((ir & 0x7fe00000) >> 20) |
                      ((ir & 0x000ff000)) );
   uint32_t i_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
                      (ir >> 20) );
   uint32_t b_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
                      ((ir & 0x7e000000) >> 20) |
                      ((ir & 0x00000f00) >> 7) |
                      ((ir & 0x00000080) ? 0x00000800 : 0x0) );
   uint32_t s_imm = ( ((ir & 0x80000000) ? 0xfffff000 : 0x0) |
                      ((ir & 0xfe000000) >> 20) |                  
                      ((ir & 0x00000f80) >> 7) );

   switch (opcode) {
      case 0x37: // LUI
         if (rd) REG(rd) = u_imm;
         pc = pc + 4;
         break;
      case 0x17: // AUIPC
         if (rd) REG(rd) = pc + u_imm;
         pc = pc + 4;
         break;
      case 0x6f: // JAL
         if (rd) REG(rd) = pc + 4;
         pc = pc + j_imm;
         break;
      case 0x67: // JALR
         if (rd) REG(rd) = pc + 4;
         pc = rs1val + i_imm;
         break;
      case 0x63: // Branch
         pc = pc + 4;
         switch (funct3) {
            case 0: if (rs1val == rs2val) pc = pc + b_imm - 4; break;
            case 1: if (rs1val != rs2val) pc = pc + b_imm - 4; break;
            case 4: if ((int32_t)rs1val < (int32_t)rs2val) pc = pc + b_imm - 4; break;
            case 5: if ((int32_t)rs1val >= (int32_t)rs2val) pc = pc + b_imm - 4; break;
            case 6: if (rs1val < rs2val) pc = pc + b_imm - 4; break;
            case 7: if (rs1val >= rs2val) pc = pc + b_imm - 4; break;
         }
         break;
      case 0x03: { // Load
         uint32_t addr = rs1val + i_imm;
         if (rd) {
            switch (funct3) {
               case 0: 
                  REG(rd) = RV32IZicsr_LoadS8(image, addr); break;
               case 1:
                  if (addr & 0x1) { REG(rd) = 0; break; }
                  REG(rd) = RV32IZicsr_LoadS16(image, addr); break;
               case 2:
                  if (addr & 0x3) { REG(rd) = 0; break; }
                  REG(rd) = RV32IZicsr_LoadU32(image, addr); break;
               case 4: 
                  REG(rd) = RV32IZicsr_LoadU8(image, addr); break;
               case 5:
                  if (addr & 0x1) { REG(rd) = 0; break; }
                  REG(rd) = RV32IZicsr_LoadU16(image, addr); break;
            }
         }
         pc = pc + 4;
         break;
      }
      case 0x23: { // Store
         uint32_t addr = rs1val + s_imm;
         switch (funct3) {
            case 0: RV32IZicsr_StoreU8(image, addr, rs2val); break;
            case 1: 
               if (addr & 0x1) RV32IZicsr_StoreU32(image, addr, 0);
               RV32IZicsr_StoreU16(image, addr, rs2val); break;
            case 2: 
               if (addr & 0x3) RV32IZicsr_StoreU32(image, addr, 0);
               RV32IZicsr_StoreU32(image, addr, rs2val); break;
         }
         pc = pc + 4;
         break;
      }
      case 0x13: // ARITH-I
      case 0x33: { // ARITH
         uint32_t s2 = (opcode == 0x13 ? i_imm : rs2val);
         uint8_t shamt = ((ir & 0x01f00000) >> 20);

         if (rd) {
            if ((opcode == 0x33 && funct3 == 0x0 && !funct7) || // ADD
                (opcode == 0x13 && funct3 == 0x0)) // ADDI
               REG(rd) = rs1val + s2;
            else if (funct3 == 0x0 && funct7)  // SUB
               REG(rd) = rs1val - s2;
            else if (funct3 == 0x1) // SLL
               REG(rd) = rs1val << (opcode == 0x13 ? shamt : s2);
            else if (funct3 == 0x2) // SLT
               REG(rd) = (int32_t)rs1val < (int32_t)s2;
            else if (funct3 == 0x4)  // SLTU
               REG(rd) = rs1val ^ s2;
            else if (funct3 == 0x5 && !funct7) // SRL
               REG(rd) = rs1val >> (opcode == 0x13 ? shamt : s2);
            else if (funct3 == 0x5 && funct7) // SRA
               REG(rd) = ((rs1val >> (opcode == 0x13 ? shamt : s2)) | (~0U << (opcode == 0x13 ? shamt : s2)));
            else if (funct3 == 0x6) // OR
               REG(rd) = rs1val | s2;
            else if (funct3 == 0x7) // AND
               REG(rd) = rs1val & s2;
         }
         pc = pc + 4;
         break;
      }
      default: pc = pc + 4; break;
   }

   state->pc = pc & ~1 & RV32IZicsr_RAM_MASK;
}