/* XXX: get rid of newly-redundant Menu button declarations ASAP! */

#include "global.h"
#include "GameManager.h"
#include "RageLog.h"
#include "NoteSkinManager.h"
#include "RageInputDevice.h"
#include "ThemeManager.h"
#include "LightsManager.h"	// for NUM_CABINET_LIGHTS
#include "Game.h"
#include "Style.h"

GameManager*	GAMEMAN = NULL;	// global and accessable from anywhere in our program

enum 
{
	TRACK_1 = 0,
	TRACK_2,
	TRACK_3,
	TRACK_4,
	TRACK_5,
	TRACK_6,
	TRACK_7,
	TRACK_8,
};

enum
{
	GAME_DANCE,		// Dance Dance Revolution
	GAME_PUMP,		// Pump It Up
	GAME_EZ2,		// Ez2dancer
	GAME_PARA,		// ParaPAraParadise
	GAME_DS3DDX,		// Dance Station 3DDX.
	GAME_BM,		// Beatmania IIDX
	GAME_MANIAX,		// DanceManiax
	GAME_TECHNO,		// TechnoMotion
	GAME_PNM,		// pop n music
	GAME_LIGHTS,		// cabinet lights (not really a game)
	NUM_GAMES,		// leave this at the end
	GAME_INVALID,
};

const int DANCE_COL_SPACING = 64;

struct {
	const char *name;
	int NumTracks;
} const StepsTypes[NUM_STEPS_TYPES] = {
	{ "dance-single",	4 },
	{ "dance-double",	8 },
	{ "lights-cabinet",	NUM_CABINET_LIGHTS },
};

