#include <q_blockchain.h>

// ---------------- q_blockchain ----------------

q_blockchain::q_blockchain(const Blockchain& q_bc){

    chain_data = std::vector<q_block>();
    pending_data = std::vector<std::string>();

    difficulty = q_bc.get_difficulty();
    mining_reward = q_bc.get_mining_reward();
    number_of_transactions_per_block = q_bc.get_number_of_transactions_per_block();

    const std::vector<Block*>& blocks = q_bc.get_chain();
    const std::vector<Transaction*>& trans = q_bc.get_pending();

    for(const Block *b : blocks){
        chain_data.emplace_back(q_block(*b));
    }

    for(const Transaction *t : trans){
        pending_data.emplace_back(t->to_string());
    }

}

q_blockchain::q_blockchain()
:difficulty(), mining_reward(0), number_of_transactions_per_block(0)
{}

q_blockchain::q_blockchain(const q_blockchain &q)
: difficulty(q.difficulty), mining_reward(q.mining_reward),
number_of_transactions_per_block(q.number_of_transactions_per_block),
chain_data(q.chain_data), pending_data(q.pending_data)
{}


int q_blockchain::get_difficulty() const {return difficulty;}
int q_blockchain::get_mining_reward() const {return mining_reward;}
int q_blockchain::get_number_of_transactions_per_block() const {return number_of_transactions_per_block;}
const std::vector<std::string>& q_blockchain::get_pending() const {return pending_data;}
const std::vector<q_block>& q_blockchain::get_chain() const {return chain_data;}

std::ostream& operator<<(std::ostream& out, const q_blockchain &q){
    out << q.difficulty << "|" << q.mining_reward << "|" << q.number_of_transactions_per_block<< std::endl;
    out << "----- Block Data ----" << std::endl;
    for(const q_block &blk : q.chain_data){
        out << blk.get_data() << std::endl;
        out << "------- Transactions -------" << std::endl;
        for(const std::string& trans: blk.get_trans()){
            out << trans << std::endl;
        }
    }
    out << "--------------------" << std::endl;
    out << "----- Pending Data ----" << std::endl;
    for(const std::string &trans : q.pending_data){
        out << trans << std::endl;
    }
    return out;
}



// ---------------- q_block ----------------

q_block::q_block(const Block& blk)
:blk_data(blk.to_string())
{

    transactions = std::vector<std::string>();
    for(const Transaction *q : blk.get_transactions()){
        transactions.emplace_back(q->to_string());
    }

}

q_block::q_block() {}

q_block::q_block(const q_block& q_blk)
: blk_data(q_blk.blk_data),
transactions(q_blk.transactions)
{}

const std::vector<std::string>& q_block::get_trans() const {return transactions;}
const std::string& q_block::get_data() const {return blk_data;}


// ----------------- q_transaction --------------

q_transaction::q_transaction(std::string sender, std::string reciever, double amt){
    
    std::stringstream s;
    s << sender << "|" << reciever << "|" << std::to_string(amt);
    data = s.str();

}

q_transaction::q_transaction()
:data("") {}

q_transaction::q_transaction(const q_transaction& trans)
:data(trans.data)
{}

q_transaction::q_transaction(const Transaction& trans)
{

    std::stringstream s;
    s << trans.get_sender() << "|" << trans.get_receiver() << "|" << std::to_string(trans.get_amount());
    data = s.str();

}

const std::string& q_transaction::get_data() const {return data;}
