AM_CXXFLAGS=-Wall -g -fPIC -std=gnu++11 -DVERSION=\"$(VERSION)\" -DPROG="\"$(PACKAGE)\""

bin_PROGRAMS = algatorc
algatorc_SOURCES = algatorc.cpp
include_HEADERS = Timer.hpp TestSetIterator.hpp TestCase.hpp ETestSet.hpp EParameter.hpp Entity.hpp ParameterSet.hpp AbsAlgorithm.hpp Log.hpp JSON.hpp JSONValue.hpp

lib_LIBRARIES = libAlgatorc.a
libAlgatorc_a_SOURCES = ParameterSet.cpp TestCase.cpp EParameter.cpp ETestSet.cpp TestSetIterator.cpp Entity.cpp Timer.cpp JSON.cpp JSONValue.cpp AbsAlgorithm.cpp
algatorc_LDADD=libAlgatorc.a
