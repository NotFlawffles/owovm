#include "../include/vm.h"


void VM::fetch(void) {
    pc++;
}

void VM::decode(void) {
    header = (memory[pc] & 0xC000) >> 0xE;
    data = memory[pc] & 0x3FFF;
}

void VM::execute(void) {
    if (header == 0 || header & 2) {
        memory[--sp] = memory[pc];
    }

    else if (header == 1)
        executeInstruction();
}

void VM::executeInstruction(void) {
    switch (data & 0xF) {
        case OHlt:
            if (conditional()) {
                if (matchesFlag())
                    hlt();
            
                break;
            }

            hlt();
            break;

        case OAdd:
            if (conditional()) {
                if (matchesFlag())
                    add();
            
                break;
            }

            add();
            break;

        case OSub:
            if (conditional()) {
                if (matchesFlag())
                    sub();
            
                break;
            }

            sub();
            break;

        case OMul:
            if (conditional()) {
                if (matchesFlag())
                    mul();
            
                break;
            }

            mul();
            break;

        case ODiv:
            if (conditional()) {
                if (matchesFlag())
                    div();
            
                break;
            }

            div();
            break;

        case OAnd:
            if (conditional()) {
                if (matchesFlag())
                    and_();
            
                break;
            }

            and_();
            break;

        case OOr:
            if (conditional()) {
                if (matchesFlag())
                    or_();
            
                break;
            }

            or_();
            break;

        case OXor:
            if (conditional()) {
                if (matchesFlag())
                    xor_();
            
                break;
            }

            xor_();
            break;

        case ONot:
            if (conditional()) {
                if (matchesFlag())
                    not_();
            
                break;
            }

            not_();
            break;

        case OLsl:
            if (conditional()) {
                if (matchesFlag())
                    lsl();
            
                break;
            }

            lsl();
            break;

        case OLsr:
            if (conditional()) {
                if (matchesFlag())
                    lsr();
            
                break;
            }

            lsr();
            break;

        case OMod:
            if (conditional()) {
                if (matchesFlag())
                    mod();
            
                break;
            }

            mod();
            break;

        case OJump:
            if (conditional()) {
                if (matchesFlag())
                    jump();
            
                break;
            }

            jump();
            break;

        case OCmp:
            if (conditional()) {
                if (matchesFlag())
                    cmp();
            
                break;
            }

            cmp();
            break;
    }
}

bool VM::matchesFlag(void) {
    return ((data & 0xFF) >> 5) & flags;
}

bool VM::conditional(void) {
    return (data & 0x1F) >> 0x4;
}

void VM::hlt(void) {
    flags ^= 0x8;
}

void VM::add(void) {
    memory[sp + 1] = memory[sp + 1] + memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::sub(void) {
    memory[sp + 1] = memory[sp + 1] - memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::mul(void) {
    memory[sp + 1] = memory[sp + 1] * memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::div(void) {
    memory[sp + 1] = memory[sp + 1] / memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::and_(void) {
    memory[sp + 1] = memory[sp + 1] & memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::or_(void) {
    memory[sp + 1] = memory[sp + 1] | memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::xor_(void) {
    memory[sp + 1] = memory[sp + 1] ^ memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::not_(void) {
    memory[sp] = ~memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::lsl(void) {
    memory[sp + 1] = memory[sp + 1] << memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::lsr(void) {
    memory[sp + 1] = memory[sp + 1] >> memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::mod(void) {
    memory[sp + 1] = memory[sp + 1] % memory[sp];
    memory[sp] = 0;
    sp++;
}

void VM::jump(void) {
    pc = memory[sp];
}

void VM::cmp(void) {
    flags |= (memory[sp + 1] - memory[sp] > 0) << 0x2;
    flags |= (memory[sp + 1] - memory[sp] == 0) << 0x1;
    flags |= (memory[sp + 1] - memory[sp] < 0);
}

VM::VM(void) {
    memory.reserve(0x1000);
}

void VM::loadProgram(std::vector<i16> program) {
    for (size_t i = 0; i < program.size(); i++)
        memory[i] = program[i];
}

void VM::run(void) {
    flags |= 0x8;

    while (flags & 0x8) {
        fetch();
        decode();
        execute();
    }
}