//
// Important:  Every game must define the buttons: "Start", "Back", "MenuLeft", "Operator" and "MenuRight"
//
Game g_Games[NUM_GAMES] = 
{
	{	// GAME_DANCE
		"dance",					// m_szName
		"Dance Dance Revolution",	// m_szDescription
		2,							// m_iNumControllers
		false,						// m_bCountNotesSeparately
		NUM_DANCE_BUTTONS,			// m_iButtonsPerController
		{	// m_szButtonNames
			"MenuLeft",
			"MenuRight",
			"MenuUp",
			"MenuDown",
			"Start",
			"Select",
			"Back",
			"Insert Coin",
			"Operator",
			"Left",
			"Right",
			"Up",
			"Down",
		},
		{	// m_DedicatedMenuButton
			GAME_BUTTON_MENULEFT,	// MENU_BUTTON_LEFT
			GAME_BUTTON_MENURIGHT,	// MENU_BUTTON_RIGHT
			GAME_BUTTON_MENUUP,	// MENU_BUTTON_UP
			GAME_BUTTON_MENUDOWN,	// MENU_BUTTON_DOWN
			GAME_BUTTON_START,	// MENU_BUTTON_START
			GAME_BUTTON_SELECT,	// MENU_BUTTON_SELECT
			GAME_BUTTON_BACK,	// MENU_BUTTON_BACK
			GAME_BUTTON_COIN,	// MENU_BUTTON_COIN
			GAME_BUTTON_OPERATOR	// MENU_BUTTON_OPERATOR
		},
		{	// m_SecondaryMenuButton
			DANCE_BUTTON_LEFT,		// MENU_BUTTON_LEFT
			DANCE_BUTTON_RIGHT,		// MENU_BUTTON_RIGHT
			DANCE_BUTTON_UP,		// MENU_BUTTON_UP
			DANCE_BUTTON_DOWN,		// MENU_BUTTON_DOWN
			GAME_BUTTON_START,		// MENU_BUTTON_START
			GAME_BUTTON_SELECT,		// MENU_BUTTON_SELECT
			GAME_BUTTON_BACK,		// MENU_BUTTON_BACK
			GAME_BUTTON_COIN,		// MENU_BUTTON_COIN
			GAME_BUTTON_OPERATOR,		// MENU_BUTTON_OPERATOR
		},
		{	// m_iDefaultKeyboardKey
			{	// PLAYER_1
				KEY_DEL,			// GAME_BUTTON_MENULEFT
				KEY_PGDN,			// GAME_BUTTON_MENURIGHT
				KEY_HOME,			// GAME_BUTTON_MENUUP
				KEY_END,			// GAME_BUTTON_MENUDOWN
				KEY_ENTER,			// GAME_BUTTON_START
				KEY_SLASH,		 	// GAME_BUTTON_SELECT
				KEY_ESC,			// GAME_BUTTON_BACK
				KEY_F1,				// GAME_BUTTON_COIN
				KEY_SCRLLOCK,			// GAME_BUTTON_OPERATOR

				KEY_LEFT,			// DANCE_BUTTON_LEFT
				KEY_RIGHT,			// DANCE_BUTTON_RIGHT
				KEY_UP,				// DANCE_BUTTON_UP
				KEY_DOWN,			// DANCE_BUTTON_DOWN
				NO_DEFAULT_KEY,		 	// DANCE_BUTTON_UPLEFT
				NO_DEFAULT_KEY,			// DANCE_BUTTON_UPRIGHT
			},
			{	// PLAYER_2
				KEY_KP_SLASH,			// GAME_BUTTON_MENULEFT
				KEY_KP_ASTERISK,		// GAME_BUTTON_MENURIGHT
				KEY_KP_HYPHEN,			// GAME_BUTTON_MENUUP
				KEY_KP_PLUS,			// GAME_BUTTON_MENUDOWN
				KEY_KP_ENTER,			// GAME_BUTTON_START
				KEY_KP_C0,		 	// GAME_BUTTON_SELECT
				KEY_NUMLOCK,			// GAME_BUTTON_BACK
				NO_DEFAULT_KEY,			// GAME_BUTTON_COIN
				NO_DEFAULT_KEY,			// GAME_BUTTON_OPERATOR

				KEY_KP_C4,			// DANCE_BUTTON_LEFT
				KEY_KP_C6,			// DANCE_BUTTON_RIGHT
				KEY_KP_C8,			// DANCE_BUTTON_UP
				KEY_KP_C2,			// DANCE_BUTTON_DOWN
			},
		},
		TNS_MARVELOUS,		// m_mapMarvelousTo
		TNS_PERFECT,		// m_mapPerfectTo
		TNS_GREAT,		// m_mapGreatTo
		TNS_GOOD,		// m_mapGoodTo
		TNS_BOO,		// m_mapBooTo
	},
	{	// GAME_LIGHTS
		"lights",			// m_szName
		"Lights",			// m_szDescription
		1,				// m_iNumControllers
		false,				// m_bCountNotesSeparately
		NUM_LIGHTS_BUTTONS,		// m_iButtonsPerController
		{	// m_szButtonNames
			"MenuLeft",
			"MenuRight",
			"MenuUp",
			"MenuDown",
			"Start",
			"Select",
			"Back",
			"Insert Coin",
			"Operator",

			"MarqueeUpLeft",
			"MarqueeUpRight",
			"MarqueeLrLeft",
			"MarqueeLrRight",
			"ButtonsLeft",
			"ButtonsRight",
			"BassLeft",
			"BassRight",
		},
		{	// m_DedicatedMenuButton
			GAME_BUTTON_MENULEFT,		// MENU_BUTTON_LEFT
			GAME_BUTTON_MENURIGHT,		// MENU_BUTTON_RIGHT
			GAME_BUTTON_MENUUP,		// MENU_BUTTON_UP
			GAME_BUTTON_MENUDOWN,		// MENU_BUTTON_DOWN
			GAME_BUTTON_START,		// MENU_BUTTON_START
			GAME_BUTTON_SELECT,		// MENU_BUTTON_SELECT
			GAME_BUTTON_BACK,		// MENU_BUTTON_BACK
			GAME_BUTTON_COIN,		// MENU_BUTTON_COIN
			GAME_BUTTON_OPERATOR		// MENU_BUTTON_OPERATOR
		},
		{	// m_SecondaryMenuButton
			LIGHTS_BUTTON_MARQUEE_UP_LEFT,		// MENU_BUTTON_LEFT
			LIGHTS_BUTTON_MARQUEE_UP_RIGHT,		// MENU_BUTTON_RIGHT
			LIGHTS_BUTTON_MARQUEE_LR_LEFT,		// MENU_BUTTON_UP
			LIGHTS_BUTTON_MARQUEE_LR_RIGHT,		// MENU_BUTTON_DOWN
			GAME_BUTTON_START,			// MENU_BUTTON_START
			GAME_BUTTON_SELECT,			// MENU_BUTTON_SELECT
			GAME_BUTTON_BACK,			// MENU_BUTTON_BACK
			GAME_BUTTON_COIN,			// MENU_BUTTON_COIN
			GAME_BUTTON_OPERATOR,			// MENU_BUTTON_OPERATOR
		},
		{	// m_iDefaultKeyboardKey
			{	// PLAYER_1
				KEY_DEL,			// GAME_BUTTON_MENULEFT
				KEY_PGDN,			// GAME_BUTTON_MENURIGHT
				KEY_HOME,			// GAME_BUTTON_MENUUP
				KEY_END,			// GAME_BUTTON_MENUDOWN
				KEY_ENTER,			// GAME_BUTTON_START
				NO_DEFAULT_KEY,			// GAME_BUTTON_SELECT
				KEY_ESC,			// GAME_BUTTON_BACK
				KEY_F1,				// GAME_BUTTON_COIN
				KEY_SCRLLOCK,			// GAME_BUTTON_OPERATOR

				KEY_Cq,				// LIGHTS_BUTTON_MARQUEE_UP_LEFT
				KEY_Cw,				// LIGHTS_BUTTON_MARQUEE_UP_RIGHT
				KEY_Ce,				// LIGHTS_BUTTON_MARQUEE_LR_LEFT
				KEY_Cr,				// LIGHTS_BUTTON_MARQUEE_LR_RIGHT
				KEY_Ct,			 	// LIGHTS_BUTTON_BUTTONS_LEFT
				KEY_Cy,				// LIGHTS_BUTTON_BUTTONS_RIGHT
				KEY_Cu,				// LIGHTS_BUTTON_BASS_LEFT
				KEY_Ci,				// LIGHTS_BUTTON_BASS_RIGHT
			},
			{	// PLAYER_2
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MENULEFT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MENURIGHT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MENUUP
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MENUDOWN
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_START
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_SELECT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_BACK
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_COIN
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_OPERATOR

				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MARQUEE_UP_LEFT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MARQUEE_UP_RIGHT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MARQUEE_LR_LEFT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_MARQUEE_LR_RIGHT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_BUTTONS_LEFT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_BUTTONS_RIGHT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_BASS_LEFT
				NO_DEFAULT_KEY,		// LIGHTS_BUTTON_BASS_RIGHT
			},
		},
		TNS_MARVELOUS,	// m_mapMarvelousTo
		TNS_PERFECT,	// m_mapPerfectTo
		TNS_GREAT,		// m_mapGreatTo
		TNS_GOOD,		// m_mapGoodTo
		TNS_BOO,		// m_mapBooTo
	},
};

