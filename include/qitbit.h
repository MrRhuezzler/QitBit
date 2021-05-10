#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "types.h"
#include "blockchain.h"
#include "q_blockchain.h"

#include "breep/network/tcp.hpp"

class qitbit {

    public:
        void on_connection(breep::tcp::network& network, const breep::tcp::peer& peer);
        void on_incoming_blockchain(Blockchain& bc, breep::tcp::netdata_wrapper<q_blockchain>& data);
        void on_incoming_transaction(Blockchain& bc, breep::tcp::netdata_wrapper<q_transaction>& data);

};
