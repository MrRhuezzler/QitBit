#pragma once

#include "breep/network/tcp.hpp"
#include "breep/util/serialization.hpp"

#include "SHA256.h"
#include "q_blockchain.h"

#include <string>
#include <ctime>
#include <algorithm>
#include <sstream>

class q_transaction;

class Transaction {

    std::string sender_address;
    std::string receiver_address;
    double amount;
    std::string time_string;
    std::string hash_string;

public:

    Transaction(const q_transaction& trans);
    Transaction(std::string sender, std::string receiver, double amount);
    Transaction(const std::string& string_format);

    std::string get_time() const;
    std::string get_sender() const;
    std::string get_receiver() const;
    double get_amount() const;
    std::string get_hash() const;

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& out, const Transaction &a);

};