# Continuous Limit Order Book — Trading Simulator

Luis Emilio Arce Ruvalcaba / A01647064  
Marcial Becerra Rodríguez / A01646589  

## Introduction

This project implements an educational version of a Continuous Limit Order Book (CLOB), similar to the systems used by modern stock exchanges. The idea is to receive buy and sell limit orders, place them in an order book, and execute trades whenever prices are compatible. The program also shows a live terminal view that updates as new orders are generated and matched.

The main goal is not to build a full trading platform, but to understand the core concepts behind real-time market systems: price priority, time priority, order matching, and how the order book evolves over time.

## Research on Stock Exchange

Modern stock exchanges are almost completely electronic. Traders send orders to the exchange, the orders are stored in a central order book, and a matching engine decides which trades are executed. Everything happens very fast, but the basic ideas are simple.

A limit order is an instruction to buy or sell a stock at a specific price or better:

- Buy limit: buy at a maximum price or lower  
- Sell limit: sell at a minimum price or higher  

These orders are organized in an order book:

- Bids: buy orders, sorted from highest price to lowest  
- Asks: sell orders, sorted from lowest price to highest  

The matching engine usually follows two important rules:

1. Price priority – better prices go first  
2. Time priority – if two orders have the same price, the one that arrived earlier has priority  

A book is called continuous because matching is not done in fixed time batches; new orders can trigger trades at any moment.  

Our simulator uses these same principles, but in a smaller, controlled environment so it is easier to study and understand.

## Software Design and Implementation

# Code Structure

The project contains three main components:

`Order`  
Represents a single limit order including id, symbol, side, price, quantity, and timestamp.

`Trade`   
Represents a single executed trade between a buyer and a seller.

`OrderBook` 
Stores buy and sell orders for one symbol using two priority queues.

`MatchingEngine` 
Manages multiple order books, generates order IDs and timestamps, and runs the matching logic.

The separation between the engine logic and the terminal UI keeps the project easier to understand and maintain.

# Matching Logic

The core of the project is the interaction between OrderBook and MatchingEngine.

- OrderBook uses two std::priority_queue containers:
  - One for buy orders, with BuyCmp as comparator  
    - Higher prices have higher priority  
    - For the same price, earlier timestamps have priority  
  - One for sell orders, with SellCmp as comparator  
    - Lower prices have higher priority  
    - For the same price, earlier timestamps have priority  

- MatchingEngine holds an std::unordered_map<std::string, OrderBook>:
  - Each stock symbol (in our demo we use "AAPL") has its own OrderBook
  - The engine also generates unique order IDs and timestamps

When a new order is added through MatchingEngine::addOrder:

1. A unique id and a ts (timestamp counter) are assigned.  
2. The order is inserted into the correct priority queue inside the OrderBook:
   - Buy orders go to the buy priority queue  
   - Sell orders go to the sell priority queue  
3. MatchingEngine::runMatching calls OrderBook::matchAll for the given symbol.  
4. Inside matchAll:
   - While there is at least one buy and one sell order and  
     bestBuy.price >= bestSell.price, a trade is created.  
   - The traded quantity is the minimum of the two order quantities.  
   - If an order is only partially filled, the remaining quantity is pushed back into the appropriate priority queue.  
   - The loop stops when no more matches are possible.  

This process gives us a realistic continuous matching flow, where several trades can happen after each new order.

# Terminal Real-Time UI

The user interface is implemented inside main.cpp. It is a simple, terminal-based dashboard that updates continuously as the simulation runs.

Key elements of the UI:

- A loop that runs for a fixed number of steps (steps = 40 in the example).  
- On each step:
  - A random order is generated:
    - Side: buy or sell  
    - Quantity: random between 10 and 100  
    - Price: random between 149.0 and 152.0  
  - The order is sent to the MatchingEngine with addOrder.  
  - The engine tries to match the order using runMatching.  
- The screen is cleared using clearScreen(), which sends ANSI escape codes to the terminal.  
- The program prints:
  - A short header describing the new order  
  - Any trades that were executed in that step  
  - The current state of the order book using printBook  

The order book is printed in a table-like format, with a BIDS (buy) section and an ASKS (sell) section, so it is easy to see the depth of the market at a glance.

This is not a graphical UI, but it behaves like a small real-time dashboard that lets you watch how the book evolves as more orders arrive.

# Key Functions

Some of the main functions and methods are:

- `OrderBook::addOrder(const Order& o)`  
  Inserts a new order into the buy or sell priority queue, depending on the side.

- `OrderBook::matchAll(std::vector<Trade>& trades)`  
  Repeatedly matches the best buy and best sell orders while they are compatible, and records all trades in the trades vector.

- `OrderBook::getBuyPQ()` and `OrderBook::getSellPQ()`  
  Return constant references to the priority queues. In main.cpp, these are copied into local variables so the book can be printed without modifying the original queues.

- `MatchingEngine::addOrder(const std::string& symbol, bool isBuy, double price, int qty)`  
  Creates a new order, assigns it an ID and timestamp, and forwards it to the corresponding OrderBook.

- `MatchingEngine::runMatching(const std::string& symbol)`  
  Calls matchAll on the OrderBook of the given symbol and returns the list of executed trades.

- `MatchingEngine::getOrderBook(const std::string& symbol)`  
  Returns a pointer to the OrderBook so the UI can display it.

- `printBook(const OrderBook* book)` (in main.cpp)  
  Prints the current state of the bids and asks for one symbol in a clean text format.

- `clearScreen()` (in main.cpp)  
  Clears the terminal and moves the cursor to the top-left corner before redrawing the UI.

## How to Build and Run

This project is designed to be built and run using make from the command line.

The repository is organized as follows:

Root folder 
-Trading/  
-- clob.h
-- clob.cpp  
-- main.cpp  

To compile and run the simulator:

From the root of the repository (trading-simulator-chichasquads), you simply type:

- `make`  
  This command reads the provided Makefile and calls clang++ with the correct options and source files. It produces the executable file trading_simulator.exe in the root folder.  

- `make run`  
  This command first ensures that the project is compiled (by building trading_simulator.exe if needed) and then runs the program with ./trading_simulator.exe. When it starts, you will see the continuous order generation and the live order book updates in the terminal.

In other words, make is used to build the program, and make run is used to build  and then execute the trading simulator.

## Conclusions and Learnings

# Emilio Arce

Building this matching engine showed me how the choice of data structures directly affects both correctness and performance. Implementing the buy and sell priority queues with different comparators was especially interesting, because a small comparator function completely changes the behavior of the system. The real-time terminal visualization made it much easier to see what was happening, and it taught me how even a simple text-based dashboard can make a technical system more understandable.

# Marcial Becerra

I learned how price–time priority can be implemented in practice using std::priority_queue with custom comparators. Working with partial fills and reinserted orders helped me understand how a matching engine really behaves when orders do not always match in a single step. Separating the matching logic from the terminal UI also made it clearer how to keep the code organized and easier to extend in future projects.