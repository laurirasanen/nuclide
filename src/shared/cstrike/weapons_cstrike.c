/*
 * Copyright (c) 2016-2020 Marco Hladik <marco@icculus.org>
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

/* called whenever a cstrike gun fires a successful shot */
void
Cstrike_ShotMultiplierAdd(player pl, int shots)
{
	/* more than 12 is enough, you can barely hit the barn */
	pl.cs_shotmultiplier = bound(0, pl.cs_shotmultiplier + shots, 12);

	pl.cs_shottime = 0.2f;
	pl.punchangle[0] = -2 * (pl.cs_shotmultiplier / 6);
	pl.punchangle[1] = random(-1, 1);
}

/* generate an accuracy value that we'll pass onto TraceAttack */
float
Cstrike_CalculateAccuracy(player pl, float divisor)
{
	if (divisor == -1) {
		/* snipers shoot way less accurate overall. */
		return (pl.viewzoom < 1.0f) ? 0.0f : 0.05;
	} else {
		return  (pl.cs_shotmultiplier / divisor);
	}
}

/* called whenever cstrike guns aren't firing */
void
Cstrike_ShotMultiplierUpdate(player pl)
{
	if ((pl.cs_shotmultiplier > 0) && (pl.cs_shottime <= 0.0f)) {
		pl.cs_shottime = pl.w_attack_next + 0.01;
		pl.cs_shotmultiplier--;
	}

	pl.cs_shottime = max(0, pl.cs_shottime - input_timelength);
}
void
w_cstrike_weaponrelease(void)
{
	player pl = (player)self;
	Cstrike_ShotMultiplierUpdate(pl);
}
