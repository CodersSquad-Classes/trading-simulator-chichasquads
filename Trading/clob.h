#ifndef CLOB_H
#define CLOB_H

// Basic includes
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>

// Order data
struct Order {
    int id;
    std::string symbol;
    bool isBuy;
    double price;
    int qty;
    long long ts;
};

// Trade data
struct Trade {
    int buyId;
    int sellId;
    std::string symbol;
    double price;
    int qty;
};

// Buy priority comparator
struct BuyCmp {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price == b.price) {
            return a.ts > b.ts;
        }
        return a.price < b.price;
    }
};

// Sell priority comparator
struct SellCmp {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price == b.price) {
            return a.ts > b.ts;
        }
        return a.price > b.price;
    }
};

// Order book for one symbol
class OrderBook {
public:
    // Create order book for a symbol
    explicit OrderBook(const std::string& sym);

    // Add one order to the book
    void addOrder(const Order& o);

    // Match all possible orders and save trades
    void matchAll(std::vector<Trade>& trades);

    // Get symbol name
    const std::string& getSymbol() const;

    // Get buy priority queue (read only copy used in UI)
    const std::priority_queue<Order, std::vector<Order>, BuyCmp>& getBuyPQ() const;

    // Get sell priority queue (read only copy used in UI)
    const std::priority_queue<Order, std::vector<Order>, SellCmp>& getSellPQ() const;

private:
    // Check if best buy and best sell can match
    bool canMatch() const;

    std::string symbol;
    std::priority_queue<Order, std::vector<Order>, BuyCmp> buyPQ;
    std::priority_queue<Order, std::vector<Order>, SellCmp> sellPQ;
};

// Matching engine for many symbols
class MatchingEngine {
public:
    // Create empty engine
    MatchingEngine();

    // Add new order and return its id
    int addOrder(const std::string& symbol, bool isBuy, double price, int qty);

    // Run matching for one symbol and return all trades
    std::vector<Trade> runMatching(const std::string& symbol);

    // Get pointer to order book for UI
    const OrderBook* getOrderBook(const std::string& symbol) const;

private:
    // Get or create one order book
    OrderBook& getOrCreateBook(const std::string& symbol);

    std::unordered_map<std::string, OrderBook> books;
    int nextId;
    long long ts;
};

#endif