#pragma once

class RTI_EXPORT HLAtoken : public rti1516e::DataElement {

    rti1516e::HLAfixedArray encoding_;
    rti1516e::HLAbyte prototype_;

public:
    HLAToken() {

    }
};