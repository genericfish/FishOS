#pragma once

namespace Kernel::Drivers {

#define COM1 0x3F8

class Serial {
public:
    Serial();

    void write(char);
    void write(char const*);

    char read();

private:
    bool m_failure = false;
};

}
