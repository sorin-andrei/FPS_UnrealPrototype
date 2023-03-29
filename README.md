# Unreal Action Shooter

A FPS shooter prototype inspired by Max Payne. The player is able to pick up weapons, dual-wield, and perform actions that slow down time. Check out example video: 

[![Example Video](https://img.youtube.com/vi/fS1mVGM-Xk0/0.jpg)](https://www.youtube.com/watch?v=fS1mVGM-Xk0)

The player can pick up and mix and match the following weapons

* Pistol - Single fire system
* Machine Gun - Continuous firing system, if the player holds the left mouse button, the weapon will keep firing
* Shotgun - Functionally similar to the pistol, but with multiple hitscans 

These are all children of a main Weapon class, that has multiple parameters, like firing type (Automatic/Single fire), firing delay, damage, number of hitscans, recoil, ammo capacity and more. This allows to easily create and configure new weapons.

The player can press the jump button in order to perform a dive and slow down time

The player can press the crouch button to slide into enemies and damage them

These player actions are stored into the ActionComponent, and are Actor independent. This means that you can easily attach this component to any actor (plug and play), which will enable these abilities.

[![Example Video](https://img.youtube.com/vi/5uZ1TJ-ximA/0.jpg)](https://www.youtube.com/watch?v=5uZ1TJ-ximA)

