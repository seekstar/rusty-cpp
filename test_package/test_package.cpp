#include <iostream>
#include <rusty/macro.h>
#include <rusty/primitive.h>

int main(void) {
  rusty_assert(1);
  rusty_assert_eq(rusty::next_power_of_two((size_t)42), 64,
                  "Math doesn't exist anymore");
  rusty_assert_eq(rusty::next_power_of_two((size_t)233), 256);

  return EXIT_SUCCESS;
}
