#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] test : public contract
{
  using contract::contract;

public:

  [[eosio::action]]
  void a(name user)
  {
    require_auth(user);
    print("Hi, ", user, " from a");
  }

  [[eosio::action]]
  void b(name user)
  {
    require_auth(user);
    print("Hi, ", user, " from b");
  }

  [[eosio::action]]
  void c(name user)
  {
    require_auth(user);
    print("Hi, ", user, " from c");
  }

  [[eosio::action]]
  void notify(name user, const std::string msg)
  {
    print(msg);
  }

};

extern "C" {
  void apply(uint64_t receiver, uint64_t code, uint64_t action)
  {
    if (code == receiver && action == name("a").value) {
      execute_action(name(receiver), name(code), &test::a);
    }
    else if (code == receiver && action == name("b").value) {
      execute_action(name(receiver), name(code), &test::b);
    }
    else if (code == receiver && action == name("c").value) {
      execute_action(name(receiver), name(code), &test::c);
    }
    else if (code == name("addressbook").value && action == name("notify").value) {
      execute_action(name(receiver), name(code), &test::notify);
    }
  }
};
