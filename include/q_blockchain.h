#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "blockchain.h"

#include "breep/network/tcp.hpp"

BREEP_DECLARE_TYPE(std::string);
BREEP_DECLARE_TEMPLATE(std::vector);

class Transaction;
class Block;
class Blockchain;

class q_transaction {


    std::string data;

    public:
        q_transaction();
        q_transaction(const q_transaction& trans);
        q_transaction(const Transaction& trans);
        q_transaction(std::string sender, std::string reciever, double amt);
        const std::string& get_data() const;

        BREEP_ENABLE_SERIALIZATION(q_transaction, data);

};

BREEP_DECLARE_TYPE(q_transaction);


class q_block {

    std::vector<std::string> transactions;
    std::string blk_data;

    public:
        q_block(const Block& blk);
        q_block();
        q_block(const q_block& q_blk);

        const std::vector<std::string>& get_trans() const;
        const std::string& get_data() const;

        BREEP_ENABLE_SERIALIZATION(q_block, transactions, blk_data);

};

BREEP_DECLARE_TYPE(q_block);

class q_blockchain {

    std::vector<q_block> chain_data;
    std::vector<std::string> pending_data;
    int difficulty;
    double mining_reward;
    int number_of_transactions_per_block;

    public:
        q_blockchain(const Blockchain& q_bc);
        q_blockchain();
        q_blockchain(const q_blockchain &q);

        // getters
        int get_difficulty() const;
        int get_mining_reward() const;
        int get_number_of_transactions_per_block() const;
        const std::vector<std::string>& get_pending() const;
        const std::vector<q_block>& get_chain() const;

        friend std::ostream& operator<<(std::ostream& out, const q_blockchain &q);

        BREEP_ENABLE_SERIALIZATION(q_blockchain,
                               chain_data,
                               pending_data,
                               difficulty,
                               mining_reward,
                               number_of_transactions_per_block);

};

BREEP_DECLARE_TYPE(q_blockchain);