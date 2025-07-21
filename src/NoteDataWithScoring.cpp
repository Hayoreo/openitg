#include "global.h"
#include "NoteDataWithScoring.h"
#include "NoteData.h"
#include "StatsManager.h"
#include <float.h>

namespace
{

int GetNumTapNotesWithScore( const NoteData &in, TapNoteScore tns, int iStartIndex = 0, int iEndIndex = MAX_NOTE_ROW )
{ 
	int iNumSuccessfulTapNotes = 0;
	for( int t=0; t<in.GetNumTracks(); t++ )
	{
		FOREACH_NONEMPTY_ROW_IN_TRACK_RANGE( in, t, r, iStartIndex, iEndIndex )
		{
			const TapNote &tn = in.GetTapNote(t, r);
			if( tn.result.tns >= tns )
				iNumSuccessfulTapNotes++;
		}
	}
	
	return iNumSuccessfulTapNotes;
}

int GetNumNWithScore( const NoteData &in, TapNoteScore tns, int MinTaps, int iStartRow = 0, int iEndRow = MAX_NOTE_ROW )
{
	int iNumSuccessfulDoubles = 0;
	FOREACH_NONEMPTY_ROW_ALL_TRACKS_RANGE( in, r, iStartRow, iEndRow )
	{
		int iNumNotesInRow = in.GetNumTracksWithTapOrHoldHead( r );
		TapNoteScore tnsRow = NoteDataWithScoring::LastTapNoteResult( in, r ).tns;

		if( iNumNotesInRow >= MinTaps && tnsRow >= tns )
			iNumSuccessfulDoubles++;
	}
	
	return iNumSuccessfulDoubles;
}

int GetNumHoldNotesWithScore( const NoteData &in, TapNote::SubType subType, HoldNoteScore hns )
{
	ASSERT( subType != TapNote::SubType_invalid );

	int iNumSuccessfulHolds = 0;
	for( int t=0; t<in.GetNumTracks(); ++t )
	{
		NoteData::TrackMap::const_iterator begin, end;
		in.GetTapNoteRange( t, 0, MAX_NOTE_ROW, begin, end );

		for( ; begin != end; ++begin )
		{
			const TapNote &tn = begin->second;
			if( tn.type != TapNote::hold_head )
				continue;
			if( tn.subType != subType )
				continue;
			if( tn.HoldResult.hns == hns )
				++iNumSuccessfulHolds;
		}
	}

	return iNumSuccessfulHolds;
}

int GetSuccessfulMines( const NoteData &in, int iStartIndex = 0, int iEndIndex = MAX_NOTE_ROW )
{
	int iNumSuccessfulMinesNotes = 0;
	FOREACH_NONEMPTY_ROW_ALL_TRACKS_RANGE( in, i, iStartIndex, iEndIndex )
	{
		for( int t=0; t<in.GetNumTracks(); t++ )
		{
			const TapNote &tn = in.GetTapNote(t,i);
			if( tn.type == TapNote::mine  &&  tn.result.tns == TNS_AVOIDED_MINE )
				iNumSuccessfulMinesNotes++;
		}
	}
	
	return iNumSuccessfulMinesNotes;
}

/* See NoteData::GetNumHands(). */
int GetSuccessfulHands( const NoteData &in, int iStartIndex = 0, int iEndIndex = MAX_NOTE_ROW )
{
	int iNum = 0;
	FOREACH_NONEMPTY_ROW_ALL_TRACKS_RANGE( in, i, iStartIndex, iEndIndex )
	{
		if( !in.RowNeedsHands(i) )
			continue;

		bool Missed = false;
		for( int t=0; t<in.GetNumTracks(); t++ )
		{
			const TapNote &tn = in.GetTapNote(t, i);
			if( tn.type == TapNote::empty )
				continue;
			if( tn.type == TapNote::mine ) // mines don't count
				continue;
			if( tn.result.tns <= TNS_BOO )
				Missed = true;
		}

		if( Missed )
			continue;

		/* Check hold scores. */
		for( int t=0; t<in.GetNumTracks(); ++t )
		{
			int iHeadRow;
			if( !in.IsHoldNoteAtBeat( t, i, &iHeadRow ) )
				continue;
			const TapNote &tn = in.GetTapNote( t, iHeadRow );

			/* If a hold is released *after* a hands containing it, the hands is
			 * still good.  So, ignore the judgement and only examine iLastHeldRow
			 * to be sure that the hold was still held at the point of this row.
			 * (Note that if the hold head tap was missed, then iLastHeldRow == i
			 * and this won't fail--but the tap check above will have already failed.) */
			if( tn.HoldResult.iLastHeldRow < i )
				Missed = true;
		}

		if( !Missed )
			iNum++;
	}

	return iNum;
}

/* Return the last tap score of a row: the grade of the tap that completed
 * the row.  If the row has no tap notes, return -1.  If any tap notes aren't
 * graded (any tap is TNS_NONE) or are missed (TNS_MISS), return it. */
int LastTapNoteScoreTrack( const NoteData &in, unsigned iRow )
{
	float scoretime = -9999;
	int best_track = -1;
	for( int t=0; t<in.GetNumTracks(); t++ )
	{
		/* Skip empty tracks and mines */
		const TapNote &tn = in.GetTapNote( t, iRow );
		if( tn.type == TapNote::empty || tn.type == TapNote::mine ) 
			continue;

		TapNoteScore tns = tn.result.tns;
		
		if( tns == TNS_MISS || tns == TNS_NONE )
			return t;

		float tm = tn.result.fTapNoteOffset;
		if(tm < scoretime) continue;
		
		scoretime = tm;
		best_track = t;
	}

	return best_track;
}

}

