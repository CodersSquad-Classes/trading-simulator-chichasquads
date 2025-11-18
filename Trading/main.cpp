// Simple CLOB demo with terminal UI
#include <iostream>
#include <iomanip>
#include <random>
#include <thread>
#include <chrono>
#include "clob.h"

// Print one order book
void printBook(const OrderBook* book) {
    if (!book) {
        std::cout << "No order book for this symbol\n";
        return;
    }

    auto buyPQ  = book->getBuyPQ();
    auto sellPQ = book->getSellPQ();

    std::cout << "ORDER BOOK: " << book->getSymbol() << "\n";
    std::cout << "----------------------------------------\n";

    std::cout << "BIDS (buy)\n";
    std::cout << "qty\tprice\n";
    while (!buyPQ.empty()) {
        const auto& o = buyPQ.top();
        std::cout << o.qty << "\t" << std::fixed << std::setprecision(2) << o.price << "\n";
        buyPQ.pop();
    }

    std::cout << "\nASKS (sell)\n";
    std::cout << "qty\tprice\n";
    while (!sellPQ.empty()) {
        const auto& o = sellPQ.top();
        std::cout << o.qty << "\t" << std::fixed << std::setprecision(2) << o.price << "\n";
        sellPQ.pop();
    }

    std::cout << "----------------------------------------\n";
}

// Clear terminal screen
void clearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

// Main with simple real time loop
int main() {
    MatchingEngine eng;
    std::string symbol = "AAPL";

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int> qtyDist(10, 100);
    std::uniform_real_distribution<double> priceDist(149.0, 152.0);

    int steps = 40;

    for (int i = 0; i < steps; ++i) {
        bool isBuy = (sideDist(rng) == 1);
        int qty = qtyDist(rng);
        double price = priceDist(rng);

        int id = eng.addOrder(symbol, isBuy, price, qty);
        std::vector<Trade> trades = eng.runMatching(symbol);

        clearScreen();

        std::cout << "Continuous Limit Order Book demo\n";
        std::cout << "New order id=" << id
                  << " side=" << (isBuy ? "BUY" : "SELL")
                  << " qty=" << qty
                  << " price=" << std::fixed << std::setprecision(2) << price << "\n\n";

        if (!trades.empty()) {
            std::cout << "Executed trades:\n";
            for (const auto& t : trades) {
                std::cout << "BUY " << t.buyId
                          << " vs SELL " << t.sellId
                          << " | " << t.symbol
                          << " | qty=" << t.qty
                          << " @ " << t.price << "\n";
            }
            std::cout << "\n";
        } else {
            std::cout << "No trades executed in this step\n\n";
        }

        const OrderBook* book = eng.getOrderBook(symbol);
        printBook(book);

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "\nEnd of demo\n";
    return 0;
}