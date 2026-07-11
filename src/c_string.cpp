#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
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
    str(): length(0), capacity(1) {
        data=static_cast<char *>(std::malloc(sizeof(char) * capacity));
        if(data==NULL) {
            throw std::runtime_error("Memory allocation failure");
        }
        data[0]='\0';
    }
    str(const char* s): length(std::strlen(s)), capacity(length+1) {
        data=static_cast<char *>(std::malloc(sizeof(char) * capacity));
        if(data==NULL) {
            throw std::runtime_error("Memory allocation failure");
        }
        strcpy(data,s);
    }
    str(const str& s): length(s.length), capacity(length+1) {
        data=static_cast<char *>(std::malloc((sizeof(char) * capacity)));
        if(data==NULL) {
            throw std::runtime_error("Memory allocation failure");
        }
        strcpy(data,s.data);
    }
    str(str&& s) noexcept: data(s.data), length(s.length), capacity(s.capacity) {
        s.length=s.capacity=0;
        s.data=NULL;
    }
    ~str() {
        std::free(data);
    }
    str& operator=(const str& s) {
        if(this==&s) {
            return *this;
        }
        if(capacity<s.length+1) {
            char* temp=static_cast<char *>(std::realloc(data,(sizeof(char) * (s.length+1))));
            if(temp!=NULL) {
                data=temp;
                capacity=s.length+1;
            }
            else{
                throw std::runtime_error("Memory allocation failure");
            }
        }
        length=s.length;
        strcpy(data,s.data);
        return *this;
    }
    str& operator=(const char* s) {
        std::size_t len=std::strlen(s);
        if(capacity<len+1) {
            char* temp=static_cast<char *>(std::realloc(data,(sizeof(char) * (len+1))));
            if(temp!=NULL) {
                data=temp;
                capacity=len+1;
            }
            else{
                throw std::runtime_error("Memory allocation failure");
            }
        }
        length=len;
        strcpy(data,s);
        return *this;
    }
    str& operator=(str&& s) noexcept {
        if(this==&s) {
            return *this; 
        }
        std::free(data);
        data=s.data;
        length=s.length;
        capacity=s.capacity;
        s.data=NULL;
        s.length=s.capacity=0;
        return *this;  
    }
    str operator+(const str& s) const {
        std::size_t newLength=length+s.length;
        char* temp=static_cast<char *>(std::malloc(sizeof(char) * (newLength+1)));
        if(temp!=NULL) {
            std::strcpy(temp,data);
            std::strcat(temp,s.data);
            return str(newLength,temp);
        }
        else {
            throw std::runtime_error("Memory allocation failure");
        }
    }
    bool operator==(const char* c) const noexcept {
        if(strcmp(data,c)==0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator==(const str& s) const noexcept {
        if(strcmp(data,s.data)==0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator!=(const char* c) const noexcept {
        if(strcmp(data,c)!=0) {
            return true;
        }
        else {
            return false;
        }
    }
    bool operator!=(const str& s) const noexcept {
        if(strcmp(data,s.data)!=0) {
            return true;
        }
        else {
            return false;
        }
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
        if(capacity<=length+1) {
            size_t newCapacity;
            if(capacity==0) {
                newCapacity=2;
            }
            else { 
                newCapacity=capacity*2;
            }
            char* temp=static_cast<char *>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                capacity=newCapacity;
            }
            else{
                throw std::runtime_error("Memory allocation failure");
            }
        }
        data[length]=c;
        data[++length]='\0';
        return *this;
    }
    str& operator+=(const char* c) {
        std::size_t len=std::strlen(c);
        if(capacity>=len+length+1) {
            std::strcat(data,c);
            length+=len;
        }
        else {
            char* temp;
            std::size_t newCapacity;
            if(len+length+1>capacity*2)  {
                newCapacity=length+len+1;
            }
            else {
                newCapacity=capacity*2;
            }
            temp=static_cast<char *>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                std::strcat(data,c);
                length+=len;
                capacity=newCapacity;
            }
            else {
                throw std::runtime_error("Memory allocation failure");
            }
        }
        return *this;
    }
    str& operator+=(const str& s) {
        if(this==&s) {
            return *this;
        }
        if(capacity>=length+s.length+1) {
            std::strcat(data,s.data);
            length+=s.length;
        }
        else {
            char* temp;
            std::size_t newCapacity;
            if(length+s.length+1>capacity*2) {
                newCapacity=length+s.length+1;
            }
            else {
                newCapacity=capacity*2;
            }
            temp=static_cast<char *>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                std::strcat(data,s.data);
                length+=s.length;
                capacity=newCapacity;
            }
            else {
                throw std::runtime_error("Memory allocation failure");
            }
        }
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
            char* temp=static_cast<char *>(std::realloc(data,(sizeof(char) * (length+1))));
            if(temp!=NULL) {
                data=temp;
                capacity=length+1;
                return true;
            }
            else {
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
        char* c=static_cast<char *>(std::malloc(sizeof(char) * (len+1)));
        if(c==NULL) {
            throw std::runtime_error("Memory allocation failure");
        }
        else {
            std::strncpy(c,data+idx,len);
            c[len]='\0';
            return str(len,c);  
        }
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
            if(capacity==0) {
                newCapacity=2;
            }
            char* temp=static_cast<char*>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                capacity=newCapacity;
            }
            else {
                throw std::runtime_error("Memory allocation failure");
            }
        }
        std::memmove(data+idx+1,data+idx,(length-idx)+1);
        std::memcpy(data+idx,&c,sizeof(char));
        length++;
    }
    void insert(const char* c, const std::size_t idx=0) {
        if(idx>=length) {
            throw std::out_of_range("Out of bounds memory access");
        }
        std::size_t len=std::strlen(c);
        if(length+len+1>capacity) {
            std::size_t newCapacity;
            if(length+len+1>capacity*2)  {
                newCapacity=length+len+1;
            }
            else {
                newCapacity=capacity*2;
            }
            char* temp=static_cast<char *>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                capacity=newCapacity;
            }
            else {
                throw std::runtime_error("Memory allocation failure");
            }
        }
        std::memmove(data+idx+len,data+idx,(length-idx)+1);
        std::memcpy(data+idx,c,len);
        length+=len;
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
            char* temp=static_cast<char *>(std::realloc(data,sizeof(char) * newCapacity));
            if(temp!=NULL) {
                data=temp;
                capacity=newCapacity;
            }
            else {
                throw std::runtime_error("Memory allocation failure");
            }
        }
        std::memmove(data+idx+s.length,data+idx,(length-idx)+1);
        std::memcpy(data+idx,s.data,s.length);
        length+=s.length;
    }
};

str operator+(const str& sr, const char* cs) {
    std::size_t newLength=sr.length+std::strlen(cs);
    char* temp=static_cast<char *>(std::malloc(sizeof(char) * (newLength+1)));
    if(temp!=NULL) {
        std::strcpy(temp,sr.data);
        std::strcat(temp,cs);
        return str(newLength,temp);
    }
    else {
        throw std::runtime_error("Memory allocation failure");
    }
}

str operator+(const char* cs, const str& sr) {
    std::size_t newLength=sr.length+std::strlen(cs);
    char* temp=static_cast<char *>(std::malloc(sizeof(char) * (newLength+1)));
    if(temp!=NULL) {
        std::strcpy(temp,cs);
        std::strcat(temp,sr.data);
        return str(newLength,temp);
    }
    else {
        throw std::runtime_error("Memory allocation failure");
    }
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
    auto start_s1 = std::chrono::high_resolution_clock::now();
    str s1;
    for (int i = 0; i < iterations; ++i) {
        s1 += 'a';
    }
    auto end_s1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_s1 = end_s1 - start_s1;
    std::cout << "string.cpp (realloc) time : " << elapsed_s1.count() << " ms\n";
}

int main() {
    run_performance_test(100000);
}