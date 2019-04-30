#pragma once

struct [[eosio::table, eosio::contract("eospay")]] gcontract {
    name   key;

    uint64_t primary_key()const { return key.value; }
    EOSLIB_SERIALIZE( gcontract, (key))
};

typedef eosio::multi_index<"gcontract"_n, gcontract> gcontract_table;

class gcontract_control {
private:
    name self;
    rule_controller<gcontract, gcontract_table> gcontract_controller;

public:
    // constructor
    //-------------------------------------------------------------------------
    gcontract_control(name _self)
            : self(_self)
            , gcontract_controller(_self, "gcontract"_n) {
    }

    // internal apis
    //-------------------------------------------------------------------------

    // actions
    //-------------------------------------------------------------------------
    rule_controller<gcontract, gcontract_table>& get_gcontract_rule() {
        return gcontract_controller;
    }
};

