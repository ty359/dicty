#include "dicty.h"
#include "select.h"

#include <unordered_map>

namespace Dicty {

template <typename T>
inline T lowbit(T t) {
    return t & -t;
}
template <typename T>
inline T highbit(T t) {
    T ret = 0;
    while (t != 0) {
        t -= (ret = lowbit(t));
    }
    return ret;
}

// return least 16x number not less than k
template <typename T>
inline T lower_bound16(T k) {
    const T mask16 = 16 - 1;
    const T rev_mask16 = ~mask16;
    T ret = (k & rev_mask16) + 16 * !!(k & mask16);
    return ret;
}

void Dicty::Compile(const vector<string>& keys) {
    keys_ = keys;
    MakeKeyBuffer();
    key_max_size_ = 0;
    for (auto& key: keys_) {
        key_max_size_ = std::max(key_max_size_, (int)key.length());
    }
    Select().Get(keys_, select_pos_, 100000);

    beta_.resize(select_pos_.size());

    int buffer_size = highbit((int)keys.size() * 10);
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

void Dicty::MakeKeyBuffer() {
    int tot_size = 0;
    for (int i = 0; i < (int)keys_.size(); ++ i) {
        tot_size += lower_bound16((int)keys_[i].size());
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
        tot_size += lower_bound16((int)keys_[i].size());
    }
}

}