#include <iostream>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <chrono>

class str {
    std::size_t length;
    std::size_t capacity;
    char* data;
    str(const std::size_t len, char* d) noexcept : length(len), capacity(len+1), data(d) {}
    void clear() noexcept {
        if(capacity!=0) {
            length=0;
            data[0]='\0';
        }
    }
public:
    static const std::size_t nofind=-1;
    str() : length(0), capacity(1), data(new char[capacity]{'\0'}) {}
    str(const char* s): length(std::strlen(s)), capacity(length+1) {
        data=new char[capacity];
        std::copy(s,s+length+1,data);
    }
    str(const str& s): length(s.length), capacity(length+1) {
        data=new char[capacity];
        std::copy(s.data,s.data+length+1,data);
    }
    str(str&& s) noexcept: data(s.data), length(s.length), capacity(s.capacity) {
        s.length=s.capacity=0;
        s.data=nullptr;
    }
    ~str() {
        delete[] data;
    }
    str& operator=(const char* s) {
        std::size_t len=std::strlen(s);
        if(capacity<len+1) {
            char* temp=new char[len+1];
            delete[] data;
            data=temp;
            capacity=len+1;
        }
        std::copy(s,s+len+1,data);
        length=len;
        return *this;
    }
    str& operator=(const str& s) {
        if(this==&s) {
            return *this;
        }
        if(capacity<s.length+1) {
            char* temp=new char[s.length+1];
            delete[] data;
            data=temp;
            capacity=s.length+1;
        }
        std::copy(s.data,s.data+s.length+1,data);
        length=s.length;
        return *this;
    }
    str& operator=(str&& s) noexcept {
        if(this==&s) {
            return *this; 
        }
        delete[] data;
        data=s.data;
        length=s.length;
        capacity=s.capacity;
        s.data=nullptr;
        s.length=s.capacity=0;
        return *this;  
    }
    str operator+(const str& s) const {
        std::size_t newLength=length+s.length;
        char* temp=new char[newLength+1];
        std::copy(data,data+length,temp);
        std::copy(s.data,s.data+s.length+1,temp+length);
        return str(newLength,temp);
    }
    bool operator==(const char* c) const noexcept {
        return (std::strcmp(data,c)==0)? true : false;
    }
    bool operator==(const str& s) const noexcept {
        return (std::strcmp(data,s.data)==0)? true : false;
    }
    bool operator!=(const char* c) const noexcept {
        return (std::strcmp(data,c)!=0)? true : false;
    }
    bool operator!=(const str& s) const noexcept {
        return (std::strcmp(data,s.data)!=0)? true : false;
    }
    char& operator[](const size_t idx) const {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        else {
            return data[idx];
        }
    }
    str& operator+=(const char& c) {
        if(length+2>capacity) {
            size_t newCapacity;
            if(capacity==0) {
                newCapacity=2;
            }
            else { 
                newCapacity=capacity*2;
            }
            char* temp=new char[newCapacity];
            std::copy(data,data+length+1,temp);
            delete[] data;
            data=temp;
            capacity=newCapacity;
        }
        data[length]=c;
        data[++length]='\0';
        return *this;
    }
    str& operator+=(const char* c) {
        std::size_t l=std::strlen(c);
        if(l==0) {
            return *this;
        }
        if(length+l+1>capacity) {
            size_t newCapacity;
            if(capacity*2>length+l+1) {
                newCapacity=capacity*2;
            }
            else { 
                newCapacity=length+l+1;
            }
            char* temp=new char[newCapacity];
            std::copy(data,data+length,temp);
            delete[] data;
            data=temp;
            capacity=newCapacity;
        }
        std::copy(c,c+l+1,data+length);
        length+=l;
        return *this;
    }
    str& operator+=(const str& s) {
        if(this==&s || s.length==0) {
            return *this;
        }
        if(length+s.length+1>capacity) {
            size_t newCapacity;
            if(capacity*2>length+s.length+1) {
                newCapacity=capacity*2;
            }
            else { 
                newCapacity=length+s.length+1;
            }
            char* temp=new char[newCapacity];
            std::copy(data,data+length,temp);
            delete[] data;
            data=temp;
            capacity=newCapacity;
        }
        std::copy(s.data,s.data+s.length+1,data+length);
        length+=s.length;
        return *this;
    }
    friend str operator+(const str& sr, const char* cs);
    friend str operator+(const char* cs, const str& sr);
    friend std::ostream& operator<<(std::ostream& os,const str& s) noexcept;
    friend std::istream& operator>>(std::istream& is, str& s);
    friend std::istream& getline(std::istream& is, str& s);
    std::size_t len() const noexcept {
        return length;
    }
    bool shrink_to_fit() noexcept {
        if(length+1!=capacity) {
            try {
                char* temp=new char[length+1];
                std::copy(data,data+length+1,temp);
                delete[] data;
                data=temp;
                capacity=length+1;
            }
            catch (const std::bad_alloc&) {
                return false;
            }
        }
        return true;
    }
    str substr(const std::size_t len, const std::size_t idx=0) const {
        if(idx>length || idx+len>length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        if(len==0) {
            return str();
        }
        char* c=new char[len+1];
        std::copy(data+idx,data+idx+len,c);
        c[len]='\0';
        return str(len,c);      
    }
    std::size_t find(const char& c, const std::size_t idx=0) const  {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        for(std::size_t a=idx;a<length;a++) {
            if(data[a]==c) {
                return a;
            }
        }
        return nofind;
    }
    std::size_t find(const char* c, const std::size_t idx=0) const { 
        std::size_t len=std::strlen(c);
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        if(length-idx<len) {    
            return nofind;
        }
        std::size_t currentIdx=idx,count=0,i=0,pos=0;
        while(true) {
            if(length-currentIdx<len-count) {
                return nofind;
            }
            if(data[currentIdx]==c[i]) {
                if(count==0) {
                    pos=currentIdx;
                }
                count++;
                i++;
            }
            else {
                i=0;
                if(count!=0) {
                    currentIdx=pos+1;
                    count=0;
                    continue ;
                }
            }
            if(count==len) {
                return pos;
            }
            currentIdx++;
        }
       return nofind;
    }
    std::size_t find(const str& s, const std::size_t idx=0) const { 
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        if(length-idx<s.length) {    
            return nofind;
        }
        std::size_t currentIdx=idx,count=0,i=0,pos=0;
        while(true) {
            if(length-currentIdx<s.length-count) {
                return nofind;
            }
            if(data[currentIdx]==s.data[i]) {
                if(count==0) {
                    pos=currentIdx;
                }
                count++;
                i++;
            }
            else {
                i=0;
                if(count!=0) {
                    currentIdx=pos+1;
                    count=0;
                    continue ;
                }
            }
            if(count==s.length) {
                return pos;
            }
            currentIdx++;
        }
       return nofind;
    }
    std::size_t rfind(const char& c, std::size_t idx=-1) const {
        if(idx==-1) {
            idx=length-1;
        }
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        for(std::size_t a=idx;;a--) {
            if(data[a]==c) {
                return a;
            }
            if(a==0) {
                break;
            }
        }
        return nofind;
    }
    void insert(const char& c, const std::size_t idx=0) {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        if(length+2>capacity) {
            std::size_t newCapacity=capacity*2;
            char* temp=new char[newCapacity];
            std::copy(data,data+length+1,temp); 
            delete[] data;        
            data=temp;
            capacity=newCapacity;
        }
        std::copy_backward(data+idx,data+length+1,data+length+2);
        data[idx]=c;
        length++;
    }
    void insert(const char* c, const std::size_t idx=0) {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access"); 
        }                                                           
        std::size_t l=std::strlen(c);
        if(length+l+1>capacity) {
            std::size_t newCapacity;
            if(length+l+1>capacity*2)  {
                newCapacity=length+l+1;
            }
            else {
                newCapacity=capacity*2;
            }
            char* temp=new char[newCapacity];
            std::copy(data,data+length+1,temp);
            delete[] data;
            data=temp;
            capacity=newCapacity;
        }
        std::copy_backward(data+idx,data+length+1,data+length+l+1);
        std::copy(c,c+l,data+idx);
        length+=l;
    }
    void insert(const str& s, const std::size_t idx=0) {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access"); 
        }                                                           
        if(length+s.length+1>capacity) {
            std::size_t newCapacity;
            if(length+s.length+1>capacity*2)  {
                newCapacity=length+s.length+1;
            }
            else {
                newCapacity=capacity*2;
            }
            char* temp=new char[newCapacity];
            std::copy(data,data+length+1,temp);
            delete[] data;
            data=temp;
            capacity=newCapacity;
        }
        std::copy_backward(data+idx,data+length+1,data+length+s.length+1);
        std::copy(s.data,s.data+s.length,data+idx);
        length+=s.length;
    }
};

