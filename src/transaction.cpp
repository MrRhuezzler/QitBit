#include <transaction.h>

std::ostream& operator<<(std::ostream& out, const Transaction &a){

    out << "Time : " << a.get_time() << std::endl;
    out << "Sender : " << a.get_sender() << std::endl;
    out << "Receiver : " << a.get_receiver() << std::endl;
    out << "Amount : " << a.get_amount() << std::endl;
    out << "Hash : " << a.get_hash();
    return out;

}

Transaction::Transaction(const q_transaction& trans){
    
    std::stringstream stream(trans.get_data());

    std::string temp;

    int i = 0;
    while(stream.good()){
        getline(stream, temp, '|');

        switch(i){
            case 0:
                sender_address = temp;
                break;
            case 1:
                receiver_address = temp;
                break;
            case 2:
                amount = std::stod(temp);
                break;
        }

        i++;
    }

    time_t _time = time(nullptr);
    tm *utc = gmtime(&_time);
    time_string = std::string(asctime(utc));
    time_string.pop_back();

    hash_string = SHA256(time_string + sender_address + receiver_address + std::to_string(amount)).str();

}

// FROM|TO|50.0|May 2019|Hdjhkdfuekr
Transaction::Transaction(const std::string& string_format){

   std::stringstream stream(string_format);
   std::string substr;

   int i = 0;
   while(stream.good()){

       getline(stream, substr, '|');
       switch (i)
       {
       case 0:
           sender_address = substr;
           break;
       case 1:
           receiver_address = substr;
           break;
       case 2:
           amount = std::stod(substr);
           break;
       case 3:
           time_string = substr;
           break;
       case 4:
           hash_string = substr;
       }

       i++;
   }

}

Transaction::Transaction(std::string sender_address, std::string receiver_address, double amount)
: sender_address(sender_address),
  receiver_address(receiver_address),
  amount(amount)
{

    time_t _time = time(nullptr);
    tm *utc = gmtime(&_time);
    time_string = std::string(asctime(utc));
    time_string.pop_back();

    hash_string = SHA256(time_string + sender_address + receiver_address + std::to_string(amount)).str();

}

std::string Transaction::get_time() const { return time_string; }
std::string Transaction::get_sender() const { return sender_address; }
std::string Transaction::get_receiver() const { return receiver_address; }
double Transaction::get_amount() const { return amount; }
std::string Transaction::get_hash() const { return hash_string; }

std::string Transaction::to_string() const {

    std::stringstream s;
    // FROM|TO|50.0|May 2019|Hdjhkdfuekr
    s << sender_address << "|"
      << receiver_address << "|"
      << amount << "|"
      << time_string << "|"
      << hash_string;

    return s.str();

}
