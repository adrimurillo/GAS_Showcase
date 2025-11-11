# Unreal Engine 5 – Gameplay Ability System Demo

A technical showcase built to explore Unreal Engine’s **Gameplay Ability System (GAS)**.  

---

## Abilities Overview

| Ability | Description | Technical Focus |
|----------|--------------|-----------------|
| **Dash** | Short burst of directional movement | Root motion, cooldown, and activation conditions |
| **Fireball** | Aimed projectile that explodes on impact, leaving a persistent AoE damage field | Targeting, projectile spawning, GameplayCue feedback |
| **Flash Grenade** | Temporarily blinds enemies within radius | Targeting, projectile spawning |
| **KAY/O-style Ultimate (Valorant)** | Suprpresses and blocks abilities via tags | Tag-driven ability suppression triggered by expanding periodic pulses |
| **Healing Zone** | Regenerates health of allies standing inside the area | Periodic GameplayEffect and area overlap detection and attribute updates |
| **Damage Zone** | Damages actors continuously while inside the area | Periodic GameplayEffect and area overlap detection and attribute updates |
| **Root Zone** | Roots players who remain inside the area for too long | Conditional effect trigger based on duration and tags |
| **Slow Zone + Damage if moving** | Slows down actors within the area and continuously damages them if they move | Movement Checks with Tags and conditional effects |
| **Weapon Manager + EquipWeapon Ability** | System for equipping and unequipping weapons via a dedicated ability | Ability-driven weapon switching and replicated equipment state |

---

## Integrated UI
The UI displays information provided by GAS:
- Health
- Energy
- Abilities
- Cooldowns
- Key Bindings
- Ability States

---

## Demonstration

<div align="center">
<table>
  <tr>
    <td align="center">
      <img src="media/Dash.gif" width="450"><br>
      <sub>Dash ability</sub>
    </td>
    <td align="center">
      <img src="media/Fireball.gif" width="450"><br>
      <sub>Fireball ability</sub>
    </td>
  </tr>

  <tr>
    <td align="center">
      <img src="media/Flash.gif" width="450"><br>
      <sub>Flash ability</sub>
    </td>
     <td align="center">
      <img src="media/Silence.gif" width="450"><br>
       <sub>Silence ability</sub>
    </td>
  </tr>

  <tr>
    <td align="center">
      <img src="media/Root.gif" width="450"><br>
      <sub>Root ability</sub>
    </td>
    <td align="center">
      <img src="media/SlowDamage.gif" width="450"><br>
      <sub>Slow and damage Zone</sub>
    </td>
  </tr>

  <tr>
    <td align="center">
      <img src="media/HealZone.gif" width="450"><br>
      <sub>Heal Zone</sub>
    </td>
    <td align="center">
      <img src="media/Replication.gif" width="450"><br>
      <sub>Replicated abilities</sub>
    </td>
  </tr>

  <tr>
    <td align="center">
      <img src="media/Overview.png" width="450"><br>
      <sub>UI overview</sub>
    </td>
    <td align="center">
      <img src="media/Active&amp;CdAbilities.png" width="450"><br>
      <sub>UI active abilities and abilities in cooldown</sub>
    </td>
  </tr>
</table>
</div>

<p align="center"><sub>Examples of ability activation, area effects, and GameplayCue feedback.</sub></p>

---

## Technical Highlights

- Modular ability setup supporting easy extension
- Attribute-driven combat system with instant and periodic effects
- Tag-based gating and conditional logic
- Root Motion integration for movement abilities
- Full multiplayer replication

---

## Technologies Used

- Unreal Engine 5.6
- C++
- Gameplay Ability System (GAS)
- GameplayTags / GameplayCues / AttributeSets
- Niagara (for simple placeholder VFX)

---

## Purpose

This project was created as a **technical exploration of GAS** — focusing on gameplay systems architecture, scalability, and replication.

---

### Notes

Code examples available inside the `/Source/GASRPG/` folder.
