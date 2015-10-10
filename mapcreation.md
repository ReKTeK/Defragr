# Map Creation for Defragr

This document will go over how to produce a map for Defragr, what you need in order to make a playable map and some caveats that are involved.

## Requirements

### Start

* Unreal Editor (UE4).
* Defragr Source / Binary.
* Patience.

### Level

You mostly just need a generic UE4 player spawn. To add this just add the `Player Start` class to your map and magic, the player should spawn when you press play. The level should also have at minimum a start and finish checkpoint. To create these simply drag and drop 2 checkpoints (custom made classes) on to the scene, size them like you would with a BSP and set one of them to be a starting checkpoint and the other to be an ending checkpoint.

## Tools / Classes

This section will cover the custom elements that come with Defragr.

### Checkpoints

Checkpoints can either be a starting checkpoint, split checkpoint or ending checkpoint. These can be set after placing a checkpoint into the scene then selecting a type from the drop down in the details view.

Note: You CANNOT chain or connect checkpoints together to make a shape that has more than 6 sides at this point in time. You CAN however utilize the geometry editor to create more complicated shapes than just a cuboid.

* Start checkpoints will trigger the lap time when the player exits its area. It's recommended that you have only one of these in your level, having more than one will not have any effect on gameplay.
* Split checkpoints will tell the player what their split time is once they pass through it.
* End checkpoints are basically the finish line. You can have more than one in your scene. Passing through more than one will not affect the time of the lap.

### Teleports

Teleport Volumes act as a catch for the player, if a player passes through one of these they will be teleported to a Teleport Target specified in the teleport volume they pass through. Teleport volumes act as BSPs and the geometry editor can be used to create more complicated shapes other than the standard cuboid.