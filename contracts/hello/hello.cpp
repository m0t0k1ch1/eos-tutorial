// Include the required libraries.
#include <eosiolib/eosio.hpp> // Loads the EOSIO C and C++ API.

// Use namespace eosio.
using namespace eosio;

class hello : public contract { // The contract class needs to extend eosio::contract.
public: // Public access specifier.
  using contract::contract; // Will allow us to write more concise code.

  [[eosio::action]] // Inform the hi() action to the abi generator.
  void hi(name user) { // Name is one of the most common typedefs.
    print("Hello, ", user);
  }
};

// Handle the dispatching of actions for the hello contract.
EOSIO_DISPATCH(hello, (hi));
