#include "RTI/encoding/DataElement.h"

#include <typeinfo>

namespace rti1516e {
    DataElement::~DataElement() {
    }

    bool DataElement::isSameTypeAs(const DataElement& inData) const
    {
        return typeid(*this).operator==(typeid(inData));
    }

    Integer64 DataElement::hash() const {
        return 42;
    }

}