Style g_Styles[] = 
{
	{	// STYLE_DANCE_SINGLE
		&g_Games[GAME_DANCE],					// m_Game
		true,									// m_bUsedForGameplay
		true,									// m_bUsedForEdit
		true,									// m_bUsedForDemonstration
		true,									// m_bUsedForHowToPlay
		"single",								// m_szName
		STEPS_TYPE_DANCE_SINGLE,				// m_StepsType
		ONE_PLAYER_ONE_SIDE,		// m_StyleType
		4,										// m_iColsPerPlayer
		{	// m_ColumnInfo[NUM_PLAYERS][MAX_COLS_PER_PLAYER];
			{	// PLAYER_1
				{ TRACK_1,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_3,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_4,	+DANCE_COL_SPACING*1.5f, NULL },
			},
			{	// PLAYER_2
				{ TRACK_1,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_3,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_4,	+DANCE_COL_SPACING*1.5f, NULL },
			},
		},
		{	// m_iInputColumn[MAX_GAME_CONTROLLERS][MAX_GAME_BUTTONS]
			{ 0, 3, 2, 1, Style::END_MAPPING },
			{ 0, 3, 2, 1, Style::END_MAPPING }
		},
		{	// m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
			0, 1, 2, 3
		},
		false, // m_bNeedsZoomOutWith2Players
		true, // m_bCanUseBeginnerHelper
	},
	{	// STYLE_DANCE_VERSUS
		&g_Games[GAME_DANCE],				// m_Game
		true,									// m_bUsedForGameplay
		false,									// m_bUsedForEdit
		true,									// m_bUsedForDemonstration
		false,									// m_bUsedForHowToPlay
		"versus",								// m_szName
		STEPS_TYPE_DANCE_SINGLE,				// m_StepsType
		TWO_PLAYERS_TWO_SIDES,		// m_StyleType
		4,										// m_iColsPerPlayer
		{	// m_ColumnInfo[NUM_PLAYERS][MAX_COLS_PER_PLAYER];
			{	// PLAYER_1
				{ TRACK_1,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_3,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_4,	+DANCE_COL_SPACING*1.5f, NULL },
			},
			{	// PLAYER_2
				{ TRACK_1,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_3,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_4,	+DANCE_COL_SPACING*1.5f, NULL },
			},
		},
		{
			{ 0, 3, 2, 1, Style::END_MAPPING },
			{ 0, 3, 2, 1, Style::END_MAPPING }
		},
		{	// m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
			0, 1, 2, 3
		},
		false, // m_bNeedsZoomOutWith2Players
		true, // m_bCanUseBeginnerHelper
	},
	{	// STYLE_DANCE_DOUBLE
		&g_Games[GAME_DANCE],				// m_Game
		true,									// m_bUsedForGameplay
		true,									// m_bUsedForEdit
		true,									// m_bUsedForDemonstration
		false,									// m_bUsedForHowToPlay
		"double",								// m_szName
		STEPS_TYPE_DANCE_DOUBLE,				// m_StepsType
		ONE_PLAYER_TWO_SIDES,		// m_StyleType
		8,										// m_iColsPerPlayer
		{	// m_ColumnInfo[NUM_PLAYERS][MAX_COLS_PER_PLAYER];
			{	// PLAYER_1
				{ TRACK_1,	-DANCE_COL_SPACING*3.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_3,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_4,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_5,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_6,	+DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_7,	+DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_8,	+DANCE_COL_SPACING*3.5f, NULL },
			},
			{	// PLAYER_2
				{ TRACK_1,	-DANCE_COL_SPACING*3.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_3,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_4,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_5,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_6,	+DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_7,	+DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_8,	+DANCE_COL_SPACING*3.5f, NULL },
			},
		},
		{	// m_iInputColumn[MAX_GAME_CONTROLLERS][MAX_GAME_BUTTONS]
			{ 0, 3, 2, 1, Style::END_MAPPING },
			{ 4, 7, 6, 5, Style::END_MAPPING }
		},
		{	// m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
			0,1,2,3,4,5,6,7
		},
		false, // m_bNeedsZoomOutWith2Players
		false, // m_bCanUseBeginnerHelper
	},
	{	// STYLE_LIGHTS_CABINET
		&g_Games[GAME_LIGHTS],				// m_Game
		true,									// m_bUsedForGameplay
		true,									// m_bUsedForEdit
		false,									// m_bUsedForDemonstration
		false,									// m_bUsedForHowToPlay
		"cabinet",								// m_szName
		STEPS_TYPE_LIGHTS_CABINET,				// m_StepsType
		ONE_PLAYER_ONE_SIDE,		// m_StyleType
		NUM_CABINET_LIGHTS,						// m_iColsPerPlayer
		{	// m_ColumnInfo[NUM_PLAYERS][MAX_COLS_PER_PLAYER];
			{	// PLAYER_1
				{ TRACK_1,	-DANCE_COL_SPACING*3.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_3,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_4,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_5,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_6,	+DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_7,	+DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_8,	+DANCE_COL_SPACING*3.5f, NULL },
			},
			{	// PLAYER_2
				{ TRACK_1,	-DANCE_COL_SPACING*3.5f, NULL },
				{ TRACK_2,	-DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_3,	-DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_4,	-DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_5,	+DANCE_COL_SPACING*0.5f, NULL },
				{ TRACK_6,	+DANCE_COL_SPACING*1.5f, NULL },
				{ TRACK_7,	+DANCE_COL_SPACING*2.5f, NULL },
				{ TRACK_8,	+DANCE_COL_SPACING*3.5f, NULL },
			},
		},
		{	// m_iInputColumn[MAX_GAME_CONTROLLERS][MAX_GAME_BUTTONS]
			{ 0, 1, 2, 3, 4, 5, 6, 7, 8, Style::END_MAPPING },
			{ 0, 1, 2, 3, 4, 5, 6, 7, 8, Style::END_MAPPING },
		},
		{	// m_iColumnDrawOrder[MAX_COLS_PER_PLAYER];
			0,1,2,3,4,5,6,7
		},
		false, // m_bNeedsZoomOutWith2Players
		false, // m_bCanUseBeginnerHelper
	},
};

