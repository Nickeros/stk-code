//  $Id: TrackSel.cxx,v 1.19 2004/08/23 14:52:11 grumbel Exp $
//
//  TuxKart - a fun racing game with go-kart
//  Copyright (C) 2004 Steve Baker <sjbaker1@airmail.net>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "TrackSel.h"
#include "tuxkart.h"
#include "WidgetSet.h"
#include "StartScreen.h"
#include "TrackManager.h"

#include <string>

using std::string;

TrackSel::TrackSel(RaceSetup& raceSetup_)
  : raceSetup(raceSetup_)
{
	menu_id = widgetSet -> vstack(0);

	widgetSet -> label(menu_id, "Choose a Track", GUI_LRG, GUI_TOP, 0, 0);
	widgetSet -> space(menu_id);

	int ha = widgetSet -> harray(menu_id);

	int col1 = widgetSet -> varray(ha);
	int col2 = widgetSet -> varray(ha);

	for (unsigned int i = 0; i != track_manager.tracks.size()/2; ++i)
		widgetSet -> state(col1, track_manager.tracks[i].name.c_str(), GUI_SML, i, 0);

	for (unsigned int i = track_manager.tracks.size()/2; i != track_manager.tracks.size(); ++i)
		widgetSet -> state(col2, track_manager.tracks[i].name.c_str(), GUI_SML, i, 0);

	widgetSet -> layout(menu_id, 0, 1);
}

TrackSel::~TrackSel()
{
	widgetSet -> delete_widget(menu_id) ;
}
	
void TrackSel::update(float dt)
{
	
	widgetSet -> timer(menu_id, dt) ;
	widgetSet -> paint(menu_id) ;

	{
		glClear(GL_DEPTH_BUFFER_BIT);
		if( widgetSet -> token (widgetSet -> click()) != MENU_RETURN ) {
			TrackData track_data = track_manager.tracks[widgetSet -> token (widgetSet -> click())];

			float x     = 0.5f;
			float y     = 0.0f;
			float scale = .003f;

			glBegin ( GL_LINE_LOOP ) ;
			for ( int i = 0 ; i < int(track_data.driveline.size()) ; i++ )
			{
				glVertex2f ( x + ( track_data.driveline[i][0] ) * scale,
				             y + ( track_data.driveline[i][1] ) * scale ) ;
			}
			glEnd () ;
		}
	}
}

void TrackSel::select()
{
	raceSetup.track = widgetSet -> token ( widgetSet -> click() );
	StartScreen::current()->switchToGame();
}

void TrackSel::keybd(const SDL_keysym& key)
{
	switch ( key.sym )
	{
	case SDLK_LEFT:
	case SDLK_RIGHT:
	case SDLK_UP:
	case SDLK_DOWN:
		widgetSet -> pulse(widgetSet -> cursor(menu_id, key.sym), 1.2f);
		break;
		
	case SDLK_RETURN: select(); break;
	
	case SDLK_ESCAPE:
		guiStack.pop_back();
	default: break;
	}
}

void TrackSel::point(int x, int y)
{
	widgetSet -> pulse(widgetSet -> point(menu_id, x, y), 1.2f);
}

void TrackSel::stick(int x, int y)
{
	widgetSet -> pulse(widgetSet -> stick(menu_id, x, y), 1.2f);
}

/* EOF */
