!注意:检查确认编译版本是否生产版本(util.hpp的 DEBUG宏为 false;TOKEN_CONTRACT宏为生产的代币合约)


cd $HOME/wyl/work/eos/eospay
eosio-cpp -o eospay.wasm eospay.cpp -abigen

cleos system newaccount  --transfer eosio tokenbonus.e EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV --stake-net "1.0000 EOS" --stake-cpu "1.0000 EOS" --buy-ram "10.0000 EOS"

cleos set account permission tokenbonus.e active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"tokenbonus.e","permission":"eosio.code"},"weight":1}]}' owner -p tokenbonus.e

cleos set contract tokenbonus.e ../eospay/ -p tokenbonus.e

cleos push action tokenbonus.e cvariable '[[{"key":1,"value":10000},{"key":2,"value":8}], true]' -p tokenbonus.e
cleos get table tokenbonus.e tokenbonus.e rvariable

/*设置有权限的游戏合约*/
cleos push action tokenbonus.e cgcontract '[[{"key":"threecards.e"}], true]' -p tokenbonus.e
cleos push action tokenbonus.e cgcontract '[[{"key":"eoswangyilin"}], false]' -p tokenbonus.e

cleos get table tokenbonus.e tokenbonus.e gcontract

cleos push action tokenbonus.e referral '[ "game", "eoswangyilin", "killua.x"]' -p game

cleos push action tokenbonus.e updateorder '[ "eoswangyilin", 10003]' -p eoswangyilin


cleos push action eosio.token transfer '[ "eoswangyilin", "tokenbonus.e", "1.0000 EOS", "recharge,10001|充值100元宝|tokenbonus.e|meet.one"]' -p eoswangyilin
cleos push action eosio.token transfer '[ "killua.x", "tokenbonus.e", "2.0000 EOS", "recharge,10003|充值200元宝|tokenbonus.e|meet.one"]' -p killua.x

cleos push action eosio.token transfer '[ "threecards.e", "tokenbonus.e", "0.1000 EOS", "issue4game,eoswangyilin|200000"]' -p  threecards.e
cleos push action eosio.token transfer '[ "threecards.e", "tokenbonus.e", "10.0000 EOS", "issue4game,eoswangyilin|20000,killua.x|10000,luffy.x|70000,satan.x|10000,game|10"]' -p  threecards.e
cleos push action eosio.token transfer '[ "threecards.e", "tokenbonus.e", "10.0000 EOS", "issue4game,eoswangyilin|20000,killua.x|10000,luffy.x|40000"]' -p  threecards.e

cleos get currency balance estoken.e  peesexsexsex

cleos get currency balance eosio.token  eoswangyilin

cleos get table tokenbonus.e tokenbonus.e pool

cleos get table tokenbonus.e tokenbonus.e playerid
cleos get table tokenbonus.e tokenbonus.e orderlog
cleos get table tokenbonus.e tokenbonus.e channel
cleos get table tokenbonus.e tokenbonus.e channelid

cleos push action tokenbonus.e dividend '[13]' -p tokenbonus.e


cleos push action tokenbonus.e newchannel '['starteos', 10000]' -p tokenbonus.e
cleos push action tokenbonus.e closechannel '['starteos']' -p tokenbonus.e
cleos push action tokenbonus.e modchanowner '['starteos', 'a']' -p tokenbonus.e
cleos push action tokenbonus.e modchanrate '['starteos', 100]' -p tokenbonus.e
cleos push action tokenbonus.e chanbonuspay '['starteos']' -p tokenbonus.e
cleos push action tokenbonus.e clearchannel '['starteos']' -p tokenbonus.e

cleos push action eosio.token transfer '[ "threecards.e", "tokenbonus.e", "0.1000 EOS", "channeldividend,eoswangyilin|10000|meetone,killua.x|20000|starteos,luffy.x|30000|meetone,game|50000|meetone"]' -p  threecards.e


------------RPC调用调试

http://47.96.237.226:8888/v1/chain/get_table_rows
{
  "code": "tokenbonus.e",
  "table": "player",
  "scope": "tokenbonus.e",
  "json":"true",
  "lower_bound":"abcdef2",
  "upper_bound":"abcdef2a"
}