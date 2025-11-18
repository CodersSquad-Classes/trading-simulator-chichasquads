# Continuous Limit Order Book — Trading Simulator

Luis Emilio Arce Ruvalcaba / A01647064
Marcial Becerra Rodríguez /A01646589

## Introduction

This project implements a simple version of a Continuous Limit Order Book (CLOB), similar to the systems used by modern stock exchanges.  
The goal is to receive buy and sell limit orders, place them in an order book, and execute trades whenever the prices match.  
The program also includes a live terminal view that updates as new orders come in.

The objective is not to recreate a professional trading platform, but to understand the core ideas behind real-time market systems: price priority, time priority, order matching, and visualizing market depth.

## Research on Stock Exchange

Modern stock exchanges operate almost entirely electronically.  
Traders send orders, the exchange processes them, and a central matching engine determines which trades get executed.

Two common types of limit orders are:

- Buy Limit: buy at a maximum price or better  
- Sell Limit: sell at a minimum price or better  

These orders are stored in an order book, separated into:

- Bids: buy orders sorted from highest price to lowest  
- Asks: sell orders sorted from lowest price to highest  

The matching engine follows two simple rules:

1. Price priority — better prices go first  
2. Time priority — if prices are the same, the earliest order goes first  

A system is called “continuous” because matching happens all the time, not in fixed batches.

This simulator applies these rules in a smaller and more educational way.

## Software Design and Implementation

### Code Structure

The project is divided into three main components:

- `Order`  
  Holds information for a single limit order: id, side, price, quantity, and timestamp.

- `OrderBook`  
  Stores orders for a single stock symbol using two priority queues (one for buys, one for sells).

- `MatchingEngine`  
  Manages multiple order books, generates order IDs, and triggers the matching process.

### Matching Logic

Whenever a new order arrives:

1. A unique ID and timestamp are assigned  
2. The order is inserted into the correct priority queue  
3. The engine checks whether the best BUY and best SELL can match  
   - A trade happens if bestBuy.price >= bestSell.price  
4. The trade quantity is the minimum of both orders  
5. If an order is only partially filled, the remainder is pushed back into the queue  
6. The process repeats until no more matches are possible  

This creates a realistic continuous matching flow.

### Terminal Real-Time UI

The live terminal view includes:

- Automatic screen clearing using ANSI escape codes  
- Colored output (green for buys, red for sells)  
- A clean table-like view of the top levels of the order book  
- Short pauses between updates to simulate a live feed  

It’s a simple dashboard, but it gives the feeling of watching a small exchange in action.

### Key Functions

- `addOrder()`  
  Creates a new order and inserts it in the right place.

- `matchAll()`  
  Executes all possible trades based on price–time priority.

- `getBuyPQCopy()` and `getSellPQCopy()`  
  Provide safe copies of the priority queues for printing.

- `printOrderBook()`  
  Displays the current state of bids and asks in a readable way.

The logic of the engine and the UI is intentionally kept separate to make the code clearer.

## How to Build and Run

### Requirements

- C++20 compiler (clang++ recommended)  
- make installed  
- A terminal that supports ANSI color codes (PowerShell, Git Bash, VSCode Terminal)

### Project Structure

trading-simulator-chichasquads
-Trading
--clob.h
--clob.cpp
--main.cpp

## Conclusions and Learnings

Emilio Arce

I learned how price–time priority is implemented in real exchanges using priority queues.
Working with partial fills and reinserted orders helped me understand how trades are executed behind the scenes.
This project also reinforced the importance of clean separation between the engine logic and the UI.

Marcial Becerra

Building the matching engine helped me see how data structures directly affect correctness and speed.
I enjoyed working with custom comparators for priority_queue and seeing how the order book changes in real time.
The terminal UI part also taught me how small touches like colors and layout make a technical simulation much easier to follow.