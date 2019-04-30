/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/multi_index.hpp>
#include <vector>
#include <string>

#include "util/util.hpp"

#include "common/common.hpp"

#include "contract/rule_controller.hpp"
#include "contract/variable_control.hpp"
#include "contract/gcontract_control.hpp"
#include "contract/player_control.hpp"
#include "contract/channel_control.hpp"
#include "contract/order_control.hpp"

// Extacted from eosio.token contract:
namespace eosio {
   class [[eosio::contract("eosio.token")]] token : public eosio::contract {
   public:
      using eosio::contract::contract;

      [[eosio::action]]
      void transfer( eosio::name        from,
                     eosio::name        to,
                     eosio::asset       quantity,
                     const std::string& memo );
      using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;
   };
}

class [[eosio::contract("eospay")]] eospay : public eosio::contract {
  public:
    eospay( eosio::name self, eosio::name first_receiver, eosio::datastream<const char*> ds ):eosio::contract(self,first_receiver,ds),variable_controller(self),gcontract_controller(self),player_controller(self),channel_controller(self),order_controller(self){}

/*
    [[eosio::action]]
    void cleanall();
*/

    [[eosio::action]]
    void referral(name send, name ref, name reg);
    
    [[eosio::action]]
    void cvariable(const std::vector<rvariable> &rules, bool truncate) 
    {
        variable_controller.get_rvariable_rule().create_rules(rules, truncate);
    }

    [[eosio::action]]
    void cgcontract(const std::vector<gcontract> &rules, bool truncate) 
    {
        gcontract_controller.get_gcontract_rule().create_rules(rules, truncate);
    }

    [[eosio::action]]
    void newchannel(const name& owner, const uint64_t& rate) 
    {
        require_auth( get_self() );
        channel_controller.new_channel(owner, rate);
    }

    [[eosio::action]]
    void closechannel(const name& owner) 
    {
        require_auth( get_self() );
        channel_controller.change_channel_status(owner, false);
    }

    [[eosio::action]]
    void clearchannel(const name& owner) 
    {
        require_auth( get_self() );
        channel_controller.erase_channel(owner);
    }

    [[eosio::action]]
    void modchanowner(const name& owner, const name& newowner) 
    {
        require_auth( get_self() );
        channel_controller.change_channel_owner(owner, newowner);
    }

    [[eosio::action]]
    void modchanrate(const name& owner, const uint64_t& rate) 
    {
        require_auth( get_self() );
        channel_controller.change_channel_rate(owner, rate);
    }
    
    [[eosio::action]]
    void chanbonuspay(const name& owner);

    /*接收eos转账通知,做相应的业务处理*/
    void eos_transfer ( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );

    /*接收自定义的代币转账通知,做相应的业务处理*/
    void cust_transfer( name    from,
                        name    to,
                        asset   quantity,
                        string  memo );

    [[eosio::action]]
    void updateorder(const name& from, const uint64_t& orderno) 
    {
        /*校验调用方是否合法*/
        auto& gcontracts = gcontract_controller.get_gcontract_rule();
        auto& rules = gcontracts.get_table();
        auto rule = rules.find(from.value);
        check(rule != rules.end(), "is not system contract.." );

        require_auth(from);

        order_control::OrderLogStruct order;
        order.order_no = orderno;
        order_controller.update_order(order);
    }

  private:
    struct memo
    {
        uint64_t order_no; /*订单号*/
        string good_name;  /*商品名称*/
        name   referral; /*推荐人*/
        name   channel;  /*渠道商*/
    };

    void parseRechargeMemo(string memo, struct memo& ret);

  private:
    variable_control variable_controller;      /*参数配置操作类*/
    gcontract_control gcontract_controller;    /*调用白名单*/
    
    player_control player_controller;
    channel_control channel_controller;
    order_control order_controller;
};



