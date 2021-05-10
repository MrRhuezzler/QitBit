#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <ncurses.h>


struct name
{
    char nick[100];
};

void send_trasaction(int maxY, int maxX, double balance){

    WINDOW *sendTransWin = newwin(maxY / 3, maxX / 3, 0, 0);
    box(sendTransWin, 0, 0);
    refresh();
    wrefresh(sendTransWin);

    keypad(sendTransWin, true);

    std::string title_string = "QitBIT Network - Transact";
    mvwprintw(sendTransWin, 1, (maxX / 6) - (title_string.size() / 2), title_string.c_str());

    std::string blank_string(title_string.size(), '-');
    mvwprintw(sendTransWin, 2, (maxX / 6) - (blank_string.size() / 2), blank_string.c_str());

    std::string receiver_string = "Receiver (Don't Mess up the spelling)";
    mvwprintw(sendTransWin, 4, 1, receiver_string.c_str());

    char* receiver = new char[100];

    echo();
    mvwgetnstr(sendTransWin, 5, 1, receiver, 100);
    noecho();

    std::string amount_string = "Amount ( You have : " + std::to_string(balance) + " )";
    mvwprintw(sendTransWin, 6, 1, amount_string.c_str());

    char* amount = new char[100];

    echo();
    mvwgetnstr(sendTransWin, 7, 1, receiver, 100);
    noecho();


    delete[] receiver;
    delete[] amount;

    delwin(sendTransWin);

}


void get_nick(int maxY, int maxX){


    WINDOW *getNickWin = newwin(maxY / 3, maxX / 3, 0, 0);
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

    std::fstream nick;
    nick.open("qitbit.nick", std::ios::binary | std::ios::out);

    if(nick.is_open()){
        nick.write((char*)&n, sizeof(name));
    }

    delwin(getNickWin);

}

bool main_menu(int maxY, int maxX, double balance, std::string nick){
    
    WINDOW *mainMenuWin = newwin(maxY / 3, maxX / 3, 0, 0);
    box(mainMenuWin, 0, 0);
    refresh();
    wrefresh(mainMenuWin);

    keypad(mainMenuWin, true);

    std::string title_string = "QitBIT Network";
    mvwprintw(mainMenuWin, 1, (maxX / 6) - (title_string.size() / 2), title_string.c_str());
    
    std::string blank_string(title_string.size(), '-');
    mvwprintw(mainMenuWin, 2, (maxX / 6) - (blank_string.size() / 2), blank_string.c_str());

    std::string acct_details;
    std::string bal_string = std::to_string(balance);
    std::string spaces((maxX / 3) - nick.size() - bal_string.size() - 4, ' ');

    acct_details += bal_string;
    acct_details += spaces;
    acct_details += nick;

    mvwprintw(mainMenuWin, 3, (maxX / 6) - (acct_details.size() / 2), acct_details.c_str());

    std::vector<std::string> options = {"Transact QitBIT", "Blockchain Viewer", "Mine QitBIT", "Quit"};

    int choice = KEY_UP;
    int highlight = 0;

    do{

        for(int i = 0; i < options.size(); i++){
            if(i == highlight)
            {
                wattron(mainMenuWin, A_REVERSE);
            }
            
            mvwprintw(mainMenuWin, 4 + i, (maxX / 6) - (options[i].size() / 2), options[i].c_str());
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
        send_trasaction(maxY, maxX, balance);
        return true;
        break;

        case 1:
        // view_blockchain(int maxY, int maxX);
        return true;
        break;

        case 2:
        // mine_block(int maxY, int maxX);
        return true;
        break;

        case 3:
            return false;
        break;

    }

    delwin(mainMenuWin);
}

void connect_to_peer(int maxY, int maxX){

    WINDOW *connToPeer = newwin(maxY / 3, maxX / 3, 0, 0);
    box(connToPeer, 0, 0);
    refresh();
    wrefresh(connToPeer);

    keypad(connToPeer, true);

    std::string title_string = "QitBIT Network";
    mvwprintw(connToPeer, 1, (maxX / 6) - (title_string.size() / 2), title_string.c_str());
    
    std::string blank_string(title_string.size(), '-');
    mvwprintw(connToPeer, 2, (maxX / 6) - (blank_string.size() / 2), blank_string.c_str());

    std::string peer_string = "Peer Host";    
    mvwprintw(connToPeer, 4, 1, peer_string.c_str());

    char* host = new char[100];

    echo();
    mvwgetnstr(connToPeer, 5, 1, host, 100);
    noecho();


    std::string port_string = "Peer Port";    
    mvwprintw(connToPeer, 6, 1, port_string.c_str());

    char* port = new char[10];

    echo();
    mvwgetnstr(connToPeer, 7, 1, port, 10);
    noecho();


    delete[] host;
    delete[] port;

    delwin(connToPeer);

}

int main(){

    initscr();
    cbreak();
    noecho();

    curs_set(0);

    int maxX, maxY;
    getmaxyx(stdscr, maxY, maxX);

    std::fstream nick;
    nick.open("qitbit.nick", std::ios::binary | std::ios::in);

    if(nick.is_open()){
        
        name n;
        nick.read((char*)&n, sizeof(name));

        connect_to_peer(maxY, maxX);
        while(main_menu(maxY, maxX, 50.0, n.nick));

    }else{
        get_nick(maxY, maxX);
    }

    getch();

    nick.close();
    endwin();

    return 0;
}