#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <unordered_set>

namespace Dicty {

using std::vector;
using std::string;

inline int HashSelect(const char *data, size_t size, const vector<int>& select_pos, const vector<int>& beta) {
    int ret = 0;
    for (int i = 0; i < (int)select_pos.size() && select_pos[i] < (int)size; ++ i) {
        ret += data[select_pos[i]] * beta[i];
    }
    return ret;
}

class Dicty {
public:
    void Compile(const vector<string>& keys);
    inline int Get(const char *key_data, size_t key_size) {
        int ret = At(key_data, key_size);
        if (ret == -1) {
            return -1;
        }
        if (key_size_[ret] != key_size || memcmp(key_data_[ret], key_data, key_size) != 0) {
            return -1;
        }
        return ret;
    }
    inline int Get(const string& key) {
        return Get(key.data(), key.size());
    }

    inline int At(const char *key_data, size_t key_size) {
        return buffer_[HashSelect(key_data, key_size, select_pos_, beta_) & buffer_size_bit_mask_];
    }
    inline int At(const string& key) {
        return At(key.data(), key.size());
    }

    void Stat() {
        printf("buffer_size: %d\nselect size: %d\n", (int)buffer_.size(), (int)select_pos_.size());
    }

private:
    bool MakeBuffer(int buffer_size);
    void MakeKeyBuffer();

    vector<string> keys_;
    vector<string> padding_keys_;
    vector<char> key_buffer_;
    vector<char *> key_data_;
    vector<size_t> key_size_; 
    int key_max_size_;
    vector<int> select_pos_;
    vector<int> beta_;
    int buffer_size_bit_mask_;
    vector<int> buffer_;
};

static bool IsUnique(const vector<string>& keys, const vector<int>& select_pos) {
    std::unordered_set<string> s_keys;
    for (auto& key: keys) {
        string s_key = "";
        s_key.reserve(select_pos.size() + 1);
        for (auto pos: select_pos) {
            s_key.push_back(key[pos]);
        }
        if (s_keys.count(s_key) != 0) {
            return false;
        }
        s_keys.insert(s_key);
    }
    return true;
}

// TODO: too slow
static void GetMinSelect(const vector<string>& keys, int key_max_size, vector<int>& select_pos) {
    vector<vector<int>> select_que;
    for (int i = 0; i < key_max_size; ++ i) {
        select_que.push_back({i});
    }

    for (int i = 0; i < (int)select_que.size(); ++ i) {
        if (IsUnique(keys, select_que[i])) {
            select_pos = select_que[i];
            return ;
        }
        for (int j = select_que[i].back() + 1; j < key_max_size; ++ j) {
            auto new_select = select_que[i];
            new_select.push_back(j);
            select_que.emplace_back(new_select);
        }
    }
}

inline int lowbit(int t) {
    return t & -t;
}
inline int highbit(int t) {
    int ret = 0;
    while (t != 0) {
        t -= (ret = lowbit(t));
    }
    return ret;
}

bool Dicty::MakeBuffer(int buffer_size) {
    std::unordered_set<int> bucket;
    for (auto& key: keys_) {
        int k = HashSelect(key.data(), key.size(), select_pos_, beta_) & (buffer_size - 1);
        if (bucket.count(k) != 0) {
            return false;
        }
        else {
            bucket.insert(k);
        }
    }

    buffer_.resize(buffer_size);
    for (int i = 0; i < buffer_size; ++ i) {
        buffer_[i] = -1;
    }
    buffer_size_bit_mask_ = buffer_size - 1;
    
    for (int i = 0; i < (int)keys_.size(); ++ i) {
        auto& key = keys_[i];
        int k = HashSelect(key.data(), key.size(), select_pos_, beta_) & (buffer_size - 1);
        buffer_[k] = i;
    }
    return true;
}

// return least 16x number not less than k
template <typename T>
inline T lower_bound16(T k) {
    const T mask16 = 16 - 1;
    const T rev_mask16 = ~mask16;
    T ret = (k & rev_mask16) + 16 * !!(k & mask16);
    return ret;
}

void Dicty::MakeKeyBuffer() {
    int tot_size = 0;
    for (int i = 0; i < (int)keys_.size(); ++ i) {
        tot_size += lower_bound16(keys_[i].size());
    }
    key_buffer_.resize(tot_size + 16);
    char *key_buffer_data = (char *)lower_bound16((size_t)key_buffer_.data());
    tot_size = 0;
    key_data_.resize(keys_.size());
    key_size_.resize(keys_.size());
    for (int i = 0; i < (int)keys_.size(); ++ i) {
        key_data_[i] = key_buffer_data + tot_size;
        key_size_[i] = keys_[i].size();
        memcpy(key_data_[i], keys_[i].data(), key_size_[i]);
        tot_size += lower_bound16(keys_[i].size());
    }
}

void Dicty::Compile(const vector<string>& keys) {
    keys_ = keys;
    MakeKeyBuffer();
    key_max_size_ = 0;
    for (auto& key: keys_) {
        key_max_size_ = std::max(key_max_size_, (int)key.length());
    }
    padding_keys_ = keys;
    for (auto& key: padding_keys_) {
        key.resize(key_max_size_, '\0');
    }
    GetMinSelect(padding_keys_, key_max_size_, select_pos_);
    beta_.resize(select_pos_.size());

    int buffer_size = highbit(keys.size() * 10);
    while (true) {
        int _ = 300;
        while (_--) {
            for (int i = 0; i < (int)beta_.size(); ++ i) {
                beta_[i] = (rand() & 0xffff) + 1;
            }
            if (MakeBuffer(buffer_size)) {
                return ;
            }
        }
        buffer_size *= 2;
    }
}

}
