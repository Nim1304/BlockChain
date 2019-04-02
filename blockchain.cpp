#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <fstream>
#include <algorithm>
#define pb push_back //short for push_back() while using vectors...
// #include <crypto++>

using namespace std;

// For holding transaction information for each block
struct Transaction
{
    double amount;
    std::string rec;
    std::string sen;
    time_t tme; // Storing the time of transaction.

};

Transaction *d1; // Global ptr to Transaction to use later in the program.

//Hashing Function...Important,Right?..xd
size_t hash1(std::string s)
{
    size_t HashVal = 0;
    std::string::iterator i;
    for (i = s.begin(); i != s.end(); i++)
    {
        HashVal += (HashVal * 37) + *i;
    }
    return HashVal;
}

// Block information class
class Block
{
    int index;
    size_t previous_hash;         // To store hash of previous block
    size_t current_hash;          // //Stores the value of hash of the blocks
    Transaction data;        
    /*double amt;
    string receiver;
    string sender;
    time_t timestamp;*/
    public:
    size_t get_hash()
    {
        return current_hash;
    }
    size_t get_prev_hash()
    {
        return previous_hash;
    }
    Transaction *get_data()
    {
        return &data;
    }
    int get_index()
    {
        return index;
    }
    size_t generate_block_hash(); //To generate block hash
    bool is_hash_valid();
    Block(){}
    Block(int idx, Transaction d, size_t PrevHash);
     // Constructor used to add block easily by just passing the arguments...
};

Block::Block(int idx, Transaction d, size_t PrevHash)
{
    index = idx;
    previous_hash = PrevHash;
    data = d;
    /*amt=d.amount;
    receiver=d.rec;
    sender=d.sen;
    timestamp=d.tme;*/
    current_hash = generate_block_hash();
}
size_t Block::generate_block_hash() // This is where magic happens...
{
    std::string before_hash = get_data()->rec + get_data()->sen + std::to_string(get_data()->amount) + std::to_string(get_data()->tme);

    return hash1(std::to_string(hash1(before_hash) + (hash1(std::to_string(previous_hash)) << 1)));
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
  std::vector<Block> Bchain;
  public: // Public declaration.Can make them private though.
    Block create_genesis();
    Block create_block(Transaction data);
    //Constructor
    Chain();

    std::vector<Block> get_chain();
    Block *latest_block();
    bool is_chain_valid();
    void add_block(Block b);

    void display_chain();
};

Block Chain::create_block(Transaction data)
{
    int index=Bchain.size();
    size_t previous_hash=index>0 ? latest_block()->get_hash() : 0;
    Block B(index,data,previous_hash);
    return B;
}

Block Chain::create_genesis() //Genesis... because origin is importante`..
{
    time_t timestamp;
    //Transaction d(0,"Genesis","genesis",time(&timestamp));
    d1 = (Transaction *)malloc(sizeof(struct Transaction));
    d1->amount = 0;
    d1->rec = "Genesis";
    d1->sen = "genesis";
    d1->tme = time(&timestamp);
    Block genesis(0, *d1, 0);
    return genesis;
}

void Chain::add_block(Block new_block)
{
    Bchain.pb(new_block);
}

//Somebody's getting sneaky..Pointer to the last block to see what happens when we alter its data...
Block *Chain::latest_block()
{
    return & (get_chain().back());
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

std::vector<Block> Chain::get_chain()
{
    return Bchain;
}

void Chain::display_chain()
{
    std::vector<Block>::iterator i;
    for (i = Bchain.begin(); i != Bchain.end(); i++)
    {
        Block c = *i;
        cout << "Index: " << c.get_index() << endl;
        cout << "Amount: " << c.get_data()->amount << endl;
        cout << "Sender: " << c.get_data()->sen << endl;
        cout << "Receiver: " << c.get_data()->rec<< endl;
        cout << "Hash: " << c.get_hash() << endl;
        cout << "Previous hash: " << c.get_prev_hash() << endl;
        cout << "Is block valid? :" << c.is_hash_valid() << endl;
        cout << "==============================================\n";
        //cout<<"Is chain valid? (1/0): "<<Chain.is_chain_valid()<<endl;
    }
}

/*int get_index()
{
    Block B;
    ifstream op;
    op.open("Database.bin",ios::binary|ios::in);
    int i;
    while(!op.eof())
    {
        op.read((char *)&B,sizeof(B));
        i=B.index;
    }
}*/

int main()
{
    Chain coin;
    Block B,C;
    ofstream o;
    ifstream p;
    int i = 0, switch_main, l = 1;
    double amt;
    time_t timestamp;
    std::string s, r;
    while (l)
    {
        cout << "Do you want to make a transaction?(1/0):";
        cin >> switch_main;
        switch (switch_main)
        {
        case 1:
            d1 = (Transaction *)malloc(sizeof(struct Transaction));
            //b = (Block *)malloc(sizeof(Block));
            cout << "Enter amount:";
            cin >> amt;
            cout << "Enter sender's address:";
            cin >> s;
            cout << "Enter receiver's address:";
            cin >> r;
            d1->sen = s;
            d1->rec = r;
            d1->amount = amt;
            d1->tme = time(&timestamp);
            
            cout << "Creating a block by taking the transactions...\n";
            B=coin.create_block(*d1);
            cout << "Block created.\n";
            
            
            o.open("Database.bin",ios::binary|ios::app);
            o.write((char *) &B,sizeof(B));
            o.close();
            
            coin.add_block(B);
            
            cout << "Adding block to the chain...\n";
            
            cout << "Block added.\nCongratulations !! You have just mined 12.5 HashCoin.\n";
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

    
    cout<<"\n\n\n\n\nHacking in progress...";
    Block *Hackblock = coin.latest_block();                        //Hackblock...
    Hackblock->get_data()->rec = "Nimish";
    Hackblock->get_data()->amount = 10000;
    coin.display_chain();
    cout << "Is chain valid?: " << coin.is_chain_valid() << endl;
    cout<<"Sorry ...Chain not valid.Transaction cannot proceed...\n";


   /* p.open("Database.bin",ios::binary|ios::in);
    while(!p.eof())
    {
        p.read((char *)&B,sizeof(B));
       
        //cout<<B.data.sender<<endl<<B.data.receiver;
        cout<<B.index<<endl<<B.current_hash<<B.sender<<endl;
        cout<<sizeof(B)<<endl;
    }
    p.close();*/
}