TapNoteResult NoteDataWithScoring::LastTapNoteResult( const NoteData &in, unsigned iRow )
{
	int iTrack = LastTapNoteScoreTrack( in, iRow );
	if( iTrack == -1 )
		return TapNoteResult();
	return in.GetTapNote(iTrack, iRow).result;
}


/* Return the minimum tap score of a row.  If the row isn't complete (not all
 * taps have been hit), return TNS_NONE or TNS_MISS. */
TapNoteScore NoteDataWithScoring::MinTapNoteScore( const NoteData &in, unsigned row )
{
	TapNoteScore score = TNS_MARVELOUS;
	for( int t=0; t<in.GetNumTracks(); t++ )
	{
		/* Ignore mines, or the score will always be TNS_NONE. */
		const TapNote &tn = in.GetTapNote(t, row);
		if( tn.type == TapNote::empty || tn.type == TapNote::mine )
			continue;
		score = min( score, tn.result.tns );
	}

	return score;
}

bool NoteDataWithScoring::IsRowCompletelyJudged( const NoteData &in, unsigned row )
{
	return MinTapNoteScore( in, row ) >= TNS_MISS;
}

void NoteDataWithScoring::GetActualRadarValues( const NoteData &in, PlayerNumber pn, float fSongSeconds, RadarValues& out )
{
	// The for loop and the assert are used to ensure that all fields of 
	// RadarValue get set in here.
	FOREACH_RadarCategory( rc )
	{
		switch( rc )
		{
		case RADAR_NUM_TAPS_AND_HOLDS:	out[rc] = (float) GetNumNWithScore( in, TNS_GOOD, 1 );			break;
		case RADAR_NUM_JUMPS:			out[rc] = (float) GetNumNWithScore( in, TNS_GOOD, 2 );			break;
		case RADAR_NUM_HOLDS:			out[rc] = (float) GetNumHoldNotesWithScore( in, TapNote::hold_head_hold, HNS_OK );	break;
		case RADAR_NUM_MINES:			out[rc] = (float) GetSuccessfulMines( in );						break;
		case RADAR_NUM_HANDS:			out[rc] = (float) GetSuccessfulHands( in );						break;
		case RADAR_NUM_ROLLS:			out[rc] = (float) GetNumHoldNotesWithScore( in, TapNote::hold_head_roll, HNS_OK ); break;
		default:	ASSERT(0);
		}
	}
}

/*
 * (c) 2001-2004 Chris Danford, Glenn Maynard
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
