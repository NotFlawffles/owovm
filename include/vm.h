#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#define MEMORY_SIZE 4096

typedef int16_t i16;

class VM {
    i16 header, data, pc = 512, sp = MEMORY_SIZE - 1,
        hp = 0x800, flags = 0x0;

    std::vector<i16> memory;

    enum Opcodes : i16 {
        OSwp,
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
        OCmp,
        OPop,
        OSyscall,
        OAlloc
    };

    std::map<i16, std::function<void(void)>> instructions = {
        {OSwp, std::bind(&VM::swp, this)},
        {OAdd, std::bind(&VM::add, this)},
        {OSub, std::bind(&VM::sub, this)},
        {OMul, std::bind(&VM::mul, this)},
        {ODiv, std::bind(&VM::div, this)},
        {OAnd, std::bind(&VM::and_, this)},
        {OOr, std::bind(&VM::or_, this)},
        {OXor, std::bind(&VM::xor_, this)},
        {ONot, std::bind(&VM::not_, this)},
        {OLsl, std::bind(&VM::lsl, this)},
        {OLsr, std::bind(&VM::lsr, this)},
        {OMod, std::bind(&VM::mod, this)},
        {OJump, std::bind(&VM::jump, this)},
        {OCmp, std::bind(&VM::cmp, this)},
        {OPop, std::bind(&VM::pop, this)},
        {OSyscall, std::bind(&VM::syscall, this)},
        {OAlloc, std::bind(&VM::alloc, this)},
    };

    void tick(void);
    void fetch(void);
    void decode(void);
    void execute(void);
    void executeInstruction(void);
    bool matchesFlag(void);
    bool conditional(void);

    void swp(void);
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
    void pop(void);
    void syscall(void);
    void alloc(void);

    // kernel area

    void kbhit(void);

    enum Syscalls : i16 {
        SReset,
        SExit,
        SRead,
        SWrite
    };

    enum FD : i16 {
        FStdin = 0x1,
        FStdout = 0xFF
    };

    std::map<i16, i16> FDS = {{0, FStdin}, {1, FStdout}};

    std::map<i16, std::function<void(void)>> syscallTable = {
        {SReset, std::bind(&VM::reset, this)},
        {SExit, std::bind(&VM::exit, this)},
        {SRead, std::bind(&VM::read, this)},
        {SWrite, std::bind(&VM::write, this)}
    };

    void reset(void);
    void exit(void);
    void read(void);
    void write(void);

    public:
    VM(void);
    void loadProgram(std::vector<i16> program);
    void run(void);
};
