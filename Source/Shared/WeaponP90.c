/*
OpenCS Project
Copyright (C) 2015 Marco "eukara" Hladik

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

.int iClip_P90;

// Weapon Info
weaponinfo_t wptP90 = { 
	WEAPON_P90, 		// Identifier
	SLOT_PRIMARY,
	2350, 				// Price
	CALIBER_57MM, 		// Caliber ID
	235, 				// Max Player Speed
	1, 					// Bullets Per Shot
	50, 				// Clip/MagSize
	26, 				// Damage Per Bullet
	1, 					// Penetration Multiplier
	4096, 				// Bullet Range
	0.84, 				// Range Modifier
	TYPE_AUTO,
	0.07, 				// Attack-Delay
	3.3, 				// Reload-Delay
	iAmmo_57MM, 		// Caliber Pointer
	iClip_P90 			// Clip Pointer
};

// Anim Table
enum {
	ANIM_P90_IDLE,
	ANIM_P90_RELOAD,
	ANIM_P90_DRAW,
	ANIM_P90_SHOOT1,
	ANIM_P90_SHOOT2,
	ANIM_P90_SHOOT3
};

void WeaponP90_Draw( void ) {
#ifdef QWSSQC
	OpenCSGunBase_Draw();
	sound( self, CHAN_WEAPON, "weapons/p90_boltpull.wav", 1, ATTN_IDLE ); // TODO: Move to the client...?
#else
	View_PlayAnimation( ANIM_P90_DRAW );
#endif
}

void WeaponP90_PrimaryFire( void ) {
#ifdef QWSSQC
	if ( OpenCSGunBase_PrimaryFire() == TRUE ) {
		sound( self, CHAN_WEAPON, "weapons/p90-1.wav", 1, ATTN_NORM );
	}
#else
	int iRand = ceil( random() * 3 );
	
	if ( iRand == 1 ) {
		View_PlayAnimation( ANIM_P90_SHOOT1 );
	} else if ( iRand == 2 ) {
		View_PlayAnimation( ANIM_P90_SHOOT2 );
	} else {
		View_PlayAnimation( ANIM_P90_SHOOT3 );
	}
#endif
}

void WeaponP90_Reload( void ) {
	#ifdef QWSSQC
	if ( OpenCSGunBase_Reload() == TRUE ) {
		// Play Sound
	}
	#else
	View_PlayAnimation( ANIM_P90_RELOAD );
	#endif
}
