/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

class player_control : public eosio::contract 
{
    public:
        player_control( name self ):eosio::contract(self,self,datastream<const char*>(nullptr, 0)),playerids(self,self.value),players(self,self.value){}

        struct [[eosio::table, eosio::contract("eospay")]] playerid {
            uint64_t id;
            uint64_t primary_key()const { return 0; }
            EOSLIB_SERIALIZE( playerid, (id))
        };

        struct [[eosio::table, eosio::contract("eospay")]] player
        {
            name   owner;
            uint64_t id;     /*用户序号*/
            name   referral; /*推荐人*/
            asset  buying;   /*总充值数量*/
            asset  reward;   /*累计用户收到的推荐奖励*/
            time_point_sec last_buy; /*上一次充值时间*/

            uint64_t primary_key()const { return owner.value; }
            uint64_t get_id()const { return id; }
            EOSLIB_SERIALIZE( player, (owner)(id)(referral)(buying)(reward)(last_buy))
        };

        typedef eosio::multi_index<"player"_n, player,eosio::indexed_by< "byid"_n, eosio::const_mem_fun<player, uint64_t, &player::get_id>>> players_table;
        typedef eosio::multi_index<"playerid"_n, playerid> playerid_table;
        
        playerid_table playerids;
        players_table players;

    public:
        /*校验用户是否已注册*/
        bool is_empty_player(const name& player) 
        {
            bool ret = false;
            auto itr = players.find( player.value );
            if ( itr == players.end())
            {
                ret = true;
            }
            
            return ret;
        }

        /*注册用户*/
        void new_player(const name& player, const name& referral)
        {
            auto itr = players.find( player.value );
            if ( itr == players.end())
            {
                /// initialize  balance
                itr = players.emplace( get_self(), [&]( auto& tb ) {
                    symbol symbol(symbol("EOS",4));
                    tb.owner = player;
                    tb.id = next_id();
                    tb.referral = referral;
                    tb.buying = asset{0, symbol};
                    tb.reward = asset{0, symbol};
                    tb.last_buy = time_point_sec::min();
                });
            }
        }

        player_control::players_table::const_iterator get_player_cbegin() 
        {
            return players.cbegin();
        }
        
        player_control::players_table::const_iterator get_player_cend() 
        {
            return players.cend();
        }
        
        player_control::players_table::const_iterator get_player_iter(const name& player) 
        {
            auto itr = players.find( player.value );
            check(itr != players.end(), "player need register.." );
            
            return itr;
        }
        
        name get_referral(const name& player) 
        {
            auto itr = players.find( player.value );
            check(itr != players.end(), "player need register.." );
            
            return itr->referral;
        }

        uint64_t next_id() 
        {
            auto itr = playerids.begin();
            if(itr == playerids.end())
            {
                itr = playerids.emplace( get_self(), [&]( auto& tb ) {
                    tb.id = 0;
                });
            }
            
            playerids.modify( itr, same_payer, [&]( auto& rb ) {
                rb.id += 1;
            }); 

            return itr->id;
        }
        
        uint64_t get_id() 
        {
            auto itr = playerids.begin();
            return itr->id;
        }

        auto get_id_index() 
        {
            auto itr = players.get_index<"byid"_n>();
            return itr;
        }
        
        void clean()
        {
            // 判断是否是调试状态
            print("cleanall begin");
            assert_debug();

            auto itr = players.cbegin();
            while(itr != players.cend()) 
            {
                itr = players.erase(itr);
            }
            
            auto itr1 = playerids.cbegin();
            while(itr1 != playerids.cend()) 
            {
                itr1 = playerids.erase(itr1);
            }
        }
};


