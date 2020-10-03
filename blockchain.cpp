
/* Blockchain Fundamentals
 *       Project by,
 *  Nimish Shah
 *  Agnim Chakraborty
 *  Chirag Kathuria
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <algorithm>
#include "sha256.h"
#include "md5.h"
#include "md5.cpp"

#define pb push_back //short for push_back while using vector

using namespace std;

struct Transaction  // For holding transaction information for each block
{
    double amount;
    std::string rec;
    std::string sen;
    time_t tme; // Storing the time of transaction.
};

Transaction *d1; // Global ptr to Transaction to use later in the program.


class Block  // Block information class
{
    int index;
    std::string previous_hash; // To store hash of previous block
    std::string current_hash;  // Stores the value of hash of the blocks
    std::string block_addr;    // Like UID to store the uniquely available block address

  public:
    Transaction data;

    std::string get_hash() //Returns the current hash of the block
    {
        return current_hash;
    }

    std::string get_prev_hash() //Returns hash of previous block
    {
        return previous_hash;
    }

    int get_index()
    {
        return index;
    }

    std::string get_block_addr();      //To get block address
    std::string generate_block_hash(); //To generate block hash
    bool is_hash_valid();              //To check if the hash generated is valid
    Block() {}
    //Parameterizeds Constructor used to add block easily by just passing the arguments...
    Block(int idx, Transaction d, std::string PrevHash);
};

Block::Block(int idx, Transaction d, std::string PrevHash)
{
    index = idx;
    previous_hash = PrevHash;
    data = d;
    current_hash = generate_block_hash();
    block_addr= get_block_addr();
}

std::string Block:: get_block_addr(){
    std::string block_address;
    std::string to_hash= previous_hash+data.rec;
    picosha2::hash256_hex_string(to_hash,block_address);
    block_address=joyee::md5(block_address);
    return block_address;
}

std::string Block::generate_block_hash() // This is where magic happens...
{
    std::string hashed;
    std::string before_hash = data.rec + data.sen + std::to_string(data.amount) + std::to_string(data.tme);

    picosha2::hash256_hex_string(before_hash, hashed); //You can use any of the method given
    //hashed = joyee::md5(before_hash);                      // ...
    return hashed;
}

// Checks for valid Block
bool Block::is_hash_valid()
{
    return get_hash() == generate_block_hash();
}

//@@@@@@@@@@@@@@@@@@@@@@ CHAIN @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CHAIN @@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CHAIN @@@@@@@@@@@@@@@@@@@@@@

//Chain of blocks...
class Chain
{

  public:
    std::vector<Block> Bchain;
    Block create_genesis();
    Block create_block(Transaction data); //Creates a block to be added in the chain later.
    //Constructor
    Chain(); //This initializes Bchain with the genesis block

    std::vector<Block> get_chain();
    Block *latest_block(); //Returns the lastest block in the chain
    bool is_chain_valid(); //Checks if the chain is valid.
    void add_block(Block b);

    void display_chain(); //Displaying the chain
};

Block Chain::create_block(Transaction data)
{
    int index = Bchain.size();
    std::string previous_hash = index > 0 ? latest_block()->get_hash() : std::to_string(0);
    Block B(index, data, previous_hash);
    return B;
}

Block Chain::create_genesis() //Genesis... because origin is important.
{
    time_t timestamp;
    d1 = (Transaction *)malloc(sizeof(Transaction));
    d1->amount = 0;
    d1->rec = "Genesis";
    d1->sen = "genesis";
    d1->tme = time(&timestamp);
    Block genesis(0, *d1, "0");
    return genesis;
}

void Chain::add_block(Block new_block)
{
    Bchain.pb(new_block); //Adding a new block to the chain.
}

std::vector<Block> Chain::get_chain()
{
    return Bchain;
}

//Somebody's getting sneaky..Pointer to the last block to see what happens when we alter its data...
Block *Chain::latest_block()
{
    return &(Bchain.back());
}

bool Chain::is_chain_valid()
{
    std::vector<Block>::iterator i;
    for (i = Bchain.begin() + 1; i != Bchain.end(); i++)
    {
        Block current_block = *(i), previous_block = *(i - 1);
        if (current_block.is_hash_valid() == 0)
            return false;
        if (current_block.get_prev_hash() != previous_block.get_hash())
        {
            return false;
        }
    }
    return true;
}

Chain::Chain()
{
    Block genesis = create_genesis();
    Bchain.pb(genesis);
}

void Chain::display_chain()
{
    std::vector<Block>::iterator i;
    for (i = Bchain.begin(); i != Bchain.end(); i++)
    {
        cout << "\n======================Block Start========================\n";
        cout << "Block Address: "<<i->get_block_addr() <<endl;
        cout << "Index: " << i->get_index() << endl;
        cout << "Amount: " << i->data.amount << endl;
        cout << "Sender: " << i->data.sen << endl;
        cout << "Receiver: " << i->data.rec << endl;
        cout << "Time (seconds from 1/1/1970):" << i->data.tme << endl;
        cout << "Hash: " << i->get_hash() << endl;
        cout << "Previous hash: " << i->get_prev_hash() << endl;
        cout << "Is block valid? :" << i->is_hash_valid() << endl;
        
        if (i->data.sen=="genesis")
            cout<<"Genensis Block"<<endl;
        else if (i->data.amount==0) 
            cout<< "Signature notifying block"<<endl;
        else 
            cout<<"Transaction Block"<<endl;
        if (i->is_hash_valid()==false)
            cout<<"Transaction data modified, Immutability disturbed, Block Aborted, Chain Abandoned"<<endl;
        cout << "======================Block End========================\n";
    }
}

int main()
{
    Chain coin;
    Block B;

    int i = 0, switch_main, l = 1;
    double amt;
    time_t timestamp;
    std::string s, r;
    vector<string> recv;
    vector<string> send;
    while (l)
    {
        cout << "Do you want to make a transaction?(1/0):";
        cin >> switch_main;
       
        switch (switch_main)
        {

        case 1:
            d1 = (Transaction *)malloc(sizeof(Transaction));
            cout << "Enter amount:";
            cin >> amt;
            cout << "Enter sender's address:";
            cin >> s;
            cout << "Enter receiver's address:";
            cin >> r;
            send.pb(s);
            recv.pb(r);
            d1->sen = s;
            d1->rec = r;
            d1->amount = amt;
            d1->tme = time(&timestamp);

            cout << "Creating a block by taking the transactions...\n";
            B = coin.create_block(*d1);
            cout << "Block created.\n";

            coin.add_block(B);
            if (send.size()!=recv.size())
                cout << "Sender-Reciever mismatch, instability detected"<<endl;

            cout << "Adding block to the chain...\n";

            cout << "Block added.\n";
            break;

        case 0:
            l = 0;
            break;

        default:
            cout << "Invalid key entered.Enter again." << endl;
            break;
        }
    }

    coin.display_chain();
    cout << "Is chain valid?: " << coin.is_chain_valid() << endl;

    cout << "\n\n\n\n\nHacking in progress..."<<endl;
    Block *Hackblock;
    Hackblock = coin.latest_block(); //Hackblock...
    Hackblock->data.rec = "Nimish";
    recv.pb(Hackblock->data.rec);
    Hackblock->data.amount = 10000;
    coin.display_chain();
    if (send.size()!=recv.size())
    cout << "Sender-Reciever mismatch, Possible Intruder: "<<recv.back()<<endl;
    cout << "Is chain valid?: " << coin.is_chain_valid() << endl;
    cout << "Sorry ...Chain not valid.Transaction cannot proceed...\n ";
}