#define NUM_STYLES ARRAYLEN(g_Styles)


GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::GetStylesForGame( const Game *pGame, vector<const Style*>& aStylesAddTo, bool editor ) const
{
	for( unsigned s=0; s<NUM_STYLES; s++ ) 
	{
		const Style* style = &g_Styles[s];
		if( style->m_pGame != pGame)
			continue;
		if( !editor && !style->m_bUsedForGameplay )	
			continue;
		if( editor && !style->m_bUsedForEdit )	
			continue;

		aStylesAddTo.push_back( style );
	}
}

const Style* GameManager::GetEditorStyleForStepsType( StepsType st ) const
{
	for( unsigned s=0; s<NUM_STYLES; s++ ) 
	{
		const Style* style = &g_Styles[s];
		if( style->m_StepsType == st && style->m_bUsedForEdit )
			return style;
	}

	ASSERT(0);	// this Game is missing a Style that can be used with the editor
	return NULL;
}


void GameManager::GetStepsTypesForGame( const Game *pGame, vector<StepsType>& aStepsTypeAddTo ) const
{
	FOREACH_StepsType( st )
	{
		bool found = false;
		for( unsigned s=0; !found && s<NUM_STYLES; s++ )
		{
			const Style* style = &g_Styles[s];
			if( style->m_pGame != pGame || style->m_StepsType != st )
				continue;

			found = true;
		}
		if( found )
			aStepsTypeAddTo.push_back( st );
	}
}

