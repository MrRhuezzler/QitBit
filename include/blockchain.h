#pragma once

#include "breep/network/tcp.hpp"
#include "breep/util/serialization.hpp"

#include "block.h"
#include "transaction.h"

#include <vector>
#include <string>

class Transaction;
class Block;
class q_blockchain;
class q_transaction;
class q_block;

class Blockchain {

    std::vector<Block*> chain;
    std::vector<Transaction*> pending;

    int difficulty;
    double mining_reward;
    int number_of_transactions_per_block;

    public:
        Blockchain(const q_blockchain& bc);
        Blockchain();
        ~Blockchain();

        // getters
        int get_difficulty() const;
        double get_mining_reward() const;
        int get_number_of_transactions_per_block() const;
        const std::vector<Block*>& get_chain() const;
        const std::vector<Transaction*>& get_pending() const;
        const Transaction* get_last_transaction() const;

        double mine_pending_transactions();
        
        // void add_transactions(std::string sender_address, std::string reciever_address, double amount);
        void add_transactions(q_transaction trans);
        
        void update_blockchain_if_longer(const Blockchain& other);

        friend std::ostream& operator<<(std::ostream& out, const Blockchain& b);

        double get_balance(std::string user);


    private:
        Block* genesis_blk();
        Block* get_last_blk();

        void replace(const Blockchain& bc);

        static bool verify_block_chain(const Blockchain& bc);

    // BREEP_ENABLE_SERIALIZATION(Blockchain, chain, pending, difficulty, mining_reward, number_of_transactions_per_block);

};

// BREEP_DECLARE_TYPE(Blockchain);
