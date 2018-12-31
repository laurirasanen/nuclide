/*
	Copyright 2016-2018 Marco "eukara" Hladik
	
	MIT LICENSE

	Permission is hereby granted, free of charge, to any person 
	obtaining a copy of this software and associated documentation 
	files (the "Software"), to deal in the Software without 
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

/*

TODO: This gamemode is odd about balancing, right now the last surviving terrorist
 	  will decide the match. Still have to think about what rules to set up.
*/

/*
=================
func_escapezone_touch
=================
*/
void func_escapezone_touch( void ) {
	if ( ( other.classname == "player" ) && ( other.team == TEAM_T ) ) {
		entity eOld = self;
		self = other;

		Spawn_MakeSpectator();
		self.classname = "player";
		forceinfokey( self, "*dead", "0" );
		self.health = 0;
		Rules_CountPlayers();
		self = eOld;

		if ( iAlivePlayers_T == 0 ) {
			Rules_RoundOver( TEAM_T, 2500, FALSE );
		}
	}
}

/*
=================
SPAWN: func_escapezone

Entry function for the terrorist escape zone
=================
*/
void func_escapezone( void ) {
	self.angles = '0 0 0';
	self.movetype = MOVETYPE_NONE;
	self.solid = SOLID_TRIGGER;

	if ( self.model ) {
		setmodel( self, self.model );
	} else {
		setsize( self, self.mins, self.maxs );
	}

	self.model = 0;
	self.touch = func_escapezone_touch;
	
	iEscapeZones++;
}