void GameManager::GetDemonstrationStylesForGame( const Game *pGame, vector<const Style*> &vpStylesOut ) const
{
	vpStylesOut.clear();

	for( unsigned s=0; s<NUM_STYLES; s++ ) 
	{
		const Style* style = &g_Styles[s];
		if( style->m_pGame == pGame && style->m_bUsedForDemonstration )
			vpStylesOut.push_back( style );
	}
	
	ASSERT( vpStylesOut.size()>0 );	// this Game is missing a Style that can be used with the demonstration
}

const Style* GameManager::GetHowToPlayStyleForGame( const Game *pGame ) const
{
	for( unsigned s=0; s<NUM_STYLES; s++ ) 
	{
		const Style* style = &g_Styles[s];
		if( style->m_pGame == pGame && style->m_bUsedForHowToPlay )
			return style;
	}

	ASSERT(0);	// this Game is missing a Style that can be used with HowToPlay
	return NULL;
}

void GameManager::GetEnabledGames( vector<const Game*>& aGamesOut ) const
{
	for( int g=0; g<NUM_GAMES; g++ )
	{
		const Game *pGame = &g_Games[g];
		if( IsGameEnabled( pGame ) )
			aGamesOut.push_back( pGame );
	}
}

const Game* GameManager::GetDefaultGame() const
{
	return &g_Games[0];
}

int GameManager::GetIndexFromGame( const Game* pGame ) const
{
	for( int g=0; g<NUM_GAMES; g++ )
	{
		if( &g_Games[g] == pGame )
			return g;
	}
	ASSERT(0);
	return 0;
}

const Game* GameManager::GetGameFromIndex( int index ) const
{
	ASSERT( index >= 0 );
	ASSERT( index < NUM_GAMES );
	return &g_Games[index];
}

