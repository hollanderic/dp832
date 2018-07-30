
#include <cstdio>

class stringy;

class stringy {
public:
    stringy(int channel) {chan_ =channel;};
    static stringy create(int channel);
    ~stringy(){ printf("killing %d\n",chan_);};

    void source(char *str, int n);


private:
    int chan_;

};

stringy stringy::create(int channel) {
    stringy strung(channel);
    return  strung;
}

void stringy::source(char* str, int n) {
    snprintf(str,n,"CHAN%d",chan_);
}



int main() {
    char str[100];

    stringy::create(4).source(str,100);

    printf("%s\n",str);
    return 0;

}