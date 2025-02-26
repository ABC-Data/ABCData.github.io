CLANDESTINE STUDIOS
CSD2450
DERELICT LAND
MILESTONE 6

--------------------------------------------------------------------------------------------------------------------------------------------
Technical:
--------------------------------------------------------------------------------------------------------------------------------------------
If you opened the game via Visual Studio in Debug mode:
To play the game (without editor mode), press F5. If you want to go back to editor mode, press F5 again.
In release mode, the application is automatically in play mode, and the editor entirely hidden.
To toggle fullscreen, press F11. It may require more than 1 press under some conditions.
To see toggle the on-screen debug info, you can press the ` key.

!!!IMPORTANT!!!
IF YOU ARE PLAYING ON A LAPTOP, PLEASE USE FN+[F-KEY] INSTEAD OF JUST THE F-KEY OR YOU WILL CRASH!
DO NOT TEST GAMEPLAY ON PASCAL COMPUTERS, THEY ARE NOT ABLE TO PROPERLY DISPLAY THE GAME'S EFFECTS!
For gameplay, please use Tesla PCs, one tested one is DIT0958SG (Middle block, 3rd row from front, 2nd right most when facing whiteboard)

You must run the game at 60 FPS. If it is not already doing so, you can either enable VSync (for 60Hz monitors) and/or set the max frame rate of the application.
For Nvidia cards, this can be done in the Nvidia Control Panel.

--------------------------------------------------------------------------------------------------------------------------------------------
Game Concept:
--------------------------------------------------------------------------------------------------------------------------------------------

A 2D post-apocalyptic side-scrolling game with turn-based combat & resource management mechanics.
Set in Singapore in 2040, you play as a scavenger named Yu. You are tasked to find a way out of the wasteland of Singapore and to find a new home.

--------------------------------------------------------------------------------------------------------------------------------------------
Gameplay Controls, Usage
--------------------------------------------------------------------------------------------------------------------------------------------
LMB - Player Movement, Clicking on Objects in Combat mode, Clicking on UI elements, Picking up items in inventory
RMB - Discard items (that are picked up)
I - Alternative key to toggle inventory
E - Interact with Toggle Markers, progress through dialogue. Can be used to skip main menu animation, intro cutscene, skip dialogue animation
1,2, etc - Use inventory items to attack, heal, etc.
Esc - Alternative key to toggle pause menu

--------------------------------------------------------------------------------------------------------------------------------------------
How to Play, Solve
--------------------------------------------------------------------------------------------------------------------------------------------
Starting the game at the Main Menu, there are options to start or quit the game. Volume controls are also present.

Click around to move the player. Interact with the toggle marker to get items.
Said items will appear in your inventory, which can accessed by clicking on the bag icon in the top right corner of
the main screen. Right next to it is the pause button, which will enable you to the pause screen containing the How To
Play screen and the Quit Game functions.

In the current version the game, the second toggle marker will contain an encounter with an enemy.
There will be a dialogue sequence, progressed thorugh by pressing the E key. Currently, the dialogue is not skippable.
At the end, there is a transition to a black screen, before going to the Combat scene.

When encountering an enemy, you will enter combat mode.
During combat mode, your inventory will be shown at the bottom. To the bottom left, a red bar representing Yu's Health Points (HP), and a green bar 
representing his Action Points (AP).

Action Points will start accruing over time. Click on a weapon to use it, spending AP in the process. Mouse over the weapon to look at it's tooltip.
This will deal an amount of damage to the enemy, with a chance of landing a Critical hit, which will deal additional damage. If you try to do too many actions 
you will not be able to as you do not have enough AP. 

Clicking on Consumables will trigger their effect. In the tutorial level, you should have a Lion Balm, which gives you an instant heal.

The enemy will fight back and deal damage to you at regular intervals.
Additionally, it may instead boost its resistance to damage.
You can opt to block any oncoming attacks from the enemy.

Eventually, you will be able to kill the enemy and exit combat mode. Depending on the outcome, a Victory / Defeat
screen is shown, showing the loot you've gained and placed into your inventory on killing the enemy, as well as
some damage-related statistics.

After defeating the enemy, continuing through the level to the end of the pier, you will transition into the Hub scene.
There is a short tutorial about what you can do in the Hub - crafting, cooking, and heading into other areas.

Moving the player to a close proximity to the Crafting Anvil, Cooking Station, or the Quest Board will cause it to continuously fade in and fade out, indicating that it can be interacted with. 

Crafting requires a few various materials to craft out an item. Select an item to craft by clicking its icon on the left UI will show the item's name, icon, description and crafting material requirements. If there are enough materials, it will show "You can craft this item" in green and the craft button will be lighted up. Pressing craft will cause the item to be added into the inventory. 
(To try this, close the crafting menu by pressing E, and press the Z key 3 times. This should add the item into the inventory. Interact with the crafting anvil again, and selecting the makeshift flamethrower should now be able to be crafted and added into the inventory.)
Cooking is similar to crafting, except that you will need to play a little minigame to successfully create the item. A ticker will move from left to right. Press the Cook button when the ticker is in the green zone in the middle to successfully cook.

When interacting with the quest board, you can select where to go. Select Area 1, and the quest description should show. Pressing Start will then bring the player into the next scene.
Area 1 is more than just a single map. You will encounter arrows prompting you to Press E along the way, which leads you to different sub-areas. These areas have different monster and item spawns, allowing you to get different items. Beware, as the deeper you go, the stronger the enemies get. There is only 1 way back to the hub, which is at the beginning of the first area, so if you venture far, expect to backtrack the same amount to get back safely.

After getting enough materials, you will be able to go to the Pier. This is the endgame, and going to the Pier will end the game with a short sequence of events.
--------------------------------------------------------------------------------------------------------------------------------------------
Existing Bugs
--------------------------------------------------------------------------------------------------------------------------------------------

If you are running the game at above 60 FPS, the following will occur:
Clicking around in the game (LMB) may take multiple clicks to register.
Clicking "Continue" in the Victory screen of combat may take multiple presses.
Animations (namely, their positions) will seem off.

On some machines, the scene transition animation does not play. Please use a different machine (See above for some instructions)

--------------------------------------------------------------------------------------------------------------------------------------------
Cheats
--------------------------------------------------------------------------------------------------------------------------------------------
Key - Effect
M - Toggle speed (double/normal speed)
G - Spawns an Insta-Kill Godsword weapon.
H - Fully restores HP & AP
L - Sets your HP to 1 (For testing losing)
Z - Spawns 1 Metal Pieces item
R - Returns to Hub (note that this is a forced action, so won't look smooth when doing so)
P - Spawns the required items to unlock the pier (ending)