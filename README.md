# FP-Growth Algorithm Implementation

This project provides a C++ implementation of the **FP-Growth algorithm** for mining frequent itemsets and generating association rules from transactional data. It also calculates the lift for strong association rules to determine the correlation between itemsets.

---

## Table of Contents

* [Introduction](#introduction)
* [Features](#features)
* [How it Works](#how-it-works)
* [Project Structure](#project-structure)
* [Getting Started](#getting-started)
    * [Prerequisites](#prerequisites)
    * [Building and Running](#building-and-running)
* [Input Data Format](#input-data-format)
* [Output Explanation](#output-explanation)
* [Team Members](#team-members)
* [Contributing](#contributing)
* [License](#license)

---

## Introduction

The FP-Growth (Frequent Pattern Growth) algorithm is an efficient and scalable method for mining frequent itemsets. Unlike Apriori, it doesn't generate candidate itemsets, which can significantly reduce the computational cost, especially for large datasets. This implementation further extends the analysis by deriving association rules and evaluating their strength using confidence and lift metrics.

---

## Features

* **FP-Tree Construction**: Efficiently builds an FP-Tree from transactional data.
* **Frequent Itemset Mining**: Discovers all frequent itemsets based on a user-defined minimum support.
* **Association Rule Generation**: Generates association rules from the mined frequent itemsets.
* **Strong Rule Identification**: Filters association rules based on a user-defined minimum confidence.
* **Lift Calculation**: Computes the lift for strong association rules to determine positive, negative, or no correlation.
* **CSV Data Handling**: Reads transactional data from a CSV file.

---

## How it Works

The project follows these main steps:

1.  **Data Reading**: Reads transactional data from a specified CSV file (`t14.csv`). Each transaction is expected to have a transaction number and a comma-separated list of items.
2.  **Support and Confidence Input**: Prompts the user to enter the desired minimum support and minimum confidence thresholds.
3.  **Transaction Filtering and Sorting**:
    * Calculates the frequency of each individual item.
    * Sorts items within each transaction based on their global frequency in descending order.
    * Removes infrequent items (those below `min_support`) from transactions and the global frequency map.
4.  **FP-Tree Construction**: Builds an FP-Tree by inserting the pre-processed transactions.
5.  **Frequent Itemset Generation**: Iterates through all possible subsets of items within each filtered transaction to identify frequent itemsets, ensuring they meet the `min_support` threshold.
6.  **Association Rule Generation**: For each frequent itemset, it generates all possible association rules (e.g., if `{A, B, C}` is frequent, it generates `A -> B,C`, `B -> A,C`, `C -> A,B`, `A,B -> C`, etc.).
7.  **Strong Rule Identification**: Calculates the confidence for each generated association rule. Rules with confidence equal to or above `min_confidence` are classified as "strong rules."
8.  **Lift Calculation**: For each strong rule, the lift metric is calculated to determine the type of correlation (positive, negative, or no correlation) between the antecedent (left side) and consequent (right side) of the rule.

---

## Project Structure

* `main.cpp`: Contains the main logic for reading data, building the FP-Tree, mining frequent itemsets, generating rules, and calculating lift.
* `FPNode` struct: Represents a node in the FP-Tree, holding an item, its support count, and pointers to its children.
* `strongRule` struct: Stores details of a strong association rule (left side, right side, support, confidence).
* `t14.csv`: (Expected) The input CSV file containing transactional data.

---

## Getting Started

### Prerequisites

* A C++ compiler (e.g., g++).

### Building and Running

1.  **Clone the repository:**

    ```bash
    git clone [https://github.com/your-username/fp-growth-project.git](https://github.com/your-username/fp-growth-project.git)
    cd fp-growth-project
    ```

2.  **Place your data file:**
    Ensure you have your transactional data in a CSV file named `t14.csv` in the same directory as your source code.

3.  **Compile the code:**

    ```bash
    g++ main.cpp -o fp_growth -std=c++11
    ```

4.  **Run the executable:**

    ```bash
    ./fp_growth
    ```
    The program will then prompt you to enter the minimum support and confidence percentages.

---

## Input Data Format

The program expects a CSV file named `t14.csv` with the following structure:

```csv
TransactionID,Items
1,"A,B,C"
2,"B,C,D"
3,"A,C,E"
4,"A,B,D"
````

- The first line is expected to be a header row (it will be skipped).
- Each subsequent line represents a transaction.
- The first column is the `TransactionID` (an integer).
- The second column contains the `Items` in that transaction, separated by commas. Items can be single characters. Double quotes around the items are handled by the parser.

---

## Output Explanation

The program will output the following in the console:

- **Minimum Support and Confidence**: Displays the calculated absolute minimum support and confidence values.
- **FP-Tree Structure**: A hierarchical representation of the constructed FP-Tree, showing items and their support counts.
- **All Frequent Itemsets**: Lists all itemsets that meet the minimum support threshold, grouped by their size, along with their support counts.
- **Association Rules**: Presents all potential association rules derived from the frequent itemsets.
- **Strong Rules**: Lists only the association rules that satisfy both the minimum support and minimum confidence thresholds, along with their calculated support and confidence.
- **Lift for Strong Rules**: For each strong rule, it calculates and displays the lift value, indicating whether the correlation between the antecedent and consequent is positive, negative, or non-existent.

---

## Team Members

| Name                    | GitHub Profile                      |
| :---------------------- | :---------------------------------- |
| Ali Ashraf Ibrahim Said | [Link to Member 1's GitHub Profile] |
| Arsany Adel             | [Link to Member 2's GitHub Profile] |
| Omar Amgad              | [Link to Member 3's GitHub Profile] |
| [Member 4 Name]         | [Link to Member 4's GitHub Profile] |
| [Member 5 Name]         | [Link to Member 5's GitHub Profile] |
| [Member 6 Name]         | [Link to Member 6's GitHub Profile] |