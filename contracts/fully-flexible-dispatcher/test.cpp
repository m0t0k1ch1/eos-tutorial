#include <eosiolib/eosio.hpp>

using namespace eosio;

struct in {
  name user;
  EOSLIB_SERIALIZE(in, (user))
};

void a(uint64_t self, uint64_t code)
{
  eosio_assert(code == self, "assertion failed in a");
  auto data = unpack_action_data<in>();
  require_auth(data.user);
  print("Hi, ", data.user, " from a");
}

void b(uint64_t self, uint64_t code)
{
  eosio_assert(code == self, "assertion failed in b");
  auto data = unpack_action_data<in>();
  require_auth(data.user);
  print("Hi, ", data.user, " from b");
}

void c(uint64_t self, uint64_t code)
{
  eosio_assert(code == self, "assertion failed in c");
  auto data = unpack_action_data<in>();
  require_auth(data.user);
  print("Hi, ", data.user, " from c");
}

extern "C" {
  void apply(uint64_t receiver, uint64_t code, uint64_t action)
  {
    switch (action) {
      case name("a").value: return a(receiver, code);
      case name("b").value: return b(receiver, code);
      case name("c").value: return c(receiver, code);
    }
  }
};
