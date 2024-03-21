Ce projet a pour objectif d'implémenter des techniques de rendu d'images par lancers de rayons.
On réalise un moteur de lancer de rayons permettant de rendre des scènes composées d'objets géométriques simples tels que des sphères et des cubes.
Le polymorphisme est utilisé pour créer une classe abstraite d'objets qui peuvent être touchés par un rayon, qu'on dérive pour créer les sphères, cubes, etc...

Le calcul est parallélisé avec OpenMP.

La compilation se fait avec : g++ -Wall .\main.cpp -o a.exe -fopenmp
en précisant le nombre de threads nb_threads dans main.cpp
On effectue le rendu avec : .\a.exe > image.ppm
Il faut ensuite utiliser un lecteur de ppm, du genre : https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

Ce projet s'appuie sur les différentes ressources ci-dessous :
- https://research.project-10.de/openrt/#lectures
- https://raytracing.github.io/books/RayTracingInOneWeekend.html#thevec3class
- https://connect.ed-diamond.com/GNU-Linux-Magazine/glmf-122/decouverte-de-la-programmation-parallele-avec-openmp

Améliorations à faire :
- accélérer le calcul en implémentant des bounding boxes et en utilisant le GPU
- faire des sources lumineuses
- ajouter des textures et des objets plus complexes
- proposer différents formats d'images