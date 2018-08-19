#include <eosiolib/eosio.hpp>

/*
#include <eosiolib/types.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/asset.hpp>
*/

#include <string>
#include <vector>

namespace foodapp{
    using std::string;
    using std::vector;

    //@abi table
    struct order{
        account_name customer;
        vector<string> items;
        string first_name;
        string last_name;
        uint64_t primary_key() const { return customer; }
        EOSLIB_SERIALIZE( order, (customer)(items)(first_name)(last_name))
    };

    typedef eosio::multi_index<N(orders), order> order_table;

    class foodapp_contract : public eosio::contract{
        public:
            explicit foodapp_contract(action_name self);

            //@abi action
            void makeorder(account_name customer, const vector<string>& items, const string& first_name, const string&
            last_name){
                require_auth(customer);

                eosio_assert(first_name.size()>0, "provide a first name");
                eosio_assert(last_name.size()>0, "provide a last name");
                order_table orders(_self, _self);
                auto itr = orders.find(customer);
                eosio_assert(itr==orders.end(), "you placed an order already");

                orders.emplace(customer, [&](auto& order){
                    order.customer = customer;
                    order.items = items;
                    order.first_name = first_name;
                    order.last_name = last_name;
                });
            }

            //@abi action
            void changeorder(account_name customer, const vector<string>& items, const string& first_name, const string&
            last_name){
                require_auth(customer);

                eosio_assert(first_name.size()>0, "provide a first name");
                eosio_assert(last_name.size()>0, "provide a last name");
                order_table orders(_self, _self);
                auto itr = orders.find(customer);
                eosio_assert(itr!=orders.end(), "you haven't placed an order already");

                orders.modify(itr, 0, [&](auto& order){
                    order.customer = customer;
                    order.items = items;
                    order.first_name = first_name;
                    order.last_name = last_name;
                });
            }

            //@abi action
            void removeorder(account_name customer){
                require_auth(customer);

                order_table orders(_self, _self);
                auto itr = orders.find(customer);
                eosio_assert(itr!=orders.end(), "you haven't placed an order already");

                orders.erase(itr);
            }

            //@abi action
            void clearorder(){

            }
    };

    foodapp_contract::foodapp_contract(action_name self) : contract(self){

    }
}

EOSIO_ABI( foodapp::foodapp_contract, (makeorder) (changeorder) (removeorder) (clearorder) )
//
// Created by Jae Chung on 14/8/2018.
//
