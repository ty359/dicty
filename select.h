#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cmath>

namespace Dicty {

using std::vector;
using std::string;
using std::unordered_set;
using std::unordered_map;

inline double Entropy(const vector<double>& pp) {
    double e = .0;
    for (auto p: pp) {
        e -= p * std::log2(p);
    }
    return e;
}

class Select {
public:
    struct WithEntropy {
        vector<int> s;
        double e;
        double avg_e() const {
            return e / (double)s.size();
        }
        bool operator< (const WithEntropy& other) const {
            return this->avg_e() < other.avg_e();
        }
    };

    int Get(const vector<string>& keys, vector<int>& s, int search_limit = -1);

private:
    bool IsUnique(const vector<int>& s) {
        std::unordered_set<string> s_keys;
        for (auto& key: keys_) {
            string s_key = "";
            s_key.reserve(s.size() + 1);
            for (auto pos: s) {
                if (pos >= (int)key.size())
                    s_key.push_back('\0');
                else
                    s_key.push_back(key[pos]);
            }
            if (s_keys.count(s_key) != 0) {
                return false;
            }
            s_keys.insert(s_key);
        }
        return true;
    }

    double GetEntropy(const vector<int>& s) {
        std::unordered_map<string, int> s_keys;
        for (auto& key: keys_) {
            string s_key = "";
            s_key.reserve(s.size() + 1);
            for (auto pos: s) {
                if (pos >= (int)key.size())
                    s_key.push_back('\0');
                else
                    s_key.push_back(key[pos]);
            }
            s_keys[s_key] += 1;
        }
        
        std::vector<double> pp;
        for (auto& item: s_keys) {
            pp.push_back(item.second / (double)keys_.size());
        }

        return Entropy(pp);
    }

    vector<vector<int>> GetAdjSelect(const vector<int>& s) {
        vector<vector<int>> ret;
        for (int i = 0, j = 0; i < key_max_size_; i ++) {
            if (j < (int)s.size() && i == s[j]) {
                j ++;
            }
            else {
                ret.push_back({});
                ret.back().reserve(s.size() + 1);
                for (int k = 0; k < j; ++ k) {
                    ret.back().push_back(s[k]);
                }
                ret.back().push_back(i);
                for (int k = j; k < (int)s.size(); ++ k) {
                    ret.back().push_back(s[k]);
                }
            }
        }
        return ret;
    }

private:
    vector<string> keys_;
    int key_max_size_;
};

}

namespace std {
template <>
struct hash<vector<int>> {
    size_t operator() (const vector<int>& v) const {
        size_t ret = 0;
        for (auto vv: v) {
            ret = (ret << 25) + (ret >> 39) + vv;
        }
        return ret;
    }
};
}

inline int Dicty::Select::Get(const vector<string>& keys, vector<int>& s, int search_limit) {
    const double EPS = 1e-10;

    // key_max_size
    keys_ = keys;
    key_max_size_ = 0;
    for (auto& key: keys_) {
        key_max_size_ = std::max(key_max_size_, (int)key.size());
    }

    // check unique
    unordered_set<string> unique_check;
    for (auto& key: keys_) {
        if (!unique_check.count(key)) {
            unique_check.insert(key);
        }
        else {
            return -1;
        }
    }

    // search
    unordered_set<vector<int>> vis;
    std::priority_queue<WithEntropy> que;
    que.push({{}, .0});
    vis.insert({});
    while (!que.empty() && search_limit --) {
        auto se = que.top();
        que.pop();
        if (std::abs(std::exp2(se.e) - (double)keys_.size()) < EPS) {
            s = se.s;
            return 0;
        }
        for (auto& next_s: GetAdjSelect(se.s)) {
            if (!vis.count(next_s)) {
                vis.insert(next_s);
                que.push({next_s, GetEntropy(next_s)});
            }
        }
    }

    // not found in search_limit
    s.clear();
    for (int i = 0; i < key_max_size_; ++ i) {
        s.push_back(i);
    }
    return 1;
}
