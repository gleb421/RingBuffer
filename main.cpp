#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>

using namespace std;

template<typename T>
class ringbuffer {
private:
    T *buff;
    int capac;
    int size;
    int head;
    int tail;

public:
    ringbuffer(int val) : capac(val), size(0), head(0), tail(0), buff(new T[val]) {}

    //1
    void push_in_tail(T val) {
        if (size < capac) {
            buff[tail] = val;
            tail = (tail + 1) % capac;
            size++;
        } else {
            throw out_of_range("Buffer is full!");
        }
    }

    void pop_from_tail() {
        if (size > 0) {
            tail = (tail - 1 + capac) % capac;
            size--;
        } else {
            throw out_of_range("Buffer is empty!");
        }
    }

    //2
    void push_in_head(T val) {
        if (size < capac) {
            head = (head - 1 + capac) % capac;
            buff[head] = val;
            size++;
        } else {
            throw out_of_range("Buffer is full!");
        }
    }

    void pop_from_head() {
        if (size > 0) {
            head = (head + 1) % capac;
            size--;
        } else {
            throw out_of_range("Buffer is empty!");
        }
    }

    class iterator {
    private:
        T *buff;
        int capac;
        int pos;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        iterator(int p, T *b, int c) : pos(p), buff(b), capac(c) {}

        T &operator*() const {
            return buff[pos];
        }

        iterator &operator++() {
            pos = (pos + 1) % capac;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator--() {
            pos = (pos - 1 + capac) % capac;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator &sec) const {
            return pos == sec.pos;
        }

        bool operator!=(const iterator &sec) const {
            return !(*this == sec);
        }

        iterator &operator=(const iterator &sec) {
            if (this != &sec) {
                buff = sec.buff;
                capac = sec.capac;
                pos = sec.pos;
            }
            return *this;
        }

        iterator &operator+=(difference_type n) {
            pos = (pos + n) % capac;
            return *this;
        }

        iterator operator+(difference_type n) const {
            iterator tmp = *this;
            return tmp += n;
        }

        iterator &operator-=(difference_type n) {
            pos = (pos - n + capac) % capac;
            return *this;
        }

        iterator operator-(difference_type n) const {
            iterator tmp = *this;
            return tmp -= n;
        }

        difference_type operator-(const iterator &sec) const {
            return (pos - sec.pos + capac) % capac;
        }

        reference operator[](difference_type n) const {
            return buff[(pos + n) % capac];
        }

        bool operator<(const iterator &sec) const {
            return (*this - sec) < 0;
        }

        bool operator>(const iterator &sec) const {
            return (*this - sec) > 0;
        }

        bool operator<=(const iterator &sec) const {
            return (*this - sec) <= 0;
        }

        bool operator>=(const iterator &sec) const {
            return (*this - sec) >= 0;
        }
    };

    iterator begin() {
        return iterator(head, buff, capac);
    }

    iterator end() {
        return iterator(tail, buff, capac);
    }

    void print_all_elements() {
        iterator it = begin();
        for (int i = 0; i < size; i++) {
            cout << *it << " ";
            ++it;
        }
        cout << endl;
    }

    void insert_on(iterator pos, T val) {
        if (size == capac) {
            throw out_of_range("Buffer is full!");
        }
        iterator it = end();
        for (auto i = it; i != pos; --i) {
            *i = *(i - 1);
        }
        *pos = val;
        size++;
    }

    void erase_on(iterator pos) {
        if (size == 0) {
            throw out_of_range("Buffer is empty");
        }
        for (auto i = pos; i != end(); ++i) {
            *i = *(i + 1);
        }
        size--;
    }

    T &front() {
        if (size == 0) {
            throw out_of_range("Buffer is empty");
        }
        return buff[head];
    }

    T &back() {
        if (size == 0) {
            throw out_of_range("Buffer is empty");
        }
        return buff[(tail - 1 + capac) % capac];
    }

    const T &operator[](int index) const {
        if (index >= size) {
            throw out_of_range("Index out of range");
        }
        return buff[(head + index) % capac];
    }

    void new_capac(int val) {
        T *tmp = new T[val];
        for (int i = 0; i < size; i++) {
            tmp[i] = buff[(head + i) % capac];
        }
        delete[] buff;
        buff = tmp;
        head = 0;
        tail = size;
        capac = val;
    }
};


int main() {
    //test1
    ringbuffer<int> s(5);

    s.push_in_tail(1);
    s.print_all_elements();

    s.push_in_tail(2);
    s.push_in_tail(3);
    s.push_in_head(0);
    s.push_in_head(-1);
    s.print_all_elements();

    s.new_capac(7);

    s.insert_on(s.begin() + 1, -2);

    s.print_all_elements();

    s.erase_on(s.begin() + 2);

    s.print_all_elements();

    int a1 = s.front();
    int a2 = s.back();
    cout << a1 << " " << a2 << "\n";
    cout << s[3] << "\n";
    sort(s.begin(), s.end());
    sort(s.begin(), s.end(), [](const auto& a, const auto& b) {
        return a > b;
    });
    s.print_all_elements();

    //test2
    ringbuffer<vector<int>> p(5);

    vector<int> a = {1, 2, 5, 4, 5};
    p.push_in_head(a);
    a.clear();
    a = p.front();

    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }


}