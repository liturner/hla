#include "RTI/encoding/BasicDataElements.h"

#include "Endianness.h"
#include "RTI/VariableLengthData.h"

#define ALIGN_OF_CHAR 1;
#define ALIGN_OF_SHORT 2;
#define ALIGN_OF_INT 4;
#define ALIGN_OF_LONG 4;
#define ALIGN_OF_FLOAT 4;
#define ALIGN_OF_DOUBLE 8;

#define SIZE_OF_CHAR 1;
#define SIZE_OF_SHORT 2;
#define SIZE_OF_INT 4;
#define SIZE_OF_LONG 4;
#define SIZE_OF_FLOAT 4;
#define SIZE_OF_DOUBLE 8;

/// Fixed Length applies to the SimpleDataType. During encoding and decoding we just
/// pointer cast in here (which would fail hard for std::string e.g)
#define IMPLEMENT_ENCODING_HELPER_CLASS_FIXED_LENGTH(EncodableDataType, SimpleDataType)\
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
\
}; \
\
EncodableDataType::EncodableDataType() : _impl(new EncodableDataType##Implementation()) {} \
EncodableDataType::EncodableDataType(SimpleDataType const & inData) : _impl(new EncodableDataType##Implementation(inData)) {} \
EncodableDataType::EncodableDataType(SimpleDataType* inData) : _impl(new EncodableDataType##Implementation(inData)) {} \
EncodableDataType::EncodableDataType(EncodableDataType const & rhs) : _impl(new EncodableDataType##Implementation(*rhs._impl)) {} \
EncodableDataType::~EncodableDataType() {delete _impl;}\
EncodableDataType& EncodableDataType::operator=(EncodableDataType const & rhs) {\
    if(this == &rhs) {return *this;}\
    _impl->operator=(*rhs._impl);\
    return *this;\
}\
EncodableDataType& EncodableDataType::operator=(SimpleDataType rhs) {\
    _impl->operator=(rhs);\
    return *this;\
}\
std::auto_ptr<DataElement> EncodableDataType::clone() const {\
    return std::auto_ptr<DataElement>(new EncodableDataType(*this));\
}\
SimpleDataType EncodableDataType::get() const {return _impl->get();}\
void EncodableDataType::set(SimpleDataType inData) {_impl->set(inData);}\
void EncodableDataType::setDataPointer(SimpleDataType* inData) throw (EncoderException) {_impl->setDataPointer(inData);}\
EncodableDataType::operator SimpleDataType () const {return _impl->get();}\
VariableLengthData EncodableDataType::encode() const throw (EncoderException) {\
    VariableLengthData data;\
    this->encode(data);\
    return data;\
}\
void EncodableDataType::encodeInto(std::vector<Octet>& buffer) const throw (EncoderException) {\
    const VariableLengthData encodedData(encode());\
    for(int i = 0; i < encodedData.size(); ++i) {\
        buffer.push_back(static_cast<const char*>(encodedData.data())[i]);\
    }\
}\
size_t EncodableDataType::decodeFrom(std::vector<Octet> const & inData, size_t index) throw (EncoderException) {\
    if(inData.size() < index + getEncodedLength()) {\
        throw EncoderException( L"Cannot decode from buffer. Remaining buffer is too small to contain our data." );\
    }\
    VariableLengthData encodedData(&inData.data()[index], getEncodedLength());\
    decode(encodedData);\
    return index + getEncodedLength();\
}\
\

namespace rti1516e {
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIchar, char )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_ENCODING_HELPER_CLASS_FIXED_LENGTH( HLAboolean, bool )
    IMPLEMENT_ENCODING_HELPER_CLASS_FIXED_LENGTH( HLAbyte, Octet )
    IMPLEMENT_ENCODING_HELPER_CLASS_FIXED_LENGTH( HLAfloat32BE, float )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32LE, float )
    IMPLEMENT_ENCODING_HELPER_CLASS_FIXED_LENGTH( HLAfloat64BE, double )
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

    Integer64 HLAboolean::hash() const
    {
        return this->get() + 1;
    }

    unsigned int HLAboolean::getOctetBoundary() const {
        return ALIGN_OF_INT;
    }

    size_t HLAboolean::getEncodedLength() const throw (EncoderException) {
        return SIZE_OF_INT;
    }

    void HLAboolean::encode(VariableLengthData& inData) const throw (EncoderException) {
        char byteBuffer[getEncodedLength()];
        int value = this->get() ? 1 : 0;
        tt::memcpybe(byteBuffer, reinterpret_cast<char*>(&value), getEncodedLength());
        inData.setData(byteBuffer, getEncodedLength());
    }

    void HLAboolean::decode(VariableLengthData const & inData) throw (EncoderException) {
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

    Integer64 HLAbyte::hash() const
    {
        return this->get() + 1;
    }

    unsigned int HLAbyte::getOctetBoundary() const {
        return ALIGN_OF_CHAR;
    }

    size_t HLAbyte::getEncodedLength() const throw (EncoderException) {
        return SIZE_OF_CHAR;
    }

    void HLAbyte::encode(VariableLengthData& inData) const throw (EncoderException) {
        const char value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAbyte::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const char*>(inData.data()));
    }

    /*************************************************************************
     *  HLAfloat32BE
     ************************************************************************/

    Integer64 HLAfloat32BE::hash() const
    {
        Integer64 returnHash(0);
        const float inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    unsigned int HLAfloat32BE::getOctetBoundary() const {
        return ALIGN_OF_FLOAT;
    }

    size_t HLAfloat32BE::getEncodedLength() const throw (EncoderException) {
        return SIZE_OF_FLOAT;
    }

    void HLAfloat32BE::encode(VariableLengthData& inData) const throw (EncoderException) {
        const float value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAfloat32BE::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const float*>(inData.data()));
    }

    /*************************************************************************
     *  HLAfloat64BE
     ************************************************************************/

    Integer64 HLAfloat64BE::hash() const
    {
        Integer64 returnHash(0);
        const double inData(get());
        memcpy(&returnHash, &inData, getEncodedLength());
        return returnHash;
    }

    unsigned int HLAfloat64BE::getOctetBoundary() const {
        return ALIGN_OF_DOUBLE;
    }

    size_t HLAfloat64BE::getEncodedLength() const throw (EncoderException) {
        return SIZE_OF_DOUBLE;
    }

    void HLAfloat64BE::encode(VariableLengthData& inData) const throw (EncoderException) {
        const double value = this->get();
        inData.setData(&value, getEncodedLength());
    }

    void HLAfloat64BE::decode(VariableLengthData const & inData) throw (EncoderException) {
        if(inData.size() < getEncodedLength()) {
            throw EncoderException( L"Cannot decode from buffer. Its too small to contain our data." );
        }
        this->set(*static_cast<const double*>(inData.data()));
    }

}