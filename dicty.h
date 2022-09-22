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
    inline int Get(const char *key_data, size_t key_size) const {
        int ret = At(key_data, key_size);
        if (ret == -1) {
            return -1;
        }
        if (key_size_[ret] != key_size || memcmp(key_data_[ret], key_data, key_size) != 0) {
            return -1;
        }
        return ret;
    }
    inline int Get(const string& key) const {
        return Get(key.data(), key.size());
    }

    inline int At(const char *key_data, size_t key_size) const {
        return buffer_[HashSelect(key_data, key_size, select_pos_, beta_) & buffer_size_bit_mask_];
    }
    inline int At(const string& key) const {
        return At(key.data(), key.size());
    }


    void Compile(const vector<string>& keys);

    void Stat() {
        printf("buffer_size: %d\nselect size: %d\n", (int)buffer_.size(), (int)select_pos_.size());
    }

private:
    bool MakeBuffer(int buffer_size);
    void MakeKeyBuffer();

private:
    vector<string> keys_;
    vector<char> key_buffer_;
    vector<char *> key_data_;
    vector<size_t> key_size_; 
    int key_max_size_;
    vector<int> select_pos_;
    vector<int> beta_;
    int buffer_size_bit_mask_;
    vector<int> buffer_;
};

}
