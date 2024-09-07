#include "RTI/encoding/BasicDataElements.h"

#include "Endianness.h"
#include "RTI/VariableLengthData.h"

#define IMPLEMENT_ENCODING_HELPER_CLASS(EncodableDataType, SimpleDataType)\
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
\

namespace rti1516e {
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIchar, char )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_ENCODING_HELPER_CLASS( HLAboolean, bool )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAbyte, Octet )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32BE, float )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32LE, float )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat64BE, double )
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

    VariableLengthData HLAboolean::encode() const throw (EncoderException) {
        VariableLengthData data;
        this->encode(data);
        return data;
    }

    void HLAboolean::encode(VariableLengthData& inData) const throw (EncoderException) {
        char byteBuffer[getEncodedLength()];
        unsigned int value = this->get() ? 1 : 0;
        tt::memcpybe(byteBuffer, reinterpret_cast<char*>(&value), getEncodedLength());
        inData.setData(byteBuffer, getEncodedLength());
    }

    Integer64 HLAboolean::hash() const
    {
        return this->get() + 1;
    }

    unsigned int HLAboolean::getOctetBoundary () const {
        return 4;
    }

    size_t HLAboolean::getEncodedLength() const throw (EncoderException) {
        return 4;
    }

    void HLAboolean::encodeInto(std::vector<Octet>& buffer) const throw (EncoderException) {

    }

    void HLAboolean::decode(VariableLengthData const & inData) throw (EncoderException) {

    }

    size_t HLAboolean::decodeFrom(std::vector<Octet> const & buffer, size_t index) throw (EncoderException) {
        return 0;
    }

}