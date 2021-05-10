#include <qitbit.h>

void qitbit::on_connection(breep::tcp::network& network, const breep::tcp::peer& peer){

    if(peer.is_connected()){

        // std::cout << peer.id_as_string() << " just connected !" << std::endl;

    }else{
        // std::cout << peer.id_as_string() << " disconnected !" << std::endl;
    }

}


void qitbit::on_incoming_blockchain(Blockchain& bc, breep::tcp::netdata_wrapper<q_blockchain>& data){

    Blockchain incoming = Blockchain(data.data);
    bc.update_blockchain_if_longer(incoming);

}

void qitbit::on_incoming_transaction(Blockchain& bc, breep::tcp::netdata_wrapper<q_transaction>& data){
    bc.add_transactions(data.data);
}