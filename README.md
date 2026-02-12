# Robotino 4 ROS 2 Bringup

## Overview

This repository provides the launch configuration required to bring up a **Robotino 4 (Festo)** mobile robotic platform within a ROS 2 environment. It initializes the hardware interface, odometry estimation, robot description publication, and onboard sensing modules, establishing the minimal computational substrate required for navigation, perception (with lidar), and higher-level autonomy stacks.

The launch system is designed under a modular robotics architecture paradigm, allowing individual subsystems to be enabled, replaced, or extended without modifying the global execution pipeline.

---

## Network Configuration (Required)

The Robotino platform communicates with the host computer via a **wired local Ethernet connection**.

### Network Parameters

| Device | IP Address | Netmask |
|------|-------------|---------|
Robotino | `172.27.1.1` | `255.255.0.0` |
Laptop | `172.27.1.*` | `255.255.0.0` |

Where `*` must be any unused address except `1`.

Example valid host configuration:

    IP: 172.27.1.10
    Netmask: 255.255.0.0
    Gateway: (leave empty)
    DNS: (leave empty)

---

### `/etc/hosts` Configuration

Both the robot **and** the laptop must contain mutual hostname resolution entries.

Edit on **both machines**:
Add:

    172.27.1.1 robotino (in the laptop)
    172.27.1.10 laptop (in the Robotino)
    
---

*(Replace `172.27.1.10` with your actual laptop IP.)*

---

### Robot Credentials

The Robotino base uses default credentials:

| Field | Value |
|------|------|
Username | `robotino` |
Password | `robotino` |
Hostname | `robotino` |

---

## System Architecture

The launch file instantiates a namespaced runtime environment (`/base`) containing the following subsystems:

| Module | Package | Node | Function |
|------|--------|------|---------|
Base Driver | `base` | `base_node` | Low-level communication with Robotino hardware controller |
Odometry | `base` | `robotino_odometry_node` | Wheel odometry estimation and TF publication |
Kinematics | `robot_state_publisher` | `robot_state_publisher` | Publishes full TF tree from URDF/XACRO |
Joint States | `joint_state_publisher` | `joint_state_publisher` | Publishes joint states (optional) |
LIDAR | `urg_node` | `urg_node_driver` | Hokuyo URG laser scanner driver |

---

## Launch Parameters

The system exposes configurable arguments:

| Argument | Description | Default |
|--------|-------------|--------|
`namespace` | Top-level namespace | `base` |
`tf_prefix` | TF frame prefix | `""` |
`hostname` | Robotino controller IP | `172.27.1.1` |
`rsp_freq` | Robot state publisher frequency | `20 Hz` |
`bumper_timeout` | Emergency stop delay after bumper hit | `2.0 s` |
`motor_timeout` | Motor watchdog timeout | `0.0 s` |
`launch_odom_tf` | Publish odom TF | `true` |
`launch_jsb` | Launch joint state broadcaster | `false` |

---

## Node Descriptions

### Base Driver — `base_node`
Responsible for hardware communication with the Robotino platform. It manages:

- Motor commands
- Bumper feedback
- Hardware watchdog
- Communication with the robot controller via TCP/IP

---

### Odometry — `robotino_odometry_node`
Computes wheel-based odometry and optionally broadcasts: 

    odom → base_link


This transform can be enabled or disabled via launch argument.

---

### Robot State Publisher
Processes the robot kinematic description from a **XACRO model**:

    justina_description/urdf/robotino/robotino.urdf.xacro


It generates the full TF tree required for perception, planning, and control modules.

---

### LIDAR Driver — URG Node
Launches the Hokuyo laser driver configured for:

- Serial port: `/dev/lidar`
- Baudrate: `115200`
- Scan range: ±90°
- Frame: `laser_link`

This sensor provides planar laser scans for navigation and obstacle avoidance stacks.

---

## Required Dependencies

You must manually install required binary dependencies:
    sudo apt install ./*.deb

Download the `.deb` packages from: ['robotino_debs´](https://drive.google.com/drive/u/2/folders/1mw1bWyApip28-f8s3Nak7lKHyuw1cKLb) 

---

## External Repositories

### Robot Description Package
The robot model is provided by ['justina_description'](https://github.com/Biorobotica-UNAM/justina_description)

---

### Hokuyo Driver

You must also install the URG driver package:

    urg_node
    
If not already present:

    sudo apt install ros-<distro>-urg-node

---

## Execution

Run the system with:

    ros2 launch base base_driver.launch.xml


---

## Author: Joshua Martinez

Bio-Robotics Laboratory  
Universidad Nacional Autónoma de México
