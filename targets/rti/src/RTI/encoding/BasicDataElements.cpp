#include "RTI/encoding/BasicDataElements.h"

#include "Endianness.h"
#include "RTI/VariableLengthData.h"


/// Fixed Length applies to the SimpleDataType. During encoding and decoding we just
/// pointer cast in here (which would fail hard for std::string e.g)
#define IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(EncodableDataType, SimpleDataType, Length)\
\
\
class EncodableDataType##Implementation {\
    SimpleDataType value_;\
    SimpleDataType* pValue_;\
\
public:\
    EncodableDataType##Implementation() : value_(), pValue_(&value_) {}\
    explicit EncodableDataType##Implementation(const SimpleDataType& value) : value_(value), pValue_(&value_) {}\
    explicit EncodableDataType##Implementation(SimpleDataType* const pValue) : value_(), pValue_(pValue) {}\
    EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs) : value_(*rhs.pValue_), pValue_(&value_) {}\
    EncodableDataType##Implementation& operator=(const EncodableDataType##Implementation& rhs) {\
        if(this == &rhs) {return *this;}\
        *pValue_ = rhs.get();\
        return *this;\
    }\
    EncodableDataType##Implementation& operator=(const SimpleDataType& rhs) {\
        *pValue_ = rhs;\
        return *this;\
    }\
    void setDataPointer(SimpleDataType* inData) throw (EncoderException) {\
        if(inData == NULL) {\
            throw EncoderException(L"Null");\
        }\
        pValue_ = inData;\
    }\
    \
    void set(SimpleDataType inData) { *pValue_ = inData; }\
    SimpleDataType get() const { return *pValue_; }\
    \
    void decode(VariableLengthData const & inData) throw (EncoderException); \
    void encode(VariableLengthData& inData) const throw (EncoderException);\
    Integer64 hash() const;\
    \
    VariableLengthData encode() const throw (EncoderException) {                                         \
        VariableLengthData data;                                                                                            \
        this->encode(data);                                                                                                 \
        return data;\
    }\
    \
    unsigned int getOctetBoundary() const {\
        return Length;\
    }\
\
    size_t getEncodedLength() const throw (EncoderException) {\
        return Length;\
    }\
    \
    void encodeInto(std::vector<Octet>& buffer) const throw (EncoderException) {\
        const VariableLengthData encodedData(encode());\
        for(int i = 0; i < encodedData.size(); ++i) {\
            buffer.push_back(static_cast<const char*>(encodedData.data())[i]);\
        }\
    }\
    \
    size_t decodeFrom(std::vector<Octet> const & inData, size_t index) throw (EncoderException) {\
        if(inData.size() < index + getEncodedLength()) {\
            throw EncoderException( L"Cannot decode from buffer. Remaining buffer is too small to contain our data." );\
        }\
        VariableLengthData encodedData(&inData.data()[index], getEncodedLength());\
        decode(encodedData);\
        return index + getEncodedLength();\
    }\
    \
}; \


