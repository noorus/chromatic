#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "chromaticTypes.h"
#include "chromaticChords.h"
#include "chromaticScales.h"

namespace chromatic {

  using std::wstring;
  using std::vector;

  struct ChordProgressionStep {
    wstring str;
    Triad chord;
    ChordProgressionStep( const wstring& _str, Triad _chord ):
    str( _str ), chord( _chord ) {}
  };

  typedef vector<ChordProgressionStep> ProgressionVector;

  typedef vector<wstring> StringVector;
  typedef boost::char_separator<wchar_t,wstring::traits_type> CharSeparator;
  typedef boost::tokenizer<CharSeparator,wstring::const_iterator,wstring> CharTokenizer;

  void explode( const wstring& str, wstring delims, StringVector& v )
  {
    CharSeparator sep( delims.c_str() );
    CharTokenizer tokenizer( str, sep );
    for ( CharTokenizer::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it )
    {
      v.push_back( *it );
    }
  }

  wstring implode( const ProgressionVector& v, wstring delim )
  {
    std::wostringstream str;
    for ( ProgressionVector::const_iterator it = v.begin(); it != v.end(); ++it )
    {
      if ( it != v.begin() )
        str << delim;
      str << (*it).str;
    }
    return str.str();
  }

  class ChordProgression {
  protected:
    ProgressionVector progression;
    DiatonicScale scale;
  public:
    void print()
    {
      wstring prog = implode( progression, L"-" );
      wprintf_s( L"Chord progression %s in %s:\r\n", prog.c_str(), scale.getName().c_str() );
      for ( ProgressionVector::iterator it = progression.begin(); it != progression.end(); ++it )
      {
        wprintf_s( L"- %s\r\n", (*it).chord.getName().c_str() );
        wprintf_s( L"  %s\r\n", (*it).chord.getString().c_str() );
      }
    }
    ChordProgression( DiatonicScale _scale, wstring _progression ):
    scale( _scale )
    {
      StringVector chordtypes;
      explode( _progression, L"-", chordtypes );
      for ( StringVector::iterator it = chordtypes.begin(); it != chordtypes.end(); ++it )
      {
        if ( boost::iequals( (*it), L"i" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Tonic ), scale.getTriad( Degree_Tonic ) ) );
        } else if ( boost::iequals( (*it), L"ii" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Supertonic ), scale.getTriad( Degree_Supertonic ) ) );
        } else if ( boost::iequals( (*it), L"iii" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Mediant ), scale.getTriad( Degree_Mediant ) ) );
        } else if ( boost::iequals( (*it), L"iv" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Subdominant ), scale.getTriad( Degree_Subdominant ) ) );
        } else if ( boost::iequals( (*it), L"v" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Dominant ), scale.getTriad( Degree_Dominant ) ) );
        } else if ( boost::iequals( (*it), L"vi" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Submediant ), scale.getTriad( Degree_Submediant ) ) );
        } else if ( boost::iequals( (*it), L"vii" ) ) {
          progression.push_back( ChordProgressionStep( scale.getDegree( Degree_Subsemitone ), scale.getTriad( Degree_Subsemitone ) ) );
        }
      }
    }
  };

}