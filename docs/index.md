---
layout: home
---

# A GUI for control & supervising multiple robots remotely

## Team
-  E/15/140, Jaliyagoda A.J.N.M., [email](mailto:nuwanjaliyagoda@eng.pdn.ac.lk)
-  E/15/173, Karunarathne S.D.D.D, [email](mailto:dinelkadilshani95@gmail.com)
-  E/15/350, Tennakoon T.M.P.B., [email](mailto:pasan96tennakoon@gmail.com )

<div class="container p-2 mb-4">
    <img class="img-fluid" src="img_cover.jpg" style="width:100%" />
</div>

## Table of Contents
1. [Introduction](#introduction)
2. [Solution Architecture](#solution-architecture )
3. [Hardware & Software Designs](#hardware-and-software-designs)
4. [Testing](#testing)
5. [Detailed budget](#detailed-budget)
6. [Conclusion](#conclusion)
7. [Links](#links)

---

## Introduction

This idea came from a project that is related to swarm intelligence. According to the definition, "Swarm intelligence is the collective behavior of decentralized, self-organized systems"  Swarm robotics is applying swarm intelligence to accomplish a bigger task. And it also similar to the behavior of animals like bees, ants, birds, etc.

One of the greatest fallbacks of swarm intelligence-related research is that it is difficult to simulate the algorithms in the real world unless you have a large number of robots to test these algorithms. Building a group of robots takes a lot of time and it is very expensive. As a solution to this problem, we can design or buy general purpose robots which have hardware capabilities to run basic swarm intelligence related algorithms. But buying a set of pre-built robots doesn’t solve the whole problem since it is too expensive yet.

The final goal of this project is not only to design a general-purpose swarm robot unit but designing the simulation platform too. This simulation platform will be able to control basic functionalities of the robots such as assign a robot into a defined place, recharge the robot’s battery when it is draining out, program the robots with giving algorithms, etc… This simulation arena can be accessed from a remote location and these remote users can upload their own algorithms into the robots which are placed in the arena. After upload, they can run it on robots and see the response of the robots using Data and Video feedbacks. Research teams who are working in the field of Swarm Robotics can test their algorithms without taking much effort into hardware. So it saves their time and money.


## Solution Architecture

### Identifying the scope

Since this whole project is beyond our scope for this unified project we are planning to do only a part of the main project. We hope to create a simulation platform for these robots and control and monitor a few parameters using a remote GUI (Graphical User Interface).

Our goal is to develop a GUI that a user can select robots and specify locations for them in the simulation arena. Then robots will be moved to these specified locations within the smallest possible time by avoiding collisions with other robots.

![image](https://lh3.googleusercontent.com/oVs53LnyXGMfnh-y_nBgmU_YDACVA7Tfymeu5Z4-h4BzeH0SctfKRm3PIrBaxXoScG0rKwPC0k3C0nGl9zy3bpGmmuWg_Sjklvr9w44oo_zRZyuC8v9EMA4h4f0r2Ajm3Mph04vN)

![image](https://lh5.googleusercontent.com/HTSKR140nti-RIpG1k3usUMKRCTb-mH50cKifNEvgWXlKaw7pq35MpgWZDGYpeNC23EOrKrIwBz7G5I1dGtqC2WPUs2P5Cvotvz2DXyK-ltj41uRV4tYFZDIqMvFZ_dpElQSJPlK)

This project is not only limited to swarm intelligence based problems. We can use what we will develop to increase the efficiency of the real world problems like Search and Rescue Missions, Bomb detection, Planetary exploration, etc. Common facts of these problems are that there is a group or robots which are located in a geographical area and the person who needs controls them from a remote location.

### Identifying the requirements

Since we need to remotely monitor and control robots, it is required to establish a communication link between the simulation arena and the end user in real time. This communication link should be able to send and receive messages in both directions

Another requirement is that robots should be able to move into a specified location of the arena. In this case, the robot should be able to identify its current coordinates and the distance of the travel precisely.

When a large number of robots moving in a limited area, there is a high probability to be collisions between robots. We need to find a suitable mechanism to avoid such kind of situations.

We are allowing remote users (through the internet) to control and monitor the robots, so it is compulsory to think about the security of the network.

### Suggested Data Communication Flow

![image](https://lh6.googleusercontent.com/rRBsPnq2q08RItJrOOUl24NToVQVab7-NkRbkIyj4s2Dx_hV7DAXa7t3Pksz_oK9A_Jw1aXZMhqJ2C3rawJe50007GJIUuaQyX9U5smtsw2jh8ZCtYFD4IZ13xs9jmr4aT7lEQR6)

## Hardware and Software Designs

### Communication Protocol between Robot Server and End User

 When finding a solution for our project, we found a few challenging points. Since we hope to remotely monitor and control ‘multiple’ robots, we wanted to have a real-time communication method between our end nodes (in this case, robots) and the client (in this case, human users) The connection should be able to send control signals from user to robots as  well as response messages from the robots.

We first considered RESTful API based server-client architecture, but it was rejected due to it doesn’t support full duplex communication. Next, we looked into use a MQTT Broker between our robots and end user. MQTT (Message Queue Telemetry Transport) was originally developed for low power IoT devices. It is a topic based on communication. Nodes can subscribe to topics, and publish into topics. When a device published data into a topic, MQTT broker will inform it to all the devices/nodes which were subscribed into that topic.

![image](https://lh3.googleusercontent.com/9x6lqOtQcuyfx_6ZljvprX_f4Ge40mra2HzxUduWg-hu4hS8ShRbw7wYclEv-_xNDRe1EB3oPKN_xb6LzMcuHXmNh7B_2XmCMXr-5klSxbE-yVyi3BhI5VFKWT9bFoAnkaIQtm7j)

We found another communication protocol known as WebRTC. It is a free and open source project which is developed for communication between browsers and mobile applications in real time. WebRTC is a plugin-free API that most of the modern web browsers support. It has multiple standards and protocols, including STUN/TURN servers, signaling, JSEP, ICE, SIP, SDP, NAT, UDP/TCP, network sockets, and more.

Generally, WebRTC is designed for stream Video and Audio, but there is a channel for data/media stream too. One of our requirements is to remotely monitor the robots and that includes both video and data. So we finally decided to use WebRTC as our communication method.

You can learn more about WebRTC from following links.

- (WebRTC : Official Page)[https://webrtc.org/]
- (What Is WebRTC and How Does It Work?)[https://www.innoarchitech.com/blog/what-is-webrtc-and-how-does-it-work]

### Data Communication between Robots and Robot Server

We decided to use WiFi as the default communication method between our robots and the robot server. The main reason to choose WiFi is that we can implement two-way communication for multiple channels with the minimum hardware cost. And can be used in various communication protocols, which are developed on top of the basic WiFi protocols such as *IEEE 802.11*

### Robot Navigation Control

Since our robots will be controlled remotely, robots should be able to handle incoming commands while navigating. Due to the practical hardware problems, robots can’t move on straight directions and take precise turns without feedback loop based control structures. Most of the available sensors that measure the distance have errors and when we continually taking measurements, it will add cumulative errors. So we decided to avoid this by doing a small modification to our platform.

We decided to use a black color grid on the arena, so robots can follow the lines and correct the cumulating errors from the junctions when it is passing them. Not only that, it can use this grid system to take 90 degree turns using floor color sensors and a simple feedback loop.
        
![image](https://lh4.googleusercontent.com/mhdhV3hEShwdRqkrXKJOVsXaxmmWWyCQHZofo7Zj35E_DdeiMdjlKG7Mn-LlOFKhjNYIWzLixrZDM_8DaWyapz_MdR8cNnKjW9sR_jM90yWlsa-1CIvpPeHAQtQFOmFh432sj9rJ)
        
To identify the black lines from the white background, we decided to use IR Transmitter Receiver pairs. Principle of this sensor is that white color background reflects the IR beam emitted by the IR diode, but not by the black color background. The voltage output of the Photodiode will depend on the amount of reflection. Currently, we hope to feed this digital signal into our microcontroller as an external hardware interrupt, rather than polling digital inputs. So we decided to use an Op-amp circuit with Voltage comparator arrangement to convert this analog reading into a digital signal.

![image](https://lh4.googleusercontent.com/EXu9kh1dxkxzQ8bcB-Njhr9LQSx-P5mUyWWhtXjO-MgJjJWiMSMKktbMKIfO0yp9lKArK-De4BbKHmYxV3gcwJPdpz3bvOh3apgT4h78ilQe-biOC84FtE5_2nsfmE6VvaLH_Imr)

The comparator is an electronic decision-making circuit that makes use of an operational amplifier very high gain in its open-loop state, that is, there is no feedback resistor. The Op-amp comparator compares one analog voltage level with another analog voltage level, or some preset reference voltage, VREF and produces an output signal based on this voltage comparison. In other words, the op-amp voltage comparator compares the magnitudes of two voltage inputs and determines which is the largest of the two.

![image](https://lh3.googleusercontent.com/CINvns1QH_nKd1ro3jkafmwWWnVos8s7Fe2F9hyBVfAB1HgzJ4BiOhEykjj391OCWpbpM6vTLlwxAbKh4hBgM5oJQkNy81fEcPeuvZsCIZuttCUyQ4bCF71J-50sGikOQ-xb4vV3)

We have designed the comparator circuit using the IC named, LM324. The operational amplifier LM324 IC can work like a normal comparator, and it comprises four independent op-amps internally. This IC has designed with low-power, bandwidth and high stability for operating with single power supply over extensive voltage ranges. The range of operating voltages of this IC includes 3.0 V for low and 32 V for high. The range of common mode input mainly comprises the negative voltage supply, thus removing the requirement of outside biasing components in several applications. The range of output voltage also comprises the negative voltage supply.

![image](https://lh3.googleusercontent.com/8fSXVzziB57ADIE0zwsXcgJ-pBvAZ5-yPyb1KmiMIwY81d7ZUFg8i32k7pn_JLdiPVW3SQKZuWjuuyfsFRsXgNg78Jl9Bn8mT93k8LnW2fYtrDmmDOUWseI8MAajT5goItYQb7Q8)

Voltage Reference for the comparator circuit is provided by a Multi-turn trimming resistor because it can be configured as a voltage divider more precisely than a usual Preset Resistor.

Here are the Schematic Diagram and the PCB layout of the comparator circuit we have designed. We used an open-source software called Fritzing to design the PCB. Circuit board was fabricated using the PCB milling method, which engraves and isolate the circuit paths on copper plate.

![image](https://lh5.googleusercontent.com/ovJSo-kEye-_iiR0aFCcqpqjzZQmwCYR9UEamIM4JaoYVP6gnD4xagnz3zxEHP6EPfbYqF0VchH0SAEzSpzjqekaBfSuX3BLOuvDYaUUyI1tFU_BRuXIEhv06Jc66Vhg7YjC6ayj)

### Overview of the PCB Design

To design the PCB, we used free and open-source software named Fritzing. The PCB is a single layer design and we used jumper cables to avoid crossed signal paths. To fabricate the PCB, the design was exported as a Gerber file and processed with the software named dipTrace.

![image](https://lh6.googleusercontent.com/4-Irggu_mr5OnIZ5ieTZn3zTe9eDO4L4mUO8bhxl-gZexeft33459Q3VjctsPpMWl3bqzo0MpMQFwi86A1XJQ8F_fu-RmyydBDe9pJOl5Te83A6B4NToMuo3KxOvgfaLpRujeQcJ)

![image](https://lh5.googleusercontent.com/6WjtHTVayS64AumnLSL4jOlXGqhSXSrUUmhrN_PIG79Kb1cjrzLGyXBIX4ZTQ49rbEhDUGQ0SEXt-8f4zb_whaWrNnqZgcBxI5-o8J5fVQn7EEDsuh_GYOEt3MDMJtXvQdiB_PHn)

After processing the Gerber file, it was imported into a software named, FlatCAM to convert the PCB design into a toolpath for machinery. The G-Code files which exported from the FlatCam were sent to the CNC milling machine, and the machine fabricated the PCB by few isolating milling and drilling cycles.

After two and a half hours of soldering, we were able to completely assemble the PCB as shown below. (We used a ready-made power supply known as a  DC to DC Buck converter to reduce the battery voltage of 8.4v to 5v and it is the green color module you can see in below image)

![image](https://lh4.googleusercontent.com/Mq-q2uFwNLxhVQJUC_KXp3zNKBjYDcfbHHSyd5cyw-oMnDsl01lp-rWKFSTMw4vjLKhxFfvq8hU8Vtmkw4pk68XrPYU0wvUZx4Ebg8m8jRRxvrNQGj0Xndrp26hpbWZaJCVnumWO)

### Overview of the Robot Design

We have used SolidWorks for designing the structure of the robot. After a few design revisions, we came up with the following design. We decided to use a round shape for the robot.

![image](https://lh4.googleusercontent.com/h_qCNbKT042jw0_-sLOCKr6Wg2V6C4PqwdVWWeUILBKw8MmnqJTaV6HBhVFb1_kvUHlr0EeswL7_KBv3Gs8ttlxiuK020IGxXrMbxSTumAnSVFydYofzyfLKaJdQ_CgELLZjbV6I)

We used a black 3mm Cladding Board as the raw material of the base. The design was fabricated by CNC milling, using a 1.5mm end milling bit with a contour milling operation. Wheels also manufactured using the same material and using the same method.

We used 5mm x 60mm Hex bolts, M5 nuts, and M5 washers as the spacers between two base plates and the PCB layer.

![image](https://lh5.googleusercontent.com/MWNT_epQUupunAG5jsEVQvMhUQ1dX78HkFA2OuSbA7vew0X0Qbfv6asgYTjffmIuAKyO9odLy59deUh8ik0B32umTrkjB-f_dL5qx8WWF9F0q_f1cgtdIw-Kk8Dps-bF5zm8SvsP)

![image](https://lh3.googleusercontent.com/Y9o757NppvhQgalAO7L45qGfM4xlQ0CSbULiWl_dsgsvTIt9wBT3CafKY1ISfXWpbfczqssS3fvSZ9zoxM-e5ld5gm7TJSOa-GJ2P3OXtHiKtTe2jB_hI7Ys_AbYA5QKalV2LgQT)

![image](https://lh6.googleusercontent.com/8krYIG5PGphWnT7aW11lJU67qWQj96g91JZwe1wzMzniYRw-C5twiv4VnTjjvGC4iaoTd3UFRrAuk6oaDWhmu64hgcwIW1_vJXkjDzXbDAf0uTAxxAs5D6VwKZi-N6-szjDT9UQ3)


## Testing

 testing done on hardware and software, detailed + summarized results

## Detailed budget

[//]: # (all items and costs

| Item          | Quantity  | Unit Cost  | Total  |
| ------------- |:---------:|:----------:|-------:|
| Sample item   | 5         | 10 LKR     | 50 LKR |

## Conclusion

 what was achieved, future developments, commercialization plans

## Links

- [Project Repository](https://github.com/cepdnaclk/repository-name)
- [Project Page](https://cepdnaclk.github.io/repository-name)
- [Department of Computer Engineering](http://www.ce.pdn.ac.lk/)
- [University of Peradeniya](https://eng.pdn.ac.lk/)

- [Custom Page](page_1)

[//]: # (Please refer this to learn more about Markdown syntax)
[//]: # (https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
