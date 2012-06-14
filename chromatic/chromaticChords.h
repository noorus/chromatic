#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "chromaticTypes.h"

namespace chromatic {

  using std::wstring;
  using std::vector;

  enum ChordType: int {
    ChordType_Major,
    ChordType_Minor,
    ChordType_Augmented,
    ChordType_Diminished,
    ChordType_SuspendedFourth,
    ChordType_SuspendedSecond
  };

  const wchar_t* g_chordsFullStr[6] = {
    L"Major", L"Minor", L"Augmented", L"Diminished", L"Suspended Fourth", L"Suspended Second"
  };

  const wchar_t* g_chordsSuffixStr[6] = {
    L"", L"m", L"a", L"o", L"sus4", L"sus2"
  };

  struct Chord {
  protected:
    wstring _name;
    wstring _str;
  public:
    virtual const wstring& getName() = 0;
    virtual const wstring& getString() = 0;
  };

  struct Triad;
  typedef vector<Triad> TriadVector;

  struct Triad: public Chord {
  public:
    Note first;
    Note second;
    Note third;
    ChordType type;
    Triad( const Note& root, ChordType _type ): first( root ), type( _type )
    {
      switch ( type ) {
        case ChordType_Major:
          second = root + Interval_ThirdMajor;
          third = root + Interval_Fifth;
        break;
        case ChordType_Minor:
          second = root + Interval_ThirdMinor;
          third = root + Interval_Fifth;
        break;
        case ChordType_Augmented:
          second = root + Interval_ThirdMajor;
          third = root + Interval_Fifth + 1;
        break;
        case ChordType_Diminished:
          second = root + Interval_ThirdMinor;
          third = root + Interval_Fifth - 1;
        break;
        case ChordType_SuspendedFourth:
          second = root + Interval_Fourth;
          third = root + Interval_Fifth;
        break;
        case ChordType_SuspendedSecond:
          second = root + Interval_SecondMajor;
          third = root + Interval_Fifth;
        break;
      }
    }
    void print()
    {
      wprintf_s( L"Chord %s:\r\n", getName().c_str() );
      wprintf_s( L"- %s\r\n", getString().c_str() );
    }
    const wstring& getName()
    {
      if ( _name.empty() )
      {
        _name = g_notesSharpStr[first];
        _name.append( L" " );
        _name.append( g_chordsFullStr[type] );
      }
      return _name;
    }
    const wstring& getString()
    {
      if ( _str.empty() )
      {
        _str.resize( 16 );
        int chars = swprintf_s( &_str[0], 16, L"%s-%s-%s", g_notesSharpStr[first], g_notesSharpStr[second], g_notesSharpStr[third] );
        if ( chars < 16 )
          _str.reserve( chars );
      }
      return _str;
    }
    static Triad makeMajor( Note root ) {
      return Triad( root, ChordType_Major );
    }
    static Triad makeMinor( Note root ) {
      return Triad( root, ChordType_Minor );
    }
    static Triad makeAugmented( Note root ) {
      return Triad( root, ChordType_Augmented );
    }
    static Triad makeDiminished( Note root ) {
      return Triad( root, ChordType_Diminished );
    }
    static Triad makeSuspendedFourth( Note root ) {
      return Triad( root, ChordType_SuspendedFourth );
    }
    static Triad makeSuspendedSecond( Note root ) {
      return Triad( root, ChordType_SuspendedSecond );
    }
  };

}