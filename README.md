# Metroidvania Prototype

> **Note:** This is a source-only snapshot of an older Unreal Engine project.
>
> I removed the original `Content/` directory before making the repo public because it contained binary game assets and other project content I don't want to redistribute. That means the project won't run as-is if you clone it.
>
> The original C++ development history is still here.

I love Metroidvanias and 2D action games, and this project was basically an excuse to dig into how some of those systems actually work.

I spent most of my time experimenting with player movement, camera behavior, combat, enemies, items, and Unreal's Gameplay Ability System. A few parts of it were especially fun to build, particularly the room-aware 2.5D camera and the combat/hit-detection pieces.

## A few interesting bits

### 2.5D camera system

One of the parts I had the most fun with was the camera.

The player is constrained to a 2D movement plane, while the camera sits on a spring arm and adjusts itself based on what the player is doing.

It can lead ahead while you're moving, shift vertically when looking up or ducking, and stay inside room-specific camera bounds instead of blindly following the player everywhere.

The camera offset is interpolated independently by axis, which let me control how quickly it responds in different situations and made the movement feel a lot less rigid.

Each room can define its own camera bounds, and I added debug drawing so I could see both the room limits and the inner area where the camera was free to move.

Relevant code:

- `Source/MetroidVania/Characters/Player/MVPlayerCharacter.cpp`
- `Source/MetroidVania/Gameplay/MVRoomCameraBounds.cpp`

### Combat and hit detection

Weapons have their own collision component, but the hitbox isn't active all the time.

Instead, collision can be switched on and off around an attack so hits only register during the part of the action where the weapon should actually be dangerous.

When something overlaps the hitbox, the weapon checks whether it's a character and then routes that into the weapon's damage behavior.

The weapon system also hooks into Unreal's Gameplay Ability System, including:

- weapon-owned ability-system components
- attributes and gameplay effects
- abilities granted by equipped weapons
- player ability input

Relevant code:

- `Source/MetroidVania/Items/MVWeapon.cpp`
- `Source/MetroidVania/Characters/Abilities/`

### Enemy behavior

I spent some time experimenting with enemy behavior as well.

One of the enemies was a wolf with its own movement and attack behavior, including a jump attack. Some of that logic lived in Unreal Behavior Trees and Blueprint assets that were part of the removed `Content/` directory, so not all of it is visible in this source-only version.

The original development history is still preserved, including the work around building and finishing that enemy behavior.

### Character movement and abilities

The player character includes a bunch of smaller platformer-style experiments, including:

- constrained 2.5D movement
- double jumping
- crouching and looking up
- weapon equipping
- Gameplay Ability System integration
- camera behavior driven by player state

A lot of this project was me learning how Unreal wanted these systems to fit together while also trying to keep as much of the gameplay logic in C++ as possible.