bool GameManager::IsGameEnabled( const Game *pGame ) const
{
	CStringArray asNoteSkins;
	NOTESKIN->GetNoteSkinNames( pGame, asNoteSkins, false ); /* don't omit default */
	return asNoteSkins.size() > 0;
}

int GameManager::StepsTypeToNumTracks( StepsType st )
{
	ASSERT_M( st < NUM_STEPS_TYPES, ssprintf("%i", st) );
	return StepsTypes[st].NumTracks;
}

StepsType GameManager::StringToStepsType( CString sStepsType )
{
	sStepsType.MakeLower();

	// HACK!  We elminitated "ez2-single-hard", but we should still handle it.
	if( sStepsType == "ez2-single-hard" )
		sStepsType = "ez2-single";

	// HACK!  "para-single" used to be called just "para"
	if( sStepsType == "para" )
		sStepsType = "para-single";

	for( int i=0; i<NUM_STEPS_TYPES; i++ )
		if( StepsTypes[i].name == sStepsType )
			return StepsType(i);
	
	// invalid StepsType
	LOG->Warn( "Invalid StepsType string '%s' encountered.  Assuming this is 'dance-single'.", sStepsType.c_str() );
	return STEPS_TYPE_DANCE_SINGLE;
}

CString GameManager::StepsTypeToString( StepsType st )
{
	ASSERT_M( st < NUM_STEPS_TYPES, ssprintf("%i", st) );
	return StepsTypes[st].name;
}

CString GameManager::StepsTypeToThemedString( StepsType st )
{
	CString s = StepsTypeToString( st );
	if( THEME->HasMetric( "StepsType", s ) )
		return THEME->GetMetric( "StepsType", s );
	else
		return s;
}

CString GameManager::StyleToThemedString( const Style* style )
{
	CString s = style->m_szName;
	s = Capitalize( s );
	if( THEME->HasMetric( "Style", s ) )
		return THEME->GetMetric( "Style", s );
	else
		return s;
}

const Game* GameManager::StringToGameType( CString sGameType )
{
	for( int i=0; i<NUM_GAMES; i++ )
		if( !sGameType.CompareNoCase(g_Games[i].m_szName) )
			return &g_Games[i];

	return NULL;
}


const Style* GameManager::GameAndStringToStyle( const Game *game, CString sStyle )
{
	for( unsigned s=0; s<NUM_STYLES; s++ ) 
	{
		const Style* style = &g_Styles[s];
		if( style->m_pGame != game )
			continue;
		if( sStyle.CompareNoCase(style->m_szName) == 0 )
			return style;
	}

	return NULL;
}


CString GameManager::GetMenuButtonSecondaryFunction( const Game *pGame, GameButton gb ) const
{
	/*
	 * Each GameButton can be used in gameplay (if any gameplay style maps to
	 * it) and/or map to a menu button (if m_DedicatedMenuButton or m_SecondaryMenuButton
	 * map to it).
	 *
	 * If a button is only used in gameplay or is only used in menus, return ""; the 
	 * primary description is sufficient.
	 *
	 * If a button is used in both gameplay and menus, return szSecondaryNames[] for
	 * the MenuButton.
	 */
	vector<const Style*> aStyles;
	this->GetStylesForGame( pGame, aStyles );
	bool bUsedInGameplay = false;
	for( unsigned i = 0; i < aStyles.size(); ++i )
	{
		const Style *pStyle = aStyles[i];
		FOREACH_GameController(gc)
		{
			const StyleInput si = pStyle->GameInputToStyleInput( GameInput(gc,gb) );
			if( si.IsValid() )
				bUsedInGameplay = true;
		}
	}

	static const char *szSecondaryNames[] =
	{
		"MenuLeft",
		"MenuRight",
		"MenuUp",
		"MenuDown",
		"MenuStart",
		"MenuSelect",
		"MenuBack",
		"Coin",
		"Operator"
	};

	FOREACH_MenuButton(m)
	{
		if( !bUsedInGameplay && pGame->m_DedicatedMenuButton[m] == gb )
			return "";
		else if( bUsedInGameplay && pGame->m_SecondaryMenuButton[m] == gb )
			return szSecondaryNames[m];
	}

	return ""; // only used in gameplay
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
