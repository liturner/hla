#include "RTI/encoding/BasicDataElements.h"

#include "Endianness.h"
#include "RTI/VariableLengthData.h"

/// Fixed Length applies to the SimpleDataType. During encoding and decoding we just
/// pointer cast in here (which would fail hard for std::string e.g)
#define IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(EncodableDataType, SimpleDataType, Length)                    \
                                                                                                                        \
class EncodableDataType##Implementation {                                                                               \
    SimpleDataType value_;                                                                                              \
    SimpleDataType* pValue_;                                                                                            \
                                                                                                                        \
public:                                                                                                                 \
    EncodableDataType##Implementation() : value_(), pValue_(&value_) {}                                                 \
                                                                                                                        \
    explicit EncodableDataType##Implementation(const SimpleDataType& value) : value_(value), pValue_(&value_) {}        \
                                                                                                                        \
    explicit EncodableDataType##Implementation(SimpleDataType* const pValue) : value_(), pValue_(pValue) {}             \
                                                                                                                        \
    EncodableDataType##Implementation(const EncodableDataType##Implementation& rhs) : value_(*rhs.pValue_), pValue_(&value_) {}\
                                                                                                                        \
    EncodableDataType##Implementation& operator=(const EncodableDataType##Implementation& rhs) {                        \
        if(this == &rhs) {return *this;}                                                                                \
        *pValue_ = rhs.get();                                                                                           \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    EncodableDataType##Implementation& operator=(const SimpleDataType& rhs) {                                           \
        *pValue_ = rhs;                                                                                                 \
        return *this;                                                                                                   \
    }                                                                                                                   \
                                                                                                                        \
    void setDataPointer(SimpleDataType* inData) throw (EncoderException) {                                              \
        if(inData == NULL) {                                                                                            \
            throw EncoderException(L"Null");                                                                            \
        }                                                                                                               \
        pValue_ = inData;                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    void set(SimpleDataType inData) {                                                                                   \
        *pValue_ = inData;                                                                                              \
    }                                                                                                                   \
                                                                                                                        \
    SimpleDataType get() const {                                                                                        \
        return *pValue_;                                                                                                \
    }                                                                                                                   \
                                                                                                                        \
    void decode(VariableLengthData const & inData) throw (EncoderException);                                            \
                                                                                                                        \
    void encode(VariableLengthData& inData) const throw (EncoderException);                                             \
                                                                                                                        \
    Integer64 hash() const;                                                                                             \
                                                                                                                        \
    VariableLengthData encode() const throw (EncoderException) {                                                        \
        VariableLengthData data;                                                                                        \
        this->encode(data);                                                                                             \
        return data;                                                                                                    \
    }                                                                                                                   \
                                                                                                                        \
    unsigned int getOctetBoundary() const {                                                                             \
        /* TODO: This is wrong. We must know the bit size and align to nearest memory alignment*/                       \
        return Length;                                                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    size_t getEncodedLength() const throw (EncoderException) {                                                          \
        return Length;                                                                                                  \
    }                                                                                                                   \
                                                                                                                        \
    void encodeInto(std::vector<Octet>& buffer) const throw (EncoderException) {                                        \
        const VariableLengthData encodedData(encode());                                                                 \
        for(int i = 0; i < encodedData.size(); ++i) {                                                                   \
            buffer.push_back(static_cast<const char*>(encodedData.data())[i]);                                          \
        }                                                                                                               \
    }                                                                                                                   \
                                                                                                                        \
    size_t decodeFrom(std::vector<Octet> const & inData, size_t index) throw (EncoderException) {                       \
        if(inData.size() < index + getEncodedLength()) {                                                                \
            throw EncoderException( L"Cannot decode from buffer. Remaining buffer is too small to contain our data." ); \
        }                                                                                                               \
        VariableLengthData encodedData(&inData.data()[index], getEncodedLength());                                      \
        decode(encodedData);                                                                                            \
        return index + getEncodedLength();                                                                              \
    }                                                                                                                   \
                                                                                                                        \
};

#define IMPLEMENT_ENCODING_HELPER_CLASS(EncodableDataType, SimpleDataType)                                              \
                                                                                                                        \
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
}

