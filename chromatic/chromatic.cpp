#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

namespace chromatic {

  using std::wstring;
  using std::vector;

  enum Note: int {
    Note_C = 0,
    Note_Cs = 1,
    Note_Df = 1,
    Note_D = 2,
    Note_Ds = 3,
    Note_Ef = 3,
    Note_E = 4,
    Note_F = 5,
    Note_Fs = 6,
    Note_Gf = 6,
    Note_G = 7,
    Note_Gs = 8,
    Note_Af = 8,
    Note_A = 9,
    Note_As = 10,
    Note_Bf = 10,
    Note_B = 11
  };

  const wchar_t* g_notesSharpStr[12] = {
    L"C", L"C#", L"D", L"D#", L"E", L"F", L"F#", L"G", L"G#", L"A", L"A#", L"B"
  };

  const wchar_t* g_notesFlatStr[12] = {
    L"C", L"Db", L"D", L"Eb", L"E", L"F", L"Gb", L"G", L"Ab", L"A", L"Bb", L"B"
  };

  typedef int Semitones;

  enum Interval: Semitones {
    Interval_First = 0,
    Interval_SecondMinor,
    Interval_SecondMajor,
    Interval_ThirdMinor,
    Interval_ThirdMajor,
    Interval_Fourth,
    Interval_FourthMajor,
    Interval_Fifth,
    Interval_SixthMinor,
    Interval_SixthMajor,
    Interval_SeventhMinor,
    Interval_SeventhMajor,
    Interval_Octave
  };

  enum Degree: int {
    Degree_Tonic = 0,
    Degree_Supertonic,
    Degree_Mediant,
    Degree_Subdominant,
    Degree_Dominant,
    Degree_Submediant,
    Degree_Subsemitone
  };

  inline Note operator +( const Note& n, Semitones i )
  {
    int t = (int)n + i;
    while ( t > Note_B )
      t -= Interval_Octave;
    return (Note)t;
  }

  inline Note operator -( const Note& n, Semitones i )
  {
    int t = (int)n - i;
    while ( t < Note_C )
      t += Interval_Octave;
    return (Note)t;
  }

  inline Note& operator +=( Note& n, Semitones i )
  {
    int t = (int)n + i;
    while ( t > Note_B )
      t -= Interval_Octave;
    n = (Note)t;
    return n;
  }

  inline Note& operator -=( Note& n, Semitones i )
  {
    int t = (int)n - i;
    while ( t < Note_C )
      t += Interval_Octave;
    n = (Note)t;
    return n;
  }

  inline Degree& operator++( Degree& d, int unused )
  {
    d = (Degree)( (int)d + 1 );
    return d;
  }

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

  struct Triad {
  protected:
    wstring _name;
    wstring _str;
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
  public:
    virtual const wstring& getName() = 0;
  };

  class DiatonicScale: public Scale {
  protected:
    DiatonicScaleMode mode;
    wstring _str;
  public:
    Note notes[7];
    DiatonicScale( Note root, DiatonicScaleMode _mode ): mode( _mode )
    {
      switch ( mode ) {
        case ScaleMode_Major:
          notes[0] = root;
          for ( int i = 1; i < 7; i++ ) {
            notes[i] = notes[i-1] + g_diatonicScaleDefMajor[i-1];
          }
        break;
        case ScaleMode_Minor:
          notes[0] = root;
          for ( int i = 1; i < 7; i++ ) {
            notes[i] = notes[i-1] + g_diatonicScaleDefMinor[i-1];
          }
        break;
      }
    }
    void print()
    {
      wprintf_s( L"Scale %s:\r\n", getName().c_str() );
      wprintf_s( L"- %s\r\n", getString().c_str() );
      wprintf_s( L"Chords in %s:\r\n", getName().c_str() );
      for ( Degree i = Degree_Tonic; i <= Degree_Subsemitone; i++ ) {
        wprintf_s( L"- %s\r\n", getTriad( i ).getName().c_str() );
      }
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