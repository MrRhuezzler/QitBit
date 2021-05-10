#include <iostream>
#include <string>

#include <block.h>

std::ostream& operator<<(std::ostream& out, const Block& blk){

    out << "--" << blk.get_index() << "--" << std::endl;
    out << "Timestamp : " << blk.get_time() << std::endl;
    out << "Previous Block Hash : " << blk.get_prev_hash() << std::endl;
    out << "Block Hash : " << blk.get_hash() << std::endl;
    out << "Nonce : " << blk.get_nonce() << std::endl;
    const std::vector<Transaction*>& trans = blk.get_transactions();
    out << "--------------- TRANSACTIONS --------------------" << std::endl;
    for(const Transaction *t : trans) {
        out << *t << std::endl;
        out << "-------------------------------------------------" << std::endl;
    }
    return out;

}

Block::Block(const Block& blk)
:index(blk.get_index()),
prev_hash_string(blk.get_prev_hash()),
nonce(blk.get_nonce())
{

    transactions = std::vector<Transaction*>();
    for(const Transaction* trans : blk.get_transactions()){
        transactions.emplace_back(new Transaction(*trans));
    }

}

// s << index << "|"
//   << time_string << "|"
//   << transactions_hash_string << "|"
//   << hash_string << "|"
//   << nonce << "|"
//   << prev_hash_string;
Block::Block(const q_block& blk){

    std::stringstream stream(blk.get_data());
    std::string substr;

    int i = 0;
    while(stream.good()){

        getline(stream, substr, '|');
        switch (i)
        {
        case 0:
            index = std::stoi(substr);
            break;
        case 1:
            time_string = substr;
            break;
        case 2:
            transactions_hash_string = substr;
            break;
        case 3:
            hash_string = substr;
            break;
        case 4:
            nonce = std::stoi(substr);
            break;
        case 5:
            prev_hash_string = substr;
            break;
        }

        i++;
    }

    transactions = std::vector<Transaction*>();
    for(const std::string& trans : blk.get_trans()){
        transactions.emplace_back(new Transaction(trans));
    }

}

Block::Block(int index, std::vector<Transaction*> transactions, time_t _time, std::string prev_hash_string)
: index(index),
transactions(transactions),
prev_hash_string(prev_hash_string),
nonce(0)
{

    tm *utc = gmtime(&_time);
    time_string = std::string(asctime(utc));
    time_string.pop_back();

    transactions_hash_string = "";
    for(const Transaction *t : transactions){
        transactions_hash_string += t->get_hash();
    }

}

double Block::mine_block(int difficulty){

    std::string hash_check_string(difficulty, '0');

    std::string temp_hash = SHA256(std::to_string(index) +
                                   time_string +
                                   transactions_hash_string +
                                   prev_hash_string +
                                   std::to_string(nonce)).str();
    
    // std::cout << "Started to Mine ( Block : " << index << " )" << std::endl;

    auto st = std::chrono::high_resolution_clock::now();

    while (temp_hash.substr(0, difficulty) != hash_check_string){
        nonce += 1;
        temp_hash = SHA256(std::to_string(index) +
                           time_string +
                           transactions_hash_string +
                           prev_hash_string +
                           std::to_string(nonce)).str();
    }

    auto sp = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration<double, std::milli>(sp - st);

    // std::cout << "Completed Mining ( Block : " << index << " )" << " in " << time_taken.count() << "ms" << std::endl;
    
    hash_string =  SHA256(std::to_string(index) +
                   time_string +
                   transactions_hash_string +
                   prev_hash_string +
                   std::to_string(nonce)).str();


    return time_taken.count();

}

std::string Block::get_time() const { return time_string; }
std::string Block::get_hash() const { return hash_string; }
std::string Block::get_prev_hash() const { return prev_hash_string; }
int Block::get_nonce() const { return nonce; }
int Block::get_index() const { return index; }
const std::vector<Transaction*>& Block::get_transactions() const { return transactions; }

std::string Block::to_string() const {

    std::stringstream s;

    s << index << "|"
      << time_string << "|"
      << transactions_hash_string << "|"
      << hash_string << "|"
      << nonce << "|"
      << prev_hash_string;

    return s.str();

}

Block::~Block(){
    
    for(const Transaction *t : transactions){
        delete t;
    }

}