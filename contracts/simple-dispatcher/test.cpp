#include <eosiolib/eosio.hpp>

using namespace eosio;

class test : public contract {
public:
  using contract::contract;

  [[eosio::action]]
  void a(name user) {
    require_auth(user);
    print("Hi, ", user, " from a");
  }

  [[eosio::action]]
  void b(name user) {
    require_auth(user);
    print("Hi, ", user, " from b");
  }

  [[eosio::action]]
  void c(name user) {
    require_auth(user);
    print("Hi, ", user, " from c");
  }
};

extern "C" {
  void apply(uint64_t receiver, uint64_t code, uint64_t action) {
    if (code == receiver) {
      switch (action) {
      case name("a").value:
        execute_action(name(receiver), name(code), &test::a);
        break;
      case name("b").value:
        execute_action(name(receiver), name(code), &test::b);
        break;
      case name("c").value:
        execute_action(name(receiver), name(code), &test::c);
        break;
      }
    }
  }
};