str operator+(const str& sr, const char* cs) {
    std::size_t l=std::strlen(cs);
    char* temp=new char[sr.length+l+1];
    std::copy(sr.data,sr.data+sr.length,temp);
    std::copy(cs,cs+l+1,temp+sr.length);
    return str(sr.length+l,temp);
}

str operator+(const char* cs, const str& sr) {
    std::size_t l=std::strlen(cs);
    char* temp=new char[sr.length+l+1];
    std::copy(cs,cs+l,temp);
    std::copy(sr.data,sr.data+sr.length+1,temp+l);
    return str(sr.length+l,temp);
}

std::ostream& operator<<(std::ostream& os, const str& s) noexcept {
    os << s.data;
    return os;
}

std::istream& operator>>(std::istream& is, str& s) { 
    char ch;
    s.clear();
    while(is.get(ch) && ch!='\n' && ch!=' ' && ch!='\t') {
        s+=ch;
    }
    return is;
}

std::istream& getline(std::istream& is, str& s) {
    char ch;
    s.clear();
    while(is.get(ch) && ch!='\n') {
        s+=ch;
    }
    return is;
}

void run_performance_test(int iterations) {
    std::cout << "--- Starting Performance Test (" << iterations << " iterations) ---\n";
    auto start_s2 = std::chrono::high_resolution_clock::now();
    str s2;
    for (int i = 0; i < iterations; ++i) {
        s2 += 'a';
    }
    auto end_s2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_s2 = end_s2 - start_s2;
    std::cout << "string2.cpp (new[]) time  : " << elapsed_s2.count() << " ms\n";
    std::cout << "--------------------------------------------------------\n";
}

int main() {
    run_performance_test(100000);
}