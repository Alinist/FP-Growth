#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct FPNode
{
    char item;
    int support;
    unordered_map<char, FPNode *> children;

    FPNode(char itemName, int itemSupport = 0)
        : item(itemName), support(itemSupport) {}

    void addTransaction(string &transaction, int index)
    {
        if (index == transaction.size())
        {
            return;
        }

        char currentItem = transaction[index];
        if (children.find(currentItem) == children.end())
        {
            children[currentItem] = new FPNode(currentItem);
        }
        children[currentItem]->support++;

        children[currentItem]->addTransaction(transaction, index + 1);
    }

    void printTree(int depth = 0)
    {
        string indent(depth * 3, '-');
        if(support == 0)
        {
            cout << item << "->\n";
        }
        else
        {
            cout << indent << "Item : " << item << ", Support : " << support << "\n";
        }
        
        for (auto &child : children)
        {
            child.second->printTree(depth + 1);
        }
    }

    ~FPNode()
    {
        for (auto &child : children)
        {
            delete child.second;
        }
    }
};

struct strongRule
{
    string left;
    string right;
    float support;
    float confidence;
};

vector<pair<int, string>> transactions;
unordered_map<char, int> freqArr;
float min_support, min_confidence;
vector<unordered_map<string, int>> allItemsets;
vector<pair<string, string>> assocationRules;
vector<strongRule> strongRules;

bool freqComp(char item1, char item2)
{
    return (freqArr[item1] > freqArr[item2]);
}

void readExcelFile()
{
    ifstream inFile("..//t14.csv");

    if (!inFile.is_open())
    {
        cout << "ERROR: File Open\n";
        return;
    }

    string line;

    getline(inFile, line);

    while (getline(inFile, line))
    {
        stringstream ss(line);
        int transactionNumber;
        string items;

        ss >> transactionNumber;

        ss.ignore(1, ',');

        getline(ss, items);

        if (items.size() > 0 && items[0] == '"')
            items.erase(0, 1);

        if (items.size() > 1 && items[items.size() - 1] == '"')
            items.erase(items.size() - 1, 1);

        unordered_set<char> seenItems;
        string uniqueItems;

        for (char item : items)
        {
            if (item != ',' && seenItems.find(item) == seenItems.end()) 
            {
                seenItems.insert(item);
                uniqueItems += item;
            }
        }

        transactions.push_back({transactionNumber, uniqueItems});
    }

    inFile.close();
}

void readSuppConf()
{
    cout << "Please enter the minimum support percentage: ";
    cin >> min_support;

    min_support = min_support / 100 * transactions.size();

    cout << "Please enter the minimum confidence percentage: ";
    cin >> min_confidence;

    min_confidence /= 100;

    cout << "min support = " << min_support / transactions.size() << " min conf = " << min_confidence << "\n\n";
}

void filterTransactions()
{
    for (auto &transaction : transactions)
    {
        for (char item : transaction.second)
        {
            freqArr[item]++;
        }
    }

    for (auto &transaction : transactions)
    {
        sort(transaction.second.begin(), transaction.second.end(), freqComp);
    }

    for (auto &transaction : transactions)
    {
        for (int i = 0; i < (int)transaction.second.size(); i++)
        {
            if (freqArr[transaction.second[i]] < min_support)
            {
                transaction.second.erase(transaction.second.begin() + i);
                i--;
            }
        }
        if (!transaction.second.size())
            transactions.erase(transactions.begin() + transaction.first - 1);
    }

    for (auto &item : freqArr)
    {
        if (item.second < min_support)
        {
            freqArr.erase(item.first);
        }
    }
}

void buildFPTree()
{
    cout << "\n///////////////////////////////\n\n\n";

    FPNode *root = new FPNode('#');

    for (auto &transaction : transactions)
    {
        root->addTransaction(transaction.second, 0);
    }

    cout << "FP-Tree Structure:-\n\n";
    root->printTree();

    delete root;
}

