# Defragr

Precision movement FPS ported off the Quake 3 series of movement physics, most notibly the Defrag and CPMA mods for Quake 3. Defragr includes a near direct port of idTech3's physics for the player.

## Current features

* CPMA Strafe jumping
* Wall strafing
* Down ramp boosting
* Teleport volumes
* Checkpoint system and volumes
* Jump pads

## Planned features

* Weapons
* Pickups
* Multiplayer
* Editor
* Steam integration

## Creating Custom Maps

You can create your own custom maps using the Unreal Editor, keep in mind though that Defragr is still in development and a lot of CPMA mechanics have not been implimented and some could even change completely (such as jump height / gravity). So be careful when making custom maps because if one mechanic changes your map may become unplayable.

If you are a brave soul who wishes to embark on creating custom maps then there is a separate 

## Testing

Considering this is a game, and cannot be machine tested, testing the mechanics is rather convoluted to ensure the mechanics remain accurate to CPMA. However if you do make changes to the physics of the player there is a scene called `dev_testing_grounds` in which has obstacles that the player can be tested against to make sure the changes to the player don't break anything else.

## Developing

### Support

Open a ticket [here](https://github.com/Zinglish/Defragr/issues) or if you want to ask a couple of questions, see the Contact section just below.

### Contributing

Fork the code then place a pull request [here](https://github.com/Zinglish/Defragr/pulls). Ensure that the code isn't specific towards a mod and/or a fork. Pull requests are usually dealt with within 4 days.

### Contact

My direct point of contact would be on the Slack channel [Zinglish Does Stuff](https://zinglishdoesstuff.slack.com). I am mostly online however if you cannot catch me there I am available at almost all times via email: zinglish[at]gmail.com.

### Branching

* Master: a fully working copy of the game. Simply download and compile/load in editor.
* Development: mainly for myself, it contains the most up to date code working or not.
* Nightly: contains all code pushes for the day. This code could contains bugs so beware.

## Donating

[![Image](https://img.shields.io/badge/Donate-Paypal-brightgreen.svg "Donate with Paypal") ](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HWBNT3M2XDCPL "Paypal")

I work on this project by myself in my spare time as I have a full time job. If you like my work and/or use it in other projects please consider donating using a badge above or at the very least Staring [Defragr](https://github.com/Zinglish/Defragr) on Github.

## License

This project supports the GNU v3.0 license.