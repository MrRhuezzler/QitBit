#include <blockchain.h>

std::ostream& operator<<(std::ostream& out, const Blockchain& b){

    for(const Block *blk : b.chain){
        out << *blk << std::endl;
    }

    out << "------------ Pending ------------" << std::endl;

    for(const Transaction *trans : b.pending){
        out << *trans << std::endl;
    }

    return out;

}

Blockchain::Blockchain(const q_blockchain& bc)
: difficulty(bc.get_difficulty()),
number_of_transactions_per_block(bc.get_number_of_transactions_per_block()),
mining_reward(bc.get_mining_reward())
{

    chain = std::vector<Block*>();
    pending = std::vector<Transaction*>();

    for(const std::string& trans : bc.get_pending()){
        pending.emplace_back(new Transaction(trans));
    }

    for(const q_block& blk : bc.get_chain()){
        chain.emplace_back(new Block(blk));
    }

}

Blockchain::Blockchain()
: difficulty(2),
number_of_transactions_per_block(10),
mining_reward(50.0)
{

    chain = std::vector<Block*>();
    pending = std::vector<Transaction*>();

    chain.push_back(genesis_blk());

}

double Blockchain::mine_pending_transactions(){

    int n = 0;
    double time_elapsed = 0.0;

    if(pending.size() > 0){

        while(pending.size() > 0)
        {

            std::vector<Transaction*> slice = std::vector<Transaction*>();
            n = 0;

            while(n != number_of_transactions_per_block && pending.size() > 0)
            {
                
                slice.push_back(pending.back());
                pending.pop_back();
                n++;

            }

            Block *new_block = new Block(chain.size(), slice, time(0), get_last_blk()->get_hash());
            time_elapsed = new_block->mine_block(difficulty);

            chain.push_back(new_block);

        }

    }

    return time_elapsed;

}
void Blockchain::add_transactions(q_transaction trans){
// void Blockchain::add_transactions(std::string sender_address, std::string reciever_address, double amount) {

    Transaction *new_transaction = new Transaction(trans);
    pending.push_back(new_transaction);

}

Block* Blockchain::get_last_blk(){ return chain.back(); }

Block* Blockchain::genesis_blk(){

    std::vector<Transaction*> genesis_transaction = {new Transaction("Miner Rewards", "bob", mining_reward)};
    Block* genesis = new Block(0, genesis_transaction, time(0), "");
    genesis->mine_block(difficulty);
    return genesis;

}

int Blockchain::get_difficulty() const {return difficulty;}
double Blockchain::get_mining_reward() const {return mining_reward;}
int Blockchain::get_number_of_transactions_per_block() const {return number_of_transactions_per_block;}
const std::vector<Block*>& Blockchain::get_chain() const {return chain;}
const std::vector<Transaction*>& Blockchain::get_pending() const {return pending;}

void Blockchain::replace(const Blockchain& bc){

    difficulty = bc.get_difficulty();
    mining_reward = bc.get_mining_reward();
    number_of_transactions_per_block = bc.get_number_of_transactions_per_block();

    chain.clear();
    for(const Block *blk : bc.get_chain()){
        chain.emplace_back(new Block(*blk));
    }

    pending.clear();
    for(const Transaction *trans : bc.get_pending()){
        pending.emplace_back(new Transaction(*trans));
    }

}

void Blockchain::update_blockchain_if_longer(const Blockchain& other){

    if(!Blockchain::verify_block_chain(other)){
        return ;
    }

    if(other.get_chain().size() > chain.size()){
        replace(other);
    }else if(other.get_chain().size() == chain.size()){
        if(other.get_pending().size() > pending.size()){
            replace(other);
        }
    }

}

bool Blockchain::verify_block_chain(const Blockchain& bc){

    if(bc.get_chain().size() == 1){
        return true;
    }

    auto chain = bc.get_chain();

    for(int i = 1; i < chain.size(); i++){
        
        Block *prev_blk = chain[i - 1];
        Block *curr_blk = chain[i];

        if(curr_blk->get_prev_hash() != prev_blk->get_hash()){
            return false;
        }

    }

    return true;

}

double Blockchain::get_balance(std::string user){

    double amt;

    for(const Block* blk : chain){
        for(const Transaction* trans : blk->get_transactions()){
            
            if(trans->get_sender() == user){
                amt -= trans->get_amount();
            }

            if(trans->get_receiver() == user){
                amt += trans->get_amount();
            }

        }
    }

    return amt;

}

const Transaction* Blockchain::get_last_transaction() const {return pending.back();}

Blockchain::~Blockchain(){

    for(const Block *b : chain){
        delete b;
    }

    for(const Transaction *t : pending){
        delete t;
    }

}