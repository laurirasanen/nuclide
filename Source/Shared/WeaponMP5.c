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

.int iClip_MP5;

// Weapon Info
weaponinfo_t wptMP5 = { 
	WEAPON_MP5, 		// Identifier
	SLOT_PRIMARY,
	1500, 				// Price
	CALIBER_9MM, 		// Caliber ID
	240, 				// Max Player Speed
	1, 					// Bullets Per Shot
	30, 				// Clip/MagSize
	26, 				// Damage Per Bullet
	1, 					// Penetration Multiplier
	4096, 				// Bullet Range
	0.84, 				// Range Modifier
	TYPE_AUTO,
	0.08, 				// Attack-Delay
	2.6, 				// Reload-Delay
	iAmmo_9MM, 			// Caliber Pointer
	iClip_MP5 			// Clip Pointer
};

// Anim Table
enum {
	ANIM_MP5_IDLE,
	ANIM_MP5_RELOAD,
	ANIM_MP5_DRAW,
	ANIM_MP5_SHOOT1,
	ANIM_MP5_SHOOT2,
	ANIM_MP5_SHOOT3
};

void WeaponMP5_Draw( void ) {
#ifdef QWSSQC
	OpenCSGunBase_Draw();
	sound( self, CHAN_WEAPON, "weapons/mp5_slideback.wav", 1, ATTN_IDLE ); // TODO: Move to the client...?
#else
	View_PlayAnimation( ANIM_MP5_DRAW );
#endif
}

void WeaponMP5_PrimaryFire( void ) {
#ifdef QWSSQC
	if ( OpenCSGunBase_PrimaryFire() == TRUE ) {
		if ( random() <= 0.5 ) {
			sound( self, CHAN_WEAPON, "weapons/mp5-1.wav", 1, ATTN_NORM );
		} else {
			sound( self, CHAN_WEAPON, "weapons/mp5-2.wav", 1, ATTN_NORM );
		}
	}
#else
	int iRand = ceil( random() * 3 );
	if ( iRand == 1 ) {
		View_PlayAnimation( ANIM_MP5_SHOOT1 );
	} else if ( iRand == 2 ) {
		View_PlayAnimation( ANIM_MP5_SHOOT2 );
	} else {
		View_PlayAnimation( ANIM_MP5_SHOOT3 );
	}
#endif
}

void WeaponMP5_Reload( void ) {
#ifdef QWSSQC
	if ( OpenCSGunBase_Reload() == TRUE ) {
		// Play Sound
	}
#else
	View_PlayAnimation( ANIM_MP5_RELOAD );
#endif
}
