#pragma once

#include "breep/network/tcp.hpp"
#include "breep/util/serialization.hpp"

#include "transaction.h"
#include "q_blockchain.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <sstream>

class Transaction;
class q_block;

class Block {

    // time_t _time;

    int index;
    std::string time_string;

    std::vector<Transaction*> transactions;
    std::string transactions_hash_string;

    std::string prev_hash_string;
    std::string hash_string;

    int nonce;


public:

    Block(int index, std::vector<Transaction*> transactions, time_t _time, std::string prev_hash);
    Block(const q_block& blk);
    Block(const Block& blk);
    ~Block();

    std::string get_time() const;
    std::string get_hash() const;
    std::string get_prev_hash() const;
    int get_nonce() const;
    int get_index() const;
    const std::vector<Transaction*>& get_transactions() const;
    double mine_block(int difficulty);
    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& out, const Block& blk);

    // BREEP_ENABLE_SERIALIZATION(Block, index, time_string, transactions, transactions_hash_string, prev_hash_string, hash_string, nonce);

};

// BREEP_DECLARE_TYPE(Block);
