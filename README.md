# Limit Order Book Simulator

A high-performance order book engine written in C++, simulating the core matching system used by stock exchanges and HFT firms.

## What it does

- Accepts **limit orders** (buy/sell at a specific price) and **market orders** (buy/sell at best available price)
- Matches orders automatically when bid price >= ask price
- Handles **partial fills** — if buy wants 50 units but only 30 available, trades 30 and keeps 20 in book
- Supports **order cancellation**
- Maintains bids sorted highest-to-lowest, asks sorted lowest-to-highest

## Data Structures

- `std::map<int, int, greater<int>>` for bids — O(log n) insert/delete, automatic descending sort
- `std::map<int, int>` for asks — O(log n) insert/delete, automatic ascending sort

## Order Types

| Type | Description |
|------|-------------|
| `buy` | Limit buy — add to bid book at specified price |
| `sell` | Limit sell — add to ask book at specified price |
| `market_buy` | Consume cheapest available asks immediately |
| `market_sell` | Consume highest available bids immediately |

## Build & Run

```bash
g++ main.cpp -o ob && ./ob
```

## Example Output

```
Trade: 10 units at price 100
Market Trade: 30 units at price 101
ASKS:
101 | 30
---
BIDS:
100 | 40
99  | 20
```