void getFreqItemsets()
{
    cout << "\n\n///////////////////////////////\n\n";

    allItemsets.resize(freqArr.size());

    for (int i = 0; i < transactions.size(); i++)
    {
        for (int j = 1; j < (1 << transactions[i].second.size()); j++)
        {
            string subset = "";

            for (int b = 0; b < transactions[i].second.size(); b++)
            {
                if (j & (1 << b))
                {
                    subset += transactions[i].second[b];
                }
            }
            allItemsets[subset.size() - 1][subset]++;
        }
    }

    for (auto &item : freqArr)
    {
        allItemsets[0][string(1, item.first)] = item.second;
    }

    for (int i = 0; i < allItemsets.size(); i++)
    {
        for (auto itemset : allItemsets[i])
        {
            if (itemset.second < min_support)
            {
                allItemsets[i].erase(itemset.first);
            }
        }
    }

    cout << '\n';

    for (int i = 0; i < allItemsets.size(); i++)
    {
        if (allItemsets[i].empty())
            continue;
        cout << "All Itemsets of size " << i + 1 << " :-\n\n";
        for (auto itemset : allItemsets[i])
        {
            cout << " -Itemset : " << itemset.first << ", Support Count : " << itemset.second << '\n';
        }
        cout << '\n';
    }
}

void getAssociationRules()
{
    for (int i = 1; i < allItemsets.size(); i++)
    {
        for (auto &item : allItemsets[i])
        {
            string currentString = item.first;
            int n = currentString.size();

            for (int j = 1; j < (1 << n) - 1; j++)
            {
                string subset = "";

                for (int b = 0; b < n; b++)
                {
                    if (j & (1 << b))
                    {
                        subset += currentString[b];
                    }
                }

                assocationRules.push_back({subset, currentString});
            }
        }
    }

    cout << '\n';
    cout << "///////////////////////////////\n";
    cout << "\n\n";
    cout << "Association Rules :- \n\n";
    for (int i = 0; i < assocationRules.size(); i++)
    {
        string left = assocationRules[i].first;
        string right = assocationRules[i].second;
        for (int i = 0; i < left.size(); i++)
        {
            right.erase(find(right.begin(), right.end(), left[i]));
        }
        cout << " -Rule = " << left << " -> " << right << '\n';
    }

    cout << "\n\n///////////////////////////////\n\n\n";
}

void getStrongRules()
{
    for (int i = 0; i < assocationRules.size(); i++)
    {
        string currentString = assocationRules[i].second;
        string subset = assocationRules[i].first;
        float conf = float(allItemsets[currentString.size() - 1][currentString]) / allItemsets[subset.size() - 1][subset];

        if (min_confidence <= conf)
        {
            string right = currentString;
            for (int i = 0; i < subset.size(); i++)
            {
                right.erase(find(right.begin(), right.end(), subset[i]));
            }

            float support = float(allItemsets[currentString.size() - 1][currentString]) / transactions.size();

            struct strongRule tmp;

            tmp.confidence = conf;
            tmp.support = support;
            tmp.left = subset;
            tmp.right = right;

            strongRules.push_back(tmp);
        }
    }
    cout << "Strong Rules :- \n\n";
    for (int i = 0; i < strongRules.size(); i++)
    {
        cout << " -Rule = " << strongRules[i].left << " -> " << strongRules[i].right << ", support : " << strongRules[i].support << ", confidence : " << strongRules[i].confidence << '\n';
    }

    cout << "\n\n///////////////////////////////\n\n\n";
}

void getLift()
{
    cout << "Lift for Strong Rules :- \n\n";
    for (int i = 0; i < strongRules.size(); i++)
    {
        string tmp = strongRules[i].left + strongRules[i].right;

        sort(tmp.begin(), tmp.end(), freqComp);

        float supLeft = (float)allItemsets[strongRules[i].left.size() - 1][strongRules[i].left] / transactions.size();
        float supRight = (float)allItemsets[strongRules[i].right.size() - 1][strongRules[i].right] / transactions.size();
        float supTop = (float)allItemsets[tmp.size() - 1][tmp] / transactions.size();

        float lift = supTop / (supLeft * supRight);

        cout << " -Rule = " << strongRules[i].left << " -> " << strongRules[i].right << ", Lift : " << lift << ", ";

        if (lift > 1)
            cout << "Positive Correlation\n";
        else if (lift < 1)
            cout << "Negative Correlation\n";
        else
            cout << "No Correlation\n";
    }

    cout << "\n\n///////////////////////////////\n";
}

int main()
{
    readExcelFile();

    readSuppConf();

    filterTransactions();

    buildFPTree();

    getFreqItemsets();

    getAssociationRules();

    getStrongRules();

    getLift();  

    return 0;
}