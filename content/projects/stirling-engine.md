---
title: Stirling Engine Design and Machining
summary: A Stirling engine that I modeled in SOLIDWORKS, documented with drawings for manufacturing, and then machined and assembled in the shop. The finished engine ran at 600 RPM.
date: October 2025
status: Complete
tags: SOLIDWORKS, Drawings for manufacturing, Lathe, Mill, CNC
order: 3
---
<script src="../js/model-viewer.min.js" defer></script>

<figure class="model wide"><model-viewer src="../models/stirling-assembled.glb" alt="3D model of the assembled Stirling engine" camera-controls disable-zoom disable-pan touch-action="pan-y" interaction-prompt="none" auto-rotate auto-rotate-delay="0" rotation-per-second="12deg" camera-orbit="35deg 70deg auto" field-of-view="24deg" shadow-intensity="0.6" exposure="1" environment-image="neutral"></model-viewer><figcaption>The SOLIDWORKS assembly. Click and drag to rotate it.</figcaption></figure>

## Overview

This was a course project where we designed a Stirling engine in SOLIDWORKS and then built it ourselves in the machine shop. The purpose of the project was to give us experience with CAD, reading and composing SOLIDWORKS drawings, machining on the lathe and the mill, and a beginner's introduction to CNC, as well as generally working in a lab.

The machining and assembly part of the project was done together with my classmate, **Andrew Miley**.

## The result: 600 RPM

Once everything was machined and assembled, our engine ran at **600 RPM**.

<div class="compare square">
  <figure class="video"><video autoplay muted loop playsinline controls preload="metadata" poster="../images/stirling-running-poster.jpg"><source src="../files/stirling-running.mp4" type="video/mp4">Your browser can't play this video.</video><figcaption>The finished engine running</figcaption></figure>
  <figure><a href="../images/stirling-photo-assembled.jpg" target="_blank" rel="noopener"><img src="../images/stirling-photo-assembled.jpg" alt="The finished Stirling engine on a shop bench: aluminum base plate, cylinder plate and bearing block, brass flywheel and power cylinder" loading="lazy"></a><figcaption>The assembled engine in the shop</figcaption></figure>
</div>

## Designing for manufacturing

The biggest focus for me in this project was designing the parts so they could actually be made, and composing the drawings that we would machine from. A model can look right on the screen, but in the shop the drawing is what you go off of, so every dimension and tolerance needs to be there.

- I modeled each component of the engine in SOLIDWORKS, dimensionally accurate to the engineering specification and fit.
- I put all of the components together into the full engine assembly, which let me check the fit and alignment before anything was machined.
- I composed the drawing package: an assembled view with a section view, an exploded view with numbered balloons, and a bill of materials covering all 39 line items. The drawings are in inches, with tolerances of ±0.1, ±0.01 and ±0.001 depending on the number of decimal places.

<div class="gallery drawings">
  <a href="../images/stirling-dwg-1.png" target="_blank" rel="noopener"><img src="../images/stirling-dwg-1.png" alt="Assembled view drawing of the Stirling engine with section C-C" loading="lazy"><span>Assembled view and section</span></a>
  <a href="../images/stirling-dwg-2.png" target="_blank" rel="noopener"><img src="../images/stirling-dwg-2.png" alt="Exploded view drawing of the Stirling engine with numbered balloons" loading="lazy"><span>Exploded view</span></a>
  <a href="../images/stirling-dwg-3.png" target="_blank" rel="noopener"><img src="../images/stirling-dwg-3.png" alt="Bill of materials for the Stirling engine, 39 items" loading="lazy"><span>Bill of materials</span></a>
</div>

Click a drawing to see it full size, or [open the drawing package (PDF)](../files/stirling-engine-drawings.pdf).

## Exploded view

<figure class="drawing wide"><a href="../images/stirling-dwg-2.png" target="_blank" rel="noopener"><img src="../images/stirling-dwg-2.png" alt="Exploded view drawing of the Stirling engine" loading="lazy"></a><figcaption>Exploded view of the engine assembly</figcaption></figure>

## In the machine shop

Andrew and I machined our parts on the lathe and the mill, and we also got some beginner experience on the CNC. The parts we made were:

- the **base plate**
- the **bearing block**
- the **cylinder plate**, the middle component between the two pistons
- the brass **valve** on the side of the engine

The screws and bolts were not made by us. After machining, we assembled the engine ourselves.

<figure class="video"><video muted loop playsinline controls preload="metadata" poster="../images/stirling-hand-test-poster.jpg"><source src="../files/stirling-hand-test.mp4" type="video/mp4">Your browser can't play this video.</video><figcaption>Turning the flywheel by hand after assembly to check that everything moves freely.</figcaption></figure>

## What I took from it

This project helped me build skills such as machining on the lathe and the mill, and it gave me my first experience with CNC. It also reinforced what I had learned about CAD and drawings in my coursework. Seeing parts made from drawings I composed showed me how important it is to think about manufacturing while designing, not after.

<!--
PRIVATE NOTES (not published on the website):
- Exploded 3D model: the AssemExpl.3MF export came out in the assembled position (SOLIDWORKS saved it collapsed).
  The "Exploded view" section shows the exploded drawing for now. Swap in a second 3D viewer
  (static/models/stirling-exploded.glb) once a working exploded export is available.
- RPM confirmed as 600, matching the resume.
-->