namespace rti1516e {
    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAASCIIchar, char, 1)

    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAboolean, bool, 4)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAbyte, Octet, 1)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat32BE, float, 4)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat32LE, float, 4)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat64BE, double, 8)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAfloat64LE, double, 8)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger16LE, Integer16, 2)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger16BE, Integer16, 2)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger32BE, Integer32, 4)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger32LE, Integer32, 4)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger64BE, Integer64, 8)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAinteger64LE, Integer64, 8)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAoctet, Octet, 1)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAoctetPairBE, OctetPair, 2)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAoctetPairLE, OctetPair, 2)

    IMPLEMENT_ENCODING_HELPER_CLASS_IMPL_FIXED_LENGTH(HLAunicodeChar, wchar_t, 2)

    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeString, std::wstring )

    /*************************************************************************
     *  HLAASCIIchar
     ************************************************************************/

    Integer64 HLAASCIIcharImplementation::hash() const {
        return this->get();
    }

    void HLAASCIIcharImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        char byteBuffer[1];
        byteBuffer[0] = this->get();
        inData.setData(byteBuffer, 1);
    }

    void HLAASCIIcharImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        this->set(*static_cast<const char*>(inData.data()));
    }

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

    void HLAbooleanImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        int decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        if (decodedData != 0 && decodedData != 1) {
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

    void HLAbyteImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
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
        float encodedValue = 0;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAfloat32BEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        float decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAfloat32LE
     ************************************************************************/

    Integer64 HLAfloat32LEImplementation::hash() const {
        Integer64 returnHash(0);
        const float inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAfloat32LEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const float value = this->get();
        float encodedValue = 0;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAfloat32LEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        float decodedData = 0;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAfloat64BE
     ************************************************************************/

    Integer64 HLAfloat64BEImplementation::hash() const {
        Integer64 returnHash(0);
        const double inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAfloat64BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const double value = this->get();
        double encodedValue = 0;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAfloat64BEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        double decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAfloat64LE
     ************************************************************************/

    Integer64 HLAfloat64LEImplementation::hash() const {
        Integer64 returnHash(0);
        const double inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAfloat64LEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const double value = this->get();
        double encodedValue = 0;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAfloat64LEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        double decodedData = 0;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger16LE
     ************************************************************************/

    Integer64 HLAinteger16LEImplementation::hash() const {
        return get();
    }

    void HLAinteger16LEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer16 value = get();
        Integer16 encodedValue = 0;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger16LEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer16 decodedData = 0;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger16BE
     ************************************************************************/

    Integer64 HLAinteger16BEImplementation::hash() const {
        return get();
    }

    void HLAinteger16BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer16 value = get();
        Integer16 encodedValue = 0;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger16BEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer16 decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger32LE
     ************************************************************************/

    Integer64 HLAinteger32LEImplementation::hash() const {
        return get();
    }

    void HLAinteger32LEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer32 value = get();
        Integer32 encodedValue = 0;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger32LEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer32 decodedData = 0;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger32BE
     ************************************************************************/

    Integer64 HLAinteger32BEImplementation::hash() const {
        return get();
    }

    void HLAinteger32BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer32 value = get();
        Integer32 encodedValue = 0;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger32BEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer32 decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger64LE
     ************************************************************************/

    Integer64 HLAinteger64LEImplementation::hash() const {
        return get();
    }

    void HLAinteger64LEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer64 value = get();
        Integer64 encodedValue = 0;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger64LEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer64 decodedData = 0;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAinteger64BE
     ************************************************************************/

    Integer64 HLAinteger64BEImplementation::hash() const {
        return get();
    }

    void HLAinteger64BEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const Integer64 value = get();
        Integer64 encodedValue = 0;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAinteger64BEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        Integer64 decodedData = 0;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAoctet
     ************************************************************************/

    Integer64 HLAoctetImplementation::hash() const {
        return get();
    }

    void HLAoctetImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        char byteBuffer[1];
        byteBuffer[0] = get();
        inData.setData(byteBuffer, 1);
    }

    void HLAoctetImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        this->set(*static_cast<const char*>(inData.data()));
    }

    /*************************************************************************
     *  HLAoctetPairBE
     ************************************************************************/

    Integer64 HLAoctetPairBEImplementation::hash() const {
        Integer64 returnHash(0);
        const OctetPair inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAoctetPairBEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const OctetPair value = get();
        OctetPair encodedValue;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAoctetPairBEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        OctetPair decodedData;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAoctetPairLE
     ************************************************************************/

    Integer64 HLAoctetPairLEImplementation::hash() const {
        Integer64 returnHash(0);
        const OctetPair inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    void HLAoctetPairLEImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const OctetPair value = get();
        OctetPair encodedValue;
        tt::memcpyle(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAoctetPairLEImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        OctetPair decodedData;
        tt::memcpyle(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    /*************************************************************************
     *  HLAunicodeChar
     ************************************************************************/

    Integer64 HLAunicodeCharImplementation::hash() const {
        return get();
    }

    void HLAunicodeCharImplementation::encode(VariableLengthData& inData) const throw (EncoderException) {
        const wchar_t value = get();
        wchar_t encodedValue;
        tt::memcpybe(reinterpret_cast<char*>(&encodedValue), reinterpret_cast<const char*>(&value), getEncodedLength());
        inData.setData(&encodedValue, getEncodedLength());
    }

    void HLAunicodeCharImplementation::decode(VariableLengthData const& inData) throw (EncoderException) {
        if (inData.size() < getEncodedLength()) {
            throw EncoderException(L"Cannot decode from buffer. Its too small to contain our data.");
        }
        wchar_t decodedData;
        tt::memcpybe(reinterpret_cast<char*>(&decodedData), static_cast<const char*>(inData.data()), inData.size());
        this->set(decodedData);
    }

    IMPLEMENT_ENCODING_HELPER_CLASS(HLAASCIIchar, char)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAboolean, bool)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAbyte, Octet)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat32BE, float)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat32LE, float)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat64BE, double)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAfloat64LE, double)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger16LE, Integer16)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger16BE, Integer16)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger32BE, Integer32)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger32LE, Integer32)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger64BE, Integer64)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAinteger64LE, Integer64)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctet, Octet)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctetPairBE, OctetPair)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAoctetPairLE, OctetPair)
    IMPLEMENT_ENCODING_HELPER_CLASS(HLAunicodeChar, wchar_t)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeString, std::wstring )
}
