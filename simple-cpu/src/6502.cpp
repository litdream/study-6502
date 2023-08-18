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
        Byte data = memory[PC];
        PC++;
        cycles--;

        return data;
    }

    static constexpr Byte INS_LDA_IM = 0xA9;
    
    void Execute( u32 cycles, Mem& memory) {
        while (cycles >0) {
            Byte Ins = FetchByte( cycles, memory );   // next instruction
            Byte value;
            
            switch (Ins) {
            case INS_LDA_IM:
                value = FetchByte(cycles, memory);
                A = value;
                Z = (A == 0);
                N = (A & 0b10000000) > 0;    // Set if bit 7 of A is set.  (??) why >?

                printf("LDA immediate: %d\n", value);
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
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;    // 66

    // Execute
    cpu.Execute( 2, mem );
    
    return 0;
}
