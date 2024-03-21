Ce projet a pour objectif d'implémenter des techniques de rendu d'images par lancers de rayons.
On réalise un moteur de lancer de rayons permettant de rendre des scènes composées d'objets géométriques simples tels que des sphères et des cubes.

Le calcul est parallélisé avec OpenMP.


Ce projet s'appuie sur les différentes ressources ci-dessous :
- https://research.project-10.de/openrt/#lectures
- https://raytracing.github.io/books/RayTracingInOneWeekend.html#thevec3class
- https://connect.ed-diamond.com/GNU-Linux-Magazine/glmf-122/decouverte-de-la-programmation-parallele-avec-openmp


Améliorations à faire :
- accélérer le calcul en implémentant des bounding boxes et en utilisant le GPU
- faire des sources lumineuses
- ajouter des textures et des objects plus complexes