# Raycast Test!

This project is intended to be a segway to learn more about raycasting in the style of old 'pseudo-3d' games, such as Doom and Wolfenstein3D.
It uses a raycast detection algorithm to render primitive cubes to make it look 3D, even when all the logic is done in 2D.

## TODO
I gotta work on making some actual lighting instead of the janky system I have.
I think it would be a good idea to keep the system for a simulated player 'light source' rather than using the most likely more complex logic needed for actual lighting.

I also need to convert the existing system to use the % of screen taken rather than the actual pixel height, to be honest.
So that's the next objective.


To compile, you will need the Allegro 5 libraries, located at the main website here. https://www.allegro.cc/

## TODO 2
Write a texture system which takes an existing bitmap texure and maps it onto a cube while maintaining the scale
(aka no artifacts and weird visual bugs)

A good way to make the system is to rip apart the texture pixel by pixel, and then scale each pixel onto it's appropriate spot. The whole system will work by reading an appropriate texture from a file directory, deciding which 'side' of the wall you're looking up by checking the specific tile, and then decide to render as a wall or a floor/ceiling combo.

Maybe a good plan would to write render functions for each type of tile, so as
to not write one big renderer that does it all by hand.

# POSSIBLE REWRITE
I've found a bit more understandable math for making and rendering walls, so the best idea would be to rewrite the entire game engine from scratch. With that, I would be able to do a lot more, and learn some more interesting techniques...
It'll be a lot of work though, is it justified?
We'll find out.


