#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the 8085's registers
unsigned char A, B, C, D, E, H, L, F;
unsigned short PC, SP;

// Define the 8085's memory
unsigned char memory[65536];

// Flag bit positions in the F register
#define CARRY_FLAG 0x01
#define AUX_CARRY_FLAG 0x10
#define PARITY_FLAG 0x04
#define ZERO_FLAG 0x40
#define SIGN_FLAG 0x80

// Macro to set/clear flag bits
#define SET_FLAG(f) (F |= (f))
#define CLEAR_FLAG(f) (F &= ~(f))

// Macro to check the parity of a byte
#define CHECK_PARITY(x) (__builtin_parity(x) == 0)

// Function to update flags based on the result of an operation
void update_flags(unsigned char result)
{
    if (result == 0)
    {
        SET_FLAG(ZERO_FLAG);
    }
    else
    {
        CLEAR_FLAG(ZERO_FLAG);
    }

    if (result & 0x80)
    {
        SET_FLAG(SIGN_FLAG);
    }
    else
    {
        CLEAR_FLAG(SIGN_FLAG);
    }

    if (CHECK_PARITY(result))
    {
        SET_FLAG(PARITY_FLAG);
    }
    else
    {
        CLEAR_FLAG(PARITY_FLAG);
    }
}

// Function to print the state of the registers and memory with spacing and instruction count
void print_state(int instruction_count)
{
    printf("\nInstruction %03d:\n", instruction_count);
    printf("| Register | Value |\n");
    printf("|----------|-------|\n");
    printf("| A        | %02X   |\n", A);
    printf("| B        | %02X   |\n", B);
    printf("| C        | %02X   |\n", C);
    printf("| D        | %02X   |\n", D);
    printf("| E        | %02X   |\n", E);
    printf("| H        | %02X   |\n", H);
    printf("| L        | %02X   |\n", L);
    printf("| F        | %02X   |\n", F);
    printf("| PC       | %04X  |\n", PC);
    printf("| SP       | %04X  |\n", SP);
    printf("| Memory   |       |\n");
    for (int i = 0; i < 16; i++)
    {
        printf("| %04X     | %02X   |\n", i, memory[i]);
    }
}

// Function to emulate an instruction and print its state
void emulate_instruction(int *instruction_count)
{
    unsigned char opcode = memory[PC];
    PC++;

    unsigned short address;
    unsigned char data;

    printf("Executing opcode: %02X\n", opcode);

    switch (opcode)
    {
    case 0x00: // NOP
        printf("NOP\n");
        break;
    case 0x06: // MVI B, data
        B = memory[PC++];
        printf("MVI B, %02X\n", B);
        break;
    case 0x0E: // MVI C, data
        C = memory[PC++];
        printf("MVI C, %02X\n", C);
        break;
    case 0x16: // MVI D, data
        D = memory[PC++];
        printf("MVI D, %02X\n", D);
        break;
    case 0x1E: // MVI E, data
        E = memory[PC++];
        printf("MVI E, %02X\n", E);
        break;
    case 0x26: // MVI H, data
        H = memory[PC++];
        printf("MVI H, %02X\n", H);
        break;
    case 0x2E: // MVI L, data
        L = memory[PC++];
        printf("MVI L, %02X\n", L);
        break;
    case 0x3E: // MVI A, data
        A = memory[PC++];
        printf("MVI A, %02X\n", A);
        break;
    case 0x80: // ADD B
        A += B;
        update_flags(A);
        printf("ADD B\n");
        break;
    case 0x81: // ADD C
        A += C;
        update_flags(A);
        printf("ADD C\n");
        break;
    case 0x86: // ADD M
        A += memory[(H << 8) | L];
        update_flags(A);
        printf("ADD M\n");
        break;
    case 0xC6: // ADI data
        A += memory[PC++];
        update_flags(A);
        printf("ADI %02X\n", A);
        break;
    case 0x7F: // MOV A, A
        A = A;
        printf("MOV A, A\n");
        break;
    case 0x78: // MOV A, B
        A = B;
        printf("MOV A, B\n");
        break;
    case 0x79: // MOV A, C
        A = C;
        printf("MOV A, C\n");
        break;
    case 0x7A: // MOV A, D
        A = D;
        printf("MOV A, D\n");
        break;
    case 0x7B: // MOV A, E
        A = E;
        printf("MOV A, E\n");
        break;
    case 0x7C: // MOV A, H
        A = H;
        printf("MOV A, H\n");
        break;
    case 0x7D: // MOV A, L
        A = L;
        printf("MOV A, L\n");
        break;
    case 0x7E: // MOV A, M
        A = memory[(H << 8) | L];
        printf("MOV A, M\n");
        break;
    case 0x01: // LXI B, data16
        C = memory[PC++];
        B = memory[PC++];
        printf("LXI B, %02X%02X\n", B, C);
        break;
    case 0x11: // LXI D, data16
        E = memory[PC++];
        D = memory[PC++];
        printf("LXI D, %02X%02X\n", D, E);
        break;
    case 0x21: // LXI H, data16
        L = memory[PC++];
        H = memory[PC++];
        printf("LXI H, %02X%02X\n", H, L);
        break;
    case 0x31: // LXI SP, data16
        SP = memory[PC++];
        SP |= memory[PC++] << 8;
        printf("LXI SP, %04X\n", SP);
        break;
    case 0x32: // STA addr
        address = memory[PC++];
        address |= memory[PC++] << 8;
        memory[address] = A;
        printf("STA %04X\n", address);
        break;
    case 0x76: // HLT
        printf("HLT encountered. Exiting.\n");
        exit(0);
    default:
        printf("Unimplemented opcode: %02X\n", opcode);
        break;
    }

    (*instruction_count)++;
    print_state(*instruction_count);
}

