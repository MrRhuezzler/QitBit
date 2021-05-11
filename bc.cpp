// Standard
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>

// Breep
#include <breep/network/tcp.hpp>
#include <breep/util/serialization.hpp>

// NCurses
#include <ncurses.h>

// Custom
#include <qitbit.h>
#include <types.h>
#include <transaction.h>
#include <block.h>
#include <blockchain.h>

#define FA 2

struct name
{
    char nick[100];
};


void mine_block(Blockchain& bc, int maxY, int maxX, std::string nick){


    WINDOW *mineBlkWin = newwin(maxY / FA, maxX / FA, 0, 0);
    box(mineBlkWin, 0, 0);
    refresh();
    wrefresh(mineBlkWin);

    keypad(mineBlkWin, true);

    std::string title_string = "QitBIT Network - Mine";
    mvwprintw(mineBlkWin, 1, (maxX / (2 * FA)) - (title_string.size() / 2), title_string.c_str());

    std::string blank_string(title_string.size(), '-');
    mvwprintw(mineBlkWin, 2, (maxX / (2 * FA)) - (blank_string.size() / 2), blank_string.c_str());


    bc.add_transactions(q_transaction("Miner Rewards", nick, bc.get_mining_reward()));
    double time_elapsed = bc.mine_pending_transactions();

    if(time_elapsed == 0.0){

        std::string elapsed_info_string = "Nothing to Mine !";
        mvwprintw(mineBlkWin, 3, (maxX / (2 * FA)) - (elapsed_info_string.size() / 2), elapsed_info_string.c_str());

    }else{

        std::string elapsed_info_string = "Time Taken to Mine";
        mvwprintw(mineBlkWin, 3, (maxX / (2 * FA)) - (elapsed_info_string.size() / 2), elapsed_info_string.c_str());

        std::string elapsed_string = std::to_string(time_elapsed);
        mvwprintw(mineBlkWin, 4, (maxX / (2 * FA)) - (elapsed_string.size() / 2), elapsed_string.c_str());

    }

    int c;

    do {
        c = wgetch(mineBlkWin);
    }while(c != 10);

    delwin(mineBlkWin);
}


void view_blockchain(const Blockchain& bc, int maxY, int maxX){

    WINDOW *viewBlkWin = newwin(maxY, maxX, 0, 0);
    box(viewBlkWin, 0, 0);
    refresh();
    wrefresh(viewBlkWin);

    keypad(viewBlkWin, true);

    std::string title_string = "QitBIT Network - Blockchain";
    mvwprintw(viewBlkWin, 1, (maxX) - (title_string.size() / 2), title_string.c_str());

    std::string blank_string(title_string.size(), '-');
    mvwprintw(viewBlkWin, 2, (maxX) - (blank_string.size() / 2), blank_string.c_str());


    std::cout << bc << std::endl;

    getch();

    int c;
    do {
        c = wgetch(viewBlkWin);
    }while(c != 10);

    delwin(viewBlkWin);

}

void send_trasaction(Blockchain& bc, int maxY, int maxX, double balance, std::string nick){

    WINDOW *sendTransWin = newwin(maxY / FA, maxX / FA, 0, 0);
    box(sendTransWin, 0, 0);
    refresh();
    wrefresh(sendTransWin);

    keypad(sendTransWin, true);

    std::string title_string = "QitBIT Network - Transact";
    mvwprintw(sendTransWin, 1, (maxX / (2 * FA)) - (title_string.size() / 2), title_string.c_str());

    std::string blank_string(title_string.size(), '-');
    mvwprintw(sendTransWin, 2, (maxX / (2 * FA)) - (blank_string.size() / 2), blank_string.c_str());

    std::string receiver_string = "Receiver (Don't Mess up the spelling)";
    mvwprintw(sendTransWin, 4, 1, receiver_string.c_str());

    char receiver[100];

    echo();
    mvwgetnstr(sendTransWin, 5, 1, (char*)receiver, 100);
    noecho();

    std::string amount_string = "Amount ( You have : " + std::to_string(balance) + " )";
    mvwprintw(sendTransWin, 6, 1, amount_string.c_str());

    char amount[100];

    echo();
    mvwgetnstr(sendTransWin, 7, 1, (char*)amount, 100);
    noecho();

    std::string amt_string = amount;
    std::string rer_string = receiver;

    double amt = std::stod(amt_string);

    q_transaction trans = q_transaction(nick, rer_string, amt);
    bc.add_transactions(trans);

    int c;
    do {
        c = wgetch(sendTransWin);
    }while(c != 10);

    delwin(sendTransWin);
}


