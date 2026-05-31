#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<chrono>
#include<fstream>
#include<sstream>
using namespace std;
using namespace std;

struct order{
    int id;
    string type;
    int price;
    int quantity;
};

class orderbook{
public:
    map<int,int,greater<int>> bids;
    map<int,int> asks;
    void addorder(order o);
    void matchorders();
    void printbook();
    void cancelorder(int price, int quantity, string type);
};

void orderbook::matchorders(){
    while(!bids.empty() && !asks.empty()){
        auto best_bid = bids.begin();
        auto best_ask = asks.begin();
        if(best_bid->first >= best_ask->first){
            int traded = min(best_bid->second, best_ask->second);
            cout << "Trade: " << traded << " units at price " << best_ask->first << "\n";
            best_bid->second -= traded;
            best_ask->second -= traded;
            if(best_bid->second == 0) bids.erase(best_bid);
            if(best_ask->second == 0) asks.erase(best_ask);
        } else break;
        
    }
}

void orderbook::addorder(order o){
    if(o.type == "buy") bids[o.price] += o.quantity;
else if(o.type == "sell") asks[o.price] += o.quantity;
    if(o.type == "market_buy"){
    int remaining = o.quantity;
    while(remaining > 0 && !asks.empty()){
        auto best_ask = asks.begin();
        int traded = min(remaining, best_ask->second);
        cout << "Market Trade: " << traded << " units at price " << best_ask->first << "\n";
        remaining -= traded;
        best_ask->second -= traded;
        if(best_ask->second == 0) asks.erase(best_ask);
    }
}
if(o.type == "market_sell"){
    int remaining = o.quantity;
    while(remaining > 0 && !bids.empty()){
        auto best_bid = bids.begin();
        int traded = min(remaining, best_bid->second);
        cout << "Market Trade: " << traded << " units at price " << best_bid->first << "\n";
        remaining -= traded;
        best_bid->second -= traded;
        if(best_bid->second == 0) bids.erase(best_bid);
    }
}
    matchorders();
}

void orderbook::printbook(){
    cout << "ASKS:\n";
    for(auto it = asks.rbegin(); it != asks.rend(); it++)
        cout << it->first << " | " << it->second << "\n";
    cout << "---\n";
    cout << "BIDS:\n";
    for(auto& p : bids)
        cout << p.first << " | " << p.second << "\n";
}
void orderbook::cancelorder(int price, int quantity, string type){
    if(type == "buy"){
        bids[price] -= quantity;
        if(bids[price] <= 0) bids.erase(price);
    } else {
        asks[price] -= quantity;
        if(asks[price] <= 0) asks.erase(price);
    }
}
void loadorders(orderbook& ob, string filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string id, type, price, quantity;
        getline(ss, id, ',');
        getline(ss, type, ',');
        getline(ss, price, ',');
        getline(ss, quantity, ',');
        order o = {stoi(id), type, stoi(price), stoi(quantity)};
        ob.addorder(o);
    }
}
int main(){
    orderbook ob;
    auto start = chrono::high_resolution_clock::now();
    loadorders(ob, "orders.csv");
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    cout << "\nProcessed in: " << duration.count() << " nanoseconds\n";
    cout << "\nAfter cancel:\n";
    ob.printbook();
}