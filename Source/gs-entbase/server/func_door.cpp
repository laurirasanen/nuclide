/***
*
*   Copyright (c) 2016-2019 Marco 'eukara' Hladik. All rights reserved.
* 
* 	See the file LICENSE attached with the sources for usage details.
*
****/

// TODO: Finish these
#define SF_MOV_OPEN		1
#define SF_MOV_UNLINK 		4
#define SF_MOV_PASSABLE	8
#define SF_MOV_TOGGLE		32
#define SF_MOV_USE			256

enum
{
	STATE_RAISED = 0,
	STATE_LOWERED,
	STATE_UP,
	STATE_DOWN
};

class CFuncDoor : CBaseTrigger
{
	vector m_vecPos1;
	vector m_vecPos2;
	vector m_vecDest;
	vector m_vecMoveDir;
	float m_flSpeed;
	float m_flLip;
	float m_iState;
	float m_flNextTrigger;
	float m_flWait;
	float m_flDelay;
	int m_iMoveSnd;
	int m_iStopSnd;
	int m_iDamage;
	int m_iLocked;
	
	void() CFuncDoor;
	virtual void() SetMovementDirection;
	virtual void(vector vdest, void() func) MoveToDestination;
	virtual void() MoveToDestination_End;
	virtual void() MoveAway;
	virtual void() MoveBack;
	virtual void() Precache;
	virtual void() Arrived;
	virtual void() Returned;
	virtual void() Trigger;
	virtual void() Blocked;
	virtual void() Touch;
	virtual void() PlayerUse;
	virtual void() m_pMove = 0;
};

void CFuncDoor :: Precache ( void )
{
	if( m_iMoveSnd > 0 && m_iMoveSnd <= 10 ) {
		precache_sound( sprintf( "doors/doormove%i.wav", m_iMoveSnd ) );
	} else {
		precache_sound( "common/null.wav" );
	}

	if( m_iStopSnd > 0 && m_iStopSnd <= 8 ) {
		precache_sound( sprintf( "doors/doorstop%i.wav", m_iStopSnd ) );
	} else {
		precache_sound( "common/null.wav" );
	}
}

void CFuncDoor :: PlayerUse ( void )
{
	eActivator.gflags &= ~GF_USE_RELEASED;
	Trigger();
}

void CFuncDoor :: Arrived ( void )
{
	m_iState = STATE_RAISED;

	if( m_iStopSnd > 0 && m_iStopSnd <= 8 ) {
		sound( self, CHAN_VOICE, sprintf( "doors/doorstop%i.wav", m_iStopSnd ), 1.0, ATTN_NORM );
	} else {
		sound( self, CHAN_VOICE, "common/null.wav", 1.0, ATTN_NORM );
	}

	if ( !( spawnflags & SF_MOV_USE ) ) {
		touch = Touch;
	}
	if ( m_flWait < 0 ) {
		return;
	}

	if ( !( spawnflags & SF_MOV_TOGGLE ) ) {
		think = MoveBack;
		nextthink = ( ltime + m_flWait );
	}
}

void CFuncDoor :: Returned ( void )
{
	if( m_iStopSnd > 0 && m_iStopSnd <= 8 ) {
		sound( self, CHAN_VOICE, sprintf( "doors/doorstop%i.wav", m_iStopSnd ), 1.0, ATTN_NORM );
	} else {
		sound( self, CHAN_VOICE, "common/null.wav", 1.0, ATTN_NORM );
	}

	if ( !( spawnflags & SF_MOV_USE ) ) {
		touch = Touch;
	}
    
	m_iState = STATE_LOWERED;
}

void CFuncDoor :: MoveBack ( void )
{
	if( m_iMoveSnd > 0 && m_iMoveSnd <= 10 ) {
		sound( self, CHAN_VOICE, sprintf( "doors/doormove%i.wav", m_iMoveSnd ), 1.0, ATTN_NORM );
	} else {
		sound( self, CHAN_VOICE, "common/null.wav", 1.0, ATTN_NORM );
	}

	if ( !( spawnflags & SF_MOV_USE ) ) {
		touch = __NULL__;
	}
    
	m_iState = STATE_DOWN;
	MoveToDestination( m_vecPos1, Returned );
}

void CFuncDoor :: MoveAway ( void )
{
	if ( m_iState == STATE_UP ) {
		return;
	}

	if( m_iMoveSnd > 0 && m_iMoveSnd <= 10 ) {
		sound( self, CHAN_VOICE, sprintf( "doors/doormove%i.wav", m_iMoveSnd ), 1.0, ATTN_NORM );
	} else {
		sound( self, CHAN_VOICE, "common/null.wav", 1.0, ATTN_NORM );
	}

	if ( !( spawnflags & SF_MOV_TOGGLE ) ) {
		if ( m_iState == STATE_RAISED ) {
			nextthink = ( ltime + m_flWait );
			return;
		}
	}

	m_iState = STATE_UP;
	MoveToDestination( m_vecPos2, Arrived );
}

