//
// Created by nrx on 22.04.2020.
//

#ifndef LABA2_OPTION_H
#define LABA2_OPTION_H

template<typename T>
class Option {
public:
    static constexpr int NOT_CONTAIN = -1;
    Option(const T &r) {
        isOk = true;
        _result = r;
    }
    Option(const T &r, bool flag) {
        isOk = flag;
        _result = r;
    }
    Option() {
        isOk = false;
    }

    operator bool() {
        return isOk;
    }
    T* result() {
      return &_result;
    }
private:
    T _result;
    bool isOk;
};

#endif