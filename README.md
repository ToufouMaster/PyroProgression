# This is a fork of the official PyroProgression.
I'm Owen, and i'm adding stuff to PyroProgression because Nichiren is not (i love his work)
So my versions are likely going to be discontinued when he will come back, but for now enjoy my additions
(v1.6 and cubemegamod user only: dont forget to download the new CubeMegaMod version that fixes bugs with the current version of PyroProgression
Link here: https://github.com/ToufouMaster/CubeMegaMod/releases/tag/1.5.7-pyro2)

# PyroProgression
A Cube World steam release mod that adds an alpha like XP and levelling playstyle.

## Table of contents
1. [Introduction](#introduction)
2. [Description](#description)
    1. [Features](#features)
3. [Installation](#installation)
4. [Changelog](#changelog)

## Introduction
The idea of adding XP and leveling to Cube World release has been there since the since a few days after the release probably. However, in the past 3 years (since the release), nobody has really attempted to creature such as mod as it involves more that just adding XP and levels. The modding scene was not quite far enough or did not want to put in all the effort to make such a mod. Now, 3 years later, we are finally there. Special thanks to `PyroThunderzz` for convincing me to get started with this mod, enthousiastically bringing in ideas and watching my modding streams. *I guess you have your own mod named after you now...* Big thanks to `S.` and `2 AZ ToufouMaster` for helping me during the development with testing and feedback. On top of that I would like to thank all playtesters of the mod (`CaterpillarCreditUnion`, `Coldurs`, `spenny`, `Shlomopoco`, `Nerah`, `mharr`, `Tabs` and last but definitely not least my man `TheBagel3`), for helping me point out a seemingly infinite amount of bugs.

## Description
This is a mod that adds XP when killing enemies, levels and scaling of the player, enemies and gear. And all that in a hopefully balanced, intuitive and integrated way. I hope to get you as exited as we were making this mod. Note that this mod does not do anything to the existing region lock. It is balanced around being region locked, but you can still have loads of fun with different region lock mods as they are not game breaking.

![Mod](https://i.imgur.com/0zKnyZ0.png)

### Features
- XP is gained based on the level, star rating and type of enemy **if and only if you or your pet kills the creature**. *(This means having the actual last damaging hit)*
- Gaining XP will fill up the XP-bar and your character will Level Up if the treshold is reached.
- On level up, your health will be restored to full.
- On level up, you gain 2 skill points to spend on combat stats.
- Each level gives extra stats on your health, damage, crit, haste, armor, resistance and stamina.
- Each region further away from your starting region *(set automatically)* increases the level of creatures and gear there.
  - The first region has enemies from LV 1 - 5, and each region afterwards has the levels of their creatures increased by 3.
- Gear has a level, independent of star rating, which adds stats. A higher level has a higher chance of being better than a lower level, but this does not have to be the case!
- Gear cost is scaled with region distance.
- Gold bags give 100 gold base gold and is scaled with region distance.
- You can only equip gear that is a maximum of 0 levels higher than your current level.
- Gold drops are scaled by enemy level.
- You can recenter the players region by typing `/recenter` in the chat.
- You can fix the levels of your artifacts by typing '/fixartifacts' in chat.
- Each artifact you obtain increases the number of skill points you can spend by 1.
- You can spend skill points on combat stats, but the more points you put into a skill, the more it will cost.

## Installation
Download the latest version of `PyroProgression_v.x.x.zip` from the latest release from [the release page](https://github.com/thetrueoneshots/PyroProgression/releases). If this `.zip` file includes a `CubeModLoader.fip` you will have to put that file in the same directory as your `cubeworld.exe`. If such a file is not added, you can get the latest modloader release from [the modloader release page](https://github.com/thetrueoneshots/Cube-World-Mod-Launcher/releases). You will have to create a new folder called mods in the folder where your `cubeworld.exe` is located if you do not have one already. You will have to put the `PyroProgression_v.x.x.dll` there. That is all there is to it! Enjoy playing

## Changelog
- `[v.1.6]` Reverted scaling again, skillpoint system and artifact levels.
    - Reverted the XP system back to the v.1.4 version temporaly to avoid NicOz modpack incompatibility.
    - Reverted the v1.5 scaling balance of gears.
    - Added a skillpoint system with combat stat increase as it's core.
    - Added levels to artifacts that increase passive stats like riding speed more.
- `[v.1.5]` Reverted scaling and region names
    - Reverted the XP system back to the v.1.1 version.
    - Increases the scaling cost of items in the shops.
    - (Tried) to balance the scaling of gear more.
    - The big region name in the top right corner now also displays the region level.
- `[v.1.4]` Star rating bug and scaling buffs
    - Bug fixed which caused a higher star and higher level weapon to have worse stats than a lower star and lower level weapon.
    - Lowered XP gain and curve by a factor of 100 for a more appealing scaling.
    - Enemies, player and gear is scaled up.
    - Bug fixed which caused the `/recenter` command to show still show the enemies old health stats. 
- `[v.1.3]` Enemy and player scaling updates
    - Enemy and items range from 1-5 * region distance
    - Player starting stats are lowered
    - Enemy starting stats are lowered
    - Fixed enemy attacking speed bug
- `[v.1.2]` Multiplayer XP sharing and scaling modifications
    - XP is now calculated based on the hosts level and enemy levels and divided between all players
    - Scaling is modified to be non game-breaking
- `[v.1.1]` Level number formatting and recenter command
    - Levels are now formatted as: `xxx`, `x.xxK` and `x.xxM`.
    - With the command `/recenter` you can recenter the scaling region of the player.
- `[v.1.0]` First official release for the PyroProgression mod.
