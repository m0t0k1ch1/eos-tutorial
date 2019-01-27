// include the required libraries
#include <eosiolib/eosio.hpp> // loads the EOSIO C and C++ API
#include <eosiolib/print.hpp>

// use namespace eosio
using namespace eosio;

class hello : public contract { // the contract class needs to extend eosio::contract
public: // public access specifier
  using contract::contract; // will allow us to write more concise code

  [[eosio::action]] // inform the hi() action to the abi generator
  void hi(name user) { // name is one of the most common typedefs
    print("Hello, ", user);
  }
};

// handle the dispatching of actions for the hello contract
EOSIO_DISPATCH(hello, (hi));