void CFuncDoor :: Trigger ( void )
{
	if ( m_flNextTrigger > ltime ) {
		if ( !( spawnflags & SF_MOV_TOGGLE ) ) {
			return;
		}
	}
	m_flNextTrigger = ltime + m_flWait;

	// Only trigger stuff when we are done moving
	if ( ( m_iState == STATE_RAISED ) || ( m_iState == STATE_LOWERED ) ) {
		if ( m_flDelay > 0 ) {
#ifdef GS_DEVELOPER
			print( sprintf( "CFuncDoor: Delayed trigger of `%s`\n", m_strTarget ) );
#endif
			CBaseTrigger::UseTargets_Delay( m_flDelay );
		} else {
#ifdef GS_DEVELOPER
			dprint( sprintf( "CFuncDoor: Normal trigger of `%s`\n", m_strTarget ) );
#endif
			CBaseTrigger::UseTargets();
		}
	}

	if ( ( m_iState == STATE_UP ) || ( m_iState == STATE_RAISED ) ){
		MoveBack();
		return;
	}

	MoveAway();
}

void CFuncDoor :: Touch ( void )
{
	if ( spawnflags & SF_MOV_TOGGLE ) {
		return;
	}

	if ( m_iLocked ) {
		return;
	}

	if ( other.movetype == MOVETYPE_WALK ) {
		if ( other.absmin[2] <= maxs[2] - 2 ) {
			Trigger();
		}
	}
}

void CFuncDoor :: Blocked ( void )
{
	if( m_iDamage ) {
		//Damage_Apply( other, self, dmg, other.origin, FALSE );
	}

	if ( m_flWait >= 0 ) {
		if ( m_iState == STATE_DOWN ) {
			MoveAway ();
		} else {
			MoveBack ();
		}
	}
}


void CFuncDoor :: Respawn ( void )
{
	solid = SOLID_BSP;
	movetype = MOVETYPE_PUSH;
	setorigin( this, origin );
	setmodel( this, model );

	blocked = Blocked;
	//Use = Trigger;

	if ( m_flWait == -1 ) {
		spawnflags |= SF_MOV_TOGGLE;
	}

	if ( !m_flSpeed ) {
		m_flSpeed = 100;
	}

	if ( !m_iDamage ) {
		m_iDamage = 2;
	}

	if ( spawnflags & SF_MOV_USE ) {
		touch = __NULL__;
		gflags |= GF_USABLE;
	} else {
		touch = Touch;
		gflags -= gflags & GF_USABLE;
	}

	m_iState = STATE_LOWERED;
	m_vecPos1 = origin;
	m_vecPos2 = ( m_vecPos1 + m_vecMoveDir * ( fabs( m_vecMoveDir * size ) - m_flLip ) );

	if ( spawnflags & SF_MOV_OPEN ) {
		setorigin( this, m_vecPos2 );
		m_vecPos2 = m_vecPos1;
		m_vecPos1 = origin;
	}
}

void CFuncDoor :: CFuncDoor ( void )
{
	for ( int i = 1; i < ( tokenize( __fullspawndata ) - 1 ); i += 2 ) {
		switch ( argv( i ) ) {
		case "speed":
			m_flSpeed = stof( argv( i + 1 ) );
			break;
		case "lip":
			m_flLip = stof( argv( i + 1 ) );
			break;
		case "movesnd":
			m_iMoveSnd = stoi( argv( i + 1 ) );
			break;
		case "stopsnd":
			m_iStopSnd = stoi( argv( i + 1 ) );
			break;
		case "delay":
			m_flDelay = stof( argv( i + 1 ) );
			break;
		case "wait":
			m_flWait = stof( argv( i + 1 ) );
			break;
		default:
			break;
		}
	}

	CFuncDoor::Precache();
	CFuncDoor::SetMovementDirection();
	CFuncDoor::Respawn();
	CBaseTrigger::CBaseTrigger();

	if ( m_strTargetName ) {
		m_iLocked = TRUE;
	}
}

void CFuncDoor :: SetMovementDirection ( void )
{
	if ( angles == '0 -1 0' ) {
		m_vecMoveDir = '0 0 1';
	} else if ( angles == '0 -2 0' ) {
		m_vecMoveDir = '0 0 -1';
	} else {
		makevectors( angles );
		m_vecMoveDir = v_forward;
	}

	angles = '0 0 0';
}

void CFuncDoor :: MoveToDestination_End ( void )
{
	setorigin( this, m_vecDest );
	velocity = '0 0 0';
	nextthink = -1;
	m_pMove();
}

void CFuncDoor :: MoveToDestination ( vector vDestination, void() func )
{
	vector vecDifference;
	float flTravel, fTravelTime;

	if ( !m_flSpeed ) {
		objerror( "No speed defined for moving entity! Will not divide by zero." );
	}

	m_pMove = func;
	m_vecDest = vDestination;
	think = MoveToDestination_End;

	if ( vDestination == origin ) {
		velocity = '0 0 0';
		nextthink = ( ltime + 0.1 );
		return;
	}

	vecDifference = ( vDestination - origin );
	flTravel = vlen( vecDifference );
	fTravelTime = ( flTravel / m_flSpeed );

	if ( fTravelTime < 0.1 ) {
		velocity = '0 0 0';
		nextthink = ltime + 0.1;
		return;
	}

	nextthink = ( ltime + fTravelTime );
	velocity = ( vecDifference * ( 1 / fTravelTime ) );
}

CLASSEXPORT( func_door, CFuncDoor )

void func_water ( void )
{
	spawnfunc_CFuncDoor();
	self.classname = "CFuncWater";
	self.solid = SOLID_BSP;
	self.skin = CONTENT_WATER;
}
