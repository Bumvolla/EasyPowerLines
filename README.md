<a href="https://www.unrealengine.com/">![Unreal Engine](https://img.shields.io/badge/Unreal-5.4%2B-dea309)</a>

<br/>
<p align="center">
  <a href="https://github.com/Bumvolla/UE_EasyPowerLines">
    <img src="Resources/Icon128.png" alt="Logo" width="80" height="80">
  </a>
<h3 align="center">Easy power lines</h3>

  <p align="center">
     Unreal plugin that integrates tools to procedurally generate power lines
    <br/>
    <br/>
    <a href="https://bumvolla.github.io/2025/02/20/EasyPowerLinesDocs/"><strong>Docs</strong></a>
  </p>

## Features

- Performant power lines procedural generation
- Acurate paralel catenary calculation
- Class preset based construction
- Randomization
- Terrain snaping

## Usage

- Install plugin by cloning the repository or downloading the latest release
- Create a BP using the <strong>Utility pole presset</strong> parent class
- Assign it's values:
  - Static Mesh
  - Target points
- Drag and drop the <strong>Spline utility pole</strong> actor to the scene and fill it's values:
  - Class presset
  - Wire Mesh
  - Spline settings
  - Snapping
  - Randomization
- Click the <strong>Generate</strong> button
