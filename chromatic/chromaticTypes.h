//
// Chromatic musical utility
// Copyright (c) 2012 noorus
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#pragma warning( disable: 4620 )

namespace chromatic {

  // Basic types

  typedef int Semitones;

  // Interval

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

  // Note

  enum Note: int {
    Note_C  = 0,
    Note_Cs = 1,
    Note_Df = 1,
    Note_D  = 2,
    Note_Ds = 3,
    Note_Ef = 3,
    Note_E  = 4,
    Note_F  = 5,
    Note_Fs = 6,
    Note_Gf = 6,
    Note_G  = 7,
    Note_Gs = 8,
    Note_Af = 8,
    Note_A  = 9,
    Note_As = 10,
    Note_Bf = 10,
    Note_B  = 11
  };

  const wchar_t* g_notesSharpStr[12] = {
    L"C", L"C#", L"D", L"D#", L"E", L"F", L"F#", L"G", L"G#", L"A", L"A#", L"B"
  };

  const wchar_t* g_notesFlatStr[12] = {
    L"C", L"Db", L"D", L"Eb", L"E", L"F", L"Gb", L"G", L"Ab", L"A", L"Bb", L"B"
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

  inline Note& operator ++( Note& n )
  {
    int t = (int)n;
    t++;
    while ( t > Note_B )
      t -= Interval_Octave;
    n = (Note)t;
    return n;
  }

  inline Note& operator --( Note& n )
  {
    int t = (int)n;
    t--;
    while ( t < Note_C )
      t += Interval_Octave;
    n = (Note)t;
    return n;
  }

  // Degree

  enum Degree: int {
    Degree_Tonic = 0,
    Degree_Supertonic,
    Degree_Mediant,
    Degree_Subdominant,
    Degree_Dominant,
    Degree_Submediant,
    Degree_Subsemitone
  };

  inline Degree& operator++( Degree& d )
  {
    d = (Degree)( (int)d + 1 );
    return d;
  }

  inline Degree& operator--( Degree& d )
  {
    d = (Degree)( (int)d - 1 );
    return d;
  }

}