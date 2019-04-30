cd /home/wangyilin/wyl/work/eos/eospay/eosio.token

eosio-cpp -o eosio.token.wasm eosio.token.cpp -abigen

cleos system newaccount  --transfer eosio estoken.e EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV --stake-net "1.0000 EOS" --stake-cpu "1.0000 EOS" --buy-ram "10.0000 EOS"

cleos set contract estoken.e ../eosio.token/ -p estoken.e

cleos push action estoken.e create '["tokenbonus.e", "1000000000.0000 ES"]' -p estoken.e

cleos get currency stats estoken.e ES


