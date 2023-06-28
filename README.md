# Cypto-arbitrage
A program written in C++.
Crypto Arbitrage Project

This project is designed to perform crypto arbitrage by comparing the prices of a token on different exchanges. It utilizes multithreading to fetch data simultaneously from four exchanges and determines the best difference in token prices among them.

Dependencies

iostream: Used for input and output operations.
curl/curl.h: Provides functions for making HTTP requests.
nlohmann/json.hpp: A JSON library for parsing and manipulating JSON data.
string: Used for string operations.
vector: Implements a dynamic array.
mutex: Provides synchronization primitives for protecting shared data.
queue: Implements a queue data structure.
thread: Allows concurrent execution of multiple threads.
Functions

WriteCallback
This function is a callback used by the libcurl library for writing HTTP response data. It appends the received data to the output string.

fetchdata
This function fetches data from a given URL using the libcurl library. It parses the JSON response and extracts the target token's price from each exchange. The obtained prices are stored in a priority queue, sorted in descending order based on the price.

main
The main function is the entry point of the program. It prompts the user to enter the token they want to trade and the target currency. It initializes a vector of exchanges, a priority queue to store the token prices, and a vector of threads.

Next, it iterates through each exchange, constructs the API URL for fetching token prices, and creates a thread for fetching data from that exchange. Each thread executes the fetchdata function.

After all the threads have completed their execution, the program finds the maximum and minimum token prices along with their corresponding exchange names from the priority queue.

Finally, it outputs the token prices for each exchange, the best trade opportunity (with the maximum difference in prices), and a closing message.

Usage

To use this program, follow these steps:

Compile the program with the necessary dependencies (e.g., g++ Main.cpp -o output -Wall -std=c++17 -lcurl ).
Run the compiled executable (crypto_arbitrage).
Use ./output command.
Enter the token you want to trade.
Enter the target currency (e.g., USDT, BUSD).
Wait for the program to fetch data from each exchange.
The program will display the token prices for each exchange, the best trade opportunity, and a closing message.
