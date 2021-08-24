#include <Kernel/Drivers/Serial/Serial.h>

#include <Kernel/StdLib.h>
#include <sys/io.h>

namespace Kernel::Drivers {
Serial::Serial()
{
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);
    outb(COM1, 0x03);
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03);
    outb(COM1 + 2, 0xC7);
    outb(COM1 + 4, 0x0B);
    outb(COM1 + 4, 0x1E);

    outb(COM1, 0x29);

    if (inb(COM1) != 0x29) {
        m_failure = true;
        return;
    }

    outb(COM1 + 4, 0x0F);
}

void Serial::write(char c)
{
    while (!(inb(COM1 + 5) & 0x20))
        ;

    outb(COM1, c);
}

void Serial::write(char* c)
{
    write(const_cast<char const*>(c));
}

void Serial::write(char const* c)
{
    for (auto i = 0UZ; i < strlen(c); i++)
        write(c[i]);
}

char Serial::read()
{
    return inb(COM1);
}

}
