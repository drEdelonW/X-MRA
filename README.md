# X-MRA — Hexapod Robotics Platform

> **Experimental / Early Prototype** — active R&D, architecture is stabilizing.

X-MRA is a personal R&D project: a modular six-legged robot designed around
**FPS-style operator control** and a **distributed leg architecture** where each
leg is an autonomous node that owns its own kinematics, compute, and power.

---

## Concept

Most hexapod projects treat legs as dumb servo arrays driven by a central
controller. X-MRA inverts this: the **body choreographer** thinks in
*Cartesian tip positions*, not joint angles. Each **leg ganglion** owns the
IK, knows its own geometry, and answers one question:

> *"Can you place your tip at this point in body space — yes or no?"*

This mirrors how biological motor control works, and makes the system
naturally resilient: losing a leg degrades capability without breaking the
whole.

The choreographer is also responsible for **collision prediction** during gait
animation planning — evaluating waypoints for leg tip trajectories and
confirming with each leg that the planned path is reachable and clear before
committing to the cycle.

### Operator Control Model

Inspired by first-person shooter games:

| Input | Action |
|---|---|
| **WASD** | Body translation in world space, oriented to camera heading |
| **Mouse X/Y** | Azimuth / elevation (course and aim) |
| **Independent** | Body pose and movement direction are decoupled |

This gives omnidirectional movement with intuitive human-familiar control —
no need to think in robot coordinates.

---

## Architecture

```
┌-------------------------------------┐
|           ArachnidBody              |  <- Choreographer / mission logic
|  ┌-------------┐  ┌--------------┐  |
|  | Coordinator |  |Choreographer |  |
|  | (pose math) |  | (gait/anim)  |  |
|  └-------------┘  └--------------┘  |
|         | tip position commands     |
└---------+---------------------------┘
          |  (shared bus — CAN / RS-485, TBD)
    ┌-----┴------------------------------┐
    |  Leg Ganglion x 6  (STM32F4xx)     |
    |  ┌----------┐  ┌----------------┐  |
    |  |  FK / IK |  | Joint drivers  |  |
    |  └----------┘  └----------------┘  |
    |  ┌----------┐  ┌----------------┐  |
    |  |ToF sensor|  |  Local battery |  |
    |  └----------┘  └----------------┘  |
    └------------------------------------┘
```

### Layer breakdown

| Layer | Responsibility |
|---|---|
| `ProtoPWM` | HAL: raw PWM generation |
| `ProtoServo` | Servo abstraction: angle ↔ pulse width |
| `JointBase / ServoJoint` | Joint with angle validation and NaN-skip logic |
| `ArachnidLeg` | FK/IK, tip position in leg-space and body-space |
| `ArachnidBody` | Choreographer: gait, animation, aim, coordinator |
| `robot_spec` | Concrete hardware instantiation (MG996R + PCA9685) |

---

## Key Design Decisions

**NaN as joint-skip sentinel** — passing `JFREEZE = rad(NAN)` to a joint
skips it during `applyPose`, allowing partial pose updates without touching
uninvolved joints.

**Type-safe physical units** — `Angle`, `Millimeters`, `MicroSeconds` are
distinct types. Passing degrees where radians are expected is a compile error,
not a runtime bug.

**`PATTERN_LEG` macro** — gait patterns expressed as bitmasks over leg
indices. Iterating a pattern is one line. Leg subsets (tripod, wave, etc.)
are composable.

**Choreographer thinks in Cartesian space** — `ArachnidBody` never sets
joint angles directly. It sets tip positions; legs solve the IK internally.

---

## Hardware (current prototype)

| Component | Part |
|---|---|
| SBC | Raspberry Pi Zero 2W |
| PWM array | PCA9685 (16-ch, I2C) |
| Servos | MG996R (x18, 3 per leg) |
| Frame | Aluminium kit (AliExpress) |
| Power | Lab PSU -> DC-DC buck (servo rail) + separate digital rail |

**Planned / next iteration:**

- Per-leg STM32F401 ganglion (local IK, local battery)
- Linear actuators with encoders (replacing angular servos)
- ToF sensor (VL53Lxx) on distal phalanx — predictive ground contact
- IMU on body — terrain adaptation and stabilization
- Inter-leg bus: CAN or RS-485 (discovery protocol TBD)
- 8-leg variant for redundancy and load capacity

---

## Kinematics

Three-segment leg: **coxa -> femur -> tibia**

```
FK:  joint angles  ->  tip position (Cartesian)
IK:  tip position  ->  joint angles (analytical, closed-form)
```

Coordinate frames:
- **Leg space** — origin at coxa mount point, X along leg axis
- **Body space** — origin at body center
- `bodyToLeg()` / `legToBody()` handle the transform

