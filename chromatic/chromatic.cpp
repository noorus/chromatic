#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include "chromaticInterval.h"
#include "chromaticNote.h"
#include "chromaticChords.h"

using namespace chromatic;

Note noteFromString( const wstring& str )
{
  for ( int i = 0; i < 12; i++ )
  {
    if ( !_wcsicmp( str.c_str(), g_notesSharpStr[i] ) || !_wcsicmp( str.c_str(), g_notesFlatStr[i] ) )
      return (Note)i;
  }
  return Note_C;
}

Triad chordFromString( wstring str )
{
  // rather naïve
  ChordType type = ChordType_Major;
  if ( str.length() > 1 )
  {
    wstring suf = str.substr( str.length()-1 );
    if ( str.length() > 4 ) {
      suf = str.substr( str.length()-4 );
      if ( boost::iequals( suf, L"sus4" ) ) {
        type = ChordType_SuspendedFourth;
        str.erase( str.length()-4, 4 );
      } else if ( boost::iequals( suf, L"sus2" ) ) {
        type = ChordType_SuspendedSecond;
        str.erase( str.length()-4, 4 );
      }
    } else if ( boost::iequals( suf, L"m" ) ) {
      type = ChordType_Minor;
      str.erase( str.length()-1, 1 );
    } else if ( boost::iequals( suf, L"a" ) ) {
      type = ChordType_Augmented;
      str.erase( str.length()-1, 1 );
    } else if ( boost::iequals( suf, L"o" ) ) {
      type = ChordType_Diminished;
      str.erase( str.length()-1, 1 );
    }
  }
  return Triad( noteFromString( str ), type );
}

DiatonicScale scaleFromString( wstring str )
{
  DiatonicScaleMode mode = ScaleMode_Major;
  if ( boost::iequals( str.substr( str.length()-1 ), L"m" ) ) {
    mode = ScaleMode_Minor;
    str.erase( str.length()-1, 1 );
  }
  Note note = noteFromString( str );
  return DiatonicScale( note, mode );
}

int wmain( int argc, wchar_t* argv[] )
{
  if ( argc < 2 ) {
    wprintf_s( L"Syntax: %s <action>\r\n", argv[0] );
    wprintf_s( L"Valid actions: chord, scale, progression\r\n" );
    return EXIT_FAILURE;
  }
  if ( !_wcsicmp( argv[1], L"chord" ) )
  {
    if ( argc < 3 ) {
      wprintf_s( L"Syntax: %s chord <name>\r\n", argv[0] );
      return EXIT_FAILURE;
    }
    Triad chord = chordFromString( argv[2] );
    chord.print();
    return EXIT_SUCCESS;
  }
  else if ( !_wcsicmp( argv[1], L"scale" ) )
  {
    if ( argc < 3 ) {
      wprintf_s( L"Syntax: %s scale <name>\r\n", argv[0] );
      return EXIT_FAILURE;
    }
    DiatonicScale scale = scaleFromString( argv[2] );
    scale.print();
  }
  else if ( !_wcsicmp( argv[1], L"progression" ) )
  {
    if ( argc < 4 ) {
      wprintf_s( L"Syntax: %s progression <progression> <scale>\r\n", argv[0] );
      return EXIT_FAILURE;
    }
    DiatonicScale scale = scaleFromString( argv[3] );
    wstring chords = argv[2];
    ChordProgression progression( scale, chords );
    progression.print();
    return EXIT_SUCCESS;
  } else {
    wprintf_s( L"Syntax: %s <action>\r\n", argv[0] );
    wprintf_s( L"Valid actions: chord, scale, progression\r\n" );
    return EXIT_FAILURE;
  }
}