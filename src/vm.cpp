#include "../include/vm.h"
#include <cstdio>
#include <algorithm>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>
#include <cstdlib>


void VM::fetch(void) {
    pc++;
}

void VM::decode(void) {
    header = (memory[pc] & 0xC000) >> 0xE;
    data = memory[pc] & 0x3FFF;
}

void VM::execute(void) {
    if (header == 0 || header & 2)
        memory[--sp] = memory[pc];

    else if (header == 1)
        executeInstruction();
}

void VM::executeInstruction(void) {
    if (conditional()) {
        if (matchesFlag())
            instructions[data & 0xFF]();

        return;
    }

    instructions[data & 0xFF]();
}

bool VM::matchesFlag(void) {
    return ((data & 0xF00) >> 0x9) & flags;
}

bool VM::conditional(void) {
    return (data & 0x100) >> 0x8;
}

void VM::swp(void) {
    memory[sp + 1] ^= memory[sp];
    memory[sp] ^= memory[sp + 1];
    memory[sp + 1] ^= memory[sp];
}

void VM::add(void) {
    memory[sp + 1] = memory[sp + 1] + memory[sp];
    memory[sp++] = 0;
}

void VM::sub(void) {
    memory[sp + 1] = memory[sp + 1] - memory[sp];
    memory[sp++] = 0;
}

void VM::mul(void) {
    memory[sp + 1] = memory[sp + 1] * memory[sp];
    memory[sp++] = 0;
}

void VM::div(void) {
    memory[sp + 1] = memory[sp + 1] / memory[sp];
    memory[sp++] = 0;
}

void VM::and_(void) {
    memory[sp + 1] = memory[sp + 1] & memory[sp];
    memory[sp++] = 0;
}

void VM::or_(void) {
    memory[sp + 1] = memory[sp + 1] | memory[sp];
    memory[sp++] = 0;
}

void VM::xor_(void) {
    memory[sp + 1] = memory[sp + 1] ^ memory[sp];
    memory[sp++] = 0;
}

void VM::not_(void) {
    memory[sp] = ~memory[sp];
    memory[sp++] = 0;
}

void VM::lsl(void) {
    memory[sp + 1] = memory[sp + 1] << memory[sp];
    memory[sp++] = 0;
}

void VM::lsr(void) {
    memory[sp + 1] = memory[sp + 1] >> memory[sp];
    memory[sp++] = 0;
}

void VM::mod(void) {
    memory[sp + 1] = memory[sp + 1] % memory[sp];
    memory[sp++] = 0;
}

void VM::jump(void) {
    lr = pc + 1;
    pc = memory[sp] - 1;
    memory[sp++] = 0;
}

void VM::cmp(void) {
    flags |= (memory[sp + 1] - memory[sp] > 0) << 0x2;
    flags |= (memory[sp + 1] - memory[sp] == 0) << 0x1;
    flags |= (memory[sp + 1] - memory[sp] < 0);
}

void VM::pop(void) {
    int16_t count = memory[sp];

    for (int16_t i = -1; i < count; i++)
        memory[sp++] = 0;
}

void VM::syscall(void) {
    syscallTable[memory[sp]]();
}

void VM::alloc(void) {
    if (!memory[sp])
        return;

    memory[hp++] = memory[sp];
    memory[--sp] = hp;
    hp += memory[sp + 1];
}

void VM::str(void) {
    memory[memory[sp + 1]] = memory[sp];
}

void VM::ld(void) {
    sp--;
    memory[sp] = memory[memory[sp + 1]];
}

void VM::rev(void) {
    u16 count = memory[sp];
    memory[sp++] = 0;
    std::reverse(memory.begin() + sp, memory.begin() + MEMORY_SIZE - 1 - count);
}

void VM::ret(void) {
    pc = lr;
}

void VM::kbhit(void) {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
        memory[0x0] = ch;
}

void VM::reset(void) {}

void VM::exit(void) {
    std::exit(memory[sp + 1]);
}

void VM::read(void) {
    u16 fd = memory[sp + 3],
        buf = memory[sp + 2],
        count = memory[sp + 1];

    std::string input;
    std::cin >> input;

    for (size_t i = 0; i < input.length(); i++)
        memory[i + 1] = input[i];

    for (u16 i = 0, j = FDS[fd]; i < count; i++)
        memory[buf + i] = memory[j + i];
}

void VM::write(void) {
    u16 fd = memory[sp + 3],
        buf = memory[sp + 2],
        count = memory[sp + 1];

    for (u16 i = 0; i < count; i++) {
        memory[FDS[fd] + i] = memory[buf + i];
        std::putc(memory[FDS[fd] + i], fd ? stdout : stdin);
    }
}

void VM::tick(void) {
    fetch();
    decode();
    execute();
    kbhit();
}

VM::VM(void) {
    memory.reserve(0xFFFF);
}

void VM::loadProgram(std::vector<u16> program) {
    for (size_t i = 0; i < program.size(); i++)
        memory[i + 1025] = program[i];
}

void VM::run(void) {
    flags |= 0x8;

    while (flags & 0x8)
        tick();
}
