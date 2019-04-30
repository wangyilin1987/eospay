#pragma once

struct [[eosio::table, eosio::contract("eospay")]] rvariable {
    uint64_t key;
    uint32_t value;

    rvariable() {
    }

    uint64_t primary_key() const {
        return key;
    }

    EOSLIB_SERIALIZE(
            rvariable,
            (key)
            (value)
    )
};

typedef eosio::multi_index< "rvariable"_n, rvariable> rvariable_table;

class variable_control {
private:
    
    name self;
    rule_controller<rvariable, rvariable_table> rvariable_controller;

public:
    // constructor
    //-------------------------------------------------------------------------
    variable_control(name _self)
            : self(_self)
            , rvariable_controller(_self, "variable"_n) {
    }

    // internal apis
    //-------------------------------------------------------------------------

    // actions
    //-------------------------------------------------------------------------
    rule_controller<rvariable, rvariable_table>& get_rvariable_rule() {
        return rvariable_controller;
    }
};