int main()
{
    // Initialize the 8085's registers and memory
    A = B = C = D = E = H = L = F = 0;
    PC = 0x0000; // Program counter starts at 0
    SP = 0xFFFF; // Stack pointer starts at top of memory

    // User input for the program
    printf("Enter 8085 assembly instructions (end with 'HLT'):\n");

    char input[256];
    int address = 0;
    while (1)
    {
        fgets(input, sizeof(input), stdin);
        if (strncmp(input, "HLT", 3) == 0)
        {
            memory[address++] = 0x76; // HLT opcode
            break;
        }
        else if (strncmp(input, "MVI B,", 6) == 0)
        {
            memory[address++] = 0x06; // MVI B opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI C,", 6) == 0)
        {
            memory[address++] = 0x0E; // MVI C opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI D,", 6) == 0)
        {
            memory[address++] = 0x16; // MVI D opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI E,", 6) == 0)
        {
            memory[address++] = 0x1E; // MVI E opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI H,", 6) == 0)
        {
            memory[address++] = 0x26; // MVI H opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI L,", 6) == 0)
        {
            memory[address++] = 0x2E; // MVI L opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "MVI A,", 6) == 0)
        {
            memory[address++] = 0x3E; // MVI A opcode
            memory[address++] = (unsigned char)strtol(input + 6, NULL, 16);
        }
        else if (strncmp(input, "ADD B", 5) == 0)
        {
            memory[address++] = 0x80; // ADD B opcode
        }
        else if (strncmp(input, "ADD C", 5) == 0)
        {
            memory[address++] = 0x81; // ADD C opcode
        }
        else if (strncmp(input, "ADD M", 5) == 0)
        {
            memory[address++] = 0x86; // ADD M opcode
        }
        else if (strncmp(input, "ADI ", 4) == 0)
        {
            memory[address++] = 0xC6; // ADI opcode
            memory[address++] = (unsigned char)strtol(input + 4, NULL, 16);
        }
        else if (strncmp(input, "MOV A, A", 8) == 0)
        {
            memory[address++] = 0x7F; // MOV A, A opcode
        }
        else if (strncmp(input, "MOV A, B", 8) == 0)
        {
            memory[address++] = 0x78; // MOV A, B opcode
        }
        else if (strncmp(input, "MOV A, C", 8) == 0)
        {
            memory[address++] = 0x79; // MOV A, C opcode
        }
        else if (strncmp(input, "MOV A, D", 8) == 0)
        {
            memory[address++] = 0x7A; // MOV A, D opcode
        }
        else if (strncmp(input, "MOV A, E", 8) == 0)
        {
            memory[address++] = 0x7B; // MOV A, E opcode
        }
        else if (strncmp(input, "MOV A, H", 8) == 0)
        {
            memory[address++] = 0x7C; // MOV A, H opcode
        }
        else if (strncmp(input, "MOV A, L", 8) == 0)
        {
            memory[address++] = 0x7D; // MOV A, L opcode
        }
        else if (strncmp(input, "MOV A, M", 8) == 0)
        {
            memory[address++] = 0x7E; // MOV A, M opcode
        }
        else if (strncmp(input, "LXI B,", 6) == 0)
        {
            memory[address++] = 0x01; // LXI B opcode
            unsigned short data16 = (unsigned short)strtol(input + 6, NULL, 16);
            memory[address++] = data16 & 0xFF;        // Low-order byte
            memory[address++] = (data16 >> 8) & 0xFF; // High-order byte
        }
        else if (strncmp(input, "LXI D,", 6) == 0)
        {
            memory[address++] = 0x11; // LXI D opcode
            unsigned short data16 = (unsigned short)strtol(input + 6, NULL, 16);
            memory[address++] = data16 & 0xFF;        // Low-order byte
            memory[address++] = (data16 >> 8) & 0xFF; // High-order byte
        }
        else if (strncmp(input, "LXI H,", 6) == 0)
        {
            memory[address++] = 0x21; // LXI H opcode
            unsigned short data16 = (unsigned short)strtol(input + 6, NULL, 16);
            memory[address++] = data16 & 0xFF;        // Low-order byte
            memory[address++] = (data16 >> 8) & 0xFF; // High-order byte
        }
        else if (strncmp(input, "LXI SP,", 7) == 0)
        {
            memory[address++] = 0x31; // LXI SP opcode
            unsigned short data16 = (unsigned short)strtol(input + 7, NULL, 16);
            memory[address++] = data16 & 0xFF;        // Low-order byte
            memory[address++] = (data16 >> 8) & 0xFF; // High-order byte
        }
        else if (strncmp(input, "STA ", 4) == 0)
        {
            memory[address++] = 0x32; // STA opcode
            unsigned short addr = (unsigned short)strtol(input + 4, NULL, 16);
            memory[address++] = addr & 0xFF;        // Low-order byte
            memory[address++] = (addr >> 8) & 0xFF; // High-order byte
        }
        else
        {
            printf("Unknown instruction: %s\n", input);
        }
    }

    int instruction_count = 0;
    while (1)
    {
        emulate_instruction(&instruction_count);
    }

    return 0;
}