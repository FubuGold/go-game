#if !defined(CUSTOM_UTIL_H)
#define CUSTOM_UTIL_H

#include <utility>

#define BOARD_SIZE 19

/**
 * @brief Custom pair<int,int> queue. Use when need extreme optimize
 * 
 */
struct CustomQueue {
    static const int QU_SZ = BOARD_SIZE * BOARD_SIZE;
    std::pair<int,int> q[QU_SZ];
    int st_ptr = 0,ed_ptr = 0;
    void clear() {ed_ptr = st_ptr;}
    bool empty() {return size() == 0;}
    void push(const std::pair<int,int> &val) {
        q[ed_ptr++] = val;
        if (ed_ptr >= QU_SZ) ed_ptr -= QU_SZ;
    }
    void emplace(int x,int y) {
        q[ed_ptr].first = x;
        q[ed_ptr++].second = y;
        if (ed_ptr >= QU_SZ) ed_ptr -= QU_SZ;
    }
    std::pair<int,int> front() {
        return q[st_ptr];
    }
    void pop() {
        st_ptr++;
        if (st_ptr >= QU_SZ) st_ptr -= QU_SZ;
    }
    int size() {
        if (ed_ptr >= st_ptr) return ed_ptr - st_ptr;
        return ed_ptr + QU_SZ - st_ptr;
    }
};

#endif // CUSTOM_UTIL_H
