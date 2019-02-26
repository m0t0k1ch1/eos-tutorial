#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] addressbook : public contract
{
  using contract::contract;

public:

  addressbook(name receiver, name code, datastream<const char*> ds) : contract(receiver, code, ds) {}

  // For usability, the contract should have the ability to both create and modify a table row with a single action.
  [[eosio::action]]
  void upsert(
      name user,
      std::string first_name,
      std::string last_name,
      uint64_t age,
      std::string street,
      std::string city,
      std::string state
  ) {
    // The only account authorized to modify the address book is the user.
    require_auth(user);

    // Instantiate the table.
    // To instantiate a table, consider it's two required arguments:
    // 1. The "code", which represents the contract's account.
    // 2. The "scope" which make sure the uniqueness of the contract.
    address_index addresses(_self, _self.value);

    auto iterator = addresses.find(user.value);
    if (iterator == addresses.end()) { // The user isn't in the table.
      // emplace method accepts two arguments,
      // 1. the "payer" of this record who pays the storage usage and a callback function.
      // 2. The callback function for the emplace method must use a lamba to create a reference.
      addresses.emplace(user, [&](auto& row) {
                                row.key        = user;
                                row.first_name = first_name;
                                row.last_name  = last_name;
                                row.age        = age;
                                row.street     = street;
                                row.city       = city;
                                row.state      = state;
                              });

      send_summary(user, "successfully emplaced record to addressbook");
      increment_counter(user, "emplace");
    }
    else { // The user is in the table.
      // modify method accepts three arguments:
      // 1. The iterator defined earlier,
      //    present set to the user as declared when calling this action.
      // 2. The "scope" or "ram payer", which in this case is the user,
      //    as previously decided when proposing the design for this contract.
      // 3. The callback function to handle the modification of the table.
      addresses.modify(iterator, user, [&](auto& row) {
                                         row.key        = user;
                                         row.first_name = first_name;
                                         row.last_name  = last_name;
                                         row.age        = age;
                                         row.street     = street;
                                         row.city       = city;
                                         row.state      = state;
                                       });

      send_summary(user, "successfully modified record to addressbook");
      increment_counter(user, "modify");
    }
  }

  [[eosio::action]]
  void erase(name user)
  {
    require_auth(user);
    address_index addresses(_self, _self.value);
    auto iterator = addresses.find(user.value);

    // A contract cannot erase a record that doesn't exist,
    // so assert that the record indeed exists before proceeding.
    eosio_assert(iterator != addresses.end(), "Record does not exist");

    addresses.erase(iterator);

    send_summary(user, "successfully erased record from addressbook");
    increment_counter(user, "erase");
  }

  // Behave as a "transaction receipt".
  [[eosio::action]]
  void notify(name user, std::string msg)
  {
    require_auth(_self);
    require_recipient(user);
  }

private:

  struct [[eosio::table]] person {
    name key;
    std::string first_name;
    std::string last_name;
    uint64_t age;
    std::string street;
    std::string city;
    std::string state;

    // Every multi_index struct requires a primary key to be set.
    // EOSIO wraps boost::multi_index.
    uint64_t primary_key() const { return key.value; }

    uint64_t get_secondary_1() const { return age; }
  };

  // Notify helper for sending inline transactions
  void send_summary(name user, std::string msg)
  {
    action(
        permission_level{_self, "active"_n},                // A permission_level struct
        _self,                                              // The contract to call
        "notify"_n,                                         // The action
        std::make_tuple(user, user.to_string() + " " + msg) // The data to pass to the action
    ).send();
  }

  // Send an inline action to the abcounter contract.
  void increment_counter(name user, std::string type)
  {
    action(
        permission_level{_self, "active"_n},
        "abcounter"_n,
        "count"_n,
        std::make_tuple(user, type)
    ).send();
  }

  // people table
  // - Use the _n operator to define an eosio::name type and use that name the table.
  // - Pass in the singular person struct defined in the previous step.
  // - Declare this table's type.
  typedef multi_index<"people"_n, person,

                      // Specify the name of index as "byage" and
                      // the second type parameter as a function call operator should extract a const value as an index key.
                      indexed_by<"byage"_n, const_mem_fun<person, uint64_t, &person::get_secondary_1>>

                      > address_index;

};

EOSIO_DISPATCH(addressbook, (upsert)(erase)(notify));
