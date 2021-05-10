#include <breep/network/tcp.hpp>
#include <breep/util/serialization.hpp>

#include <iostream>
#include <string>

struct name {

    std::string name_;

    BREEP_ENABLE_SERIALIZATION(name, name_)
    
};

BREEP_DECLARE_TYPE(name)
BREEP_DECLARE_TYPE(std::string)

class p2pNetwork {

    name m_name;
    std::unordered_map<boost::uuids::uuid, std::string,  boost::hash<boost::uuids::uuid>> m_nicknames;

    public:

        p2pNetwork(const std::string& name)
        :m_nicknames() {
            m_name.name_ = name;
        }


        void on_connection(breep::tcp::network& network, const breep::tcp::peer& peer) {
            if (peer.is_connected()) {
                // it's a new buddy that we don't know, but that doesn't know us either.
                // we'll tell him who we are using the send_object_to method, that sends
                // an object to one particular peer.
                network.send_object_to(peer, m_name);
            } else {
                std::cout << m_nicknames.at(peer.id()) << " disconnected." << std::endl;
            }
        }


        void on_name_received(breep::tcp::netdata_wrapper<name>& dw) {
            // Someone tell us his/her name. We'll consider him connected from now.
            m_nicknames.insert(std::make_pair(dw.source.id(), dw.data.name_));
            std::cout << dw.data.name_ << " connected." << std::endl;
        }


        void on_message(breep::tcp::netdata_wrapper<std::string>& dw){
            std::cout << m_nicknames.at(dw.source.id()) << " : " << dw.data << std::endl;
        }


};

int main() {

    int host_port;
    std::cout << "Hosting Port : ";
    std::cin >> host_port;

    std::string nick;
    std::cout << "Nickname : ";
    std::cin >> nick;

    breep::tcp::network net(host_port);
    p2pNetwork p2pClient(nick);

    net.add_data_listener<name>([&p2pClient](breep::tcp::netdata_wrapper<name>& dw) -> void {
        p2pClient.on_name_received(dw);
    });

    net.add_data_listener<std::string>([&p2pClient](breep::tcp::netdata_wrapper<std::string>& dw) -> void {
        p2pClient.on_message(dw);
    });

    net.add_connection_listener([&p2pClient](breep::tcp::network& net, const breep::tcp::peer& peer) -> void {
        p2pClient.on_connection(net, peer);
    });

    net.add_disconnection_listener([&p2pClient](breep::tcp::network& net, const breep::tcp::peer& peer) -> void {
        p2pClient.on_connection(net, peer);
    });

    int would_you_connect;
    std::cout << "Would you like to connect to someone : ";
    std::cin >> would_you_connect;

    if(would_you_connect){
        
        std::string client_address;
        std::cout << "Client Address : ";
        std::cin >> client_address;

        int client_port;
        std::cout << "Client Port : ";
        std::cin >> client_port;

        if(!net.connect(boost::asio::ip::address::from_string(client_address), client_port)){
            std::cerr << "Connection Failed !!" << std::endl;
            return 1;
        }

    }else{
        net.awake();
    }


    std::string message;
	std::getline(std::cin, message);
	while (message != "/q") {
		net.send_object(message);
		std::getline(std::cin, message);
	}


    net.disconnect();
    return 0;
}
