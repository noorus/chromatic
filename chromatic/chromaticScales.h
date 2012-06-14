#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "chromaticTypes.h"
#include "chromaticChords.h"

namespace chromatic {

  using std::wstring;
  using std::vector;

  typedef Semitones DiatonicScaleDef [7];
  typedef ChordType DiatonicScaleChordDef [7];

  const DiatonicScaleDef g_diatonicScaleDefMajor = {
    2, 2, 1, 2, 2, 2, 1
  };

  const DiatonicScaleChordDef g_diatonicScaleChordsMajor = {
    ChordType_Major,
    ChordType_Minor,
    ChordType_Minor,
    ChordType_Major,
    ChordType_Major,
    ChordType_Minor,
    ChordType_Diminished
  };

  const wchar_t* g_DiatonicScaleDegreesMajor[7] = {
    L"I", L"ii", L"iii", L"IV", L"V", L"vi", L"vii"
  };

  const DiatonicScaleDef g_diatonicScaleDefMinor = {
    2, 1, 2, 2, 1, 2, 2
  };

  const DiatonicScaleChordDef g_diatonicScaleChordsMinor = {
    ChordType_Minor,
    ChordType_Diminished,
    ChordType_Major,
    ChordType_Minor,
    ChordType_Minor,
    ChordType_Major,
    ChordType_Major
  };

  const wchar_t* g_DiatonicScaleDegreesMinor[7] = {
    L"i", L"ii", L"III", L"iv", L"v", L"VI", L"VII"
  };

  enum DiatonicScaleMode: int {
    ScaleMode_Minor = 0,
    ScaleMode_Major
  };

  const wchar_t* g_scaleModesStr[2] = {
    L"Minor", L"Major"
  };

  class Scale {
  protected:
    wstring _name;
    wstring _str;
  public:
    virtual const wstring& getName() = 0;
    virtual cosnt wstring& getString() = 0;
  };

  class DiatonicScale: public Scale {
  protected:
    DiatonicScaleMode mode;
  public:
    Note notes[7];
    DiatonicScale( Note root, DiatonicScaleMode _mode ): mode( _mode )
    {
      switch ( mode ) {
        case ScaleMode_Major:
          notes[0] = root;
          for ( int i = 1; i < 7; i++ )
            notes[i] = notes[i-1] + g_diatonicScaleDefMajor[i-1];
        break;
        case ScaleMode_Minor:
          notes[0] = root;
          for ( int i = 1; i < 7; i++ )
            notes[i] = notes[i-1] + g_diatonicScaleDefMinor[i-1];
        break;
      }
    }
    void print()
    {
      wprintf_s( L"Scale %s:\r\n", getName().c_str() );
      wprintf_s( L"- %s\r\n", getString().c_str() );
      wprintf_s( L"Chords in %s:\r\n", getName().c_str() );
      for ( Degree i = Degree_Tonic; i <= Degree_Subsemitone; i++ )
        wprintf_s( L"- %s\r\n", getTriad( i ).getName().c_str() );
    }
    const wchar_t* getDegree( Degree degree )
    {
      switch ( mode )
      {
        case ScaleMode_Major:
        default:
          return g_DiatonicScaleDegreesMajor[degree];
        break;
        case ScaleMode_Minor:
          return g_DiatonicScaleDegreesMinor[degree];
        break;
      }
    }
    Triad getTriad( Degree degree )
    {
      switch ( mode )
      {
        case ScaleMode_Major:
        default:
          return Triad( notes[degree], g_diatonicScaleChordsMajor[degree] );
        break;
        case ScaleMode_Minor:
          return Triad( notes[degree], g_diatonicScaleChordsMinor[degree] );
        break;
      }
    }
    const wstring& getName()
    {
      if ( _name.empty() )
      {
        _name.resize( 16 );
        int chars = swprintf_s( &_name[0], 16, L"%s %s", g_notesSharpStr[notes[0]], g_scaleModesStr[mode] );
        if ( chars < 16 )
          _name.reserve( chars );
      }
      return _name;
    }
    const wstring& getString()
    {
      if ( _str.empty() )
      {
        _str.resize( 32 );
        int chars = swprintf_s( &_str[0], 32, L"%s-%s-%s-%s-%s-%s-%s",
          g_notesSharpStr[notes[0]],
          g_notesSharpStr[notes[1]],
          g_notesSharpStr[notes[2]],
          g_notesSharpStr[notes[3]],
          g_notesSharpStr[notes[4]],
          g_notesSharpStr[notes[5]],
          g_notesSharpStr[notes[6]]
        );
        if ( chars < 32 )
          _str.reserve( chars );
      }
      return _str;
    }
  };

  struct ChordProgressionStep {
    wstring str;
    Triad chord;
    ChordProgressionStep( const wstring& _str, Triad _chord ):
    str( _str ), chord( _chord ) {}
  };

  typedef vector<ChordProgressionStep> ProgressionVector;

  typedef vector<Triad> TriadVector;
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