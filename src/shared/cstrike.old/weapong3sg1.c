/*
 * Copyright (c) 2016-2019 Marco Hladik <marco@icculus.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

.int iMag_G3SG1;

// Weapon Info
weaponinfo_t wptG3SG1 = { 
	WEAPON_G3SG1, 		// Identifier
	SLOT_PRIMARY,		// Slot
	5000, 				// Price
	CALIBER_762MM, 		// Caliber ID
	0.84, 				// Max Player Speed
	1, 					// Bullets Per Shot
	20, 				// Clip/MagSize
	80, 				// Damage Per Bullet
	3, 					// Penetration Multiplier
	8192, 				// Bullet Range
	0.98, 				// Range Modifier
	TYPE_AUTO,			// Firing Type
	0.25, 				// Attack-Delay
	4.6, 				// Reload-Delay
	iAmmo_762MM, 		// Caliber Pointer
	iMag_G3SG1, 		// Clip Pointer
	200,				// Accuracy Divisor
	0.55,				// Accuracy Offset
	1.4,				// Max Inaccuracy
	6,					// Minimum Crosshair Distance
	4,					// Crosshair Movement Delta
	1.65,				// Armor penetration ratio
	ATYPE_CARBINE,		// Animation Type
	SHELL_RIFLE
};

// Anim Table
enum {
	ANIM_G3SG1_IDLE,
	ANIM_G3SG1_SHOOT1,
	ANIM_G3SG1_SHOOT2,
	ANIM_G3SG1_RELOAD,
	ANIM_G3SG1_DRAW
};

void WeaponG3SG1_Draw( void ) {
	#ifdef SSQC
	BaseGun_Draw();
	#else
	View_PlayAnimation( ANIM_G3SG1_DRAW );
	#endif
}

void WeaponG3SG1_PrimaryFire( void ) {
	#ifdef SSQC
	if ( BaseGun_PrimaryFire() == TRUE ) {
		// Play Sound
		dprint("[DEBUG] FIRE!\n");
		sound( self, CHAN_WEAPON, "weapons/g3sg1-1.wav", 1, ATTN_NORM );
	}
	#else

	if ( random() <= 0.5 ) {
		View_PlayAnimation( ANIM_G3SG1_SHOOT1 );
	} else {
		View_PlayAnimation( ANIM_G3SG1_SHOOT2 );
	}
	BaseGun_ShotMultiplierHandle( 1 );
	#endif
}

void WeaponG3SG1_SecondaryFire( void ) {
#ifdef SSQC
	if ( self.viewzoom == 1.0 ) {
		self.viewzoom = 0.45;
	} else if ( self.viewzoom == 0.45 ) {
		self.viewzoom = 0.15;
	} else {
		self.viewzoom = 1.0;
	}
	
	self.fAttackFinished = time + 0.5;
#endif
}

void WeaponG3SG1_Reload( void ) {
	#ifdef SSQC
	if ( BaseGun_Reload() == TRUE ) {
		// Play Sound
	}
	#else
	View_PlayAnimation( ANIM_G3SG1_RELOAD );
	#endif
}