#pragma once

#include <iostream>
#include <vector>

#define MEMORY_SIZE 4096

typedef int16_t i16;

class VM {
    i16 header, data, pc = -1, sp = MEMORY_SIZE;
    i16 flags = 0x0;
    std::vector<i16> memory;

    enum Opcodes : i16 {
        OHlt,
        OAdd,
        OSub,
        OMul,
        ODiv,
        OAnd,
        OOr,
        OXor,
        ONot,
        OLsl,
        OLsr,
        OMod,
        OJump,
        OCmp
    };

    void fetch(void);
    void decode(void);
    void execute(void);
    void executeInstruction(void);
    bool matchesFlag(void);
    bool conditional(void);

    void hlt(void);
    void add(void);
    void sub(void);
    void mul(void);
    void div(void);
    void and_(void);
    void or_(void);
    void xor_(void);
    void not_(void);
    void lsl(void);
    void lsr(void);
    void mod(void);
    void jump(void);
    void cmp(void);

    public:
    VM(void);
    void loadProgram(std::vector<i16> program);
    void run(void);
};
