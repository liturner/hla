# HLA

This repository is unstable and should not be used by other projects at this time.

The aim of this repository is for personal development and learning of the HLA standards
produced by SISO and IEEE. It is not for production, is sloppy, has bugs and is not usable.

This library includes files provided by IEEE under the permissions listed in the various
source files. Note that these file may have been modified in this repository!

## Notes

C++ "compliance" goes as far as exact naming and capitalisation. This can be seen throughout
the C++ API provided for HLA. At the simplest level we see "Octet" being a typedef. Other HLA
types like "HLAfixedRecord" are also exact, with the possible exception that they are
in a namespace "rti1516e". Interestingly the namespace is not mentionined in the HLA FOM (MIM)

The rti1516e:DataElement class looks interesting for our use case. It is an interface specifying
consistent encoding and decoding of and data.

"HLAobjectRoot" is of type "HLAtoken" and exist only in the MIM. However, "HLAtoken" is
a HLAfixedArray of HLAbyte, both of which are exactly matched in the API. Note here also 
that rti1516e:HLAfixedArray uses a pimpl pattern and also specifies
rti1516e:HLAfixedRecordImplementation.

At initial investigation, all HLA FOM Objects can be fully defined down to basic types, and a few
basic concepts like fixed or variable arrays. I believe it should be possible to generate a fully
compliant set of C++ classes from a HLA FOM with little effort.

The C++ API is something like C++98, throws lots of deprecation with anything higher than 98, and
cannot compile under C++17. Initial investigation shows trivial changes needed (like remove "throws"
or change some #MACROs to constexpr etc.), though these should not be undertaken until discussions 
with IEEE, or unless we are ok to commit to maintaining "HLA 17" for example. Portico and other HLA
implementations seem to have each made minor modifications to the IEEE provided API. Maybe a HLA 17
would be a good idea for an actual project?

There are a few big chunks to consider:
1. The whole federate thing etc. is the runtime connection and interaction
2. There is a lot of boiler plate for encoding and decoding of the basic types
3. The types defined in the FOMs need to be defined or generated.

## Case Study HLAobjectRoot

rti1516e::HLAfixedArray -> 
    rti1516e::DataElement
        rti1516e::VariableLengthData
    rti1516e::EncoderException

## Standards

### NATO

- STANAG 4603
- https://nmsg.sto.nato.int/amsp/hla  ! Great links in the Master Plan!

### IEEE

#### IEEE Std 1516 IEEE Standard for Modeling and Simulation (M&S) High Level Architecture (HLA)
IEEE 1516-2010 Framework and Rules
IEEE 1516.1-2010 Federate Interface Specification
IEEE 1516.2-2010 Object Model Template Specification
IEEE 1516.4-2007 IEEE Recommended Practice for Verification, Validation, and Accreditation of a Federation - an Overlay to the High Level Architecture Federation Development and Execution Process

https://standards.ieee.org/downloads/ DOWNLOAD APIs!!!
