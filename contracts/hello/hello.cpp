// Include the required libraries.
#include <eosiolib/eosio.hpp> // Loads the EOSIO C and C++ API.

// Use namespace eosio.
using namespace eosio;

class [[eosio::contract]] hello : public contract // The contract class needs to extend eosio::contract.
{
  using contract::contract; // Will allow us to write more concise code.

public: // Public access specifier.

  [[eosio::action]] // Inform the hi() action to the abi generator.
  void hi(name user) // Name is one of the most common typedefs.
  {
    print("Hello, ", user);
  }

};

// Handle the dispatching of actions for the hello contract.
EOSIO_DISPATCH(hello, (hi));