name get_nick(int maxY, int maxX){


    WINDOW *getNickWin = newwin(maxY / FA, maxX / FA, 0, 0);
    box(getNickWin, 0, 0);
    refresh();
    wrefresh(getNickWin);

    keypad(getNickWin, true);

    std::string title_string = "Welcome to QitBIT Network";
    mvwprintw(getNickWin, 1, 1, title_string.c_str());

    std::string blank_string(title_string.size(), '-');
    mvwprintw(getNickWin, 2, 1, blank_string.c_str());

    std::string nickname_string = "Enter a nick (You can't change it again)";
    mvwprintw(getNickWin, 3, 1, nickname_string.c_str());

    name n;

    echo();
    mvwgetnstr(getNickWin, 4, 1, (char*)&n.nick, 100);
    noecho();

    std::fstream nick_file;
    nick_file.open("qitbit.nick", std::ios::binary | std::ios::out);

    if(nick_file.is_open()){
        nick_file.write((char*)&n, sizeof(name));
    }

    delwin(getNickWin);
    nick_file.close();
    return n;

}

int main_menu(Blockchain& bc, int maxY, int maxX, double balance, std::string nick){
    
    WINDOW *mainMenuWin = newwin(maxY / FA, maxX / FA, 0, 0);
    box(mainMenuWin, 0, 0);
    refresh();
    wrefresh(mainMenuWin);

    keypad(mainMenuWin, true);

    std::string title_string = "QitBIT Network";
    mvwprintw(mainMenuWin, 1, (maxX / (2 * FA)) - (title_string.size() / 2), title_string.c_str());
    
    std::string blank_string(title_string.size(), '-');
    mvwprintw(mainMenuWin, 2, (maxX / (2 * FA)) - (blank_string.size() / 2), blank_string.c_str());

    std::string acct_details;
    std::string bal_string = std::to_string(balance);
    std::string spaces((maxX / FA) - nick.size() - bal_string.size() - 4, ' ');

    acct_details += bal_string;
    acct_details += spaces;
    acct_details += nick;

    mvwprintw(mainMenuWin, 3, (maxX / (2 * FA)) - (acct_details.size() / 2), acct_details.c_str());

    std::vector<std::string> options = {"Transact QitBIT", "Blockchain Viewer", "Mine QitBIT", "Refresh QitBIT", "Quit"};

    int choice = KEY_UP;
    int highlight = 0;

    do{

        for(int i = 0; i < options.size(); i++){
            if(i == highlight)
            {
                wattron(mainMenuWin, A_REVERSE);
            }
            
            mvwprintw(mainMenuWin, 4 + i, (maxX / (2 * FA)) - (options[i].size() / 2), options[i].c_str());
            wattroff(mainMenuWin, A_REVERSE);
        }

        choice = wgetch(mainMenuWin);

        switch(choice){
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                    highlight = options.size() - 1;
            break;
            case KEY_DOWN:
                highlight++;
                if(highlight == options.size())
                    highlight = 0;
            break;
        }

    }while(choice != 10);


    switch(highlight){

        case 0:
        send_trasaction(bc, maxY, maxX, balance, nick);
        clear();
        return 1;
        break;

        case 1:
        view_blockchain(bc, maxY, maxX);
        clear();
        return 2;
        break;

        case 2:
        mine_block(bc, maxY, maxX, nick);
        clear();
        return 3;
        break;

        case 3:
        return 4;
        break;

    }

    delwin(mainMenuWin);
    return 0;

}