IK validity is checked before any motion is applied — the leg reports
`can / cannot` reach the target before committing to movement.

---

## Sensor Integration

### ToF on distal phalanx (TBD)
Instead of blind timed foot-down, the leg will measure distance to surface
in real time during swing phase and transition to stance on proximity threshold.
Result: reliable contact on uneven terrain without terrain maps.
Sensor selection and mounting geometry are not yet finalized.

### IMU on body (mounted, bring-up pending)
IMU is physically installed on the body board but not yet initialized in firmware.
Planned use:
- Horizon stabilization (body-level compensation)
- Heading hold
- Proactive load redistribution before tipping

---

## Operator Interface

All input is handled over **HID** — USB HID, Bluetooth HID, or virtual HID.
This unifies physical gamepads, keyboards, and browser-based interfaces
behind the same input pipeline.

Current input sources:
- USB / Bluetooth gamepad (D-pad and analog sticks)
- Keyboard (WASD + arrow keys) for development and bench testing
- Web interface with virtual on-screen sticks (experimental) —
  intended for phone connection, maps touch input to the same HID event model

Intended control mapping:
- **Left stick / WASD** — body translation in world space, oriented to camera heading
- **Right stick / mouse** — azimuth and elevation (course and aim)
- Gamepad and keyboard inputs are treated as equivalent HID sources by the control layer

---

## Project Status

| Module | Status |
|---|---|
| FK / IK math | ✅ Working |
| Joint abstraction layer | ✅ Working |
| Servo drivers (MG996R / PCA9685) | ✅ Working |
| Body choreographer | 🔧 Early prototype |
| ARM / DISARM sequences | 🔧 In progress |
| Gait patterns (tripod) | 🔧 In progress |
| Distributed leg ganglion (STM32) | 📐 Planned |
| ToF predictive contact | 🔲 TBD — sensor not selected |
| CAN/RS-485 inter-leg bus | 📐 Planned |
| FPS operator control (full) | 📐 Planned |
| 8-leg variant | 📐 Planned |

---

## Build

Requires: `g++`, `make`, Raspberry Pi or Linux host with I2C enabled.

```bash
git clone https://github.com/drEdelonW/X-MRA.git
cd X-MRA/GNU_MAKE

# First build
make

# Clean rebuild
make fresh
```

Target platform: **Raspberry Pi Zero 2W** and higher (ARMv8, Linux).

> GPIO / I2C access requires either root or `gpio` group membership on RPi.

---

## Build Log / Bring-up History

Video documentation of hardware bring-up sessions, tests, and iterations:

▶ [X-MRA Bring-up Playlist on YouTube](https://www.youtube.com/playlist?list=PLBNP-gnOy5nuQffrY3RSSDNDsbqb6kefV)

---

## Background

**X-MRA** stands for **eXperimental Modular Robotic Arachnid**.

Each word carries intent:
- **eXperimental** — this is a research platform, not a product
- **Modular** — legs are self-contained nodes; the system operates
  with whatever legs are currently attached, healthy, and discovered
- **Robotic Arachnid** — eight-legged archetype, though six-legged
  configurations are the current focus

The name also carries a deliberate reference to the **chimera** —
a creature assembled from parts that belong to different wholes.
Legs that think independently. A body that orchestrates without
micromanaging. An operator interface borrowed from first-person games.
A robot that can lose a limb, re-discover its topology, and keep moving.

X-MRA started as a personal exploration of FPS-style robot control.
During the war in Ukraine the underlying architecture influenced real
deployed systems — stabilized weapon platforms and UGV control infrastructure.
The hexapod itself is now being rebuilt in Canada as a platform for
agricultural and industrial autonomy research: terrain navigation where
wheeled robots cannot go.

---

## Roadmap

Near term:
- IMU bring-up and basic horizon stabilization
- Stable tripod gait with real hardware validation (Freenove kit)
- Leg discovery protocol over shared bus (topology-aware choreographer)

Mid term:
- ToF sensor selection and integration on distal phalanx
- Predictive ground contact replacing timed foot-down
- Choreographer collision prediction for leg tip waypoints
- Per-leg STM32 ganglion prototype (local IK, local power)

Long term:
- CAN or RS-485 inter-leg bus with hot-plug discovery
- 8-leg configuration for redundancy and load capacity
- Linear actuators with encoders replacing angular servos
- Agricultural / industrial terrain autonomy (vineyards, terraced fields,
  underground environments)

---

## License

MIT — do whatever you want, attribution appreciated.

---

*dr.Edelon — Saskatoon, SK, Canada*
