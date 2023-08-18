#include <stdio.h>
#include <stdlib.h>

// http://www.6502.org/users/obelisk/6502/architecture.html

using Byte = unsigned char;      // 1 byte
using Word = unsigned short;     // 2 byte

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize() {
        for (int i=0; i< MAX_MEM; ++i)
            Data[i] = 0;
    }

    Byte operator[]( u32 Address ) const {
        // assert:  0 <= Address <= MAX_MEM
        return Data[Address];
    }

    Byte& operator[]( u32 Address ) {
        // assert:  0 <= Address <= MAX_MEM
        return Data[Address];
    }

    void WriteWord(u32 cycles, Word Value, u32 Address) {
        Data[Address] = Value & 0xFF;
        Data[Address +1] = (Value >> 8);
        cycles -= 2;
    }
};
    

struct CPU {
    
    // Registers
    Word PC;     // program counter
    Word SP;     // Stack Pointer

    Byte A, X, Y;   // General Purpose Register

    // Processor Status,  or called Flag
    Byte C : 1;	//0: Carry Flag	
    Byte Z : 1;	//1: Zero Flag
    Byte I : 1; //2: Interrupt disable
    Byte D : 1; //3: Decimal mode
    Byte B : 1; //4: Break
    Byte Unused : 1; //5: Unused
    Byte V : 1; //6: Overflow
    Byte N : 1; //7: Negative

    void Reset(Mem& memory) {
        PC = 0xFFFC;
        SP = 0x00FF;
        C = Z = I = D = B = V = N = Unused = 0;
        A = X = Y = 0;

        memory.Initialize();
    }

    Byte FetchByte( u32& cycles, Mem& memory) {
        Byte rtn = ReadByte(cycles, PC, memory);
        PC++;
        return rtn;
    }
    Word FetchWord( u32& cycles, Mem& memory) {
        Word data = memory[PC];
        PC++;

        data |= (memory[PC] << 8);   // Be careful about ENDIANESS!!
        PC++;

        cycles +=2;
        
        return data;
    }

    Byte ReadByte( u32& cycles, u32 addr, Mem& memory) {
        Byte data = memory[addr];
        cycles--;
        return data;
    }
    
    static constexpr Byte
    INS_LDA_IM = 0xA9,    // lda, immediate  $A9
        INS_LDA_ZP = 0xA5,    // lda, zero page, $A5
        INS_LDA_ZPX = 0xB5,
        INS_JSR = 0x20
        ;
    
    
    void LDASetStatus() {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;    // Set if bit 7 of A is set.  (??) why >?
    }
    
    void Execute( u32 cycles, Mem& memory) {
        while (cycles >0) {
            Byte Ins = FetchByte( cycles, memory );   // next instruction

            Byte value;
            Byte ZeroPageAddr;
            
            Word wordValue;
            Word SubAddr;
            
            switch (Ins) {
            case INS_LDA_IM:
                value = FetchByte(cycles, memory);
                A = value;   // Set to A register (Accumulator)
                LDASetStatus();
                break;

            case INS_LDA_ZP:
                ZeroPageAddr = FetchByte(cycles, memory);
                A = ReadByte( cycles, ZeroPageAddr, memory);
                LDASetStatus();
                 break;

            case INS_LDA_ZPX:
                ZeroPageAddr = FetchByte(cycles, memory);
                ZeroPageAddr += X;
                cycles--;
                A = ReadByte( cycles, ZeroPageAddr, memory);
                LDASetStatus();
                break;

            case INS_JSR:
                SubAddr = FetchWord(cycles, memory);
                memory.WriteWord(cycles, PC-1, SP);
                PC = SubAddr;
                cycles--;
                break;
                
            default:
                printf("Instruction not handled %d\n", Ins);
                throw -1;
            }
        }
    }
};


int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);

    // Just testing:  (PC=0xFFFC)
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42; 
    mem[0xFFFE] = 0x42; 
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    
    // Execute
    cpu.Execute( 9, mem );
    
    return 0;
}