#define IMPLEMENT_ENCODING_HELPER_CLASS(EncodableDataType, SimpleDataType)                                              \
EncodableDataType::EncodableDataType() : _impl(new EncodableDataType##Implementation()) {}                              \
                                                                                                                        \
EncodableDataType::EncodableDataType(SimpleDataType const & inData) :                                                   \
    _impl(new EncodableDataType##Implementation(inData)) {}                                                             \
                                                                                                                        \
EncodableDataType::EncodableDataType(SimpleDataType* inData) :                                                          \
    _impl(new EncodableDataType##Implementation(inData)) {}                                                             \
                                                                                                                        \
EncodableDataType::EncodableDataType(EncodableDataType const & rhs) :                                                   \
    _impl(new EncodableDataType##Implementation(*rhs._impl)) {}                                                         \
                                                                                                                        \
EncodableDataType::~EncodableDataType() {                                                                               \
    delete _impl;                                                                                                       \
}                                                                                                                       \
                                                                                                                        \
EncodableDataType& EncodableDataType::operator=(EncodableDataType const & rhs) {                                        \
    if(this == &rhs) {return *this;}                                                                                    \
    _impl->operator=(*rhs._impl);                                                                                       \
    return *this;                                                                                                       \
}                                                                                                                       \
                                                                                                                        \
EncodableDataType& EncodableDataType::operator=(SimpleDataType rhs) {                                                   \
    _impl->operator=(rhs);                                                                                              \
    return *this;                                                                                                       \
}                                                                                                                       \
                                                                                                                        \
std::auto_ptr<DataElement> EncodableDataType::clone() const {                                                           \
    return std::auto_ptr<DataElement>(new EncodableDataType(*this));                                                    \
}                                                                                                                       \
                                                                                                                        \
SimpleDataType EncodableDataType::get() const {                                                                         \
    return _impl->get();                                                                                                \
}                                                                                                                       \
                                                                                                                        \
void EncodableDataType::set(SimpleDataType inData) {                                                                    \
    _impl->set(inData);                                                                                                 \
}                                                                                                                       \
                                                                                                                        \
void EncodableDataType::setDataPointer(SimpleDataType* inData) throw (EncoderException) {                               \
    _impl->setDataPointer(inData);                                                                                      \
}                                                                                                                       \
                                                                                                                        \
EncodableDataType::operator SimpleDataType () const {                                                                   \
    return _impl->get();                                                                                                \
}                                                                                                                       \
                                                                                                                        \
VariableLengthData EncodableDataType::encode() const throw (EncoderException) {                                         \
    return _impl->encode();                                                                                             \
}                                                                                                                       \
                                                                                                                        \
unsigned int EncodableDataType::getOctetBoundary() const {                                                              \
    return _impl->getOctetBoundary();                                                                                   \
}                                                                                                                       \
                                                                                                                        \
size_t EncodableDataType::getEncodedLength() const throw (EncoderException) {                                           \
    return _impl->getEncodedLength();                                                                                   \
}                                                                                                                       \
                                                                                                                        \
void EncodableDataType::encodeInto(std::vector<Octet>& buffer) const throw (EncoderException) {                         \
    return _impl->encodeInto(buffer);                                                                                   \
}                                                                                                                       \
                                                                                                                        \
size_t EncodableDataType::decodeFrom(std::vector<Octet> const & inData, size_t index) throw (EncoderException) {        \
    return _impl->decodeFrom(inData, index);                                                                            \
}                                                                                                                       \
                                                                                                                        \
void EncodableDataType::decode(VariableLengthData const & inData) throw (EncoderException) {                            \
    _impl->decode(inData);                                                                                              \
}                                                                                                                       \
                                                                                                                        \
void EncodableDataType::encode(VariableLengthData& inData) const throw (EncoderException) {                             \
    _impl->encode(inData);                                                                                              \
}                                                                                                                       \
                                                                                                                        \
Integer64 EncodableDataType::hash() const {                                                                             \
    return _impl->hash();                                                                                               \
}                                                                                                                       \
                                                                                                                        \

namespace rti1516e {
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIchar, char )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAboolean, bool, 4)
    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAbyte, Octet, 1)
    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat32BE, float, 4)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32LE, float )
    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat64BE, double, 8)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat64LE, double )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16LE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16BE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32BE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32LE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64BE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64LE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctet, Octet )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairBE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairLE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeChar, wchar_t )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeString, std::wstring )

    /*************************************************************************
     *  HLAboolean
     ************************************************************************/

    Integer64 HLAbooleanImplementation::hash() const {
        return this->get();
    }

    void HLAbooleanImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        char byteBuffer[getEncodedLength()];
        int value = this->get() ? 1 : 0;
        tt::memcpybe(byteBuffer, reinterpret_cast<char*>(&value), getEncodedLength());
        inData.setData(byteBuffer, getEncodedLength());
    }

    void HLAbooleanImplementation::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        int decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        if(decodedData != 0 && decodedData != 1) {
            throw EncoderException(L"Decoded HLAboolean was neither 1 nor 0. Our decoder is strict.");
        }
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAbyte
     ************************************************************************/

    Integer64 HLAbyteImplementation::hash() const {
        return this->get() + 1;
    }

    void HLAbyteImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const char value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAbyteImplementation::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const char*>(inData.data()));
    }

    /*************************************************************************
     *  HLAfloat32BE
     ************************************************************************/

    Integer64 HLAfloat32BEImplementation::hash() const {
        Integer64 returnHash(0);
        const float inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAfloat32BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const float value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAfloat32BEImplementation::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const float*>(inData.data()));
    }

    /*************************************************************************
     *  HLAfloat64BE
     ************************************************************************/

    Integer64 HLAfloat64BEImplementation::hash() const
    {
        Integer64 returnHash(0);
        const double inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAfloat64BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const double value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAfloat64BEImplementation::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const double*>(inData.data()));
    }

    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIchar, char )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAboolean, bool)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAbyte, Octet)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat32BE, float)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32LE, float )
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat64BE, double)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat64LE, double )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16LE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16BE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32BE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32LE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64BE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64LE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctet, Octet )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairBE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairLE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeChar, wchar_t )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeString, std::wstring )

}