#include <FSH/Types.h>

// FIXME: Actually make these tests work.
bool test_div_by_zero() {
    auto z = 0;

    for (auto i = 10; i >= 0; i--)
        z /= i;
}

bool test_page_fault() {
    *reinterpret_cast<u32*>(0x1BADBABE) = 0xDEADBEEF;

    return true;
}
