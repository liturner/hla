#include "RTI/VariableLengthData.h"

#include <cstring>

class rti1516e::VariableLengthDataImplementation {
    size_t size_;
    char* data_;
    bool isDataOwner_;
    VariableLengthDataDeleteFunction delete_;

    void clear() {
        if(isDataOwner_) {
            if(delete_ != NULL) {
                delete_(data_);
            } else {
                delete[] data_;
            }
        }
        data_ = NULL;
        delete_ = NULL;
        size_ = 0;
        isDataOwner_ = true;
    }

public:
    VariableLengthDataImplementation() :  size_(0), data_(NULL), isDataOwner_(true), delete_(NULL) {}

    VariableLengthDataImplementation(void const * inData, size_t inSize) : size_(inSize), isDataOwner_(true), delete_(NULL) {
        data_ = new char[inSize];
        memcpy(data_, inData, inSize);
    }

    VariableLengthDataImplementation(VariableLengthDataImplementation const & rhs) : size_(rhs.size_), isDataOwner_(true), delete_(NULL) {
        data_ = new char[size_];
        memcpy(data_, rhs.data_, size_);
    }

    ~VariableLengthDataImplementation() {
        clear();
    }

    VariableLengthDataImplementation& operator=(VariableLengthDataImplementation const & rhs) {
        if(this == &rhs){
            return *this;
        }

        setData(rhs.data_, rhs.size_);
        isDataOwner_ = rhs.isDataOwner_;
        delete_ = rhs.delete_;
        return *this;
    }

    void const * data() const {
        return data_;
    }

    size_t size() const {
        return size_;
    }

    void setData(void const * inData, size_t inSize) {
        clear();
        size_ = inSize;
        data_ = new char[size_];
        memcpy(data_, inData, size_);
    }

    void setDataPointer(void* inData, size_t inSize) {
        clear();
        isDataOwner_ = false;
        data_ = static_cast<char*>(inData);
        size_ = inSize;
    }

    void takeDataPointer(void* inData, size_t inSize, VariableLengthDataDeleteFunction deleteFunction = NULL) {
        clear();
        data_ = static_cast<char*>(inData);
        size_ = inSize;
        delete_ = deleteFunction;
    }
};

rti1516e::VariableLengthData::VariableLengthData () : _impl(new VariableLengthDataImplementation()) {}

rti1516e::VariableLengthData::VariableLengthData(void const *inData, size_t inSize) : _impl(new VariableLengthDataImplementation(inData, inSize)) {}

rti1516e::VariableLengthData::VariableLengthData(VariableLengthData const &rhs) : _impl(new VariableLengthDataImplementation(*rhs._impl)) {}

rti1516e::VariableLengthData::~VariableLengthData() {
    delete _impl;
}

rti1516e::VariableLengthData & rti1516e::VariableLengthData::operator=(VariableLengthData const &rhs) {
    if(this == &rhs) {
        return *this;
    }

    _impl->operator=(*rhs._impl);
    return *this;
}

void const * rti1516e::VariableLengthData::data() const {
    return _impl->data();
}

size_t rti1516e::VariableLengthData::size() const {
    return _impl->size();
}

void rti1516e::VariableLengthData::setData(void const *inData, size_t inSize) {
    return _impl->setData(inData, inSize);
}

void rti1516e::VariableLengthData::setDataPointer(void *inData, size_t inSize) {
    return _impl->setDataPointer(inData, inSize);
}

void rti1516e::VariableLengthData::takeDataPointer(void *inData, size_t inSize, VariableLengthDataDeleteFunction func) {
    return _impl->takeDataPointer(inData, inSize, func);
}
