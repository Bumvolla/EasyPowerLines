<a href="https://www.unrealengine.com/">![Unreal Engine](https://img.shields.io/badge/Unreal-5.1%2B-dea309)</a>
<img width="1920" height="1080" alt="FabBannerEPL" src="https://github.com/user-attachments/assets/6e7aa775-1c52-4a9c-a48a-cb7be98a4c2a" />

<p align="center">
  <a href="https://bumvolla.github.io/2025/02/20/EasyPowerLinesDocs/"><strong>Docs</strong></a>
  <br/><br/>
  <b>Unreal plugin that integrates tools to procedurally generate power lines.</b>
</p>

## Introduction and motivations

The idea of this plugin is offering a suite of useful tools to easily build the power grid of your games or environments.

In my journey learning 3D environment art I discovered that a huge amount of environments needed a power grid and it was a VERY tedious job to set up with a more traditional approach.

I also found [Any Austin](https://www.youtube.com/@any_austin)'s videos trying to make sense of popular video games' power grids and wanted to make a tool that helped developers create more believable worlds in that sense.

[Do Grand Theft Auto V’s Power Lines Connect To Anything?](https://www.youtube.com/watch?v=c3iqVTDXK1Y)

## Installation

The plugin can be adquired in [Fab](https://fab.com/s/df6499c5a50f) or cloned or downloaded for free from [GitHub](https://github.com/Bumvolla/EasyPowerLines).

If you got Git installed on your computer you can clone the repository by typing this command in a terminal in your engine "Plugins/Marketplace" folder or your project "Plugins" folder:


~~~
git clone https://github.com/Bumvolla/EasyPowerLines.git
~~~

or, if you already got your unreal project in a git repository:


~~~
git submodule add https://github.com/Bumvolla/EasyPowerLines.git
~~~



If not, you can download the .zip file in the [latest release](https://github.com/Bumvolla/EasyPowerLines/releases/latest) and extract it in your project "Plugins" folder
## Features

- Performant power lines procedural generation
- Acurate paralel catenary calculation
- Class preset based construction
- Randomization
- Terrain snaping

### Spline utility pole
Build your power line with one single spline, you can determine spacing, randomization and way more parameters.
Poles have terrain snapping and alignement.
<br/>
<br/>
<img width="1343" height="437" alt="SplineUtilityPole" src="https://github.com/user-attachments/assets/7e4ae103-1375-4e5f-8a6d-7a420f5ab94a" />

### Utility pole
Single pole that will connect to another single pole, you can determine wire slack, randomization and way more parameters.
Poles have terrain snapping and alignement.
<br/>
<br/>
<img width="1220" height="458" alt="UtilityPole" src="https://github.com/user-attachments/assets/1b99b755-c44a-401a-9bd7-bcc212bfcba5" />

### Point to point catenary
One single mathematically acurate catenary that goes from a world space point to another, you can determine wire slack.
<br/>
<br/>
<img width="1291" height="530" alt="PointToPointCatenary" src="https://github.com/user-attachments/assets/9baad684-d3a6-4445-a207-0b5ca11c1956" />

### Point to point catenary
Same as point to point catenary but with a sequence of points instead of 2, you can determine wire slack and randomization
<br/>
<br/>
<img width="1175" height="788" alt="MultiplePointCatenary" src="https://github.com/user-attachments/assets/3ab5a84a-c27c-44a4-85de-68f522952946" />

