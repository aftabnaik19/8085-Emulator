#include <stdio.h>

// Define enums for register names
enum registerName
{
    B,
    C,
    D,
    E,
    H,
    L,
    accumulator, // General purpose registers
    stackPointer,
    programCounter,
    instruction,
    flags
};
enum busType
{
    dataBus,
    addressBus,
    controlBus
};
// Define a struct for 8085 registers
struct Registers
{
    // Define bitfields for each register
    union
    {
        unsigned short BC_combined : 16;
        struct
        {
            unsigned char C : 8;
            unsigned char B : 8;
        } BC;
    };

    union
    {
        struct
        {
            unsigned char E : 8;
            unsigned char D : 8;
        } DE;

        unsigned short DE_combined : 16;
    };

    union
    {
        struct
        {
            unsigned char L : 8;
            unsigned char H : 8;
        } HL;

        unsigned short HL_combined : 16;
    };
    struct
    {
        unsigned short addressBus : 16;
        unsigned char dataBus : 8;
        unsigned char controlBus : 8;
    } bus;

    unsigned short StackPointer : 16;
    unsigned short programCounter : 16;

    unsigned char Accumulator : 8;
    unsigned char instruction : 8;
    unsigned char flags : 8;
};

// Function to print register values in binary
void printAllRegisters(struct Registers cpu)
{
    printf("CombinerRegisters:\n");
    printf("BC: %04X\n", cpu.BC_combined);
    // printf("BC: %04X\n", cpu.BC); can be used directly but it would be difficult to do different 16 bit operations
    printf("DE: %04X\n", cpu.DE_combined);
    printf("HL: %04X\n", cpu.HL_combined);

    // Pad with 0 for A register
    printf("\nRegisters:\n");
    printf("B: %02X\n", cpu.BC.B);
    printf("C: %02X\n", cpu.BC.C);
    printf("D: %02X\n", cpu.DE.D);
    printf("E: %02X\n", cpu.DE.E);
    printf("H: %02X\n", cpu.HL.H);
    printf("L: %02X\n", cpu.HL.L);

    // other important registers
    printf("\n");
    printf("Program Counter: %02X\n", cpu.programCounter);
    printf("Stack Pointer: %02X\n", cpu.StackPointer);
    printf("Instruction: %02X\n", cpu.instruction);
    printf("Accumulator: %02X\n", cpu.Accumulator);
    printf("Flags: %02X\n", cpu.flags);
    // Print the bus in hexadecimal format
    printf("\nBus:\n");
    printf("Address: %04X\n", cpu.bus.addressBus);
    printf("Data: %02X\n", cpu.bus.dataBus);
    printf("Control:  %02X\n", cpu.bus.controlBus);
}
unsigned char memory[40][40];

int main()
{
    // Declare a struct variable of type Registers
    struct Registers cpu;

    // Initialize some register values
    cpu.BC.B = 0xA;
    cpu.BC.C = 0xB;

    cpu.DE.D = 0x1;
    cpu.DE.E = 0x2;

    cpu.HL.H = 0x5;
    cpu.HL.L = 0x36;
    cpu.BC_combined = 0x35FF;

    cpu.programCounter = 0xD9CF;
    cpu.StackPointer = 0x89BF;
    cpu.instruction = 0x8F;
    cpu.Accumulator = 0xFF;
    cpu.flags = 0x2D;

    // Print register values in binary
    printf("Register values in binary (each hex digit represents 4 bits):\n");
    printAllRegisters(cpu);
    int value = 0;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            memory[i][j] = 0;
            value++;
            printf("%03d [%02X]  ", memory[i][j], value);
        }
        printf("\n\n");
    }
    printf("%d", value);
    return 0;
}
