# MOBAHeroTest

An experiment where I programmed a MOBA hero, focusing on ability logic. No health components, thus damage only appears in the output log.

This project uses and RTS camera by Mark Vatsel

https://github.com/vatsel/Unreal-RTS-Camera

Controls:

Basic:

-Right Click on ground: move to location

-Right Click on enemy (hero): attack enemy (hero

-S: stop all actions (if not locked in action)

Abilities:

-Q: Dashes to an enemy (hero) and deals damage. Can be recast within the next 4 seconds, goes on cooldown for 8 seconds after 2nd cast/expiration.
Hero is locked during the dash.

-W: Blocks the next attack, only text output for the purpose of this project. Goes on cooldown for 6 seconds on cast.

-E: Resets the attack timer, increases the hero's attack speed and attacks, R hits and Q hits give stacking movement speed, 15% up to 3 times while the ability is active.The ability goes on cooldown for 11 seconds at cast and the effect lasts for 3 seconds. 

-R: Blinks (teleports instantly) 5 times behind the target and in front of them (back-front-back-front-close front) dealing damage. Hero is locked for the duration.
The ability goes on cooldown for 10 seconds at cast.

Animation usage is rudamentary and only serves to show that actions are in progress.
