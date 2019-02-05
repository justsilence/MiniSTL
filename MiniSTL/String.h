#ifndef STRING_H__
#define STRING_H__

#include <iostream>
#include <memory>
#include <utility>

namespace MiniSTL {
    class string {
        friend std::ostream& operator<<(std::ostream &os, const string& str);
        public:
            typedef std::size_t size_type;
            typedef char value_type;
            typedef char * iterator;
            typedef const char * const_iterator;
            

            string() : elem(nullptr), first_free(nullptr), cap(nullptr) { }
            string(const char *);
            string(size_t, char);
            string(const string &);
            string(string &&);
            string& operator=(const string &);
            string& operator=(const char *);
            string& operator=(char);
            char& operator[](size_t n) {
                return elem[n];
            }
            const char& operator[](size_t n) const {
                return elem[n];
            }
            string& operator+=(const string &s);
            string& operator+=(const char* );
            string& operator+=(char );


            ~string();

            void push_back(const char &);
            void push_back(char &&);
            void pop_back();

            const char front() const {
                return *elem;
            }

            const char back() const {
                return *(first_free - 1);
            }

            char& front() {
                return *elem;
            }

            char& back() {
                return *(first_free - 1);
            }

            iterator begin() {
                return elem;
            }

            const_iterator begin() const {
                return elem;
            }

            iterator end() {
                return first_free;
            }

            const_iterator end() const {
                return first_free;
            }

            size_type size() const {
                return first_free - elem;
            }

            size_type length() const {
                return size();
            }

            size_type capacity() const {
                return cap - elem;
            }

            bool empty() const {
                return size() == 0;
            }

            void clear() {
                free();
            }

            const char* c_str() const;



        private:
            static std::allocator<char> alloc;
            void check_n_alloc() {
                if (size() == capacity())
                    reallocate();
            }
            std::pair<char*, char*> alloc_n_copy(const char* , const char* );
            void free();
            void reallocate();
            void reallocate(size_t);
            char *elem;
            char *first_free;
            char *cap;

    };

    std::allocator<char> string::alloc;

    void string::push_back(const char &c) {
        check_n_alloc();
        alloc.construct(first_free++, c);
    }

    void string::push_back(char &&c) {
        check_n_alloc();
        alloc.construct(first_free++, std::move(c));
    }

    void string::pop_back() {
        if (elem) {
            alloc.destroy(--first_free);
        }
    }

    std::pair<char*, char*> string::alloc_n_copy(const char* b, const char* e) {
        auto data = alloc.allocate(e - b);
        return {data, std::uninitialized_copy(b, e, data)};
    }

    string::string(const char* c) {
        auto newdata = alloc_n_copy(c, c + strlen(c));
        elem = newdata.first;
        first_free = cap = newdata.second;
    }

    string::string(size_t sz, char c) {
        elem = alloc.allocate(sz);
        first_free = std::uninitialized_fill_n(elem, sz, c);
        cap = first_free;
    }

    void string::free() {
        if (elem) {
            for (auto p = first_free; p != elem; )
                alloc.destroy(--p);
            alloc.deallocate(elem, cap - elem);
        }
    }

    string::string(const string &s) {
        auto newdata = alloc_n_copy(s.begin(), s.end());
        elem = newdata.first;
        first_free = cap = newdata.second;
    }

    string::string(string &&s) {
        auto newdata = alloc_n_copy(s.begin(), s.end());
        elem = std::move(newdata.first);
        first_free = cap = std::move(newdata.second);
    }

    string& string::operator=(const string& right) {
        auto data = alloc_n_copy(right.begin(), right.end());
        free();
        elem = data.first;
        first_free = cap = data.second;
        return *this;
    }

    string& string::operator=(const char *c) {
        auto data = alloc_n_copy(c, c + strlen(c));
        free();
        elem = data.first;
        first_free = cap = data.second;
        return *this;
    }

    string& string::operator=(char c) {
        free();
        alloc.construct(first_free++, c);
        return *this;
    }

    string& string::operator+=(const string& s) {
        size_t new_size = s.size() + size();
        if (capacity() < new_size) 
            reallocate(new_size);
        for (int i = 0; i < s.size(); ++i) 
            alloc.construct(first_free++, s[i]);
        return *this;
    }

    string& string::operator+=(const char* s) {
        size_t new_size = size() + strlen(s);
        if (capacity() < new_size) 
            reallocate(new_size);
        for (int i = 0; i < strlen(s); ++i) 
            alloc.construct(first_free++, s[i]);
        return *this;
    }

    string& string::operator+=(char c) {
        size_t new_size = size() + 1;
        if (capacity() < new_size)
            reallocate(new_size);
        alloc.construct(first_free++, c);
        return *this;
    }

    string::~string() {
        free();
    }

    void string::reallocate() {
        auto newcapacity = size() ? 2 * size() : 1;
        auto newdata = alloc.allocate(newcapacity);
        auto dest = newdata;
        auto e = elem;
        for (std::size_t i = 0; i != size(); ++i) 
            alloc.construct(dest++, std::move(*e++));
        elem = newdata;
        first_free = dest;
        cap = elem + newcapacity;
    }

    void string::reallocate(size_t n) {
        auto newcapacity = 2 * n;
        auto newdata = alloc.allocate(newcapacity);
        auto dest = newdata;
        auto e = elem;
        for (std::size_t i = 0; i != size(); ++i) 
            alloc.construct(dest++, std::move(*e++));
        elem = newdata;
        first_free = dest;
        cap = elem + newcapacity;
    }

    const char* string::c_str() const {
        char* arr = new char[size() + 1];
        char* curr = elem;
        for (int i = 0; i < size(); ++i) {
            arr[i] = *(curr++);
        }
        arr[size()] = '\0';
        return arr;
    }

    std::ostream& operator<<(std::ostream& os, const string& str) {
        for (auto const ch : str) 
            os << ch;
        return os;
    }

}



#endif