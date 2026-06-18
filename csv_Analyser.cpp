#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Order {
    string user;
    int qty;
    double price;
};

class Book {
public:
    string ticker;
    vector<Order> buy, sell;
    Book(string t) : ticker(t) {}
};

vector<string> split(string s, char c) {
    vector<string> res;
    stringstream ss(s);
    string item;
    while (getline(ss, item, c)) res.push_back(item);
    return res;
}

vector<string> parse(string s) {
    vector<string> res;
    stringstream ss(s);
    string item;
    while (ss >> item) res.push_back(item);
    return res;
}

void part1() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    ifstream in(file);
    vector<string> tickers;
    vector<int> counts;
    string line;
    while (getline(in, line)) {
        auto f = split(line, ',');
        if (f.size() < 3) continue;
        string t = f[2];
        int idx = -1;
        for (int i = 0; i < tickers.size(); i++)
        if (tickers[i] == t) { idx = i; break; }
        if (idx != -1) counts[idx]++;
        else { tickers.push_back(t); counts.push_back(1); }
    }
    in.close();
    for (int i = 0; i < tickers.size(); i++)
        for (int j = i + 1; j < tickers.size(); j++)
            if (tickers[j] < tickers[i]) {
                swap(tickers[i], tickers[j]);
                swap(counts[i], counts[j]);
            }
    for (int i = 0; i < tickers.size(); i++)
        cout << tickers[i] << " " << counts[i] << "\n";
}

void part2() {
    string file;
    cin >> file;
    cin.ignore(1000, '\n');
    ifstream in(file);
    vector<string> users;
    vector<int> qtys;
    string line;
    while (getline(in, line)) {
        auto f = split(line, ',');
        if (f.size() < 4) continue;
        string u = f[1];
        int q = stoi(f[3]);
        int idx = -1;
        for (int i = 0; i < users.size(); i++)
            if (users[i] == u) { idx = i; break; }
        if (idx != -1) qtys[idx] += q;
        else { users.push_back(u); qtys.push_back(q); }
    }
    in.close();
    string u;
    while (getline(cin, u)) {
        if (u.empty() || u[0] == '\r') break;
        while (!u.empty() && (u.back() == ' ' || u.back() == '\r')) u.pop_back();
        int idx = -1;
        for (int i = 0; i < users.size(); i++)
            if (users[i] == u) { idx = i; break; }
        cout << u << " " << (idx != -1 ? qtys[idx] : 0) << "\n";
    }
}

void part3() {
    string filePath = "./actual_output/Q1/CSV/";
    string YOUR_ROLL_NUMBER = "CS24M039";
    string fileName;
    cin >> fileName;
    string file = filePath + YOUR_ROLL_NUMBER + "/" + fileName;
    cin.ignore(1000, '\n');
    ofstream out(file);
    out << "Ticker,Seller,Buyer,Qty,Price,Time\n";
    vector<Book> books;
    int tm = 0;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) break;
        auto f = parse(line);
        if (f.size() < 5) continue;
        string ty = f[0], u = f[1], t = f[2];
        int q = stoi(f[3]);
        double p = stod(f[4]);
        int bid = -1;
        for (int i = 0; i < books.size(); i++)
            if (books[i].ticker == t) { bid = i; break; }
        if (bid == -1) { books.push_back(Book(t)); bid = books.size() - 1; }
        if (ty == "BUY") {
            while (q > 0 && !books[bid].sell.empty()) {
                int si = 0;
                for (int i = 1; i < books[bid].sell.size(); i++)
                    if (books[bid].sell[i].price < books[bid].sell[si].price) si = i;
                if (books[bid].sell[si].price > p) break;
                int mq = (q < books[bid].sell[si].qty) ? q : books[bid].sell[si].qty;
                out << t << "," << books[bid].sell[si].user << "," << u << "," << mq << "," << fixed << setprecision(2) << books[bid].sell[si].price << "," << tm++ << "\n";
                q -= mq; books[bid].sell[si].qty -= mq;
                if (books[bid].sell[si].qty == 0) books[bid].sell.erase(books[bid].sell.begin() + si);
            }
            if (q > 0) books[bid].buy.push_back({u, q, p});
        } else {
            while (q > 0 && !books[bid].buy.empty()) {
                int bi = 0;
                for (int i = 1; i < books[bid].buy.size(); i++)
                    if (books[bid].buy[i].price > books[bid].buy[bi].price) bi = i;
                if (books[bid].buy[bi].price < p) break;
                int mq = (q < books[bid].buy[bi].qty) ? q : books[bid].buy[bi].qty;
                out << t << "," << u << "," << books[bid].buy[bi].user << "," << mq << "," << fixed << setprecision(2) << p << "," << tm++ << "\n";
                q -= mq; books[bid].buy[bi].qty -= mq;
                if (books[bid].buy[bi].qty == 0) books[bid].buy.erase(books[bid].buy.begin() + bi);
            }
            if (q > 0) books[bid].sell.push_back({u, q, p});
        }
    }
    out.close();
}

int main() {
    string part;
    cin >> part;
    
    if (part == "P1") {
        part1();
    } else if (part == "P2") {
        part2();
    } else {
        part3();
    }
    return 0;
}