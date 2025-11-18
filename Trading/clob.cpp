// Implementations for CLOB
#include "clob.h"

// Create order book for one symbol
OrderBook::OrderBook(const std::string& sym)
    : symbol(sym) { }

// Add order to correct side
void OrderBook::addOrder(const Order& o) {
    if (o.isBuy) {
        buyPQ.push(o);
    } else {
        sellPQ.push(o);
    }
}

// Check if best buy and best sell can match
bool OrderBook::canMatch() const {
    if (buyPQ.empty() || sellPQ.empty()) {
        return false;
    }
    const Order& bestBuy  = buyPQ.top();
    const Order& bestSell = sellPQ.top();
    return bestBuy.price >= bestSell.price;
}

// Match all possible orders and save trades
void OrderBook::matchAll(std::vector<Trade>& trades) {
    while (canMatch()) {
        Order bestBuy  = buyPQ.top();
        Order bestSell = sellPQ.top();

        buyPQ.pop();
        sellPQ.pop();

        int dealQty = std::min(bestBuy.qty, bestSell.qty);
        double dealPrice = bestSell.price;

        Trade t;
        t.buyId  = bestBuy.id;
        t.sellId = bestSell.id;
        t.symbol = symbol;
        t.price  = dealPrice;
        t.qty    = dealQty;
        trades.push_back(t);

        bestBuy.qty  -= dealQty;
        bestSell.qty -= dealQty;

        if (bestBuy.qty > 0) {
            buyPQ.push(bestBuy);
        }
        if (bestSell.qty > 0) {
            sellPQ.push(bestSell);
        }
    }
}

// Get symbol name
const std::string& OrderBook::getSymbol() const {
    return symbol;
}

// Get buy priority queue
const std::priority_queue<Order, std::vector<Order>, BuyCmp>&
OrderBook::getBuyPQ() const {
    return buyPQ;
}

// Get sell priority queue
const std::priority_queue<Order, std::vector<Order>, SellCmp>&
OrderBook::getSellPQ() const {
    return sellPQ;
}

// Create empty engine
MatchingEngine::MatchingEngine()
    : nextId(1), ts(0) { }

// Get or create order book for symbol
OrderBook& MatchingEngine::getOrCreateBook(const std::string& symbol) {
    auto it = books.find(symbol);
    if (it == books.end()) {
        it = books.emplace(symbol, OrderBook(symbol)).first;
    }
    return it->second;
}

// Add one new order to the engine
int MatchingEngine::addOrder(const std::string& symbol, bool isBuy, double price, int qty) {
    Order o;
    o.id     = nextId++;
    o.symbol = symbol;
    o.isBuy  = isBuy;
    o.price  = price;
    o.qty    = qty;
    o.ts     = ts++;

    OrderBook& book = getOrCreateBook(symbol);
    book.addOrder(o);
    return o.id;
}

// Run matching for one symbol and return list of trades
std::vector<Trade> MatchingEngine::runMatching(const std::string& symbol) {
    std::vector<Trade> trades;
    auto it = books.find(symbol);
    if (it == books.end()) {
        return trades;
    }
    it->second.matchAll(trades);
    return trades;
}

// Get pointer to one order book
const OrderBook* MatchingEngine::getOrderBook(const std::string& symbol) const {
    auto it = books.find(symbol);
    if (it == books.end()) {
        return nullptr;
    }
    return &it->second;
}