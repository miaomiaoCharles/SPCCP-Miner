# SPCCP-Miner

## Introduction

- An interactive system to mine congested junctions, named SPCCP-Miner. This system include data layer, algorithm layer, and display layer. These layers are mainly implemented in /Source/mainwindow.cpp.
- **./Headers:** the header file of SPCCP-Miner, these files stores all predefined functions.
- **./Sources:** the most important files in this system, all functions are implemented in this folder.
- **./Forms:** The interface design scheme of the system is stored in this folder.
- **./Datas:** Some of the data files for testing are stored in this folder.
- **SPCCP-Miner.exe:** The executable program, users can open it to start SPCCP-Miner.

## Usage

- how to download the SPCCP-Miner from this web.

We can click the top right corner of the code->download ZIP to download the zip package, unzip it and **open SPCCP-Miner.exe to start this system**.

![Untitled](Demo%20e654a3d0f2cf4413bc7a69e37f7c0829/Untitled.png)

- The format of data in SPCCP-Miner:

Whether the user inputs the data themselves or reads it from . /data folder, the data should follow the following format:

First, the first few rows show the neighbor relationship between roads. We indicate the road name in the first position of each row, followed by the neighboring road of that road. For example, this row indicate that the road of A has two neighbor roads: C and I. 

```cpp
A C I    
```

After that, we should input the congestion instance of road, which represents the time period  at which road is congested. For example,  this row indicate that road A is congested at first time span, road C is congested at third time span.

```cpp
A1 C3   
```

Finally, we can get a complete data set, which can upload to system to generate comlete and correct SPCCPs.
