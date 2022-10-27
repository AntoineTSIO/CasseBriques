Carte à trou : < redacted >


When starting a game, seed it and keep the seed somewhere.
Offer a possibility to have a custom-seeded game.


Discuss whether invincibility + shield prevents the shield from depleting.


Keys/actions : 4 directions, bomb AND do_nothing.


Will we put a limmit to how much bombs we get ?


max_range : do we put a limit or not ? If yes, Game struct need one more attribute,
 and it should be set at the start of a game.
A good limit would be <(min(max_height, max_width) - 1) / 2>, since explosions go both
 ways up to < range > without counting the tile the bomb is initially on.


Additionnal objects that might be funny :
 - Confusion
 - Teleport
 - Spawn a random bomb with random stats
 - Ghost (Pass through walls and objects, can't be touched by bombs. Maybe 4 or 5 turns ?)