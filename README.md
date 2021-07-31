# SelectionPluginUE4

![](Resources/SelectionDemo.gif)

C++ plugin for Unreal Engine 4.25 that allows single and rectangle selection

## What is included

* C++ code for
  * USelectionComponent that can be attached to anything derived from APawn to store selection state and display selection cirle
  * USelectionManagerComponent that should be attached to AGameMode to keep track of all USelectionComponents
  * USelectionHUDComponent that should be added and wired a bit into AHUD to display selection rectangle
* Assets
  * Material for selection circle made according to this great [tutorial](https://www.youtube.com/watch?v=uwi6UDiNNX8)

## Install

* Clone this repository into Plugins folder in your UE4 project
* Right click your *.uproject file to generate Visual Studio project files
* Rebuild solution

## Use

* Create something that inherits from APawn. For example blueprint BP_Pawn
* Add to BP_Pawn USelectionComponent
* Adjust USelectionComponent properties
  * Set `Plane` as `Static Mesh`
  * Set `MAT_SelectionCirle` as `M Material`
  * Adjust scale and position USelectionComponent to match your pawn 
  * Adjust colors
* Add USelectionManagerComponent to AGameMode instance
* Adjust USelectionManagerComponent properties
  * Select keys for selection rectangle
  * Set selection properties related to parameters passed into `AHUD::GetActorsInSelectionRectangle`
* Add USelectionHUDComponent to AHUD that your AGameMode isntance uses
* In blueprint of AHUD in event graph add `Event Receive Draw Hud` and call USelectionHUDComponent::ReceiveDrawHUD

## Don't forget 
* To enable cursor in your PlayerController
* If PlayerController rotates when you use selection rectangle set `USelectionManagerComponent::m_BlockControllerRotationWhenModifierIsPressed` to true
* Set your GameMode as current one in World Settings and add to it your PlayerController and HUD
* Fact that by default selection rectangle apprears only when `USelectionManagerComponent::m_MouseKey` (Left Mouse Button) and `USelectionManagerComponent::m_ModifierKey` (Alt) are pressed
* Fact that by default to add more actors to selection or to exclude actor from selection `USelectionManagerComponent::m_AdditiveSelectionKey` (Left Shift) should be pressed

