---
title: Reverse-Engineered Pull-Up Bar Assembly
summary: I took apart a commercial pull-up bar, hand-measured all 16 parts, and rebuilt it as a fully mated SOLIDWORKS assembly with an exploded view, BOM and dimensioned drawings.
date: November 2025
status: Complete
tags: SOLIDWORKS, Reverse engineering, Technical drawings
order: 2
---
<script src="../js/model-viewer.min.js" defer></script>

<figure class="model wide light"><model-viewer src="../models/pullup-assembled.glb" alt="3D model of the pull-up bar assembly" camera-controls disable-zoom disable-pan touch-action="pan-y" interaction-prompt="none" auto-rotate auto-rotate-delay="0" rotation-per-second="12deg" camera-orbit="35deg 65deg 1.7m" field-of-view="25deg" shadow-intensity="0.6" exposure="1" environment-image="neutral"></model-viewer><figcaption>My SOLIDWORKS assembly. Click and drag to rotate it.</figcaption></figure>

## Overview

This was my SOLIDWORKS capstone, a course project in reverse engineering. I started with a store-bought doorway pull-up bar and ended with a complete digital twin: every part modeled to measured dimensions, assembled with proper mates, and documented in a full drawing package.

<div class="compare">
  <figure><img src="../images/pullup-photo-01.jpg" alt="The commercial doorway pull-up bar before disassembly" loading="lazy"><figcaption>The real pull-up bar</figcaption></figure>
  <figure class="drawing"><img src="../images/pullup-dwg-1.png" alt="SOLIDWORKS assembly drawing of the pull-up bar in four views" loading="lazy"><figcaption>My assembly drawing</figcaption></figure>
</div>

## My process

1. **Disassembly and measurement.** I took the bar completely apart, measured all 16 parts by hand, and drew dimensioned sketches of each one to work from.
2. **Part modeling.** I modeled every component as a dimensionally accurate SOLIDWORKS part, including the bent tubes, foam grips and fasteners. I used helical sweeps to model the threads on the screws, bolt and nuts.
3. **Assembly.** I assembled all 16 parts into a fully mated assembly, so the model fits together the same way the real bar does.
4. **Documentation.** I produced an exploded view, a numbered bill of materials tied to the individual part drawings, and a dimensioned drawing for each part.

## Exploded view and bill of materials

<figure class="drawing wide"><a href="../images/pullup-dwg-2.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-2.png" alt="Exploded view drawing of the pull-up bar with numbered balloons" loading="lazy"></a><figcaption>Exploded view. The numbered balloons match the bill of materials below.</figcaption></figure>

<table class="bom">
  <thead><tr><th>No.</th><th>Part</th><th>Qty</th></tr></thead>
  <tbody>
    <tr><td>1</td><td>Plastic bar</td><td>1</td></tr>
    <tr><td>2</td><td>Rectangular foam pad</td><td>1</td></tr>
    <tr><td>3</td><td>Curved bar</td><td>2</td></tr>
    <tr><td>4</td><td>Screw (small)</td><td>2</td></tr>
    <tr><td>5</td><td>Lock washer</td><td>4</td></tr>
    <tr><td>6</td><td>Acorn nut</td><td>4</td></tr>
    <tr><td>7</td><td>Curved bar foam</td><td>2</td></tr>
    <tr><td>8</td><td>Tube cap</td><td>6</td></tr>
    <tr><td>9</td><td>Foam grip #1</td><td>6</td></tr>
    <tr><td>10</td><td>Bar (inner)</td><td>1</td></tr>
    <tr><td>11</td><td>Screw</td><td>2</td></tr>
    <tr><td>12</td><td>Bar (outer)</td><td>1</td></tr>
    <tr><td>13</td><td>Bolt</td><td>1</td></tr>
    <tr><td>14</td><td>Foam grip #2</td><td>2</td></tr>
  </tbody>
</table>

## Part drawings

A selection of the dimensioned part drawings. Click any drawing to see it full size, or [open the complete drawing package (PDF)](../files/pullup-bar-drawings.pdf).

<div class="gallery drawings">
  <a href="../images/pullup-dwg-4.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-4.png" alt="Tube cap drawing" loading="lazy"><span>Tube cap</span></a>
  <a href="../images/pullup-dwg-6.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-6.png" alt="Acorn nut drawing with modeled threads" loading="lazy"><span>Acorn nut</span></a>
  <a href="../images/pullup-dwg-9.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-9.png" alt="Large screw drawing with helical thread" loading="lazy"><span>Large screw</span></a>
  <a href="../images/pullup-dwg-11.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-11.png" alt="Curved bar drawing" loading="lazy"><span>Curved bar</span></a>
  <a href="../images/pullup-dwg-17.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-17.png" alt="Bar 2 drawing with detail view" loading="lazy"><span>Outer bar</span></a>
  <a href="../images/pullup-dwg-18.png" target="_blank" rel="noopener"><img src="../images/pullup-dwg-18.png" alt="Bolt drawing" loading="lazy"><span>Bolt</span></a>
</div>

## Taking it apart

Photos from the disassembly, used as references while measuring and modeling.

<div class="gallery">
  <a href="../images/pullup-photo-02.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-02.jpg" alt="Pull-up bar, second angle" loading="lazy"></a>
  <a href="../images/pullup-photo-20.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-20.jpg" alt="Curved bar removed from the assembly" loading="lazy"></a>
  <a href="../images/pullup-photo-03.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-03.jpg" alt="Straight bar section" loading="lazy"></a>
  <a href="../images/pullup-photo-16.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-16.jpg" alt="Plastic bar" loading="lazy"></a>
  <a href="../images/pullup-photo-18.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-18.jpg" alt="Rectangular foam pad" loading="lazy"></a>
  <a href="../images/pullup-photo-21.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-21.jpg" alt="Curved bar foam" loading="lazy"></a>
  <a href="../images/pullup-photo-04.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-04.jpg" alt="Foam grip" loading="lazy"></a>
  <a href="../images/pullup-photo-06.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-06.jpg" alt="Tube cap" loading="lazy"></a>
  <a href="../images/pullup-photo-10.jpg" target="_blank" rel="noopener"><img src="../images/pullup-photo-10.jpg" alt="Screws" loading="lazy"></a>
</div>

## Skills used

- SOLIDWORKS part and assembly modeling, including helical sweeps for threaded hardware
- Dimensioned drawings with section and detail views
- Exploded views and bills of materials
- Hand measurement and dimensioned sketching
