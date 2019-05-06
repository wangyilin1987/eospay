/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include "eospay.hpp"

void eospay::parseRechargeMemo(string memo, struct memo& ret)
{
    auto parts = split(memo, ",");

    auto buf = split(parts[1], "|");

    /*参数至少需要有4个;订单号|商品名称|推荐人|渠道商*/
    check(buf.size() >= 4, "incorrect number of parameters" );
    auto order_num = buf[0];

    /*订单号不能为空*/
    check(order_num.length(), "error!order_no is null" );

    /*订单号必须为数字*/
    check(isdigit_my(order_num), "error!order_no must be number");

    /*订单号*/
    ret.order_no = atoll(order_num.c_str());

    print("order_num:",order_num,";order_no:",ret.order_no,";");

    /*商品名称*/
    ret.good_name = buf[1];
    print("good:",ret.good_name,";");

    /*推荐人*/
    ret.referral = name(buf[2].c_str());
    print("referral:",ret.referral,";");

    /*渠道商*/
    ret.channel = name(buf[3].c_str());
    print("channel:",ret.channel,";");

    print("parseRechargeMemo end.");
}

/*
void eospay::cleanall()
{
    // 判断是否是调试状态
    print_f("cleanall begin");
    assert_debug();
    require_auth( get_self() );
    
    player_controller.clean();
}
*/

void eospay::chanbonuspay(const name& owner)
{
    asset fund(0, symbol("EOS",4));
    require_auth( get_self() );
    
    fund = channel_controller.withdraw_channel_fund(owner);
    if(fund.amount > 0)
    {
        transaction out;
        eosio::token::transfer_action intransfer{"eosio.token"_n, { get_self(), "active"_n }};
        out.actions.emplace_back(intransfer.to_action( get_self(), owner, fund, std::string("bonus") ));
        out.delay_sec = 0;
        out.send( get_self().value, get_self() );
    }
}

void eospay::referral(name send, name ref, name reg)
{
    require_auth(send);
    
    /*校验推荐人必须已注册*/
    check( !player_controller.is_empty_player(ref), "ref need regist." );

    /*校验被推荐人必须末注册*/
    check( player_controller.is_empty_player(reg), "reg must new player." );

    /// initialize  balance
    player_controller.new_player(reg, ref);
}

void eospay::eos_transfer(name    from,
                          name    to,
                          asset   quantity,
                          string  memo)
{
    check( quantity.symbol == symbol("EOS",4), "symbol must be EOS" );

    if( from == get_self() )
    {
        /*忽略_self transfer其他token的通知*/
        return;
    }

    if( to != get_self() ) 
    {
        /*防止"假转账通知"攻击*/
        return;
    }
    check(quantity.amount > 0, "must purchase a positive amount.");

    if(memo.substr(0,8) == "recharge")
    {
        /*充值入口*/
        struct memo mo;
        parseRechargeMemo(memo, mo);

        order_control::OrderLogStruct order;
        order.order_no = mo.order_no;
        order.from = from;
        order.pay = quantity;
        /*创建充值订单*/
        order_controller.new_order(order);       
    }
    player_controller.new_player(from, to);
}

void eospay::cust_transfer(name    from,
                           name    to,
                           asset   quantity,
                           string  memo)
{
    check( quantity.symbol == symbol(GAME_TOKEN,4), "symbol must be GAME_TOKEN" );

    if( from == get_self() )
    {
        /*忽略_self transfer其他token的通知*/
        return;
    }

    if( to != get_self() ) 
    {
        /*防止"假转账通知"攻击*/
        return;
    }
    check(quantity.amount > 0, "must purchase a positive amount.");

    print_f("from: %.", from);
    print_f("to: %.", to);
    print("end cust_transfer..");
}

extern "C" {
    void apply( uint64_t receiver, uint64_t code, uint64_t action ) 
    {
        if( action == "onerror"_n.value) {
            /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ 
            check(code == "eosio"_n.value, "onerror action's are only valid from the \"eosio\" system account");
        }        

        if(code == name("eosio.token").value && action == name("transfer").value)
        {
            eosio::execute_action(name(receiver), name(code), &eospay::eos_transfer);
        }
        else if(code == name(TOKEN_CONTRACT).value && action == name("transfer").value)
        {
            eosio::execute_action(name(receiver), name(code), &eospay::cust_transfer);
        }
        else if( code == receiver )
        {
            switch( action )
            { 
                EOSIO_DISPATCH_HELPER(eospay, (referral)(cvariable)(cgcontract)(newchannel)(closechannel)(modchanowner)(modchanrate)(chanbonuspay)(clearchannel)(updateorder))
            } 
            /* does not allow destructor of thiscontract to run: eosio_exit(0); */ 
        } 
    }
}
