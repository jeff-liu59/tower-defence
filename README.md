# tower-defence
In-terminal tower defence game created for university.

Project was done in 2023 using C.

Game will open by prompting for the following inputs:
 - Lives given to the player
 - Starting money available to the player
 - Starting point for the enemies
 - Ending point for the enemies
 - Coordinates for opposite corners of a body of water
 - Inputs to create the path for enemies to follow (This is input using u, d, l and r for up, down, left and right respectively)

After this the game will enter a command loop, where letter commands can be input for certain actions to occur:
- 'e \<number of enemies\>': Spawn in enemies at the starting point
- 't \<x-location\> \<y-location\>': Build a tower at the input location
- 'm \<distance\>': Moves enemies the input distance along the path
- 'u \<x-location\> \<y-location\>': Upgrades a tower at the input location, if there is a tower there
- 'a \<attack-times\>': Commands towers to attack enemies in range the input number of times
- 'r \<x-location\> \<y-location\> \<x-offset\> \<y-offset\>': Converts any land tile at the input location and with x and y offset input to be converted to water. (e.g. if x-offset is 2, every 2nd tile in each row will be converted to rain)
- 'f \<flood-times\>': Causes any tiles neighbouring water tiles to be converted to water tiles
- 'c \<x-teleporter-1\> \<y-teleporter-1\> \<x-teleporter-2\> \<y-teleporter-2\>': Builds teleporters at each of the given inputs if they are along the path. Enemies that enter a teleporter will be immediately transported to the other teleporter
