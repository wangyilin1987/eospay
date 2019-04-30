/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

class order_control : public eosio::contract 
{
    public:
        order_control( name self ):eosio::contract(self,self,datastream<const char*>(nullptr, 0)),orderlogs(self,self.value){}

        typedef struct [[eosio::table, eosio::contract("eospay")]] orderlog
        {
            uint64_t order_no; /*订单号*/
            name   from;       /*充值用户*/
            asset  pay;        /*付款数量*/
            checksum256 transaction_id;/*交易id*/  
            uint8_t status = 0;  /*订单状态;0-等待充值;1-充值成功*/
            time_point_sec time; /*交易时间*/

            uint64_t primary_key()const { return order_no; }
            uint64_t get_name()const { return from.value; }

            EOSLIB_SERIALIZE( orderlog, (order_no)(from)(pay)(transaction_id)(status)(time))
        }OrderLogStruct;

        //typedef eosio::multi_index<"orderlog"_n, orderlog,eosio::indexed_by< "byname"_n, eosio::const_mem_fun<orderlog, uint64_t, &orderlog::get_name>>> orderlog_table;
        typedef eosio::multi_index<"orderlog"_n, orderlog,
                                    eosio::indexed_by< "byname"_n, eosio::const_mem_fun<orderlog, uint64_t, &orderlog::get_name>>
                                  > orderlog_table;
/*        typedef eosio::multi_index<"orderlog"_n, orderlog,
                                            eosio::indexed_by< "byname"_n, eosio::const_mem_fun<orderlog, uint64_t, &orderlog::get_name>>,
                                            eosio::indexed_by< "bytransid"_n, eosio::const_mem_fun<orderlog, uint64_t, &orderlog::get_status>>
                                        > orderlog_table;*/
        orderlog_table orderlogs;

    public:
        /*登记订单*/
        void new_order(const struct orderlog& order)
        {
            auto itr = orderlogs.find( order.order_no );
            check(itr == orderlogs.end(), "error! order_no had exist.." );

            checksum256 transaction_id;
            auto size = transaction_size();
            char buf[size];
            uint32_t read = read_transaction( buf, size );
            check( size == read, "read_transaction failed");
            transaction_id = sha256(buf, read);

            itr = orderlogs.emplace( get_self(), [&]( auto& tb ) {
                tb.order_no = order.order_no;
                tb.from = order.from;
                tb.pay = order.pay;
                tb.transaction_id = transaction_id;
                tb.status = 0;                
                tb.time = current_time_point_sec();
            });
        }

        void update_order(const struct orderlog& order)
        {
            auto itr = orderlogs.find( order.order_no );
            check(itr != orderlogs.end(), "error! order_no not found.." );

            check(itr->status == 0, "error! order's status had be 1.." );

            orderlogs.modify( itr, same_payer, [&]( auto& tb ) {
                tb.status = 1;
            });
        }

        order_control::orderlog_table::const_iterator get_order_cbegin() 
        {
            return orderlogs.cbegin();
        }
        
        order_control::orderlog_table::const_iterator get_order_cend() 
        {
            return orderlogs.cend();
        }
        
        order_control::orderlog_table::const_iterator get_order_iter(const uint64_t& order_no) 
        {
            auto itr = orderlogs.find( order_no );
            check(itr != orderlogs.end(), "order need create.." );
            
            return itr;
        } 

        auto get_name_index() 
        {
            auto itr = orderlogs.get_index<"byname"_n>();
            return itr;
        }

        void clean()
        {
            // 判断是否是调试状态
            print("cleanall begin");
            assert_debug();

            auto itr = orderlogs.cbegin();
            while(itr != orderlogs.cend()) 
            {
                itr = orderlogs.erase(itr);
            }
        }
};