std::pair<std::string, std::pair<std::string, std::string>> connect_to_peer(int maxY, int maxX){

    WINDOW *connToPeer = newwin(maxY / FA, maxX / FA, 0, 0);
    box(connToPeer, 0, 0);
    refresh();
    wrefresh(connToPeer);

    keypad(connToPeer, true);

    std::string title_string = "QitBIT Network";
    mvwprintw(connToPeer, 1, (maxX / (2 * FA)) - (title_string.size() / 2), title_string.c_str());
    
    std::string blank_string(title_string.size(), '-');
    mvwprintw(connToPeer, 2, (maxX / (2 * FA)) - (blank_string.size() / 2), blank_string.c_str());

    std::string hport_string = "Host Port";    
    mvwprintw(connToPeer, 4, 1, hport_string.c_str());

    char* host_port = new char[10];

    echo();
    mvwgetnstr(connToPeer, 5, 1, host_port, 10);
    noecho();

    std::string peer_string = "Peer Host";    
    mvwprintw(connToPeer, 6, 1, peer_string.c_str());

    char* host = new char[100];

    echo();
    mvwgetnstr(connToPeer, 7, 1, host, 100);
    noecho();


    std::string port_string = "Peer Port";    
    mvwprintw(connToPeer, 8, 1, port_string.c_str());

    char* port = new char[10];

    echo();
    mvwgetnstr(connToPeer, 9, 1, port, 10);
    noecho();

    std::pair<std::string, std::pair<std::string, std::string>> returnable(host_port, {host, port});

    delete[] host;
    delete[] port;

    delwin(connToPeer);

    return returnable;

}

int main() {


    initscr();
    cbreak();
    noecho();

    curs_set(0);

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    
    // int hosting_port = 8080;
    qitbit q;
    Blockchain q_bc = Blockchain();

    std::fstream nick_file;
    nick_file.open("qitbit.nick", std::ios::binary | std::ios::in);

    int hasStarted = false;

    name nick;

    bool didConnect = false;

    if(nick_file.is_open()){
        nick_file.read((char*)&nick, sizeof(name));
    }else{
        nick = get_nick(maxY, maxX);
    }


    auto host_n_port = connect_to_peer(maxY, maxX);

    breep::tcp::network q_p2p(std::stoi(host_n_port.first));

    q_p2p.add_connection_listener([&q](breep::tcp::network& net, const breep::tcp::peer& peer) -> void {
        q.on_connection(net, peer);
    });

    q_p2p.add_data_listener<q_blockchain>([&q, &q_bc, &q_p2p](breep::tcp::netdata_wrapper<q_blockchain>& data) -> void {
        
        if(q_p2p.self().id_as_string() != data.source.id_as_string()){ 
            q.on_incoming_blockchain(q_bc, data);
        }
   
    });

    q_p2p.add_data_listener<q_transaction>([&q, &q_bc, &q_p2p](breep::tcp::netdata_wrapper<q_transaction>& data) -> void{
        if(q_p2p.self().id_as_string() != data.source.id_as_string()){ 
            q.on_incoming_transaction(q_bc, data);
        }
    });

    q_p2p.add_data_listener<Request>([&q_p2p, &q_bc](breep::tcp::netdata_wrapper<Request>& data) -> void {

        if(q_p2p.self().id_as_string() != data.source.id_as_string()){ 

            Request typ = data.data;

            switch(typ){
                case Request::blockchain:
                    q_p2p.send_object<q_blockchain>(q_blockchain(q_bc));
                    break;

                default:
                    break;
            }

        }
    });

    double balance = 0.0;

    if(host_n_port.second.first != "0.0.0.0"){

        auto addr_ = boost::asio::ip::address::from_string(host_n_port.second.first);
        didConnect = q_p2p.connect(addr_, std::stoi(host_n_port.second.second));

    }else{

        didConnect = true;
        q_p2p.awake();

    }

    if(didConnect){

        q_p2p.send_object<Request>(Request::blockchain);

        int returned_value;
        do{

            balance = q_bc.get_balance(nick.nick);
            returned_value = main_menu(q_bc, maxY, maxX, balance, nick.nick);

            switch(returned_value){

                case 1:
                q_p2p.send_object<q_transaction>(q_transaction(*q_bc.get_last_transaction()));
                break;

                case 3:
                q_p2p.send_object<q_blockchain>(q_blockchain(q_bc));
                break;

            }


        }while(returned_value);

        q_p2p.disconnect();

    }

    q_p2p.join();
    nick_file.close();
    endwin();

    return 0;
}