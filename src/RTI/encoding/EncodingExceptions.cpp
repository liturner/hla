#include "RTI/encoding/EncodingExceptions.h"

namespace rti1516e
{

    EncoderException::EncoderException(std::wstring const & message) throw() : _msg(message) {
    }

    std::wstring EncoderException::what() const throw() {
        return _msg;
    }

}