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

  [[eosio::action]]
  void notify(name user, const std::string msg) {
    print(msg);
  }
};

#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS)                                     \
  extern "C" {                                                                   \
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {              \
      bool self_action = code == receiver && action != "notify"_n.value;         \
      bool notify = code == "addressbook"_n.value && action == "notify"_n.value; \
      if (self_action || notify) {                                               \
        switch (action) {                                                        \
          EOSIO_DISPATCH_HELPER(TYPE, MEMBERS);                                  \
        }                                                                        \
      }                                                                          \
    }                                                                            \
  }                                                                              \

EOSIO_DISPATCH_CUSTOM(test, (a)(b)(c)(notify));
